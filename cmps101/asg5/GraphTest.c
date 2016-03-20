// David Sun
// ddsun@ucsc.edu
// pa5 (FindComponents)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "List.h"

int main (void) {
   printf ("This program tests the graph client\n");
   Graph test = newGraph (8);
   addArc (test, 1, 2); 
   addArc (test, 2, 3); addArc (test, 2, 5); addArc (test, 2, 6);
   addArc (test, 3, 4); addArc (test, 3, 7);
   addArc (test, 4, 3); addArc (test, 4, 8);
   addArc (test, 5, 1); addArc (test, 5, 6);
   addArc (test, 6, 7);
   addArc (test, 7, 6); addArc (test, 7, 8);
   addArc (test, 8, 8);
   printf ("Adjacency list representation of the test graph:\n");
   printGraph (stdout, test);
   printf ("The order of this graph is (Expected 8) %d\n", getOrder (test));
   printf ("The size of this graph is (Expected 14) %d\n", getSize (test));
   List verts = newList();
   Graph trans = transpose (test);
   Graph copytest = copyGraph (test);
   printf ("The adjacency list representation of the copyGraph (test):\n");
   printGraph (stdout, copytest);
   printf ("The adjacency list representation of the transpose (test):\n");
   printGraph (stdout, trans);
   for (int i = 1; i <= getOrder (test); ++i) append (verts, i);
   DFS (test, verts);
   printf ("Expected vertex list after DFS(): 1 2 5 3 7 6 4 8\n");
   printf ("..Actual vertex list after DFS(): ");
   printList (stdout, verts);
   printf ("\n");
   printf ("Test of function getParent()\n");
   printf ("Expected output: NIL 1 2 3 2 7 3 4\n");
   printf ("..Actual output: ");
   for (int i = 1; i <= getOrder (test); ++i) {
      int par = getParent (test, i);
      printf (par == -1 ? "NIL " : "%d ", par);
   }
   printf ("\n");
   printf ("Test of function getDiscover()\n");
   printf ("Expected output: 1 2 3 4 13 9 8 5\n");
   printf ("..Actual output: ");
   for (int i = 1; i <= getOrder (test); ++i) 
      printf ("%d ", getDiscover (test, i));
   printf ("\n");
   printf ("Test of function of getFinish()\n");
   printf ("Expected output: 16 15 12 7 14 10 11 6\n");
   printf ("..Actual output: ");
   for (int i = 1; i <= getOrder (test); ++i)
      printf ("%d ", getFinish (test, i));
   printf ("\n");
   freeList (&verts);
   freeGraph (&copytest);
   freeGraph (&trans);
   freeGraph (&test);
   return EXIT_SUCCESS;
}