#include <stdio.h>
#include <stdlib.h>
#include "client.h"


int main(int argc,char** argv){  
  int sockfd;
  struct sockaddr_in servaddr;  
  char recvBuf[MAXBUF];
  char getInput[MAXBUF];  

  struct recvLine *recvline;
  struct sendLine sendline;  
  
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    perror("failed to create the socket!");
    exit(errno);
  }

  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");    

  if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
    perror("failed to connet the server!");
    exit(errno);
  }

  freshCity();
  char headSign[2] = {0x01,0x00};
  char tailSign = 0x00;
  int endSign = 0;

  /*input the city name*/
  while(fgets(getInput,MAXBUF,stdin)!=NULL){
    getInput[strlen(getInput)-1] = '\0';    
    trim(getInput);
    endSign=0;
    
    if(getInput[0]==0x63 && strlen(getInput)==1){              //input 'c' to clear
      freshCity();
    }
    else if(strlen(getInput)==0){       //just enter
      continue;
    }
    else if(getInput[0]==0x23 && strlen(getInput)==1){         //input '#' to end program running
      break;      
    }
    else{
      sendline = makeSendline(getInput,headSign,tailSign);

      send(sockfd,(char*)&sendline,sizeof(sendline),0);

      if(recv(sockfd,recvBuf,MAXBUF,0)==0){
        perror("the server terminated prematurely!");
        exit(errno);
      }          
      recvline = (struct recvLine*)recvBuf;      
      display(recvline);        

      if(recvline->lineHead[0]==0x03){        //the city name is correct            
          char dayNum[MAXBUF];

          char packHead[2] = {0x02,0x01};
          char packTail = 0x01;

          int backSign = 0;
          int sendSign = 1;

          /*input the number to request the weather message*/
          while(fgets(dayNum,MAXBUF,stdin)!=NULL){          
            dayNum[strlen(dayNum)-1] = '\0';
            trim(dayNum);
            backSign = 0;
            sendSign = 1;
            if(strlen(dayNum)==0){
              continue;
            }
            else if(strlen(dayNum)!=1){
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
                  while(fgets(dayNum,MAXBUF,stdin)!=NULL){                                  
                    dayNum[strlen(dayNum)-1] = '\0';                                                                        
                    if(strlen(dayNum)==0){
                      continue;
                    }
                    else if(strlen(dayNum)!=1){
                      printf("%s\n","input error");
                      printf("%s","Please enter the day number(below 10, e.g. 1 means today):");                                          
                    }                  
                    else{
                      packHead[0]=0x02; packHead[1]=0x01;
                      if(dayNum[0]>=0x31 && dayNum[0]<=0x39){
                        packTail = dayNum[0] - 0x30;
                        break;
                      }
                      else{
                        printf("%s\n","input error");                    
                        printf("%s","Please enter the day number(below 10, e.g. 1 means today):");
                      }                                  
                    }
                  }              
                break;
                case 0x72: backSign=1;freshCity(); sendSign = 0; break;    //input 'r' to turn back 
                case 0x63: fresh(); sendSign = 0; break;          //input 'c' to clear
                case 0x23: endSign =1; sendSign = 0; break;            //input '#' to end the program     
                default:
                  printf("%s\n","input error"); 
                  sendSign=0; break;
              }

              if(backSign==1 || endSign==1){
                break;
              } 

              if(sendSign==1){
                sendline = makeSendline(recvline->city,packHead,packTail);    
                send(sockfd,(char*)&sendline,sizeof(sendline),0);
                if(recv(sockfd,recvBuf,MAXBUF,0) ==0){
                   perror("the server terminated prematurely!");
                   exit(errno);
                };
                recvline = (struct recvLine*)recvBuf;
                WeathMsgDisp(recvline);  //display the weather message!              
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


