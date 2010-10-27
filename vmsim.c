/* vmsim.c
 *
 * Primary source file for the Virtual Memory Simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include "tlbtest.h"
#include "vmsim.h"
#include "wsstest.h"
#include "pttest.h"
#include "prtest.h"
#include <string.h>

int main(int argc, char *argv[]) {

    // Open the file.
	int store, i, j;
	int rollingMiss = 0;
	int pageSize, tlbSize;
	char* end;
	char* line = (char *)malloc(33 * sizeof(char));
	unsigned long long instAddr;
	unsigned long long dataAddr;
	char* rw = (char *)malloc(sizeof(char));
	struct VPN* vpn;
	unsigned long address;
	
	char* testChoice = argv[1];
	FILE *fp = fopen(argv[2], "r");
	pageSize = atoi(argv[3]);
	tlbSize = atoi(argv[4]);
	
	// Create the tlb.
	struct TLB* tlb = createTLB(tlbSize, pageSize);
	
	while (strcmp(fgets(line, 33, fp), "#eof") != 0)
	{
		sscanf(line, "0x%llx: %s 0x%llx", &instAddr, rw, &dataAddr);
			
		// VPN to be inserted.
		vpn = createVPN();
			
		// Convert to 32bit.
		address = (int) instAddr & 0xffffffff;
		vpn->number = address;
		insertIntoTLB(tlb, vpn, &rollingMiss);
		printf("Address added: %lx\n", address);
			
		address = (int) dataAddr & 0xffffffff;
		vpn->number = address;
		insertIntoTLB(tlb, vpn, &rollingMiss);
		printf("Address added: %lx\n", address);
			
		printf("Read/Write: %s\n", rw);
	}

	// Count number of entries in evicted and in tlb.
	printf("\nTLB misses: %d\n", rollingMiss);
	
	int totalUnique = countUnique(tlb);
	printf("VPNs: %d\n", totalUnique);
	
    return 0;
}
