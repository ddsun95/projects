// David Sun and Curtis Liew
// ddsun@ucsc.edu
// cliew@ucsc.edu

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cstring>

using namespace std;

#include "auxlib.h"
#include "stringset.h"
#include "lyutils.h"
#include "astree.h"
#include "symtab.h"

string cppopt = "";
const string cpp = "/usr/bin/cpp";
const size_t LINESIZE = 1024;
FILE* tokout;
FILE* symout;


vector<string> split (const string& line, const string& delimiters) {
   vector<string> words;
   size_t end = 0;
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   return words;
}

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

//function to scan options
void scanopts (int argc, char** argv) {
   opterr = yy_flex_debug = yydebug = 0;
   for (;;) {
      int option = getopt (argc, argv, "ly@:D:");
      if (option == EOF) break;
      switch (option) {
         case 'l': yy_flex_debug = 1;                break;
         case 'y': yydebug = 1;                      break;
         case '@': set_debugflags (optarg);          break;
         case 'D': cppopt += "-D"; cppopt += optarg; break;
         default: 
            cerr << "-" << (char) optopt << ": invalid option" << endl;
            set_exitstatus (EXIT_FAILURE);
            break;
      }
   }
   if (optind >= argc) {
      cerr << "usage: oc [-ly] [-@ flag...] [-D string] program.oc"
           << endl;
      set_exitstatus (EXIT_FAILURE);
      exit (get_exitstatus());
   }
}

void yyinopen (string command) {
   yyin = popen (command.c_str(), "r");
   if (yyin == NULL) {
      syserrprintf (command.c_str());
      exit (get_exitstatus());
   } 
}

int main (int argc, char** argv) {
   set_execname (basename (argv[0]));
   scanopts (argc, argv);
   char* filename = argv[optind];
   string command = cpp + " " + cppopt;
   if (cppopt.length() != 0) command += " ";
   command += filename;
   yyinopen (command);
   string ocfile = split (filename, ".").at (0);
   string strfile = ocfile + ".str";
   string tokfile = ocfile + ".tok";
   string astfile = ocfile + ".ast";
   string symfile = ocfile + ".sym";
   FILE* strout = fopen (strfile.c_str(), "w");
   tokout       = fopen (tokfile.c_str(), "w");
   FILE* astout = fopen (astfile.c_str(), "w"); 
   symout       = fopen (symfile.c_str(), "w");
   yyparse(); 
   dump_stringset (strout);
   dump_astree (astout, yyparse_astree);
   typecheck (yyparse_astree);
   pclose (yyin);
   fclose (strout);
   fclose (tokout);
   fclose (astout);
   fclose (symout);
   free_ast (yyparse_astree);
   yylex_destroy();
   return (get_exitstatus());
}
