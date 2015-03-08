#include <stdio.h>

#define CREDFILE "credentials.txt"

int authenticate(char *username, char *password) {
  
  FILE credfile = fopen(CREDFILE, "r");

  char *teststring;
  int i;
  for(i = 0; (teststring = fgets(credfile)) != EOF; i++) {
    if(strcmp(username, teststring) && (i % 2) == 0) {
      if(strcmp(password, fgets(credfile))) {
        return 1;
      }
    }
  }
  
  return 0;
}
