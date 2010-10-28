//Disconnected version stuff
//#include<stdlib.h>
//#include<stdio.h>
//END disconnected

#include "prtest.h"

//To be replaced with the page array from part 0
struct memList {
  unsigned int value;
  struct memList* next;
};

struct memList* exampleList() {
  //Create artificial list
  struct memList *head, *next, *temp;
  //  int numbers[7] = {4,2,3,4,4,1,4};
  int numbers[20] = {4,2,3,4,4,1,4,4,2,2,1,5,5,7,5,8,6,4,9,1};
  int i;
  head = (struct memList *) malloc(sizeof(struct memList));
  next = head;
  for(i=0;i<sizeof(numbers)/sizeof(int);i++) {
    //printf("%i ", numbers[i]);
    next->value = numbers[i];
    temp = (struct memList*) malloc(sizeof(struct memList));
    next->next = temp;
    next = temp;
  }
  return head;
}

struct fifoQueue {
  unsigned int value;
  struct fifoQueue *next;
};

/**
 * Removes elt from the queue if it exists -- return the queue head if succeeds, NULL if fails
 * Comparison is based on value. Note that this does not move the queue's head --
 * outside functions are responsible for this.
 */
struct fifoQueue* fifoRemove(struct fifoQueue *elt, struct fifoQueue *queue) {
  struct fifoQueue *temp = queue, *previous;
  previous = temp;
  if(elt==NULL) {
    printf("Error: elt not set when remove called\n");
    exit(1);
  }
  if(queue==NULL) {
    return NULL;
  }

  //Head case
  //  printf("%i_%i\n",temp->value,elt->value);
  if(temp->value==elt->value) { //Move the head
    //    printf("head modification\n");
    temp = queue;
    queue = queue->next;
    temp->next = NULL;
    return queue;
  }
  //The rest
  temp = temp->next;
  while(temp!=NULL) {
    //    printf("%i_%i\n",temp->value,elt->value);
    if(temp->value==elt->value) {
      previous->next = temp->next; //temp is now skipped
      temp->next = NULL;
      return queue;
    }
    previous = temp;
    temp = temp->next;
  }
  return NULL;
}

/**
 * Add element to a fifoQueue;
 * Returns evicted elt or NULL if none evicted;
 * add makes no assumptions but is slower
 * addFast assume the queue is not full so does no eviction
 */
struct fifoQueue* fifoAdd(struct fifoQueue *elt, struct fifoQueue *queue, int MAX) {
  struct fifoQueue *temp, *next, *extras;
  int size;
  elt->next = queue;
  queue = elt;
  temp = queue;
  size=1;
  if(temp==NULL) return NULL;
  //  while(temp!=NULL) {
  //    printf("%i!\n",temp->value);
  //    temp = temp->next;
  //  }
//  temp = queue;
  while(temp->next!=NULL) {
    next = temp->next;
    size++;
    if(size>MAX) {
      //Check for more than one extra elt
      extras = next->next;
      while(extras!=NULL) {
	printf("Warning: extra list elt lost\n");
      }
      //
      temp->next = NULL;
      return next;
    }
    //    if(next->next==NULL) {
    //      temp->next = NULL;
    //      return next;
    //    }
    temp = temp->next;
  }
  return NULL;
}
void fifoAddFast(struct fifoQueue *elt, struct fifoQueue *queue) {
  elt->next = queue;
  queue = elt;
}

//The first fifo is constructed using an array and a counter.
//The second fifo uses a list and a size check, to allow for
//removal of an internal elt to move back to the first fifo.
void prtest_2nd_fifo(struct memList *list, int numframes) {
  struct memList *head, *next;
  unsigned int fifo[numframes*3/4], filled[numframes*3/4], lastFirstFifo; //filled so that accesses to initial int value are checked
  struct fifoQueue *queue=NULL, *elt=NULL, *evicted, *temp;
  int i,j, hardfaults=0, softfaults=0, numframes1=numframes*3/4, numframes2=numframes/4;
  int found=0;
  
  //Initialize first fifo
  for(i=0;i<numframes;i++) {
    fifo[i] = 0;
    filled[i] = 0;
  }
  
  //Initialize second fifo

  //Get list
  head = list;
  next = head;
  
  //Enter list elts to fifo
  i=0;
  while(next!=NULL && next->next!=NULL) {
    found=0;
    for(j=0;j<numframes;j++) {
      if(next->value == fifo[j] && filled[j]) {
	found=1;
	break;
      }
    }

    if(found) {
      next = next->next;
      continue;
    }

    //Missed first fifo, now check second
    if(filled[i]) {

      //Search for item in second fifo
      elt = (struct fifoQueue*) malloc(sizeof(struct fifoQueue));
      elt->value = next->value;

      //      evicted = queue;
      //      printf("elt is %i\n", elt->value);
      //      while(evicted!=NULL) {
      //	printf("  vs %i \n",evicted->value);
      //	evicted = evicted->next;
      //      }
      temp = fifoRemove(elt,queue);
      if(temp!=NULL) { //Found elt in the secondFifo ==softfault
	queue = temp; //reassign queue to the head of the modified queue (in case head was removed)
	//	printf("found!!\n");
	//Add end of firstFifo to secondFifo
	lastFirstFifo = fifo[i];
	elt->value = lastFirstFifo;

	//	evicted = queue;
	//	while(evicted!=NULL) {
	//	  printf("%i_____\n",evicted->value);
	//	  evicted = evicted->next;
	//	}

	evicted = fifoAdd(elt,queue,numframes2); //Replace with addFast once checked
	queue = elt;

	//	printf("AFTER\n");
	//	evicted = queue;
	//	while(evicted!=NULL) {
	//	  printf("%i_____\n",evicted->value);
	//	  evicted = evicted->next;
	//	}

	if(evicted!=NULL) { //Error check
	  printf("Error: queue size should not be exceeded here\n");
	  exit(1);
	}

	//Add secondFifo->value back to first queue
	fifo[i] = next->value;
	softfaults++;

      } else { 	// ==hardfault

	elt->value=fifo[i];
	fifo[i] = next->value; //firstFifo replaced
	evicted = fifoAdd(elt,queue,numframes/4); //secondFifo replaced
	queue = elt;
	//	evicted = queue;
	//	while(evicted!=NULL) {
	//	  printf("%i_____\n",evicted->value);
	//	  evicted = evicted->next;
	//	}

	hardfaults++;
	if(evicted!=NULL) {
	  //	  printf("evicting %i\n",evicted->value);
	  free(evicted);
	  evicted = NULL;
	}

      }

    } else {
      filled[i]=1;
      fifo[i]=next->value;
      hardfaults++;
    }

    i = (i+1)%numframes1;
    next = next->next;
  }
  printf("hard faults: %i\n",hardfaults);
  printf("soft faults: %i\n",softfaults);

  //Release second queue, firstqueue should release by itself
  while(queue!=NULL) {
    elt = queue;
    queue = queue->next;
    free(elt);
  }
}


//The first queue is constructed using an array and a counter,
//because it only needs to support fixed size and removal from
//the last used
void prtest_fifo(struct memList *list, int numframes) {
  struct memList *head, *next;
  unsigned int fifo[numframes], filled[numframes]; //filled so that accesses to initial int value are checked
  int i,j, hardfaults=0;
  int found=0;
  //Initialize array
  for(i=0;i<numframes;i++) {
    fifo[i] = 0;
    filled[i] = 0;
  }

  //Get list  
  head = list;
  next = head;

  //Enter list elts to fifo
  i=0;
  while(next!=NULL && next->next!=NULL) {
    found=0;
    for(j=0;j<numframes;j++) { //Is it already in here?
      if(next->value == fifo[j] && filled[j]) {
	found=1;
	break;
      }
    }
    if(found) {
      next = next->next;
      continue;
    } else {
      fifo[i]=next->value;
      filled[i]=1;
      hardfaults++;
      i = (i+1)%numframes;
      next = next->next;
    }
  }
  printf("hard faults: %i\n",hardfaults);
  printf("soft faults: 0\n");
}

//prtest main function
int part4main(int argc, char**argv) {
  printf("Hello world\n");
  //  exampleList();
  prtest_fifo(exampleList(),6);
  prtest_2nd_fifo(exampleList(),8);
  return 0;
}
//END main prtest main function
