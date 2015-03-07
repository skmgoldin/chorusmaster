#include "datastructures.h"
#include <stdlib.h>
#include <sys/socket.h>
#include "wireio.h"
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include "servertools.h"
#include "globalvalues.h"

struct candlemsg *readcandlemsg(struct candlemsg *candlemsg, int clntsock) {

  candlemsg->versionid = readfield(clntsock, candlemsg->versionid,
                                   VERSIONIDLEN);

  candlemsg->from = readfield(clntsock, candlemsg->from,
                                   FROMLEN);

  candlemsg->reqtype = readfield(clntsock, candlemsg->reqtype,
                                   REQTYPELEN);

  candlemsg->msg = readfield(clntsock, candlemsg->msg,
                                   MSGLEN);

  return candlemsg;
}

char *readfield(int clntsock, char *field, int fieldsize) {

  char *buf = malloc(sizeof(char) * fieldsize);
  int len = sizeof(buf);
  int flags = 0;

  recv(clntsock, buf, len, flags);

  printf("%s\n", buf);

  strcpy(field, buf);

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

struct candlemsg *sendcandlemsg(struct candlemsg *candlemsg, int sock) {
  sendfield(sock, candlemsg->versionid, sizeof(candlemsg->versionid));
  sendfield(sock, candlemsg->from, sizeof(candlemsg->from));
  sendfield(sock, (char *) candlemsg->livestatus,
            sizeof(candlemsg->livestatus));
  sendfield(sock, candlemsg->reqtype, sizeof(candlemsg->reqtype));
  sendfield(sock, candlemsg->msg, sizeof(candlemsg->msg));

  return candlemsg;
}

char *sendfield(int sock, char *field, int fieldsize) {

  send(sock, field, fieldsize, 0);

  return field;
}
