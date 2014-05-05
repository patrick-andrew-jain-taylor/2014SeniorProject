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
/***************************************************
    To read a jpg image file and download
    it as a texture map for openGL
    Derived from Tom Lane's example.c
-- Obtain & install jpeg stuff from web 
    (jpeglib.h, jerror.h jmore.h, jconfig.h,jpeg.lib)
****************************************************/
#include <jpeglib.h>    
#include <jerror.h>
unsigned char * jdata;      // data for the image
//================================
unsigned char * LoadJPEG(char* FileName)
//================================
{
  unsigned long x, y;
  unsigned int texture_id;
  unsigned long data_size;     // length of the file
  int channels;               //  3 =>RGB   4 =>RGBA 
  unsigned int type;  
    unsigned char * rowptr[1];  // pointer to an array
  struct jpeg_decompress_struct info; //for our jpeg info
  struct jpeg_error_mgr err;          //the error handler

  FILE* file = fopen(FileName, "rb");  //open the file

  info.err = jpeg_std_error(& err);     
  jpeg_create_decompress(& info);   //fills info structure

   //if the jpeg file doesn't load
  if(!file) {
     fprintf(stderr, "Error reading JPEG file %s!", FileName);
     return 0;
  }

  jpeg_stdio_src(&info, file);    
  jpeg_read_header(&info, TRUE);   // read jpeg file header

  jpeg_start_decompress(&info);    // decompress the file

  //set width and height
    x = info.output_width;
    y = info.output_height;
 channels = info.num_components;
 type = GL_RGB;
      if(channels == 4) type = GL_RGBA;

   data_size = x * y * 3;

//--------------------------------------------
// read scanlines one at a time & put bytes 
//    in jdata[] array. Assumes an RGB image
//--------------------------------------------
  jdata = (unsigned char *)malloc(data_size);
  sendBuff = (char *)malloc(data_size);
  
  while (info.output_scanline < info.output_height) // loop
    {
// Enable jpeg_read_scanlines() to fill our jdata array
    rowptr[0] = (unsigned char *)jdata +  // secret to method
            3* info.output_width * info.output_scanline; 

   jpeg_read_scanlines(&info, rowptr, 1);
  }
//---------------------------------------------------

jpeg_finish_decompress(&info);   //finish decompressing
/*
//----- create OpenGL tex map (omit if not needed) --------
        glGenTextures(1,&texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
  gluBuild2DMipmaps(GL_TEXTURE_2D,3,x,y,GL_RGB,GL_UNSIGNED_BYTE,jdata);
  */

  fclose(file);                    //close the file
  return jdata;
}
void *socketClient(void *ptr)
{
  int sockfd = 0;//,n = 0;
  
  struct sockaddr_in serv_addr;
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
	for
  write(connfd, (char *)jdata, strlen(jdata));	
  free(jdata);
}
int main(int argc, char *argv[]){
	jdata = LoadJPEG(argv[0]);
	socketClient();
	return 0;
}