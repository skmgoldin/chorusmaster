#include "../datastructures/datastructures.h"

struct clntreq *readclntreq(struct clntreq *clntreq, int clntsock) {

  void *buf = malloc(sizeof(struct clntreq));
  int len = sizeof(struct clntreq);
  int flags = 0;

  int recv(clntsock, buf, len, flags);
  
  free(buf);
}
