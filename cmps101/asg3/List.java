// David Sun
// ddsun@ucsc.edu
// Pa3 (Sparse)
// List.java

import static java.lang.System.*;

class List {

   private class Node {
      Object data;
      Node next;
      Node prev;
      public Node (Object obj) { data = obj; next = prev = null; }
      public boolean equals (Object x) { 
         Node n = (Node)x;
         return this.data.equals (n.data); 
      }
      public String toString() { return data.toString(); }
   }

   private Node head, tail, cursor;
   private int length, index;

   public List() 
   { head = tail = cursor = null; length = 0; index = -1; }

   public int length() { return length; }

   public int index() { return index; }

   /* returns front element on the precondition length() > 0 */
   public Object front() {
      if (length() <= 0)
         throw new RuntimeException ("called front() on empty List"); 
      return head.data; 
   }

   /* returns back element on the precondition length() > 0 */
   public Object back() {
      if (length() <= 0)
         throw new RuntimeException ("called back() on empty List");
      return tail.data; 
   }

   public Object get() {
      if (length() <= 0)
         throw new RuntimeException ("called get() on empty List");
      if (index() < 0)
         throw new RuntimeException ("called get() on null cursor");
      return cursor.data; 
   }

   public boolean equals (Object x) {
      List that = (List)x;
      if (this.length != that.length) return false;
      for (Node t = this.head, u = that.head; t != null && u != null;
           t = t.next, u = u.next) if (!t.equals (u)) return false;
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
   void prepend (Object obj) {
      Node n = new Node (obj);
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
   void append (Object obj) {
      Node n = new Node (obj);
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
   void insertBefore (Object obj) {
      if (length() <= 0)
         throw new RuntimeException ("insertBefore() on empty list");
      if (index < 0)
         throw new RuntimeException ("insertBefore() on null cursor");
      Node n = new Node (obj);
      n.next = cursor;
      n.prev = cursor.prev;
      if (cursor == head) head = n;
      if (cursor.prev != null) cursor.prev.next = n;
      cursor.prev = n;
      length++;
   }

   /* insertAfter() with preconditions length() > 0 and index() >= 0 */
   void insertAfter (Object obj) {
      if (length() <= 0)
         throw new RuntimeException ("insertBefore() on empty list");
      if (index < 0)
         throw new RuntimeException ("insertBefore() on null cursor");
      Node n = new Node (obj);
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
   
   public String toString() {
      String result = "";
      for (Node t = head; t != null; t = t.next) result += t.toString() + " ";
      return result.trim();
   }
}
