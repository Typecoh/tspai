#include<linux/module.h>
#include<linux/init.h>


// 给led 传递一个 参数
int led = 0;
module_param(led,int,0664);
MODULE_PARM_DESC(led, "这是一个led参数");

//  传递一个 数组参数
int len = 0;
int arr[10] = {0};
module_param_array(arr,int,&len,0664);
MODULE_PARM_DESC(arr, "这是一个数组");



// 入口函数
static int ModulePararm_init(void)
{
    printk("led is %d\n",led);
    int i;
    for(i = 0; i < len; i++){
        printk("arr[%d] is %d\n",i,arr[i]);
    }
    return 0;
}

//出口函数
static void ModulePararm_exit(void)
{
    printk(KERN_ERR "hello printk %s\n", "exit");
}

// 加载到 moudle中
module_init(ModulePararm_init);
module_exit(ModulePararm_exit);

// 许可证
MODULE_LICENSE("GPL");

// 个人信息
MODULE_AUTHOR("Typecoh");
MODULE_VERSION("v1.0.1");



