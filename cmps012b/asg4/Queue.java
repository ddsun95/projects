/*****************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 12/7/2014
 * Queue.java
 * This file contains the implementation of the Queue 
 * which is used for storing descriptions and
 * Options/Destination tags.
 *****************************************************/
import java.util.Iterator;
import java.util.NoSuchElementException;

class Queue<E> implements Iterable<E> {
	private class Node{
		E elem;
		Node next;
	}
	
	private int size = 0;	  //Size counter
	private Node head = null; //Beginning of the list
	private Node tail = null; //End of the list
	
	/* Boolean for empty list */
	public boolean isEmpty(){
		return(head == null);
	}
	
	/* Insert into the Queue */
	public void insert(E elem){
		Node n = new Node();
		n.elem = elem;
		if(isEmpty()){
			head = n;
			tail = n;
		}else{
			tail.next = n;
			tail = tail.next;
		}
		size++;
	}
	
	public Iterator <E> iterator(){
		return new Itor();
	}
	
	/* Returns the size of the list */
	public int getSize(){
		return this.size;
	}

   /* Iterator interface useful for printing elements */
   class Itor implements Iterator <E>{
      Node current = head;
      public boolean hasNext(){
         return current != null;
      }
      public E next(){
         if (!hasNext()) throw new NoSuchElementException();
         E result = current.elem;
         current = current.next;
         return result;
      }
      public void remove(){
         throw new UnsupportedOperationException();
      }
   }
}
