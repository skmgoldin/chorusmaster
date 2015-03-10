#include <stdio.h>
#include "../sharedcode/servertools.h"

int main(int argc, char **argv) {

  char *listenport = *(argv + 1);

  int servsock = makeserver(listenport);    

  printf("%s\n", "I'm clientlistener.");
  while(1) {
    getconnection(servsock);
  }

  return 0;

}
