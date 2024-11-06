#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

struct device_test
{
    int major; // 主设备
    int minor; // 子设备

    dev_t dev;
    struct cdev mycdev;

    struct class *myclass;
    struct device *device;
};
struct device_test dev1;

int myopen(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    printk("open....\n");
    return 0;
}
ssize_t myread(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    printk("read...\n");
    struct device_test *test =  (struct device_test *)file->private_data;

    printk("private_data.minor is %d\n",test->minor);
    return 0;
}
ssize_t mywrite(struct file *file, const char __user *buff, size_t size, loff_t *offset)
{
    printk("write....\n");
    return 0;
}
int myclose(struct inode *inode, struct file *file)
{
    printk("close....\n");
    return 0;
}

struct file_operations myfile_operations = {
    .owner = THIS_MODULE,
    .open = myopen,
    .read = myread,
    .write = mywrite,
    .release = myclose};

static int dev_t_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev1.dev, 0, 1, "char_alloc");
    if (ret < 0)
        printk("alloc is faild....\n");
    else
        printk("alloc is success...\n");
    dev1.major = MAJOR(dev1.dev); // 通过 MAJOR()函数进行主设备号获取
    dev1.minor = MINOR(dev1.dev); // 通过 MINOR()函数进行次设备号获取
    printk("major is %d\n", dev1.major);
    printk("minor is %d\n", dev1.minor);

    // 获取到设备号之后,进行注册字符设备
    // 1 初始化 得到mycdev
    cdev_init(&dev1.mycdev, &myfile_operations);
    dev1.mycdev.owner = THIS_MODULE;
    // 2 添加
    ret = cdev_add(&dev1.mycdev, dev1.dev, 1);

    if (ret < 0)
    {
        printk("add is faild...\n");
    }
    printk("add is success...\n");

    // 创建设备节点
    // 创建类
    dev1.myclass = class_create(THIS_MODULE, "test");
    // 创建driver
    device_create(dev1.myclass, NULL, dev1.dev, NULL, "test");

    return 0;
}

static void dev_t_exit(void)
{

    cdev_del(&dev1.mycdev);
    unregister_chrdev_region(dev1.dev, 1); // 释放字符驱动设备号

    // 删除节点
    device_destroy(dev1.myclass, dev1.dev);
    class_destroy(dev1.myclass);
    printk("module exit \n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh"); // 作者信息