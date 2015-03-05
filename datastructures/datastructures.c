#include "datastructures.h"

struct candlemsg *alloccandlemsg() {
  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));
  candlemsg->versionid = malloc(sizeof(char * 32));
  candlemsg->username = malloc(sizeof(char * 32));
  candlemsg->livestatus = malloc(sizeof(int));
  candlemsg->reqtype = malloc(sizeof(char * 32));
  candlemsg->msg = malloc(sizeof(char * 4096));
}
