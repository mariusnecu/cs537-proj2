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

