    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <string.h>
    #include <stdlib.h>
    #include <sys/ioctl.h>

    #define LED_ON_FUNC _IOW('L', 0, int)
    #define LED_OFF_FUNC _IOW('L', 1, int)

    struct args{
        int a;
        int b;
        int c;
    };

    int main(int argc, char const *argv[])
    {

        int fd = open("/dev/ICOT", O_RDWR);

        int buff = 0;
        int ret1 = -1;

        printf("fd is %d\n", fd);

        struct args Myargs;

        Myargs.a = 1;
        Myargs.b = 0;
        Myargs.c = 1;

        ioctl(fd, LED_ON_FUNC, &Myargs);

        close(fd);

        return 0;
    }
