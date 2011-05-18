CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -g -Wuninitialized

PROG = glsh
HDRS = tokenizer.h parser.h execute.h
SRCS = glsh.c tokenizer.c parser.c execute.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

glsh.o: glsh.c tokenizer.c parser.c execute.c
tokenizer.o: tokenizer.c tokenizer.h
parser.o: parser.c tokenizer.c tokenizer.h parser.h
execute.o: execute.c execute.h parser.h

clean:
	rm -f $(PROG) $(OBJS)
