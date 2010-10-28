/* vmsim.c
 *
 * Primary source file for the Virtual Memory Simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmsim.h"
#include "wsstest.h"
#include "pttest.h"
#include "prtest.h"
#include "tlbtest.h"

int main(int argc, char *argv[]) {

	unsigned long long instAddr;
	unsigned long long dataAddr;
	unsigned long address1, address2;
	struct linkedAddr* llAddr = createLinkedAddr();    
	
	// Use 'head' to scan the linked list for every line in the pin trace.
	// Implementation for linkedAddr is in vmsim.h
	// struct linkedAddr* head = llAddr;
	
	char* rw = (char *)malloc(sizeof(char));
	char* line = (char *)malloc(33 * sizeof(char));
	
	char* testChoice = argv[1];
	FILE *fp = fopen(argv[2], "r");
	int pageSize = atoi(argv[3]);
	
	// Parse and store each line.
    mem_loc *prev = NULL;
	while (fgets(line, 33, fp))
	{
		sscanf(line, "0x%llx: %s 0x%llx", &instAddr, rw, &dataAddr);

		// Populate global structure for tests to use
		// (all addresses found so far)
		// Convert to 32 bits
		address1 = (int) instAddr & 0xffffffff; 
		address2 = (int) dataAddr & 0xffffffff;
		
        // Populate one-dimensional linked list.
        mem_loc *instLoc = malloc(sizeof(mem_loc));
        instLoc->address = instAddr;
        mem_loc *dataLoc = malloc(sizeof(mem_loc));
        dataLoc->address = dataAddr;
        instLoc->next = dataLoc;
        if (prev == NULL) {
            mem_head = instLoc;
        } else {
            prev->next = instLoc;
        }
        prev = dataLoc;


		
		//struct addr* address = createAddr(address1, rw, address2);
		//addToEnd(llAddr, address);
		//-------------------------------------------
	}
	
	if (strcmp(testChoice, "tlbtest") == 0)
	{
        if (argc < 4) {
            fprintf(stderr, "usage: vmsim tlbtest pintrace pagesize tlbsize");
            exit(1);
        }
    	int tlbSize = atoi(argv[4]);
		// fp needs to be reset before being used again.
		fp = fopen(argv[2], "r");
		tlbtest(fp, tlbSize, pageSize);
	}
	
	if (strcmp(testChoice, "pttest") == 0)
	{
        pttest(pageSize);		
	}
	
	if (strcmp(testChoice, "wsstest") == 0)
	{
		
	}
	
    return 0;
}

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
