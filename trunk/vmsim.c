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

unsigned long parseNext(FILE* fp);

int main(int argc, char *argv[]) {
    // Open the file.
	// vimsim tlbtest tracefile pagesize tblsize
	int store, i;
	char* end;
	char arr[33];
	unsigned long long trace_address;
	unsigned long address;
	
	FILE *fp = fopen(argv[2], "r");

	while ((store = fgetc(fp)) != EOF)
	{
		if (store == '\n')
		{
			arr[i] = store;
			// Convert it, add it.
			trace_address = strtoq(arr, &end, 16);
			address = (unsigned long) trace_address & 0xffffffff;
			printf("Address added: %lx\n", address);
			i = 0;
		}
		else {
			arr[i] = store;
			i++;
		}
	}
    return 0;
}
