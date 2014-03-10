#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
 
int socketServer(void)
{
  int listenfd = 0,connfd = 0;
  
  struct sockaddr_in serv_addr;
 
  char recvBuff[2];  
  int numrv;  
 
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(recvBuff, '0', sizeof(recvBuff));
      
  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(17350);    
 
  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  
  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }
  
  connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

  return 0;
}

int socketClient(void)
{
  int sockfd = 0,n = 0;
  char sendBuff[2];
  struct sockaddr_in serv_addr;
 
  memset(sendBuff, '0' ,sizeof(sendBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    }
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(17350);
  serv_addr.sin_addr.s_addr = inet_addr("192.168.0.101");
 
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }
  //begin MUX incrementation
	uint16_t hcount = 0; //used to generate 60Hz*480 clock
	uint16_t vcount = 0; //used to generate 60Hz clock
	//basic implementation of 60Hz clock -- NOT FINAL
	while(1){//loop indefinitely
		//Check valid data first
		if (hcount < 640){ //0-639 valid data
			if ((vcount < 480) && ((hcount % 4) == 0)){ //0-479 valid data, hcount has a factor of 4
				//tell RPI1 to inc mux
				sendBuff = (char*)(hcount << 2); //cast to char*
				write(sockfd, sendBuff, strlen(sendBuff));
			}
		}
		//HSYNC first
		//@660 -> HSYNC set low
		if (hcount == 660) digitalWrite(7, LOW);
		//@756 -> HSYNC set high
		else if (hcount == 756) digitalWrite(7, HIGH);
		else if (hcount == 800) {
			//VSYNC Second
			//@494 -> VSYNC set low
			if (vcount == 494) digitalWrite(1, LOW);
			//@495 -> VSYNC set high
			else if (vcount == 495) digitalWrite(1, HIGH);
			vcount = (vcount++) % 525; //increment VSYNC < 525
		}
		hcount = (hcount++ % 800); //increment HSYNC < 800
	}
  return 0;
}

int RPi2Setup(void)
{
	wiringPiSetup();
	//set modes
	pinMode(7, OUTPUT);	// GPIO 4  -- HSYNC0
	pinMode(0, INPUT);	// GPIO 17 -- HSOUT
	pinMode(1, OUTPUT);	// GPIO 18 -- VSYNC0
	pinMode(2, INPUT);	// GPIO 21 -- VSOUT
	pinMode(3, OUTPUT);	// GPIO 22 -- A0
	pinMode(4, INPUT);	// GPIO 23 -- EXTCLK
	pinMode(5, OUTPUT);	// GPIO 24 -- CLAMP
	pinMode(6, OUTPUT);	// GPIO 25 -- COAST
	//set clocks
	digitalWrite(7, HIGH);
	digitalWrite(1, HIGH);
	return 0;
}

int main(void)
{
	pthread_t server;
	pthread_t client;
	pthread_create(&client, NULL, socketClient, NULL);
	pthread_create(&server, NULL, socketServer, NULL);
	return 0;
}
