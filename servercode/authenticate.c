#include <stdio.h>
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>
#include "../sharedcode/datastructures.h"
#include "authenticate.h"

#define CREDFILE "servercode/credentials.txt"

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist) {

  if(finduser(candlemsg->from, userlist) != NULL) {
    return 1;
  }

  else if(strcmp(candlemsg->reqtype, LOGIN) == 0) { 
    if(login(candlemsg->from, candlemsg->msg)) {
      adduser(candlemsg->from, userlist);
      return 1;
    }

    return 0;
  }
  
  return 0;
}


int login(char *username, char *password) {

FILE *credfile = fopen(CREDFILE, "r");
  if(!credfile) {
    perror("File opening failed");
    exit(1);
  }

  char *teststring = malloc(sizeof(char) * MSGLEN);
  int i;
  for(i = 0; fgets(teststring, FROMLEN, credfile) != NULL; i++) {
    if(strcmp(username, teststring) == 0 && (i % 2) == 0) {
      if(strcmp(password, fgets(teststring, MSGLEN, credfile)) == 0) {
        free(teststring);
        fclose(credfile);
        return 1;
      }
    }
  }
  
  rewind(credfile);

  free(teststring);
  fclose(credfile);
  return 0;
}
