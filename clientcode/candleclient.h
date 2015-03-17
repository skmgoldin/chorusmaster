#ifndef CANDLECLIENT_H
#define CANDLECLIENT_H

int login(char *servip, char *servport);

int showrunner(char *servip, char *servport, char *mysock);

int inputhandler(char *servip, char *servport);

char **parseinput(char *input);

int logout(char *servip, char *servport);

#endif
