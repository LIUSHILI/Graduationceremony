#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/platform_device.h>


static struct platform_device *my_device;

static int __init my_device_init(void)
{
	int ret = 0;
	 
	my_device = platform_device_alloc("pf_key_dev", -1);

	ret =  platform_device_add(my_device);

	if(ret)
		platform_device_put(my_device);

	return ret;
}

static void my_device_exit(void)
{
	platform_device_unregister(my_device);
}

module_init(my_device_init);
module_exit(my_device_exit);

MODULE_AUTHOR("LiuShiLi");
MODULE_LICENSE("Dual BSD/GPL");

