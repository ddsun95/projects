// David Sun and Curtis Liew
// ddsun@ucsc.edu
// cliew@ucsc.edu

#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <string>
#include <vector>
using namespace std;

#include "auxlib.h"
#include "symtab.h"

extern FILE* tokout;
extern FILE* symout; 

struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   size_t blocknr;
   attr_bitset attributes;
   const string* structname;
   symbol_table* fields;
   const string* lexinfo;    // pointer to lexical information
   vector<astree*> children; // children of this n-way node
   astree (int symbol, int filenr, int linenr,
           int offset, const char* clexinfo);
};
// Append one child to the vector of children.
astree* adopt1 (astree* root, astree* child);
// Append two children to the vector of children.
astree* adopt2 (astree* root, astree* left, astree* right);
astree* adopt3 (astree*, astree*, astree*, astree*);
astree* adopt4 (astree*, astree*, astree*, astree*, astree*);
astree* adoptch (astree*, astree*);
astree* sch (astree*, int);
astree* prot_or_func (astree*, astree*, astree*, astree*);
astree* porf (astree*, astree*, astree*);
// Dump an astree to a FILE.
void dump_astree (FILE* outfile, astree* root);

// Debug print an astree.
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);

// Recursively free an astree.
void free_ast (astree* tree);

// Recursively free two astrees.
void free_ast2 (astree* tree1, astree* tree2);

void free_ast3 (astree*, astree*, astree*);

RCSH("$Id: astree.h,v 1.4 2015-04-09 19:31:47-07 - - $")
#endif
