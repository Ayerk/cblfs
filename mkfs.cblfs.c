
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//mkfs.cblfs


int main(){

  // open the device
  int fd = open("/dev/nvme0n1",O_RDWR);
  if(fd<0){
    perror("open");
    return 1;
  }

  // allocate a buffer
  char* buffer = malloc(4096);
  if(!buffer){
    perror("malloc");
    return 1;
  }
  memset(buffer,0,4096);

  strcpy(buffer,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

  // write to the device
  int ret = write(fd,buffer,strlen(buffer));
  if(ret<0){
    goto clean;
    perror("write");
  } 
  printf("write operation completed successfully\n");
  close(fd);

  fd = open("/dev/nvme0n1",O_RDWR);
  if(fd<0){
    perror("open");
    return 1;
  }
  memset(buffer,0,4096);

  // read from the device
  ret = read(fd,buffer,4096);
  if(ret<0){
    goto clean;
    perror("read");
  }
  printf("read : %s\n",buffer);


clean:
  free(buffer);
  close(fd);


}
