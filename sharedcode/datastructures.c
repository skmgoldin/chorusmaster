#include "datastructures.h"
#include <stdlib.h>
#include <string.h>

struct candlemsg *alloccandlemsg(char *versionid, char *from) {
  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * 32);
  strcpy(candlemsg->versionid, "candlechat 1.0");

  candlemsg->from = malloc(sizeof(char) * 32);
  *(candlemsg->from) = '\n';

  candlemsg->livestatus = malloc(sizeof(int));
  *(candlemsg->livestatus) = 0;

  candlemsg->reqtype = malloc(sizeof(char) * 32);
  *(candlemsg->reqtype) = '\n';

  candlemsg->msg = malloc(sizeof(char) * 4096);
  *(candlemsg->msg) = '\n';

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
