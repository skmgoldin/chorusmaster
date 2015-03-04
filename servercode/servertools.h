#ifndef SERVERTOOLS_H
#define SERVERTOOLS_H

int makeserver(char *port);

struct addrinfo * gethints(int fam, int socktype, int flags,
                           struct addrinfo *hints);

int getsock(int domain, int type, int protocol);

#endif
