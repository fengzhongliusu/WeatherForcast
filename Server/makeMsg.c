#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

struct recvLine makeMsg(struct WeathMsg weathMsg,char* cityname,char dayNum,char* sign,int sendNum){
    struct recvLine recvline;
    strncpy(recvline.lineHead,sign,2);

    strncpy(recvline.city,cityname,sizeof(recvline.city));  
    recvline.city[strlen(cityname)] = '\0';
    
	recvline.year[0] = weathMsg.dateNum[0];
	recvline.year[1] = weathMsg.dateNum[1];
	recvline.month = weathMsg.dateNum[2];
	recvline.day = weathMsg.dateNum[3];
	recvline.inputNum = dayNum;
	if(sendNum==3){
		strncpy(recvline.weatherMsg,weathMsg.weatherMsg,6);
	}
	else{
		strncpy(recvline.weatherMsg,weathMsg.weatherMsg,2);	
	}
	memset(recvline.zero,0,94);
	return recvline;
}