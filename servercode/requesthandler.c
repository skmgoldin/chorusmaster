#include <stdlib.h>
#include "servertools.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "requesthandler.h"


int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);

  int sock = makeserver(port); 

  while(1) {
    int clntsock = getconnection(sock);
    parserequest(clntsock);
  }

  return 0;
}


