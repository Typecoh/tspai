#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

    printf("open success !\n");
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

// 设置 GPIO的输入/输出
int GPIO_MODE(char *number, char *dir, char *val)
{
    int fd1;
    int fd2;
    char direction[100];
    char value[100];
    sprintf(direction, "/sys/class/gpio/gpio%s/direction", number);
    sprintf(value, "/sys/class/gpio/gpio%s/value", number);
    // 将 /sys/class/gpio/gpio108 这个路径下的 direction 改成 输出功能
    // 将 /sys/class/gpio/gpio108 这个路径下的 value 设置成 高电平

    // printf("direction = %s\n", direction);
    // printf("value     = %s\n", value);

    fd1 = open(direction, O_WRONLY);
    fd2 = open(value, O_WRONLY);

    // 指定 模式
    int ret;
    ret = write(fd1, dir, sizeof(dir));
    ret = write(fd2, val, sizeof(val));

    return 0;
}

int main(int argc, char *argv[])
{

    sprintf(gpios, "/sys/class/gpio/gpio%s", argv[1]);

    printf("%s\n", gpios);

    if (access(gpios, F_OK) == 0)
    {
        printf("file is exit!\n");
        unexport(argv[1]);
    }
    else
    {
        printf("file is not exit!\n");
        export(argv[1]);
    }

    GPIO_MODE(argv[1], argv[2], argv[3]);

    return 0;
}
