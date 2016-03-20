#David Sun
#1357057
#CMPS 12B/M
#11/19/2014
#Makefile
#Makefile builds the all the classes and contains 
#phony targets to remove class files
#I also added in another phony target called debug
#which runs the program with the -d flag for a file "test.txt"

JAVASRC    = xref.java auxlib.java Tree.java Queue.java
SOURCES    = ${JAVASRC} Makefile
ALLSOURCES = ${SOURCES}
MAINCLASS  = xref
CLASSES    = ${patsubst %.java, %.class, ${JAVASRC}}

all: ${CLASSES}

%.class: %.java
	javac -Xlint $<

clean:
	rm -f *.class

test: all
	java xref test.txt

debug: all
	java xref -d test.txt

.PHONY: clean all test debug
