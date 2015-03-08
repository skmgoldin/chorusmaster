#include <stdio.h>
#include "../sharedcode/datastructures.h"
#include "logger.h"

struct candlemsg *candlelog(struct candlemsg *candlemsg) {

  printf("%s%s%s%s\n", candlemsg->versionid, candlemsg->reqtype, candlemsg->from, candlemsg->msg);

  return candlemsg;
}

char *serverlog(char *msg) {

  printf("%s\n", msg);

  return msg;
}
