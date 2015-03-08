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
                                  *userlist) {
  
  struct usernode *newnode = malloc(sizeof(struct usernode));
  newnode = initusernode(newnode, username);

  if(userlist->head == NULL) {
    userlist->head = newnode;
    return userlist;
  }

  struct usernode *currnode = userlist->head;

  while(currnode->next != NULL) {
    currnode = currnode->next;
  }

  currnode->next = newnode;

  return userlist;
}

int deinitusernode(struct usernode *node) {

  free(node->username);
  free(node);

  return 0;
}

struct userlist *rmvuser(char *username, struct userlist
                         *userlist) {
  
  struct usernode *currnode = userlist->head;

  if(strcmp(currnode->username, username) == 0) {
    struct usernode *newhead = currnode->next;
    deinitusernode(currnode);
    userlist->head = newhead;

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

struct userlist *inituserlist(struct userlist *userlist) {

  userlist->head = NULL;

  return userlist;
}

int deinituserlist(struct userlist *userlist) {

  if(userlist->head == NULL) {
    return 0;
  }

  struct usernode *currnode = userlist->head;
  deinitallnodes(currnode);

  free(userlist);

  return 0;
}

int deinitallnodes(struct usernode *currnode) {

  if(currnode->next == NULL) {
    free(currnode);
    return 0;
  }

  deinitallnodes(currnode->next);
  free(currnode);

  return 0;
}

struct usernode *finduser(char *username, struct userlist *userlist) {

  struct usernode *currnode = userlist->head;

  while(currnode != NULL) {
    if(strcmp(username, currnode->username) == 0) { return currnode; }
    currnode = currnode->next;
  }

  return NULL; 
}
