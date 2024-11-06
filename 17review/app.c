#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    int fd1 = open("/dev/test1", O_RDWR);
    int fd2 = open("/dev/test2", O_RDWR);

    char buff1[32] = "divices1 is open!";
    char buff2[32] = "devices2 is open!";
    int ret1 = -1;
    int ret2 = -1;

    printf("fd1 is %d\n", fd1);
    printf("fd2 is %d\n", fd2);

    if (fd1 > 0)
        printf("test1 open is success....\n");
    else
        printf("test1 open is error....\n");

    if (fd2 > 0)
        printf("test2 open is success.....\n");
    else
        printf("test2 open is error...\n");

    ret1 = write(fd1, buff1, sizeof(buff1));
    ret2 = write(fd2, buff2, sizeof(buff2));


    memset(buff1, 0, sizeof(buff1));
    memset(buff2, 0, sizeof(buff2));

    ret1 = read(fd1, buff1, sizeof(buff1));
    ret2 = read(fd2, buff2, sizeof(buff2));
    close(fd1);
    close(fd2);

    return 0;
}
