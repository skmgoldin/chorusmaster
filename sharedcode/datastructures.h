#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct candlemsg *alloccandlemsg(char *versionid, char *from, char *reqtype,
                                 char *msg);

int dealloccandlemsg(struct candlemsg *candlemsg);

struct candlemsg {
  char *versionid; //32 chars
  char *from;    //32 chars
  char *reqtype;   //32 chars
  char *msg;       //4096 chars
};

struct userslist *adduser(char *username, struct userslist
                                  *users); 
  
struct usernode *initusernode(struct usernode *newnode,
                                      char *username);

struct userslist *inituserslist(struct userslist *users);

int deinitusernode(struct usernode node);

struct userslist *rmvuser(char *username, struct userslist
                                  *users);

struct userslist {
  struct usernode *head;
};

struct usernode {
  char *username;
  struct usernode *next;
};

#endif
