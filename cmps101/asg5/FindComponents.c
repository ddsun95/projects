// David Sun
// ddsun@ucsc.edu
// pa5 (FindComponents)

#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
#include "Graph.h"
#include "List.h"

#define NIL    -1
#define MAXLINE 160

void process (Graph graph, char** linebuf) {
   for (int i = 1;;) {
      char *row, *col;
      row = __strtok_r (linebuf[i++], " ", &col);
      if (atoi (row) == 0 && atoi (col) == 0) break;
      addArc (graph, atoi (row), atoi (col));
   }
}

int main (int argc, char** argv) {
   if (argc != 3) {
      printf ("usage: FindComponents infile outfile\n");
      exit (1);
   }
   int lines = 0;
   char buf[MAXLINE];
   FILE* count = fopen (argv[1], "r");
   FILE* read  = fopen (argv[1], "r");
   FILE* out   = fopen (argv[2], "w");
   if (count == NULL || read == NULL) {
      printf ("Unable to open %s for reading\n", argv[1]);
      exit (1);
   }
   if (out == NULL) {
      printf ("Unable to open %s for writing", argv[2]);
      exit (1);
   }
   while (fgets (buf, MAXLINE, count) != NULL) ++lines;
   char** linebuf = malloc (sizeof (char*) * lines);
   for (int i = 0; i < lines; ++i) {
      char line[MAXLINE];
      fgets (line, MAXLINE, read);
      linebuf[i] = malloc (sizeof (line) + 1);
      strcpy (linebuf[i], line);
      strtok (linebuf[i], "\n");
   }
   Graph dfsg = newGraph (atoi (linebuf[0]));
   process (dfsg, linebuf);
   Graph tran = transpose (dfsg);
   List vertlist = newList();
   for (int i = 1; i <= getOrder (dfsg); ++i) append (vertlist, i);
   DFS (dfsg, vertlist);
   DFS (tran, vertlist);

   int strong = 0, strong_;
   for (int i = 1; i <= getOrder (tran); ++i)
      if (getParent (tran, i) == NIL) ++strong;
   strong_ = strong;
   List comps[strong];
   fprintf (out, "Adjacency list representation of G:\n");
   printGraph (out, dfsg);
   fprintf (out, "\n");
   fprintf (out, "G contains %d strongly connected components:\n", strong);
   for (int i = 0; i < strong; ++i) comps[i] = newList();
   for (moveFront (vertlist); index (vertlist) >= 0; moveNext (vertlist)) {
      if (getParent (tran, get (vertlist)) == NIL) --strong;
      append (comps[strong], get (vertlist));
   }
   for (int i = 1; i <= strong_; i++) {
      fprintf (out, "Component %d: ", i);
      printList (out, comps[i - 1]);
      fprintf (out, "\n");
   }
   for (int i = 0; i < strong_; i++) freeList (&comps[i]);
   for (int i = 0; i < lines; i++) free (linebuf[i]);
   free (linebuf);
   freeGraph (&dfsg);
   freeGraph (&tran);
   freeList (&vertlist);
   fclose (out);
   fclose (read);
   fclose (count);
   return EXIT_SUCCESS;
}