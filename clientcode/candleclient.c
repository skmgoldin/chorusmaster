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
#include <netdb.h>
#include <sys/wait.h>

static char *servip;
static char *servport;
static char *username;
static char *listenport;
static char *usid;

void siginthandler() {
  logout(servip, servport);
  free(username);
  exit(1);
}

int logout(char *servip, char *servport) {

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGOUT, servport, usid, NULLFIELD, NULLFIELD);
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
  free(servinfo);

  showrunner(servip, servport, mysock); 
  
  return 0;
}

int showrunner(char *servip, char *servport, char *mysock) {

  pid_t pid = fork();
  int status;

  if(pid == 0) {
    /* Child process */
    execl("./clientlistener", "./clientlistener", mysock, NULL);
  } else if(pid > 0) {
    /* Parent process */
    login(servip, servport);
    while(1) {
      if(waitpid(pid, &status , WNOHANG)) {
        exit(0);
      }
      inputhandler(servip, servport);
    }
  }

  // Kill the clientlistener and message the server to say bye
  return 0;
}

int inputhandler(char *servip, char *servport) {

  char *input = malloc(sizeof(char) * MSGLEN);

  fgets(input, MSGLEN, stdin);

  int i;
  for(i = 0; *(input + i) != ' ' && *(input + i) != '\n'; i++) {;}
  char *reqtype = malloc(sizeof(char) * (i + 1));
  strncpy(reqtype, input, i);
  *(reqtype + i) = '\0';

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, reqtype, listenport, usid, NULLFIELD, (input + (i + 1)));
 
  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);



  dealloccandlemsg(candlemsg);
  dealloccandlemsg(reply);
  free(reqtype);
  free(input);

  return 0;
}

int login(char *servip, char *servport) {

  char *usernamebuf = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);

  printf("%s", "Username: ");
  fgets(usernamebuf, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGIN, listenport, usernamebuf, NULLFIELD, msg);
  free(msg);

  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);

  if(strcmp(reply->reqtype, NEWUSID) == 0) {
    usid = malloc(sizeof(char) * USIDLEN);
    strcpy(usid, reply->msg);

    username = malloc(sizeof(char) * FROMLEN);
    strcpy(username, usernamebuf);
    free(usernamebuf);

    dealloccandlemsg(reply);

  } else if(strcmp(reply->reqtype, AUTHFAIL) == 0) {
    printf("%s\n", reply->msg);
    dealloccandlemsg(reply);
    free(usernamebuf);
    return login(servip, servport);
  }

  return 0;
}
