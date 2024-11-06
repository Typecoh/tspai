#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

struct file_operations misc_fops = {
    .owner = THIS_MODULE};

struct miscdevice misc_dev =
    {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "test",
        .fops = &misc_fops};

static int
misc_init(void) // 驱动入口函数
{
    int ret;
    // 注册杂项设备
    ret = misc_register(&misc_dev);

    if (ret < 0)
        printk("misc registe is error \n");

    printk("misc registe is succeed \n");

    return 0;
}
static void misc_exit(void) // 驱动出口函数
{
    misc_deregister(&misc_dev);
    printk("misc exit \n");
}
module_init(misc_init);
module_exit(misc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh");