#include "datastructures.h"
#include <stdlib.h>
#include <sys/socket.h>
#include "wireio.h"
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include "servertools.h"


#define VERSIONIDLEN 32
#define CLNTIDLEN 32
#define LIVESTATUSLEN 1
#define REQTYPELEN 32
#define MSGLEN 4096

struct candlemsg *readcandlemsg(struct candlemsg *candlemsg, int clntsock) {

  candlemsg->versionid = readfield(clntsock, candlemsg->versionid,
                                   VERSIONIDLEN);
  candlemsg->versionid = readfield(clntsock, candlemsg->clntid,
                                   CLNTIDLEN);
  candlemsg->versionid = readfield(clntsock, (char *) candlemsg->livestatus,
                                   LIVESTATUSLEN);
  candlemsg->versionid = readfield(clntsock, candlemsg->reqtype,
                                   REQTYPELEN);
  candlemsg->versionid = readfield(clntsock, candlemsg->msg,
                                   MSGLEN);

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

int makeconnection(char *ip, char *port) {
  struct sockdata *sockdata = allocsockdata();
  sockdata = makesock(ip, port, sockdata);

  if(connect(sockdata->sock, sockdata->servinfo->ai_addr,
             sockdata->servinfo->ai_addrlen) != 0) {
    fprintf(stderr, "%s\n", "connect failed.");
    exit(1);
  }

  int sock = sockdata->sock;

  deallocsockdata(sockdata);

  return sock;
}
