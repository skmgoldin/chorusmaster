#include <stdio.h>
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>

#define CREDFILE "credentials.txt"

int authenticate(char *username, char *password) {
  
  FILE *credfile = fopen(CREDFILE, "r");
  if(!credfile) {
    perror("File opening failed");
    exit(1);
  }

  char *teststring;
  int i;
  for(i = 0; *(teststring = fgets(teststring, FROMLEN, credfile)) != EOF; i++) {
    printf("%s\n", teststring);
    if(strcmp(username, teststring) && (i % 2) == 0) {
      if(strcmp(password, fgets(teststring, MSGLEN, credfile))) {
        return 1;
      }
    }
  }
  
  fclose(credfile);
  return 0;
}
