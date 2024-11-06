#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
int main(int argc, char const *argv[])
{
    char buff[32] = "123456";
    
    int fd = open("/dev/chardev", O_RDWR);
    if (fd == -1)
    {
        printf("open failed...\n");
        return -1;
    }

    write(fd, buff, sizeof(buff));
    printf("buff is %s\n",buff);
    memset(buff,0,sizeof(buff));
    read(fd, buff, 1024);
    printf("buff is %s\n",buff);
    close(fd);
    return 0;
}
