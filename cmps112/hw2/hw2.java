// David Sun
// CMPS-112 Homework 2
// Partner: none

// To run, type in command line arguments: javac Number.java
//                           		    then: java Number
// Compatible with eclipse IDE as well

import static java.lang.System.*;
import java.util.*;

class hw2 {
	
	public static void main (String[] args) {
		Scanner scan = new Scanner (in);
		out.println ("press q to quit");
		for (;;) {
			out.print ("sayNum: ");
			String num = scan.nextLine();
			if (num.equals ("q")) break;
			if (!num.matches ("[0-9]+")) {
				out.println ("invalid format");
				continue;
			}
			sayNum (num);
			out.println();
		}
	}
	
   /* hashmap with names of the powers of 3 */
	public static HashMap<Integer, String> names() {
		HashMap<Integer, String> ret = new HashMap<Integer, String>();
		for (int i = 0; i < 23; ++i) {
			switch (i) {
				case 0: ret.put (i, ""); break;
				case 1: ret.put (i, "thousand"); break;
				case 2: ret.put (i, "million"); break;
				case 3: ret.put (i, "billion"); break;
				case 4: ret.put (i, "trillion"); break;
				case 5: ret.put (i, "quadrillion");
				case 6: ret.put (i, "quintillion");
				case 7: ret.put (i, "sextillion");
				case 8: ret.put (i, "septillion");
				case 9: ret.put (i, "octillion");
				case 10: ret.put (i, "nonillion");
				case 11: ret.put (i, "decillion");
				case 12: ret.put (i, "undecillion");
				case 13: ret.put (i, "duodecillion");
				case 14: ret.put (i, "tredecillion");
				case 15: ret.put (i, "quattuordecillion");
				case 16: ret.put (i, "quindecillion");
				case 17: ret.put (i, "sexdecillion");
				case 18: ret.put (i, "septendecillion");
				case 19: ret.put (i, "octodecillion");
				case 20: ret.put (i, "novemdecillion");
				case 21: ret.put (i, "vigintillion");
			}
		}
		return ret;
	}
	
	public static grouping[] groups (String number) {
		while (number.length() % 3 != 0) number = "0" + number;
		String[] numGroup = number.split ("(?<=\\G...)");
		grouping[] gps = new grouping[numGroup.length];
		for (int i = numGroup.length - 1; i >= 0; i--) {
			grouping g = new grouping (numGroup[i]);
			gps[i] = g;
		}
		return gps;
	}
	public static void sayNum (String num) {
		grouping[] g = groups (num);
		HashMap<Integer, String> hm = names();
		for (int i = 0; i < g.length; ++i) {
			if (g[i].toString().length() == 0) continue;
			out.print (g[i].toString() + hm.get (g.length - i - 1) + " ");
		}
	}
}
/* grouping class, generates the string for groups of three digits */
class grouping {
	int fst, snd, thr;
	public grouping (String number) {
		thr = number.charAt (0) - 48;
		snd = number.charAt (1) - 48;
		fst = number.charAt (2) - 48;
	}
	public grouping() { fst = snd = thr = 0; }
	public String toString () {
		boolean teens = snd == 1;
		String retval = "";
		switch (fst) {
			case 0: retval = teens ? "ten" : ""; break;
			case 1: retval = teens ? "eleven" : "one"; break;
			case 2: retval = teens ? "twelve" : "two"; break;
			case 3: retval = teens ? "thirteen" : "three"; break;
			case 4: retval = teens ? "fourteen" : "four"; break;
			case 5: retval = teens ? "fifteen" : "five"; break;
			case 6: retval = teens ? "sixteen" : "six"; break;
			case 7: retval = teens ? "seventeen" : "seven"; break;
			case 8: retval = teens ? "eighteen" : "eight"; break;
			case 9: retval = teens ? "nineteen" : "nine"; break;
			default : break;
		}
		switch (snd) {
			case 2: retval = "twenty " + retval + " "; break;
			case 3: retval = "thirty " + retval + " "; break;
			case 4: retval = "forty " + retval + " "; break;
			case 5: retval = "fifty " + retval + " "; break;
			case 6: retval = "sixty " + retval + " "; break;
			case 7: retval = "seventy " + retval + " "; break;
			case 8: retval = "eighty " + retval + " "; break;
			case 9: retval = "ninety " + retval + " "; break;
			default: break;
		}
		switch (thr) {
			case 1: retval = "one hundred " + retval + " "; break;
			case 2: retval = "two hundred " + retval + " "; break; 
			case 3: retval = "three hundred " + retval + " "; break;
			case 4: retval = "four hundred " + retval + " "; break;
			case 5: retval = "five hundred " + retval + " "; break;
			case 6: retval = "six hundred " + retval + " "; break;
			case 7: retval = "seven hundred " + retval + " "; break;
			case 8: retval = "eight hundred " + retval + " "; break;
			case 9: retval = "nine hundred " + retval + " "; break;
		}
		return retval;
	}
}