#David Sun
#1357057
#CMPS 12B/M
#10/21/2014
#Makefile
#This is the Makefile that compiles BusinessSearch.java
#and Business.java, it creates a JARFILE called BusinessSearch


JAVASRC = BusinessSearch.java Business.java
SOURCES = README makefile ${JAVASRC}
MAINCLASS = BusinessSearch
CLASSES = BusinessSearch.class Business.class
JARFILE = BusinessSearch
JARCLASSES = ${CLASSES}

all: ${JARFILE}

${JARFILE}: ${CLASSES}
	echo "Main-class: ${MAINCLASS}" > Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	rm Manifest
	chmod +x ${JARFILE}

${CLASSES}: ${JAVASRC}
	javac -Xlint ${JAVASRC}
clean:
	rm ${CLASSES} ${JARFILE}

.PHONY: clean all