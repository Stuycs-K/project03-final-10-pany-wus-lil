#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define BUFFER_SIZE 1024

// comment out the long one when not in use
#define DEBUG(msg,...)
//#define DEBUG(msg,...) printf("debug: " msg, ##__VA_ARGS__)

struct card {
  char color;
  int number;
  struct card *next;
};

void err(int i, char*message);
int printCards(struct card * hand);
struct card * draw(struct card * head);
void add(struct card * head, char _color, int num);
struct card * create(char _color, int num);
bool search(struct card * head, char _color, int num);
bool removeCard(struct card ** head, char _color, int num);
struct card * makeHand(int n);
bool matches(char card1Color, int card1Num, char card2Color, int card2Num);

#endif
