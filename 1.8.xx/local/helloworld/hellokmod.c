#include <linux/init.h>
#include <linux/module.h>

#include "helloworld.h"

static int hello_init(void)
{
    printk(KERN_ALERT "init,%s\n", HELLO_WORLD);
    return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
        printk(KERN_ALERT "exit,%s\n", HELLO_WORLD);
}
module_exit(hello_exit);

