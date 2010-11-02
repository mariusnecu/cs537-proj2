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
#include "vmsim.h"

// Track the next frame to be allocated
int currentFrame = 0;

// Values to be calculated based on the page size
int entriesPerTable;
int levels;
int bitsPerLevel;
int rootBits;

// Track the number of tables used and their order.
int tablesInUse = 0;
frame_list *inUseList;
frame_list *inUseTail;

void pttest(int pagesize) {
    // Calculate number of entries per page, given that each entry takes up
    // 2^3 bytes of space
    entriesPerTable = pagesize >> 3;

    // Sanity check for size of page
    if (entriesPerTable <= 0) {
        char *err_msg = "Invalid page size (smaller than page table entry).";
        fprintf(stderr, "%s", err_msg);
        exit(1);
    }

    // Determine the number of page table levels needed and the number of bits
    // translated by each level
    int offset = (int)ceil(log2((double)pagesize));
    double b = floor(log2((double)entriesPerTable));
    levels = (int)ceil((32.0 - offset) / b);
    bitsPerLevel = (int)b;
    rootBits = 32 - offset - ((levels - 1) * bitsPerLevel);

    // Create the root page table
    page_table *root = initializePageTable();

    // Process the trace in order, allocating pages as needed
    mem_loc *loc = mem_head;
    while (loc != NULL) {
        unsigned int location = loc->address;

        page_table *tbl = root;

        // Traverse the multiple levels until the lowest level table is found.
        int j;
        for (j=0; j<levels-1; j++) {
            unsigned int tbl_idx;
            if (j == 0) {
                tbl_idx = location >> (32 - rootBits);
            } else {
                tbl_idx = location << (rootBits + (j - 1) * bitsPerLevel) >> (32 - bitsPerLevel);
            }
            
            // If the entry indexed by tbl_vpn is NULL, we need to create a new
            // page table to point to and create the entry in the current table.
            entry *e = tbl->entries[tbl_idx];
            if (e == NULL) {
                e = malloc(sizeof(entry));
                page_table *new_tbl = initializePageTable();
                e->to_pt = new_tbl;
                tbl->entries[tbl_idx] = e;
            }
            tbl = e->to_pt;
        }
        
        // Now tbl must be the lowest level table, so map to frame number.
        unsigned int vpn = location >> offset;
        unsigned int idx = vpn << (32 - bitsPerLevel) >> (32 - bitsPerLevel);
        entry *en = tbl->entries[idx];
        if (en == NULL) {
            en = malloc(sizeof(entry));
            en->to_frame = currentFrame;
            en->vpn = vpn;
            tbl->entries[idx] = en;
            currentFrame++;

            // Track the order of virtual pages to frame mappings.
            frame_list *fr_node = malloc(sizeof(frame_list));
            fr_node->ent = en;
            if (inUseList == NULL) {
                inUseList = inUseTail = fr_node;
            } else {
                // Insert the node into the sorted list.
                frame_list *inUseIter = inUseList;
                frame_list *inUsePrev = NULL;
                while (1) {
                    if (inUseIter != NULL && vpn > inUseIter->ent->vpn) {
                        inUsePrev = inUseIter;
                        inUseIter = inUseIter->next;
                        continue;
                    }
                    if (inUsePrev == NULL) {
                        inUseList = fr_node;
                    } else {
                        inUsePrev->next = fr_node;
                    }                  
                    fr_node->next = inUseIter;
                    break;
                }
            }
        }
        loc = loc->next;     
    }

    // Produce output
    printf("PT: %d\n", tablesInUse);
    frame_list *node = inUseList;
    while (node != NULL) {
        printf("VPN %d: PFN %d\n", node->ent->vpn, node->ent->to_frame);
        node = node->next;
    }
}

// Create a new page table and a new empty array of entries.
page_table *initializePageTable() {
    page_table *pt = (page_table *)malloc(sizeof(page_table));
    entry **entries = malloc(sizeof(entry*) * entriesPerTable);
    pt->entries = entries;
    
    // Use one physical frame to store the new page table.
    pt->frame_number = currentFrame;
    currentFrame++;

    // Keep track of how many page tables have been created.
    tablesInUse++;

    return pt;
}

