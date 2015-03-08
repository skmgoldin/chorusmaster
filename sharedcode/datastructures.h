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

struct liveuserslist *addliveuser(char *username, struct liveuserslist
                                  *liveusers); 
  
struct liveusernode *initliveusernode(struct liveusernode *newnode,
                                      char *username);

struct liveuserslist *initliveuserslist(struct liveuserslist *liveusers);

struct liveuserslist *rmvliveuser(char *username, struct liveuserslist
                                  *liveusers);

struct liveuserslist {
  struct liveusernode *head;
};

struct liveusernode {
  char *username;
  struct liveusernode *next;
};

#endif
