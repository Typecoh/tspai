#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>

struct device_node *device_node;
struct device_node *parent_device_node;
struct device_node *child_node;
const struct of_device_id mynode_of_match[] = {
    {.compatible = "my_devicetree"},
    {}};

const struct of_device_id * of_devices_ids;

int MyProbe(struct platform_device *platform_device)
{
    // printk("this is a probe with devicetree\n");

    // device_node = of_find_node_by_name(NULL, "myled");
    // printk("device_node name is %s\n", device_node->name);

    // device_node = of_find_node_by_path("/typecoh/myled");
    // printk("device_node name is %s\n", device_node->name);

    // parent_device_node = of_get_parent(device_node);
    // printk("parent_device_node name is %s\n", parent_device_node->name);

    // child_node = of_get_next_child(parent_device_node, NULL);
    // printk("child_node name is %s\n", child_node->name);
    // while (child_node)
    // {
    //     child_node = of_get_next_child(parent_device_node, child_node);

    //     if(child_node == NULL){
    //         break;
    //     }
    //     printk("child_node name is %s\n", child_node->name);
    // }

    // device_node = of_find_compatible_node(NULL, NULL, "my_devicetree");
    // printk("device_node name is %s\n", device_node->name);

    device_node = of_find_matching_node_and_match(NULL, mynode_of_match, &of_devices_ids);

    printk("device_node name is %s\n", device_node->name);

    return 0;
}

int MyRemove(struct platform_device *platform_device)
{
    printk("platform_device %s is removed\n", platform_device->name);
    return 0;
}

const struct of_device_id of_match_table_id[] = {
    {.compatible = "my_devicetree"},
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
