#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/kernel.h>
struct kobject *kobject;
struct kobj_type kobj_type;
// 入口函数
void kobject_release(struct kobject *kobj)
{
    printk("kobject: (%p): %s\n", kobj, __func__);
    kfree(kobj);
}
struct kobj_type kobj_type = {
    // 这个 release 是一个 函数 指针， 再给函数 指针赋值 的时候是直接把函数名字给他
    .release = kobject_release,
};
static int ktype_init(void)
{

    kobject = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    int ret = kobject_init_and_add(kobject, &kobj_type, NULL, "%s", "kobject");
    if (ret < 0)
    {
        printk("kobject init failed\n");
        return -1;
    }
    return 0;
}
// 出口函数
static void ktype_exit(void)
{
    printk("goodbye Mykobject!\n");

    kobject_put(kobject);
}
// 加载到 moudle中
module_init(ktype_init);
module_exit(ktype_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");