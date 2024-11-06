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
struct device_test dev2;

int myopen(struct inode *inode, struct file *file)
{
    dev1.minor = 0;
    dev2.minor = 1;
    file->private_data = container_of(inode->i_cdev, struct device_test, mycdev);
    printk("open....\n");
    return 0;
}

ssize_t myread(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    printk("read...\n");
    struct device_test *test = (struct device_test *)file->private_data;
    if (test->minor == 0)
        printk("this is test1\n");
    if (test->minor == 1)
        printk("this is test2\n");

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

    ret = alloc_chrdev_region(&dev1.dev, 0, 2, "char_alloc");
    if (ret < 0)
        printk("alloc is faild....\n");
    else
        printk("alloc is success...\n");
    dev1.major = MAJOR(dev1.dev); // 通过 MAJOR()函数进行主设备号获取
    dev1.minor = MINOR(dev1.dev); // 通过 MINOR()函数进行次设备号获取
    printk("major is %d\n", dev1.major);
    printk("minor is %d\n", dev1.minor);

    dev1.mycdev.owner = THIS_MODULE;
    // 获取到设备号之后,进行注册字符设备
    // 1 初始化 得到mycdev
    cdev_init(&dev1.mycdev, &myfile_operations);
    // 2 添加
    ret = cdev_add(&dev1.mycdev, dev1.dev, 1);

    if (ret < 0)
        printk("add is faild...\n");
    printk("add is success...\n");

    // 创建设备节点
    // 创建类
    dev1.myclass = class_create(THIS_MODULE, "test1");
    // 创建driver
    device_create(dev1.myclass, NULL, dev1.dev, NULL, "test1");

    /********************** 设备 *************************/
    dev2.major = MAJOR(dev1.dev);
    dev2.minor = MAJOR(dev1.dev + 1);

    printk("major is %d\n", dev2.major);
    printk("minor is %d\n", dev2.minor);

    // 加载到内核中
    dev2.mycdev.owner = THIS_MODULE;
    cdev_init(&dev2.mycdev, &myfile_operations);
    ret = cdev_add(&dev2.mycdev, dev1.dev + 1, 1);

    if (ret < 0)
    {
        printk("add is faild...\n");
    }
    printk("add is success...\n");

    // 创建设备节点
    dev2.myclass = class_create(THIS_MODULE, "test2");
    device_create(dev2.myclass, NULL, dev1.dev + 1, NULL, "test2");

    return 0;
}

static void dev_t_exit(void)
{

    unregister_chrdev_region(dev1.dev, 1);     // 释放字符驱动设备号
    unregister_chrdev_region(dev1.dev + 1, 1); // 释放字符驱动设备号
    cdev_del(&dev1.mycdev);
    cdev_del(&dev2.mycdev);

    // 删除节点
    device_destroy(dev1.myclass, dev1.dev);
    device_destroy(dev2.myclass, dev1.dev + 1);

    class_destroy(dev1.myclass);
    class_destroy(dev2.myclass);
    printk("module exit \n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh"); // 作者信息
