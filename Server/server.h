#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#define MAXLEN 256
#define MAXLINE 4096
#define SERVER_PORT 6666
#define LISTENQ 16

enum Weather {shower=0,clear,cloudy,rain,fog};

/*the data send to server*/
struct sendLine
{
  char lineHead[2];
  char city[30];
  char lineTail;
};

/*the data recv from server*/
struct recvLine
{
  char lineHead[2];
  char city[30];
  unsigned char year[2];
  unsigned char month;
  unsigned char day;
  char inputNum;
  char weatherMsg[6];
  char zero[94];
};

/*weather message*/
struct WeathMsg
{ 
  unsigned char dateNum[4];
  char weatherMsg[6];  
  char zero[10];
};

void bzero(void*s,int n);
pid_t fork(void);
int close(int fd);

struct WeathMsg randWeath(void);
int findCity(char* cityStr);
struct recvLine makeMsg(struct WeathMsg weathMsg,char* cityname,char dayNum,char* sign,int sendNum);
