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

int yy_flex_debug = 0;
int yy_debug = 0;
string CPP_OPT = "";
const string CPP = "/usr/bin/cpp";
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

// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      //printf ("%s:line %d: [%s]\n", filename, linenr, buffer);
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
      //printf ("DIRECTIVE: line %d file \"%s\"\n", linenr, filename);
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         //printf ("token %d.%d: [%s]\n",
                 //linenr, tokenct, token);
         intern_stringset (token);
      }
      ++linenr;
   }
}

//function to scan options
void scanopts (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "ly@:D:");
      if (option == EOF) break;
      switch (option) {
         case 'l': 
            yy_flex_debug = 1; 
            break;

         case 'y': 
            yy_debug = 1;
            break;

         case '@': 
            set_debugflags (optarg);
            break;

         case 'D': 
            CPP_OPT += "-D"; CPP_OPT += optarg;
            break;

         default: 
            cerr << "-" << (char) optopt << ": invalid option" << endl;
            set_exitstatus (EXIT_FAILURE);
            break;
      }
   }
}

int main (int argc, char** argv) {
   set_execname (argv[0]);
   scanopts (argc, argv);
   if (optind >= argc) {
      cerr << "usage: oc [-ly] [-@ flag...] [-D string] program.oc"
           << endl;
      set_exitstatus (EXIT_FAILURE);
      return (get_exitstatus());
   }
   char* filename = argv[optind];
   string command = CPP + " " + CPP_OPT;
   if (CPP_OPT.length() != 0) command += " ";
   command += filename;
   vector<string> filevec = split (filename, ".");
   string outfile = filevec.at (0) + ".str";
   FILE* pipe = popen (command.c_str(), "r");
   FILE* out = fopen (outfile.c_str(), "w");
   if (pipe == NULL) syserrprintf (command.c_str());
   else {
      cpplines (pipe, filename);
      int pclose_rc = pclose (pipe);
      eprint_status (command.c_str(), pclose_rc);
   }
   dump_stringset (out);
   fclose (out);
   return get_exitstatus();
}
