#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

int findCity(char* cityStr){
	FILE  *fp;
	char cityName[100];

	/*open a file and read a row*/
	fp = fopen("Server/city.txt","r");
	if(fp==NULL){
		perror("Error opening file");
		return -1;
	}
	while(fgets(cityName,100,fp)!=NULL){
		cityName[strlen(cityName)-1] = '\0';					
		if(strncmp(cityStr,cityName,strlen(cityName)-1)==0){
		   if(cityStr[strlen(cityName)-1]==0x20 || cityStr[strlen(cityName)-1]=='\0'){   //the cityStr is the str before the space or whole string
		   	  memset(cityStr+strlen(cityName)-1,0,1);   //memset 0 from the space to the end 
		   	  return 1;		   	  
		   }			
		}
	}
	return 0;
}