#include<linux/module.h>
#include<linux/init.h>
// 入口函数
static int helloworld_init(void)
{
    printk("hello world!\n");
    return 0;
}
//出口函数
static void helloworld_exit(void)
{
    printk("hello world bye!\n");
}
// 加载到 moudle中
module_init(helloworld_init);
module_exit(helloworld_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");