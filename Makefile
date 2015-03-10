CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SERVSRCS = requesthandler.c authenticate.c logger.c $(SHAREDSRCS)
CLNTSRCS = candleclient.c $(SHAREDSRCS)
CLNTLSTNSRCS = clientlistener.c $(SHAREDSRCS)
SHAREDSRCS = wireio.c servertools.c msgvalidation.c candlemsg.c userlist.c conninfo.c sockdata.c
SERVOBJS = $(SERVSRCS:.c=.o)
CLNTOBJS = $(CLNTSRCS:.c=.o)
CLNTLSTNOBJS = $(CLNTLSTNSRCS:.c=.o)
SERVER = servercode/
SHARED = sharedcode/
CLNT = clientcode/

SERVPROG = server
CLNTPROG = client
CLNTLSTNPROG = clientlistener

all: clean $(SERVPROG) $(CLNTPROG) $(CLNTLSTNPROG)

$(SERVPROG): $(SERVOBJS)
	$(LD) $(LDFLAGS) $(SERVOBJS) -o $(SERVPROG)

$(CLNTPROG): $(CLNTOBJS)
	$(LD) $(LDFLAGS) $(CLNTOBJS) -o $(CLNTPROG)

$(CLNTLSTNPROG): $(CLNTLSTNOBJS)
	$(LD) $(LDFLAGS) $(CLNTLSTNOBJS) -o $(CLNTLSTNPROG)

%.o: $(SERVER)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(SHARED)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(CLNT)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(SERVPROG) $(CLNTPROG) $(CLNTLSTNPROG) 
