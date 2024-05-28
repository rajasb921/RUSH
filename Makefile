CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = rush.c
OBJS = $(SRCS:.c=.o)
DEPS = rush.h

all: rush

rush: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) rush
