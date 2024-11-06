#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

struct kobject *kobject;
int va1;
int va2;

ssize_t myshow1(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{

    ssize_t count;

    count = sprintf(buf, "this is show1 is %d\n", va1);

    return count;
}

ssize_t mystore1(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &va1);

    return count;
}

ssize_t myshow2(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{

    ssize_t count;

    count = sprintf(buf, "this is show2 is %d\n", va2);

    return count;
}

ssize_t mystore2(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &va2);

    return count;
}

struct kobj_attribute value1 = __ATTR(value1, 0664, myshow1, mystore1);
struct kobj_attribute value2 = __ATTR(value2, 0664, myshow2, mystore2);

struct attribute *attrss[] = {
    &value1.attr,
    &value2.attr,
    NULL,
};

const struct attribute_group attrs = {
    .attrs = attrss,
};

static int attr_init(void)
{
    kobject = kobject_create_and_add("mykobject", NULL);
    va1 = 1;
    va2 = 2;
    int ret = sysfs_create_group(kobject, &attrs);

    if (ret < 0)
    {
        printk("error...");
    }

    return 0;
}
// 出口函数
static void attr_exit(void)
{
    printk("goodbye Mykobject!\n");

    kobject_put(kobject);
}
// 加载到 moudle中
module_init(attr_init);
module_exit(attr_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");