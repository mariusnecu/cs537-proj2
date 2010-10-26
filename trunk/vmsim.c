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
	char arr[33];
	char first[20];
	char second[20];
	char* rw;
	struct VPN* vpn;
	unsigned long long trace_address;
	unsigned long address;
	
	char* testChoice = argv[1];
	FILE *fp = fopen(argv[2], "r");
	pageSize = atoi(argv[3]);
	tlbSize = atoi(argv[4]);
	
	// Create the tlb.
	struct TLB* tlb = createTLB(tlbSize, pageSize);
	
	while ((store = fgetc(fp)) != EOF)
	{
		// Check for eof
		if (store == 'o')
		{
			break;
		}
		if (store == '\n')
		{
			arr[i] = store;
			
			j = 0;
			// Pad unneeded with spaces to easily tokenize.
			while (arr[j] != '\n')
			{
				if (arr[j] == ':')
				{
					arr[j] = 0x20;
				}
				j++;
			}
			
			char* cpy = (char *)malloc(sizeof(arr));
			strcpy(cpy, arr);

			// First address.
			char* tok = strtok(cpy, "\t ");
			strcpy(first, tok);
			
			// R or W
			tok = strtok(NULL, "\t ");
			if (tok != NULL)
			{
				rw = (char *)malloc(sizeof(tok));
				strcpy(rw, tok);
			} 
			else break;
			
			// Second address.
			tok = strtok(NULL, "\t ");
			if (tok != NULL)
			{
				strcpy(second, tok);
			}
			else break;
			
			// VPN to be inserted.
			vpn = createVPN();
			
			// Convert to 32bit.
			trace_address = strtoq(first, &end, 16);
			address = (unsigned long) trace_address & 0xffffffff;
			vpn->number = address;
			insert(tlb, vpn, &rollingMiss);
			printf("Address added: %lx\n", address);
			
			trace_address = strtoq(second, &end, 16);
			address = (unsigned long) trace_address & 0xffffffff;
			vpn->number = address;
			insert(tlb, vpn, &rollingMiss);
			printf("Address added: %lx\n", address);
			
			printf("Read/Write: %s\n", rw);
			
			// Null out character arrays.
			int k = 0;
			while (k < 20)
			{
				first[k] = '\0';
				second[k] = '\0';
				k++;
			}
			
			int y = 0;
			while (y < 33)
			{
				arr[y] = '\0';
				y++;
			}
			
			free(rw);
			free(cpy);
			i = 0;
		}
		else {
			arr[i] = store;
			i++;
		}
	}

	// Count number of entries in evicted and in tlb.
	printf("\nTLB misses: %d\n", rollingMiss);
	
	int totalUnique = countUnique(tlb);
	printf("VPNs: %d\n", totalUnique);
	
    return 0;
}
