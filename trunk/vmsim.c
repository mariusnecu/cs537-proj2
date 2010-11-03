///////////////////////////////////////////////////////////////////////////////
//
// Project 2 - Virtual Memory
// Section 2
//
// Authors:
//  Troy Cornell
//  Richard Joiner
//  Zach Ovanin
//  Jeremy Weiss
//
// Files:
//  prtest.c, prtest.h
//  pttest.c, pttest.h
//  tlbtest.c, tlbtest.h
//  wsstest.c, wsstest.h
//  vmsim.c, vmsim.h
//
////////////////////////////////////////////////////////////////////////////////

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

  unsigned long long instAddr, dataAddr;
  unsigned int address1, address2;  

  if (argc < 4) {
    printUsage();
    exit(1);
  }
	
  char* rw = (char *)malloc(sizeof(char));
  char* line = (char *)malloc(33 * sizeof(char));

  char* testChoice = argv[1];
  FILE *fp = fopen(argv[2], "r");
  int pageSize = atoi(argv[3]);
	
  // Parse and store each line.
  mem_loc *prev = NULL;
  while (fgets(line, 33, fp))
    {
      if (strcmp(line, "#eof\n") == 0 || strcmp(line, "#eof") == 0 || strcmp(line, "\n") == 0) {
	break;
      }
		
      sscanf(line, "0x%llx: %s 0x%llx", &instAddr, rw, &dataAddr);

      // Populate global structure for tests to use
      // (all addresses found so far)
      // Convert to 32 bits
      address1 = (unsigned int) instAddr & 0xffffffff; 
      address2 = (unsigned int) dataAddr & 0xffffffff;
    
      // Populate one-dimensional linked list.
      mem_loc *instLoc = malloc(sizeof(mem_loc));
      instLoc->address = address1;
      mem_loc *dataLoc = malloc(sizeof(mem_loc));
      dataLoc->address = address2;
      instLoc->next = dataLoc;
      if (prev == NULL) {
	mem_head = instLoc;
      } else {
	prev->next = instLoc;
      }
      prev = dataLoc;
      //-------------------------------------------
    }
	
  if (strcmp(testChoice, "tlbtest") == 0)	{
    if (argc < 5) {
      printUsage();
      exit(1);
    }
    int tlbSize = atoi(argv[4]);
    tlbtest(tlbSize, pageSize);
  } else if (strcmp(testChoice, "pttest") == 0) {
    pttest(pageSize);		
  } else if (strcmp(testChoice, "wsstest") == 0) {
    if (argc < 5) {
      printUsage();
      exit(1);
    }
    int windowSize = atoi(argv[4]);
    wsstest(pageSize, windowSize);	
  } else if (strcmp(testChoice, "prtest-fifo") == 0) {
    if (argc < 5) {
      printUsage();
      exit(1);
    }
    int frames = atoi(argv[4]);
    prtest_fifo(pageSize, frames);
  } else if (strcmp(testChoice, "prtest-2fifo") == 0) {
    if (argc < 5) {
      printUsage();
      exit(1);
    }
    int frames = atoi(argv[4]);
    prtest_2nd_fifo(pageSize, frames);
  } else {
    printUsage();
    exit(1);
  }
	
  return 0;
}

void printUsage() {
  fprintf(stderr, "usage: vmsim test tracefile pagesize [extra]\n");
  fprintf(stderr, "  test: [tlbtest|wsstest|pttest|prtest-fifo|prtest-2fifo]\n");
  fprintf(stderr, "  tracefile: filename containing pin output\n");
  fprintf(stderr, "  pagesize: integer representing page size (in bytes)\n");
  fprintf(stderr, "  extra: integer representing tlb size (for tlbtest), window size (for wsstest)\n    or number of frames (for prtest variations)\n");
}
