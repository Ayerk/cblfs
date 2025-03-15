
#include <fcntl.h>
#include <stdio.h>


//mkfs.ext4


int main(){

  int fd = open("/dev/nvme0n1",O_RDWR);
  if(fd<0){
    perror("open");
    return 1;
  }
}
