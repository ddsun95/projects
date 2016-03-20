/****************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/23/2014
 * queue.h
 * this file defines the functions for the queue ADT
 ****************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

/* External interface declarations */

struct queue;
typedef struct queue queue;
typedef char *queue_item_t;

/* Function declarations */

queue *queue_new(void);

void queue_free(queue*);

void queue_insert(queue*, queue_item_t);

queue_item_t queue_remove(queue*);

bool queue_isempty(queue*);

#endif
