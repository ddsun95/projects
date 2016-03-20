/***********************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/7/2014
 * queue.c
 * queue.c contains the functions for the queue
 * and the unit tests.
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

#define MAXSIZE 100
#define DESTROY_VALUE 0

int tests_run = 0; // keep track of number of unit tests run
/* variables for queue */
int head;
int rear;
int elements;

typedef enum q_status {
    /* Enumerated status codes for queue operations */
    q_success = 0,
    q_failure
}   q_status;


/* Declare the queue array data structure of size 100*/
static int queue[MAXSIZE];

/**** Functions on queues ****/
q_status q_init(void) {
	head = 0;
	rear = -1;
	elements = 0;

	if(head == 0 && rear == -1 && elements == 0)
		return q_success;

	return q_failure;
}

q_status q_insert(int value) {
    /* Insert an item into back of queue
    
       Returns q_success on success.
    */

    if(elements == 100){	//return a failure code on trying to insert in a full queue
        return q_failure;
    }

    if(rear == MAXSIZE - 1){	//wrap around if needed
        rear = -1;
    }	
	rear++;
    queue[rear] = value; 	//increment rear, insert the value
    elements++;		       //increment the number of elements	
    return q_success;

		

}

q_status q_remove(int *value) {
    /* Remove item from front of queue

       Stores item at front of queue into pointer
       given as argument. Removes item from queue.

       Returns qSuccess on success.
    */
	if(value == NULL){	//return failure on a null argument
		return q_failure;
	}
	
    if(elements == 0){	//return failure if the number of elements is 0
       return q_failure;
    }

    *value = queue[head]; //retrieve the value of head, increment head
	head++;				//increment head
    if(head == 100) {   
        head = 0;
    }
    elements--;	//decrement the number of elements by 1
    return q_success;

}

q_status q_peek(int *value) {
    /* Peek at item at front of queue

       Stores item at front of queue into pointer
       given as argument. Queue is not altered.

       Returns qSuccess on success.
    */
	if(value == NULL){	//return failure on a null argument
		return q_failure;
	}

    if(elements == 0){	//return failure on when there are no elements
        return q_failure;
    }

    *value = queue[head]; //assign the value to the placeholder
    return q_success;

}

q_status q_destroy(void) {

    int i;
    for(i = 0; i < MAXSIZE; i++)	//destroy the array, sets all values to 0
        queue[i] = DESTROY_VALUE;

    return q_success;

}

/* returns 0 on if not empty, 1 if empty */
int is_empty(void){
	int i;
	for(i = 0; i < MAXSIZE; i++){	
		if(queue[i] != 0)
			return 0;
	}
	return 1;
}

/**** Unit tests ****/

/* My unit test for testing whether or not the queue is empty
 * This test should return true, since nothing is inserted */
char * test_empty(void){
	mu_assert("init", q_init() == q_success);
	mu_assert("value", is_empty() == 1);
	mu_assert("destroy", q_destroy() == q_success);
	return NULL;
}

/* My unit test for testing whether or not the queue is not empty
 * This test should return false, since an item is inserted */
char * test_not_empty(void){
	mu_assert("init", q_init() == q_success);
	mu_assert("insert", q_insert(1) == q_success);
	mu_assert("value", is_empty() == 0);
	mu_assert("destroy", q_destroy() == q_success);
	return NULL;
}
/** MY UNIT TEST OF TESTING/REMOVING 3 VALUES **/
char * insert_remove_three(void){
	int v;
	mu_assert("init", q_init() == q_success);
	mu_assert("insert", q_insert(1) == q_success);
	mu_assert("insert", q_insert(2) == q_success);
	mu_assert("insert", q_insert(3) == q_success);
	mu_assert("remove", q_remove(&v) == q_success);
	mu_assert("value", v == 1);
	mu_assert("remove", q_remove(&v) == q_success);
	mu_assert("value", v == 2);
	mu_assert("remove", q_remove(&v) == q_success);
	mu_assert("value", v == 3);
	mu_assert("destroy", q_destroy() == q_success);
	return NULL;
}

char * test_lifecycle(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_one_insert(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(7) == q_success);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_one_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(16) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 16);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_two_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(8) == q_success);
    mu_assert("insert", q_insert(91) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 8);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 91);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_multi_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(8) == q_success);
    mu_assert("insert", q_insert(91) == q_success);
    for(int i = 0; i < 1000; i++) {
        mu_assert("insert", q_insert(8) == q_success);
        mu_assert("insert", q_insert(91) == q_success);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == 8);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == 91);
    }
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("value", v == 8);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_indexed_insert_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 1000; i++) {
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("insert", q_insert(30 + i) == q_success);
    }
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_too_many_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("remove", q_remove(&v) == q_failure);
    return NULL;
}

char * test_insert_too_many_remove(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 30; i++) {
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
    }
    mu_assert("remove", q_remove(&v) == q_failure);
    return NULL;
}

char * test_peek(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_peek(&v) == q_success);
    mu_assert("value", v == 18);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_two(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("insert", q_insert(42) == q_success);
    mu_assert("peek", q_peek(&v) == q_success);
    mu_assert("value", v == 18);
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_deep(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    for(int i = 0; i < 30; i++) {
        mu_assert("insert", q_insert(i) == q_success);
    }
    for(int i = 0; i < 1000; i++) {
        mu_assert("peek", q_peek(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("remove", q_remove(&v) == q_success);
        mu_assert("value", v == i);
        mu_assert("insert", q_insert(30 + i) == q_success);
    }
    mu_assert("destroy", q_destroy() == q_success);
    return NULL;
}

char * test_peek_empty(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("peek", q_peek(&v) == q_failure);
    return NULL;
}

char * test_peek_two_empty(void) {
    int v;
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("insert", q_insert(42) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("remove", q_remove(&v) == q_success);
    mu_assert("peek", q_peek(&v) == q_failure);
    return NULL;
}

char * test_remove_null(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_remove(NULL) == q_failure);
    return NULL;
}

char * test_peek_null(void) {
    mu_assert("init", q_init() == q_success);
    mu_assert("insert", q_insert(18) == q_success);
    mu_assert("peek", q_peek(NULL) == q_failure);
    return NULL;
}


char * all_tests(void) {
    /* Run all unit tests */
    mu_run_test(test_lifecycle);
    mu_run_test(test_one_insert);
    mu_run_test(test_one_insert_remove);
    mu_run_test(test_two_insert_remove);
    mu_run_test(test_multi_insert_remove);
    mu_run_test(test_indexed_insert_remove);
    mu_run_test(test_too_many_remove);
    mu_run_test(test_insert_too_many_remove);
    mu_run_test(test_peek);
    mu_run_test(test_peek_two);
    mu_run_test(test_peek_deep);
    mu_run_test(test_peek_empty);
    mu_run_test(test_peek_two_empty);
    mu_run_test(test_remove_null);
    mu_run_test(test_peek_null);
	mu_run_test(insert_remove_three);
	mu_run_test(test_empty);
	mu_run_test(test_not_empty);
    return NULL;
}


/**** Main unit test runner ****/

int main(int argc, char *argv[]) {
    printf("Queue Module unit tests\n");
    char *result = all_tests();
    if (result) {
        printf("FAILURE at %s\n", result);
    } else {
        printf("ALL TESTS PASS\n");
    }
    printf("Tests run: %d\n", tests_run);
    return EXIT_SUCCESS;
}
