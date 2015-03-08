#include <stdio.h>
#include <string.h>
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>

#define CREDFILE "servercode/credentials.txt"

int authenticate(char *username, char *password) {
  
  FILE *credfile = fopen(CREDFILE, "r");
  if(!credfile) {
    perror("File opening failed");
    exit(1);
  }

  char *teststring = malloc(sizeof(char) * MSGLEN);
  int i;
  for(i = 0; fgets(teststring, FROMLEN, credfile) != NULL; i++) {
    printf("%s\n", teststring);
    printf("%s%s\n", username, password);
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
