#include "../sharedcode/userlist.h"
#include "../sharedcode/wireio.h"
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/globalvalues.h"
#include <stdlib.h>
#include <stdio.h>

int broadcast(char *msg, struct userlist *to) {

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, BROADCAST, NULLFIELD, NULLFIELD, NULLFIELD, msg);

  struct usernode *currnode = to->head;
  while(currnode != NULL) {
    struct candlemsg *reply;
    reply = candleexchange(candlemsg, currnode->ip, currnode->port);
    if(reply == NULL) {;}
    else {
      dealloccandlemsg(reply);
      currnode = currnode->next;
    }
  }

  free(msg);
  dealloccandlemsg(candlemsg);

  return 0;
}
