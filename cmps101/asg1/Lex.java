// David Sun
// ddsun@ucsc.edu (1357057)
// pa1 (Lex)

import java.util.Scanner;
import java.io.File;
import java.io.IOException;
import java.io.FileWriter;
import static java.lang.System.*;

class Lex {

   public static void main (String[] args) throws IOException {
      if (args.length != 2) {
         err.println ("usage: Lex infile outfile");
         exit (1);
      }
      Scanner scan = new Scanner (new File (args[0]));                  
      FileWriter fw = new FileWriter (new File (args[1]));
      int lineCount = lines (args[0]);
      String[] lineArray = new String[lineCount];
      for (int i = 0; scan.hasNextLine(); ++i)
         lineArray[i] = scan.nextLine();
      List L = new List();
      for (int i = 0; i < lineArray.length; ++i) {
         String temp = lineArray[i];
         if (L.length() == 0) {
            L.append (i);
            continue;
         }
         L.moveFront();
         while (L.index() >= 0) {
            /* iterate through the list until we hit an element 
             * lexigoraphically greater than or equal to and insert the
             * element before the one we found */
            if (temp.compareTo (lineArray[L.get()]) <= 0) {
               L.insertBefore (i);
               break;
            }
            L.moveNext();
            /* if the item we're inserting is larger than all other 
             * items in the list, insert it as the last element */
            if (L.index() == -1) {  
               L.append (i);
               break;
            }
         }
      }
      for (L.moveFront(); L.index() >= 0; L.moveNext()) {
         fw.write(lineArray[L.get()]);
         if (L.index() != L.length() - 1) fw.write('\n');
      }
      fw.close();
      scan.close();
   }

   public static int lines (String filename) throws IOException {
      Scanner count = new Scanner (new File (filename));
      int lines = 0;
      for (; count.hasNextLine(); count.nextLine()) ++lines;
      return lines;
   }
}
