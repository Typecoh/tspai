#include <stdio.h>

int main(int argc, char const *argv[])
{

    // 定义一个 指针数组
    int *p[4];

    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {5, 6, 7, 8};
    int arr3[] = {9, 10, 11, 12};
    int arr4[] = {13, 14, 15, 16};

    p[0] = arr1;
    p[1] = arr2;
    p[2] = arr3;
    p[3] = arr4;

    printf("%d\n", *p[0]);
    printf("%d\n", *p[1]);
    printf("%d\n", *p[2]);
    printf("%d\n", *p[3]);

    // 定义一个数组指针
    // 这个指针是指向 int 类型的数组，并且这个数组是有4个元素

    int (*q)[4];
    q = &arr1;

   
    return 0;
}