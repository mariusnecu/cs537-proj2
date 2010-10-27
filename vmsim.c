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
	
	char* rw = (char *)malloc(sizeof(char));
	char* line = (char *)malloc(33 * sizeof(char));
	
	char* testChoice = argv[1];
	FILE *fp = fopen(argv[2], "r");
	int pageSize = atoi(argv[3]);
	int tlbSize = atoi(argv[4]);
	
	while (strcmp(fgets(line, 33, fp), "#eof") != 0)
	{
		sscanf(line, "0x%llx: %s 0x%llx", &instAddr, rw, &dataAddr);
		
		// Populate global structure for tests to use
		// (all addresses found so far)
		
		//-------------------------------------------
	}
	
	if (strcmp(testChoice, "tlbtest") == 0)
	{
		fp = fopen(argv[2], "r");
		tlbtest(fp, tlbSize, pageSize);
	}
	
    return 0;
}
