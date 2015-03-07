#include "datastructures.h"
#include <stdlib.h>
#include <string.h>

struct candlemsg *alloccandlemsg(char *versionid, char *from, char *reqtype,
                                 char *msg) {

  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * 32);
  strcpy(candlemsg->versionid, versionid);

  candlemsg->from = malloc(sizeof(char) * 32);
  strcpy(candlemsg->from, from);

  candlemsg->reqtype = malloc(sizeof(char) * 32);
  strcpy(candlemsg->reqtype, reqtype);

  candlemsg->msg = malloc(sizeof(char) * 4096);
  strcpy(candlemsg->msg, msg);

  return candlemsg;
}

int dealloccandlemsg(struct candlemsg *candlemsg) {

  free(candlemsg->msg);
  free(candlemsg->reqtype);
  free(candlemsg->livestatus);
  free(candlemsg->from);
  free(candlemsg->versionid);
  free(candlemsg);

  return 0;
}
