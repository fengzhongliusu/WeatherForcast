#include <stdio.h>
#include <stdlib.h>
#include "client.h"


/**
*erase the space in the head
*/
void trim(char *str){
    int i=0;    
    while(str[i]==0x20){
      i++;
    }
    strncpy(str,str+i,strlen(str)-i);  // trim the space in the head 
    str[strlen(str)-i] = '\0';

    i=strlen(str)-1;
    while(str[i]==0x20){      
      i--;
    }
    memset(str+i+1,0,strlen(str)-i);  //trim the space in the end
}

/**
*construct the data to send
*/
struct sendLine makeSendline(char *getInput,char* lineHead,char lineTail){
   struct sendLine sendline;
   memset(&sendline,0,sizeof(sendline));
   strcpy(sendline.lineHead,lineHead);
   sendline.lineTail = lineTail;
   strncpy(sendline.city,getInput,sizeof(sendline.city));
   sendline.city[29] = 0;
   return sendline;
}


/**
*the message showing after input the city name
*/
void display(struct recvLine* recvline){
  if(recvline->lineHead[0]==0x03){
    fresh();
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
    printf("city: %s  Today is:%d/%d/%d   Weather information is as follows:\n",recvline->city,
    (recvline->year[0]<<8)+recvline->year[1],recvline->month,recvline->day);    
        
    if(recvline->inputNum==0x03 && recvline->lineHead[1]==0x42){      
      printf("The 1th day's weather is: %s;Temp:%d\n", weather[(int)recvline->weatherMsg[0]],recvline->weatherMsg[1]);
      printf("The 2th day's weather is: %s;Temp:%d\n", weather[(int)recvline->weatherMsg[2]],recvline->weatherMsg[3]);
      printf("The 3th day's weather is: %s;Temp:%d\n", weather[(int)recvline->weatherMsg[4]],recvline->weatherMsg[5]);      
    }
    else{
      if(recvline->inputNum==0x01){
         printf("Today's Weather is: %s;Temp:%d\n", weather[(int)recvline->weatherMsg[0]],recvline->weatherMsg[1]);                    
      }
      else{
         printf("The %dth Weather is: %s;Temp:%d\n",recvline->inputNum, weather[(int)recvline->weatherMsg[0]],recvline->weatherMsg[1]);                                 
      }      
    }

  }  
  else{
    printf("Sorry, no given day's weather information for city %s!\n",recvline->city);
  }
}
