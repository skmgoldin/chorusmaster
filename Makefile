CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SRCS = servertools.c requesthandler.c wireio.c authenticate.c
OBJS = $(SRCS:.c=.o)
PREFIX = servercode/

PROG = server

all: clean $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

%.o: $(PREFIX)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS)
