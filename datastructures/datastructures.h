#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg {
  char *versionid;
  char *clntid;
  int livestatus;
  char *reqtype;
  char *msg;
};

struct clntreq {
  struct clnt *clnt;
  struct req *req;
};

struct clnt {
  char *username;
  int isonline;
};

struct req {
  struct clntmsg *msg;
  struct clntlogin *login;
};

struct clntmsg {
  struct clnt *from;
  struct clnt *to;

  char *msg;
};

struct clntlogin {
  struct clnt *clnt;
  char *pw;
};

struct liveusers {
  struct liveusernode *head;
};

struct liveusernode {
  struct clnt *user;
  struct liveusernode *next;
};

#endif
