#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/configfs.h>

struct kset *kset1;
struct kobject *kobject1;
struct kobject *kobject2;
struct kobj_type kobj_type;

int myfilter(struct kset *kset, struct kobject *kobj)
{
    if (strcmp(kobj->name, "kobject1") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int myevent(struct kset *kset, struct kobject *kobj, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "MYDEVICE=%s", "TYPECOH");
    return 0;
}

const char *myname(struct kset *kset, struct kobject *kobj)
{
    return "Typecoh";
}

const struct kset_uevent_ops uevent_ops = {
    .name = myname,
    .filter = myfilter,
    .uevent = myevent,
};

// 入口函数
static int udev_init(void)
{
    printk("hello Mykobject!\n");
    // 创建kset并添加到kernel中
    // 第一个参数表示 这个kset 的 名字 第二个参数设置NULL， 第三个参数 设置 NULL ,表示 创建在 根目录下面
    kset1 = kset_create_and_add("kset1", &uevent_ops, NULL);

    // 通过申请内存的方式
    kobject1 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    kobject1->kset = kset1;

    int ret = kobject_init_and_add(kobject1, &kobj_type, NULL, "%s", "kobject1");

    // 通过申请内存的方式
    kobject2 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    kobject2->kset = kset1;

    ret = kobject_init_and_add(kobject2, &kobj_type, NULL, "%s", "kobject2");

    ret = kobject_uevent(kobject1, KOBJ_CHANGE);
    ret = kobject_uevent(kobject2, KOBJ_ADD);

    if (ret < 0)
    {
        printk("kobject1 init failed\n");
        return -1;
    }

    return 0;
}
// 出口函数
static void udev_exit(void)
{
    printk("bye Mykobject!\n");
    kobject_put(kobject1);
    kobject_put(kobject2);
    kset_unregister(kset1);
}
// 加载到 moudle中
module_init(udev_init);
module_exit(udev_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");