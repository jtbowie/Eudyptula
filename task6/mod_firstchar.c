#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h> /* We're a module */
#include <linux/kernel.h> /* Alex the Kidd in Kernel-Land */
#include <asm/uaccess.h>

#define MY_DEV_NAME "firstchar"
#define MAX_MSG_LEN 0xFF

static int occupied;
static struct miscdevice firstchar_dev;
static char msg[MAX_MSG_LEN];
static char *msgptr;

static int firstchar_open(struct inode *inode, struct file *filp)
{
	static int counter = 0;
	if (occupied > 0)
		return -EBUSY;
	sprintf(msg, "This device has been opened %d times!\n", counter++);
	occupied++;
	msgptr = (char *)&msg;
	try_module_get(THIS_MODULE);
	return 0;
}

static ssize_t firstchar_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	int consumed = 0;

	while (len && *msgptr) {
		put_user(*(msgptr++),buffer++);
		len--;
		consumed++;
	}

	return consumed;
}

static ssize_t firstchar_write(struct file *filp, const char __user *usr, size_t len, loff_t *offset)
{
	printk(KERN_ALERT "Unauthorized write to /dev/%s\n", MY_DEV_NAME);
	return -EINVAL;
}

static int firstchar_release(struct inode *inode, struct file *filp)
{
	occupied--;
	module_put(THIS_MODULE);

	return 0;
}

static struct file_operations fops = {
	.read = firstchar_read,
	.open = firstchar_open,
	.write = firstchar_write,
	.release = firstchar_release
};

static int __init firstchar_init(void)
{
	static int ret;
	
	firstchar_dev.minor = MISC_DYNAMIC_MINOR;
	firstchar_dev.name = MY_DEV_NAME;
	firstchar_dev.fops = &fops;

	ret = misc_register(&firstchar_dev);
	if (ret)
		return ret;

	printk(KERN_DEBUG "Loaded %s successfully!\n", MY_DEV_NAME); /* Let the world know it */
	printk(KERN_DEBUG "%s got minor number %d\n", MY_DEV_NAME, firstchar_dev.minor);

	return(0); /* Insmod will fail without return. */
}

/* All good things must come to an end */

void cleanup_module(void)
{
	printk(KERN_DEBUG "I hate to see you leave, but...\n");
	misc_deregister(&firstchar_dev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonathan Bowie");
MODULE_DESCRIPTION("1be5456d23a8 misc_chrdev");
MODULE_VERSION("0.1");

module_init(firstchar_init);
