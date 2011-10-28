#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/delay.h>  
#include <linux/poll.h>  
#include <linux/irq.h>  // #define S3C_EINT(x) ((x) + S3C_IRQ_EINT_BASE)   #define IRQ_EINT(x) S3C_EINT(x)  
#include <asm/irq.h>  
#include <asm/io.h>  
#include <linux/interrupt.h>  
#include <asm/uaccess.h>  
 
#include <linux/platform_device.h>  
#include <linux/cdev.h>  
#include <linux/miscdevice.h>  
 
#define DEVICE_NAME "rs485"  
 
 

 
static int s3c64xx_buttons_open(struct inode *inode, struct file *file)  
{  
	return 0;  
}  
 
static int s3c64xx_buttons_close(struct inode *inode, struct file *file)  
{ 
	return 0;  
}  


static int s3c64xx_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)  
{ 
	return 0;
}  


  
static struct file_operations dev_fops = {  
	.owner   = THIS_MODULE,  
	.open    = s3c64xx_buttons_open,  
	.release = s3c64xx_buttons_close,  
	.read    = s3c64xx_buttons_read,  
 
};  
 
static struct miscdevice misc = { /*�����ַ��豸�ṹ��*/  
	.minor = MISC_DYNAMIC_MINOR,  /*���豸�� ��ʾ�Զ�����*/  
	.name  = DEVICE_NAME,         /*�豸��*/  
	.fops  = &dev_fops,           /*�豸����*/  
};  
 
static int __init dev_init(void)  
{  
	int ret;  
	ret = misc_register(&misc);             /*ע��һ�������豸 �ڼ���ģ��ʱ���Զ������豸�ļ��� Ϊ���豸��Ϊ10���ַ��豸*/  
	printk(DEVICE_NAME"\tinitialized\n");   /*����mknodָ����豸�ļ�����Ϊmisc_register()�����class_device_create()����device_create()*/  
	return ret;  
}  
 
static void __exit dev_exit(void)  
{  
    misc_deregister(&misc);                 /*ע�������豸 �Զ�ɾ���豸�ļ�*/ 
}
module_init(dev_init);  
module_exit(dev_exit);  
MODULE_LICENSE("Daul BSD/GPL");  
MODULE_AUTHOR("LiuShiLi");