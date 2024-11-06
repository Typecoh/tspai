#include <stdio.h>

struct inner {
    int a;
};

struct Mytest {
    int a;
    int b;
    int (*check)();
    struct inner myinner;
};

int check() {
    printf ("this is check function\n");
    return 0;
}

int main()
{

    struct Mytest test = {
        .a = 10,
        .b = 20,
        .check = check,
        .myinner = {
            .a = 30,
        }
    };

    test.check();  

    printf("test.a = %d, test.b = %d\n", test.a, test.b);

    printf("myinner.a = %d\n",test.myinner.a);

    return 0;
}