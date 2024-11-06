#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

extern int add(int a,int b);

static int dev_t_init(void)
{

    int sum = add(1,2);
    printk("hello b sum is %d!\n",sum);
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
