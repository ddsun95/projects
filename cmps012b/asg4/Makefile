#David Sun
#1357057
#CMPS 12B/M
#Makefile
#This file builds all classes and runs the program
#Contains the phony target test to run cyoa.java with demo.adventure

JAVASRC    = Stack.java Room.java Queue.java cyoa.java
SOURCES    = ${JAVASRC} Makefile demo.adventure
ALLSOURCES = ${SOURCES}
MAINCLASS  = cyoa
CLASSES    = ${patsubst %.java, %.class, ${JAVASRC}}

all: ${CLASSES}

%.class: %.java
	javac -Xlint $<

clean:
	rm -f *.class

test: all
	java cyoa demo.adventure


.PHONY: clean all test