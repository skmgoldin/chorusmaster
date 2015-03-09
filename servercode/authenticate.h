#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include "../sharedcode/conninfo.h"

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *lockoutlist, struct conninfo *conninfo);

int login(char *username, char *password);

#endif
