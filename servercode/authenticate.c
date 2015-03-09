#include <stdio.h>
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/userlist.h"
#include "authenticate.h"
#include "../sharedcode/sockdata.h"
#include "../sharedcode/wireio.h"

#define CREDFILE "servercode/credentials.txt"
#define TRIES 3
#define LOCKOUTTIME 60

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *lockoutlist, struct sockdata *sockdata) {

  if(finduser(candlemsg->from, userlist) != NULL) {
    /* User is already authenticated. */
    // Check if sockdata is different!
    return 1;
  }

  if(finduser(candlemsg->from, lockoutlist) != NULL) {
    /* User is currently locked out. */
    struct candlemsg *outmsg = alloccandlemsg();
    char *msg = "You are locked out. Try again later.";
    outmsg = packcandlemsg(outmsg, LOCKOUT, NULLFIELD, candlemsg->from, msg); 
    sendcandlemsg(outmsg, sockdata->sock);
    return 0;
  }

  else if(strcmp(candlemsg->reqtype, LOGIN) == 0) { 
    /* User is requesting to login. They get TRIES attempts. */

    if(login(candlemsg->from, candlemsg->msg)) {
      /* Login was successful, add user to userlist and authenticate. */
      adduser(candlemsg->from, sockdata, userlist);
      return 1;
    }

    /* First attempt failed, request try again. */
    int i;
    for(i = 0; i < TRIES - 1; i++) {
      if(login(candlemsg->from, candlemsg->msg)) {
        /* Login was successful, add user to userlist and authenticate. */
        adduser(candlemsg->from, userlist);
        return 1;
      }
    }

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
