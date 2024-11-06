#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/timer.h>

#define LED_ON_FUNC   _IOW('L', 0, int)
#define LED_OFF_FUNC  _IOW('L', 1, int)

struct devices
{
    dev_t dev;
    struct cdev mycdev;
    struct class *myclass;
    int major;
    int minor;
    char kbuff[128];
    int sec;
};

struct devices MyDevices1;


int MyOpen(struct inode *inode, struct file *file)
{
    // 判断是哪一个 devices
    file->private_data = container_of(inode->i_cdev, struct devices, mycdev);
    return 0;
}

ssize_t MyRead(struct file *file, char __user *buf, size_t size, loff_t *offset)
{

    struct devices *readDevices = (struct devices *)file->private_data;

    if (size > sizeof(readDevices->kbuff))
    {
        size = sizeof(readDevices->kbuff);
    }

    if (copy_to_user(buf, &readDevices->sec, sizeof(readDevices->sec)))
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
    return 0;
}

long MyIoctl (struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd){
        case LED_ON_FUNC:
            printk("LED_OPEN\n");    
            break;
        case LED_OFF_FUNC:
            printk("LED_CLOSE\n"); 
            break;
    }
    return 0;
}

struct file_operations myfile_operations = {
    .owner = THIS_MODULE,
    .open = MyOpen,
    .read = MyRead,
    .write = MyWrite,
    .release = MyClose,
    .unlocked_ioctl = MyIoctl};

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
        printk("ICOT add is faild...\n");
    printk("ICOT add is success...\n");

    // 创建设备节点 （作为桥梁连接上层和底层之间交互）
    MyDevices1.myclass = class_create(THIS_MODULE, "ICOT");
    device_create(MyDevices1.myclass, NULL, MyDevices1.dev, NULL, "ICOT");

    return 0;
}

static void test_exit(void)
{
    // 释放设备号

    device_destroy(MyDevices1.myclass, MyDevices1.dev);
    class_destroy(MyDevices1.myclass);

    cdev_del(&MyDevices1.mycdev);
    unregister_chrdev_region(MyDevices1.dev, 1);
    printk("release...\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh");
