#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
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

void fresh(void);
void freshCity(void);
void display(struct recvLine* recvline);
void WeathMsgDisp(struct recvLine* recvline);
struct sendLine makeSendline(char *getInput,char* lineHead,char lineTail);

int main(int argc,char** argv){  
  int sockfd;
  struct sockaddr_in servaddr;  
  char recvBuf[MAXBUF];
  char getInput[MAXBUF];  

  struct recvLine *recvline;
  struct sendLine sendline;  

  if(argc != 2){
    perror("Usage: ./client IPaddr(server ip)");
    exit(1);
  }
  
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    perror("failed to create the socket!");
    exit(2);
  }

  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(SERVER_PORT);

  if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
    perror("failed to connet the server!");
    exit(3);
  }

  freshCity();
  char headSign[2] = {0x01,0x00};
  char tailSign = 0x00;
  int endSign = 0;
  while(fgets(getInput,MAXBUF,stdin)!=NULL){
    getInput[strlen(getInput)-1] = '\0';    
    endSign=0;
    
    if(getInput[0]==0x63){
      freshCity();
    }
    else if(getInput[0]==0x23){
      break;      
    }
    else{
    sendline = makeSendline(getInput,headSign,tailSign);    
    send(sockfd,(char*)&sendline,sizeof(sendline),0);
    if(recv(sockfd,recvBuf,MAXBUF,0)==0){
      perror("the server terminated prematurely!");
      exit(4);
    }
    //printf("%s", "string received from server:\n");        
    recvline = (struct recvLine*)recvBuf;
    /*printf("%s\n",recvline->city);
    printf("%02x %02x\n",recvline->lineHead[0],recvline->lineHead[1]);        
    */
    display(recvline);        

    if(recvline->lineHead[0]==0x03){        //the city name is correct
        char cityname[30];
        strncpy(cityname,recvline->city,30);        
        char dayNum[MAXBUF];
        char packHead[2] = {0x02,0x01};
        char packTail = 0x01;

        int backSign = 0;
        int sendSign = 1;
        while(fgets(dayNum,MAXBUF,stdin)!=NULL){          
          dayNum[strlen(dayNum)-1] = '\0';
          backSign = 0;
          sendSign = 1;
          if(strlen(dayNum)!=1){
            printf("%s\n","input error");
            sendSign = 0;
          }          
          else{
            switch(dayNum[0]){
              case 0x31:
                packHead[0]=0x02; packHead[1]=0x01; packTail=0x01; break;
              case 0x32:
                packHead[0]=0x02; packHead[1]=0x02; packTail=0x03; break;
              case 0x33:
                printf("%s","Please enter the day number(below 10, e.g. 1 means today):");
                int insideSign = 1;  
                while(fgets(dayNum,MAXBUF,stdin)!=NULL){                                  
                  dayNum[strlen(dayNum)-1] = '\0';
                  insideSign = 1;
                  //sendSign = 1;
                  if(strlen(dayNum)!=1){
                    printf("%s\n","input error");
                    printf("%s","Please enter the day number(below 10, e.g. 1 means today):");
                    insideSign = 0;
                    sendSign=0;
                  }
                  else{
                  switch(dayNum[0]){
                    case 0x31:packHead[0]=0x02; packHead[1]=0x01; packTail=0x01; break;
                    case 0x32:packHead[0]=0x02; packHead[1]=0x01; packTail=0x02; break;
                    case 0x33:packHead[0]=0x02; packHead[1]=0x01; packTail=0x03; break;
                    case 0x34:packHead[0]=0x02; packHead[1]=0x01; packTail=0x04; break;
                    case 0x35:packHead[0]=0x02; packHead[1]=0x01; packTail=0x05; break;
                    case 0x36:packHead[0]=0x02; packHead[1]=0x01; packTail=0x06; break;
                    case 0x37:packHead[0]=0x02; packHead[1]=0x01; packTail=0x07; break;
                    case 0x38:packHead[0]=0x02; packHead[1]=0x01; packTail=0x08; break;
                    case 0x39:packHead[0]=0x02; packHead[1]=0x01; packTail=0x09; break;
                    default:
                    printf("%s\n","input error");                    
                    printf("%s","Please enter the day number(below 10, e.g. 1 means today):");
                    //sendSign = 0;
                    insideSign==0; break;
                  }                  
                }
                  if(insideSign==1){
                    break;
                  }
                }              
              break;
              case 0x72: backSign=1;freshCity(); sendSign = 0; break;
              case 0x63: fresh(); sendSign = 0; break;
              case 0x23: endSign =1; sendSign = 0; break;                 
              default:
                printf("%s\n","input error"); 
                sendSign=0; break;
            }

            if(backSign==1 || endSign==1){
              break;
            } 

            if(sendSign==1){
              sendline = makeSendline(cityname,packHead,packTail);    
              send(sockfd,(char*)&sendline,sizeof(sendline),0);
              if(recv(sockfd,recvBuf,MAXBUF,0) ==0){
                 perror("the server terminated prematurely!");
                 exit(4);
              };
              recvline = (struct recvLine*)recvBuf;
              WeathMsgDisp(recvline);
            }
          }

        }
        if(endSign==1){
          break;
        }
    } 
  }      
    memset(recvBuf,0,MAXBUF);
  }
}




/**
*construct the data to send
*/
struct sendLine makeSendline(char *getInput,char* lineHead,char lineTail){
   struct sendLine sendline;
   memset(&sendline,0,sizeof(sendline));
   strcpy(sendline.lineHead,lineHead);
   sendline.lineTail = lineTail;
   strcpy(sendline.city,getInput);
   return sendline;
}

/**
*fresh the weather forecast interface
*/
void fresh(void){
    system("clear");
    printf("%s\n","Please enter the given number to query");
    printf("%s\n%s\n%s\n","1.today","2.three days from today","3.custom day by yourself");
    printf("%s\n","(r)back,(c)cls,(#)exit");
    printf("%s\n","============================================================");
}

/**
*fresh the window to input the city name
*/
void freshCity(void){
    system("clear");
    printf("%s\n","Welcome to NJUCS Weather Forecast Demo Program!");
    printf("%s\n%s\n", "Please input City Name in Chinese pinyin(e.g. nanjing or beijing)","(c)cls,(#)exit");
}
/**
*the message showing after input the city name
*/
void display(struct recvLine* recvline){
  if(recvline->lineHead[0]==0x03){
    system("clear");
    printf("%s\n","Please enter the given number to query");
    printf("%s\n%s\n%s\n","1.today","2.three days from today","3.custom day by yourself");
    printf("%s\n","(r)back,(c)cls,(#)exit");
    printf("%s\n","============================================================");
  }else{
    printf("%s%s%s\n","Sorry, Server does not have weather information for city ",recvline->city,"!");
    printf("%s\n","Welcome to NJUCS Weather Forecast Demo Program!");
    printf("%s\n%s\n", "Please input City Name in Chinese pinyin(e.g. nanjing or beijing)","(c)cls,(#)exit");
  }  

}

/**
*Display the weather message
*/
void WeathMsgDisp(struct recvLine* recvline){
  char tmpCmp[3] = {0x01,0x41};
  char tmpCmpOr[3] = {0x01,0x42};               
  char weather[10][20]={"shower","clear","cloudy","rain","fog"};  

  if(strncmp(recvline->lineHead,tmpCmp,2)==0 || strncmp(recvline->lineHead,tmpCmpOr,2)==0){      //have weather message    
    printf("city: %s  Today is:%d/%02x/%02x   Weather information is as follows:\n",recvline->city,
    (recvline->year[0]<<8)+recvline->year[1],recvline->month,recvline->day);    
        
    if(recvline->inputNum==0x03 && recvline->lineHead[1]==0x42){      
      printf("The 1th day's weather is: %s;Temp:%d\n", weather[recvline->weatherMsg[0]],recvline->weatherMsg[1]);
      printf("The 2th day's weather is: %s;Temp:%d\n", weather[recvline->weatherMsg[2]],recvline->weatherMsg[3]);
      printf("The 3th day's weather is: %s;Temp:%d\n", weather[recvline->weatherMsg[4]],recvline->weatherMsg[5]);      
    }
    else{
      if(recvline->inputNum==0x01){
         printf("Today's Weather is: %s;Temp:%d\n", weather[recvline->weatherMsg[0]],recvline->weatherMsg[1]);                    
      }
      else{
         printf("The %dth Weather is: %s;Temp:%d\n",recvline->inputNum, weather[recvline->weatherMsg[0]],recvline->weatherMsg[1]);                                 
      }      
    }

  }  
  else{
    printf("%s!\n","Sorry, no given day's weather information for city ",recvline->city);
  }
}

