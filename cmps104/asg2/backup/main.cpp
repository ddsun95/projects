// David Sun
// ddsun@ucsc.edu

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

string cppopt = "";
const string cpp = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

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
   FILE* strout = fopen (strfile.c_str(), "w");
   FILE* tokout = freopen (tokfile.c_str(), "w", stdout);
   while (yylex() != YYEOF);
   dump_stringset (strout);
   fclose (strout);
   fclose (tokout);
   //close yyin
}
