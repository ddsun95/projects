// David Sun
// ddsun@ucsc.edu
// Pa3 (Sparse)
// ListTest.java

import static java.lang.System.*;

class ListTest {

   public static void main (String[] args) {
      List strlist = new List();
      List listcpy = new List();
      strlist.append ("second");
      strlist.prepend ("first");
      strlist.append ("third");
      out.println (strlist.front()); // expected = "first"
      out.println (strlist.back());  // expected = "third"
      strlist.append ("fourth");
      strlist.append ("fifth");
      out.println (strlist.length()); // expected = 5;

      /* test toString method and cursor functions
       * output should be the same */
      for (strlist.moveFront(); strlist.index() >= 0; strlist.moveNext()) {
         out.print (strlist.get() + " ");
      }
      out.println();
      out.println (strlist);
      listcpy.append ("first");
      listcpy.append ("second");
      listcpy.append ("third");
      listcpy.append ("fourth");
      listcpy.append ("fifth");
      out.println (strlist.equals (listcpy)); // expected = true
      listcpy.append ("sixth");
      listcpy.append ("seventh");
      listcpy.append ("eighth");
      listcpy.append ("ninth");
      listcpy.append ("tenth");
      listcpy.deleteFront();
      listcpy.deleteFront();
      listcpy.deleteFront();

      /* expected = fourth fifth sixth seventh eighth ninth tenth */
      out.println (listcpy); 

      /* expected = false */
      out.println (listcpy.equals (strlist));
      listcpy.moveFront();
      listcpy.moveNext();
      listcpy.delete();

      /* expected = fourth sixth seventh eighth ninth tenth */
      out.println (listcpy); 
      listcpy.deleteBack();
      listcpy.deleteFront();

      /*expected = sixth seventh eighth ninth */
      out.println (listcpy);

      listcpy.moveFront();
      listcpy.moveNext();
      listcpy.moveNext();
      out.println (listcpy.index()); // expected = 2
      listcpy.insertBefore ("before_eighth");
      listcpy.insertAfter ("after_eighth");
      listcpy.delete();

      
      /* expected output = sixth seventh before_eiegth after_eigth ninth */
      out.println (listcpy);

      /* prints the list in reverse using movePrev() func */
      for (listcpy.moveBack(); listcpy.index() >= 0; listcpy.movePrev())
         out.print (listcpy.get() + " ");
      out.println();
   }
}