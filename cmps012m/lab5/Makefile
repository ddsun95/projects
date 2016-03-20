#David Sun
#1357057
#CMPS 12B/M
#11/15/2014
#Makefile
#Makefile that compiles the program
#clean removes all object files
#spotless removes all built files
#test compiles and runs the program in valgrind

CC = gcc
CFLAGS = -std=c99 -g -Wall -Werror -Wfatal-errors -g
OBJS = list.o
OUTPUT = list

${OUTPUT}: ${OBJS}
	${CC} $< -o ${OUTPUT}  

test:	${OUTPUT}
	valgrind ./${OUTPUT}
	      
clean:
	rm -f *.o

spotless: clean
	rm -f ${OUTPUT}


.PHONY: clean spotless test
