#include <stdio.h>
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/userlist.h"
#include "authenticate.h"
#include "../sharedcode/sockdata.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/conninfo.h"
#include "logger.h"

#define CREDFILE "servercode/credentials.txt"
#define TRIES 3
#define LOCKOUTTIME 60

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *loginlist, struct userlist *lockoutlist,
                 struct conninfo *conninfo) {

  if(finduser(candlemsg->from, userlist) != NULL) {
    /* User is already authenticated. */
    if(finduser(candlemsg->from, userlist)->ip != conninfo->ip) {
      /* User is logging in from a new location, log out old location. */
      rmvuser(candlemsg->from, userlist);
      adduser(candlemsg->from, conninfo->ip, candlemsg->stableport, userlist);
    }
    return 1;
  }

  if(finduser(candlemsg->from, lockoutlist) != NULL) {
    /* User is currently locked out. */
    struct candlemsg *reply = alloccandlemsg();
    reply = packcandlemsg(reply, LOCKOUT, NULLFIELD, NULLFIELD, candlemsg->from,
                          "You are locked out. Try again later.\n"); 
    sendcandlemsg(reply, conninfo->sock);
    dealloccandlemsg(reply);
    return 0;
  }

  else if(strcmp(candlemsg->reqtype, LOGIN) == 0) { 
    /* User is requesting to login. They get TRIES attempts. */

    if(login(candlemsg->from, candlemsg->msg)) {
      /* Login was successful, add user to userlist and authenticate. */
      adduser(candlemsg->from, conninfo->ip, candlemsg->stableport, userlist);
      return 1;
    }

    /* Login failed, request try again. */
    if(finduser(candlemsg->from, loginlist) == NULL) {
      adduser(candlemsg->from, conninfo->ip, candlemsg->stableport, loginlist);
      struct usernode *user = finduser(candlemsg->from, loginlist);
      user->missedcheckins++;
    } else {
      struct usernode *user = finduser(candlemsg->from, loginlist);
      user->missedcheckins++;
      if(user->missedcheckins >= TRIES) {
        adduser(candlemsg->from, conninfo->ip, candlemsg->stableport, lockoutlist);
        rmvuser(candlemsg->from, loginlist);

        struct candlemsg *reply = alloccandlemsg();
        reply = packcandlemsg(reply, LOCKOUT, NULLFIELD, NULLFIELD, candlemsg->from,
                              "You are locked out. Try again later.\n"); 
        sendcandlemsg(reply, conninfo->sock);
        dealloccandlemsg(reply);
        return 0;
      }
    }

    struct candlemsg *reply = alloccandlemsg();
    reply = packcandlemsg(reply, LOGIN, NULLFIELD, NULLFIELD, NULLFIELD,
                          "Incorrect username or password.\n");
    sendcandlemsg(reply, conninfo->sock);
    dealloccandlemsg(reply);

    /* Login was not successful. User is added to lockout list. */
    // Add user to lockout list.
    return 0;
  }
  
  /* User is not authenticated */
  return 0;
}


int login(char *username, char *password) {

FILE *credfile = fopen(CREDFILE, "r");
  if(!credfile) {
    perror("File opening failed");
    exit(1);
  }

  char *teststring = malloc(sizeof(char) * MSGLEN);
  int i;
  for(i = 0; fgets(teststring, FROMLEN, credfile) != NULL; i++) {
    if(strcmp(username, teststring) == 0 && (i % 2) == 0) {
      if(strcmp(password, fgets(teststring, MSGLEN, credfile)) == 0) {
        free(teststring);
        fclose(credfile);
        return 1;
      }
    }
  }
  
  rewind(credfile);

  free(teststring);
  fclose(credfile);
  return 0;
}
