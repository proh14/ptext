CC=gcc
CFLAGS=-Wall --debug -Wextra -pedantic -std=c99	
ptext: ptext.c
	$(CC) ptext.c -o ptext $(CFLAGS) 
