#include <stdio.h>
#include "../sharedcode/datastructures.h"
#include "logger.h"
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>
#include <ctype.h>

struct candlemsg *candlelog(struct candlemsg *candlemsg) {

  serverlog("message");

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

char *serverlog(char *msg) {

  char *buf = malloc(sizeof(char) * MSGLEN * 2);

  sprintf(buf, "%s%s%s\n", "==", msg, "==");

  int i;
  for(i = 0; *(buf + i) != '\0'; i++) {
    *(buf + i) = toupper(*(buf + i));
  }

  writelog(buf);

  free(buf);

  return msg;
}

char *writelog(char *msg) {

  printf("%s", msg);

  return msg;
}
