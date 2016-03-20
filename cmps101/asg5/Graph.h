// David Sun
// ddsun@ucsc.edu
// pa5 (FindComponents)

#include "List.h"
#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct graphobj* Graph;

//
// constructors/destructors
//
Graph newGraph (int);
void freeGraph (Graph*);

//
// Access Functions
//
int getOrder    (Graph);
int getSize     (Graph);
int getParent   (Graph, int);
int getDist     (Graph, int);
int getDiscover (Graph, int);
int getFinish   (Graph, int);

//
// Manipulation Procedures
//
void addEdge  (Graph, int, int);
void addArc   (Graph, int, int);
void DFS      (Graph, List);

//
// Other Operations
//
void printGraph (FILE*, Graph);
Graph transpose (Graph);
Graph copyGraph (Graph);

#endif