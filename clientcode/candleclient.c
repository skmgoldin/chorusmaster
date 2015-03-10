#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/globalvalues.h"
#include <string.h>
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/servertools.h"
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include "../sharedcode/sockdata.h"
#include <sys/socket.h>

//#define CANDLEPORT "4444" // I need to get an arbitrary port from the system.

static char *servip;
static char *servport;
static char *username;
static char *listenport;

void siginthandler() {
  logout(servip, servport);
  free(username);
  exit(1);
}

int logout(char *servip, char *servport) {

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGOUT, NULLFIELD, username, NULLFIELD, NULLFIELD);
  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);
  dealloccandlemsg(reply);
  return 0;
}

int main(int argc, char **argv) {

  signal(SIGINT, siginthandler);

  if(*(argv + 1) == NULL || *(argv + 2) == NULL) {
    printf("%s\n", "Run again with an ip address and port number to connect "
           "to.");
    exit(1);
  }

  servip = *(argv + 1);
  servport = *(argv + 2);

  int servsock = makeserver("0");

  struct sockaddr *servinfo = malloc(sizeof(struct sockaddr));
  socklen_t addrlen = sizeof(struct sockaddr);
  memset(servinfo, '0', sizeof(struct sockaddr));
  getsockname(servsock, servinfo, &addrlen);

  char *myport = malloc(sizeof(char) * IPLEN);   
  sprintf(myport, "%d", ntohs(((struct sockaddr_in *) servinfo)->sin_port)); 
  listenport = malloc(sizeof(char) * PORTLEN);
  strcpy(listenport, myport);
  printf("%s\n", listenport);
  free(myport);

  char *mysock = malloc(sizeof(char) * IPLEN);   
  sprintf(mysock, "%d", servsock);
  showrunner(servip, servport, mysock); 
  
  return 0;
}

int showrunner(char *servip, char *servport, char *mysock) {

  pid_t pid = fork();

  if(pid == 0) {
    /* Child process */
    execl("./clientlistener", "./clientlistener", mysock, NULL); //Get an arbitrary port!
  } else if(pid > 0) {
    /* Parent process */
    login(servip, servport);
    inputhandler(servip, servport);
  }

  // Kill the clientlistener and message the server to say bye
  return 0;
}

int inputhandler(char *servip, char *servport) {

  char *input = malloc(sizeof(char) * MSGLEN);

  while(1) {
    fgets(input, MSGLEN, stdin);
    printf("%s\n", input);

    memset(input, '0', MSGLEN); // Probably unnecessary
     
  }

  free(input);
}

int login(char *servip, char *servport) {

  char *usernamebuf = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);

  printf("%s", "Username: ");
  fgets(usernamebuf, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGIN, listenport, usernamebuf, NULLFIELD, msg); //Get an arbitrary port!
  free(msg);

  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);

  if(atoi(reply->msg) == 1) {
    username = malloc(sizeof(char) * FROMLEN);
    strcpy(username, usernamebuf);
    free(usernamebuf);
    dealloccandlemsg(reply);
    return 0;
  } else {
    printf("%s\n", reply->msg);
    free(usernamebuf);
    dealloccandlemsg(reply);
    return login(servip, servport);
  }

  return 0;
}
