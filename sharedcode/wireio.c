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

struct candlemsg *readcandlemsg(int clntsock) {

  char *versionid = malloc(sizeof(char) * VERSIONIDLEN);
  versionid = readfield(clntsock, versionid, VERSIONIDLEN);

  char *from = malloc(sizeof(char) * FROMLEN);
  from = readfield(clntsock, from, FROMLEN);

  char *reqtype = malloc(sizeof(char) * REQTYPELEN);
  reqtype = readfield(clntsock, reqtype, REQTYPELEN);

  char *msg = malloc(sizeof(char) * MSGLEN);
  msg = readfield(clntsock, msg, MSGLEN);

  struct candlemsg *candlemsg = alloccandlemsg(versionid, from, reqtype, msg);

  free(versionid);
  free(from);
  free(reqtype);
  free(msg);

  return candlemsg;
}

char *readfield(int clntsock, char *field, int fieldsize) {

  char *buf = malloc(sizeof(char) * fieldsize);
  int len = fieldsize;
  int flags = 0;

  recv(clntsock, buf, len, flags);

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
  sendfield(sock, candlemsg->versionid, VERSIONIDLEN);
  sendfield(sock, candlemsg->from, FROMLEN);
  sendfield(sock, candlemsg->reqtype, REQTYPELEN);
  sendfield(sock, candlemsg->msg, MSGLEN);

  return candlemsg;
}

char *sendfield(int sock, char *field, int fieldsize) {

  send(sock, field, fieldsize, 0);

  return field;
}
