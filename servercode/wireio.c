#include "../datastructures/datastructures.h"
#include <stdlib.h>
#include <sys/socket.h>

struct clntreq *readclntreq(struct clntreq *clntreq, int clntsock) {

  void *buf = malloc(sizeof(struct clntreq));
  int len = sizeof(struct clntreq);
  int flags = 0;

  recv(clntsock, buf, len, flags);
  
  free(buf);

  return clntreq;
}
