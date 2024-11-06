#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <asm/atomic.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>

struct devices
{
    dev_t dev;
    struct cdev mycdev;
    struct class *myclass;
    int major;
    int minor;
    char kbuff[128];
    unsigned int *gpio_dir;
    unsigned int *gpio_dr;
};

struct devices MyDevices1;
static struct semaphore semlock;

int MyOpen(struct inode *inode, struct file *file)
{
    down(&semlock); // 信号量减一
    // 判断是哪一个 devices
    file->private_data = container_of(inode->i_cdev, struct devices, mycdev);
    printk("open.... \n");
    return 0;
}

ssize_t MyRead(struct file *file, char __user *buf, size_t size, loff_t *offset)
{

    struct devices *readDevices = (struct devices *)file->private_data;

    printk("read.... \n");
    if (size > sizeof(readDevices->kbuff))
    {
        size = sizeof(readDevices->kbuff);
    }

    if (copy_to_user(buf, readDevices->kbuff, size))
    {
        printk("copy data to user fail!\n");
        return -EIO;
    }
    else
    {
        printk("copy data to user success!\n");
        printk("send success data is %s\n", readDevices->kbuff);
    }

    return 0;
}

ssize_t MyWrite(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    printk("write.... \n");

    struct devices *writeDevices = (struct devices *)file->private_data;

    if (size > sizeof(writeDevices->kbuff))
        size = sizeof(writeDevices->kbuff);
    if (copy_from_user(writeDevices->kbuff, buf, size))
    {
        printk("copy data from user fail!\n");
        return -EIO;
    }

    return 0;
}

int MyClose(struct inode *inode, struct file *file)
{
    printk("close.... \n");
    up(&semlock); // 信号量加1
    return 0;
}

struct file_operations myfile_operations = {
    .owner = THIS_MODULE,
    .open = MyOpen,
    .read = MyRead,
    .write = MyWrite,
    .release = MyClose};

// 文件操作

static int test_init(void)
{
    printk("设备号申请\n");

    int ret = -1;

    ret = alloc_chrdev_region(&MyDevices1.dev, 0, 1, "char_alloc");
    if (ret == 0)
        printk("alloc is success\n");
    else
        printk("alloc is error\n");

    // 获取自动分配设备的主设备和此设备
    MyDevices1.major = MAJOR(MyDevices1.dev); // 通过 MAJOR()函数进行主设备号获取
    MyDevices1.minor = MINOR(MyDevices1.dev); // 通过 MINOR()函数进行次设备号获取

    printk("major1 is %d\n", MyDevices1.major);
    printk("minor1 is %d\n", MyDevices1.minor);

    MyDevices1.mycdev.owner = THIS_MODULE;
    // 设备号获取成功之后
    // 字符设备初始化
    cdev_init(&MyDevices1.mycdev, &myfile_operations);
    // 字符设备添加
    ret = cdev_add(&MyDevices1.mycdev, MyDevices1.dev, 1);

    if (ret < 0)
        printk("SEM add is faild...\n");
    printk("SEM add is success...\n");

    // 创建设备节点 （作为桥梁连接上层和底层之间交互）
    MyDevices1.myclass = class_create(THIS_MODULE, "SEM");
    device_create(MyDevices1.myclass, NULL, MyDevices1.dev, NULL, "SEM");

    sema_init(&semlock, 1); // initialize

    return 0;
}

static void test_exit(void)
{
    // 释放设备号

    device_destroy(MyDevices1.myclass, MyDevices1.dev);
    class_destroy(MyDevices1.myclass);

    cdev_del(&MyDevices1.mycdev);
    unregister_chrdev_region(MyDevices1.dev, 1);

    iounmap(MyDevices1.gpio_dr);
    iounmap(MyDevices1.gpio_dir);

    printk("release...\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh");
