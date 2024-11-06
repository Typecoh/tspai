#include<linux/module.h>
#include<linux/init.h>

// 入口函数
static int __init printk_init(void)
{
    // printk(消息等级， "格式化字符串"，可变传参)；
    // 消息等级 0 - 7 数值越小优先级越高
    // 输出执行printk的条件是： 当printk的优先级高于 内核终端的优先级时会被打印
    printk("hello printk  %s\n", "init");
    return 0;
}

//出口函数
static void __exit printk_exit(void)
{
    printk("hello printk %s\n", "exit");
}

// 加载到 moudle中
module_init(printk_init);
module_exit(printk_exit);

// 许可证
MODULE_LICENSE("GPL");

// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.1");
