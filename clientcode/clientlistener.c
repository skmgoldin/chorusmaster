#include <stdio.h>
#include "../sharedcode/servertools.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/conninfo.h"
#include "../sharedcode/candlemsg.h"
#include <stdlib.h>
#include "../sharedcode/globalvalues.h"
#include <string.h>

int main(int argc, char **argv) {

  int servsock = atoi(*(argv + 1));

  while(1) {
    struct conninfo *conninfo = getconnection(servsock);

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    if(strcmp(candlemsg->reqtype, REQFAIL) == 0) {
      printf("%s\n", candlemsg->msg);
    }

    if(strcmp(candlemsg->reqtype, BROADCAST) == 0) {
      printf("%s\n", candlemsg->msg);
    }

    if(strcmp(candlemsg->reqtype, MESSAGE) == 0) {
      printf("%s\n", candlemsg->msg);
    }

    deallocconninfo(conninfo); // Connection closed here
    dealloccandlemsg(candlemsg);
  }

  return 0;

}
