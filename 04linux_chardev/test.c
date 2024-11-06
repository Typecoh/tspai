#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    char buff[32] = {0};

    int fd = open("/dev/chardev", O_RDWR);
    if (fd == -1)
    {
        printf("open failed...\n");
        return -1;
    }
    write(fd, buff, sizeof(buff));
    read(fd, buff, sizeof(buff));
    close(fd);
    return 0;
}
