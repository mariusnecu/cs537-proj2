///////////////////////////////////////////////////////////////////////////////
//
// Project 2 - Virtual Memory
// Section 2
//
// Authors:
//  Troy Cornell
//  Richard Joiner
//  Zach Ovanin
//  Jeremy Weiss
//
// Files:
//  prtest.c, prtest.h
//  pttest.c, pttest.h
//  tlbtest.c, tlbtest.h
//  wsstest.c, wsstest.h
//  vmsim.c, vmsim.h
//
////////////////////////////////////////////////////////////////////////////////

/* tlbtest.c
 *
 * Implements a Transfer-lookaside buffer to simulate
 * the first step of virtual memory address translation.
 *
 */

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
	
  // Cycle through all memory addresses in the
  // pin trace; insert into the TLB.
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

// Allocate the TLB
struct TLB* createTLB(int size)
{
  struct TLB* ptr = (struct TLB*)malloc(sizeof(struct TLB));
  ptr->eList = NULL;
  ptr->arr = (struct VPN**)malloc(size * sizeof(struct VPN *));
  ptr->size = size;
	
  return ptr;
}

// Create a Virtual Page Number with specified integer value.
struct VPN* createVPN(int vpn)
{
  struct VPN* vpn_obj = (struct VPN*)malloc(sizeof(struct VPN));
  vpn_obj->number = vpn;

  // VPN must be used since it's being created, so increment timer.
  vpn_obj->lastUsed = timer;
  timer++;

  vpnCount++;
  // Increment the count of VPNs
  return vpn_obj;
}

// Insert a virtual page number into a specified transfer-lookaside
// buffer with a miss counter to increment if the VPN isn't in the TLB.
// Implements LRU algorithm to decide on which VPN to evict from TLB.
void insertIntoTLB(struct TLB* tlb, int vpn, int *rollingMiss)
{
  // Check if VPN is currently in the TLB.
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
  tlb->eList = addToEvictList(tlb->eList, tlb->arr[index]);

  // Insert VPN into TLB.
  tlb->arr[index] = vpn_obj;

}

// Removes a VPN from passed evict list, returns the VPN that was
// successfully removed, or NULL if nothing was.
struct VPN* removeFromEvictList(struct evictList* eList, int vpn)
{
  struct evictList *node = eList;
  struct evictList *prev = NULL;
  while (node != NULL) {
    if (node->arr->number == vpn) {
      if (prev != NULL) {
	prev->next = node->next;
      } else {
	eList = node->next;
      }
      return node->arr;
    }
    node = node->next;
  }
  // No VPN match
  return NULL;
}

// Add to a specified eviction list.
struct evictList* addToEvictList(struct evictList* eList, struct VPN* vpn_obj)
{
  struct evictList *node = eList;
  while (node != NULL) {
    if (node->next == NULL) {
      struct evictList *el = malloc(sizeof(struct evictList));
      el->arr = vpn_obj;
      node->next = el;
      return eList;
    }
    node = node->next;
  }
 
  // eList was null; allocate, store the VPN and return.
  eList = malloc(sizeof(struct evictList));
  eList->arr = vpn_obj;
  return eList;
}
