#ifndef TLBTEST_H_
#define TLBTEST_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct TLB
{
	int size;
	int pageSize;
	struct VPN** arr;
	struct evictList* eList;
};

struct VPN
{
	unsigned long number;
	int hits;
};

struct evictList
{
	int size;
	struct VPN** arr;
};

// Prototypes
void tlbtest(int tlbSize, int pageSize);
struct TLB* createTLB(int size, int pageSize);
struct VPN* createVPN();
struct evictList* createEvictList(int initSize);
struct evictList* add(struct evictList* eList, struct VPN* vpn);
int inEvictList(struct evictList* eList, struct VPN* vpn);
void removeFromEvictList(struct evictList* eList, struct VPN* vpn);
void insertIntoTLB(struct TLB* tlb, struct VPN* vpn, int *rollingMiss);
int countUnique(struct TLB* tlb);
//-----------

#endif

