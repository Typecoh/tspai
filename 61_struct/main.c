#include <stdio.h>
struct Mytest {
    int a;
    int b;
    int (*add)(int a,int b);
};

int add(int a,int b) {
    return a + b;
}

int main()
{

    struct Mytest t;
    t.a = 10;
    t.b = 20;

    t.add = add;

    int result = t.add(t.a, t.b);

    printf("a = %d, b = %d\n", t.a, t.b);
    printf("result = %d\n", result);


    return 0;
}