/********************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/19/2014
 * Queue.java
 * This file contains the functions for the generic queue
 * It also implements the iterator interface to allow for
 * easier printing of the queue
 ********************************************************/
import java.util.Iterator;
import java.util.NoSuchElementException;

class Queue <Item> implements Iterable <Item> {

   private class Node {
      Item item;
      Node next;
   }
   private Node head = null;
   private Node tail = null;

   public boolean isempty() {
      return(head == null);
   }

   public void insert(Item newitem) {
      Node n = new Node(); //create new node
      n.item = newitem;
      if(isempty()){       //if list is empty, set head and tail to point at new node
         head = n;
         tail = n;
      } else {
         tail.next = n;    //insert as last element
         tail = tail.next;
      }
   }

   public Iterator <Item> iterator() {
      return new Itor ();
   }

   class Itor implements Iterator <Item> {
      Node current = head;
      public boolean hasNext() {
         return current != null;
      }
      public Item next() {
         if (! hasNext ()) throw new NoSuchElementException();
         Item result = current.item;
         current = current.next;
         return result;
      }
      public void remove() {
         throw new UnsupportedOperationException();
      }
   }
}
