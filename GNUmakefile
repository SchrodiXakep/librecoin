## Copyright (C) 2015 Schrodi ##
# A basic makefile.
CC = gcc
CFLAGS = -Wall -Wextra -DBIG_JOINS=1 -fno-strict-aliasing -g -DNDEBUG
LFLAGS = -lconfig -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -ldl
IFLAGS = -I/usr/include/mysql

all : librecoin
librecoin : sqlfunc.o helpfile.o main.o
	$(CC) $(LFLAGS) -o $@ $^

main.o : main.c
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) -o $@ -c $<

helpfile.o : helpfile.c
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) -o $@ -c $<

sqlfunc.o : sqlfunc.c
	$(CC) $(IFLAGS) $(LFLAGS) $(CFLAGS) -o $@ -c $<

clear : clean
clean :
	rm -f librecoin main.o helpfile.o sqlfunc.o error.log
