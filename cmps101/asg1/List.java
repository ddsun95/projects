// David Sun
// ddsun@ucsc.edu (1357057)
// pa1 (Lex)

import static java.lang.System.*;

class List {

   private class Node {
      int data;
      Node next;
      Node prev;
      
      /* Node constructor */
      public Node (int data) 
      { this.data = data; next = null; prev = null; }

      public String toString() 
      { return Integer.toString(data); }
   }

   private Node head;
   private Node tail;
   private Node cursor;
   private int length;
   private int index;

   public List() 
   { head = tail = cursor = null; length = 0; index = -1; }

   public int length() { return length; }

   public int index() { return index; }

   /* returns front element on the precondition length() > 0 */
   public int front() {
      if (length() <= 0)
         throw new RuntimeException ("called front() on empty List"); 
      return head.data; 
   }

   /* returns back element on the precondition length() > 0 */
   public int back() {
      if (length() <= 0)
         throw new RuntimeException ("called back() on empty List");
      return tail.data; 
   }

   public int get() {
      if (length() <= 0)
         throw new RuntimeException ("called get() on empty List");
      if (index() < 0)
         throw new RuntimeException ("called get() on null cursor");
      return cursor.data; 
   }


   public boolean equals (List L) {
      if (this.length != L.length) return false;
      for (Node a = this.head, b = L.head; a != null && b != null;
           a = a.next, b = b.next) if (a.data != b.data) return false;
      return true;
   }

   void clear() { head = tail = cursor = null; length = 0; index = -1; }

   /* moves cursor to the first element if length() > 0 */
   void moveFront() { 
      if (length() > 0) { cursor = head; index = 0; }
   }

   /* moves cursor to the last element if length() > 0 */
   void moveBack() { 
      if (length() > 0) { cursor = tail; index = length - 1; }
   }

   /* move cursor to previous element */
   void movePrev() {
      if (cursor != null) {
         cursor = cursor.prev;
         index = cursor == null ? -1 : index - 1;
      }
   }

   /* move cursor to next element */
   void moveNext() {
      if (cursor != null) {
         cursor = cursor.next;
         index = cursor == null ? -1 : index + 1;
      }
   }

   /* add element to list, if list is non-empty, add element as first */
   void prepend (int data) {
      Node n = new Node (data);
      if (length() == 0) {
         head = tail = n;
      } else {
         n.next = head;
         n.next.prev = n;
         head = n;
      }
      length++;
   }

   /* add element to list, if list is non-empty, add element as last */
   void append (int data) {
      Node n = new Node (data);
      if (length() == 0) {
         head = tail = n;
      } else {
         tail.next = n;
         n.prev = tail;
         tail = tail.next;
      }
      length++;
   }

   /* insertBefore() with preconditions length() > 0 and index() >= 0 */
   void insertBefore (int data) {
      if (length() <= 0)
         throw new RuntimeException ("insertBefore() on empty list");
      if (index < 0)
         throw new RuntimeException ("insertBefore() on null cursor");
      Node n = new Node (data);
      n.next = cursor;
      n.prev = cursor.prev;
      if (cursor == head) head = n;
      if (cursor.prev != null) cursor.prev.next = n;
      cursor.prev = n;
      length++;
   }

   /* insertAfter() with preconditions length() > 0 and index() >= 0 */
   void insertAfter (int data) {
      if (length() <= 0)
         throw new RuntimeException ("insertBefore() on empty list");
      if (index < 0)
         throw new RuntimeException ("insertBefore() on null cursor");
      Node n = new Node (data);
      n.prev = cursor;
      n.next = cursor.next;
      if (cursor == tail) tail = n;
      if (cursor.next != null) cursor.next.prev = n;
      cursor.next = n;
      length++;
   }

   /* deleteFront() with precondition that length() > 0 */
   void deleteFront() {
      if (length <= 0) 
         throw new RuntimeException ("deleteBack() on empty list");
      if (cursor == head) { cursor = null; index = -1; }
      if (tail == head) tail = null;
      head = head.next;
      if (head != null) head.prev = null;
      length--;
      return;
   }

   /* deleteBack() with precondition that length() > 0 */
   void deleteBack() {
      if (length <= 0) 
         throw new RuntimeException ("deleteBack() on empty list");
      if (cursor == tail) { cursor = null; index = -1; }
      if (head == tail) head = null;
      tail = tail.prev;
      if (tail != null) tail.next = null;
      length--;
   }

   /* delete() on the precondition length() > 0 and index() >= 0 */
   void delete() {
      if (length() <= 0)
         throw new RuntimeException ("delete() on empty list");
      if (index() < 0)
         throw new RuntimeException ("delete() on null cursor");
      if (cursor == tail) deleteBack();
      else if (cursor == head) deleteFront();
      else {
         cursor.prev.next = cursor.next;
         cursor.next.prev = cursor.prev;
         cursor = null;
         index = -1;
         length--;
      }
   }

   /* returns a copy of the list */
   public List copy() {
      List L = new List();
      for (Node t = head; t != null; t = t.next) L.append (t.data);
      return L;
   }

   /* concats current list with another */
   public List concat (List L) {
      List R = new List();
      for (Node t = head; t != null; t = t.next) R.append (t.data);
      for (Node u = L.head; u != null; u = u.next) R.append (u.data);
      return R;
   }

   public String toString() {
      String result = "";
      for (Node t = head; t != null; t = t.next) 
         result += t.toString() + " ";
      return result.trim();
   }
}
