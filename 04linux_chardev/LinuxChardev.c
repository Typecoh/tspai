#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#define MY_NAME "chardev"
int major = 0;

int myopen(struct inode *inode, struct file *file)
{
    printk("open....\n");
    return 0;
}
ssize_t myread(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    printk("read...\n");
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
