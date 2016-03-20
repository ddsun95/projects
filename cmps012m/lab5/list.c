/**
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/15/2014
 * list.c
 * This file is the linked list file with
 * functions to insert, remove, print, and destroy
 * the list.
 * The completed program must have no memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>

// A node in a singly-linked list
struct node {
    int value;
    struct node *next;
};

// Head of the linked list
struct node *head;

// Insert a value into linked list
void list_insert(int v) {
    struct node* n = (struct node*)malloc(sizeof(struct node)); //allocate memory
    n->value = v;   //assign v to value field
    n->next = head; //assign the next pointer to point at the old head
    head = n;       //assign head to point at the newly inserted node
}

// Insert two values at once into linked list
void list_insert2(int a, int b) {
	void list_insert(int v);
   	list_insert(b); //insert b first
   	list_insert(a); //insert a second so that it will be the first element in the list
}

// Remove an element from linked list
void list_remove(int v) {
	//use two pointers
	//One to navigate through the list
	//And the other to call free() on.
	if(head == NULL){  //if there are no elements in the list, quit with an exit code.
		exit(1);
	}
    struct node *t;				
    struct node *t_follow;
    for(t = head; t->next != NULL; t = t->next){
    	if(t->next->value == v){
    		t_follow = t->next;
    		t->next = t->next->next;
    		free(t_follow);
    	}
    }
    t_follow = NULL;    //ensure no dangling pointers
}

// Print out all values in linked list
void list_printall(void) {
    struct node *p = head;
    int n = 0;
    while(p) {
        printf("%d ", p->value);
        p=p->next;
        n++;
        if(n > 100) break;
    }
    printf("\n");
}

// Deallocate all memory used in linked list
void list_destroy(void) {
    if(head && head->next == NULL){
    	free(head);
        head = NULL;
    	return;
    }

    struct node *t = head;
    struct node *ft = t;
    while(t){
    	ft = t;
    	t = t->next;
    	free(ft);
    }
    ft = NULL;      //ensure no dangling pointers
    head = NULL;    //ensure no dangling pointers
}

int main(int argc, char *argv[]) {
    printf("Test linked lists\n");
    list_printall(); // Should print nothing
    list_insert(42);
    list_insert2(17, 10);
    list_insert(18);
    list_remove(10);
    list_printall(); // Should print 18 17 42
    // Cleanup memory
    list_destroy();
    return 0;
}
