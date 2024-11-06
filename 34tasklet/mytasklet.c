#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h> // 包含延时函数

int irq;
#define GPIO3_B4 108

#define GPIO_DIR 0xFE760008 // GPIO 的 方向寄存器
#define GPIO_DR 0xFE760000  // GPIO 的 数据寄存器

#define high 0x10001000 // 高电平
#define low 0x10000000  // 低电平

#define output 0x10001000 // 输出模式
#define input 0x10000000  // 输入模式

unsigned int *gpio_dir;
unsigned int *gpio_dr;

struct tasklet_struct mytasklet;
unsigned long data = 100;

void mytasklet_func(unsigned long data)
{
    printk("data is %ld\n", data);
}

irqreturn_t handler(int irq, void *args)
{
    printk("GPIO interrupt\n");
    // 处理中断下文
    tasklet_schedule(&mytasklet);
    return IRQ_HANDLED;
}

// 入口函数
static int mytasklet_init(void)
{

    gpio_dr = ioremap(GPIO_DR, 4);
    gpio_dir = ioremap(GPIO_DIR, 4);

    irq = gpio_to_irq(GPIO3_B4);

    printk("GPIO3_B4 IRQ number: %d\n", irq);
    int ret = request_irq(irq, handler, IRQF_TRIGGER_RISING, "test", NULL);
    if (ret < 0)
    {
        printk("request_irq failed with error: %d\n", ret);
        return ret;
    }
    else
        printk("request_irq : %d\n", ret);
    *(gpio_dir) = input;

    // 初始化 中断下文

    tasklet_init(&mytasklet, mytasklet_func, data);

    return 0;
}

static void mytasklet_exit(void)
{
    free_irq(irq, NULL);
    iounmap(gpio_dr);
    iounmap(gpio_dir);
    // 使能中断下文 count - 1
    tasklet_enable(&mytasklet);
    // 销毁中断下文
    tasklet_kill(&mytasklet);
    printk("bye bye\n");
}
module_init(mytasklet_init);
module_exit(mytasklet_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");