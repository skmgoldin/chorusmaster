#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg *alloccandlemsg(char *versionid, char *from, char *reqtype,
                                 char *msg);

int dealloccandlemsg(struct candlemsg *candlemsg);

struct candlemsg {
  char *versionid; //32 chars
  char *from;    //32 chars
  char *reqtype;   //32 chars
  char *msg;       //4096 chars
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
