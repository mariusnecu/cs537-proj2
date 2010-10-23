/* vmsim.c
 *
 * Primary source file for the Virtual Memory Simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include "tlbtest.h"
#include "wsstest.h"
#include "pttest.h"
#include "prtest.h"
#include <string.h>

unsigned long parseNext(FILE* fp);

int main(int argc, char *argv[]) {
    // Open the file.
	// vimsim tlbtest tracefile pagesize tblsize
	int store, i, j;
	char* end;
	char arr[33];
	char first[20];
	char second[20];
	unsigned long long trace_address;
	unsigned long address;
	
	FILE *fp = fopen(argv[2], "r");

	while ((store = fgetc(fp)) != EOF)
	{
		if (store == '\n')
		{
			arr[i] = store;
			
			j = 0;
			// Pad unneeded with spaces to easily tokenize.
			while (arr[j] != '\n')
			{
				if (arr[j] == ':' || arr[j] == 'W' || arr[j] == 'R')
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
			
			// Second address.
			tok = strtok(NULL, "\t ");
			strcpy(second, tok);
			
			// Convert to 32bit.
			trace_address = strtoq(first, &end, 16);
			address = (unsigned long) trace_address & 0xffffffff;
			printf("Address added: %lx\n", address);
			
			trace_address = strtoq(second, &end, 16);
			address = (unsigned long) trace_address & 0xffffffff;
			printf("Address added: %lx\n", address);
			
			free(cpy);
			i = 0;
		}
		else {
			arr[i] = store;
			i++;
		}
	}
    return 0;
}
