#ifndef SERVERTOOLS_H
#define SERVERTOOLS_H

struct sockdata {
  int sock;
  struct addrinfo *servinfo;
  struct addrinfo *hints;
};

int makeserver(char *port);

struct sockdata *makesock(char *ip, char *port, struct sockdata *sockdata);

struct addrinfo * gethints(int fam, int socktype, int flags,
                           struct addrinfo *hints);

int getsock(int domain, int type, int protocol);

int getconnection(int sock);

int freeport(int servsock);

struct sockdata *allocsockdata();

int deallocsockdata(struct sockdata *sockdata);

#endif