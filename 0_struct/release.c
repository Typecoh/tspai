#include <stdio.h>

struct Mytest {
    int a;
    int b;
    int (*check)();
};

int main()
{

    struct Mytest test = {
        .a = 10,
        .b = 20,
    };


    return 0;
}