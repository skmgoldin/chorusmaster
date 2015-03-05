struct clnt {
  char *username;
  int isonline;
}

struct clntreq {
  struct clnt *id = NULL;
  char *reqtype = NULL;

}

struct clntmsg {
  struct clnt *id = NULL;
  struct msg *msg = NULL;
}

struct msg {
  struct clnt *id = NULL;

  struct clnt *from;
  struct clnt *to;

  char *msg;
}

struct clntlogin {
  struct clnt *id = NULL;
  char *pw = NULL;
}
