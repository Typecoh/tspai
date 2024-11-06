#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>

int main(int argc, char const *argv[])
{

    int fd = open("/dev/POLL", O_RDWR);

    char buff[32] = {0};
    int ret = -1;
    struct pollfd fds[1];

    printf("fd is %d\n", fd);

    if (fd > 0)
        printf("POLL open is success....\n");
    else
        printf("POLL open is error....\n");

    fds[0].fd = fd;
    fds[0].events = POLLIN; // 监视fd这个文件中是不是有可读的数据

    // 进行轮询查询
    while (1)
    {
        ret = poll(fds, 1, 3000); // 轮询
        if (!ret)
            printf("poll timeout\n");
        else
        {
            printf("poll success\n");
            if (fds[0].revents == POLLIN) // 返回的mask 会 给到 revents
            {
                read(fd, buff, sizeof(buff));
                printf("buff is %s\n", buff);
                sleep(1);
            }
        }
    }

    close(fd);

    return 0;
}
