UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS= -L./bin

ifeq ($(UNAME), Linux)
	INC_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	INC_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif


all: test
	
test: test.o liblist.so libsvgparser.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/test bin/test.o -llist -lxml2 -lsvgparser
	
test.o: src/main.c
	$(CC) $(CFLAGS) -I$(INC_PATH) -c src/main.c

liblist.so: LinkedListAPI.o
	$(CC) -shared -o bin/liblist.so bin/LinkedListAPI.o

LinkedListAPI.o: include/LinkedListAPI.h src/LinkedListAPI.c
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -o bin/LinkedListAPI.o

libsvgparser.so: SVGParser.o
	$(CC) -shared -o bin/libsvgparser.so bin/SVGParser.o

SVGParser.o: include/SVGParser.h src/SVGParser.c
	$(CC) $(CFLAGS) -I$(INC_PATH) -c -fpic src/SVGParser.c -lxml2 -o bin/SVGParser.o

clean:
	rm -rf bin/*.o bin/*.so

#Enter this line before running make -> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin