CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
SERVSRCS = requesthandler.c authenticate.c logger.c messagerouting.c $(SHAREDSRCS)
SHAREDSRCS = wireio.c servertools.c msgvalidation.c candlemsg.c userlist.c conninfo.c sockdata.c msgnode.c
SERVOBJS = $(SERVSRCS:.c=.o)
SERVER = servercode/
SHARED = sharedcode/

SERVPROG = server

all: clean $(SERVPROG)

$(SERVPROG): $(SERVOBJS)
	$(LD) $(LDFLAGS) $(SERVOBJS) -o $(SERVPROG)

%.o: $(SERVER)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(SHARED)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(SERVPROG) 
