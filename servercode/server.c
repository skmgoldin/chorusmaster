#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

  if(*(argv + 1) == NULL) {
    printf("%s\n", "Run again with a port number, please.");
    exit(1);
  }

  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  memset(&hints, 0, sizeof hints); 
  hints.ai_family = AF_UNSPEC;     
  hints.ai_socktype = SOCK_STREAM; 
  hints.ai_flags = AI_PASSIVE;     

  if((status = getaddrinfo(NULL, *(argv + 1), &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
 
  while(1) {

    //Wait for a client to connect
    //Send to login handler
  }

}
