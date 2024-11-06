#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

static int major; // 主设备
static int minor; // 子设备
// 传入主设备和子设备

module_param(major, int, 0664);
module_param(minor, int, 0664);

dev_t dev;

static int dev_t_init(void)
{
    // 如果 传入了 主设备和子设备号
    if (major)
    {
        // 通过 MKDEV(ma,mi) 计算出 设备号
        dev = MKDEV(major, minor);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
        // 函数返回值：申请成功返回 0，申请失败返回负数
        int ret = register_chrdev_region(dev, 1, "char_name");
        printk("ret is %d\n", ret);
        if (ret < 0)
        {
            printk("reg is error!\n");
        }
        else
        {
            printk("reg is success!\n");
        }
    }
    return 0;
}

static void dev_t_exit(void)
{
    unregister_chrdev_region(dev, 1); // 释放字符驱动设备号
    printk("module exit \n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh"); // 作者信息
