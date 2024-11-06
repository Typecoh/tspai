#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

// 声明一个函数
extern int add(int a, int b)
{
    return a + b;
}

EXPORT_SYMBOL(add);

static int dev_t_init(void)
{

    printk("hello a!\n");
    return 0;
}

static void dev_t_exit(void)
{

    printk("module exit \n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh"); // 作者信息
