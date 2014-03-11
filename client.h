#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 6666
#define MAXBUF 4096

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

in_addr_t inet_addr(const char* cp);
void fresh(void);
void freshCity(void);
void display(struct recvLine* recvline);
void WeathMsgDisp(struct recvLine* recvline);
struct sendLine makeSendline(char *getInput,char* lineHead,char lineTail);