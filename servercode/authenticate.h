#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

int authenticate(struct candlemsg *candlemsg, struct userlist *userlist);

int login(char *username, char *password);

#endif
