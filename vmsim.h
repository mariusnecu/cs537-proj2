#import <math.h>

struct TLB
{
	int size;
	int pageSize;
	struct VPN* arr[];
};

struct VPN
{
	unsigned long number;
	int hits;
};

struct TLB* createTLB(int size, int pageSize);
void insert(struct TLB* tlb, struct VPN* vpn, int *rollingMiss);

struct TLB* createTLB(int size, int pageSize)
{
	struct TLB* ptr = (struct TLB*)malloc(sizeof(struct TLB));
	ptr->size = size;
	ptr->pageSize = pageSize;
	
	int i;
	for (i = 0; i<size; i++)
	{
		struct VPN* vpn = (struct VPN*)malloc(sizeof(struct VPN));
		vpn->number = 0;
		vpn->hits = 0;
		ptr->arr[i] = vpn;
	}
	
	return ptr;
}

// This function will do all of the heavy lifting for the TLB.
// Specify which tlb the vpn should be inserted into, as well as
// a counter (which should be called by reference) to keep track
// of total misses in the tlb thus far.
void insert(struct TLB* tlb, struct VPN* vpn, int *rollingMiss)
{
	int j;
	int checkHit, slotFound;
	
	// virtNum is the number of bits to be masked with vpn->num.
/*	unsigned long virtNum = logl(2^(tlb->pageSize))/logl(2);
	unsigned long mask = vpn->number & virtNum;*/
	
	for (j = 0; j<tlb->size; j++)
	{
		if (tlb->arr[j] != 0)
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
				free(tlb->arr[j]);
				tlb->arr[j] = vpn;
				slotFound = 1;
				break;
			}
		}
		if (slotFound == 0)
		{
			// Page replacement algorithm here.
			
		}
		*rollingMiss = *rollingMiss + 1;
	}
}
