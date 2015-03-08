#ifndef LOGGER_H
#define LOGGER_H

struct candlemsg *candlelog(struct candlemsg *candlemsg);

char *writelog(char *msg);

char *serverlog(char *msg);

#endif
