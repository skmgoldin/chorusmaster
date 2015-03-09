#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/datastructures.h"
#include "../sharedcode/globalvalues.h"
#include <string.h>
#include "../sharedcode/candlemsg.h"

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

  char *username = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);

  printf("%s", "Username: ");
  fgets(username, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  struct candlemsg *candlemsg = alloccandlemsg(VERSIONID, username,
                                               LOGIN, msg);

  free(username);
  free(msg);

  sendcandlemsg(candlemsg, sock);

  dealloccandlemsg(candlemsg);

  return 0;
}
