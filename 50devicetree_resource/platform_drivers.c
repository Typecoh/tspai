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
const char *out_string;
int MyProbe(struct platform_device *platform_device)
{

    device_node = of_find_node_by_name(NULL, "myled");

    struct resource *MyResource = platform_get_resource(platform_device, IORESOURCE_MEM, 0);
    if(MyResource == NULL){
        printk("resource is NULL\n");
        return -1;
    }
    
    printk("memory address: 0x%08llx\n", MyResource->start);

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
