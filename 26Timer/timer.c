#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

static void function_test(struct timer_list *timer_list);

DEFINE_TIMER(MyTimer,function_test);
static void function_test(struct timer_list *timer_list)
{
    printk("This is a timer\n");
    mod_timer(&MyTimer,jiffies_64 + msecs_to_jiffies(5000));
}

// 入口函数
static int timer_init(void)
{
    printk("Timer start...\n");

    MyTimer.expires = jiffies_64 + msecs_to_jiffies(5000);
    add_timer(&MyTimer);
    return 0;
}
// 出口函数
static void timer_exit(void)
{
    del_timer(&MyTimer);  // remove timer
    printk("Timer end...\n");
}
// 加载到 moudle中
module_init(timer_init);

module_exit(timer_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");
