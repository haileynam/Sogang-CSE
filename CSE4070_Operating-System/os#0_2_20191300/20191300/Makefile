SRCS = bitmap.c debug.c hash.c hex_dump.c list.c user.c main.c
OBJS = $(SRCS:.c=.o)
PROG = testlib
CC = gcc
CFLAGS = -g

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): round.h

clean:
	rm -f $(OBJS) $(PROG)