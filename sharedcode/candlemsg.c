#include "candlemsg.h"
#include "globalvalues.h"

struct candlemsg *alloccandlemsg() {

  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * VERSIONIDLEN);
  memset(candlemsg->versionid, 0, VERSIONIDLEN);

  candlemsg->reqtype = malloc(sizeof(char) * REQTYPELEN);
  memset(candlemsg->reqtype, 0, REQTYPELEN);

  candlemsg->from = malloc(sizeof(char) * FROMLEN);
  memset(candlemsg->from, 0, FROMLEN);

  candlemsg->to = malloc(sizeof(char) * TOLEN);
  memset(candlemsg->to, 0, TOLEN);

  candlemsg->msg = malloc(sizeof(char) * MSGLEN);
  memset(candlemsg->msg, 0, MSGLEN);

  return candlemsg;
}

struct candlemsg *packcandlemsg(struct candlemsg *candlemsg, char *reqtype,
                                char *from, char *to, char *msg) {

  strcpy(candlemsg->versionid, VERSIONID);

  reqtype = validatemsg(reqtype, REQTYPELEN);
  strcpy(candlemsg->reqtype, reqtype);

  from = validatemsg(from, FROMLEN);
  strcpy(candlemsg->from, from);

  to = validatemsg(to, TOLEN);
  strcpy(candlemsg->to, to);

  msg = validatemsg(msg, MSGLEN);
  strcpy(candlemsg->msg, msg);

  return candlemsg;
}

int dealloccandlemsg(struct candlemsg *candlemsg) {

  free(candlemsg->versionid);
  free(candlemsg->reqtype);
  free(candlemsg->from);
  free(candlemsg->to);
  free(candlemsg->msg);

  free(candlemsg);

return 0;
}
