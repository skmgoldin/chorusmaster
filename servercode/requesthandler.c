#include <stdlib.h>
#include "../sharedcode/servertools.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "requesthandler.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/userlist.h"
#include "authenticate.h"
#include <unistd.h>
#include "logger.h"
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/sockdata.h"
#include "../sharedcode/conninfo.h"

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);
  int sock = makeserver(port); 

  struct userlist *userlist = malloc(sizeof(struct userlist));
  userlist = inituserlist(userlist);

  struct userlist *lockoutlist = malloc(sizeof(struct userlist));
  lockoutlist = inituserlist(lockoutlist);

  while(1) {

    struct conninfo *conninfo = getconnection(sock); 

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    candlelog(candlemsg, conninfo);

    if(authenticate(candlemsg, userlist, lockoutlist, conninfo)) {
      serverlog("User authenticated");
      handlerequest(candlemsg, userlist, conninfo);
    } else {
      serverlog("User authentication failed");
    }

    deallocconninfo(conninfo);
    dealloccandlemsg(candlemsg);
  }

  deinituserlist(userlist);
  return 0;
}

int handlerequest(struct candlemsg *candlemsg, struct userlist *userlist,
                  struct conninfo *conninfo) {

  if(strcmp(candlemsg->reqtype, LOGIN) == 0) {
    struct candlemsg *reply = alloccandlemsg();
    reply = packcandlemsg(reply, LOGIN, NULLFIELD, NULLFIELD, NULLFIELD, "1\n");
    sendcandlemsg(reply, conninfo->sock);
    return 0;
  }
  
  return 0;
}


