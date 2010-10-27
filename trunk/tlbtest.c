#import "tlbtest.h"
#import <stdlib.h>
#import <stdio.h>
#import <string.h>

void tlbtest(FILE* fp, int tlbSize, int pageSize) 
{	
	int rollingMiss = 0;
	char* line = (char *)malloc(33 * sizeof(char));
	unsigned long long instAddr;
	unsigned long long dataAddr;
	char* rw = (char *)malloc(sizeof(char));
	struct VPN* vpn;
	unsigned long address;
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
}

void tlbtest(FILE* fp, int tlbSize, int pageSize);

