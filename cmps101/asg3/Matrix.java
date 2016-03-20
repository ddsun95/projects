// David Sun
// ddsun@ucsc.edu
// Pa3 (Sparse)
// Matrix.java

import static java.lang.System.*;

class Matrix {

   private List[] row;
   private int rows;

   private class Entry {
      int idx;
      double data;
      public Entry (int id, double d) { idx = id; data = d; }
      public boolean equals (Object that) {
         Entry ent = (Entry)that;
         return this.idx == ent.idx && this.data == ent.data;
      }
      public String toString() 
      { return "(" + idx + ", " + data + ")"; }
   }

   public Matrix (int n) {
      this.rows = n;
      this.row = new List[n + 1];
      for (int i = 1; i < row.length; ++i) this.row[i] = new List();
   }

   public int getNNZ() {
      int nnz = 0;
      for (int i = 1; i < row.length; ++i) nnz += row[i].length();
      return nnz;
   }

   public int getSize() { return rows; }

   public boolean equals (Object x) {
      Matrix that = (Matrix)x;
      if (this.row.length != that.row.length) return false;
      for (int i = 1; i < row.length; ++i)
         if (!this.row[i].equals (that.row[i])) return false;
      return true;
   }   
   
   public void makeZero() {
      for (int i = 1; i <= rows; ++i) row[i] = new List();
   }

   public Matrix copy() { 
      Matrix cpy = new Matrix (rows);
      for (int i = 1; i < row.length; ++i) {
         if (row[i].length() == 0) continue;
         for (row[i].moveFront(); row[i].index() >= 0; row[i].moveNext()) {
            Entry e = (Entry)row[i].get();
            int index = e.idx;
            double dat = e.data;
            cpy.row[i].append (new Entry (index, dat));
         }
      }
      return cpy;
   }

   // changeEntry on precondition that 1 <= i <= getSize()
   // and 1 <= j <= getSize()
   public void changeEntry (int i, int j, double x) {
      if (i < 1 || i > getSize())
         throw new RuntimeException ("pre: 1 <= i <= getSize()");
      if (j < 1 || j > getSize())
         throw new RuntimeException ("pre: 1 <= j <= getSize()");
      if (row[i].length() == 0 && x != 0) {
         row[i].append (new Entry (j , x));
      } else {
         for (row[i].moveFront(); row[i].index() >= 0;) {
            Entry e = (Entry)row[i].get();
            if (j < e.idx && x != 0) {
               row[i].insertBefore (new Entry (j, x));
               break;
            }
            if (j == e.idx && x != 0) {
               e.data = x;
               break;
            }
            if (j == e.idx && x == 0) {
               row[i].delete();
               break;
            }
            row[i].moveNext();
            if (row[i].index() == -1 && x != 0) {
               row[i].append (new Entry (j, x));
               break;
            }
         }
      }
   }
   
   public Matrix scalarMult (double x) {
      Matrix m = this.copy();
      if (x == 0) { m.makeZero(); return m; }
      for (int i = 1; i < m.row.length; ++i) {
         for (m.row[i].moveFront(); m.row[i].index() >= 0;) {
            Entry e = (Entry)m.row[i].get();
            e.data *= x;
            m.row[i].moveNext();
         }
      }
      return m;
   }

   // add() on precondition that getSize() == M.getSize()
   public Matrix add (Matrix M) {
      if (this.getSize() != M.getSize())
         throw new RuntimeException ("pre: this.getSize() == M.getSize()");
      Matrix sum = new Matrix (rows);
      Matrix that = M.copy();
      for (int i = 1; i < this.row.length; ++i) {
         this.row[i].moveFront();
         that.row[i].moveFront();
         while (this.row[i].index() >= 0 && that.row[i].index() >= 0) {
            Entry this_ = (Entry)this.row[i].get();
            Entry that_ = (Entry)that.row[i].get();
            if (this_.idx == that_.idx) {
               this.row[i].moveNext();
               that.row[i].moveNext();
               double datasum = this_.data + that_.data;
               if (datasum == 0) continue;
               sum.row[i].append (new Entry (this_.idx, datasum));
            } else if (this_.idx < that_.idx) {
               sum.row[i].append (new Entry (this_.idx, this_.data));
               this.row[i].moveNext();
            } else if (this_.idx > that_.idx) {  
               sum.row[i].append (new Entry (that_.idx, that_.data));
               that.row[i].moveNext();
            }
         }
         while (this.row[i].index() >= 0) {
            Entry entry = (Entry)this.row[i].get();
            sum.row[i].append (new Entry (entry.idx, entry.data));
            this.row[i].moveNext();
         }
         while (that.row[i].index() >= 0) {
            Entry entry = (Entry)that.row[i].get();
            sum.row[i].append (new Entry (entry.idx, entry.data));
            that.row[i].moveNext();
         }
      }
      return sum;
   }

   // sub() with precondition that getSize() == M.getSize()
   public Matrix sub (Matrix M) {
      if (this.getSize() != M.getSize())
         throw new RuntimeException ("pre: this.getSize() == M.getSize()");
      return this.add (M.scalarMult (-1));
   }

   public Matrix transpose() {
      Matrix tran = new Matrix (rows);
      for (int i = 1; i < this.row.length; ++i) {
         if (this.row[i].length() == 0) continue;
         for (this.row[i].moveFront(); this.row[i].index() >= 0;) {
            Entry this_ = (Entry)this.row[i].get();
            tran.changeEntry (this_.idx, i, this_.data);
            this.row[i].moveNext();
         }
      }
      return tran;
   }

   // mult() with precondition that getSize() == M.getSize()
   public Matrix mult (Matrix M) {
      if (this.getSize() != M.getSize())
         throw new RuntimeException ("pre this.getSize() == M.getSize()");
      Matrix mult = new Matrix (rows);
      Matrix that = M.transpose();
      for (int i = 1; i < this.row.length; ++i) {
         if (this.row[i].length() == 0) continue;
         for (int j = 1; j < that.row.length; ++j) {
            double prod = dot (this.row[i], that.row[j]);
            if (prod != 0) mult.changeEntry (i, j, prod);
         }
      }
      return mult;
   }

   public String toString() {
      String mat = "";
      for (int i = 1; i < row.length; ++i) {
         if (row[i].length() == 0) continue;
         mat += Integer.toString (i) + ": " + row[i].toString() + "\n";
      }
      return mat;
   }

   private static double dot (List one, List two) {
      double prod = 0;
      one.moveFront(); 
      two.moveFront();
      while (one.index() >= 0 && two.index() >= 0) {
         Entry one_ = (Entry)one.get();
         Entry two_ = (Entry)two.get();
         if (one_.idx == two_.idx) {
            prod += one_.data * two_.data;
            one.moveNext();
            two.moveNext();
         }
         else if (one_.idx < two_.idx) one.moveNext();
         else if (one_.idx > two_.idx) two.moveNext();
      }
      return prod;
   }
}