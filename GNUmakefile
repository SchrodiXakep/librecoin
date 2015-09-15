## Copyright (C) 2015 Schrodi ##
# A basic makefile.
CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS = -lmysqlclient -lconfig
IFLAGS = -I/usr/include/mysql

librecoin : helpfile.o main.o
	$(CC) $(LFLAGS) -o $@ $^

main.o : main.c
	$(CC) $(IFLAGS) $(CFLAGS) -o $@ -c $<

helpfile.o : helpfile.c
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) -o $@ -c $<

clear : clean
clean :
	rm -f librecoin main.o helpfile.o
