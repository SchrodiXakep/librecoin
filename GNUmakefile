## Copyright (C) 2015 Schrodi ##
# A basic makefile.
CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS =
IFLAGS =

librecoin : main.o
	$(CC) $(IFLAGS) $(LFLAGS) -o $@ $^

main.o : main.c
	$(CC) $(IFLAGS) $(CFLAGS) -o $@ -c $<

clear : clean
clean :
	rm -f librecoin main.o
