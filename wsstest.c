#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include"vmsim.h"

int page(unsigned int address, int pagesize) {
  int offset = (int)ceil(log2((double)pagesize));
  return address >> offset;
}

void wsstest(int pagesize, int windowsize) {
  mem_loc *loc = mem_head;
  int c=0,cmod,i, runningtotal=0, sum, sums = 0;
  mem_loc **window;
  int same_as[windowsize];
  //  FILE *TRACE;

  for(i=0;i<windowsize;i++) {
    same_as[i]=-2;
  }

  window = (mem_loc**) malloc(sizeof(mem_loc *) * windowsize);
  //  write(1, "HERE", strlen("HERE"));
  //  TRACE = fopen("wstrace.out","w");

  while(loc!=NULL) {

    cmod = c % windowsize;
    window[cmod] = loc;
    same_as[cmod]=-1; //Enters, now test for uniqueness
    //    fprintf(TRACE,"Time %i\n",c/2);
    //    fprintf(TRACE,"placing 0x%x",page(window[cmod]->address,pagesize));
    for(i=c-1;i>=c-windowsize && i>=0;i--) {
      if(window[i%windowsize]!=NULL
	 && page(window[cmod]->address,pagesize)
	    == page(window[i%windowsize]->address,pagesize)) {
	same_as[cmod]=i;
	//	fprintf(TRACE, ", same as %i in %i\n", page(window[i%windowsize]->address,pagesize),i);
	break;
      }
    }
//    for(i=0;i<windowsize;i++) {
//      fprintf(TRACE,"__%i\n",same_as[i]);
//    }
    if(c%2==1 && (c>windowsize || loc->next==NULL)) {
      sum=0;
      for(i=0;i<windowsize;i++) {
	if(same_as[i]==-1 || (same_as[i]<=c-windowsize && same_as[i]!=-2)) {
	  sum++;
	}
      }
      //      fprintf(TRACE,"sum %i\n",sum);
      runningtotal+=sum;
      sums++;
    }
    c++;
    loc = loc->next;
  }
  printf("WS Size: %i\n",runningtotal/sums);
}
