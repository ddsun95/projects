// David Sun and Curtis Liew
// ddsun@ucsc.edu
// cliew@ucsc.edu

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> 

#include "astree.h"
#include "stringset.h"
#include "lyutils.h"

using namespace std;

astree::astree (int symbol, int filenr, int linenr,
                int offset, const char* clexinfo):
        symbol (symbol), filenr (filenr), linenr (linenr),
        offset (offset), lexinfo (intern_stringset (clexinfo)) {
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           (void*) this, filenr, linenr, offset,
           get_yytname (symbol), lexinfo->c_str());
}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child, child->lexinfo->c_str());
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt3 (astree* root, astree* one, 
                              astree* two, astree* three) {
   adopt1 (root, one);
   adopt1 (root, two);
   adopt1 (root, three);
   return root; 
}

astree* adopt4 (astree* root, astree* one, astree* two, 
                              astree* three, astree* four) {
   adopt3 (root, one, two, three);
   adopt1 (root, four);
   return root; 
}

astree* adopt3change (astree* root, int symbol_, astree* one, 
                                    astree* two, astree* three) {
   root->symbol = symbol_;
   adopt1 (root, one);
   adopt1 (root, two);
   adopt1 (root, three);
   return root;
}

astree* sch (astree* tree, int symbol_) {
   tree->symbol = symbol_;
   return tree;
}

astree* adoptch (astree* root, astree* that) {
   if (root->symbol != that->symbol) {
      adopt1 (root, that);
   } else {
      for (size_t i = 0; i < that->children.size(); ++i)
         adopt1 (root, that->children.at (i));
   }
   return root; 
}

astree* prot_or_func (astree* root, astree* identdecl, 
                      astree* fnargs, astree* block) {
   adopt1 (root, identdecl);
   adopt1 (root, fnargs);
   if (*block->lexinfo == ";") {
      root->symbol = TOK_PROTOTYPE;
      free_ast (block);
   } else {
      root->symbol = TOK_FUNCTION;
      adopt1 (root, block);
   }
   return root;
}

astree* porf (astree* idecl, astree* fnparen, astree* block) {
   astree* root = new astree (*block->lexinfo == ";" ? 
                              TOK_PROTOTYPE : TOK_FUNCTION, 
                              idecl->filenr, idecl->linenr, 
                              idecl->offset, "");
   adopt1 (root, idecl);
   adopt1 (root, fnparen);
   if (root->symbol == TOK_PROTOTYPE) free_ast (block);
                                 else adopt1 (root, block);
   return root; 
}

static void astdump (FILE* outfile, astree* node, int depth) {
   for (int i = 0; i < depth; ++i) fprintf (outfile, "| ");
   const char* tname = get_yytname (node->symbol);
   if (strstr (tname, "TOK_") == tname) tname += 4;
   fprintf (outfile, "%s \"%s\" %ld.%ld.%ld", 
            tname, node->lexinfo->c_str(), node->filenr, 
            node->linenr, node->offset); 
}

static void dump_node (FILE* outfile, astree* node) {
   fprintf (outfile, "%4ld %3ld.%03ld %-3d %-15s (%s)",
            node->filenr, node->linenr, node->offset, 
            node->symbol, get_yytname (node->symbol),
            node->lexinfo->c_str());
   bool need_space = false;
   for (size_t child = 0; child < node->children.size(); ++child) {
      if (need_space) fprintf (outfile, " ");
      need_space = true;
      fprintf (outfile, "%p", node->children.at(child));
   }
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   astdump (outfile, root, depth);
   fprintf (outfile, "\n");
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_astree_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}


void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}

void free_ast3 (astree* one, astree* two, astree* three) {
   free_ast2 (one, two);
   free_ast (three);
}

RCSC("$Id: astree.cpp,v 1.6 2015-04-09 19:31:47-07 - - $")
