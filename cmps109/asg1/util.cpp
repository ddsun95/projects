// $Id: util.cpp,v 1.10 2014-06-11 13:34:25-07 - - $

#include <cstdlib>
#include <unistd.h>

using namespace std;

#include "util.h"
#include "debug.h"

yshell_exn::yshell_exn (const string& what): runtime_error (what) {
}

int exit_status::status = EXIT_SUCCESS;
static string execname_string;
static string pathname = "";

void exit_status::set (int new_status) {
   status = new_status;
}

int exit_status::get() {
   return status;
}

void execname (const string& name) {
   execname_string =  name.substr (name.rfind ('/') + 1);
   DEBUGF ('u', execname_string);
}

void path_name_set(const string& path_name){
   pathname = path_name;
}

string& execname() {
   return execname_string;
}

bool want_echo() {
   constexpr int CIN_FD {0};
   constexpr int COUT_FD {1};
   bool cin_is_not_a_tty = not isatty (CIN_FD);
   bool cout_is_not_a_tty = not isatty (COUT_FD);
   DEBUGF ('u', "cin_is_not_a_tty = " << cin_is_not_a_tty
          << ", cout_is_not_a_tty = " << cout_is_not_a_tty);
   return cin_is_not_a_tty or cout_is_not_a_tty;
}


wordvec split (const string& line, const string& delimiters) {
   wordvec words;
   size_t end = 0;

   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output wordvec.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   DEBUGF ('u', words);
   return words;
}



string cat_is_directory(){
   exit_status::set (EXIT_FAILURE);
   return pathname + ": Is a Directory";
}

string cat_not_exist(){
   exit_status::set (EXIT_FAILURE);
   return pathname + ": File does not exist";
}

string isDirectory(){
   exit_status::set (EXIT_FAILURE);
   return pathname + ": Is a Directory";
}

string isFile(){
   exit_status::set (EXIT_FAILURE);
   return pathname + ": Is a Plain File";
}

string notExist(){
   exit_status::set (EXIT_FAILURE);
   return pathname + ": File does not exist";
}

ostream& complain() {
   exit_status::set (EXIT_FAILURE);
   cerr << execname() << ": ";
   return cerr;
}

