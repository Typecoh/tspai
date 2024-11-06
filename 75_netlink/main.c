#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>
struct sockaddr_nl nl = {
    .nl_family = AF_NETLINK,
    .nl_pid = 0,
    .nl_groups = 1,
};

char buff[4096];
int len = 0;

int main()
{
    int ret = 0;

    int socketid = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);

    ret = bind(socketid, (struct sockaddr *)&nl, sizeof(struct sockaddr_nl));

    while (1)
    {
        memset(&buff, 0, sizeof(buff));
        ret = recv(socketid, &buff, 4096, 0);

        for (int i = 0; i < ret; i++)
        {
            if (buff[i] == '\0')
                buff[i] = '\n';
        }

        printf("buff is %s",buff);
    }

    return 0;
}