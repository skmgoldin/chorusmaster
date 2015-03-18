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
#include "../sharedcode/msgnode.h"

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

  struct msglist *pending = malloc(sizeof(struct msglist));
  pending->head = NULL;

  while(run) {

    struct conninfo *conninfo = getconnection(sock); 

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    candlelog(candlemsg, conninfo);

    if(authenticate(candlemsg, userlist, loginlist, lockoutlist, conninfo)) {
      serverlog("User authenticated");
      handlerequest(candlemsg, userlist, conninfo, pending);
    } else {
      serverlog("User authentication failed");
    }

    deallocconninfo(conninfo); // Connection is closed here.
    dealloccandlemsg(candlemsg);
  }

  deinituserlist(userlist);
  deinituserlist(lockoutlist);
  return 0;
}

int handlerequest(struct candlemsg *candlemsg, struct userlist *userlist,
                  struct conninfo *conninfo, struct msglist *pending) {


  if(strcmp(candlemsg->reqtype, LOGIN) == 0) {

    /* Check user in. */
    findusername(candlemsg->from, userlist)->lastcheckin = time(NULL);

    deliverpending(findusername(candlemsg->from, userlist), pending);

    char *buf = malloc(sizeof(char) * MSGLEN);
    sprintf(buf, "%s%s", candlemsg->from, " logged in!");
    broadcast(buf, userlist);

    return 0;
  }
  
  if(strcmp(candlemsg->reqtype, GETADDRESS) == 0) {
 
    /* Check user in. */
    findusid(candlemsg->from, userlist)->lastcheckin = time(NULL);

    struct usernode *user = findusername(candlemsg->msg, userlist);

    struct candlemsg *reply = alloccandlemsg();
    reply = packcandlemsg(reply, GETADDRESS, user->port, NULLFIELD, user->username, user->ip); //Hacky, unintended use of struct fields.
    sendcandlemsg(reply, conninfo->sock);

    dealloccandlemsg(reply);

    return 0;
  }
 
  if(strcmp(candlemsg->reqtype, ONLINE) == 0) {
 
    /* Check user in. */
    findusid(candlemsg->from, userlist)->lastcheckin = time(NULL);

    struct usernode *currnode = userlist->head;
    while(currnode != NULL) {

      char *buf = malloc(sizeof(char) * MSGLEN);
      sprintf(buf, "%s", currnode->username);

      struct candlemsg *message = alloccandlemsg();
      message = packcandlemsg(message, ONLINE, NULLFIELD,
                              NULLFIELD, NULLFIELD, buf);
      dealloccandlemsg(candleexchange(message, findusid(candlemsg->from, userlist)->ip,
                                      findusid(candlemsg->from, userlist)->port));
      dealloccandlemsg(message);

      free(buf);

      currnode = currnode->next;

    }

    return 0;
  }

  if(strcmp(candlemsg->reqtype, BROADCAST) == 0) {
   
    /* Check user in. */
    findusid(candlemsg->from, userlist)->lastcheckin = time(NULL);

    char *buf = malloc(sizeof(char) * MSGLEN);
    sprintf(buf, "%s%s%s", findusid(candlemsg->from, userlist)->username, ": ", candlemsg->msg);
    broadcast(buf, userlist);

    return 0;
  }

  if(strcmp(candlemsg->reqtype, MESSAGE) == 0) {
   
    /* Check user in. */
    findusid(candlemsg->from, userlist)->lastcheckin = time(NULL);

    /* Tokenize first word in message (the recipient). */
    int i;                                                                        
    for(i = 0; *(candlemsg->msg + i) != ' '; i++) {;}                                      
    char *deliverto = malloc(sizeof(char) * (i + 1));                               
    strncpy(deliverto, candlemsg->msg, i);                                                   
    *(deliverto + i) = '\0'; 

    /* Prepare message */
    struct candlemsg *message = alloccandlemsg();
    message = packcandlemsg(message, MESSAGE, NULLFIELD, usidtousername(candlemsg->from, userlist), NULLFIELD, candlemsg->msg);

    struct usernode *delivernode = findusername(deliverto, userlist);
    if(delivernode == NULL) {
      /* Intended recipient is not logged in. */

      addpendingmsg(pending, message);

      char *buf = malloc(sizeof(char) * MSGLEN);
      sprintf(buf, "%s%s", deliverto, " is not online. Your message will be delivered when they log on next.");

      struct candlemsg *reply = alloccandlemsg();
      reply = packcandlemsg(reply, REQFAIL, NULLFIELD, NULLFIELD, NULLFIELD, buf);
      dealloccandlemsg(candleexchange(reply, findusid(candlemsg->from, userlist)->ip, findusid(candlemsg->from, userlist)->port)); 

      dealloccandlemsg(reply);
      free(buf);
      free(deliverto);
      return 0;
    }

    struct candlemsg *rply;
    if((rply = candleexchange(message, delivernode->ip, delivernode->port)) == NULL) {
      /* Recipient is away unexpectedly */
      addpendingmsg(pending, message);

      char *buf = malloc(sizeof(char) * MSGLEN);
      sprintf(buf, "%s%s", deliverto, " is not online. Your message will be delivered when they log on next.");

      struct candlemsg *reply = alloccandlemsg();
      reply = packcandlemsg(reply, REQFAIL, NULLFIELD, NULLFIELD, NULLFIELD, buf);
      dealloccandlemsg(candleexchange(reply, findusid(candlemsg->from, userlist)->ip, findusid(candlemsg->from, userlist)->port)); 

      dealloccandlemsg(reply);
      free(buf);
      free(deliverto);
      return 0;
    }

    dealloccandlemsg(rply);
    dealloccandlemsg(candleexchange(message, findusid(candlemsg->from, userlist)->ip, findusid(candlemsg->from, userlist)->port));
    dealloccandlemsg(message);

    free(deliverto);
    return 0;
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

