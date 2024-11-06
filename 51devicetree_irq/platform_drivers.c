#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/gpio.h>

int irq;
u32 irq_type;
struct irq_data *my_irq_data;

struct device_node *device_node;
int MyProbe(struct platform_device *platform_device)
{

    device_node = of_find_node_by_name(NULL, "myirq");

    printk("device_node is %s", device_node->name);

    irq = irq_of_parse_and_map(device_node, 0);

    printk("irq number is %d\n", irq);

    // 获取 中断 标志
    my_irq_data = irq_get_irq_data(irq);
    if (my_irq_data == NULL)
    {
        printk("irq_get_irq_data failed\n");
        return -1;
    }

    irq_type = irqd_get_trigger_type(my_irq_data);

    printk("irq trigger type is %d\n", irq_type);
    // GPIO0_A3
    irq = gpio_to_irq(3);
    printk("irq is %d\n", irq);

    // // 从设备节点获取中断号
    irq = of_irq_get(device_node, 0);
    printk("irq is %d\n", irq);

    // // 获取平台设备的中断号
    irq = platform_get_irq(platform_device, 0);
    printk("irq is %d\n", irq);

    return 0;
}

int MyRemove(struct platform_device *platform_device)
{
    printk("platform_device %s is removed\n", platform_device->name);
    return 0;
}

const struct of_device_id of_match_table_id[] = {
    {.compatible = "my_myirq"},
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
