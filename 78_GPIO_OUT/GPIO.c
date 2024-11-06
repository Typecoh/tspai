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

int gpio_read_value(char *number)
{
    int fd1;
    char value[100];
    char buf[1];

    sprintf(value, "/sys/class/gpio/gpio%s/value", number);
    // 将 /sys/class/gpio/gpio108 这个路径下的 value 设置成 高电平

    fd1 = open(value, O_RDONLY);

    if(fd1 < 0){
        printf("fd1 : open file is error...\n");
    }

    // 指定 模式
    int ret;
    printf("path : %s\n",value);

    ret = read(fd1, buf, sizeof(buf));

    if(ret < 0){
        printf("ret : open file is error...\n");
        return -1;
    }

    printf("value = %s", buf);

    close(fd1);

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

    gpio_read_value(argv[1]);

    return 0;
}
