#ifndef __OIL_H__
#define __OIL_H__

#include <string>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <queue>

using namespace std;

#include "symtab.h"
#include "astree.h"

void oilall (astree*);
void oilstatement (astree*);
void oilexpr (astree*);
char regcat (astree*);
void oilop (astree*);
void oilnew (astree*);
void oilnewarr (astree*);
void oilnewstr (astree*);
void oilbinop (astree*);
void oilnot (astree*);
void oilunop (astree*);
void oilstr (astree*);
void oilstructs (astree*);
void oilfuncs (astree*);
void oilwhile (astree*);
void oilif (astree*);
void oilifelse (astree*);
string getname (astree*);
string getname (symbol*);

#endif