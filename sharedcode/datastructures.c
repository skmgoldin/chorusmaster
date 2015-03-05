#include "datastructures.h"
#include <stdlib.h>

struct candlemsg *alloccandlemsg() {
  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));
  candlemsg->versionid = malloc(sizeof(char) * 32);
  candlemsg->clntid = malloc(sizeof(char) * 32);
  candlemsg->livestatus = malloc(sizeof(int));
  candlemsg->reqtype = malloc(sizeof(char) * 32);
  candlemsg->msg = malloc(sizeof(char) * 4096);

  return candlemsg;
}
