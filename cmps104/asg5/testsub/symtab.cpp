// David Sun (ddsun@ucsc.edu)
// Curtis Liew (cliew@ucsc.edu)

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "lyutils.h"
#include "symtab.h"
#include "astree.h"

using namespace std;

vector<symbol_table*> symstack;
symbol_table* structable = new symbol_table();
int next_block = 1;
vector<int> blocknr (1, 0);
astree* cfnode = nullptr;
string* csname = nullptr; 

symbol::symbol (astree* root) : attributes (root->attributes),
               fields (root->fields), filenr (root->filenr), 
               linenr (root->linenr), offset (root->offset), 
               blocknr (root->blocknr), parameters (nullptr), 
               structname (root->structname) {}

void typecheck (astree* root) {
   if (root == nullptr) return;
   if (root->symbol == TOK_FUNCTION) {
      astree* a = root->children[0];
      cfnode = (a->symbol == TOK_ARRAY) ? 
                         a->children[1] : a->children[0];
   }
   switch (root->symbol) {
      case TOK_ROOT: symstack.push_back (new symbol_table()); break;
      case TOK_BLOCK: checkblock (root); return;
      case TOK_VARDECL: tcvardecl (root); return; 
      case TOK_PROTOTYPE: 
         setcfnode (root); tcproto (root); setcfnode (nullptr); 
         return;
      case TOK_FUNCTION: 
         setcfnode (root); tcfunction (root); setcfnode (nullptr); 
         return;
      case TOK_STRUCT: tcstruct (root); return;
      default: break;
   }
   for (size_t i = 0; i < root->children.size(); ++i)
      typecheck (root->children.at (i));
   switch (root->symbol) {
      case TOK_BOOL:   /* handled by another function */ break;
      case TOK_CHAR:   /* handled by another function */ break;
      case TOK_INT:    /* handled by another function */ break; 
      case TOK_STRING: /* handled by another function */ break;
      case TOK_VOID:   /* handled by another function */ break; 
      case TOK_TYPEID: /* handled by another function */ break;
      case '=': tcassign (root); break;
      case TOK_RETURN: tcreturn (root); break;
      case TOK_RETURNVOID: tcreturnvoid (root); break;
      case TOK_EQ: //tcequals()
      case TOK_NE: tcequals (root); break;
      case TOK_LT: //tccomp()
      case TOK_LE: //tccomp()
      case TOK_GT: //tccomp()
      case TOK_GE: tccomp (root); break;
      case '+': //tcbinop()
      case '-': //tcbinop()
      case '*': //tcbinop()
      case '/': //tcbinop()
      case '%': tcbinop (root); break; 
      case '!': tcnot (root); break;
      case TOK_POS: //tcunop()
      case TOK_NEG: tcunop (root); break;
      case TOK_ORD: tcord (root); break;
      case TOK_CHR: tcchr (root); break;
      case '.': tcfieldsel (root); break; 
      case TOK_NEW: tcnew (root); break;
      case TOK_NEWSTRING: tcnewstring (root); break;
      case TOK_NEWARRAY: tcnewarray (root); break;
      case TOK_CALL: tccall (root); break;
      case TOK_IDENT: tcident (root); break;
      case TOK_INDEX: tcindex (root); break; 
      case TOK_INTCON: tcintcon (root); break;
      case TOK_CHARCON: tccharcon (root); break;
      case TOK_STRINGCON: tcstringcon (root); break;
      case TOK_TRUE: //tcbool();
      case TOK_FALSE: tcbool (root); break;
      case TOK_NULL: tcnull (root); break;
      case TOK_IF: tcif (root); break;
      case TOK_WHILE: tcwhile (root); break;
      case TOK_FIELD: tcfield (root); break;
      default : break;
   }
}

astree* tctype (astree* type) {
   astree* idtype = type->children[0];
   switch (type->symbol) {
      case TOK_INT: tcinttok (idtype); break;
      case TOK_BOOL: tcbooltok (idtype); break;
      case TOK_CHAR: tcchartok (idtype); break;
      case TOK_STRING: tcstrtok (idtype); break;
      case TOK_VOID: tcvoidtok (idtype); break;
      case TOK_TYPEID: tctypeid (type, idtype); break;
   }
   return idtype;
}

void intern_symbol (symbol_table* st, symbol_entry se) {
   if (st == nullptr) st = new symbol_table();
   string s = outstring (se);
   fprintf (symout, "%s", s.c_str());
   fprintf (symout, "\n");
   st->insert (se).second;
}

string outstring (symbol_entry thing) {
   symbol* sym = thing.second; 
   string padding = "";
   string write = "";
   if (sym->attributes[ATTR_field]) padding += "   ";
   for (size_t a = 0; a < sym->blocknr; ++a) padding += "   ";
   write += *thing.first; write += " ";
   write += "("; 
   write += to_string (sym->filenr); write += ".";
   write += to_string (sym->linenr); write += ".";
   write += to_string (sym->offset); write += ") ";
   if (sym->attributes[ATTR_field]) write += "field ";
   write += "{";
   if (sym->attributes[ATTR_field]) {
      if (sym->structname != nullptr) write += *sym->structname;
   } else write += to_string (sym->blocknr); 
   write += "} ";
   if (sym->attributes[ATTR_struct]) {
      write += "struct ";
      write += "\"";
      if (csname != nullptr) write += *csname;
      write += "\" ";
   }
   /* primitives */
   if (sym->attributes[ATTR_int]) write += "int ";
   if (sym->attributes[ATTR_string]) write += "string ";
   if (sym->attributes[ATTR_bool]) write += "bool ";
   if (sym->attributes[ATTR_char]) write += "char ";
   if (sym->attributes[ATTR_null]) write += "null ";
   if (sym->attributes[ATTR_array]) write += "array ";

   /* other attributes */
   if (sym->attributes[ATTR_function]) write += "function ";
   if (sym->attributes[ATTR_prototype]) write += "prototype ";
   if (sym->attributes[ATTR_variable]) write += "variable ";
   if (sym->attributes[ATTR_lval]) write += "lval ";
   if (sym->attributes[ATTR_param]) write += "param ";
   return padding + write;
}


void tcbinop (astree* binop) {
   astree* l = binop->children[0];
   astree* r = binop->children[1];
   if (not (l->attributes[ATTR_int] == r->attributes[ATTR_int]))
      cerr << "error: incompatible types for expression \'"
           << *binop->lexinfo << "\' at line " << binop->linenr
           << endl;
   else 
      binop->attributes[ATTR_vreg] = binop->attributes[ATTR_int] = 1; 
}

void tcunop (astree* unop) {
   astree* op = unop->children[0];
   if (not op->attributes[ATTR_int])
      cerr << "error: incompatible types for expression at line "
           << unop->linenr << endl;
   else
      unop->attributes[ATTR_vreg] = unop->attributes[ATTR_int] = 1;
}

void tccall (astree* call) {
   auto callname = call->children[0]->lexinfo;
   symbol_table* table = symstack[0];
   auto funcptr = table->find (callname);
   if (funcptr != table->end()) {
      auto func = funcptr->second;
      auto casize = call->children.size() - 1;
      auto fasize = func->parameters->size();
      if (casize != fasize) {
         cerr << "error: call arguments at line " << call->linenr 
              << " are different from function parameters"
              << " at line " << func->linenr << endl;
         return;
      }
      vector<symbol*>* symbvec = func->parameters;
      for (size_t i = 0; i < fasize; ++i) {
         astree* calltree = call->children[i + 1];
         symbol* funcsymb = symbvec->at (i);
         attr_bitset callbits = calltree->attributes;
         attr_bitset funcbits = funcsymb->attributes;
         if (not compatible (callbits, funcbits, calltree, funcsymb)) {
            cerr << "error: invalid arguments in function call at line"
                 << " " << call->linenr << endl;
            return;
         }
      }
      call->attributes = funcptr->second->attributes;
      call->attributes[ATTR_vreg] = 1;
      if (call->attributes[ATTR_struct]) {
         call->structname = funcptr->second->structname;
         call->fields = funcptr->second->fields;
      }
   }
}

void tcident (astree* ident) {
   bool found = false;
   for (auto st = symstack.cbegin(); st != symstack.cend(); ++st) {
      symbol_table* table = *st;
      auto itor = table->find (ident->lexinfo);
      if (itor != table->end()) {
         found = true;
         ident->attributes = itor->second->attributes;
         ident->structname = itor->second->structname;
         ident->fields = itor->second->fields;
      }
   }
   if (not found)
      cerr << "error: \"" << *ident->lexinfo 
           << "\" has not " << "been declared in this scope at line " 
           << ident->linenr << endl;
}

void checkblock (astree* block) {
   enterblock (block);
   for (size_t i = 0; i < block->children.size(); ++i)
      typecheck (block->children.at (i));
   leaveblock();
}

void enterblock (astree* block) {
   block->blocknr = blocknr.back();
   symstack.push_back (new symbol_table());
   blocknr.push_back (next_block);
   next_block++;
}

void leaveblock() {
   symstack.pop_back();
   blocknr.pop_back();
}

void tcproto (astree* func) {
   astree* returntype = func->children[0];
   astree* paramlist = func->children[1];
   astree* idtype = tctype (returntype);
   returntype->blocknr = paramlist->blocknr =
   idtype->blocknr = blocknr.back();
   idtype->attributes[ATTR_prototype] = 1; 
   symbol *protosym = new symbol (idtype);
   protosym->parameters = new vector<symbol*>;
   enterblock (paramlist);
   for (size_t i = 0; i < paramlist->children.size(); ++i) {
      astree* parameter = paramlist->children[i];
      astree* idparam = tctype (parameter);
      idparam->attributes[ATTR_variable] = 
      idparam->attributes[ATTR_lval] =
      idparam->attributes[ATTR_param] = 1;
      parameter->blocknr = idparam->blocknr = blocknr.back();
      symbol* paramsym = new symbol (idparam);
      protosym->parameters->push_back (paramsym);
   }
   leaveblock();
   auto entry = make_pair (idtype->lexinfo, protosym);
   intern_symbol (symstack.front(), entry);
   for (size_t i = 0; i < func->children.size(); ++i)
      typecheck (func->children[i]);
}

astree* getcfnode() { return cfnode; }
void setcfnode (astree* cfnode_) { cfnode = cfnode_; }

void tcfunction (astree* func) {
   astree* returntype = func->children[0];
   astree* paramlist = func->children[1];
   astree* block = func->children[2];
   astree* idtype = tctype (returntype);
   returntype->blocknr = paramlist->blocknr = block->blocknr =
   idtype->blocknr = blocknr.back();
   symbol *funcsym = new symbol (idtype);
   funcsym->parameters = new vector<symbol*>();
   bool found = structable->find (returntype->lexinfo) 
             != structable->end();
   if (not found) {
      cerr << "error: unknown return type "  
           << *returntype->lexinfo << " at line " 
           << returntype->linenr << endl;
      return;
   }
   auto entry = make_pair (idtype->lexinfo, funcsym);
   intern_symbol (symstack.front(), entry);
   enterblock (paramlist);
   for (size_t i = 0; i < paramlist->children.size(); ++i) {
      astree* parameter = paramlist->children[i];
      astree* idparam = tctype (parameter);
      idparam->attributes[ATTR_variable] = 
      idparam->attributes[ATTR_lval] =
      idparam->attributes[ATTR_param] = 1;
      parameter->blocknr = idparam->blocknr = blocknr.back();
      symbol* paramsym = new symbol (idparam);
      funcsym->parameters->push_back (paramsym);
      symbol_entry symtab = make_pair (idparam->lexinfo, paramsym);
      intern_symbol (symstack.back(), symtab);
   }
   fprintf (symout, "\n");
   for (size_t i = 0; i < block->children.size(); ++i)
      typecheck (block->children[i]);
   fprintf (symout, "\n");
   leaveblock();
}

void tcstruct (astree* structure) {
   astree* type = structure->children[0];
   structure->blocknr = type->blocknr = 0;
   type->structname = type->lexinfo;
   type->attributes[ATTR_typeid] = 1;
   type->attributes[ATTR_struct] = 1;
   csname = new string (*type->structname);
   symbol* symb = new symbol (type);
   symb->structname = type->structname;
   symb->fields = new symbol_table();
   symbol_entry structentry = make_pair (type->lexinfo, symb);
   intern_symbol (structable, structentry);
   for (size_t i = 1; i < structure->children.size(); ++i) {
      astree* field = structure->children[i];
      field->structname = type->structname;
      intern_symbol (symb->fields, makefields (field));
   } 
   fprintf (symout, "\n");
}

symbol_entry makefields (astree* field) {
   astree* datatype = tctype (field);
   datatype->structname = field->structname;
   datatype->attributes[ATTR_field] = 1;
   datatype->blocknr = field->blocknr;
   symbol* symb = new symbol (datatype);
   return make_pair (datatype->lexinfo, symb);
}

void tcvardecl (astree* vardecl) {
   astree* type = vardecl->children[0];
   astree* identifier = tctype (type);
   identifier->attributes[ATTR_lval] = 
   identifier->attributes[ATTR_variable] = 1;
   vardecl->blocknr = type->blocknr = 
   identifier->blocknr = blocknr.back();
   symbol* symb = new symbol (identifier);
   if (not allowdecl (identifier)) {
      cerr << "error: duplicate definition of variable " 
           << *identifier->lexinfo << " at line "
           << identifier->linenr  << endl;
      return;
   }
   symbol_entry value = make_pair (identifier->lexinfo, symb);
   intern_symbol (symstack.back(), value);
   typecheck (vardecl->children[1]);
   if (not compatible (identifier, vardecl->children[0])) 
      cerr << "error: incompatible types at line " << vardecl->linenr
           << endl;
}

void tctypeid (astree* type, astree* idtype) {
   bool found = structable->find (type->lexinfo) != structable->end();
   if (not found) {
      cerr << "error: unknown type " << *type->lexinfo << " at line "
           << type->linenr << endl;
   } else {
      typeexists (type);
      auto a = structable->find (type->lexinfo);
      idtype->structname = type->lexinfo;
      idtype->fields = a->second->fields;
      if (idtype->structname != nullptr) 
         csname = new string (*idtype->structname);
      idtype->attributes[ATTR_struct] = 1;
   }
}

void typeexists (astree* type_id) {
   auto lookupname = type_id->lexinfo;
   auto foundthing = structable->find (lookupname);
   bool found = foundthing != structable->end();
   if (not found) {
      symbol* symb = new symbol (type_id);
      structable->insert (make_pair (lookupname, symb));
   }
   type_id->attributes[ATTR_typeid] = 1; 
}

bool allowdecl (astree* decl) {
   symbol_table* currtable = symstack.back();
   auto found = currtable->find (decl->lexinfo);
   return (found == currtable->end());
}

void tcreturn (astree* a) {
   if (cfnode != nullptr) {
      astree* returntype = a->children[0];
      if (returntype->attributes[ATTR_null] and 
          ((not cfnode->attributes[ATTR_string]) or 
           (not cfnode->attributes[ATTR_array]) or
           (not cfnode->attributes[ATTR_struct]))) {
         cerr << "error: null return type on non-pointer " 
              << "based data type at line " << a->linenr;
         return; 
      }
      if (not compatible (a, returntype))
         cerr << "error: expected return type to be " 
              << *cfnode->lexinfo << "at line " << a->linenr;
   }
   cerr << "error: syntax error at line " << a->linenr << endl;
}

void tcnot (astree* a) {
   astree* datatype = a->children[0];
   if (datatype->attributes[ATTR_array] or not
       datatype->attributes[ATTR_bool]) 
      cerr << "error: expression incompatible " 
           << "with type bool at line " << a->linenr << endl;
   else
      a->attributes[ATTR_vreg] = a->attributes[ATTR_bool] = 1;
}

void tcreturnvoid (astree* a) {
   if (cfnode == nullptr)
      cerr << "error: missing returntype on function at line " <<
      a->linenr << endl;
   else if (not cfnode->attributes[ATTR_void])
      cerr << "error: void returntype on non void function" << endl;
}

void tcinttok (astree* ast)  { ast->attributes[ATTR_int] = 1; }

void tcbooltok (astree* ast) { ast->attributes[ATTR_bool] = 1; }

void tcstrtok (astree * ast) { ast->attributes[ATTR_string] = 1; }

void tcchartok (astree* ast) { ast->attributes[ATTR_char] = 1; }

void tcvoidtok (astree* ast) { ast->attributes[ATTR_void] = 1; }

void tcnewstring (astree* a) 
{ a->attributes[ATTR_string] = a->attributes[ATTR_vreg] = 1; }

void tcintcon (astree* a) 
{ a->attributes[ATTR_const] = a->attributes[ATTR_int] = 1; }

void tccharcon (astree* a) 
{ a->attributes[ATTR_const] = a->attributes[ATTR_char] = 1; }

void tcstringcon (astree* a) 
{ a->attributes[ATTR_const] = a->attributes[ATTR_string] = 1; }

void tcbool (astree* a) 
{ a->attributes[ATTR_const] = a->attributes[ATTR_bool] = 1; }

void tcnull (astree* a) 
{ a->attributes[ATTR_const] = a->attributes[ATTR_null] = 1; }

void tcfield (astree* a) { a->attributes[ATTR_field] = 1; }

void tcnew (astree* a) {
   astree* type = a->children[0];
   bool found = structable->find (type->lexinfo) != structable->end();
   if (not found) {
      cerr << "error: unknown type \"" << *type->lexinfo 
           << "\" at line " << type->linenr << endl;
      return;
   } else {
      a->attributes[ATTR_struct] = 1;
      a->structname = type->lexinfo;
      a->fields = structable->find (type->lexinfo)->second->fields;
   }        
}

void tcequals (astree* equal) {
   astree* left = equal->children[0];
   astree* right = equal->children[1];
   if (not compatible (left, right))
      cerr << "error: incompatible types at line " << equal->linenr
           << endl;
}

void tccomp (astree* comparator) {
   astree* l = comparator->children[0];
   astree* r = comparator->children[1];
   if (not ((l->attributes[ATTR_bool] and r->attributes[ATTR_bool]) or
            (l->attributes[ATTR_char] and r->attributes[ATTR_char]) or
            (l->attributes[ATTR_int]  and r->attributes[ATTR_int])))
      cerr << "error: invalid comparison types at line " 
           << comparator->linenr << endl;
}

void tcif (astree* if_) {
   if (not if_->children[0]->attributes[ATTR_bool])
      cerr << "error: expected boolean expression in if( ) at line "
           << if_->children[0]->linenr << endl;
}

void tcwhile (astree* while_) {
   if (not while_->children[0]->attributes[ATTR_bool])
      cerr << "error: expected boolean expression in while( ) at line "
           << while_->children[0]->linenr << endl;
}

void tcord (astree* ord) {
   astree* type = ord->children[0];
   if (not type->attributes[ATTR_char]) {
      cerr << "error: incompatible operand for ord at line " 
           << ord->linenr << endl;
      return;
   } else
      ord->attributes[ATTR_vreg] = ord->attributes[ATTR_int] = 1;
}

void tcchr (astree* chr) {
   astree* type = chr->children[0];
   if (not type->attributes[ATTR_int])
      cerr << "error: incompatible operand for chr at line "
           << chr->linenr << endl;
   else
      chr->attributes[ATTR_vreg] = chr->attributes[ATTR_char] = 1;
}

void tcassign (astree* assign) {
   astree* l = assign->children[0];
   astree* r = assign->children[1];
   if (compatible (l, r) and l->attributes[ATTR_lval]) {
      assign->attributes[ATTR_lval] = 
      assign->attributes[ATTR_vaddr] = 
      assign->attributes[ATTR_variable] = 0;
      assign->attributes[ATTR_vreg] = 1; 
   } else if (not compatible (l, r)) {
      cerr << "error: incompatible types at line " << assign->linenr
           << endl;
   } else if (not l->attributes[ATTR_lval]) {
      cerr << "error: assignment to non lval token at line "
           << assign->linenr << endl;
   }
}

void tcfieldsel (astree* select) {
   //cout << select << endl;
   astree* structype = select->children[0];
   astree* selectype = nullptr;
   if (not structype->attributes[ATTR_struct]) {
      cerr << "error: non-struct type accessing field at line "
           << select->linenr << endl;
      return;
   }
   if (select->children.size() > 1) selectype = select->children[1];
   if (select == nullptr) {
      cerr << "error: no specified field type at line "
           << select->linenr << endl;
      return;
   } 
   auto find = structype->fields->find (selectype->lexinfo);
   if (find == structype->fields->end()) {
      cerr << "error: cannot find field type " << *selectype->lexinfo
           << " at line " << select->linenr << endl; 
      return;
   }
   auto field = find->second;
   select->attributes = field->attributes;
   selectype->attributes = field->attributes;
   select->attributes[ATTR_vreg] = select->attributes[ATTR_lval] = 1;
   select->attributes[ATTR_field] = 0;
   if (select->attributes[ATTR_struct]) {
      select->structname = field->structname;
      select->fields = field->fields;
   }
   if (selectype->attributes[ATTR_struct]) {
      selectype->structname = field->structname;
      selectype->fields = field->fields;
   } 
}

void tcnewarray (astree* array) {
   array->attributes[ATTR_array] = array->attributes[ATTR_vreg] = 1;
   astree* basetype = array->children[0];
   switch (basetype->symbol) {
      case TOK_INT: tcinttok (basetype); break;
      case TOK_BOOL: tcbooltok (basetype); break;
      case TOK_CHAR: tcchartok (basetype); break;
      case TOK_STRING: tcstrtok (basetype); break;
      case TOK_VOID: tcvoidtok (basetype); break;
      case TOK_TYPEID: tctypeid (array, basetype); break;
   }
}

void tcindex (astree* index) {
   astree* basetype = index->children[0];
   attr_bitset attrtype = basetype->attributes;
   if (attrtype[ATTR_bool]) index->attributes[ATTR_bool] = 1;
   if (attrtype[ATTR_int]) index->attributes[ATTR_int] = 1;
   if (attrtype[ATTR_char]) index->attributes[ATTR_char] = 1;
   if (attrtype[ATTR_string] and attrtype[ATTR_array])
      index->attributes[ATTR_string] = 1;
   else 
      index->attributes[ATTR_char] = 1;
   if (attrtype[ATTR_struct]) {
      index->attributes = basetype->attributes;
      index->fields = basetype->fields;
      index->structname = basetype->structname;
   }
   index->attributes[ATTR_vaddr] = index->attributes[ATTR_lval] = 1;
}

bool compatible (astree* one, astree* two) {
   attr_bitset one_ = one->attributes;
   attr_bitset two_ = two->attributes;
   if (one_[ATTR_null] == two_[ATTR_null]) return true;
   if (one_[ATTR_null] == two_[ATTR_string]) return true;
   if (one_[ATTR_null] == two_[ATTR_struct]) return true;
   if (one_[ATTR_null] == two_[ATTR_array]) return true;
   if (two_[ATTR_null] == one_[ATTR_string]) return true;
   if (two_[ATTR_null] == one_[ATTR_struct]) return true;
   if (two_[ATTR_null] == one_[ATTR_array]) return true;
   if (one_[ATTR_array] != two_[ATTR_array]) return false;
   if (two_[ATTR_array] != one_[ATTR_array]) return false;
   if (one_[ATTR_int] == two_[ATTR_int]) return true;
   if (one_[ATTR_char] == two_[ATTR_char]) return true; //char 
   if (one_[ATTR_string] == two_[ATTR_string]) return true; //string
   if (one_[ATTR_bool] == two_[ATTR_bool]) return true; //bool
   if (one_[ATTR_struct] == two_[ATTR_struct] and 
       one->structname == two->structname) return true; //struct
   return false;
}

bool compatible (attr_bitset one_, attr_bitset two_, 
                                       astree* tree, symbol* symb) {
   if (one_[ATTR_null] == two_[ATTR_null]) return true; 
   if (one_[ATTR_null] == two_[ATTR_string]) return true;
   if (one_[ATTR_null] == two_[ATTR_struct]) return true;
   if (one_[ATTR_null] == two_[ATTR_array]) return true;
   if (two_[ATTR_null] == one_[ATTR_string]) return true;
   if (two_[ATTR_null] == one_[ATTR_struct]) return true;
   if (two_[ATTR_null] == one_[ATTR_array]) return true;
   if (one_[ATTR_array] != two_[ATTR_array]) return false;
   if (two_[ATTR_array] != one_[ATTR_array]) return false;
   if (one_[ATTR_int] == two_[ATTR_int]) return true;
   if (one_[ATTR_char] == two_[ATTR_char]) return true; //char 
   if (one_[ATTR_string] == two_[ATTR_string]) return true; //string
   if (one_[ATTR_bool] == two_[ATTR_bool]) return true; //bool
   if (one_[ATTR_struct] == two_[ATTR_struct] and 
       tree->structname == symb->structname) return true; //struct
   return false;
}
