#include <stdio.h>
#include "../sharedcode/servertools.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/conninfo.h"
#include "../sharedcode/candlemsg.h"
#include <stdlib.h>

int main(int argc, char **argv) {

  int servsock = atoi(*(argv + 1));

  while(1) {
    struct conninfo *conninfo = getconnection(servsock);

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    printf("%s\n", candlemsg->msg);

    deallocconninfo(conninfo);
    dealloccandlemsg(candlemsg);
  }

  return 0;

}
