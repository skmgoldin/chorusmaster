#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv) {
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
