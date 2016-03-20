
/********************************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/5/2014
 * dllist.java
 * dllist.java is the linked list class
 * it contains all the functions pertaining to the doubly linked list
 ********************************************************************/
import java.io.*;
import java.util.*;
public class dllist {

   public enum position {FIRST, PREVIOUS, FOLLOWING, LAST};

   private class node {
      String item;
      node prev;
      node next;
   }

   private node first = null;
   private node current = null;
   private node last = null;
   private int currentPosition = 0;

   public void setPosition (position pos) {

      /* ignore any command that attempts to call previous when current == first
         or any command that attempts to call following when current == last  */
      if((current == first && pos == position.PREVIOUS) || 
         (current == last && pos == position.FOLLOWING)){
         return;
      }

      /* sets the current reference to the first object */
      if(pos == position.FIRST){
         current = first;
      }

      /* sets the current reference to the object before */
      if(pos == position.PREVIOUS){
         current = current.prev;
      }

      /* sets the current reference to the node after */
      if(pos == position.FOLLOWING){
         current = current.next;
      }

      /* sets the current reference to the last object */
      if(pos == position.LAST){
         current = last;
      }
   }

   /* If the first pointer is null, there are no elements in the list */
   public boolean isEmpty () {
   	return (first == null);
   }

   /* returns the item at the current reference, throws a
      NoSuchElementException if the list is empty */
   public String getItem () {
      if(isEmpty()){
         throw new NoSuchElementException();
      } else {
         return current.item;
      }
   }

   /* Start at the beginning of the list
    * Iterate through each item, counting the position
    * stop when the current position is the target
    * returns the iterations */
   public int getPosition () {
      if(isEmpty()){
      	throw new NoSuchElementException();
      } else {
      	currentPosition = 0;
      	for(node t = first; t != current; t = t.next){
      		currentPosition++;
      	}
      }
      return currentPosition;
   }

   /* Deletes a node */
   public void delete () {
    	if(isEmpty()) {    
      	throw new NoSuchElementException();
    	}else if(current == first && current == last){ //set all pointers to null if there is one element
      	first = null;
      	current = null;
      	last = null;
    	}else if(current == last){ //deletes last, sets the last item to the item before last
      	last = last.prev;
      	last.next = null;
      	current = last;
    	}else if(current == first){ //deletes first, sets the first item to the item after old first
     	 	first = first.next;
     		first.prev = null;
      	current = first;
    	}else{ //delets the current item, linking the node before to the node after
      	current.prev.next = current.next;
      	current.next.prev = current.prev;
      	current = current.next;
    	}
   }

   public void insert (String item, position pos) {
   	if(isEmpty() && (pos == position.PREVIOUS || pos == position.FOLLOWING)){
   	  	  throw new IllegalArgumentException();	
   	}

      node n = new node();   //creates a new node
      n.item = item;         //set the item equal to the argument

      /* sets first, current, last references to the new node
         since there is only one element in the list */   
      if(isEmpty()){
         first = n;                
         current = n;
         last = n;
      }

      /* insert as first element */
		if(pos == position.FIRST){
	    	n.next = first; 
	    	n.prev = null;
	    	first.prev = n;
	    	first = n;
	    	current = n;
		} 

      /* if the current position is the first element
       * insert the item as as the first element
       * otherwise stick the item before the current element */
      if(pos == position.PREVIOUS){
    	 	if(current == first) {
    		  	n.next = first; 
        		n.prev = null;
         	first.prev = n;
         	first = n;
        		current = n;
    	 	} else {
         	n.next = current;
         	n.prev = current.prev;
        		current.prev.next = n;
        		current.prev = n;
        		current = n;
     	  	}
      }

      /* if the current position is the last element
       * insert the item as the last element
	    * otherwise stick the item after the current element */
      if(pos == position.FOLLOWING){
      	if(current == last){
        		n.prev = last;
         	n.next = null;
        		last.next = n;
         	last = n;
        		current = n;
         } else {
        		n.next = current.next;
        		n.prev = current;
        		current.next.prev = n;
        		current.next = n;
        		current = n;
     		}
    	}

      /* insert as last element */
      if(pos == position.LAST){
        	n.prev = last;
         n.next = null;
         last.next = n;
         last = n;
         current = n;
      }
   }
}

