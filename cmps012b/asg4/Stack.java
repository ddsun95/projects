/************************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 12/7/2014
 * Stack.java
 * This file is the implementation stack 
 * which stores room index of the array
 ************************************************************/
/************************************************************
 * This is the class for the Stack						 	* 	  	
 * After the player makes a move to move to a room			*   
 * the array index of the room is pushed onto the 			*	
 * stack as the form of an integer primitive type			*
 * --------------------------------------------				*
 * This way if the player wishes to undo a move,			*	
 * an item gets popped from the stack and the index pointer	*
 * points to the array index given by the top of the stack	*
 ************************************************************/
import java.util.NoSuchElementException;
class Stack {
	
	class Node{
		int item;
		Node next;
	}
	
	private Node head;
	
	public Stack(){
		this.head = null;
	}
	
	public boolean isEmpty(){
		return(head == null);
	}
	
	public void push(int e){
		Node n = new Node();
		n.item = e;
		n.next = head;
		head   = n;
	}
	
	public int pop(){
		if(!isEmpty()){
			int retVal = head.item;
			head = head.next;
			return retVal;
		}else{
			throw new NoSuchElementException();
		}
	}
	
	//Eliminate all references
	//to the stack list thereby 
	//deleting the list
	public void delete(){
		head = null;
	}
}
