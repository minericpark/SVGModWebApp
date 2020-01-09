UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g
LDFLAGS= -L.

ifeq ($(UNAME), Linux)
	INC_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	INC_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif


all: SVGParser
	
SVGParser: SVGParser.o liblist.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o SVGParser SVGParser.o  -llist
	
SVGParser.o: SVGParser.c
	$(CC) $(CFLAGS) -c SVGParser.c 
	
liblist.so: LinkedListAPI.o
	$(CC) -shared -o liblist.so LinkedListAPI.o

LinkedListAPI.o: LinkedListAPI.c LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic LinkedListAPI.c

xmlExample: libXmlExample.c
	$(CC) $(CFLAGS) -I$(INC_PATH) libXmlExample.c -lxml2 -o xmlExample

clean:
	rm -rf SVGParser xmlExample *.o *.so