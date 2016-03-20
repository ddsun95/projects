// David Sun
// ddsun@ucsc.edu
// pa4 (FindPath)

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
int getOrder  (Graph);
int getSize   (Graph);
int getSource (Graph);
int getParent (Graph, int);
int getDist   (Graph, int);
void getPath  (List, Graph, int);

//
// Manipulation Procedures
//
void makeNull (Graph);
void addEdge  (Graph, int, int);
void addArc   (Graph, int, int);
void BFS      (Graph, int);

//
// Other Operations
//
void printGraph (FILE*, Graph);
void debug (Graph);

#endif