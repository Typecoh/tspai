#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/device.h>

extern struct bus_type mybus_type;

void Myrelease(struct device *dev)
{
    printk("this is release function\n");
}

struct device dev = {
    .init_name = "mydevice",
    .bus = &mybus_type,
    .release = Myrelease,
    .devt = ((255 << 20 | 0)),
};

static int device_init(void)
{
    printk("device init!\n");

    int ret = device_register(&dev);

    if (ret < 0)
    {
        printk("this is error..\n");
    }

    return 0;
}
// 出口函数
static void device_exit(void)
{
    printk("device exit!\n");
    device_unregister(&dev);
}
// 加载到 moudle中
module_init(device_init);
module_exit(device_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");