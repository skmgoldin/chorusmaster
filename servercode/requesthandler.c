#include <stdlib.h>
#include "servertools.h"

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  char *port = *(argv + 1);

  int sock = makeserver(port); 

  while(1) {

    //Wait for a client to connect
    //Send to login handler
  }

}
