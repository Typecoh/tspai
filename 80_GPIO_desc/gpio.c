#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/device.h>

#include <linux/gpio/consumer.h>
#include <linux/gpio.h>

struct device_node *device_node;
struct device_node *parent_device_node;
struct device_node *child_node;
struct gpio_desc *mygpio_desc;
int num;

int MyProbe(struct platform_device *platform_device)
{
    printk("this is a probe with devicetree\n");

    printk("name : %s\n",platform_device->name);
    // 获取mygpio_desc
    mygpio_desc = gpiod_get_optional(&platform_device->dev, "my", 0);

    if (mygpio_desc == NULL)
    {
        printk("mygpio_desc get error...\n");
        return -1;
    }

    num = desc_to_gpio(mygpio_desc);
  
    printk("num is %d\n", num);

    // 设置mode 和 value
    gpiod_direction_output(mygpio_desc, 0);
    gpiod_set_value(mygpio_desc, 1);

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
    gpiod_put(mygpio_desc);
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
