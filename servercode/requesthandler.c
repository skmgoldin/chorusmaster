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
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include "userstatus.h"

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);
  int sock = makeserver(port); 

  struct liveuserlist *liveuserlist = malloc(sizeof(struct liveuserslist));
  liveuserlist = initliveuserslist(liveuserlist);

  while(1) {

    int clntsock = getconnection(sock); // Update getconnection() to return struct sockdata.

    struct candlemsg *candlemsg = readcandlemsg(clntsock);

    candlelog(candlemsg);

    if(authenticate(candlemsg->from, candlemsg->msg)) {
      serverlog("User authenticated.");
      handlerequest(candlemsg);
    }
    else { serverlog("User authentication failed."); }

    close(clntsock); // Could this be closed earlier, as soon as the candlemessage is read?
    dealloccandlemsg(candlemsg);
  }

  return 0;
}

int handlerequest(struct candlemsg *candlemsg) {

  if(strcmp(candlemsg->reqtype, LOGIN) == 0) {
    userlogin(candlemsg->from);
    return 0;
  }
  
  return 0;
}


