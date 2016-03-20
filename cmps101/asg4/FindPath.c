// David Sun
// ddsun@ucsc.edu
// pa4 (FindPath)

#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
#include "Graph.h"
#include "List.h"

#define MAXLINE 160

void process (int lim, FILE* out, Graph graph, char** line_arr) {
   int fline = 1;
   for (int i = 1;; ++i, ++fline) {
      char* one, *two;
      one = __strtok_r (line_arr[i], " ", &two);
      if (atoi (one) == 0 && atoi (two) == 0) break;
      addEdge (graph, atoi (one), atoi (two));
   }
   printGraph (out, graph);
   fprintf (out, "\n");
   ++fline;
   for (; fline < lim; ++fline) {
      char* fst, *sec;
      fst = __strtok_r (line_arr[fline], " ", &sec);
      if (atoi (fst) == 0 && atoi (sec) == 0) break;
      int source = atoi (fst), dest = atoi (sec);
      BFS (graph, source);
      int distance = getDist (graph, dest);
      fprintf (out, "The distance from %d to %d is ", source, dest);
      if (distance == -2) fprintf (out, "infinity\n");
                     else fprintf (out, "%d\n", distance);
      List paths = newList();
      getPath (paths, graph, dest);
      if (distance == -2) fprintf (out, "No %d-%d path exists\n", source, dest);
      else {
         fprintf (out, "A shortest %d-%d path is: ", source, dest);
         printList (out, paths);
         fprintf (out, "\n");
      }
      fprintf (out, "\n");
      freeList (&paths);
   }
}

int main (int argc, char** argv) {
   if (argc != 3) {
      printf ("usage: FindPath infile outfile\n");
      exit (EXIT_FAILURE);
   }
   int lines = 0;
   char buf[MAXLINE];
   FILE* count = fopen (argv[1], "r");
   FILE* read  = fopen (argv[1], "r");
   FILE* write = fopen (argv[2], "w");
   if (count == NULL || read == NULL) {
      printf ("FindPath: Unable to open %s for reading\n", argv[1]);
      exit (EXIT_FAILURE);
   }
   if (write == NULL) {
      printf ("FindPath: Unable to open %s for writing\n", argv[2]);
      exit (EXIT_FAILURE);
   }
   while (fgets (buf, MAXLINE, count) != NULL) ++lines;
   char** linestr = malloc (sizeof (char*) * lines);
   for (int i = 0; i < lines; ++i) {
      char line[MAXLINE];
      fgets (line, MAXLINE, read);
      linestr[i] = malloc (sizeof (line) + 1);
      strcpy (linestr[i], line);
      strtok (linestr[i], "\n"); 
   }
   int vertices = atoi (linestr[0]);
   Graph graph = newGraph (vertices);
   process (lines, write, graph, linestr);
   for (int i = 0; i < lines; ++i) free (linestr[i]);
   free (linestr);
   freeGraph (&graph);
   fclose (count);
   fclose (read);
   fclose (write);
}
