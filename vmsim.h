struct TLB
{
	int size;
	int pageSize;
	struct VPN* arr[];
};

struct VPN
{
	unsigned long number;
	int time;
};

struct TLB* createTLB(int size, int pageSize);
void insert(struct TLB* tlb, struct VPN* vpn, int rollingMiss);

struct TLB* createTLB(int size, int pageSize)
{
	struct TLB* ptr = (struct TLB*)malloc(sizeof(struct TLB));
	ptr->size = size;
	ptr->pageSize = pageSize;
	
	int i;
	for (i = 0; i<size; i++)
	{
		// Null out the TLB initially to check fullness.
		ptr->arr[i] = NULL;
	}
}

// This function will do all of the heavy lifting for the TLB.
// Specify which tlb the vpn should be inserted into, as well as
// a counter (which should be called by reference) to keep track
// of total misses in the tlb thus far.
void insert(struct TLB* tlb, struct VPN* vpn, int rollingMiss)
{
	
}
