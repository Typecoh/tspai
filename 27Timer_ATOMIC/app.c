#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int fd = open("/dev/ATOMIC", O_RDWR);

    int buff = 0;
    int ret1 = -1;

    printf("fd is %d\n", fd);

    if (fd > 0)
        printf("ATOMIC open is success....\n");
    else
        printf("ATOMIC open is error....\n");
    
    while(1){
        read(fd,&buff,sizeof(buff));
        sleep(1);
        printf("sec is %d\n", buff);
    }

    close(fd);

    return 0;
}
