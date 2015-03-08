#include "globalvalues.h"
#include <string.h>
#include <stdio.h>

char *validatemsg(char *msg, int maxlen) {

  int msglen = strlen(msg);

  if(*(msg + msglen - 1) != '\n') {
    if(msglen + 1 < maxlen) {
      *(msg + (msglen)) = '\n';
      *(msg + (msglen + 1)) = '\0';
    }
    else {
      *(msg + msglen - 1) = '\n';
      *(msg + msglen) = '\0';
    }
  }

  return msg;
}
