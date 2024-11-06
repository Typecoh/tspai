#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>
#include <linux/pinctrl/pinctrl.h>

struct device_node *device_node;
struct device_node *parent_device_node;
struct device_node *child_node;
struct gpio_desc *leds[2];
struct fwnode_handle *myfwnode_handle = NULL;
int num;
int count;
struct pinctrl *mypinctrl;
struct pinctrl_state *mystate;

int MyProbe(struct platform_device *platform_device)
{
    printk("this is a probe with devicetree\n");

    printk("name is %s\n",platform_device->name);

    printk("name is %s\n",platform_device->dev.init_name);
    
    mypinctrl = pinctrl_get(&platform_device->dev);

    if (IS_ERR(mypinctrl))
    {
        printk("pinctrl_get is error\n");
        return -1;
    }

    mystate = pinctrl_lookup_state(mypinctrl, "myled1");

    if (IS_ERR(mystate))
    {
        printk("pinctrl_lookup_state is error\n");
        return -2;
    }

    int ret = pinctrl_select_state(mypinctrl, mystate);

    if (ret < 0)
    {
        printk("pinctrl_select_state error...\n");
    }

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
    pinctrl_put(mypinctrl);
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
