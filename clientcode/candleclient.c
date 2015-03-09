#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/globalvalues.h"
#include <string.h>
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/servertools.h"

#define CANDLEPORT "4444\n"

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL || *(argv + 2) == NULL) {
    printf("%s\n", "Run again with an ip address and port number to connect "
           "to.");
    exit(1);
  }

  char *servip = *(argv + 1);
  char *servport = *(argv + 2);

  //int servsock = makeserver(CANDLEPORT);    

  login(servip, servport);

  while(1) {

   // getconnection(servsock);
  }
  
  return 0;
}

int login(char *servip, char *servport) {

  char *username = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);

  printf("%s", "Username: ");
  fgets(username, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGIN, CANDLEPORT, username, NULLFIELD, msg);
  free(username);
  free(msg);

  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);
  dealloccandlemsg(reply);

  // Do something based on the reply
  
  return 0;
}
