#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "WiringPi/wiringPi/wiringPi.h"
#include "libcsv/csv.h"

pthread_t client; //thread for client
pthread_t server; //thread for server
uint16_t frameBuf[441600]; //160*120*23
const char * fileIn;


/* This code provides basic functionality to and from the MUX on board 1 from 
 * RPI1. The GPIO pins will read 6 RGB values per cycle, with 4 cycles of 
 * selector bits for a full 24-bit RGB word.
 * */
int RPi1Setup(void)
{
	wiringPiSetup();
	//set pin modes
	pinMode(7, INPUT); //GPIO 4  -- Output O1
	pinMode(0, INPUT); //GPIO 17 -- Output O2
	pinMode(1, INPUT); //GPIO 18 -- Output O3
	pinMode(2, INPUT); //GPIO 21 -- Output O4
	pinMode(3, INPUT); //GPIO 22 -- Output O5
	pinMode(4, INPUT); //GPIO 23 -- Output O6
	pinMode(5, OUTPUT);  //GPIO 24 -- Input  S1
	pinMode(6, OUTPUT);  //GPIO 25 -- Input  S0
	return 0;

}
uint8_t * GPIOMux(uint8_t * RGB){ //reads 24-bit RGB from GPIO
	// "00"
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
	RGB[23] = (uint8_t)digitalRead(7);
	RGB[22] = (uint8_t)digitalRead(0);
	RGB[21] = (uint8_t)digitalRead(1);
	RGB[20] = (uint8_t)digitalRead(2);
	RGB[19] = (uint8_t)digitalRead(3);
	RGB[18] = (uint8_t)digitalRead(4);
	// "01"
	digitalWrite(5, LOW);
	digitalWrite(6, HIGH);
	RGB[17] = (uint8_t)digitalRead(7);
	RGB[16] = (uint8_t)digitalRead(0);
	RGB[15] = (uint8_t)digitalRead(1);
	RGB[14] = (uint8_t)digitalRead(2);
	RGB[13] = (uint8_t)digitalRead(3);
	RGB[12] = (uint8_t)digitalRead(4);
	// "10"
	digitalWrite(5, HIGH);
	digitalWrite(6, LOW);
	RGB[11] = (uint8_t)digitalRead(7);
	RGB[10] = (uint8_t)digitalRead(0);
	RGB[9] 	= (uint8_t)digitalRead(1);
	RGB[8] 	= (uint8_t)digitalRead(2);
	RGB[7]	= (uint8_t)digitalRead(3);
	RGB[6] 	= (uint8_t)digitalRead(4);
	// "11"
	digitalWrite(5, HIGH);
	digitalWrite(6, HIGH);
	RGB[5] = (uint8_t)digitalRead(7);
	RGB[4] = (uint8_t)digitalRead(0);
	RGB[3] = (uint8_t)digitalRead(1);
	RGB[2] = (uint8_t)digitalRead(2);
	RGB[1] = (uint8_t)digitalRead(3);
	RGB[0] = (uint8_t)digitalRead(4);
	return RGB;
}

uint16_t displayData(uint8_t * GPIO){//converts from 24-bit RGB to 16-bit RGB
	uint8_t r = (GPIO[23] << 7) | (GPIO[22] << 6) | (GPIO[21] << 5) | (GPIO[20] << 4) | (GPIO[19] << 3) | (GPIO[18] << 2) | (GPIO[17] << 1) | GPIO[16];
	uint8_t g = (GPIO[15] << 7) | (GPIO[14] << 6) | (GPIO[13] << 5) | (GPIO[12] << 4) | (GPIO[11] << 3) | (GPIO[10] << 2) | (GPIO[9] << 1) | GPIO[8];
	uint8_t b = (GPIO[7] << 7) | (GPIO[6] << 6) | (GPIO[5] << 5) | (GPIO[4] << 4) | (GPIO[3] << 3) | (GPIO[2] << 2) | (GPIO[1] << 1) | GPIO[0];
	return (r << 11) | (g << 5) | b;
}

//CSV Code

struct counts {
	int fields;
	int rows;
	int frameCount;
	int sockfd;
	uint16_t frame[160][120];
};

void cb1 (void *s, size_t len, void *data) {
	((struct counts *)data)->frame
		[((struct counts *)data)->fields%160][((struct counts *)data)->rows%120] = atoi(s);
	printf("%u\t", 
		((struct counts *)data)->frame
			[((struct counts *)data)->fields%160][((struct counts *)data)->rows%120]);
	((struct counts *)data)->fields++;
	}
void cb2 (int c, void *data) {
	printf("\n");
	((struct counts *)data)->rows++;
	if (((struct counts *)data)->rows%120 == 0)
		frameWrite(int sockfd)
		//printf("Frame %d\n", ((struct counts *)data)->frameCount++); 
	}
	
int frameWrite(int sockfd){
	char sendBuff[38401]; //160*120*2+1
	uint16_t socket[160][120]; 
	int i = 0; int j = 0;
	for(i = 0; i < 160; i++){
		for(j = 0; j < 120; j++){
			socket[i][j] = ((struct counts *)data)->frame[i][j];
	//		printf("%x", socket[i][j]);
			sendBuff[i*j] = socket[i][j] & 0xFF;
			sendBuff[i*j+1] = socket[i][j] >> 8;
		}
	}
	write(sockfd, sendBuff, strlen(sendBuff)); //write to RPi3
	return 0;
}
	
int csvRead(int sockfd){
	FILE *fp;
	struct csv_parser p;
	char buf[1024];
	size_t bytes_read;
	uint16_t frame[160][120];
	struct counts c = {0, 0, 0, sockfd};
	
	if (csv_init(&p, 0) != 0) exit(EXIT_FAILURE);
	fp = fopen(fileIn, "rb");
	if (!fp) exit(EXIT_FAILURE);
		
	while ((bytes_read=fread(buf, 1, 1024, fp)) > 0)
		if (csv_parse(&p, buf, bytes_read, cb1, cb2, &c) != bytes_read) {
			fprintf(stderr, "Error while parsing file: %s\n",
			csv_strerror(csv_error(&p)) );
			exit(EXIT_FAILURE);
		}
		
	csv_fini(&p, cb1, cb2, &c);
	
	fclose(fp);
	printf("%d fields, %d rows\n", c.fields, c.rows);
	
	csv_free(&p);
	return 0;
}

//Networking Code

int socketServer(void)
{
  
  struct sockaddr_in serv_addr;
 
  char recvBuff[3];  
  //int numrv;  
 
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
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
  /*int connfd = */accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
  while(1)
  {
	int n = 0 ;
	if ((n = read(listenfd, recvBuff, sizeof(recvBuff)-1)) > 0) //recvBuff now contains the desired word, data is valid
	{
		pthread_join(client, NULL); //write to RPi3
	}
  }
}

void *socketClient(void *ptr)
{
  int sockfd = 0;//,n = 0;
  
  struct sockaddr_in serv_addr;
 
  memset(sendBuff, '0' ,sizeof(sendBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0) //establish socket
    {
      printf("\n Error : Could not create socket \n");
    }
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(17350);
  serv_addr.sin_addr.s_addr = inet_addr("192.168.0.102");
 
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) //connect to socket
    {
      printf("\n Error : Connect Failed \n");
    } 
	/*
	while(1)
	{
		uint8_t RGB[24];
		uint16_t socket[160][120];
		int i = 0; int j = 0;
		for(i = 0; i < 160; i++){
			for(j = 0; j < 120; j++){
				socket[i][j] = displayData((uint8_t *)GPIOMux(RGB));
	//			printf("%x", socket[i][j]);
				sendBuff[i*j] = socket[i][j] & 0xFF;
				sendBuff[i*j+1] = socket[i][j] >> 8;
			}
		}
	*/
	
	while(1)
	{
		csvread(sockfd);
		//pthread_join(server, NULL); //read from RPi2
		
	}
	
}


int main(int argc, char *argv[])
{
	RPi1Setup(); //set up RPi1 with wiringPi and correct pin modes
	fileIn = argv[1];
	pthread_create(&client, NULL, socketClient, NULL); //create thread for client using function socketClient
	//pthread_create(&server, NULL, socketServer, NULL); //create thread for server using function socketServer
	pthread_join(client, NULL);
	return 0;
}
