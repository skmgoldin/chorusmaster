#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct clntreq {
  struct clnt *clnt = NULL;
  struct req *req = NULL;
}

struct clnt {
  char *uid;
  int isonline;
}

struct req {
  struct clntmsg *msg = NULL;
  struct clntlogin *login= NULL;
}

struct clntmsg {
  struct clnt *from;
  struct clnt *to;

  char *msg;
}

struct clntlogin {
  struct clnt *clnt = NULL;
  char *pw = NULL;
}

struct liveusers {
  struct liveusernode *head;
}

struct liveusernode {
  struct clnt *user;
  struct liveusernode *next;
}

#endif
