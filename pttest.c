/* pttest.c
 *
 * Implements the multi-level page table simulation.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pttest.h"

// Example trace for testing.
int trace[] = {
    0xcca00fbd,
    0xccc1bba0,
    0xcca00fbc,
    0xccc1bfd8,
    0xcca00fce,
    0xccc1bd68,
    0xcca00fd5,
    0xccc1c4e8
};

// Track the next frame to be allocated
int currentFrame = 0;

// Values to be calculated based on the page size
int entriesPerTable;
int levels;
int bitsPerLevel;
int rootBits;

// Track the number of tables used.
int tablesInUse = 0;
/*
int main(int argc, char *argv[]) {
    pttest(4096);
    exit(0);
}
*/
void pttest(int pagesize) {
    // Calculate number of entries per page, given that each entry takes up
    // 2^6 bits of space
    entriesPerTable = pagesize >> 3;

    // Sanity check for size of page
    if (entriesPerTable <= 0) {
        char *err_msg = "Invalid page size (smaller than page table entry).";
        fprintf(stderr, "%s", err_msg);
        exit(1);
    }

    // Determine the number of page table levels needed and the number of bits
    // translated by each level
    double offset = ceil(log2((double)pagesize));
    double b = floor(log2((double)entriesPerTable));
    levels = (int)ceil((32.0 - offset) / b);
    bitsPerLevel = (int)b;
    rootBits = 32 - offset - ((levels - 1) * bitsPerLevel);
    
    // Create the root page table
    page_table *root = initializePageTable();

    // Process the trace in order, allocating pages as needed
    int i;
    for (i=0; i<sizeof(trace)/sizeof(int); i++) {
        unsigned int location = trace[i];

        page_table *tbl = root;

        // Traverse the multiple levels until the lowest level table is found.
        int j;
        for (j=0; j<levels-1; j++) {
            int tbl_vpn;
            if (j == 0) {
                tbl_vpn = location >> (32 - rootBits);
            } else {
                tbl_vpn = location << (rootBits + (j - 1) * bitsPerLevel) >> (32 - bitsPerLevel);
            }
            //entry tbl_entries[entriesPerTable] = tbl->entries;
            entry *e = tbl->entries[tbl_vpn];
            if (e == NULL) {
                e = malloc(sizeof(entry));
                page_table *new_tbl = initializePageTable();
                e->to_pt = new_tbl;
                e->valid = 1;
                tbl->entries[tbl_vpn] = e;
                tbl->used_count++;
            }
            tbl = e->to_pt;
        }
        
        // Now tbl must be the lowest level table, so index with the frame 
        // number.
        int vpn = location << (rootBits + (levels - 1) * bitsPerLevel) >> (32 - bitsPerLevel);
        entry *en = tbl->entries[vpn];
        if (en == NULL) {
            en = malloc(sizeof(entry));
            en->to_frame = currentFrame;
            en->valid = 1;
            tbl->entries[vpn] = en;
            tbl->used_count++;
            currentFrame++;
        }        
    }

    page_table *table = root;


    // Produce output
    printf("PT: %d\n", tablesInUse);
}

// Create a new page table and a new empty array of entries.
page_table *initializePageTable() {
    page_table *pt = (page_table *)malloc(sizeof(page_table));
    entry **entries = malloc(sizeof(entry*) * entriesPerTable);
    pt->entries = entries;
    tablesInUse++;
    return pt;
}

