/**
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 10/25/2014
 * Calc.java
 * This file contains the methods for CalcTest.java
 */

// Calc.java
// Define a class for doing RPN.

import java.util.*;
import java.io.*;

public class Calc {

    private double[] stack;
    private int index;
    private int size;

    // Constructor
    public Calc() {
        this.stack = new double[100];    //initializes double array of size 100
        this.index = -1;                 //no elements yet in the stack
        this.size = 0;                   //keep track of the size of stack
    }
    
    // Push a number
    public void push(double x) {
        if(depth() >= 100){
            throw new RuntimeException();
        }
            ++size;
            stack[++index] = x;

    }
    
    // Pop top number (removes)
    public double pop(){
        if(depth() < 1){
            throw new RuntimeException();
        }
            --size;
            return stack[index--];

    }
    
    // Peek at top number (does not remove)
    public double peek() {
        if (depth() < 1){
            throw new RuntimeException();
        }

            return stack[index];
        }

    
    // Add top two numbers
    public void add(){
        if(depth() < 2){
            throw new RuntimeException();
        }
            double x = pop();
            double y = pop();
            push(y + x);
    }
    
    // Subtract top two numbers (top on right side)
    public void subtract(){
        if(depth() < 2){
            throw new RuntimeException();
        }
            double x = pop();
            double y = pop();
            push(y - x);

    }

    // Multiply top two numbers
    public void multiply(){
        if(depth() < 2){
            throw new RuntimeException();
        }
            double x = pop();
            double y = pop();
            push(y * x);

    }
    
    // Divide top two numbers (top of stack is the denominator)
    public void divide(){
        if(depth() < 2){
            throw new RuntimeException();
        }
            double x = pop();
            double y = pop();

        if(x != 0)
        {
            push(y / x);
        }
        else
        {
            throw new RuntimeException();
        }

    }
    
    // Return how many numbers are in the stack
    public int depth() {
        return size;
    }

    // Returns the reciprocal of the top element of the stack
    public void reciprocal(){
        if(depth() < 1 || peek() == 0){
            throw new RuntimeException();
        }
        else
        {
            double x = pop();
            push(1 / x);
        }
    }
}
