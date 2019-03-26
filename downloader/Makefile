.PHONY: clean

all: downloader

CC=gcc
CFLAGS=-g -Wall

downloader: downloader.o
	$(CC) $(CFLAGS) -o $@ $^ 

downloader.o: downloader.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@rm -rf *.o downloader
	@rm -rf download-*
