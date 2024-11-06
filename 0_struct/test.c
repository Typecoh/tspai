#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = 1;
    int b = 2;
    int *p1, *p2;
    p1 = &a;

    p2 = p1;

    printf("p1 add is %p, value is %d\n", p1, *p1);
    printf("p2 add is %p, value is %d\n", p2, *p2);

    p1 = &b;

    printf("p1 add is %p, value is %d\n", p1, *p1);
    printf("p2 add is %p, value is %d\n", p2, *p2);

    return 0;
}
