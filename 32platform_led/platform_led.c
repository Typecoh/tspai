#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>

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

struct resource *GPIO_DIR;
struct resource *GPIO_DR;
struct resource *GPIO_VALUE;
struct resource *GPIO_MODE;

int MyOpen(struct inode *inode, struct file *file)
{
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
    if (writeDevices->kbuff[0] == 1)
    {
        *(writeDevices->gpio_dir) = GPIO_MODE->end;
        *(writeDevices->gpio_dr)  = GPIO_VALUE->end;

        printk("writeDevices->kbuff[0] is %d\n", writeDevices->kbuff[0]);
    }
    else if (writeDevices->kbuff[0] == 0)
    {
        *(writeDevices->gpio_dir) = GPIO_MODE->end;
        *(writeDevices->gpio_dr) = GPIO_VALUE->start;
        printk("writeDevices->kbuff[0] is %d\n", writeDevices->kbuff[0]);
    }
    return 0;
}

int MyClose(struct inode *inode, struct file *file)
{
    printk("close.... \n");
    return 0;
}

struct file_operations myfile_operations = {
    .owner = THIS_MODULE,
    .open = MyOpen,
    .read = MyRead,
    .write = MyWrite,
    .release = MyClose};

int MyProbe(struct platform_device *platform_device)
{
    // 获取硬件资源
    GPIO_DIR = platform_get_resource(platform_device, IORESOURCE_MEM, 0);
    GPIO_DR = platform_get_resource(platform_device, IORESOURCE_MEM, 1);
    GPIO_VALUE = platform_get_resource(platform_device, IORESOURCE_MEM, 2);
    GPIO_MODE = platform_get_resource(platform_device, IORESOURCE_MEM, 3);

    // 对获取的 硬件资源进行 操作

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
        printk("MyLed add is faild...\n");
    printk("MyLed add is success...\n");

    // 创建设备节点 （作为桥梁连接上层和底层之间交互）
    MyDevices1.myclass = class_create(THIS_MODULE, "MyLed");
    device_create(MyDevices1.myclass, NULL, MyDevices1.dev, NULL, "MyLed");

    // 将物理地址转成虚拟地址
    MyDevices1.gpio_dr  = ioremap(GPIO_DR->start, 4);
    MyDevices1.gpio_dir = ioremap(GPIO_DIR->start, 4);

    return 0;
}

int MyRemove(struct platform_device *platform_device)
{
    printk("platform_device %s is removed\n", platform_device->name);
    return 0;
}

struct platform_driver Myplatform_driver = {
    .probe = MyProbe,
    .remove = MyRemove,
    .driver = {
        .name = "MyPlatform",
        .owner = THIS_MODULE,
    },
};

// 入口函数
static int platform_driver_init(void)
{
    printk("hello platform_driver!\n");
    platform_driver_register(&Myplatform_driver);
    return 0;
}
// 出口函数
static void platform_driver_exit(void)
{
    // 释放设备号
    device_destroy(MyDevices1.myclass, MyDevices1.dev);
    class_destroy(MyDevices1.myclass);

    cdev_del(&MyDevices1.mycdev);
    unregister_chrdev_region(MyDevices1.dev, 1);

    iounmap(MyDevices1.gpio_dr);
    iounmap(MyDevices1.gpio_dir);

    printk("bye platform_driver!\n");
    platform_driver_unregister(&Myplatform_driver);
}
// 加载到 moudle中
module_init(platform_driver_init);
module_exit(platform_driver_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");
