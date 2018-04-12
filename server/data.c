#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data.h"
#include <stdbool.h>
/*
#define SERIAL_LOC = 2;
#define ZN_LOC = 0;
#define ZT_LOC = 1;
*/

int serial_number_validate(char data[]){
    printf("Serial number: %s\n", data);
	printf("serial number length %d\n", sizeof(data));
   for(int i = 0 ; i<=5 ;i++){
       if(!(( data[i]>='a' && data[i]<='z') || (data[i]>='A' && data[i]<='Z') ||(data[i]>='0' && data[i]<='9'))){
            printf("Invalid serial number\n");
            return 0;
        }
    }
    printf("Serial number valid\n");
    return 1;
}

int zone_type_validate(char data[]){
    printf("Zone type: %s\n", data);
    int value = atoi(data);
    if(value <0 || (value > 37 && value < 41 && value != 41 && value!= 81 && value != 87 && value != 88) ){
        printf("Invalid zone type \n");
        return 0;
    }
    else{
        printf("Zone type valid\n");
        return 1;
    }
return 1;
}

int zone_number_validate(char data[]){
    printf("Zone number: %s\n", data);
    int value = atoi(data);
    if(value<0 || value>64){
        printf("Invalid zone number\n");
        return 0;
    }
    else{
        printf("Zone number valid\n");
        return 1;
    }
return 1;
}

int break_data(char data_in[]){
    printf("data in, size: %d\n", sizeof(data_in)) ;
    if(sizeof(data_in) != 8 || data_in[0] == "" ){
	return 0;
    }
    char* serial_number;
    char* zone_type = malloc(2);
    char* zone_number= malloc(2);
    for (int i = 0, j = 0, y = 0, z=0; i<=sizeof(data_in)*2; i++){
        if (data_in[i] == '+'){
            j++;
            y = 0;
        }
        else{
		if(j==0){
                    zone_number[y] = data_in[i];
                    y++;
		}
                else if( j== 1){
                    zone_type[y] = data_in[i];
                    y++;
                }
                else if(j>=2 && j<=7){
                    serial_number[z] = data_in[i];
                    y++;
		    z++;
                }
            }
    }
    return (serial_number_validate(serial_number) *  zone_type_validate(zone_type) * zone_number_validate(zone_number));
}
