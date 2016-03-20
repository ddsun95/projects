/**
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 10/21/2014
 * Business.java
 * This file is the business class for the BusinessSearch program.
 */

class Business{

	public String NAME;				//Declares the name field for the business
	public String PHONE_NUMBER;		//Declares the phone number field for the business

	/* Constructor that initializes a business */
	public Business(String name, String number){
		this.NAME = name;
		this.PHONE_NUMBER = number;
	}

	/* compareTo method, important for sorting */
	public int compareTo(Business b){
		return this.NAME.compareTo(b.NAME);
	}

	/* toString method that returns the name and phone number on separate lines */
	public String toString(){
		return this.NAME + "\n" + this.PHONE_NUMBER;
	}
}