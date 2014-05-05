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
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
void socketServer(char* fbp, long int screensize) //receive from RPi1
{
  int listenfd = 0/*, connfd = 0*/;
  
  struct sockaddr_in serv_addr;
 
  char recvBuff[screensize];  

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
  }
  /*connfd =*/ accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

  //interpret data from RPi1
  while(1)
  {
	  int n = 0;
	  if((n = read(listenfd, recvBuff, sizeof(recvBuff)-1)) > 0)
	  {
		long int i = 0;
		for(i = 0; i < screensize; i++)	memset(fbp+i, recvBuff[i], 1);
	  }
  }
}

int main(int argc, char* argv[])
{
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;

  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (!fbfd) {
    printf("Error: cannot open framebuffer device.\n");
    return(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    printf("Error reading fixed information.\n");
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable information.\n");
  }
  printf("%dx%d, %d bpp\n", vinfo.xres, vinfo.yres, 
         vinfo.bits_per_pixel );

  // map framebuffer to user memory 
  screensize = finfo.smem_len;

  fbp = (char*)mmap(0, 
                    screensize, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED, 
                    fbfd, 0);

  if ((int)fbp == -1) {
    printf("Failed to mmap.\n");
  }
  else socketServer(fbp);
  munmap(fbp, screensize);
  close(fbfd);
  return 0;
}