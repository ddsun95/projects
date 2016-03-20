/*****************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 12/7/2014
 * cyoa.java
 * This file contains the code to execute an adventure
 *****************************************************/

import java.util.Scanner;
import java.util.NoSuchElementException;
import java.io.IOException;
import java.io.File;

class cyoa {
	
	public static final int EXIT_FAILURE = 130;	//Constant for exit failure
	public static final int EXIT_SUCCESS = 0;	//Constant for exit success
	public static final int INVALID = -1;		//Constant for invalid index
	public static Room[] rooms;					//Declaration of room array
	
	public static void main(String[] args) throws Exception{
		if(args.length != 1){	//Ensure proper command line arguments
			System.out.println("Usage: cyoa adventurefile");
			System.exit(EXIT_FAILURE);
		}
		//Process Rooms --------------------------------------------
		String filename = args[0];
		try{
			processRooms(filename); //Process the file
		}catch(IOException e){
			System.out.println("Invalid File!");
			System.exit(EXIT_FAILURE);			  
		}
		//----------------------------------------------------------
		String firstRoomName = rooms[0].getName();

		mergeSortRooms(rooms);	//Sort the rooms using merge-sort

		/* Define a constant value for the first room's index */
		final int firstRoomIndex = binSearch(firstRoomName, 0, rooms.length -1);

		/* Define a variable for the index in the array */
		int index = firstRoomIndex;

		System.out.println(); //Print a line, following the program specifications

		if(verifyAllTags()){  //Run the program if and only if all tags are verified
			boolean run = true;
			Scanner input = new Scanner(System.in);
			Stack stack = new Stack();
			Room thisRoom;
			while(run){
				thisRoom = rooms[index];
				thisRoom.display();
				char c = input.nextLine().charAt(0);
				switch(c){
					case 'a': case 'b': case 'c': case 'd':
					case 'e': case 'f': case 'g': case 'h':
					case 'i': case 'j': case 'k': case 'l':
						int i = c - 97;
						if(thisRoom.getTags() != null && i < thisRoom.getTags().length){
							stack.push(index);
							index = binSearch(thisRoom.getTags()[i], 0, rooms.length - 1);
							System.out.println
							("[" + thisRoom.getOpts()[i] + "]" + '\n');
						}else System.out.println("[Invalid Option.]" + '\n');
						break;
						
					case 'r': //Restart
						System.out.println("[Restart.]" + '\n'); 
						index = firstRoomIndex;	//Set the index back to the firstRoom
						stack.delete(); //Delete the stack
						break;
						
					case 'q': //Quit the game
						run = false;
						break;
						
					case 'y': //Information
						System.out.println("[Information.]" + '\n');
						for(int a = 0; a < rooms.length; a++){
							System.out.print(rooms[a].getName() + " : ");
							if(rooms[a].getTags() != null){
								for(int j = 0; j < rooms[a].getTags().length; j++){
									System.out.print(rooms[a].getTags()[j] + " ");
								}
							}
							System.out.println();
						}
						System.out.println();
						break;
						
					case 'z': //Undo by popping from the stack,
							  //If the stack is empty, catch
							  //the exception thrown by the empty stack.
						try{
							System.out.println("[Undo.]" + '\n');
							index = stack.pop();
						}catch (NoSuchElementException e){
							System.out.println("[Cannot Undo.]" + '\n');
						}
						break;

					default: //If inputs are invalid, print a message. 
						System.out.println("[Invalid Command.]" + '\n');
						break;
				}	
			}
			input.close();
		}else{
			System.out.println("You have Destination tags that are not valid!");
			System.exit(EXIT_FAILURE);
		}
		System.out.println("[quit]");
		System.exit(EXIT_SUCCESS);
	}
	
	/* Method that reads in the file
	 * For each "r [text]" increment the counter
	 * return the counter
	 */
	public static int roomCount(String filename) throws IOException{
		Scanner scan = new Scanner(new File(filename));
		int roomcount = 0;
		while(scan.hasNextLine()){
			String line = scan.nextLine();
			if(!line.equals(""))
				if(line.charAt(0) == 'r')
					roomcount++;
		}
		scan.close();
		return roomcount;
	}
	
	/* This method processes the rooms */
	public static void processRooms(String filename) throws IOException{
		Scanner scan = new Scanner(new File(filename));
		int optCount = 0;		/* Ensure number of rooms is fewer than 12 */
		boolean dflag = false;  /* Do not allow description tags if no new
								 * room has been created */
		int roomNumbers = roomCount(filename); //Fetch number of rooms
		int index = -1;
		rooms = new Room[roomNumbers];	//Allocate an array for the rooms
		while(scan.hasNextLine()){
			String line = scan.nextLine();
			//Process each line by the command
			if(!line.equals("")){
				char c = line.charAt(0);
				switch(c){
					case 'r': //Increment room index, create new Room object
							  //Set the option counter to zero
						rooms[++index] = new Room(strBuffer(line));	
						optCount = 0;								
						dflag = true; //Set to true if new room has been created							
						break;
						
					case 'd':
						if(!dflag){ //Error if no new room has been created
							System.out.println("Initialized description without " +
											   "creating new Room");
							
							/* EXIT WITH FAILURE CODE */
							System.exit(EXIT_FAILURE);
						}
						rooms[index].insertDesc(strBuffer(line));
						break;
						
					case 'o':
						dflag = false; //Do not allow any more descriptions
						optCount++;	   //Increment the option counter
						if(!scan.hasNextLine()){
							System.out.println("No lines following an option!");
							
							/* EXIT WITH FAILURE CODE */
							System.exit(EXIT_FAILURE);
						}else if(optCount > 12){ //Ensure less than 12 options
							System.out.println("You have more than twelve" + 
								               " options for a room!");

							/* EXIT WITH FAILURE CODE */
							System.exit(EXIT_FAILURE);
						}
						String tag = scan.nextLine();
						if(tag.charAt(0) != 't'){ //Ensure a destination tag follows
							System.out.println("No destination tag following an " +
								               "option! Fix your file then run "  +
								               "the program!");
							
							/* EXIT WITH FAILURE CODE */
							System.exit(EXIT_FAILURE);
						}
						String[] optags = {strBuffer(line), strBuffer(tag)};
						/* Insert into the queue the option, then the destination tag */
						rooms[index].insertOpt(optags);
						break;
						
					case 't':	//Ignore the tag commands as they are checked for
						break;
						
					case '#':	//Ignore any comments
						break;
						
					default:
						System.out.println("You have unsupported comands in your" +
								           " adventure file, fix your file then"  +
								           " run the program!");
						System.exit(EXIT_FAILURE);
						break;
				}
			}
		} 
		scan.close();
	}
		
	//-------------------------------------------------------------------------
	/* This algorithm sorts the room names by lexicographical order */
	public static void mergeSortRooms(Room[] r){
		if(r.length < 2) return;
		int mid = (r.length / 2);
		
		Room[] left =  new Room[mid];				//split the array into two
		Room[] right = new Room[r.length - mid];	//then sort both subarrays
		
		int i, j, k;
		for(i = 0; i < mid; i++){
			left[i] = r[i];
		}
		for(j = mid, k = 0; j < r.length && k < right.length; j++, k++){
			right[k] = r[j];
		}
		mergeSortRooms(left);
		mergeSortRooms(right);
		merge(left, right, r);						//finally, merge the arrays
	}
	/* MERGE SORT HELPER METHOD */
	public static void merge(Room[] left, Room[] right, Room[] merged){
		int llen = left.length;
		int rlen = right.length;
		int i = 0, j = 0, k = 0;
		while(i < llen && j < rlen){
			if(left[i].compareTo(right[j]) <= 0){
				merged[k++] = left[i++];
			}else{
				merged[k++] = right[j++];
			}
		}
		while(i < llen) merged[k++] = left[i++];
		while(j < rlen)	merged[k++] = right[j++];
	}
	//-------------------------------------------------------------------------
	/* Binary search method to search for appropriate tags */
	public static int binSearch(String key, int lo, int hi){
		if(hi >= lo){
			int mid = (lo + hi) / 2;
			if(key.compareTo(rooms[mid].getName()) > 0){
				return binSearch(key, mid + 1, hi);
			}else if(key.compareTo(rooms[mid].getName()) < 0){
				return binSearch(key, lo, mid - 1);
			}else{
				return mid;
			}
		}
		return INVALID; //NOT FOUND
	}
	//-------------------------------------------------------------------------
	/* Make sure all destination tags are valid */
	public static boolean verifyAllTags(){
		boolean allValidTags = true;
		for(int i = 0; i < rooms.length; i++){
			String[] tagArr = rooms[i].getTags();
			if(tagArr != null){
				for(int j = 0; j < tagArr.length; j++){
					if(binSearch(tagArr[j], 0, rooms.length - 1) == INVALID) 
						allValidTags = false;
				}
			}
		}
		return allValidTags;
	}
	//-------------------------------------------------------------------------
	/* Method prints all the rooms */
	public static void printRooms(){
		for(int i = 0; i < rooms.length; i++){
			System.out.println(rooms[i].getName());
		}
	}
	
	/* This String method returns all the words in the line
	 * without the first character and without any leading
	 * or trailing whitespace
	 */
	public static String strBuffer(String s){
		return(s.substring(1, s.length()).trim());
	}
}