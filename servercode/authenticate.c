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
#include <time.h>
#include "../sharedcode/msgvalidation.h"
#include "messagerouting.h"

#define CREDFILE "servercode/credentials.txt"
#define TRIES 3
#define LOCKOUTTIME 60
#define TIMEOUT 60

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *loginlist, struct userlist *lockoutlist,
                 struct conninfo *conninfo) {

  heartbeat(userlist);

  if(alreadyauthenticated(candlemsg, userlist, loginlist, lockoutlist, conninfo)) {
    return 1;
  }
 
  if(lockedout(candlemsg, userlist, loginlist, lockoutlist, conninfo)) {
    return 0;
  }

  if(loginmanager(candlemsg, userlist, loginlist, lockoutlist, conninfo)) { 
    return 1;
  }

  return 0;
}

  
int heartbeat(struct userlist *userlist) {
  
  struct usernode *currnode = userlist->head;
  while(currnode != NULL) {
    if(difftime(time(NULL), currnode->lastcheckin) > TIMEOUT) {
      if(ping(currnode) == 0) {
        struct usernode *killnode = currnode;
        currnode = currnode->next;

        char *buf = malloc(sizeof(char) * MSGLEN);
        sprintf(buf, "%s%s", killnode->username, " has not contacted the server and has been logged out.");
        rmvusername(killnode->username, userlist);
        broadcast(buf, userlist);
        free(buf);

      } else {
        currnode->lastcheckin = time(NULL);
        currnode = currnode->next;
      }
    } else {
      currnode = currnode->next;
    }
  } 
  return 0;
}

int ping(struct usernode *user) {
  struct candlemsg *ping = alloccandlemsg();
  ping = packcandlemsg(ping, PING, NULLFIELD, NULLFIELD, NULLFIELD, NULLFIELD);

  struct candlemsg *reply = candleexchange(ping, user->ip, user->port);
  dealloccandlemsg(ping);

  if(reply == NULL || strcmp(reply->reqtype, NULLFIELD) == 0) {
    dealloccandlemsg(reply);
    return 0;
  }

  dealloccandlemsg(reply);
  return 1;
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
    teststring = validatemsg(teststring);
    if(strcmp(username, teststring) == 0 && (i % 2) == 0) {
      fgets(teststring, MSGLEN, credfile);
      teststring = validatemsg(teststring);
      if(strcmp(password, teststring) == 0) {
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

int alreadyauthenticated(struct candlemsg *candlemsg, struct userlist *userlist, 
                         struct userlist *loginlist, struct userlist *lockoutlist,
                         struct conninfo *conninfo) {

  struct usernode *user = findusid(candlemsg->from, userlist);

  if(user != NULL) {
    /* User is already authenticated. */
    return 1;
  }

  /* User is not already authenticated. */
  return 0;
}

int lockedout(struct candlemsg *candlemsg, struct userlist *userlist, 
              struct userlist *loginlist, struct userlist *lockoutlist,
              struct conninfo *conninfo) { 

  struct usernode *user = findusername(candlemsg->from, lockoutlist);
  if(user != NULL) {
    /* User is currently locked out. */
    if(difftime(time(NULL), user->lastcheckin) < LOCKOUTTIME) {
      sendauthfail("You are locked out. Try again later.", conninfo->sock);
      return 1;
    } else {
      /* Lockout period expired. */
      rmvusername(candlemsg->from, lockoutlist);
      return 0;
    }
  }

  /* User is not currently locked out. */
  return 0;
}

int loginmanager(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *loginlist, struct userlist *lockoutlist,
                 struct conninfo *conninfo) {

  if(strcmp(candlemsg->reqtype, LOGIN) == 0) { 
    /* User is requesting to login. They get TRIES attempts. */

    if(login(candlemsg->from, candlemsg->msg)) {
      /* Login was successful. */

      if(findusername(candlemsg->from, userlist) != NULL) {
        /* If user was previously logged in from a different location, log them out. */
        struct usernode *olduser = findusername(candlemsg->from, userlist);

        rmvusername(olduser->username, userlist);
      }
      
      char *usid = makeusid(candlemsg);
      adduser(candlemsg->from, usid, conninfo->ip, candlemsg->stableport, userlist);
      sendnewusid(usid, conninfo->sock);
      free(usid);
      return 1;
    }

    /* Login failed. */
    if(findusername(candlemsg->from, loginlist) == NULL) {
      /* If user isn't on loginlist, add them and increment util. */
      adduser(candlemsg->from, NULLFIELD, conninfo->ip, candlemsg->stableport, loginlist);
      struct usernode *user = findusername(candlemsg->from, loginlist);
      user->util++;
    } else {
      /* User is already on loginlist, increment util. */
      struct usernode *user = findusername(candlemsg->from, loginlist);
      user->util++;
      if(user->util >= TRIES) {
        /* TRIES consecutive login failures. Lock user out. */
        adduser(candlemsg->from, NULLFIELD, conninfo->ip, candlemsg->stableport, lockoutlist);
        rmvusername(candlemsg->from, loginlist);

        sendauthfail("You are locked out. Try again later.", conninfo->sock);
        return 0;
      }
    }


    sendauthfail("Incorrect username or password", conninfo->sock);
    return 0;
  }

  /* Non-login reqtype */
  sendauthfail("You are not logged in.", conninfo->sock);
  return 0;
}

int sendauthfail(char *msg, int sock) {

  struct candlemsg *reply = alloccandlemsg();
  reply = packcandlemsg(reply, AUTHFAIL, NULLFIELD, NULLFIELD, NULLFIELD, msg);
  sendcandlemsg(reply, sock);
  dealloccandlemsg(reply);

  return 0;
}

int sendnewusid(char *msg, int sock) {

  struct candlemsg *reply = alloccandlemsg();
  reply = packcandlemsg(reply, NEWUSID, NULLFIELD, NULLFIELD, NULLFIELD, msg);
  sendcandlemsg(reply, sock);
  dealloccandlemsg(reply);

  return 0;
}

char *makeusid(struct candlemsg *candlemsg) {

  int usid = 0;

  int i;
  for(i = 0; *(candlemsg->from + i) != '\0'; i++) {
    usid = (*(candlemsg->from + i) + usid);
  }
  for(i = 0; *(candlemsg->msg + i) != '\0'; i++) {
    usid = (*(candlemsg->from + i) + usid);
  }

  usid = usid * time(NULL);

  char *usidstr = malloc(sizeof(char) * USIDLEN);
  sprintf(usidstr, "%d", usid);

  printf("%s%s\n", "USID: ", usidstr);

  return usidstr;
  
}

