// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $
// David Sun(ddsun@ucsc.edu) and Mithu Athreya(mathreya@ucsc.edu)

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

#define KEY_ONLY    1   //Constant for Key
#define KEY_EQUALS  2   //Constant for Key =
#define KEY_VALUE   3   //Constnat for Key = Value
#define EQUAL       4   //Constant for = 
#define EQ_VALUE    5   //Constant for = Value

/* Function for trim
 * Removes trailing whitespace one by one
 * Finds the first index of non-whitespace and uses substr to get rid
 * of the remaining whitespace */
string trim(const string& instr){
   string str = instr;
   while(str.size() > 0 && str.back() == ' ') str.pop_back();
   if(str.size() > 0)
      str = str.substr(str.find_first_not_of(' '), str.size());
   return str;
}

//
// Helper function for line processing 
//
void processHelper(const string& line, str_str_map& strmap, int opt){
   switch(opt){
      /* Find the key's value
       * Prints "key not found" if the key does not exist
       */
      case KEY_ONLY:{
         str_str_map::iterator target = strmap.find(line);
         if(target == strmap.end()) 
            cout << line << " : key not found " << endl;
         else
            cout << target->first << " = " << target->second << endl;
         break;
      }
      /* Removes the '=' from the string, finds the key
       * and erases it from the map
       */
      case KEY_EQUALS:{
         size_t firstEq = line.find_first_of('=');
         string delkey = trim(line.substr(0, firstEq));
         strmap.erase(strmap.find(delkey));
         break;
      }

      /* Assigns a string to the key
       * Assigns a string to the value
       * Creates a pair, and inserts the pair into the map
       */
      case KEY_VALUE:{
         size_t firstEq = line.find_first_of('=');
         string key = trim(line.substr(0, firstEq));
         string val = trim(line.substr(firstEq + 1, line.size()));
         str_str_pair strpair(key, val);
         strmap.insert(strpair);
         cout << key << " = " << val << endl;
         break;
      }

      /* Prints out all the entities in the map */
      case EQUAL:{
         str_str_map::iterator itor;
         for(itor = strmap.begin(); itor != strmap.end(); ++itor)
            cout << itor->first << " = " << itor->second << endl;
         break;
      }

      /* Prints out all the keys with the value in the map */
      case EQ_VALUE:{
         string val = trim(line.substr(1, line.size()));
         str_str_map::iterator itor;
         for(itor = strmap.begin(); itor != strmap.end(); ++itor)
            if(val == itor->second) 
               cout << itor->first << " = " << itor->second << endl;
         break;
      }

      /* In case something goes fatally wrong with line processing */
      default:{
         complain() << "Unsupported Operation" << endl;
         break;
      }
   }
}

//
// processLine does analysis on the input line
//
void processLine(const string& input, str_str_map& strmap){
   string line = trim(input);
   if(line == "" || line.at(0) == '#') return; //Ignore whitespace
   int option = -1;                            //Option number

   /* If there are no equals and the line is not empty, it is a key */
   if(line.find_first_of('=') == string::npos && line.size() >= 1)
      option = KEY_ONLY;

   /* If an = is at the back of the line and size at least 2: Key = */
   else if(line.back() == '=' && line.size() > 1) 
      option = KEY_EQUALS;

   /* If there exists an = in the middle of the string: Key = Value */
   else if(line.size() >= 3 && line.find_first_of('=') > 0 
           && line.find_first_of('=') < line.size() - 1)
      option = KEY_VALUE;

   /* If line is a single equal sign, the choose the option = */
   else if(line.size() == 1 && line == "=")
      option = EQUAL;

   /* If the beginning of the line is an = sign: = Value */
   else if(line.size() > 1 && line.at(0) == '=')
      option = EQ_VALUE;
   processHelper(line, strmap, option);
}

/* Process commands from stdin */
void inread(str_str_map& strmap){
   string nextLine;
   int lineNumber = 0;
   while(getline(cin,nextLine)){
      cout << "-: " << ++lineNumber << ": " << nextLine << endl;
      processLine(nextLine, strmap); 
   }
}

/* Process commands from a file input */
void fileread(int argc, char** argv, str_str_map& strmap){
   ifstream file;
   string nextLine;
   int lineNumber = 0;
   for(char** argp = &argv[optind]; argp != &argv[argc]; ++argp){
      file.open(trim(*argp));
      if(!file) 
         complain() << *argp << ": No such file or directory" << endl;
      else {
         while(getline(file, nextLine)){
            cout << *argp << ": " << ++lineNumber << ": " << nextLine 
                 << endl;
            processLine(nextLine, strmap);
         }
      }
      lineNumber -= lineNumber;
      file.close();
   }
}

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   bool readFromStdin = (&argv[optind] == &argv[argc]);
   str_str_map strmap;
   if(readFromStdin) inread(strmap);
                else fileread(argc, argv, strmap);
   return EXIT_SUCCESS;
}
