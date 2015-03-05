#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/datastructures.h"

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
  struct candlemsg *candlemsg = alloccandlemsg();

  printf("%s", "Username: ");
  fgets(candlemsg->clntid, 32, stdin);
  printf("%s", "Password: ");
  fgets(candlemsg->msg, 32, stdin);

  sendcandlemsg(candlemsg, sock);

  dealloccandlemsg(candlemsg);

  return 0;
}
