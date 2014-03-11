#include <stdio.h>
#include <stdlib.h>
#include "client.h"


void fresh(void){
    system("clear");
    printf("%s\n","Please enter the given number to query");
    printf("%s\n%s\n%s\n","1.today","2.three days from today","3.custom day by yourself");
    printf("%s\n","(r)back,(c)cls,(#)exit");
    printf("%s\n","============================================================");
}

void freshCity(void){
    system("clear");
    printf("%s\n","Welcome to NJUCS Weather Forecast Demo Program!");
    printf("%s\n%s\n", "Please input City Name in Chinese pinyin(e.g. nanjing or beijing)","(c)cls,(#)exit");
}
