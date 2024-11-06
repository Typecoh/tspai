#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int fd = open("/dev/WQ_NONBLOCK", O_RDWR | O_NONBLOCK);

    char buff[32] = {0};
    int ret = -1;

    printf("fd is %d\n", fd);

    if (fd > 0)
        printf("WQ_NONBLOCK open is success....\n");
    else
        printf("WQ_NONBLOCK open is error....\n");

    // 进行轮询查询
    while (1)
    {
        read(fd,buff,sizeof(buff));
        printf("buff is %s\n", buff);
        sleep(1);
    }
    close(fd);

    return 0;
}
