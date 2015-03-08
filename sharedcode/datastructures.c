#include "datastructures.h"
#include <stdlib.h>
#include <string.h>
#include "globalvalues.h"
#include "msgvalidation.h"

struct candlemsg *alloccandlemsg(char *versionid, char *from, char *reqtype,
                                 char *msg) {

  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * VERSIONIDLEN);
  memset(candlemsg->versionid, 0, VERSIONIDLEN);
  versionid = validatemsg(versionid, VERSIONIDLEN);
  strcpy(candlemsg->versionid, versionid);

  candlemsg->from = malloc(sizeof(char) * FROMLEN);
  memset(candlemsg->from, 0, FROMLEN);
  from = validatemsg(from, FROMLEN);
  strcpy(candlemsg->from, from);

  candlemsg->reqtype = malloc(sizeof(char) * REQTYPELEN);
  memset(candlemsg->reqtype, 0, REQTYPELEN);
  reqtype = validatemsg(reqtype, REQTYPELEN);
  strcpy(candlemsg->reqtype, reqtype);

  candlemsg->msg = malloc(sizeof(char) * MSGLEN);
  memset(candlemsg->msg, 0, MSGLEN);
  msg = validatemsg(msg, MSGLEN);
  strcpy(candlemsg->msg, msg);

  return candlemsg;
}

int dealloccandlemsg(struct candlemsg *candlemsg) {

  free(candlemsg->msg);
  free(candlemsg->reqtype);
  free(candlemsg->from);
  free(candlemsg->versionid);
  free(candlemsg);

  return 0;
}

struct liveuserslist *addliveuser(char *username, struct liveuserslist
                                  *liveusers) {
  
  struct liveusernode *currnode = liveusers->head;

  while(currnode->next != NULL) {
    currnode = currnode->next;
  }

  struct liveusernode *newnode = malloc(sizeof(struct liveusernode));
  newnode = initliveusernode(newnode, username);

  currnode->next = newnode;

  return liveuserslist;
}

int deinitliveusernode(struct liveusernode node) {

  free(node->username);
  free(node);

  return 0;
}

struct liveuserslist *rmvliveuser(char *username, struct liveuserslist
                                  *liveusers) {
  
  struct liveusernode *currnode = liveusers->head;

  if(strcmp(currnode->username, username) == 0) {
    struct liveusernode *newhead = currnode->next;
    deinitliveusernode(currnode->username);
    liveusers->head = newhead;

    return liveuserslist;
  }

  while(currnode->next != NULL) {

    if(strcmp(currnode->next->username, username) == 0) {
      struct liveusernode *newnext = currnode->next->next;
      deinitliveusernode(currnode->next);
      currnode->next = newnext;

      return liveuserslist;
    }

    currnode = currnode->next;
  }

  return liveuserslist;
}

struct liveusernode *initliveusernode(struct liveusernode *newnode, char *username) {

  newnode->username = malloc(sizeof(char) * FROMLEN);
  strcpy(newnode->username, username);

  newnode->next = NULL;

  return newnode;
}

struct liveuserslist *initliveuserslist(struct liveuserslist *liveusers) {

  liveusers->head = NULL;

  return liveusers;
}

