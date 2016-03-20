// $Id: inode.cpp,v 1.12 2014-07-03 13:29:57-07 - - $

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case PLAIN_INODE:
           contents = make_shared<plain_file>();
           break;
      case DIR_INODE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}
file_base_ptr inode::get_contents(){
   return contents;
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

plain_file_ptr plain_file_ptr_of (file_base_ptr ptr) {
   plain_file_ptr pfptr = dynamic_pointer_cast<plain_file> (ptr);
   if (pfptr == nullptr) throw invalid_argument ("plain_file_ptr_of");
   return pfptr;
}

directory_ptr directory_ptr_of (file_base_ptr ptr) {
   directory_ptr dirptr = dynamic_pointer_cast<directory> (ptr);
   if (dirptr == nullptr) throw invalid_argument ("directory_ptr_of");
   return dirptr;
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   string tmp = "";
   if(data.size() != 0) {
      for(const auto& wrd : data) tmp += wrd + " ";
      size = tmp.length() - 1;
   }
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data = words;
}

/*******************************************************/
/* This function returns the dereference to a directory */
inode& directory::mkdir(const string& dirname){
   inode_ptr file_node = make_shared<inode>(DIR_INODE);
   dirents.insert(pair<string,inode_ptr>(dirname, file_node));
   return *file_node;
}

/* This function returns the dereference to a plain file */
inode& directory::mkfile(const string& filename){
   inode_ptr plain_node = make_shared<inode>(PLAIN_INODE);
   dirents.insert(pair<string,inode_ptr>(filename, plain_node));
   return *plain_node;
}

map<string,inode_ptr> directory::get_map(){
   return dirents;
}

// Initializes the inital program with the root of the tree
void directory::init_root(inode_ptr ptr){
   dirents.insert(pair<string,inode_ptr>(".", ptr));
   dirents.insert(pair<string,inode_ptr>("..", ptr));
}

/* This method will add (.) to refer to itself and (..) to
 * refer to the directory's parent.
 */
void directory::add_pointers(inode_ptr parent, inode_ptr itself){
   dirents.insert(pair<string,inode_ptr>(".",  itself));
   dirents.insert(pair<string,inode_ptr>("..", parent));
}


size_t directory::size() const {
   size_t size {0};
   size = dirents.size();
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   auto mypair = dirents.find(filename);
   if(mypair == dirents.end()){
      complain() << "rm: " << notExist() << endl;
      return;
   }
   inode_ptr tptr = dirents.find(filename)->second;
   try {
      plain_file_ptr fileptr = plain_file_ptr_of(tptr->get_contents());
      dirents.find(filename)->second = nullptr;
      dirents.erase(filename);
   } catch (invalid_argument&){
      directory_ptr dirptr = directory_ptr_of(tptr->get_contents());
      if(dirptr->size() > 2) {
         complain() << "rm: directory is not empty" << endl;
         return;
      } else {
         dirents.erase(filename);
         dirptr.reset();
         tptr = nullptr;
      }
   }
}

void directory::cleardirents(){
   if(dirents.size() == 2){
      dirents.erase("..");
      dirents.erase(".");
      return;
   }
   for(auto itor = dirents.begin(); itor != dirents.end(); ++itor){
      inode_ptr file_or_dir = dirents.find(itor->first)->second;
      if(itor->first != "." and itor->first != ".."){
         try{
            directory_ptr dirptr = 
            directory_ptr_of(file_or_dir->get_contents());
            dirptr->cleardirents();
            dirents.find(itor->first)->second = nullptr;
         } catch(invalid_argument&){
            dirents.find(itor->first)->second = nullptr;
         }
      }
   }
   dirents.erase("..");
   dirents.erase(".");
}

void directory::remove_recursive(const string& filename){
   auto strpair = dirents.find(filename);
   if(strpair == dirents.end()){
      complain() << "rmr: " << notExist() << endl;
      return;
   }
   inode_ptr dirorplain = strpair->second;
   try{
      directory_ptr dptr = directory_ptr_of(dirorplain->get_contents());
      dptr->cleardirents();
      dirents.erase(filename);
   } catch(invalid_argument&) {
      dirents.erase(filename);
   }
}
/*******************************************************/


/*******************************************************/
/* creates a new root for the beginning of the program */
void inode_state::init(){
   inode_ptr root_node = make_shared<inode>(DIR_INODE);
   directory_ptr_of(root_node->contents)->init_root(root_node);
   root = root_node;
   cwd  = root_node;
}


void inode_state::final_make_file(const wordvec& navpath,
                                  const wordvec& insertion){
   if(navpath.size() == 1) {
      string fname = navpath.back();
      wordvec vecins{};
      vecins.push_back(fname);
      for(const auto&wrd : insertion) vecins.push_back(wrd);
      make_file(vecins);
      return;
   }
   inode_ptr uptr = nullptr;
   string final_file_name = navpath.back();
   wordvec vec(&navpath[0], &navpath[navpath.size() - 1]);
   map<string,inode_ptr> sub_direc = directory_ptr_of(cwd->contents)
   ->get_map();
   size_t all_accessed = 0;
   for(const auto& word : vec){
      map<string,inode_ptr>::iterator itor;
      for(itor = sub_direc.begin(); itor != sub_direc.end(); ++itor){
         if(itor->first == word){
            try {
               uptr = itor->second;
               sub_direc = directory_ptr_of(uptr->contents)->get_map();
               ++all_accessed;
               break;
            } catch (invalid_argument) {
               complain() << "make: " << notExist() << endl;
               return;
            }
         }
      }
   }
   const auto tptr = cwd;
   if(uptr != nullptr and all_accessed == vec.size()){ // found all
      wordvec v{};
      v.push_back(final_file_name);
      for(const auto& word : insertion) v.push_back(word); 
      cwd = uptr;
      make_file(v);
      cwd = tptr;
   } else {
      complain() << "make: " << notExist() << endl;
   }
}
void inode_state::make_file(const wordvec& entry){
   string file_name = entry[0];
   wordvec vec(&entry[1], &entry[entry.size()]);
   auto curr_file = directory_ptr_of(cwd->contents);
   auto file_ents = curr_file->get_map();
   map<string,inode_ptr>::iterator itor;
   for(itor = file_ents.begin(); itor != file_ents.end(); ++itor){
      if(itor->first == file_name){
         try{
            auto plainFilePointer = 
            plain_file_ptr_of(itor->second->contents);
            plainFilePointer->writefile(vec);
            return;
         } catch(invalid_argument&){
            complain() << "make: File already exists" << endl;
            return;
         }
      }
   }
   auto node_ptr  = curr_file->mkfile(file_name);
   auto plain_filep = plain_file_ptr_of(node_ptr.contents);
   if(entry.size() > 1) {
      plain_filep->writefile(vec);
   } else {
      wordvec vec {};
      plain_filep->writefile(vec);
   }
}

void inode_state::final_mkdir(const wordvec& entry){
   if(entry.size() == 1){
      make_directory(entry);
      return;
   }
   inode_ptr uptr = nullptr;
   const auto cwd_place_holder = cwd;
   string final_file_name = entry.back();
   wordvec vec(&entry[0], &entry[entry.size() - 1]);
   map<string,inode_ptr> sub_direc = directory_ptr_of(cwd->contents)
   ->get_map();
   size_t all_accessed = 0;
   for(const auto& word : vec){
      map<string,inode_ptr>::iterator itor;
      for(itor = sub_direc.begin(); itor != sub_direc.end(); ++itor){
         if(itor->first == word){
            try {
               uptr = itor->second;
               sub_direc = directory_ptr_of(uptr->contents)->get_map();
               ++all_accessed;
               break;
            } catch (invalid_argument) {
               complain() << "mkdir: " << notExist() << endl;
               return;
            }
         }
      }
   }
   if(uptr != nullptr and all_accessed == vec.size()){
      cwd = uptr;
      wordvec vecins{};
      vecins.push_back(final_file_name);
      make_directory(vecins);
      cwd = cwd_place_holder;
   } else {
      complain() << "mkdir: " << notExist() << endl;
   }
}

void inode_state::make_directory(const wordvec& entry){
   const auto curr_file = directory_ptr_of(cwd->contents);
   string dir_name = entry[0];
   auto file_ents = curr_file->get_map();
   map<string,inode_ptr>::iterator itor;
   for(itor = file_ents.begin(); itor != file_ents.end(); ++itor){
      if(itor->first == dir_name){
         complain() << "mkdir: File exists" << endl;
         return;
      }
   }
   const auto node_ptr  = curr_file->mkdir(dir_name);
   const inode_ptr t = curr_file->get_map().find(dir_name)->second;
   const auto dir_filep = directory_ptr_of(node_ptr.contents);
   dir_filep->add_pointers(cwd, t);
}

wordvec inode_state::function_pwd(wordvec vec, inode_ptr ptr){
   if(ptr == root){
      vec.push_back("/");
      return vec;
   } else {
      directory_ptr iptr = directory_ptr_of(ptr->contents);
      inode_ptr     pptr = iptr->get_map().find("..")->second;
      directory_ptr par  = directory_ptr_of(pptr->contents);
      map<string,inode_ptr> dirs = par->get_map();
      map<string,inode_ptr>::iterator itor;
      for(itor = dirs.begin(); itor != dirs.end(); ++itor){
         if(itor->second == ptr) {
            vec.push_back(itor->first);
         }
      }
      return function_pwd(vec, pptr);
   }
}

void inode_state::fn_pwd(){
   wordvec temp_vector{};
   auto    pathnames = function_pwd(temp_vector, cwd);
   reverse(pathnames.begin(), pathnames.end());
   for(auto it = pathnames.begin(); it != pathnames.end(); ++it){
      string out = *it;
      if     (out == "/")                cout << out;
      else if(it == pathnames.end() - 1) cout << out;
      else                               cout << out << "/";
   }
   cout << endl;
}
/* function sets the new prompt */
void inode_state::set_prompt(const string& new_prompt) {
   prompt = new_prompt;
}

void inode_state::final_func_cat(const wordvec& entry){
   if(entry.size() == 1){
      cat(entry);
      return;
   }
   inode_ptr uptr = nullptr;
   string final_file_name = entry.back();
   wordvec vec(&entry[0], &entry[entry.size() - 1]);
   map<string,inode_ptr> sub_direc = directory_ptr_of(cwd->contents)
   ->get_map();
   for(const auto& word : vec){
      map<string,inode_ptr>::iterator itor;
      for(itor = sub_direc.begin(); itor != sub_direc.end(); ++itor){
         if(itor->first == word){
            try {
               uptr = itor->second;
               sub_direc = directory_ptr_of(uptr->contents)->get_map();
               break;
            } catch (invalid_argument) {
               complain() << "cat: "<< notExist() << endl;
               return;
            }
         }
      }
   }
   const auto tptr = cwd;
   if(uptr != nullptr){
      wordvec v{};
      path_name_set(entry[0]);
      v.push_back(final_file_name);
      cwd = uptr;
      cat(v);
      cwd = tptr;
   } else {
      complain() << "cat: " << cat_not_exist() << endl;
   }
   cwd = tptr;
}

void inode_state::cat(const wordvec& entry){
   map<string,inode_ptr> tempdir = 
   directory_ptr_of(cwd->contents)->get_map();
   map<string,inode_ptr>::iterator itor;
   string file_name = entry[0];
   inode_ptr uptr = nullptr;
   for(itor = tempdir.begin(); itor != tempdir.end(); ++itor)
      if(file_name == itor->first)
         uptr = itor->second;
   if(uptr == nullptr){
      complain() << "cat: "<< notExist() << endl;
   } else {
      try {
         plain_file_ptr fptr = plain_file_ptr_of(uptr->contents);
         wordvec vec(fptr->refsfadfile());
         for(const auto& wrd : vec) cout << wrd + " ";
         cout << endl;
      } catch(invalid_argument) {
         complain() << "cat: "<< isDirectory() << endl;
      }
   }
}

void inode_state::final_cd(const wordvec& entry){
   if(entry.size() == 0){
      cwd = root;
      return;
   }
   size_t all_accessed = 0;
   inode_ptr uptr = nullptr;
   map<string,inode_ptr> sub_direc = 
   directory_ptr_of(cwd->contents)->get_map();
   map<string,inode_ptr>::iterator itor;
   for(const auto& word : entry){
      map<string,inode_ptr>::iterator itor;
      for(itor = sub_direc.begin(); itor != sub_direc.end(); ++itor){
         if(itor->first == word){
            try {
               uptr = itor->second;
               sub_direc = directory_ptr_of(uptr->contents)->get_map();
               ++all_accessed;
               break;
            } catch (invalid_argument) {
               complain() << "cd: File Not Found" << endl;
               return;
            }
         }
      }
   }
   if(uptr != nullptr and all_accessed == entry.size()){
      try {
         directory_ptr test = directory_ptr_of(uptr->contents);
         cwd = uptr;
      } catch (invalid_argument&) {
         complain() << "cd: file is plain file";
      }
   } else {
      complain() << "file does not exist";
   }
}

void inode_state::function_cd(const wordvec& entry){
   const directory_ptr dp = directory_ptr_of(cwd->contents);
   map<string,inode_ptr> tempdir = dp->get_map();
   if(tempdir.find(entry[1]) == tempdir.end()){
      complain() << "FILE NO FOUND" << endl;
   } else {
      try {
         inode_ptr uptr = tempdir.find(entry[1])->second;
         directory_ptr dpointer = directory_ptr_of(uptr->contents);
         cwd = tempdir.find(entry[1])->second; 
      } catch(invalid_argument) {
         complain() << "FILE IS PLAIN FILE" << endl;
      }
   }
}


inode_ptr inode_state::get_working_ptr(const wordvec& navpath){
   size_t all_accessed = 0;
   inode_ptr uptr = nullptr;
   map<string,inode_ptr> sub_direc = 
   directory_ptr_of(cwd->contents)->get_map();
   map<string,inode_ptr>::iterator itor;
   for(const auto& word : navpath){
      map<string,inode_ptr>::iterator itor;
      for(itor = sub_direc.begin(); itor != sub_direc.end(); ++itor){
         if(itor->first == word){
            try {
               uptr = itor->second;
               sub_direc = directory_ptr_of(uptr->contents)->get_map();
               ++all_accessed;
               break;
            } catch (invalid_argument) {
               return itor->second;
            }
         }
      }
   }
   return(uptr != nullptr && 
          all_accessed == navpath.size()) ? uptr : nullptr;
}

void inode_state::function_list_r(inode_ptr iptr){
   if(iptr == nullptr){
      complain() << "lsr: " << notExist() << endl;
      return;
   }
   directory_ptr direcPointer = directory_ptr_of(iptr->contents);
   map<string,inode_ptr> direcMap = direcPointer->get_map();
   map<string,inode_ptr>::iterator itor;
   function_list(iptr);
   if(direcMap.size() == 2){
      return;
   } else {
      for(itor = direcMap.begin(); itor != direcMap.end(); ++itor){
         try {
            if(itor->first != "." and itor->first != ".."){
               directory_ptr test = 
               directory_ptr_of(itor->second->contents);
               function_list_r(itor->second);
            }
         } catch (invalid_argument&) {
            //Do nothing if its a plain file
         }
      }
   }
}

// state should have function to print inode nr.
void inode_state::function_list(inode_ptr curr_pointer){
   if(curr_pointer == nullptr) {
      complain() << "ls: file does not exist" << endl;
      return;
   }
   try {
      map<string,inode_ptr> tempdir = 
      directory_ptr_of(curr_pointer->contents)->get_map();
      map<string,inode_ptr>::iterator itor;
      bool print_once = false;
      for(itor = tempdir.begin(); itor != tempdir.end(); ++itor){
         inode_ptr uptr = itor->second;
         try {
            directory_ptr dpointer = directory_ptr_of(uptr->contents);
            if(!print_once){
               list_helper(uptr);
               cout << ":" << endl;
               print_once = !print_once;
            }
            cout << setw(6) << uptr->get_inode_nr() << setw(6) << 
            dpointer->size() << "  " << itor->first;
            if(itor->first != "." && itor->first != "..") cout << "/";
            cout << endl;
         } catch (invalid_argument&) {
            plain_file_ptr fpointer = plain_file_ptr_of(uptr->contents);
            cout << setw(6) << uptr->get_inode_nr() << setw(6) << 
            fpointer->size() << "  " << itor-> first << endl;
         }  
      }
   } catch(invalid_argument&) {
      complain() << "ls: File is a plain file" << endl;
   }
}

void inode_state::list_helper(inode_ptr curr_ptr){
   wordvec temp_vector{};
   auto pathnames = function_pwd(temp_vector, curr_ptr);
   reverse(pathnames.begin(), pathnames.end());
   for(auto it = pathnames.begin(); it != pathnames.end(); ++it){
      string out = *it;
      if     (out == "/")                cout << out;
      else if(it == pathnames.end() - 1) cout << out;
      else                               cout << out << "/";
   }
}

void inode_state::function_rm(const inode_ptr cd, 
                              const string& filename){
   try{
      directory_ptr curr_dir = directory_ptr_of(cd->contents);
      curr_dir->remove(filename);
   } catch(invalid_argument&){
      complain() << "rm: " << notExist() << endl;
   }
}

void inode_state::function_final_rm(const string& filename,
                                    const wordvec& entry){
   inode_ptr file_or_dir = get_working_ptr(entry);
   if(file_or_dir == nullptr){
      complain() << "rm: " << notExist() << endl;
   } else {
      function_rm(file_or_dir, filename);
   } 
}

void inode_state::function_rmr(const inode_ptr cd, 
                               const string& filename){
   try {
      directory_ptr curr_dir = directory_ptr_of(cd->contents);
      curr_dir->remove_recursive(filename);
   } catch(invalid_argument&){
      complain() << "rm: " << notExist() << endl;
   }
}

void inode_state::function_final_rmr(const string& filename,
                                     const wordvec& entry){
   inode_ptr final_dir = get_working_ptr(entry);
   if(final_dir == nullptr){
      complain() << "rmr: " << notExist() << endl;
   } else {
      function_rmr(final_dir, filename);
   }
}
void inode_state::endprog(){
   directory_ptr rootptr = directory_ptr_of(root->contents);
   map<string,inode_ptr> direcs = rootptr->get_map();
   map<string,inode_ptr>::iterator itor;
   for(itor = direcs.begin(); itor != direcs.end(); ++itor){
      if(itor->first != (".") and itor->first != ("..")){
         rootptr->remove_recursive(itor->first);
      }
   }
   root->contents.reset();
   rootptr = nullptr;
}

void inode_state::test(){
   root = nullptr;
   cwd = nullptr;
}

void inode_state::setCwd(inode_ptr ptr){
   cwd = ptr;
}


inode_ptr inode_state::get_root(){
   return root;
}

inode_ptr inode_state::get_cwd(){
   return cwd;
}

/* function returns the prompt */
string inode_state::get_prompt(){
   return prompt;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt << "\"");
}
/***************************************************/


ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}
