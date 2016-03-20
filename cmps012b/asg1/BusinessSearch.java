/**
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 10/20/2014
 * BusinessSearch.java
 * BusinessSearch.java is contains the main function of   
 * the program which reads in a business database, sorts it, 
 * then checks whether or not a business exists in the database through user input
 */

import static java.lang.System.*;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileReader;

class BusinessSearch{
	public static void main(String[] args) throws IOException
	{
		/* checks whether or not the user has correctly entered contents onto command line */
		if(args.length != 1)
		{
			System.out.println("Usage: BusinessSearch businessDB");
			System.exit(1);		//terminate the program with an exit code of 1
		}

		/* reads the file from the command line */
		BufferedReader file = new BufferedReader(new FileReader(args[0]));

		/* declares a new BufferedReader object in which will be used later for business queries */
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));

		/* reads the first line of the file and converts the string into an integer */
		int numElems = Integer.parseInt(file.readLine());
		int i = 0; 	//Integer that keeps track of the business array index

		/* Declares an array of business with length numElems obtained from reading the first line of the file */
		Business[] busArray = new Business[numElems];

		/* Reads in the line for the file, terminates if there is no line */
		while(true){
			String line = file.readLine();

			if(line == null){
				break;
			}

			String[] nameNumber = line.split(",");	//split the line at a comma
			busArray[i++] = new Business(nameNumber[0], nameNumber[1]);
		}
		file.close();
		mergeSort(busArray);
		int searches = 0, notFound = 0;
		String in = input.readLine();
		while(!in.isEmpty()) //run so long as the input is not a blank entry
		{
			int index = binarySearch(busArray, in, 0, busArray.length - 1);
			if(index != -1)
			{
				System.out.println(busArray[index].PHONE_NUMBER);
				searches++;
			}
			else
			{
				System.out.println("NOT FOUND");
				searches++;
				notFound++;
			}
			in = input.readLine();
		}
		System.out.println(searches + " total queries, " + notFound + " not found.");
	}

	/* recursive binary search algorithm */
	public static int binarySearch(Business[] b, String target, int low, int high){
		if(high >= low){										//Base case
			int mid = (low + high) / 2;							
			if(target.toUpperCase().compareTo(b[mid].NAME.toUpperCase()) > 0)				//Target is in upper bound
			{
				/* Search upper bound */
				return binarySearch(b, target, mid + 1, high);
			} 
			else
			{
				if(target.toUpperCase().compareTo(b[mid].NAME.toUpperCase()) < 0)			//Target is in lower bound
				{
					/* Search lower bound */
					return binarySearch(b, target, low, mid - 1);
				} 
				else 
				{
					return mid;									//returns the middle index where the target was found
				}
			}
		} 		
		return -1;
	}															//returns -1, target not found

	/*
	* Merge sort algorithm that splits an array into halves, a left half and right half
	* the function then calls mergeSort recursively again on the left half and right half
	* once mergeSort has completed, the function will then merge the half arrays together into
	* the final array
	*/
	public static void mergeSort(Business[] b){
		if(b.length < 2) return;								//base case
		int half = (b.length/2);

		/** Creates two new arrays each half the length of array b **/
		Business[] left = new Business[half];
		Business[] right = new Business[b.length - half];

		int i, j, k;
		/* insert the elements of the left half into the left subarray */
		for(i = 0; i < half; i++)
		{
			left[i] = b[i];
		}

		/* insert the elements of the right half into the right subarray */
		for(j = half, k = 0; j < b.length && k < right.length; j++, k++)
		{
			right[k] = b[j];
		}

		mergeSort(left);		//sort left half
		mergeSort(right);		//sort right half
		merge(left, right, b);	//merge the two halves
	}


	/* function that merges subarrays */
	public static void merge(Business[] left, Business[] right, Business[] all){
		int leftLength = left.length;	
		int rightLength = right.length;
		int i = 0, j = 0, k = 0;
		while(i < leftLength && j < rightLength)
		{
			if(left[i].compareTo(right[j]) <= 0)	//if left elements are less than right elements, put the left elements onto the final array
			{
				all[k++] = left[i++];
			}
			else									//if right elements are left than left elements, put the right elements onto the final array
			{
				all[k++] = right[j++];
			}
		}
		/** should there be contents remaining on the left array
		 *  add the contents of the left array onto the final array or all[]
		 */
		while(i < leftLength)						
			all[k++] = left[i++];
		/* same logic with the code above, except this time for the right half of the array */
		while(j < rightLength)
			all[k++] = right[j++];
	}
}
