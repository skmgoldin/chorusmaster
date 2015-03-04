#include <stdlib.h>
#include "servertools.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "requesthandler.h"

#define BACKLOG 10

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);

  int sock = makeserver(port); 

  while(1) {
    int clntsock = getconnection(sock);
  }

  return 0;
}

int getconnection(int sock) {

  int backlog = BACKLOG;
  struct sockaddr_storage *clntaddr = NULL;
  socklen_t clntaddrsize = sizeof(struct sockaddr_storage);

  listen(sock, backlog);
  int clntsock = accept(sock, (struct sockaddr *) clntaddr, &clntaddrsize);

  return clntsock;
}
