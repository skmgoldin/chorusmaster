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
#include <signal.h>
#include <time.h>

static int run = 1;

void sighandler() {
  run = 0; 
  exit(0);
}

int main(int argc, char **argv) {

  signal(SIGINT, sighandler);
   
  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);
  int sock = makeserver(port); 

  struct userlist *userlist = malloc(sizeof(struct userlist));
  userlist = inituserlist(userlist);

  struct userlist *loginlist = malloc(sizeof(struct userlist));
  loginlist = inituserlist(loginlist);

  struct userlist *lockoutlist = malloc(sizeof(struct userlist));
  lockoutlist = inituserlist(lockoutlist);

  while(run) {

    struct conninfo *conninfo = getconnection(sock); 

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    candlelog(candlemsg, conninfo);

    if(authenticate(candlemsg, userlist, loginlist, lockoutlist, conninfo)) {
      serverlog("User authenticated");
      handlerequest(candlemsg, userlist, conninfo);
    } else {
      serverlog("User authentication failed");
    }

    deallocconninfo(conninfo);
    dealloccandlemsg(candlemsg);
  }

  deinituserlist(userlist);
  deinituserlist(lockoutlist);
  return 0;
}

int handlerequest(struct candlemsg *candlemsg, struct userlist *userlist,
                  struct conninfo *conninfo) {

  /* Check user in. */
  finduser(candlemsg->from, userlist)->lastcheckin = time(NULL);

  if(strcmp(candlemsg->reqtype, LOGIN) == 0) {
    struct candlemsg *reply = alloccandlemsg();
    reply = packcandlemsg(reply, LOGIN, NULLFIELD, NULLFIELD, NULLFIELD, "1\n");
    sendcandlemsg(reply, conninfo->sock);
    dealloccandlemsg(reply);
    return 0;
  }
  
  return 0;
}


