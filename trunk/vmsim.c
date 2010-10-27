/* vmsim.c
 *
 * Primary source file for the Virtual Memory Simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include "vmsim.h"
#include "wsstest.h"
#include "pttest.h"
#include "prtest.h"
#include <string.h>

int main(int argc, char *argv[]) {

	unsigned long long instAddr;
	unsigned long long dataAddr;
	struct linkedAddr* llAddr = createLinkedAddr();
	
	// Use 'head' to scan the linked list for every line in the pin trace.
	// Implementation for linkedAddr is in vmsim.h
	// struct linkedAddr* head = llAddr;
	
	char* rw = (char *)malloc(sizeof(char));
	char* line = (char *)malloc(33 * sizeof(char));
	
	char* testChoice = argv[1];
	FILE *fp = fopen(argv[2], "r");
	int pageSize = atoi(argv[3]);
	int tlbSize = atoi(argv[4]);
	
	// Parse and store each line.
	while (strcmp(fgets(line, 33, fp), "#eof") != 0)
	{
		sscanf(line, "0x%llx: %s 0x%llx", &instAddr, rw, &dataAddr);
		
		// Populate global structure for tests to use
		// (all addresses found so far)
		struct addr* address = createAddr(instAddr, rw, dataAddr);
		addToEnd(llAddr, address);
		//-------------------------------------------
	}
	
	if (strcmp(testChoice, "tlbtest") == 0)
	{
		// fp needs to be reset before being used again.
		fp = fopen(argv[2], "r");
		tlbtest(fp, tlbSize, pageSize);
	}
	
	if (strcmp(testChoice, "pttest") == 0)
	{
		
	}
	
	if (strcmp(testChoice, "wsstest") == 0)
	{
		
	}
	
    return 0;
}
