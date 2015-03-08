#include <stdio.h>
#include "../sharedcode/datastructures.h"
#include "logger.h"
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>

struct candlemsg *candlelog(struct candlemsg *candlemsg) {

  serverlog("==MESSAGE==");

  char *buf = malloc(sizeof(char) * MSGLEN * 2);

  sprintf(buf, "%s%s", "versionid: ", candlemsg->versionid);
  serverlog(buf);

  sprintf(buf, "%s%s", "from: ", candlemsg->from);
  serverlog(buf);

  sprintf(buf, "%s%s", "reqtype: ", candlemsg->reqtype);
  serverlog(buf);

  sprintf(buf, "%s%s", "msg: ", candlemsg->msg);
  serverlog(buf);

  free(buf);
  return candlemsg;
}

char *serverlog(char *msg) {

  printf("%s", msg);

  return msg;
}
