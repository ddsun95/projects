// David Sun
// ddsun@ucsc.edu
// pa2 (Lex)
// List.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"


// structs -------------------------------------------------------------

/* node object */
typedef struct node_obj {
   int data;
   struct node_obj* next;
   struct node_obj* prev;
} node_obj;

typedef node_obj* node;

/* list object */
typedef struct list_obj {
   node head, tail, curs;
   int index, length;
} list_obj;

/* node constructor */
node new_node (int data) {
   node n = malloc (sizeof (node_obj));
   n->next = n->prev = NULL;
   n->data = data;
   return n;
}

/* list constructor */
List newList() {
   List list = malloc (sizeof (list_obj));
   list->head = list->tail = list->curs = NULL;
   list->index = -1;
   list->length = 0;
   return list;
}

/* node destructor */
void freeNode (node* pn) {
   if (pn != NULL && *pn != NULL) {
      free (*pn);
      *pn = NULL;
   }
}

/* list destructor */
void freeList (List* pL) {
   if (pL != NULL && *pL != NULL)
      while (length (*pL) > 0) deleteFront (*pL);
   free (*pL);
   *pL = NULL;
}

int length (List L) { return L->length; }

int index (List L) { 
   return L->curs == NULL ? -1 : L->index;
}

/* front() with precondition length > 0 */
int front (List L) {
   if (L == NULL) {
      printf ("called front() on NULL List\n");
      exit (1);
   }
   if (L->length == 0) {
      printf ("called front() on empty List\n");
      exit (1);
   }
   return L->head->data;
}

/* back() with precondition length > 0 */
int back (List L) {
   if (L == NULL) {
      printf ("called back() on NULL List\n");
      exit (1);
   }
   if (L->length == 0) {
      printf ("called back() on empty List\n");
      exit (1);
   }
   return L->tail->data;
}

/* get() with precondition length > 0 and index >= 0 */
int get (List L) {
   if (L->length <= 0) {
      printf ("called get() on empty list\n");
      exit (1);
   }
   if (L->index < 0) {
      printf ("called get() on NULL cursor\n");
      exit (1);
   }
   return L->curs->data;
}

int equals (List A, List B) {
   if (length(A) != length(B)) return 0;
   for (node a = A->head, b = B->head; a != NULL && b != NULL;
        a = a->next, b = b->next) if (a->data != b->data) return 0;
   return 1;
}

/* clear the list */
void clear (List L) { 
   while (L->length > 0) deleteFront (L);
}

/* moveFront() if length > 0 */
void moveFront (List L) {
   if (length(L) > 0) { L->curs = L->head; L->index = 0; }
}

/* moveBack() if length > 0 */
void moveBack (List L) {
   if (length(L) > 0) { L->curs = L->tail; L->index = L->length - 1; }
}

/* movePrev if cursor is not NULL */
void movePrev (List L) {
   if (L->curs != NULL) {
      L->curs = L->curs->prev;
      L->index = L->curs == NULL ? -1 : L->index - 1;
   }
}

/* moveNext if cursor is not NULL */
void moveNext (List L) {
   if (L->curs != NULL) {
      L->curs = L->curs->next;
      L->index = L->curs == NULL ? -1 : L->index + 1;
   }
}

/* insert at front of list */
void prepend (List L, int data) {
   node n = new_node (data);
   if (L->length == 0) {
      L->head = L->tail = n;
   } else {
      n->next = L->head;
      n->next->prev = n;
      L->head = n;
   }
   if (L->index >= 0) L->index++;
   L->length++;
}

/* insert at back of list */
void append (List L, int data) {
   node n = new_node (data);
   if (L->length == 0) {
      L->head = L->tail = n;
   } else {
      L->tail->next = n;
      n->prev = L->tail;
      L->tail = L->tail->next;
   }
   L->length++;
}

/* insert before cursor with precondition length > 0 and index >= 0 */
void insertBefore (List L, int data) {
   if (L->length <= 0) {
      printf ("insertBefore() on empty list\n");
      exit (1);
   }
   if (L->index < 0) {
      printf ("insertBefore() on NULL cursor\n");
      exit (1);
   }
   node n = new_node (data);
   n->next = L->curs;
   n->prev = L->curs->prev;
   if (L->curs == L->head) L->head = n;
   if (L->curs->prev != NULL) L->curs->prev->next = n;
   L->curs->prev = n;
   L->index++;
   L->length++;
}

/* insert after cursor with precondition length > 0 and index >= 0 */
void insertAfter (List L, int data) {
   if (L->length <= 0) {
      printf ("insertBefore() on empty list\n");
      exit (1);
   }
   if (L->index < 0) {
      printf ("insertBefore() on NULL cursor\n");
      exit (1);
   }
   node n = new_node (data);
   n->prev = L->curs;
   n->next = L->curs->next;
   if (L->curs == L->tail) L->tail = n;
   if (L->curs->next != NULL) L->curs->next->prev = n;
   L->curs->next = n;
   L->length++;
}

/* deleteFront() with precondition length > 0 */
void deleteFront (List L) {
   if (L == NULL) {
      printf ("deleteFront() on NULL list\n");
      exit (1);
   }
   if (L->length <= 0) {
      printf ("deleteFront() on empty list\n");
      exit (1);
   }
   if (L->curs == L->head) {
      L->curs = NULL;
      L->index = -1;
   }
   node delete = L->head;
   if (L->curs == L->head) { 
      L->curs = NULL; 
      L->index--; 
   }
   if (L->tail == L->head) L->tail = NULL;
   L->head = L->head->next;
   if (L->head != NULL) L->head->prev = NULL;
   if (L->index > 0) L->index--;
   L->length--;
   freeNode (&delete);
}

/* deleteBack() with precondition length > 0 */
void deleteBack (List L) {
   if (L == NULL) {
      printf ("deleteBack() on NULL list\n");
      exit (1);
   }
   if (L->length <= 0) {
      printf ("deleteBack() on empty list\n");
      exit (1);
   }
   node delete = L->tail;
   if (L->curs == L->tail) {
      L->curs = NULL;
      L->index = -1;
   }
   if (L->head == L->tail) L->head = NULL;
   L->tail = L->tail->prev;
   if (L->tail != NULL) L->tail->next = 0;
   L->length--;
   freeNode (&delete);
}

/* delete cursor element with precondition length > 0 and index >= 0 */
void delete (List L) {
   if (L == NULL) {
      printf ("delete() on NULL list\n");
      exit (1);
   }
   if (L->length <= 0) {
      printf ("delete() on empty list\n");
      exit (1);
   }
   if (L->index < 0) {
      printf ("delete() on NULL cursor\n");
      exit (1);
   }   
   if (L->curs == L->tail) deleteBack (L);
   else if (L->curs == L->head) deleteFront (L);
   else {
      node n = L->curs;
      L->curs->prev->next = L->curs->next;
      L->curs->next->prev = L->curs->prev;
      L->curs = NULL;
      L->index = -1;
      L->length--;
      freeNode (&n);
   }
}

void printList (FILE* out, List L) {
   if (out == NULL) {
      printf ("invalid file specified\n");
      exit (1);
   }
   for (node t = L->head; t != NULL; t = t->next) fprintf (out, "%d ", t->data);
}

List copyList (List L) {
   List ret = newList();
   for (node t = L->head; t != NULL; t = t->next) append (ret, t->data);
   return ret;
}
