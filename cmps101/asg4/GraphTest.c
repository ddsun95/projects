// David Sun
// ddsun@ucsc.edu
// pa4 (FindPath)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "List.h"

int main (void) {
	printf ("This program tests the graph client\n");
   Graph test_one = newGraph (6);
   Graph test_two = newGraph (10);
   /************************************************
    * Graph test_one has a representation as follows 
    *   1--------------2
    *   |             /|\
    *   |            / | \
    *   |           /  |  \
    *   3----------4---5---6
    * In this case, all the edges are undirected
    * and we will initialize our adjacency lists to
    * represent this graph
    * We will perform BFS on vertex 3 and test other
    * appropriate functions in the ADT as well
    ***********************************************/
   addArc (test_one, 1, 2); addArc (test_one, 1, 3);
   addArc (test_one, 2, 1); addArc (test_one, 2, 4); addArc (test_one, 2, 5);
                                                     addArc (test_one, 2, 6);
   addArc (test_one, 3, 1); addArc (test_one, 3, 4);
   addArc (test_one, 4, 2); addArc (test_one, 4, 3); addArc (test_one, 4, 5);
   addArc (test_one, 5, 2); addArc (test_one, 5, 4); addArc (test_one, 5, 6);
   addArc (test_one, 6, 2); addArc (test_one, 6, 5);
   BFS (test_one, 3);
   List threesix = newList();
   List threefiv = newList();
   getPath (threesix, test_one, 6);
   getPath (threefiv, test_one, 5);
   printf ("Path from three to five, Expected output: 3 1 2 6, Test output: ");
   printList (stdout, threesix);
   printf ("\n");
   printf ("Path from three to six: Expected output: 3 4 5, Test output: ");
   printList (stdout, threefiv);
   printf ("\n");
   freeList (&threesix);
   freeList (&threefiv);
   printf ("Order: Expected = 6, Output = %d\n", getOrder (test_one));
   printf ("Parent (6) = Expected = 2, Output = %d\n", getParent (test_one, 6));
   printf ("Distance (6) = Expected = 3, Output = %d\n", getDist (test_one, 6));
   printf ("___Adjacency List of test_one, Expected___\n");
   printf ("1: 2 3\n2: 1 4 5 6\n3: 1 4\n4: 2 3 5\n5: 2 4 6\n6: 2 5\n");
   printf ("___Adjacency List of test_two, Output___\n");
   printGraph (stdout, test_one); printf ("\n");
   printf ("The source of this graph is: Expected = 3, Output = %d\n",
           getSource (test_one));

   //Finish test of graph one, tests graph two now
   addEdge (test_two, 1, 2);
   addEdge (test_two, 1, 4);
   addEdge (test_two, 2, 3);
   addEdge (test_two, 2, 5);
   addEdge (test_two, 3, 6);
   addEdge (test_two, 4, 7);
   addEdge (test_two, 5, 6);
   addEdge (test_two, 5, 8);
   addEdge (test_two, 6, 8);
   addEdge (test_two, 7, 8);
   addEdge (test_two, 8, 9);
   addEdge (test_two, 9, 10);

   /**************************************************
   * visual representation of this graph
   *  1----------2----3
   *  |          |    |
   *  |          |    |
   *  4          5----6
   *  |          |    /
   *  |          |   /
   *  |          |  /
   *  |          | /   
   *  7-----------8---9----10
   ***************************************************/
   BFS (test_two, 3);
   printf ("Parent list of each vertex after BFS (test_two, 3) (Expected)\n");
   printf ("parent 1 = 2\n");
   printf ("parent 2 = 3\n");
   printf ("parent 3 = NIL\n");
   printf ("parent 4 = 1\n");
   printf ("parent 5 = 2\n");
   printf ("parent 6 = 3\n");
   printf ("parent 7 = 8\n");
   printf ("parent 8 = 6\n");
   printf ("parent 9 = 8\n");
   printf ("parent 10 = 9\n");
   printf ("The parent vertex of each vertex after BFS (test_two, 3)\n");
   for (int i = 1; i <= getOrder (test_two); ++i) {
      printf ("parent %d = ", i);
      getParent (test_two, i) == -1 ? printf ("NIL\n") 
                                    : printf ("%d\n", getParent (test_two, i));
   }
   printf ("The size of test_two is: Expected = 24, Output = %d\n", 
           getSize (test_two));
   makeNull (test_two);
   printf ("The size of test_two after makeNull (test_two) is ");
   printf ("Expected = 0, Output =%d\n", getSize (test_two));
   freeGraph (&test_one);
   freeGraph (&test_two);
}