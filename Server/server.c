#include <stdlib.h>
#include <stdio.h>
#include "server.h"

int main(int argc, char** argv){
  int listenfd,connfd,n;
  int count =0;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr,servaddr;
  struct sendLine* recvline;

  if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0){
    perror("failed to create socket");
    exit(errno);    
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
  listen(listenfd,LISTENQ);
  printf("%s\n","server is running...waiting for connecting!");  
  for(;;){
    count++;
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);    
    printf("%s\n", "received request....");

    if((childpid = fork())==0){                  
      printf("child created to process the client request\n");
      close(listenfd);

      struct WeathMsg weathmsg;      
      struct recvLine sendline;
      char dayNum;            
      char sign[3];      
      sign[2]='\0';
      while((n=recv(connfd,buf,MAXLINE,0))>0){        
        recvline = (struct sendLine*)buf;
        
        bzero(&weathmsg,sizeof(weathmsg));
        memset(sign,0x00,2);
        dayNum = 0x00;        

        /*processing*/                                        
        if(recvline->lineHead[0]==0x01){    //comes the city name
          if(findCity(recvline->city)==1){  //have msg about the msg
            sign[0]=0x03;
            sign[1]=0x00;
            sendline = makeMsg(weathmsg,recvline->city,0x00,sign,0);
            send(connfd,(char*)&sendline,sizeof(sendline),0);
          }
          else{                             // don't have msg about the city
            sign[0]=0x04;            
            sign[1]=0x00;
            sendline = makeMsg(weathmsg,recvline->city,0x00,sign,0);
            send(connfd,(char*)&sendline,sizeof(sendline),0);
          }
        }
        else{
          int sendNum;    // one day or three day
          if(recvline->lineHead[1]==0x01){      // one day's weather msg                                
            sign[1]=0x41;          
            sendNum=0;            
            if(recvline->lineTail < 0x06){
              sign[0]=0x01;   
            }
            else{
              sign[0]=0x02; 
            }
          }
          else{                                 // three day's weather msg                    
            sign[0]=0x01; sign[1]=0x42;            
            sendNum=3;
          }
          weathmsg = randWeath();
            
          dayNum = recvline->lineTail;
          sendline = makeMsg(weathmsg,recvline->city,dayNum,sign,sendNum);                          
          send(connfd,(char*)&sendline,sizeof(sendline),0);
          printf("send a weather forecast message to the client %d...\n",count);
        }
        /*proessingh*/

        memset(recvline,0,strlen((char*)recvline));
        memset(buf,0,MAXLINE);
      }    
      if(n<0){
        printf("%s\n", "read error");                    
      }            
      printf("Connect %d terminated... \n", count);          
      exit(0);      
    }    
    close(connfd);    
  }
}