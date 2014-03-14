#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "server.h"



struct WeathMsg randWeath(void){
	struct WeathMsg weathMsg;

	//dateNum 
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);	
	int year = p->tm_year+1900;
	int month = p->tm_mon+1;
	int day = p->tm_mday;
	weathMsg.dateNum[0] = year>>8;
	weathMsg.dateNum[1] = year-(weathMsg.dateNum[0]<<8);
	weathMsg.dateNum[2] = month;
	weathMsg.dateNum[3] = day;

	enum Weather weath;	
	int i=0;
	srand((unsigned)time(0));
	while(i<5){		
		weath = rand()%5;
		switch(weath){
			case shower:weathMsg.weatherMsg[i] = 0x00;break;
			case clear:weathMsg.weatherMsg[i] = 0x01;break;
			case cloudy:weathMsg.weatherMsg[i] = 0x02;break;
			case rain:weathMsg.weatherMsg[i] = 0x03;break;
			case fog:weathMsg.weatherMsg[i] = 0x04;break;
			default:break;
		}	
		
		weathMsg.weatherMsg[i+1] = rand()%40;	
		i += 2;
	}	
	memset(weathMsg.zero,0,10);
	return weathMsg;
}

