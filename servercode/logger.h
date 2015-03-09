#ifndef LOGGER_H
#define LOGGER_H

#include "../sharedcode/conninfo.h"

struct candlemsg *candlelog(struct candlemsg *candlemsg, struct conninfo *conninfo);

char *writelog(char *msg);

char *serverlog(char *msg);

#endif
