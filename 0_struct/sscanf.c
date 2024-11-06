#include <stdio.h>

int main()
{

    char str[] = "this is age 12";
    char messgae[32] = {0};
    int age = 0;
    sscanf(str, "this is %s %d", messgae , &age);

    printf("Age: %d\n", age);
    printf("message is %s\n",messgae);

    sprintf(str, "this is New %d", age);

    printf("this is str : %s\n", str);
    
    return 0;

}