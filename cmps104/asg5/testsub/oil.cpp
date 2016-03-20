#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <sstream> 

using namespace std; 

#include "oil.h"
#include "symtab.h"

int regnum = 1; //temp register number

void oilall (astree* a) {
   oilstructs();
   oilstringcons();
   oilfuncs();
   oilstatement (a);
}

void oilstatement (astree* a) {
   if (a == nullptr) return;
   switch (a->symbol) {
      case TOK_FUNCTION: break;
      case TOK_ROOT: {
         string header = "void __ocmain (void)\n{\n";
         fprintf (oilout, header.c_str());
         for (size_t i = 0; i < a->children.size(); ++i)
            oilstatement (a->children[i]);
         break;
      }
      case TOK_BLOCK: {
         for (size_t i = 0; i < a->children.size(); ++i)
            oilstatement (a->children[i]);
         break;
      }
      case TOK_WHILE: oilwhile (a); break;
      case TOK_IFELSE: oilifelse (a); break;
      case TOK_IF: oilif (a); break;
      case TOK_RETURN: /* oilreturn */ break;
      case TOK_RETURNVOID: /* oilreturnvoid */ break;
      case TOK_VARDECL: /* oilvardecl */ break;
      default: break;
   }
}

void oilexpr (astree* a) {
   if (a == nullptr) return;
   for (size_t i = 0; i < a->children.size(); ++i)
      oilexpr (a->children[i]);
   switch (a->symbol) {
      case '+': case '-': case '*': case '%': oilbinop (a); break;
      case '=': /* oilassign */ break;
      case TOK_EQ:
      case TOK_NE:
      case TOK_LT:
      case TOK_GT:
      case TOK_GE:
      case '!': oilnot (a); break;
      case TOK_ORD:
      case TOK_CHR:
      case TOK_NEG:
      case TOK_POS:
      case TOK_NEW: oilnew (a); break;/* oilnew */
      case TOK_NEWARRAY: oilnewarr (a); break; /* oilnewarr */
      case TOK_NEWSTRING: oilnewstr (a); break; /* oilnewstr */
      case TOK_INDEX:
      case '.':
      case TOK_CALL:
      default: break;
   }
}

char regcat (astree* a) {
   char c = 's';
   if (a->attributes[ATTR_struct] or a->attributes[ATTR_string] or
       a->attributes[ATTR_array]) c = 'p';
   if (a->attributes[ATTR_int]) c = 'i';
   if (a->attributes[ATTR_char]) c = 'c';
   if (a->attributes[ATTR_bool]) c = 'b';
   if (a->attributes[ATTR_array]) c = 'a';
   if (a->attributes[ATTR_vaddr]) c = 'a';
   return c;
}

void oilop (astree* a) {
   cout << a << endl;
   ostringstream out;
   char cat = regcat (a);
   if (a->attributes[ATTR_vreg] or (a->attributes[ATTR_string] and 
                                    a->attributes[ATTR_const])) 
      out << cat << regnum;
   else if (a->attributes[ATTR_vaddr])
      out << "*" << cat << regnum;
   else if (a->attributes[ATTR_variable]) 
      out << "_" << a->lexinfo->c_str();
   else if (a->attributes[ATTR_const]) 
      out << a->lexinfo->c_str();
   fprintf (oilout, out.str().c_str());
}

void oilnew (astree* a) {
   astree* t = a->children[1];
   ostringstream emit;
   emit << "      " << "struct " << t->lexinfo->c_str() << "* p"
        << regnum++ << " = xalloc (1, sizeof (struct "
        << t->lexinfo->c_str() << "));\n";
   fprintf (oilout, emit.str().c_str()); 
}

void oilnewarr (astree* a) {
   cout << a << endl;
   string type = "";
   ostringstream emit, emit_;
   astree* base = a->children[0];
   astree* opnd = a->children[1];
   emit << "      ";
   switch (base->symbol) {
      case TOK_STRING: emit << "char*"; type += "char*"; break;
      case TOK_TYPEID: emit << "struct " << base->structname->c_str() 
                            << "*"; 
                       type += "struct ";
                       type += *(base->structname); type += "*"; break;
      case TOK_INT: emit << "int"; type += "int"; break;
      case TOK_BOOL: emit << "bool"; type += "bool"; break;
      case TOK_CHAR: emit << "char"; type += "char"; break;
      default: break;
   }
   emit << "p" << regnum++ << " = xalloc (";
   fprintf (oilout, emit.str().c_str());
   oilop (opnd);
   emit_ << ", sizeof (" << type << "));\n";
   fprintf (oilout, emit_.str().c_str());
}

void oilnewstr (astree* a) {
   cout << a << endl;
   ostringstream first, second;
   astree* opnd = a->children[0];
   first << "      " << "char* p" << regnum++ << " = xalloc (";
   fprintf (oilout, first.str().c_str());
   oilop (opnd);
   second << ", sizeof (char));\n";
   fprintf (oilout, second.str().c_str());
}

void oilbinop (astree* a) {
   ostringstream int_;
   astree* l = a->children[0];
   astree* r = a->children[1];
   fprintf (oilout, "      ");
   int_ << "int i" << regnum++;
   fprintf (oilout, int_.str().c_str());
   oilop (l);
   fprintf (oilout, "%s ", a->lexinfo->c_str());
   oilop (r);
   fprintf (oilout, ";\n");
}

void oilnot (astree* a) {
   ostringstream emit, emit_; 
   astree* operand = a->children[0];
   emit << "      " << "int i" << regnum++ << " = (int)";
   fprintf (oilout, emit.str().c_str());
   oilop (operand);
   emit_ << ";\n";
   fprintf (oilout, emit_.str().c_str());
} 

void oilunop (astree* a) {
   cout << a << endl;
}

void oildecls (astree* a) {
   cout << a << endl;
}

/* code to emit string constants */
void oilstr (astree* a) {
   cout << a << endl;
   for (auto i = stringqueue.front(); not stringqueue.empty(); 
                                          stringqueue.pop()) {
      ostringstream emit;
      astree* stringtree = i;
      string stringinfo = *(stringtree->lexinfo);
      emit << "char* s" << regnum++ << " = " << stringinfo << ";\n";
      fprintf (oilout, emit.str().c_str());
   }
}

/* code to emit structs */
void oilstructs() {
   ostringstream out, fields;
   for (auto i = structable->begin(); i != structable->end();
        ++ i) {
      string structname = *i->first;
      out << "struct s_" << structname << "{\n";
      fprintf (oilout, out.str().c_str());
      for (auto j = i->second->fields->begin(); 
                j != i->second->fields->end(); ++j) {
         string name = getname (j->second);
         fields << name << " f_" << structname << "_"
                << j->first->c_str();
         fprintf (oilout, fields.str().c_str());
      }
      fprintf (oilout, "}:\n");
   }
}

/* code to emit functions with parameters */
void oilfuncs() {
   ostringstream out;
   for (auto i = fnqueue.front(); not fnqueue.empty(); fnqueue.pop()) {
      symbol* currsym = i;
      out << getname (currsym) << " __" << currsym->lexinfo->c_str() 
          << " (";
      fprintf (oilout, out.str().c_str());
      vector<symbol*>* params = currsym->parameters;
      for (auto par = params->begin(); par != params->end(); ++par) {
         ostringstream pout;
         pout << "\n      " << getname (i) << " _" << currsym->blocknr
              << "_" << i->lexinfo->c_str();
         fprintf (oilout, pout.str().c_str());
         if (fnqueue.size() > 1) fprintf (oilout, ",");
      }
   }
}

/* code to emit while */ 
void oilwhile (astree* a) {
   cout << a << endl;
   ostringstream whead, ifhead, gotob, gotow, brk;
   whead << "while_" << a->filenr << "_" << a->linenr << "_"
         << a->offset << ":;\n";
   oilexpr (a->children[0]);
   fprintf (oilout, whead.str().c_str());
   ifhead << "if (!";
   fprintf (oilout, ifhead.str().c_str());
   oilop (a->children[1]);
   gotob << ") goto break_" << a->filenr << "_" << a->linenr << "_"
         << a->offset << ";\n";
   fprintf (oilout, gotob.str().c_str());
   oilstatement (a->children[1]);
   brk << "break_" << a->filenr << "_" << a->linenr << "_" 
       << a->offset << ":\n";
   fprintf (oilout, brk.str().c_str());
}

/* code to emit if */
void oilif (astree* a) {
   cout << a << endl;
   ostringstream ifhead, gotof, fi;
   ifhead << "if (!";
   fprintf (oilout, ifhead.str().c_str());
   oilop (a->children[0]);
   gotof << ") goto fi_" << a->filenr << "_" << a->linenr << "_"
         << a->offset << ";\n";
   fprintf (oilout, gotof.str().c_str());
   // oil st root child 1
   fi << "fi_" << a->filenr << "_" << a->linenr << "_" 
      << a->offset << ":;\n";
   fprintf (oilout, fi.str().c_str());
}

/* code to emit ifelse */
void oilifelse (astree* a) {
   cout << a << endl;
   ostringstream ifhead, gotoe, gotof, elsehead, fi;
   ifhead << "if (!";
   fprintf (oilout, ifhead.str().c_str());
   // oil op root child 0
   gotoe << ") goto else_" << a->filenr << "_" << a->linenr
         << "_" << a->offset << ";\n";
   fprintf (oilout, gotoe.str().c_str());
   // oil st root child 1
   gotof << "goto fi_" << a->filenr << "_" << a->linenr << "_"
         << a->offset << ";\n";
   fprintf (oilout, gotof.str().c_str());
   elsehead << "else_" << a->filenr << "_" << a->linenr << "_"
         << a->offset << ":;\n";
   fprintf (oilout, elsehead.str().c_str());
   fi << "fi_" << a->filenr << "_" << a->linenr << "_" 
      << a->offset << ":;\n";
   fprintf (oilout, fi.str().c_str());
}

string getname (astree* a) {
   string symbolname = "";
   if (a->attributes[ATTR_void]) {
      symbolname += "void";
      return symbolname;
   }
   if (a->attributes[ATTR_bool] || a->attributes[ATTR_char])
      symbolname += "char";
   else if (a->attributes[ATTR_int])
      symbolname += "int";
   else if (a->attributes[ATTR_string])
      symbolname += "char*";
   else if (a->attributes[ATTR_struct]) {
      symbolname += "struct s_";
      if (a->structname != nullptr) symbolname += *a->structname;
      symbolname += "*";
   }
   if (a->attributes[ATTR_array]) symbolname += "*";
   return symbolname;
}

string getname (symbol* a) {
   string symbolname = "";
   if (a->attributes[ATTR_void]) {
      symbolname += "void";
      return symbolname;
   }
   if (a->attributes[ATTR_bool] || a->attributes[ATTR_char])
      symbolname += "char";
   else if (a->attributes[ATTR_int])
      symbolname += "int";
   else if (a->attributes[ATTR_string])
      symbolname += "char*";
   else if (a->attributes[ATTR_struct]) {
      symbolname += "struct s_";
      if (a->structname != nullptr) symbolname += *a->structname;
      symbolname += "*";
   }
   if (a->attributes[ATTR_array]) symbolname += "*";
   return symbolname;
}
