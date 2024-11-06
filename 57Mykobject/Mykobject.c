#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/configfs.h>
struct kobject *kobject1;
struct kobject *kobject2;
struct kobject *kobject3;
struct kobj_type kobj_type;
// 入口函数
static int helloworld_init(void)
{
    printk("hello Mykobject!\n");
    // 创建kobject并添加到kernel中 
    // 第一个参数表示 这个kobject 的 名字 第二个参数表示 放在父目录
    kobject1 = kobject_create_and_add("kobject1", NULL);

    // 创建子kobject并添加到 kobject1中
    kobject2 = kobject_create_and_add("kobject2", kobject1);

    // 通过申请内存的方式 
    kobject3 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    int ret = kobject_init_and_add(kobject3, &kobj_type, NULL, "%s", "kobject3");

    if(ret < 0){
        printk("kobject3 init failed\n");
        return -1;
    }

    return 0;
}
// 出口函数
static void helloworld_exit(void)
{
    printk("bye Mykobject!\n");
    kobject_put(kobject1);
    kobject_put(kobject2);
    kobject_put(kobject3);
}
// 加载到 moudle中
module_init(helloworld_init);
module_exit(helloworld_exit);
// 许可证
MODULE_LICENSE("GPL");
// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.0");