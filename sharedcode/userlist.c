#include <stdlib.h>
#include "userlist.h"
#include <string.h>
#include "globalvalues.h"
#include <time.h>

struct userlist *adduser(char *username, char *ip, char *port,
                         struct userlist *userlist) {

  struct usernode *newnode = malloc(sizeof(struct usernode));
  newnode = initusernode(newnode, username, ip, port);

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
  free(node->ip);
  free(node->port);
  free(node);

  return 0;
}

struct userlist *rmvuser(char *username, struct userlist *userlist) {

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

struct usernode *initusernode(struct usernode *newnode, char *username,
                              char *ip, char *port) {

  newnode->username = malloc(sizeof(char) * FROMLEN);
  strcpy(newnode->username, username);

  newnode->ip = malloc(sizeof(char) * IPLEN);
  strcpy(newnode->ip, ip);
 
  newnode->port = malloc(sizeof(char) * PORTLEN);
  strcpy(newnode->port, port);
  *(newnode->port + strlen(port) - 1) = '\0';

  newnode->lastcheckin = time(NULL);
  
  newnode->util = 0;

  newnode->next = NULL;

  return newnode;
}

struct userlist *inituserlist(struct userlist *userlist) {

  userlist->head = NULL;

  return userlist;
}

int deinituserlist(struct userlist *userlist) {

  if(userlist->head == NULL) {
    free(userlist);
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
