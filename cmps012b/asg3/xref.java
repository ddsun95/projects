/*************************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/19/2014
 * xref.java
 * xref.java reads in words of a file and inserts the words
 * into a tree data structure. Depending on the -d option
 * xref.java will either display the line each word was found
 * or display the debug output.
 *************************************************************/
import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class xref {

    static void processFile(String filename, boolean debug) throws IOException {
        Scanner scan = new Scanner (new File(filename));
        Tree tree = new Tree();
        for (int linenr = 1; scan.hasNextLine (); ++linenr) {
            for (String word: scan.nextLine().split ("\\W+")) {
                tree.insert(word, linenr);
                //I took out the line of code that prints out each word and its line
                //To emulate the python version of this program exactly.
            }
        }
        if (debug) {
            tree.debug();
        } else {
            tree.output();
        }
    }

    public static void main(String[] args) {
        String filename = null;
        boolean debug = false;
		if(args.length == 1){     //if only one argument, assumes filename is the argument
			filename = args[0];
		} else if(args.length == 2 && args[0].equals("-d")){
			filename = args[1];
			debug = true;
		} else {
			auxlib.usage_exit("[-d] filename");
		}
        
        try {
            processFile(filename, debug);
        }catch (IOException error) {
            auxlib.warn (error.getMessage());
        }
        auxlib.exit();
    }
}

