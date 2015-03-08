#include <stdio.h>
#include "../sharedcode/datastructures.h"
#include "logger.h"
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>

struct candlemsg *candlelog(struct candlemsg *candlemsg) {

  writelog("==MESSAGE==\n");

  char *buf = malloc(sizeof(char) * MSGLEN * 2);

  sprintf(buf, "%s%s", "versionid: ", candlemsg->versionid);
  writelog(buf);

  sprintf(buf, "%s%s", "from: ", candlemsg->from);
  writelog(buf);

  sprintf(buf, "%s%s", "reqtype: ", candlemsg->reqtype);
  writelog(buf);

  sprintf(buf, "%s%s", "msg: ", candlemsg->msg);
  writelog(buf);

  free(buf);
  return candlemsg;
}


char *writelog(char *msg) {

  printf("%s", msg);

  return msg;
}
