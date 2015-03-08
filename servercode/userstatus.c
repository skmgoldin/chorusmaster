#include "../sharedcode/datastructures.h"

struct userlist *userlogin(char *username, struct userlist
                                *userlist) {

  userlist = adduser(username, userlist);

  return userlist;
}
