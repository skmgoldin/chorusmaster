CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SERVSRCS = requesthandler.c authenticate.c logger.c userstatus.c $(SHAREDSRCS)
CLNTSRCS = candleclient.c $(SHAREDSRCS)
SHAREDSRCS = datastructures.c wireio.c servertools.c msgvalidation.c
SERVOBJS = $(SERVSRCS:.c=.o)
CLNTOBJS = $(CLNTSRCS:.c=.o)
SERVER = servercode/
SHARED = sharedcode/
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

%.o: $(SHARED)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(CLNT)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(SERVPROG) $(CLNTPROG) 
