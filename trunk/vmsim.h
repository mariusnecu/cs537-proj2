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

typedef struct _mem_loc {
  struct _mem_loc *next;
  unsigned int address;
} mem_loc;

mem_loc *mem_head;

void printUsage();
