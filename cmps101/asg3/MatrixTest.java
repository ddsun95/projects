// David Sun
// ddsun@ucsc.edu
// pa3 (Sparse)
// MatrixTest.java

import static java.lang.System.*;

class MatrixTest {

   public static void main (String[] args) {
   	Matrix one = new Matrix (10);
   	Matrix two = new Matrix (10);

   	double one_ = 100, two_ = 1;
   	for (int i = 1; i <= 10; ++i) {
   		for (int j = 1; j <= 10; ++j) {
   			one.changeEntry (i, j, one_);
   			two.changeEntry (i, j, two_);
   			--one_;
   			++two_;
   		}
   	}
   	/* expected: 100 nonzero entries */
   	out.println ("Matrix one has size of " + one.getSize());
   	out.println ("Matrix one has " + one.getNNZ() + " nonzero entries."); 
   	out.println (one);          // expected: matrix with 100 entries
 											 // entries are in decreasing order
   	/* expected: 100 nonzero entries */
   	out.println ("Matrix two has size of " + two.getSize());
   	out.println ("Matrix two has " + two.getNNZ() + " nonzero entries.");
   	out.println (two);			 // expected: matrix with 100 entries
   										 // entries are in increaasing order
   	one.makeZero();
   	for (int i = 1; i <= 10; ++i) one.changeEntry (i, i, 1); 

   	/* expected: the identity matrix */
   	out.println (one);
   	Matrix three = two.mult (one);
   	out.println (two.equals (three)); // expected: true since any matrix
   												 // multiplied by identity matrix should
   	 											 // yield the same matrix
   	Matrix four = one.sub (one);
   	out.println (four.getNNZ());		 // expected = 0
   	out.println (four);					 // expected = blank
   	Matrix six = new Matrix (4);
   	Matrix sev = new Matrix (4);
   	for (int i = 1; i <= 4; ++i) {
   		for (int j = 1; j <= 4; ++j) {
   			six.changeEntry (i, j, 1);
   			sev.changeEntry (i, j, 1);
   		}
   	}

   	out.println ("Matrix six added to Matrix sev");
   	out.println (six.add (sev)); // should be a matrix with entries consisting
   	 									  // of all 2's
   	six = six.scalarMult (5);
   	sev = sev.scalarMult (5);

   	out.println ("Matrix six with all entries multiplied by 5");
   	out.println (six);			  // should be a matrix with entries all 5s

   	out.println ("Matrix sev with all entries multiplied by 5");
   	out.println (sev);			  // should be a matrix with entries all 5s

   	six.changeEntry (1, 1, 0);
   	six.changeEntry (2, 2, 0);
   	six.changeEntry (3, 3, 0);
   	six.changeEntry (4, 4, 0);
   	sev.changeEntry (1, 4, 0);
   	sev.changeEntry (2, 3, 0);
   	sev.changeEntry (3, 2, 0);
   	sev.changeEntry (4, 1, 0);
   	out.println ("Matrix six with main diagonal zeroed");
   	out.println (six);			// matrix six with main diagonal zeroed

   	out.println ("Matrix sev with main diagonal zeroed");
   	out.println (sev);			// matrix sev with other diagonal zeroed

   	out.println ("Matrix six transposed");
   	out.println (six.transpose());	// transpose of Matrix six

   	out.println ("Matrix sev transposed");
   	out.println (sev.transpose());	// transpose of Matrix sev

   	// Should throw an exception since Matrix one and Matrix six have
   	// different dimensions
   	try {
   		Matrix eig = one.mult (six);
   	} catch (RuntimeException RE) {
   		out.println ("cannot multiply Matrix one by Matrix six");
   		out.println ("dimension error");
   	}
   	Matrix eig = six.mult (six);
   	out.println ("Matrix six * Matrix six =");
   	out.println (eig);
   }
}