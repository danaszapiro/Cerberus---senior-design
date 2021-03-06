/*
    udp server socket for Alarm.com
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "parser.h" 
#include "data.h"
//#include "tcpSocket.h"
#define BUFLEN 1024  //Max length of buffer
#define PORT 80  //The port for incoming data
#define headerLength = 3;
#define checksumLength = 1;
#define validMessageLength = 13; // 3 bytes of message type + 10 bytes of data
#define validMessageLength2 = 8;
 
void kill(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
//udp var
    struct sockaddr_in si_me, si_other;     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
    char *buf_parsed;
    char replay[1]; 


    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        kill("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        kill("bind");
    }
    else
    {
        printf("UDP Listening on port: %d\n", PORT);
    } 

    //keep listening for data
    while(1)
    {
        printf("Waiting for data...\n");
        fflush(stdout);
         
        //try to receive data
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            kill("recvfrom()");
        }
         
        //print details of the client  and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data length: %d bytes\n" , recv_len);
        
	//Call parser to parse buf  and print the relevant data 
	printf("Parsing data...\n");
        buf_parsed=parser(buf, recv_len);
        //printf("Parsed Data: %s\n", buf_parsed);
	int sensor_validate = break_data(buf_parsed);
        if (sensor_validate == 1 )
        {
            printf("Valid sensor, sucessfull integration\n");
	    replay[0] = 1;
        }
        else
        {
            printf("Invalid sensor, cannot perform integration\n");
            replay[0] = 0;
	}
       //now reply the client with the same data
        if (sendto(s, replay, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            kill("sendto()");
        }
    }
//tcp();     
    close(s);
    return 0;
}
