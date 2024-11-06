#include <stdio.h>

struct sizea
{
    int a;
    double b;
};


struct sizeb
{
    int a;
    struct sizea b;
};


int main()
{

    struct sizeb mysize;

    printf("mysize is %ld\n", sizeof(mysize));

    printf("a address is %p\n", &mysize.a);
    printf("b address is %p\n", &mysize.b.a);
    printf("b address is %p\n", &mysize.b.b);

    return 0;
}