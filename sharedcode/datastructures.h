#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg *alloccandlemsg();

int dealloccandlemsg(struct candlemsg *candlemsg);

struct candlemsg {
  char *versionid; //32 chars
  char *from;    //32 chars
  int *livestatus; //1 int 
  char *reqtype;   //32 chars
  char *msg;       //4096 chars
};

struct liveusers {
  struct liveusernode *head;
};

struct liveusernode {
  struct clnt *user;
  struct liveusernode *next;
};

#endif
