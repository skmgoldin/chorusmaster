#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL || *(argv + 2) == NULL) {
    printf("%s\n", "Run again with an IP address and port number to connect "
           "to.");
    exit(1);
  }

  char *ip = *(argv + 1);
  char *port = *(argv + 2);

  return 0;
}
