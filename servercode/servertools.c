#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "servertools.h"

int makeserver(char *port) {

  struct addrinfo *servinfo = NULL;
  struct addrinfo *hints = malloc(sizeof(struct addrinfo));

  hints = gethints(AF_UNSPEC, SOCK_STREAM, AI_PASSIVE, hints);

  if(getaddrinfo(NULL, port, hints, &servinfo) != 0) {
    fprintf(stderr, "%s\n", "getaddrinfo failed.");
    exit(1);
  }

  int sock = getsock(servinfo->ai_family, servinfo->ai_socktype,
                     servinfo->ai_protocol);

  freeport(sock);
  if(bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
    fprintf(stderr, "%s\n", "Bind failed.");
    exit(1);
  }

  freeaddrinfo(hints);
  freeaddrinfo(servinfo);

  return sock;
}

struct addrinfo * gethints(int fam, int socktype, int flags,
                           struct addrinfo *hints) {

  memset(hints, 0, sizeof(struct addrinfo));

  hints->ai_family = fam;
  hints->ai_socktype = socktype;
  hints->ai_flags = flags;

  return hints;
}

int getsock(int domain, int type, int protocol) {
  int sock;

  if((sock = socket(domain, type, protocol)) < 0) {
    fprintf(stderr, "%s\n", "socket() failed.");
    exit(1);
  }

  return sock;
}

int freeport(int servsock) {

  int yes = 1;
  if(setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
    fprintf(stderr, "%s\n", "Failed to free requested port.");
    exit(1);
  }

  return 1;
}
