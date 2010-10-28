typedef struct _mem_loc {
    struct _mem_loc *next;
    unsigned int address;
} mem_loc;

mem_loc *mem_head;

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

struct addr* createAddr(int instAddr, char* rw, int dataAddr);
struct linkedAddr* createLinkedAddr();
void addToEnd(struct linkedAddr* llAddr, struct addr* address);
