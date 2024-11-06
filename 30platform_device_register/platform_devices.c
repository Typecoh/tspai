#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

// 硬件资源
static struct resource Myresource[] = {
    [0] = {
        .start = 0xFE760008, // GPIO 的 GPIO_DIR 方向寄存器
        .end = 0xFE760008,   // 结束地址与开始地址相同
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 0xFE760000, // GPIO 的 GPIO_DR  数据寄存器
        .end = 0xFE760000,   // GPIO 的 GPIO_DR  数据寄存器
        .flags = IORESOURCE_MEM,
    },
    [2] = {
        .start = 0x10000000,     // low 低电平
        .end = 0x10001000,       // high 高电平
        .flags = IORESOURCE_MEM, // GPIO 方向
    },
    [3] = {
        .start = 0x10000000,     // input 输入模式
        .end = 0x10001000,       // output 输出模式
        .flags = IORESOURCE_MEM, // GPIO 数据
    },
};

void mydevice_release(struct device *dev)
{
    printk("device release\n");
}

struct platform_device Myplatform_device = {
    .name = "MyPlatform",
    .id = -1,
    .resource = Myresource,
    .num_resources = ARRAY_SIZE(Myresource),
    .dev = {.release = mydevice_release}};

// 入口函数
static int
platform_devices_init(void)
{
    printk("hello platform_device!\n");
    platform_device_register(&Myplatform_device);
    return 0;
}
// 出口函数
static void platform_devices_exit(void)
{
    printk("bye platform_device!\n");
    platform_device_unregister(&Myplatform_device);
}
// 加载到 moudle中
module_init(platform_devices_init);
module_exit(platform_devices_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");