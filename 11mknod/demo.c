#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

static int major; // 主设备
static int minor; // 子设备
// 传入主设备和子设备

module_param(major, int, 0664);
module_param(minor, int, 0664);

dev_t dev;
struct cdev mycdev;
struct file_operations myfile_operations = {
    .owner = THIS_MODULE
};

static int dev_t_init(void)
{
    int ret;
    // 如果 传入了 主设备和子设备号
    if (major)
    {
        // 通过 MKDEV(ma,mi) 计算出 设备号
        dev = MKDEV(major, minor);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
        // 函数返回值：申请成功返回 0，申请失败返回负数
        ret = register_chrdev_region(dev, 1, "char_static");
        printk("ret is %d\n", ret);
        if (ret < 0)
            printk("reg is error!\n");
        else
            printk("reg is success!\n");
    }

    // 自动分配
    else
    {
        ret = alloc_chrdev_region(&dev, 0, 1, "char_alloc");
        if (ret < 0)
            printk("alloc is faild....\n");
        else
            printk("alloc is success...\n");
        major = MAJOR(dev); // 通过 MAJOR()函数进行主设备号获取
        minor = MINOR(dev); // 通过 MINOR()函数进行次设备号获取
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
    }

    // 获取到设备号之后,进行注册字符设备
    // 1 初始化 得到mycdev
    cdev_init(&mycdev, &myfile_operations);
    mycdev.owner = THIS_MODULE;
    // 2 添加
    ret = cdev_add(&mycdev, dev, 1);

    if (ret < 0)
    {
        printk("add is faild...\n");
    }
    printk("add is success...\n");
    return 0;
}

static void dev_t_exit(void)
{

    cdev_del(&mycdev);
    unregister_chrdev_region(dev, 1); // 释放字符驱动设备号
    printk("module exit \n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh"); // 作者信息