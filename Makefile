CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic

PROG = glsh
HDRS = tokenize.h
SRCS = glsh.c tokenize.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

glsh.o: glsh.c tokenize.c
tokenize.o: tokenize.c tokenize.h

clean:
	rm -f $(PROG) $(OBJS)
