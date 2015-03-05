#ifndef WIREIO_H
#define WIREIO_H

struct candlemsg *readcandlemsg(struct candlemsg *candlemsg, int clntsock);

char *readfield(int clntsock, char *field, int fieldsize);

int makeconnection(char *ip, char *port);

#endif
