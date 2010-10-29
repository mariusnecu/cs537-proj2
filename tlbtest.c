#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tlbtest.h"
#include "vmsim.h"

int vpnSize;
int timer = 0;
int vpnCount = 0;

void tlbtest(int tlbSize, int pageSize) 
{
    // Calculate the number of bits in a VPN	
    int offset = (int)log2((double)pageSize);
    vpnSize = 32 - offset;

	int rollingMiss = 0;
	struct TLB* tlb = createTLB(tlbSize);
	mem_loc *mem_ptr = mem_head;
	
	while (mem_ptr != NULL)
	{	
        int vpn = mem_ptr->address >> offset;
		insertIntoTLB(tlb, vpn, &rollingMiss);
		mem_ptr = mem_ptr->next;
	}
	// Count number of entries in evicted and in tlb.
	printf("TLB misses: %d\n", rollingMiss);
	
	printf("VPNs: %d\n", vpnCount);	
}

struct TLB* createTLB(int size)
{
	struct TLB* ptr = (struct TLB*)malloc(sizeof(struct TLB));
	ptr->eList = NULL;
	ptr->arr = (struct VPN**)malloc(size * sizeof(struct VPN *));
	ptr->size = size;
	
	return ptr;
}

struct VPN* createVPN(int vpn)
{
	struct VPN* vpn_obj = (struct VPN*)malloc(sizeof(struct VPN));
	vpn_obj->number = vpn;

    // VPN must be used since it's being created, so increment timer.
	vpn_obj->lastUsed = timer;
    timer++;

    // Increment the count of VPNs
	vpnCount++;
	return vpn_obj;
}

// This function will do all of the heavy lifting for the TLB.
// Specify which tlb the vpn should be inserted into, as well as
// a counter (which should be called by reference) to keep track
// of total misses in the tlb thus far.
void insertIntoTLB(struct TLB* tlb, int vpn, int *rollingMiss)
{
	int j;
	for (j = 0; j<tlb->size; j++)
	{   
        if (tlb->arr[j] != NULL) 
        {
            if (tlb->arr[j]->number == vpn)
            {
                // Addresses is in the TLB so update its last used timestamp.
                tlb->arr[j]->lastUsed = timer;
                timer++;
                return;
            }
        }
	}
        
    // If we got this far, record miss in the TLB.
    *rollingMiss = *rollingMiss + 1;

    // Grab the VPN off the evict list or create a new one.
    struct VPN *vpn_obj = removeFromEvictList(tlb->eList, vpn);
    if (vpn_obj == NULL) {
        vpn_obj = createVPN(vpn);
    } else {
        vpn_obj->lastUsed = timer;
        timer++;
    }

    // We must insert it now into the table.
    for (j = 0; j<tlb->size; j++)
    {
        if (tlb->arr[j] == NULL)
        {
            // Unused slot, insert the VPN.
            tlb->arr[j] = vpn_obj;
            return;
        }
    }
        
    // Determine page to evict
    int lowestSeen = tlb->arr[0]->lastUsed;
    int index = 0;
    for (j = 0; j<tlb->size; j++)
    {
        if (tlb->arr[j]->lastUsed < lowestSeen)
        {
            index = j;
            lowestSeen = tlb->arr[j]->lastUsed;
        }
    }
    // Add evicted VPN to evicted list here.
    addToEvictList(tlb->eList, tlb->arr[index]);

    // Insert VPN into TLB.
    tlb->arr[index] = vpn_obj;

}

struct VPN* removeFromEvictList(struct evictList* eList, int vpn)
{
    struct evictList *node = eList;
    while (node != NULL) {
        if (node->arr->number == vpn) {
            return node->arr;
        }
        node = node->next;
    }
    return NULL;
}

void addToEvictList(struct evictList* eList, struct VPN* vpn_obj)
{
    struct evictList *node = eList;
    while (node != NULL) {
        if (node->next == NULL) {
            struct evictList *el = malloc(sizeof(struct evictList));
            el->arr = vpn_obj;
            node->next = el;
            return;
        }
        node = node->next;
    }
    
    eList = malloc(sizeof(struct evictList));
}

