#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>

int fd;
int ret;
char gpio_path[100];
char file_path[100];
char buff[2];
struct pollfd fds[1];

int gpio_export(char *argv)
{
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0)
    {
        printf("open file is error...\n");
        return -1;
    }

    ret = write(fd, argv, sizeof(argv));

    if (ret < 0)
    {
        printf("write is error....\n");
        return -2;
    }

    close(fd);

    return 0;
}

int gpio_unexport(char *argv)
{
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0)
    {
        printf("open file is error...\n");
        return -1;
    }

    int ret = write(fd, argv, sizeof(argv));

    if (ret < 0)
    {
        printf("write is error....\n");
        return -2;
    }

    close(fd);

    return 0;
}

int gpio_ctrl(char *arg, char *val)
{

    sprintf(file_path, "%s/%s", gpio_path, arg);

    fd = open(file_path, O_WRONLY);
    if (fd < 0)
    {
        printf("open file_path is error...\n");
        return -1;
    }

    ret = write(fd, val, sizeof(val));

    if (ret < 0)
    {
        printf("write is error....\n");
        return -2;
    }

    close(fd);

    return 0;
}

int gpio_read_value(char *arg)
{
    sprintf(file_path, "%s/%s", gpio_path, arg);

    // printf("file_path = %s\n",file_path);

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        printf("open file_path is error...\n");
        return -1;
    }

    ret = read(fd, buff, 1);

    if (!strcmp(buff, "1"))
    {
        printf("the value is high!\n");
        return 1;
    }
    else if (!strcmp(buff, "0"))
    {
        printf("the value is low\n");
        return 0;
    }

    close(fd);

    return 0;
}

int gpio_interrupt(char *arg)
{
    sprintf(file_path, "%s/%s", gpio_path, arg);

    // printf("file_path = %s\n",file_path);

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        printf("open file_path is error...\n");
        return -1;
    }

    memset((void *)fds, 0, sizeof(fds));

    fds[0].fd = fd;
    fds[0].events = POLLPRI;

    read(fd, buff, sizeof(buff));

    ret = poll(fds, 1, -1);

    if (ret < 0)
    {
        printf("poll is error\n");
        return -1;
    }

    if (fds[0].revents & POLLPRI)
    {
        lseek(fd, 0, SEEK_SET);
        read(fd, buff, sizeof(buff));
        buff[1] = '\0';
        printf("value is %s\n",buff);
    }
    
    return 0;
}

int main(int argc, char *argv[])
{

    int value;
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);

    if (access(gpio_path, F_OK))
    {
        // printf("file is not exit...\n");
        gpio_export(argv[1]);
    }
    else
    {
        // printf("file is exit...\n");
        gpio_unexport(argv[1]);
    }

    gpio_ctrl("direction", "in");
    gpio_ctrl("edge", "both");
    // gpio_ctrl("value", argv[2]);
    // value = gpio_read_value("value");

    // printf("value is %d\n", value);
    gpio_interrupt("value");

    gpio_unexport(argv[1]);

    return 0;
}
