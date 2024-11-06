#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{

    int fd = open("/dev/test", O_RDWR);

    if (fd > 0)
        printf("open file is success...\n");
    else
        printf("open file is faild... \n");

    // read
    char buff[32];
    read(fd, buff, sizeof buff);

    close(fd);

    return 0;
}
