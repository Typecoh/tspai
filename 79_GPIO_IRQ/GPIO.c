#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>

char gpios[100];
// 创建 gpio108 ==> GPIO3_B4
int export(char *argv)
{
    int fd;
    int ret;
    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (fd < 0)
    {
        printf("open file is error...\n");
        return -1;
    }

    ret = write(fd, argv, sizeof(argv));
    if (ret < 0)
    {
        printf("write is error...\n");
        return -2;
    }

    close(fd);
}
// 删除 gpio108 ==> GPIO3_B4
int unexport(char *argv)
{
    int fd;
    int ret;
    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (fd < 0)
    {
        printf("open file is error...\n");
        return -1;
    }

    ret = write(fd, argv, sizeof(argv));
    if (ret < 0)
    {
        printf("write is error...\n");
        return -2;
    }
}

void pinctrl(char *number, char *path, char *value)
{

    char paths[100];
    sprintf(paths, "/sys/class/gpio/gpio%s/%s", number, path);

    int fd = open(paths, O_WRONLY);

    if (fd < 0)
    {
        printf("open is error...\n");
    }

    int ret = write(fd, value, sizeof(value));

    if (ret < 0)
    {
        printf("write is error...\n");
    }

    close(fd);
}

int gpio_read_value(char *number)
{
    int fd1;
    char value[100];
    char buf[1];

    sprintf(value, "/sys/class/gpio/gpio%s/value", number);
    // 将 /sys/class/gpio/gpio108 这个路径下的 value 设置成 高电平

    fd1 = open(value, O_RDONLY);

    if (fd1 < 0)
    {
        printf("fd1 : open file is error...\n");
    }

    // 指定 模式
    int ret;
    printf("path : %s\n", value);

    ret = read(fd1, buf, sizeof(buf));

    if (ret < 0)
    {
        printf("ret : open file is error...\n");
        return -1;
    }

    printf("value = %s", buf);

    close(fd1);

    return 0;
}

int gpio_interrupt(char *number, char *value)
{
    char paths[100];

    sprintf(paths, "/sys/class/gpio/gpio%s/%s", number, value);

    int fd = open(paths, O_RDONLY);

    if (fd < 0)
    {
        printf("open file_path error \n"); // 打开文件失败
        return -1;
    }

    // 定义poll 进行 阻塞检测
    struct pollfd fds[1];

    memset((void *)fds, 0, sizeof(fds)); // 清空 poll 结构体数组

    fds[0].fd = fd;
    fds[0].events = POLLPRI;

    char buff[2];

    read(fd, buff, 2); // 读取文件内容，获取中断事件的值

    int ret = poll(fds, 1, -1);

    if (ret <= 0)
    {
        printf("poll error \n"); // 调用 poll 失败或超时
        return -1;
    }

    if (fds[0].revents & POLLPRI)
    {
        read(fd, buff, 2); // 读取文件内容，获取中断事件的值
        buff[1] = '\0';
        printf("value is %s\n", buff); // 输出中断事件的值
    }
}

int main(int argc, char *argv[])
{

    sprintf(gpios, "/sys/class/gpio/gpio%s", argv[1]);

    if (access(gpios, F_OK) == 0)
    {
        unexport(argv[1]);
    }
    else
    {
        export(argv[1]);
    }

    pinctrl(argv[1], "direction", "in");
    pinctrl(argv[1], "edge", "both");
    gpio_interrupt(argv[1], "value");

    return 0;
}
