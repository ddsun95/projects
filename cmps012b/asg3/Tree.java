/***************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/19/2014
 * Tree.java
 * Tree.java contains the functions on the tree.
 * It contains functions that insert into the tree,
 * print the output of the tree,
 * and print the debug version of the output.
 ***************************************************/
import static java.lang.System.*;

class Tree {
    private class Node {
        String key;
        Queue<Integer> value;
        Node left;
        Node right;

        public Node(String key){
			this.key = key;
        }
    }
    
    private Node root;

    private void debugHelper(Node tree, int depth) {
    	if(tree == null) return;
   		String spaces = "";
   		for(int i = 0; i < depth; i++) spaces += "  ";     //add spaces for padding
    	debugHelper(tree.left, depth + 1);                 //traverse left first, increment depth by 1
    	out.printf("%s%d %s%n", spaces, depth, tree.key);  //print out middle element
    	debugHelper(tree.right, depth + 1);                //traverse right after, increment depth by 1
    }

    private void outputHelper(Node tree) {
    	if(tree == null) return;
    	outputHelper(tree.left);		//traverse left subtree first
    	out.printf("%s : ", tree.key);	//print out middle element
    	print(tree.value);				//print out the queue
    	outputHelper(tree.right);		//traverse the right subtree
    }

    /** Method for insert */
    public void insert(String key, Integer linenum) {
    	Node n = new Node(key);            //create new node
    	n.value = new Queue<Integer>();    //create the new node's queue
    	n.value.insert(linenum);           //insert the linenum into the queue
    	if(root == null){                  //create a new root node if none
    		root = n;
    	} else {
    		int comp = 0;
            Node t = root, b; /* Use two iterators since we
                               * do not have parent references
                               * let Node t denode tracker node
                               * and Node b denode backtracker since
                               * we do not have a parent reference
                               */
			while(t != null){
				comp = (key.compareTo(t.key));
				if(comp < 0){ //search left
					b = t;
					t = t.left;
                    if(t == null) b.left = n; //previous node's left child gets the new node
				} else if (comp > 0) { //search right
					b = t;
					t = t.right;          
                    if(t == null) b.right = n;
				} else if (comp == 0) {	 //keys are equal, insert into queue
					n = null;            //destroy the object   
					t.value.insert(linenum);
					return;
				}
			}
		}
    }

    /* Prints out all the elements in the queue */
    private void print(Queue<Integer> q){
    	for(Integer i : q) out.printf("%d ", i);
    	out.printf("%n");
    }
    
    /* Displays the debug output */
    public void debug() {
        debugHelper(root, 0);
    }
    
    /* Displays the regular output */
    public void output() {
        outputHelper(root);
    }
}
