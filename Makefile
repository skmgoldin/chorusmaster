CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SRCS = server.c
OBJS = $(SRCS:.c=.o)
PREFIX = server/

PROG = Server

all: clean $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

%.o: $(PREFIX)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS)
