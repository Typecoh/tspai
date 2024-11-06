#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{

    int fd = open("/dev/test", O_RDWR);

    if (fd > 0)
    {
        printf("open file is success...\n");
    }

    else
    {
        printf("open file is faild... \n");
    }

    return 0;
}
