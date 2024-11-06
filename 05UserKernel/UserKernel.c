#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include<linux/uaccess.h>

#define MY_NAME "chardev"
int major = 0;

char kbuff[128] = {0};

int myopen(struct inode *inode, struct file *file)
{
    printk("open....\n");
    return 0;
}

ssize_t myread(struct file *file, char __user *ubuff, size_t size, loff_t *offset)
{
    // 读数据，肯定是从内核空间拷贝数据到用户空间
    if (size > sizeof(kbuff))
    {
        size = sizeof(kbuff);
    }
    // copy_to_user(buf, kbuff, size) 成功返回 0
    if (copy_to_user(ubuff, kbuff, size))
    {
        printk("copy data to user fail!\n");
        return -EIO;
    }
    else{
        printk("send success data is %s\n", kbuff);
    }
    return size;
}
ssize_t mywrite(struct file *file, const char __user *ubuff, size_t size, loff_t *offset)
{
    if (size > sizeof(kbuff))
    {
        size = sizeof(kbuff);
    }

    if (copy_from_user(kbuff, ubuff, size))
    {
        printk("copy data to kernal fail!\n");
        return -EIO;
    }

    else{
        printk("rev success data is %s\n", kbuff);
    }
    return size;
}
int myclose(struct inode *inode, struct file *file)
{
    printk("close....\n");
    return 0;
}

struct file_operations fops = {
    .open = myopen,
    .read = myread,
    .write = mywrite,
    .release = myclose
};

// 入口函数
static int mydev_init(void)
{
    major = register_chrdev(0, MY_NAME, &fops);
    if (major < 0)
    {
        printk("reg failed....\n");
        return -1;
    }

    printk("reg success...\n");
    return 0;
}

// 出口函数
static void mydev_exit(void)
{
    printk("hello printk %s\n", "exit");
    unregister_chrdev(major, MY_NAME);
}

// 加载到 moudle中
module_init(mydev_init);
module_exit(mydev_exit);

// 许可证
MODULE_LICENSE("GPL");

// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.1");
