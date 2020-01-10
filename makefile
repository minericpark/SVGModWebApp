UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS= -L.

ifeq ($(UNAME), Linux)
	INC_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	INC_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif


all: SVGParser
	
liblist.so: LinkedListAPI.o
	$(CC) -shared -o bin/liblist.so bin/LinkedListAPI.o

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -o bin/LinkedListAPI.o

SVGParser: src/SVGParser.c include/SVGParser.h
	$(CC) $(CFLAGS) -I$(INC_PATH) src/SVGParser.c -lxml2 -o bin/SVGParser

clean:
	rm -rf bin/*.o bin/*.so