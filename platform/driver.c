#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
//#include <mach/regs-gpio.h>
//#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sched.h>



#define DEVICE_NAME "pf_key_dev"





static irqreturn_t buttons_interrupt(int irq, void* dev_id)
{

	return 0;
}

static int s3c24xx_buttons_open(struct inode* inode, struct file* file)
{

	printk("open device PF_KEY_DEV succeed\n");
	return 0;
}

static int s3c24xx_buttons_close(struct inode* inode, struct file* file)
{

	return 0;
}

static int s3c24xx_buttons_read(struct file* filp, char __user *buff, size_t count, loff_t *offset)
{
	return 0;
}



static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = s3c24xx_buttons_open,
	.release = s3c24xx_buttons_close,
	.read = s3c24xx_buttons_read,

};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int my_probe(struct device* dev)
{
	int ret;
	ret = misc_register(&misc);
	 
	printk("driver found device which my driver can handle!/n");
	return 0;
}

static int my_remove(struct device* dev)
{
	misc_deregister(&misc);
	printk("drvier found device unpluged!/n");
}

static struct platform_driver my_driver = {
	.probe = my_probe,
	.remove = my_remove,
	.driver = {
	.owner = THIS_MODULE,
	.name = "pf_key_dev",
	},
};

static int __init my_driver_init(void)
{
	return platform_driver_register(&my_driver);
}

static void my_driver_exit(void)
{
	platform_driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_AUTHOR("LiuShiLi");
MODULE_LICENSE("Dual BSD/GPL");