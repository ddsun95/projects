// $Id: cix.cpp,v 1.2 2015-05-12 18:59:40-07 - - $
// David Sun (ddsun@ucsc.edu)

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", CIX_EXIT},
   {"get" , CIX_GET },
   {"help", CIX_HELP},
   {"ls"  , CIX_LS  },
   {"put" , CIX_PUT },
   {"rm"  , CIX_RM  },
};

void cix_help() {
   static vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

/* bool value to see whether an argument is a port or a host */
bool isport(const string& str){
   return(str.find('.') == string::npos && str != "localhost"); 
}

/* utility function to split a string */
vector<string> split (const string& line, const string& delimiters) {
   vector<string> words;
   int end = 0;
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   return words;
}

/* copy the name */
void copyname(char* d, string s){
   for(size_t i = 0; i < s.size(); ++i) d[i] = s[i];
}

/* reads the contents of the files and outputs the file through an
   ofstream */
void cix_get(client_socket& server, string fname){
   cix_header header;
   header.command = CIX_GET;
   copyname(header.filename, fname);
   log << "sending header " << header << endl;
   send_packet(server, &header, sizeof header);
   recv_packet(server, &header, sizeof header);
   log << "received header " << header << endl;
   if(header.command != CIX_FILE){
      log << "sent CIX_LS, server did not return CIX_GET" << endl;
      log << "server returned " << header << endl;  
   } else {
      char buffer[header.nbytes + 1];
      recv_packet(server, buffer, header.nbytes);
      log << "recieved " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      ofstream outputfile(fname);
      outputfile.write(buffer, header.nbytes);
      outputfile.close();
   }
}

/* shifts the contents of the file into a stringstream then sends
   the string through a packet */
void cix_put(client_socket& server, string fname){
   cix_header header;
   header.command = CIX_PUT;
   ifstream putfile;
   putfile.open(fname);
   if(putfile.fail()){
      log << "put: invalid file" << endl;
   } else {
      copyname(header.filename, fname);
      stringstream fcontents;
      fcontents << putfile.rdbuf();
      string contentstr = fcontents.str();
      header.nbytes = contentstr.size();
      log << "sending header " << header << endl;
      send_packet(server, &header, sizeof header);
      send_packet(server, contentstr.c_str(), contentstr.size());
      log << "sent " << contentstr.size() << " bytes" << endl;
      recv_packet(server, &header, sizeof header);
      if(header.command == CIX_ACK){
         log << "file " << fname << " successfully written to server"
             << endl;
      } else {
         log << "sent CIX_PUT, server did not return CIX_ACK" << endl;
         log << "server returned " << header << endl;
      }
   }
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = CIX_LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != CIX_LSOUT) {
      log << "sent CIX_LS, server did not return CIX_LSOUT" << endl;
      log << "server returned " << header << endl;
   } else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

/* sends the name of the file to be removed through a packet */
void cix_rm(client_socket& server, string fname){
   cix_header header;
   header.command = CIX_RM;
   copyname(header.filename, fname);
   log << "sending header " << header << endl;
   send_packet(server, &header, sizeof header);
   recv_packet(server, &header, sizeof header);
   log << "received header " << header << endl;
   if(header.command != CIX_ACK){
      log << "sent CIX_RM, server did not return CIX_ACK" << endl;
      log << "server returned " << header << endl;
   } else {
      log << "file " << fname << " successfully removed" << endl;
   }
}

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if(args.size() > 2) usage();
   string host;
   in_port_t port;
   /* code to parse only a port argument */
   if(args.size() == 1){
      host = isport(args[0]) ? get_cix_server_host(args, 1):
                               get_cix_server_host(args, 0);
      port = isport(args[0]) ? get_cix_server_port(args, 0):
                               get_cix_server_port(args, 1);
   } else {
      host = get_cix_server_host(args, 0);
      port = get_cix_server_port(args, 1);
   }
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command: " << line << endl;
         vector<string> cmdvec = split(line, " ");
         if(cmdvec.size() < 1){
            log << "too few arguments" << endl;
            continue;
         }
         if(cmdvec.size() > 2){
            log << "too many arguments" << endl;
            continue;
         }
         string cmnd = cmdvec[0];
         string file = cmdvec.size() == 2 ? cmdvec[1] : "";
         const auto& itor = command_map.find (cmnd);
         cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second;
         switch (cmd) {
            case CIX_EXIT:
               throw cix_exit();
               break;

            case CIX_HELP:
               cix_help();
               break;

            case CIX_LS:
               cix_ls(server);
               break;

            case CIX_GET:
               if(file.size() == 0){
                  log << "Usage: get filename" << endl;
                  break;
               }
               cix_get(server, file);
               break;

            case CIX_PUT:
               if(file.size() == 0){
                  log << "Usage: put filename" << endl;
                  break;
               }
               cix_put(server, file);
               break;

            case CIX_RM:
               if(file.size() == 0){
                  log << "Usage: rm filename" << endl;
                  break;
               }
               cix_rm(server, file);
               break;

            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}
