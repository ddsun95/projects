// $Id: inode.h,v 1.13 2014-06-12 18:10:25-07 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
using namespace std;

#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {PLAIN_INODE, DIR_INODE};
class inode;
class file_base;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using file_base_ptr = shared_ptr<file_base>;
using plain_file_ptr = shared_ptr<plain_file>;
using directory_ptr = shared_ptr<directory>;

//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//

class inode_state {
   friend class inode;
   friend ostream& operator<< (ostream& out, const inode_state&);
   private:
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};
      string prompt {"% "};
   public:
      inode_state();
      inode_ptr get_root();                     //returns the root
      inode_ptr get_cwd();                      //returns the cwd
      inode_ptr get_working_ptr(const wordvec& navpath);
      file_base_ptr get_contents();
      void print_entities();
      void set_prompt         (const string& new_prompt);
      void make_file          (const wordvec& entry);
      void make_directory     (const wordvec& entry);
      void function_cd        (const wordvec& entry);
      void final_make_file    (const wordvec& navpath, 
                               const wordvec& insertion);
      wordvec function_pwd    (wordvec vec, inode_ptr ptr);
      void function_rm        (const inode_ptr cd, 
                               const string& filename);
      void function_final_rm  (const string& filename, 
                               const wordvec& entry);
      void fn_pwd();
      void final_func_cat     (const wordvec& entry);
      void final_mkdir        (const wordvec& entry);
      void function_list_r    (inode_ptr iptr);
      void function_list      (inode_ptr curr_ptr);                   
      void list_helper        (inode_ptr curr_ptr);
      void init();
      void final_cd           (const wordvec& entry);
      void cat                (const wordvec& pathname);

      void test               ();
      void function_rmr       (const inode_ptr cd,
                               const string& filename);
      void function_final_rmr (const string& filename,
                               const wordvec& entry);

      void endprog            ();
      void setCwd             (inode_ptr ptr);
      string get_prompt();
};


//
// class inode -
//
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//    

class inode {
   friend class inode_state;
   private:
      static int next_inode_nr;
      int inode_nr;
      inode_t type;
      file_base_ptr contents;
   public:
      file_base_ptr get_contents();
      inode (inode_t init_type);
      int get_inode_nr() const;
};

//
// class file_base -
//
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.
//

class file_base {
   protected:
      file_base () = default;
      file_base (const file_base&) = default;
      file_base (file_base&&) = default;
      file_base& operator= (const file_base&) = default;
      file_base& operator= (file_base&&) = default;
      virtual ~file_base () = default;
      virtual size_t size() const = 0;
   public:
      friend plain_file_ptr plain_file_ptr_of (file_base_ptr);
      friend directory_ptr directory_ptr_of (file_base_ptr);
};


//
// class plain_file -
//
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
//    Throws an yshell_exn for a directory.
// writefile -
//    Replaces the contents of a file with new contents.
//    Throws an yshell_exn for a directory.
//

class plain_file: public file_base {
   private:
      wordvec data;
   public:
      size_t size() const override;
      const wordvec& readfile() const;
      void writefile (const wordvec& newdata);
};

//
// class directory -
//
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an yshell_exn if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public file_base {
   private:
      map<string,inode_ptr> dirents;
   public:
      size_t size() const override;
      void print_ents();
      void init_root(inode_ptr ptr);
      map<string,inode_ptr> get_map();
      string get_name_of_self();
      void cat             (const string& pathname);
      void remove          (const string& filename);
      void rmr             (const string& filename);
      void add_pointers    (inode_ptr parent, inode_ptr itself);
      void cleardirents    ();
      void remove_recursive(const string& filename);
      inode& mkdir         (const string& dirname);
      inode& mkfile        (const string& filename);
};

#endif

