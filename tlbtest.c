#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tlbtest.h"
#include "vmsim.h"

void tlbtest(int tlbSize, int pageSize) 
{	
	int rollingMiss = 0;
	struct VPN* vpn;
	struct TLB* tlb = createTLB(tlbSize, pageSize);
	mem_loc* mem_ptr = mem_head;
	
	while (mem_ptr != NULL)
	{	
		// VPN to be inserted.
		vpn = createVPN();	
		vpn->number = mem_ptr->address;
		insertIntoTLB(tlb, vpn, &rollingMiss);
		mem_ptr = mem_ptr->next;
	}
	// Count number of entries in evicted and in tlb.
	printf("TLB misses: %d\n", rollingMiss);
	
	int totalUnique = countUnique(tlb);
	printf("VPNs: %d\n", totalUnique);	
}

struct TLB* createTLB(int size, int pageSize)
{
	int i;
	struct TLB* ptr = (struct TLB*)malloc(sizeof(struct TLB));
	ptr->eList = createEvictList(size);
	ptr->arr = (struct VPN**)malloc(size * sizeof(struct VPN));
	
	for (i = 0; i<size; i++)
	{
		ptr->arr[i] = createVPN();
	}
	ptr->size = size;
	ptr->pageSize = pageSize;
	
	return ptr;
}

struct evictList* createEvictList(int initSize)
{
	struct evictList* eList = (struct evictList*)malloc(sizeof(struct evictList));
	eList->arr = (struct VPN**)malloc(initSize * sizeof(struct VPN));
	eList->size = initSize;
	
	int j;
	for (j = 0; j<initSize; j++)
	{
		eList->arr[j] = createVPN();
	}
	
	return eList;
}

struct VPN* createVPN()
{
	struct VPN* vpn = (struct VPN*)malloc(sizeof(struct VPN));
	vpn->number = 0;
	vpn->hits = 0;
	
	return vpn;
}

// This function will do all of the heavy lifting for the TLB.
// Specify which tlb the vpn should be inserted into, as well as
// a counter (which should be called by reference) to keep track
// of total misses in the tlb thus far.
void insertIntoTLB(struct TLB* tlb, struct VPN* vpn, int *rollingMiss)
{
	int j;
	int checkHit = 0;
	int slotFound = 0;
	
	// virtNum is the number of bits to be masked with vpn->num.
	unsigned long offset = logl(tlb->pageSize)/logl(2);
	unsigned long vpnSize = 32-offset;
	vpn->number = vpn->number >> vpnSize;
	
	for (j = 0; j<tlb->size; j++)
	{
		if (tlb->arr[j]->number != 0)
		{
			if (tlb->arr[j]->number == vpn->number)
			{
				// Addresses is in the TLB.
				(tlb->arr[j]->hits)++;
				checkHit = 1;
				break;
			}
		}
	}
	if (checkHit == 0)
	{
		// We must insert it now into the table.
		for (j = 0; j<tlb->size; j++)
		{
			if (tlb->arr[j]->number == 0)
			{
				// Unused slot, insert the VPN.
				tlb->arr[j]->number = vpn->number;
				slotFound = 1;
				break;
			}
		}
		if (slotFound == 0)
		{
			if (inEvictList(tlb->eList, vpn) == 0)
			{
				// Page replacement (TLB full); VPN not in evict list.
				int lowestSeen = tlb->arr[0]->number;
				int index = 0;
				for (j = 0; j<tlb->size; j++)
				{
					if (tlb->arr[j]->number < lowestSeen)
					{
						index = j;
						lowestSeen = tlb->arr[j]->number;
					}
				}
			
				// Add evicted VPN to evicted list here.
				tlb->eList = add(tlb->eList, tlb->arr[index]);
				// ---------------------------------
			
				tlb->arr[index]->number = vpn->number;
				tlb->arr[index]->hits = 0;
			}
			else {
				// The VPN was seen already in the past (in evict list)
				int lowestSeen = tlb->arr[0]->number;
				int index = 0;
				for (j = 0; j<tlb->size; j++)
				{
					if (tlb->arr[j]->hits < lowestSeen)
					{
						index = j;
						lowestSeen = tlb->arr[j]->number;
					}
				}
			
				// Remove from the evicted list and add it back to the TLB.
				removeFromEvictList(tlb->eList, vpn);
				
				// Add evicted to the Evict List now.
				add(tlb->eList, tlb->arr[index]);
				// ---------------------------------
			
				tlb->arr[index]->number = vpn->number;
				tlb->arr[index]->hits = 0;
			}
		}
		if (checkHit == 0)
		{
			// Miss in the TLB.
			*rollingMiss = *rollingMiss + 1;
		}
	}
}

int inEvictList(struct evictList* eList, struct VPN* vpn)
{
	int j;
	for (j = 0; j<eList->size; j++)
	{
		if (eList->arr[j]->number == vpn->number)
		{
			return 1;
		}
	}
	return 0;
}

void removeFromEvictList(struct evictList* eList, struct VPN* vpn)
{
	int j;
	for (j = 0; j<eList->size; j++)
	{
		if(eList->arr[j]->number == vpn->number)
		{
			// Remove it.
			eList->arr[j]->number = 0;
			return;
		}
	}
}

struct evictList* add(struct evictList* eList, struct VPN* vpn)
{
	int j;
	int copyFound = 0;
	int insertSuccessful = 0;
	
	for (j = 0; j<eList->size; j++)
	{
		// First scan list for a copy of the vpn.
		if (eList->arr[j]->number == vpn->number)
		{
			// Already present, do nothing.
			copyFound = 1;
			break;
		}
	}
	if (copyFound == 0)
	{
		// Find an open space to insert the VPN.
		for (j = 0; j<eList->size; j++)
		{
			if (eList->arr[j]->number == 0)
			{
				// Open slot found, insert VPN.
				eList->arr[j]->number = vpn->number;
				insertSuccessful = 1;
				break;
			}
		}
		
		if (insertSuccessful == 0)
		{
			// The list is full; resize, copy and insert.
			struct evictList* cpy = createEvictList((eList->size)*10);
			
			for (j = 0; j<eList->size; j++)
			{
				cpy->arr[j]->number = eList->arr[j]->number;
				free(eList->arr[j]);
			}
			
			// Finally insert the VPN into the newly resized evictList.
			cpy->arr[eList->size+1]->number = vpn->number;
			free(eList);
			return cpy;
		}
	}
	return eList;
}

int countUnique(struct TLB* tlb)
{
	int j;
	int count = 0;
	
	// Count in TLB first.
	for (j = 0; j<tlb->size; j++)
	{
		if (tlb->arr[j]->number != 0)
		{
			count++;
		}
	}
	
	// Count evictList.
	for (j = 0; j<tlb->eList->size; j++)
	{
		if (tlb->eList->arr[j]->number != 0)
		{
			count++;
		}
	}
	
	return count;
}
