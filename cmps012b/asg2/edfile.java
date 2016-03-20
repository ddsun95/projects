/******************************************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/4/2014
 * edfile.java
 * edfile.java contains the main functions of the program
 * it allows the users to enter a file and select from a list of commands
 * each of the commands allow the lines in the file to be modified in some way.
 ******************************************************************************/
import java.util.*;
import static java.lang.System.*;
import java.io.*;
class edfile{

   public static void main (String[] args) throws Exception{ //Might throw an exception if there is no file   
   boolean want_echo = true;
   Scanner input = new Scanner(in);	//scanner to read in a filename should the user decide not to enter a file into the command line
   Scanner stdin = new Scanner(in);
   Scanner scan = null;

   if(args.length > 2){
      auxlib.usage("[-e] [filename]");
   }
   if(args.length == 2){   //If the argument has length 2, take the filename from the second argument
      try {
         scan = new Scanner(new File(args[1]));
      }catch(FileNotFoundException e){
         out.printf("Invalid File.\n");
      }
      
   }else if(args.length == 1){   //If the argument has length 1, determine whether that argument is the -e option or the filename
      if(!args[0].equals("-e")){
         want_echo = false;
         try {
            scan = new Scanner(new File(args[0]));
         }catch(FileNotFoundException e){
            out.printf("Invalid File.\n");
         }
      }
   } else {
      want_echo = false;
   }
  	dllist lines = new dllist ();

   if(scan != null)
      while(scan.hasNextLine())
         lines.insert(scan.nextLine(), dllist.position.LAST);
      
  	for (;;) {
      out.printf("Enter a command: ");
     	if (! stdin.hasNextLine()) break;
     	String inputline = stdin.nextLine();
     	if (want_echo) out.printf ("%s%n", inputline);
     	if (inputline.matches ("^\\s*$")) continue;
      char command = inputline.charAt(0);
   	switch (command) {
         case '#': //represents a comment line
            break;

      	case '$':  //sets the position to the last line
            lines.setPosition(dllist.position.LAST);
            if(lines.isEmpty()){
               out.printf("Empty list, unable to set position (LAST)!\n");
            } else {
               out.printf("%s\n", lines.getItem());
            }
            break;

         /* If there are no elements in the list
          * the program will print out a message stating such
          * Otherwise, get get the total number of items in the list
          * and iterate through each one printing them out *********/
      	case '*':  
            if(lines.isEmpty()){
               out.printf("There are no elements in the list!\n");
            } else {
               lines.setPosition(dllist.position.LAST); 
               int numLines = lines.getPosition();
               lines.setPosition(dllist.position.FIRST);
               for(int i = 0; i <= numLines; i++){
                  out.printf("%s\n", lines.getItem());
                  lines.setPosition(dllist.position.FOLLOWING);
               }  
            }
            lines.setPosition(dllist.position.LAST); //BY SPECIFICATION, sets the current line to last                   
            break;
  
         case '.': //prints out the current line
            if(lines.isEmpty()) {
               out.printf("Nothing to print, list is empty!\n");
            } else {
               out.printf("%s\n", lines.getItem());      
            }
            break;

         case '0': //sets the line to the first position, printing it out
            lines.setPosition(dllist.position.FIRST); 
            if(lines.isEmpty()){
               out.printf("Empty list, unable to set position (FIRST)!\n");
            } else {
               out.printf("%s\n", lines.getItem()); 
            }
            break;

         case '<': //sets the line to the previous position, printing it out
            lines.setPosition(dllist.position.PREVIOUS);
            if(lines.isEmpty()) {
               out.printf("Empty list, unable to set position (PREVIOUS)!\n");
            } else {
               out.printf("%s\n", lines.getItem()); 
            }            

            break;

         case '>': //sets the line to the following item, printing it out, 
            lines.setPosition(dllist.position.FOLLOWING);
            if(lines.isEmpty()) {
               out.printf("Empty list, unable to set position (FOLLOWING)!\n");
            } else {
               out.printf("%s\n", lines.getItem()); 
            }     
            break;

         case 'a': //insert a line after the current line
            String insert_after = inputline.substring(1, inputline.length()).trim();
            if(insert_after.equals("")) break;  //ignore any whitespace inputs
            try{
               lines.insert(insert_after, dllist.position.FOLLOWING);
               out.printf("%s\n", lines.getItem()); 
            }catch(IllegalArgumentException e){ //catch the illegal argument exception should the insert following on the empty list.
               out.printf("Empty list, unable to insert at position following\n");
            }
            break;

         case 'd': //deletes the current line
            if(!lines.isEmpty()){   //if the line is not empty, 
               lines.delete();      //delete the line
            }
            if(lines.isEmpty()){    //print a message telling there is nothing to print
               out.printf("You've deleted the last element, the list is empty!\n");
            } else {
               out.printf(lines.getItem()+"\n");
            }
            break;

         case 'i': //insert some text after the current line, ignores only whitespace texts
            String insert_before = inputline.substring(1, inputline.length()).trim();
            if(insert_before.equals("")) break; //ignore any whitespace inputs
            try {
               lines.insert(insert_before, dllist.position.PREVIOUS);
               out.printf("%s\n", lines.getItem());
            }catch(IllegalArgumentException e){ //Catch the illegal argument should the user insert pervious on the empty list.
               out.printf("Empty list, unable to insert at position previous\n");
            }
            break;

         /* Attempts to read the items of a specified file into the list after the current line
          * if the file cannot be read, print out a warning message
          * if the file is read in, the current line is set to the last line read in */
         case 'r':
            String file_name = inputline.substring(1, inputline.length()).trim();
            if(file_name.equals("")) break;  //ignore any whitespace inputs
            int number_of_lines = 0;
            try {
               scan = new Scanner(new File(file_name));
               while(scan.hasNextLine()){
                  if(!lines.isEmpty()) {
                     lines.insert(scan.nextLine(), dllist.position.FOLLOWING);   //if the list is not empty, insert each line following
                  }else{
                     lines.insert(scan.nextLine(), dllist.position.LAST);        //if the list is empty, insert each line as the last line
                  }
                  number_of_lines++;
               }
               out.printf("%d lines inserted into the list.\n", number_of_lines);
            }catch(IOException e){
               auxlib.warn("Invalid file!");
            }
            break;

         /* Writes all of the current lines to a specified filename 
          * if the file is unable to be written, the program prints out an error messgage
          * Otherwise, fetch the number of elements in the list, iterate through each item
          * and print each item on a separate line.
          * If there are no elements on the list, the program prints out a warning as well */
         case 'w': 
            String write_to_file = inputline.substring(1, inputline.length()).trim();
            if(write_to_file.equals("")) break; //ignore any whitespace inputs
            try {
               FileWriter f = new FileWriter(write_to_file);
               int end = 0, line_counter = 0;
               if(!lines.isEmpty()){
                  lines.setPosition(dllist.position.LAST);
                  end = lines.getPosition(); line_counter = 0;
                  lines.setPosition(dllist.position.FIRST);
                  for(int i = 0; i <= end; i++){
                     f.write(lines.getItem() + "\n");
                     line_counter++;
                     lines.setPosition(dllist.position.FOLLOWING);
                  }
               }
               f.close();
               out.printf("Success!, %d lines written\n", line_counter);
            }catch(IOException e){
               auxlib.warn("File cannot be written!");
            }
            break;

         default : 
            out.printf("Invalid command, enter new command:\n");
            break;
	      }
     	}
      /* EOF, quit the program */
      auxlib.die("END OF FILE");
   }
}

