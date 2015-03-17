#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include "../sharedcode/conninfo.h"

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *loginlist, struct userlist *lockoutlist,
                 struct conninfo *conninfo);

int login(char *username, char *password);

int alreadyauthenticated(struct candlemsg *candlemsg, struct userlist *userlist, 
                         struct userlist *loginlist, struct userlist *lockoutlist,
                         struct conninfo *conninfo); 

int lockedout(struct candlemsg *candlemsg, struct userlist *userlist, 
              struct userlist *loginlist, struct userlist *lockoutlist,
              struct conninfo *conninfo);

int loginmanager(struct candlemsg *candlemsg, struct userlist *userlist, 
                 struct userlist *loginlist, struct userlist *lockoutlist,
                 struct conninfo *conninfo);

int sendauthfail(char *msg, int sock);

int sendnewusid(char *msg, int sock);

char *makeusid(struct candlemsg *candlemsg);

#endif
