#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "../sharedcode/candlemsg.h"
#include "../sharedcode/userlist.h"
#include "../sharedcode/msgnode.h"

int main(int argc, char **argv);

int handlerequest(struct candlemsg *candlemsg, struct userlist *userlist,
                  struct conninfo *conninfo, struct msglist *pending);


#endif
