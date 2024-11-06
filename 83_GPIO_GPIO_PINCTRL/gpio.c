#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/device.h>

struct device_node *device_node;
struct device_node *parent_device_node;
struct device_node *child_node;
struct gpio_desc *leds[2];
struct fwnode_handle *myfwnode_handle = NULL;
int num;
int count;
struct pinctrl *mypinctrl;
struct pinctrl_state *mystate[2];
struct platform_device *myplatform_device;
static int gpio_state = 0;

ssize_t mystroe(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &gpio_state);
    // mygpio_func1
    if (gpio_state == 0)
    {
        pinctrl_select_state(mypinctrl, mystate[0]);
    }
    // mygpio_func2
    else
    {
        pinctrl_select_state(mypinctrl, mystate[1]);
    }
    return count;
}

ssize_t myshow(struct device *dev, struct device_attribute *attr, char *buf)
{
    size_t count = sprintf(buf, "this is value %d\n", gpio_state);
    return count;
}

struct device_attribute myattr = {

    .attr = {
        .name = "gpio_state",
        .mode = 0664,
    },
    .store = mystroe,
    .show = myshow,
};

int pinctrl_get_and_lookstate(struct device *dev)
{
    mypinctrl = pinctrl_get(dev);

    if (IS_ERR(mypinctrl))
    {
        printk("pinctrl_get is error\n");
        return -1;
    }

    mystate[0] = pinctrl_lookup_state(mypinctrl, "mygpio_func1");
    mystate[1] = pinctrl_lookup_state(mypinctrl, "mygpio_func2");

    if (IS_ERR(mystate[0]))
    {
        printk("mystate[1] is error\n");
        return -2;
    }

    if (IS_ERR(mystate[1]))
    {
        printk("mystate[1] is error\n");
        return -2;
    }
    return 0;
}

int MyProbe(struct platform_device *platform_device)
{
    printk("this is a probe with devicetree\n");

    myplatform_device = platform_device;

    device_create_file(&platform_device->dev, &myattr);

    pinctrl_get_and_lookstate(&platform_device->dev);

    return 0;
}

int MyRemove(struct platform_device *platform_device)
{
    printk("platform_device %s is removed\n", platform_device->name);
    return 0;
}

const struct of_device_id of_match_table_id[] = {
    {.compatible = "mygpios"},
    {},
};

struct platform_driver Myplatform_driver = {
    .probe = MyProbe,
    .remove = MyRemove,
    .driver = {
        .name = "MyPlatform",
        .owner = THIS_MODULE,
        .of_match_table = of_match_table_id,
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
    printk("bye platform_driver!\n");
    // pinctrl_put(mypinctrl);
    device_remove_file(&myplatform_device->dev, &myattr);
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
