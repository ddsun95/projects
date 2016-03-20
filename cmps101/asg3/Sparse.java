// David Sun
// ddsun@ucsc.edu
// Pa3 (Sparse)
// Sparse.java

import java.util.*;
import java.io.*;
import static java.lang.System.*;

class Sparse {

   public static void usage() {
      err.println ("usage: Sparse infile outfile");
      exit (1);
   }

   public static void main (String[] args) throws IOException {
      if (args.length != 2) usage();
      Scanner scan = new Scanner (new File (args[0]));
      FileWriter file = new FileWriter (new File (args[1]));
      String[] dim = scan.nextLine().split(" ");
      Matrix A = process (scan, Integer.parseInt (dim[0]), 
                                Integer.parseInt (dim[1]));
      Matrix B = process (scan, Integer.parseInt (dim[0]), 
                                Integer.parseInt (dim[2]));
      file.write ("A has " + A.getNNZ() + " non-zero entries:\n");
      file.write (A.toString());
      file.write ("\n");
      file.write ("B has " + B.getNNZ() + " non-zero entries:\n");
      file.write (B.toString());
      file.write ("\n");
      file.write ("(1.5)*A = \n");
      file.write (A.scalarMult (1.5).toString());
      file.write ("\n");
      file.write ("A+B =\n");
      file.write (A.add (B).toString());
      file.write ("\n");
      file.write ("A+A =\n");
      file.write (A.add (A).toString());
      file.write ("\n");
      file.write ("B-A =\n");
      file.write (B.sub (A).toString());
      file.write ("\n");
      file.write ("A-A =\n");
      file.write (A.sub (A).toString());
      file.write ("\n");
      file.write ("Transpose(A) =\n");
      file.write (A.transpose().toString());
      file.write ("\n");
      file.write ("A*B =\n");
      file.write (A.mult (B).toString());
      file.write ("\n");
      file.write ("B*B =\n");
      file.write (B.mult (B).toString());
      file.close();
   }

   public static Matrix process (Scanner scan, int dim, int nnz) {
      Matrix ret = new Matrix (dim);
      scan.nextLine();
      for (int i = 0; i < nnz; ++i) {
         String[] pairvalue = scan.nextLine().split(" ");
         int row = Integer.parseInt (pairvalue[0]);
         int col = Integer.parseInt (pairvalue[1]);
         double data = Double.parseDouble (pairvalue[2]);
         ret.changeEntry (row, col, data);
      }
      return ret;
   }
}