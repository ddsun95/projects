// David Sun
// ddsun@ucsc.edu
// pa5 (FindComponents)

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

#define WHITE  0
#define GREY   1
#define BLACK  2
#define NIL   -1
#define INF   -2
#define UNDEF -3

int time = 0;

typedef struct graphobj {
   List* adj;
   int *clr, *par, *dis, *dsc, *fin;
   int order, size;
} graphobj;


Graph newGraph (int n) {
   Graph graph = malloc (sizeof (graphobj));
   graph->adj  = malloc (sizeof (List) * (n + 1));
   graph->clr  = malloc (sizeof (int)  * (n + 1));
   graph->par  = malloc (sizeof (int)  * (n + 1));
   graph->dis  = malloc (sizeof (int)  * (n + 1));
   graph->dsc  = malloc (sizeof (int)  * (n + 1));
   graph->fin  = malloc (sizeof (int)  * (n + 1));
   for (size_t i = 0; i < n + 1; ++i) {
      graph->adj[i] = i == 0 ? NULL : newList();
      graph->clr[i] = WHITE;
      graph->par[i] = NIL;
      graph->dis[i] = INF;
      graph->dsc[i] = graph->fin[i] = UNDEF;
   }
   graph->order = n;
   graph->size  = 0;
   return graph;
}

void freeGraph (Graph* graph) {
   if (graph != NULL && *graph != NULL) {
      Graph g = *graph;
      free (g->clr); free (g->par); free (g->dis); free (g->fin); free (g->dsc);
      for (int i = 1; i <= g->order; ++i) freeList (&g->adj[i]);
      free (g->adj); free (g);
      g = NULL;
      graph = NULL;
   }
}

int getOrder  (Graph graph) { return graph->order; }
int getSize   (Graph graph) { return graph->size; }
 
/* precondition : u >= 1 and u <= getOrder (graph) */
int getParent (Graph graph, int u) { 
   if (u < 1 || u > getOrder (graph)) {
      printf ("getParent(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   return graph->par[u]; 
}

/* precondition: u >= 1 and u <= getOrder (G) */
int getDiscover (Graph graph, int u) {
   if (u < 1 || u > getOrder (graph)) {
      printf ("getDiscover(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   return graph->dsc[u];
}

/* precondition u >= 1 and u <= getOrder (G) */
int getFinish (Graph graph, int u) {
   if (u < 1 || u > getOrder (graph)) {
      printf ("getDiscover(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   return graph->fin[u];
}

/* precondition : u >= 1 and u <= getOrder (graph) */
/* precondition : v >= 1 and v <= getOrder (graph) */
void addArc (Graph G, int u, int v) {
   if (u < 1 || u > getOrder (G)) {
      printf ("addArc(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   if (v < 1 || v > getOrder (G)) {
      printf ("addArc(): 1 <= v <= getOrder (G)\n");
      exit (1);
   }
   List this = G->adj[u];
   if (length (this) == 0) append (this, v);
   else {
      moveFront (this);
      while (index (this) >= 0) {
         if (v < get (this)) { insertBefore (this, v); break; }
         if (v == get (this)) break;
         moveNext (this);
         if (index (this) == -1) { append (this, v); break; }
      }
   }
   ++G->size;
}

void addEdge (Graph G, int u, int v) {
   addArc (G, u, v); 
   addArc (G, v, u);
}

void visit (Graph graph, int vert, List finlist) {
   graph->clr[vert] = GREY;
   graph->dsc[vert] = ++time;
   List adjv = graph->adj[vert];
   for (moveFront (adjv); index (adjv) >= 0; moveNext (adjv)) {
      int y = get (adjv);
      if (graph->clr[y] == WHITE) {
         graph->par[y] = vert;
         visit (graph, y, finlist);
      }
   }
   graph->clr[vert] = BLACK;
   graph->fin[vert] = ++time;
   prepend (finlist, vert);
}

/* precondition: getLength (S) == getOrder (G) */
void DFS (Graph graph, List list) {
   if (length (list) != getOrder (graph)) {
      printf ("DFS(): getLength (S) == getOrder (G)\n");
      exit (1);
   }
   for (moveFront (list); index (list) >= 0; moveNext (list)) {
      int x = get (list);
      graph->clr[x] = WHITE;
      graph->par[x] = NIL;
   }
   time = 0;
   moveFront (list);
   for (int i = 1; i <= getOrder (graph); ++i) {
      int x = get (list);
      moveNext (list);
      if (graph->clr[x] == WHITE) visit (graph, x, list);
   }
   int cut = length (list) / 2;
   for (int i = 0; i < cut; ++i) deleteBack (list);
}

Graph transpose (Graph graph) {
   Graph tran = newGraph (getOrder (graph));
   for (size_t i = 1; i <= getOrder (graph); ++i) {
      List orig = graph->adj[i];
      for (moveFront (orig); index (orig) >= 0; moveNext (orig))
         addArc (tran, get (orig), i);
   }
   return tran;
}

Graph copyGraph (Graph graph) {
   Graph copy = newGraph (getOrder (graph));
   for (size_t i = 1; i <= getOrder (graph); ++i) {
      List orig = graph->adj[i];
      for (moveFront (orig); index (orig) >= 0; moveNext (orig))
         addArc (copy, i, get (orig));
   }
   return copy;
}

void printGraph (FILE* out, Graph G) {
   for (int i = 1; i <= G->order; ++i) {
      fprintf (out, "%d: ", i);
      printList (out, G->adj[i]);
      fprintf (out, "\n");
   }
}
