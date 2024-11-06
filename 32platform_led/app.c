#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    int fd1 = open("/dev/MyLed", O_RDWR);

    char buff1[32] = {0};
    int ret1 = -1;

    printf("fd1 is %d\n", fd1);

    if (fd1 > 0)
        printf("MyLed open is success....\n");
    else
        printf("MyLed open is error....\n");

    buff1[0] = atoi(argv[1]);
    
    ret1 = write(fd1, buff1, sizeof(buff1));

    close(fd1);

    return 0;
}
