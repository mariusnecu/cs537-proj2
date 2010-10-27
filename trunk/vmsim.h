struct addr
{
	char* rw;
	int instAddr;
	int dataAddr;
};

struct linkedAddr
{
	struct linkedAddr* next;
	struct addr* address;
};

struct addr* createAddr(int instAddr, char* rw, int dataAddr)
{
	struct addr* address = (struct addr*)malloc(sizeof(struct addr));
	address->rw = rw;
	address->instAddr = instAddr;
	address->dataAddr = dataAddr;
	
	return address;
}

struct linkedAddr* createLinkedAddr()
{
	struct linkedAddr* llAddr = (struct linkedAddr*)malloc(sizeof(struct linkedAddr));
	llAddr->next = NULL;
	llAddr->address = NULL;
	
	return llAddr;
}

void addToEnd(struct linkedAddr* llAddr, struct addr* address)
{
	struct linkedAddr* ptr = llAddr;
	
	while (ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	
	ptr->address = address;
	ptr = ptr->next;
}
