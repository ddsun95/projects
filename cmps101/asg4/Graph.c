// David Sun
// ddsun@ucsc.edu
// pa4 (FindPath)

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

#define WHITE 0
#define GREY  1
#define BLACK 2
#define NIL  -1
#define INF  -2

typedef struct graphobj {
   List* adj;
   int* clr;
   int* par;
   int* dis;
   int order, size, mrv;
} graphobj;


Graph newGraph (int n) {
   Graph graph = malloc (sizeof (graphobj));
   graph->adj  = malloc (sizeof (List) * (n + 1));
   graph->clr  = malloc (sizeof (int)  * (n + 1));
   graph->par  = malloc (sizeof (int)  * (n + 1));
   graph->dis  = malloc (sizeof (int)  * (n + 1));
   for (size_t i = 0; i < n + 1; ++i) {
      graph->adj[i] = i == 0 ? NULL : newList();
      graph->clr[i] = WHITE;
      graph->par[i] = NIL;
      graph->dis[i] = INF;
   }
   graph->order = n;
   graph->size  = 0;
   graph->mrv   = NIL;
   return graph;
}

void freeGraph (Graph* graph) {
   if (graph != NULL && *graph != NULL) {
      Graph g = *graph;
      free (g->clr);
      free (g->par);
      free (g->dis);
      for (int i = 1; i <= g->order; ++i) freeList (&g->adj[i]);
      free (g->adj);
      free (g);
      g = NULL;
   }
}

int getOrder  (Graph graph) { return graph->order; }
int getSize   (Graph graph) { return graph->size; }
int getSource (Graph graph) { return graph->mrv; }
 
/* precondition : u >= 1 and u <= getOrder (graph) */
int getParent (Graph graph, int u) { 
   if (u < 1 || u > getOrder (graph)) {
      printf ("getParent(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   return graph->par[u]; 
}

/* precondition : u >= 1 and u <= getOrder (graph) */
int getDist (Graph graph, int u) {
   if (u < 1 || u > getOrder (graph)) {
      printf ("getDist(): 1 <= u <= getOrder (G)\n");
      exit (1);
   }
   return graph->dis[u]; 
}

void getPath (List list, Graph graph, int u) {
   if (u == getSource (graph)) append (list, u);
   else if (getParent (graph, u) == NIL) append (list, NIL);
   else {
      getPath (list, graph, getParent (graph, u));
      append (list, u);
   }
}

void makeNull (Graph G) {
   for (int i = 1; i <= G->order; ++i) {
      List delete = G->adj[i];
      G->adj[i] = newList();
      freeList (&delete);
   }
   G->size = 0;
}

void addEdge (Graph G, int u, int v) {
   addArc (G, u, v); 
   addArc (G, v, u);
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

void BFS (Graph G, int s) {
   G->mrv = s;
   for (int i = 1; i <= G->order; ++i) {
      G->clr[i] = WHITE;
      G->dis[i] = INF;
      G->par[i] = NIL;
   }
   G->clr[s] = GREY;
   G->dis[s] = 0;
   G->par[s] = NIL;
   List queue = newList();
   append (queue, s);
   while (length (queue) > 0) {
      int x = front (queue);
      deleteFront (queue);
      List adj = G->adj[x];
      for (moveFront (adj); index (adj) >= 0; moveNext (adj)) {
         int y = get (adj);
         if (G->clr[y] == WHITE) {
            G->clr[y] = GREY;
            G->dis[y] = G->dis[x] + 1;
            G->par[y] = x;
            append (queue, y);
         }
      }
      G->clr[x] = BLACK;
   }
   freeList (&queue);
}

void printGraph (FILE* out, Graph G) {
   for (int i = 1; i <= G->order; ++i) {
      fprintf (out, "%d: ", i);
      printList (out, G->adj[i]);
      fprintf (out, "\n");
   }
}
