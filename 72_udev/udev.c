#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/configfs.h>
struct kset *kset1;
struct kobject *kobject1;
struct kobj_type kobj_type;
// 入口函数
static int udev_init(void)
{
    printk("hello Mykobject!\n");
    // 创建kset并添加到kernel中
    // 第一个参数表示 这个kset 的 名字 第二个参数设置NULL， 第三个参数 设置 NULL ,表示 创建在 根目录下面
    kset1 = kset_create_and_add("kset1", NULL, NULL);

    // 通过申请内存的方式
    kobject1 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    kobject1->kset = kset1;

    int ret = kobject_init_and_add(kobject1, &kobj_type, NULL, "%s", "kobject1");

    if (ret < 0)
    {
        printk("kobject1 init failed\n");
        return -1;
    }

    ret = kobject_uevent(kobject1, KOBJ_CHANGE);

    return 0;
}
// 出口函数
static void udev_exit(void)
{
    printk("bye Mykobject!\n");
    kobject_put(kobject1);
}
// 加载到 moudle中
module_init(udev_init);
module_exit(udev_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");