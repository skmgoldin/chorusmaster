#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include "../sharedcode/sockdata.h"

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *lockoutlist, struct sockdata *sockdata);

int login(char *username, char *password);

#endif
