#include <stdio.h>
#include "../sharedcode/servertools.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/conninfo.h"
#include "../sharedcode/candlemsg.h"

int main(int argc, char **argv) {

  char *listenport = *(argv + 1);

  int servsock = makeserver(listenport);    

  while(1) {
    struct conninfo *conninfo = getconnection(servsock);

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    printf("%s\n", candlemsg->msg);

    deallocconninfo(conninfo);
    dealloccandlemsg(candlemsg);
  }

  return 0;

}
