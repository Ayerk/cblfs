#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/nvme_ioctl.h>
#include <sys/ioctl.h>

//mkfs.cblfs
//dd if=/dev/zero of=/dev/nvme0n1 bs=5M count=4096


int oper_posix(const char *filename){

  // open the device
  int fd = open(filename,O_RDWR);
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
    perror("write");
    goto clean;
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
    perror("read");
    goto clean;
  }
  printf("read : %s\n",buffer);
  return 0;

clean:
  free(buffer);
  close(fd);
  return 1; // 添加返回值

}

#define NVME_WRITE 1
#define NVME_READ 2

int oper_nvme(const char *filename){

  // open the device
  int fd = open(filename,O_RDWR);
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

  strcpy(buffer,"1ABCDEFGHIJKLMNOPQRSTUVWXYZ");


  // write
  struct nvme_user_io io;
  memset(&io,0,sizeof(io));

  io.addr = (__u64)buffer;
  io.slba = 1;//逻辑块号 一块为512字节
  io.nblocks = 15;//覆盖大小 (nblocks+1) * 256
  io.opcode = NVME_WRITE;

  if(-1==ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io)){//equal to write
    perror("ioctl write");
    goto clean;
  }
  printf("ioctl write operation completed successfully\n");


  // read
  memset(buffer,0,4096);
  io.opcode = NVME_READ;
  if(-1==ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io)){//equal to read
    perror("ioctl read");
    goto clean;
  }
  printf("ioctl read : %s\n",buffer);
  return 0;
clean:
  free(buffer);
  close(fd);
  return 1; // 添加返回值

}

int main(int argc,char* argv[]){
    if(argc<2){
        perror("arg");
        return -1;
    }

    const char *filename = argv[1];

    // oper_posix(filename);
    oper_nvme(filename);


    return 0;

}
