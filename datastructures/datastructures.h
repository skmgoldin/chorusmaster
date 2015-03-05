#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg *alloccandlemsg();

struct candlemsg {
  char *versionid; //32 chars
  char *clntid;    //32 chars
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
