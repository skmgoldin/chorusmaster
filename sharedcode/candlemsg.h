#ifndef CANDLEMSG_H
#define CANDLEMSG_H

struct candlemsg *alloccandlemsg();

struct candlemsg *packcandlemsg(struct candlemsg *candlemsg, char *reqtype,
                                char *from, char *to, char *msg);

int dealloccandlemsg(struct candlemsg *candlemsg);

#endif
