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

#ifndef TLBTEST_H_
#define TLBTEST_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct TLB
{
  int size;
  struct VPN** arr;
  struct evictList* eList;
};

struct VPN
{
  int number;
  int lastUsed;
};

struct evictList
{
  struct evictList *next;
  struct VPN* arr;
};

// Prototypes
void tlbtest(int tlbSize, int pageSize);
struct TLB* createTLB(int size);
struct VPN* createVPN();
struct evictList* addToEvictList(struct evictList* eList, struct VPN* vpn);
struct VPN *removeFromEvictList(struct evictList* eList, int vpn);
void insertIntoTLB(struct TLB* tlb, int vpn, int *rollingMiss);
//-----------

#endif

