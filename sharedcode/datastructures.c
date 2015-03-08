#include "datastructures.h"
#include <stdlib.h>
#include <string.h>
#include "globalvalues.h"
#include "msgvalidation.h"

struct candlemsg *alloccandlemsg(char *versionid, char *from, char *reqtype,
                                 char *msg) {

  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * VERSIONIDLEN);
  memset(candlemsg->versionid, 0, VERSIONIDLEN);
  versionid = validatemsg(versionid, VERSIONIDLEN);
  strcpy(candlemsg->versionid, versionid);

  candlemsg->from = malloc(sizeof(char) * FROMLEN);
  memset(candlemsg->from, 0, FROMLEN);
  from = validatemsg(from, FROMLEN);
  strcpy(candlemsg->from, from);

  candlemsg->reqtype = malloc(sizeof(char) * REQTYPELEN);
  memset(candlemsg->reqtype, 0, REQTYPELEN);
  reqtype = validatemsg(reqtype, REQTYPELEN);
  strcpy(candlemsg->reqtype, reqtype);

  candlemsg->msg = malloc(sizeof(char) * MSGLEN);
  memset(candlemsg->msg, 0, MSGLEN);
  msg = validatemsg(msg, MSGLEN);
  strcpy(candlemsg->msg, msg);

  return candlemsg;
}

int dealloccandlemsg(struct candlemsg *candlemsg) {

  free(candlemsg->msg);
  free(candlemsg->reqtype);
  free(candlemsg->from);
  free(candlemsg->versionid);
  free(candlemsg);

  return 0;
}
