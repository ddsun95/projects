/****************************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/22/2014
 * queue.c
 * queue.c implements the functions appropriate for the queue ADT
 ****************************************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define STUBPRINTF(...) fprintf(stderr, __VA_ARGS__);

/* Internal implementation definitions */
struct queue_node {
   queue_item_t item;
   struct queue_node *link;
};

typedef struct queue_node queue_node;

struct queue {
   queue_node *front;
   queue_node *rear;
};

/* Functions */

queue *queue_new(void) {
   queue *this = (queue*)malloc(sizeof(queue)); //allocate memory for a queue
   this->front = NULL;                          //no elements yet, set both fields to null
   this->rear = NULL;
   return this;
}

void queue_free(queue *this) {
   assert(queue_isempty(this));
   free(this);
}

void queue_insert(queue *this, queue_item_t item) {
   assert(this != NULL && item != NULL);
   queue_node* qn = (queue_node*)malloc(sizeof(queue_node));   //allocate memory for a new node
   qn->item = item; //assign the item to the item field                           
   qn->link = NULL; //assign NULL to the node's next value
   if(queue_isempty(this)){ //if only one item assign front and rear to the item
      this->front = qn; 
      this->rear = qn;
   } else { //otherwise append the item to the end of the queue
      this->rear->link = qn;
      this->rear = qn;
   }
   printf("item = \"%s\"\n", item);
}

queue_item_t queue_remove(queue *this) {
   assert(!queue_isempty(this));
   queue_node* qnptr = this->front; //qnptr for queue node pointer
   queue_item_t item = this->front->item; //variable to point to node's item
   this->front = this->front->link; //assign front to next element
   free(qnptr); //free the pointer
   return item; //return the pointer
}

bool queue_isempty(queue *this) {
   return this->front == NULL;
}
