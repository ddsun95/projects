#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum { ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
         ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
         ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param,
         ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr,
         ATTR_bitset_size, ATTR_prototype, 
};
using attr_bitset = bitset<ATTR_bitset_size>;

struct symbol;
struct astree;
using symbol_table = unordered_map<const string*,symbol*>;
using symbol_entry = symbol_table::value_type;

struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset;
   size_t blocknr;
   vector<symbol*>* parameters;
   const string* structname;
   symbol (astree* root);
};
void intern_symbol (symbol_table*, symbol_entry);
string outstring (symbol_entry);
void typecheck (astree*);
void enterblock (astree*);
void leaveblock();
void checkblock (astree*);
void tcproto (astree*);
void tcnewstring (astree*);
void tcstruct (astree*);
void tcintcon (astree*);
void tccharcon (astree*);
void tcstringcon (astree*);
void tcbool (astree*);
void tcnull (astree*);
void tcfield (astree*);
void tcvardecl (astree*);
void tcreturn (astree*);
void tcreturnvoid (astree*);
void tcinttok (astree*);
void tcbooltok (astree*);
void tcstrtok (astree*);
void tcchartok (astree*);
void tcvoidtok (astree*);
void tctypeid (astree*);
astree* tctype (astree*);
bool allowdecl (astree*);
symbol_entry makefields (astree*);
void typeexists (astree*);
void tctypeid (astree*, astree*);
void tcfunction (astree*);
void tcnew (astree*);
void tcnot (astree*);
void tcequals (astree*);
void setcfnode (astree*);
void tcunop (astree*);
void tcif (astree*);
void tccomp (astree*);
void tcwhile (astree*);
void tcident (astree*);
void tcbinop (astree*);
void tccall (astree*);
void tcord (astree*);
void tcchr (astree*);
void tcassign (astree*);
void tcfieldsel (astree*); 
void tcnewarray (astree*);
void tcindex (astree*);
astree* getcfnode();
bool addfields (symbol_table*, symbol_entry);
bool compatible (astree*, astree*);
bool compatible (attr_bitset, attr_bitset, astree*, symbol*);

#endif
