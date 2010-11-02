/*
 * Main file for the working set size test, Part 2 of Programming Project 2
 * Virtual Memory  -
 * 
 * Simulates a working set of pages and calculates the average working set size
 * based on the parsed list of addresses obtained through part 0
 *
 */
 
 #include <stdio.h>
 #include <math.h>
 #include <stdlib.h>
// Contains global variable
 #include "vmsim.h"
 
 /*
  * Page structure, represents a unique virtual page for referencing purposes
  */
struct page {
	int refed;
};

 /*
  * The main wsstest method
  * Takes in three parameters, the page size, the window size, and the
  * collection of memory references
  */
int wsstest(int pagesize, int windowsize) {

	// Declare variables in 'for' loops
	int i;
	int j;

	// Assume we know the number of addresses in the trace file
	int numAddresses = 0;
	mem_loc *loc = mem_head;
	mem_loc *prev = mem_head;
	mem_loc **vs_array;
	
	// Allocate an array for the page numbers referenced
	while (loc != NULL) {
		numAddresses++;
		prev = loc;
		loc = loc->next;	
	}
	prev->next = mem_head;
	loc = mem_head;
	/*vs_array = (mem_loc **)malloc(sizeof(mem_loc *) * numAddresses);
	for (i = 0; i<numAddresses; i++) {
	  vs_array[i] = loc;
	  loc = loc->next;
	}
	loc = mem_head;*/

	// Determine how many pages are being referenced in the trace file
	unsigned int minAddress = loc->address;
	unsigned int maxAddress = loc->address;
	unsigned int currentAddress = 0;
	for (i = 0; i < numAddresses; i++) {
	  //loc = vs_array[i];
		currentAddress = loc->address;
		if (currentAddress > maxAddress) maxAddress = currentAddress;
		if (currentAddress < minAddress) minAddress = currentAddress;
		loc = loc->next;
	}
	loc = mem_head;

	int numPages = (maxAddress - minAddress)/(pagesize);
	
	// Create unique virtual pages
	struct page *x = (struct page*)malloc((sizeof(int))*numPages);
	currentAddress = minAddress;
	for (i = 0; i < numPages; i++) {
		x[i].refed = 0;
	}
	
	//printf("No fault\n");
	
	// Determine the pages referenced over a window
	int numRef = 0;
	int limit = 0;
	int sum = 0;
	int totalSize = 0;
	
	// Take care of the edge case
	if (windowsize > numAddresses) {
		limit = numAddresses;
	} else {
		limit = numAddresses - windowsize;
	}

	// Calculate 
	for (i = 0; i < limit; i++) {
	  loc = mem_head;
	  //loc = vs_array[i];
	  for (j = i; (j < windowsize + i) && (j < limit); j++) {
	    currentAddress = loc->address;
	    double arith = (currentAddress - minAddress)/pagesize;
	    //double arith2 = floor(arith);
	    int pageNum = arith;
	    if (x[pageNum].refed == 0) {
	      x[pageNum].refed = 1;
	      numRef++;
	    }
	    loc = loc->next;
	  }

	  //k -= windowsize;
	  //loc = vs_array[k];
	  loc = mem_head;
	  for (j = i; (j < windowsize + i) && (j < limit); j++) {
	    currentAddress = loc->address;
	    double arith = (currentAddress - minAddress)/pagesize;
	    //double arith2 = floor(arith);
	    int pageNum = arith;
	    x[pageNum].refed = 0;
	    loc = loc->next;
	  }
	  sum += numRef;
	  numRef = 0;
	  totalSize++;
	  mem_head = mem_head->next;
	  mem_head = mem_head->next;
	  i++;
	}
	//printf("No fault\n");
	
	/*int numRef = 1;
	  int sum = 0;
	  int totalSize = 0;
	
	// Find the average number of pages
	for (i = 0; i < numAddresses; i++) {
		for (j = i; j < windowsize + i; j++) {
			int a = array[(j)%numAddresses];
			int b = array[(j+1)%numAddresses];
			if (a != b) {
			numRef++;
			}
		}
		sum += numRef;
		totalSize++;
		numRef = 0;
		i++;
	}*/

	
	
	// Calculate the average working set size
	int averageset = sum/totalSize;
	
	printf("WS Size: %d\n", averageset);
	return 0;
}

