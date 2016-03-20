#David Sun
#1357057
#CMPS 12B/M
#10/25/2014
#Makefile
#The Makefile builds CalcTest.class and Calc.class
#Contains phony targets to run the program

JAVASRC = Calc.java CalcTest.java
SOURCES = Makefile.unit ${JAVASRC}
CLASSES = Calc.class CalcTest.class
TESTCLASS = CalcTest
CLASSPATH = "junit-4.12-beta-1.jar:hamcrest-core-1.3.jar:."

all: ${CLASSES}

${CLASSES}: ${JAVASRC}
	javac -Xlint -cp ${CLASSPATH} ${JAVASRC}

clean:
	rm ${CLASSES}

test: ${CLASSES}
	java -cp ${CLASSPATH} org.junit.runner.JUnitCore ${TESTCLASS}

.PHONY: clean all test
