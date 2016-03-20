/****************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 12/7/2014
 * Room.java
 * This file defines the properties of a Room object
 ****************************************************/

class Room {
	
	/***********************************************************************
	 * Basic premise for the room:
	 * 1) Each room has a name defined by String
	 * ---------------------------------------------------------------------
	 * 2) Each room may have an arbitrary amount of descriptions, defined by
	 *    the data type Queue, which in our case, contains a queue of String
	 * ---------------------------------------------------------------------   
	 * 3) Each room contains a queue of String[] of size 2, with the first 
	 * 	  index in the String array being the option the player gets to choose
	 *    and the second index being the destination tag
	 ***********************************************************************/

	private String name;
	private Queue<String> descriptions;
	private Queue<String[]> options;

	public Room(String name){
		this.name = name;
		this.descriptions = new Queue<String>();
		this.options = new Queue<String[]>();
	}
	
	public int compareTo(Room b){
		return this.name.compareTo(b.name);
	}
	
	//Insert an item into the queue
	public void insertDesc(String desc){
		this.descriptions.insert(desc);
	}
	
	/* Insert into the option queue */
	public void insertOpt(String[] optags){
		this.options.insert(optags);
	}
	
	/* Returns the name of the room */
	public String getName(){
		return this.name;
	}
	
	/* Print the descriptions of this room */
	public void printDescriptions(){
		for(String s : descriptions) 
			System.out.println(s + '\n');
	}
	
	/* Print all the options of this room */
	public void printOptions(){
		char c = 'a';
		for(String[] a : options){
			System.out.println(c + " - " + a[0]);
			c++;
		}
		System.out.println();
	}

	//The two methods below puts all options in an
	//array, and puts all tags in an array
	public String[] getOpts(){
		if(this.options.isEmpty()) return null;
		String[] opts = new String[options.getSize()];
		int i = 0;
		for(String[] optags : options){
			opts[i++] = optags[0];
		}
		return opts;
	}
	public String[] getTags(){
		if(this.options.isEmpty()) return null;
		String[] tags = new String[options.getSize()];
		int i = 0;
		for(String[] optags : options){
			tags[i++] = optags[1];
		}
		return tags;
	}
	
	/* Display the room */
	public void display(){
		printDescriptions();
		printOptions();
	}
}