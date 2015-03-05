CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SERVSRCS = servertools.c requesthandler.c wireio.c authenticate.c datastructures.c 
CLNTSRCS = datastructures.c candleclient.c
SERVOBJS = $(SERVSRCS:.c=.o)
CLNTOBJS = $(CLNTSRCS:.c=.o)
SERVER = servercode/
DS = datastructures/
CLNT = clientcode/

SERVPROG = server
CLNTPROG = client

all: clean $(SERVPROG) $(CLNTPROG)

$(SERVPROG): $(SERVOBJS)
	$(LD) $(LDFLAGS) $(SERVOBJS) -o $(SERVPROG)

$(CLNTPROG): $(CLNTOBJS)
	$(LD) $(LDFLAGS) $(CLNTOBJS) -o $(CLNTPROG)

%.o: $(SERVER)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(DS)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(CLNT)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(SERVPROG) $(CLNTPROG) 
