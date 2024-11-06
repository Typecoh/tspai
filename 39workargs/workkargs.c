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

struct work_data
{
    struct work_struct selfwork;
    int a;
    int b;
};

struct work_data my_work_data;

struct workqueue_struct *selfworkqueue;

void workfunction(struct work_struct *work)
{
    msleep(1000);
    struct work_data *data = container_of(work, struct work_data, selfwork);
    printk("Workqueue action: %d + %d = %d\n", data->a, data->b, data->a + data->b);
}

irqreturn_t handler(int irq, void *args)
{
    printk("GPIO interrupt\n");
    queue_work(selfworkqueue, &my_work_data.selfwork); // 加入工作队列
    return IRQ_HANDLED;
}

// 入口函数
static int workkargs_init(void)
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

    // 手动创建工作队列
    selfworkqueue = create_workqueue("selfworkqueue");
    // 初始化工作队列
    INIT_WORK(&my_work_data.selfwork, workfunction);

    my_work_data.a = 10;
    my_work_data.b = 10;

    return 0;
}

static void workkargs_exit(void)
{
    cancel_work_sync(&my_work_data.selfwork);      // 取消工作
    flush_workqueue(selfworkqueue);   // 通知工作队列赶快完成剩下工作
    destroy_workqueue(selfworkqueue); // 销毁工作队列
    free_irq(irq, NULL);
    iounmap(gpio_dr);
    iounmap(gpio_dir);
    printk("bye bye\n");
}
module_init(workkargs_init);
module_exit(workkargs_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");