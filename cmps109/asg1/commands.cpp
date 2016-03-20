// $Id: commands.cpp,v 1.11 2014-06-11 13:49:31-07 - - $
#include "commands.h"
#include "debug.h"
#include "util.h"
#include <algorithm>

commands::commands(): map ({
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
}){}

command_fn commands::at (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   command_map::const_iterator result = map.find (cmd);
   if (result == map.end()) {
      throw yshell_exn (cmd + ": no such function");
   }
   return result->second;
}

void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() < 2){
      complain() << "cat: Too few arguments" << endl;
   } else {
      const string firstword = words.at(1);
      path_name_set(firstword);
      if(firstword.at(0) == '/' and firstword.length() > 1){
         string navname = firstword.substr(1, firstword.length());
         cout << navname << endl;
         const auto placeholder = state.get_cwd();
         wordvec nav_path {};
         nav_path = split(navname, "/");
         state.setCwd(state.get_root());
         state.final_func_cat(nav_path);
         state.setCwd(placeholder);
         return;
      } else if(firstword.length() == 1 and firstword.at(0) == '/'){
          complain() << "cat: /: File is Directory" << endl;
          return;
      }
      wordvec nav_path = split(firstword, "/");
      state.final_func_cat(nav_path);
   }
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   /*
   if(words.size() > 1) {
      wordvec nav_path = split(words[1], "/");
      state.final_cd(nav_path);
   } else {
      wordvec root_dir{};
      state.final_cd(root_dir);
   }
   */
   if(words.size() > 2){
      complain() << "cd: Too many arguments" << endl;
      return;
   }
   if(words.size() > 1) path_name_set(words.at(1));
   if(words.size() == 1){
      state.setCwd(state.get_root());
      return;
   } else if(words.at(1).at(0) == '/'){
      state.setCwd(state.get_root());
   }
   wordvec nav_path = split(words.at(1), "/");
   state.final_cd(nav_path);
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   wordvec vec(&words[1], &words[words.size()]);
   for(const auto& wrd : vec) cout << wrd << " ";
   cout << endl;
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   wordvec vec(&words[1], &words[words.size()]);
   string exit_stat_str = "";
   for(const auto& wrd : vec) exit_stat_str += wrd;
   if(vec.size() == 0){
      state.endprog();
      throw ysh_exit_exn();
   } else {
      try {
         int exit_stat_int = stoi(exit_stat_str);
         exit_status::set(exit_stat_int);
         state.endprog();
         throw ysh_exit_exn();
      } catch (invalid_argument&) {
         exit_status::set(127);
         state.endprog();
         throw ysh_exit_exn();
      }
   }
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() > 2){
      complain();
      cout << "ls: Too many arguments" << endl;
      return;  
   } else if(words.size() == 1){
      state.function_list(state.get_cwd());
   } else if(words.at(1).at(0) == '/'){
      if(words.at(1).length() == 1){
         state.function_list(state.get_root());
         return;
      }
      const auto placeholder = state.get_cwd();
      state.setCwd(state.get_root());
      wordvec nav_path = split(words.at(1), "/");
      state.function_list(state.get_working_ptr(nav_path));
      state.setCwd(placeholder);
   } else {
      wordvec nav_path = split(words[1], "/");
      state.function_list(state.get_working_ptr(nav_path));
   }
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() > 2){
      complain() << "lsr: Too many arguments" << endl;
      return;
   } else if(words.size() == 1){
      state.function_list_r(state.get_cwd());
   } else if(words.at(1).at(0) == '/'){
      if(words.at(1).length() == 1){
         state.function_list_r(state.get_root());
         return;
      }
      const auto placeholder = state.get_cwd();
      state.setCwd(state.get_root());
      wordvec nav_path = split(words.at(1), "/");
      state.function_list_r(state.get_working_ptr(nav_path));
      state.setCwd(placeholder);
   } else {
      wordvec nav_path = split(words[1], "/");
      state.function_list_r(state.get_working_ptr(nav_path));
   }
}


void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() < 2){
      complain() << "make: Specify pathname" << endl;
      return;
   }
   const auto placeholder = state.get_cwd();
   if(words.at(1).at(0) == '/'){
      state.setCwd(state.get_root());
   }
   wordvec nav_path  = split(words[1], "/");
   wordvec insertion(&words[2], &words[words.size()]);
   state.final_make_file(nav_path, insertion);
   state.setCwd(placeholder);
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   //NEED TO CHECK IF THE DIR IS ALREADY IN THE DIR.
   if(words.size() < 2){
      complain() << "mkdir: Too few arguments" << endl;
   } else if(words.size() > 2) {
      complain() << "mkdir: Too many arguments" << endl;
   } else {
      path_name_set(words.at(1));
      const auto placeholder = state.get_cwd();
      if(words.at(1).at(0) == '/') {
         state.setCwd(state.get_root());
      }
      path_name_set(words.at(1));
      wordvec nav_path = split(words.at(1), "/");
      state.final_mkdir(nav_path);
      state.setCwd(placeholder);
   }
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() < 2){
      complain() << "prompt: Too few arguments" << endl;
      return;
   }
   string nprompt = "";
   wordvec vec (&words[1], &words[words.size()]);
   for(const auto& wrd: vec) nprompt += wrd + " ";
   state.set_prompt(nprompt);
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() > 1) {
      complain() << "pwd: Too many arguments" << endl;
      return;
   }
   state.fn_pwd();
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() == 1){
      complain() << "rm: Specify file name" << endl;
   } else if(words.size() > 2){
      complain() << "rm: Too many arguments" << endl;
   } else if(words.at(1) == "." or words.at(1) == ".." or 
      (words.at(1).at(0) == '/' and words.at(1).length() == 1)){
      complain() << "rmr: Cannot remove directory" << endl;
   } else {
      path_name_set(words.at(1));
      const auto placeholder = state.get_cwd();
      if(words.at(1).at(0) == '/') state.setCwd(state.get_root());
      wordvec navpath = split(words[1], "/");
      string ffname = navpath.back();
      if(navpath.size() == 1){
         state.function_rm(state.get_cwd(), ffname);
         return;
      }
      wordvec filepath(&navpath[0], &navpath[navpath.size() - 1]);
      state.function_final_rm(ffname, filepath);
      state.setCwd(placeholder);
   }
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() == 1){
      complain() << "rmr: Specify file name" << endl;
   } else if(words.size() > 2){
      complain() << "rmr: Too many arguments" << endl;
   } else if(words.at(1) == "." or words.at(1) == ".." or 
      (words.at(1).at(0) == '/' and words.at(1).length() == 1)){
      complain() << "rmr: Cannot remove directory" << endl;
   } else {
      path_name_set(words.at(1));
      const auto placeholder = state.get_cwd();
      if(words.at(1).at(0) == '/') state.setCwd(state.get_root());
      wordvec navpath = split(words[1], "/");
      string ffname = navpath.back();
      if(navpath.size() == 1){
         state.function_rmr(state.get_cwd(), ffname);
         return;
      }
      wordvec filepath(&navpath[0], &navpath[navpath.size() - 1]);
      state.function_final_rmr(ffname, filepath);
      state.setCwd(placeholder);
   }
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}
