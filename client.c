#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>

#define SERVER_PORT 6666

struct sendLine
{
  char lineHead[2];
  char city[30];
  char lineTail;
};

struct recvLine
{
  /* data */
  char lineHead[2];
  char city[30];
  char year[2];
  char month;
  char day;
  char inputNum;
  char weatherMsg[6];
  char zero[94];
};

void display(struct *recvLine){
  if(strlen(recvLine->weatherMsg)==2){
    if()
  }
}

