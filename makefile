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


all: test
	
test: test.o liblist.so libsvgparser.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o test test.o  -llist
	
test.o: src/main.c
	$(CC) $(CFLAGS) -I$(INC_PATH) -c src/main.c -lxml2

liblist.so: LinkedListAPI.o
	$(CC) -shared -o bin/liblist.so bin/LinkedListAPI.o

LinkedListAPI.o: include/LinkedListAPI.h src/LinkedListAPI.c
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c

libsvgparser.so: SVGParser.o
	$(CC) -shared -o bin/libsvgparser.so bin/SVGParser.o

SVGParser.o: include/SVGParser.h src/SVGParser.c
	$(CC) $(CFLAGS) -c -fpic src/SVGParser.c

clean:
	rm -rf bin/*.o bin/*.so

#Enter this line before running make -> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.