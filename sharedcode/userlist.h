#ifndef USERLIST_H
#define USERLIST_H

struct userlist {
  struct usernode *head;
};

struct usernode {
  char *username;
  char *ip;
  char *port;
  int missedcheckins;
  struct usernode *next;
};

struct userlist *adduser(char *username, char *ip, char *port,
                         struct userlist *userlist);

struct usernode *initusernode(struct usernode *newnode, char *username,
                              char *ip, char *port);

struct userlist *inituserlist(struct userlist *users);

int deinitusernode(struct usernode *node);

struct userlist *rmvuser(char *username, struct userlist *users);

int deinituserlist(struct userlist *userlist);

int deinitallnodes(struct usernode *currnode);

struct usernode *finduser(char *username, struct userlist *userlist);

#endif
