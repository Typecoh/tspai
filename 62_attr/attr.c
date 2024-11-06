#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>

struct kobject *kobject;
struct kobj_type kobj_type;

struct Mykobject
{
    struct kobject kobject;
    int value1;
    int value2;
};

struct Mykobject *mykobject;

struct attribute attr1 = {
    .name = "value1",
    .mode = 0666,
};

struct attribute attr2 = {
    .name = "value2",
    .mode = 0666,
};

struct attribute *attrs[] = {
    &attr1,
    &attr2,
    NULL,
};

ssize_t myshow(struct kobject *kobject, struct attribute *attr, char *buff)
{
    ssize_t count = 0;
    struct Mykobject *mykobj = container_of(kobject, struct Mykobject, kobject);

    if (strcmp(attr->name, "value1") == 0)
    {
        count = sprintf(buff, "this value1 is %d\n", mykobj->value1);
    }

    if (strcmp(attr->name, "value2") == 0)
    {
        count = sprintf(buff, "this value2 is %d\n", mykobj->value2);
    }

    return count;
}
ssize_t mystore(struct kobject *kobject, struct attribute *attr, const char *buff, size_t size)
{

    struct Mykobject *mykobj = container_of(kobject, struct Mykobject, kobject);
    if (strcmp(attr->name, "value1") == 0)
    {
        sscanf(buff, "%d\n", &mykobj->value1);
    }

    if (strcmp(attr->name, "value2") == 0)
    {
        sscanf(buff, "%d\n", &mykobj->value2);
    }
    return size;
}

struct sysfs_ops sysfs_ops = {
    .show = myshow,
    .store = mystore,
};

// 入口函数
void kobject_release(struct kobject *kobj)
{
    printk("kobject: (%p): %s\n", kobj, __func__);
    kfree(kobj);
}
struct kobj_type kobj_type = {
    // 这个 release 是一个 函数 指针， 再给函数 指针赋值 的时候是直接把函数名字给他
    .release = kobject_release,
    .default_attrs = attrs,
    .sysfs_ops = &sysfs_ops,
};

static int ktype_init(void)
{

    mykobject = kzalloc(sizeof(struct Mykobject), GFP_KERNEL);
    mykobject->value1 = 1;
    mykobject->value2 = 2;

    int ret = kobject_init_and_add(&mykobject->kobject, &kobj_type, NULL, "%s", "kobject");
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

    kobject_put(&mykobject->kobject);
}
// 加载到 moudle中
module_init(ktype_init);
module_exit(ktype_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");