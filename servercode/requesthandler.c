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
#include "messagerouting.h"

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


  if(strcmp(candlemsg->reqtype, LOGIN) == 0) {

    /* Check user in. */
    findusername(candlemsg->from, userlist)->lastcheckin = time(NULL);

    char *buf = malloc(sizeof(char) * MSGLEN);
    sprintf(buf, "%s%s", candlemsg->from, " logged in!");
    broadcast(buf, userlist);

    return 0;
  }
  
  if(strcmp(candlemsg->reqtype, BROADCAST) == 0) {
   
    /* Check user in. */
    findusid(candlemsg->from, userlist)->lastcheckin = time(NULL);

    char *buf = malloc(sizeof(char) * MSGLEN);
    sprintf(buf, "%s%s%s", findusid(candlemsg->from, userlist)->username, ": ", candlemsg->msg);
    broadcast(buf, userlist);
  }

  if(strcmp(candlemsg->reqtype, LOGOUT) == 0) {

    char *username = malloc(sizeof(char) * FROMLEN);
    username = strcpy(username, (findusid(candlemsg->from, userlist))->username);

    rmvusid(candlemsg->from, userlist);

    char *buf = malloc(sizeof(char) * MSGLEN);
    sprintf(buf, "%s%s", username, " logged out!");
    broadcast(buf, userlist);
    free(username);

    return 0;
  }
 
  return 0;
}

