#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/datastructures.h"
#include "../sharedcode/globalvalues.h"
#include <string.h>

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL || *(argv + 2) == NULL) {
    printf("%s\n", "Run again with an IP address and port number to connect "
           "to.");
    exit(1);
  }

  char *ip = *(argv + 1);
  char *port = *(argv + 2);

  int sock = makeconnection(ip, port);

  login(sock);

  while(1) {;}
  return 0;
}

int login(int sock) {

  char *versionid = malloc(sizeof(char) * VERSIONIDLEN);
  char *username = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);
  char *reqtype = malloc(sizeof(char) * MSGLEN);

  printf("%s", "Username: ");
  fgets(username, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  strcpy(versionid, VERSIONID);
  strcpy(reqtype, LOGIN);

  struct candlemsg *candlemsg = alloccandlemsg(versionid, username,
                                               reqtype, msg);

  free(versionid);
  free(username);
  free(msg);
  free(reqtype);

  sendcandlemsg(candlemsg, sock);

  dealloccandlemsg(candlemsg);

  return 0;
}
