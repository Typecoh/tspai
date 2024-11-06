#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>

struct device_node *device_node;
struct device_node *parent_device_node;
struct device_node *child_node;
struct property *myproperty;
int size;
int num;
u32 u32_vlaue;
u64 u64_vlaue;
u64 out_values[2];

int MyProbe(struct platform_device *platform_device)
{

    device_node = of_find_node_by_name(NULL, "myled");
    printk("device_node name is %s\n", device_node->name);

    // myproperty = of_find_property(device_node, "compatible", &size);
    // printk("size is %d\n", size);
    // printk("name is %s\n", myproperty->name);

    // num = of_property_count_elems_of_size(device_node, "reg", 4);
    // printk("num is %d\n", num);

    // of_property_read_u64_index(device_node, "reg", 0, &u64_vlaue);
    // printk("u64_vlaue = ox%llx\n", u64_vlaue);
    // of_property_read_u64_index(device_node, "reg", 1, &u64_vlaue);
    // printk("u64_vlaue = ox%llx\n", u64_vlaue);

    of_property_read_variable_u64_array(device_node, "reg", out_values, 1, 2);

    for(int i = 0; i < sizeof(out_values) / sizeof(out_values[0]); i++){
        printk("out_values[%d] = ox%llx\n", i, out_values[i]);
    }

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
