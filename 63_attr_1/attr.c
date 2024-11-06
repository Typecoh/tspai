#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

struct kobject *kobject;
struct kobj_type kobj_type;

struct Mykobject
{
    struct kobject kobject;
    int value1;
};

struct Mykobject *mykobject;

ssize_t myshow1(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    size_t count = 0;

    struct Mykobject *mykobject = container_of(kobj, struct Mykobject, kobject);

    count = sprintf(buf, "the value is %d\n", mykobject->value1);

    return count;
}
ssize_t mystore1(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    struct Mykobject *mykobject = container_of(kobj, struct Mykobject, kobject);

    sscanf(buf, "%d", &mykobject->value1);

    return count;
}

struct kobj_attribute value1 = __ATTR(value1, 0664, myshow1, mystore1);

struct attribute *attrs[] = {
    &value1.attr,
    NULL,
};

ssize_t myshow(struct kobject *kobject, struct attribute *attr, char *buff)
{
    ssize_t count = 0;
    struct kobj_attribute *kobj_attr = container_of(attr, struct kobj_attribute, attr);

    count = kobj_attr->show(kobject, kobj_attr, buff);

    return count;
}

ssize_t mystore(struct kobject *kobject, struct attribute *attr, const char *buff, size_t size)
{

    struct kobj_attribute *kobj_attr = container_of(attr, struct kobj_attribute, attr);
    kobj_attr->store(kobject, kobj_attr, buff, size);
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

static int attr_init(void)
{

    mykobject = kzalloc(sizeof(struct Mykobject), GFP_KERNEL);
    mykobject->value1 = 1;

    int ret = kobject_init_and_add(&mykobject->kobject, &kobj_type, NULL, "%s", "kobject");
    if (ret < 0)
    {
        printk("kobject init failed\n");
        return -1;
    }
    return 0;
}
// 出口函数
static void attr_exit(void)
{
    printk("goodbye Mykobject!\n");

    kobject_put(&mykobject->kobject);
}
// 加载到 moudle中
module_init(attr_init);
module_exit(attr_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");