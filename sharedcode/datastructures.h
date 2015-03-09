#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg {
  char *versionid; 
  char *reqtype;   
  char *from;   
  char *to;
  char *msg;     
};

struct userlist *adduser(char *username, struct userlist *users); 
  
struct usernode *initusernode(struct usernode *newnode, char *username);

struct userlist *inituserlist(struct userlist *users);

int deinitusernode(struct usernode *node);

struct userlist *rmvuser(char *username, struct userlist *users);

int deinituserlist(struct userlist *userlist);

int deinitallnodes(struct usernode *currnode);

struct usernode *finduser(char *username, struct userlist *userlist);

struct userlist {
  struct usernode *head;
};

struct usernode {
  char *username;
  struct usernode *next;
};

#endif
