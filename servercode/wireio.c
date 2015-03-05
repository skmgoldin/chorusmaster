#include "../datastructures/datastructures.h"
#include <stdlib.h>
#include <sys/socket.h>
#include "wireio.h"
#include <string.h>

#define VERSIONIDLEN 32
#define CLNTIDLEN 32
#define LIVESTATUSLEN 1
#define REQTYPELEN 32
#define MSGLEN 4096

struct candlemsg *readcandlemsg(struct candlemsg *candlemsg, int clntsock) {

  candlemsg->versionid = readfield(clntsock, candlemsg->versionid,
                                   VERSIONIDLEN);

  

  return candlemsg;
}

char *readfield(int clntsock, char *field, int fieldsize) {

  void *buf = malloc(sizeof(char) * fieldsize);
  int len = sizeof(buf);
  int flags = 0;

  recv(clntsock, buf, len, flags);

  strcpy(field, (char *) buf);

  free(buf);

  return field;
}
