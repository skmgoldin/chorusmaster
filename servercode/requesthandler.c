#include <stdlib.h>
#include "../sharedcode/servertools.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "requesthandler.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/datastructures.h"
#include "authenticate.h"
#include <unistd.h>
#include "logger.h"

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);
  int sock = makeserver(port); 

  while(1) {

    int clntsock = getconnection(sock); // Update getconnection() to return struct sockdata.

    struct candlemsg *candlemsg = readcandlemsg(clntsock); 

    candlelog(candlemsg);

    if(authenticate(candlemsg->from, candlemsg->msg)) {
      serverlog("User authenticated.");
      handlerequest(clntsock);
    }
    else { serverlog("User authentication failed."); }

    close(clntsock);
    dealloccandlemsg(candlemsg);
  }

  return 0;
}

int handlerequest(clntsock) {
  
  return 0;
}


