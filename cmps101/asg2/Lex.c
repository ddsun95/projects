// David Sun
// ddsun@ucsc.edu
// pa2 (Lex)
// Lex.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 160

int main (int argc, char* argv[]) {
   if (argc != 3) {
      printf ("usage: Lex infile outfile\n");
      exit (1);
   }
   int count = 0;
   char buf[MAX_LEN];
   FILE* cnt = fopen (argv[1], "r"); // used to count lines
   FILE* readlines = fopen (argv[1], "r"); // used to read file
   FILE* out = fopen (argv[2], "w"); // used to write to file
   if (cnt == NULL || readlines == NULL) {
      printf ("Lex: Unable to open file %s for reading\n", argv[1]);
      exit (1);
   }
   if (out == NULL) {
      printf ("Lex: Unable to open file %s for reading\n", argv[2]);
      exit (1);
   }
   while (fgets (buf, MAX_LEN, cnt) != NULL) count++; // count lines
   char** line_array = malloc (sizeof (char*) * count); // alloc array
   for (int i = 0; i < count; ++i) { // read each line into array
      char line[MAX_LEN];
      fgets (line, MAX_LEN, readlines);
      line_array[i] = malloc (sizeof (line) + 1);
      strcpy (line_array[i], line);
      strtok (line_array[i], "\n"); // remove newline character
   }
   List L = newList();
   for (int i = 0; i < count; ++i) {
      char* tmp = line_array[i];
      if (length(L) == 0) {
         append (L, i);
         continue;
      }
      moveFront (L);
      while (index(L) >= 0) {
         if (strcmp (tmp, line_array[get (L)]) <= 0) {
            insertBefore (L, i);
            break;
         }
         moveNext (L);
         if (index (L) == -1) {
            append (L, i);
            break;
         }
      }
   }
   for (moveFront (L); index (L) >= 0; moveNext (L)) {
      fprintf (out, "%s", line_array[get (L)]);
      if (index(L) != length (L) - 1) fprintf (out, "\n");
   }
   for (int i = 0; i < count; ++i) free (line_array[i]);
   free (line_array);
   freeList (&L);
   fclose (cnt);
   fclose (out);
   fclose (readlines);
}
