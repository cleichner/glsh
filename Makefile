CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -g -Wuninitialized

PROG = glsh
HDRS = tokenizer.h parser.h
SRCS = glsh.c tokenizer.c parser.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

glsh.o: glsh.c tokenizer.c parser.c
tokenizer.o: tokenizer.c tokenizer.h
parser.o: parser.c tokenizer.c tokenizer.h parser.h

clean:
	rm -f $(PROG) $(OBJS)
