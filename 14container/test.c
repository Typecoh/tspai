#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{

    int fd1 = open("/dev/test1", O_RDWR);

    if (fd1 > 0)
        printf("file1 open successs...\n");
    char buff1[32];
    read(fd1, buff1, sizeof buff1);
    close(fd1);

    int fd2 = open("/dev/test2", O_RDWR);

    printf("fd2 is %d\n", fd2);

    if (fd2 > 0)
        printf("file2 open successs...\n");
    char buff2[32];
    read(fd2, buff2, sizeof buff2);
    close(fd2);

    return 0;
}
