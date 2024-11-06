#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/sysfs.h>

int value = 1;

int mymatch(struct device *dev, struct device_driver *drv)
{
    return strcmp(dev_name(dev), drv->name);
}

int myproe(struct device *dev)
{
    struct device_driver *driver = dev->driver;
    if (driver->probe)
    {
        driver->probe(dev);
    }
    return 0;
}

ssize_t Myshow(struct bus_type *bus, char *buf)
{
    ssize_t count = 0;
    count = sprintf(buf, "this is bus_type %d", value);
    return count;
}

struct bus_attribute mybus_attr = {
    .attr = {
        .name = "value1",
        .mode = 0664,
    },
    .show = Myshow,
};

struct bus_type mybus_type = {
    .name = "mybus",
    .match = mymatch,
    .probe = myproe,
};

static int bus_init(void)
{
    printk("bus init!\n");

    int ret = bus_register(&mybus_type);

    if (ret < 0)
    {
        printk("error...\n");
        return -1;
    }

    // 创建属性文件
    ret = bus_create_file(&mybus_type, &mybus_attr);

    if (ret < 0)
    {
        printk("error...\n");
        return -1;
    }

    return 0;
}
// 出口函数
static void bus_exit(void)
{
    printk("bus exit!\n");

    bus_remove_file(&mybus_type, &mybus_attr);

    bus_unregister(&mybus_type);
}
// 加载到 moudle中
module_init(bus_init);
module_exit(bus_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");