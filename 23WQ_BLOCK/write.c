#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int fd = open("/dev/WQ_BLOCK", O_RDWR);

    char buff[32] = "hello world\n";
    int ret = -1;

    printf("fd is %d\n", fd);

    if (fd > 0)
        printf("WQ_BLOCK open is success....\n");
    else
        printf("WQ_BLOCK open is error....\n");
    printf("write before\n");
    write(fd,buff,sizeof(buff));
    printf("write after\n");
    close(fd);

    return 0;
}
