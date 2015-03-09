#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "../sharedcode/candlemsg.h"
#include "../sharedcode/userlist.h"

int main(int argc, char **argv);

int handlerequest(struct candlemsg *candlemsg, struct userlist *userlist);

#endif
