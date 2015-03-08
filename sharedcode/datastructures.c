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

struct userlist *adduser(char *username, struct userlist
                                  *users) {
  
  struct usernode *currnode = users->head;

  while(currnode->next != NULL) {
    currnode = currnode->next;
  }

  struct usernode *newnode = malloc(sizeof(struct usernode));
  newnode = initusernode(newnode, username);

  currnode->next = newnode;

  return userlist;
}

int deinitusernode(struct usernode node) {

  free(node->username);
  free(node);

  return 0;
}

struct userlist *rmvuser(char *username, struct userlist
                         *users) {
  
  struct usernode *currnode = users->head;

  if(strcmp(currnode->username, username) == 0) {
    struct usernode *newhead = currnode->next;
    deinitusernode(currnode->username);
    users->head = newhead;

    return userlist;
  }

  while(currnode->next != NULL) {

    if(strcmp(currnode->next->username, username) == 0) {
      struct usernode *newnext = currnode->next->next;
      deinitusernode(currnode->next);
      currnode->next = newnext;

      return userlist;
    }

    currnode = currnode->next;
  }

  return userlist;
}

struct usernode *initusernode(struct usernode *newnode, char *username) {

  newnode->username = malloc(sizeof(char) * FROMLEN);
  strcpy(newnode->username, username);

  newnode->next = NULL;

  return newnode;
}

struct userlist *inituserlist(struct userlist *users) {

  users->head = NULL;

  return users;
}

