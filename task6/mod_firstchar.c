#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include "firstchar.h"

#define MY_DEV_NAME "eudyptula"
#define MAX_OUT_LEN 0xFF

static int occupied;
static struct miscdevice firstchar_dev;
static char output[MAX_OUT_LEN];

static int firstchar_open(struct inode *inode, struct file *filp)
{
	static int counter;

	if (occupied > 0)
		return -EBUSY;

	sprintf(output, "This device has been opened %d times!\n", counter++);
	occupied++;

	return 0;
}

static ssize_t firstchar_read(struct file *filp, char __user *buffer,
				size_t len, loff_t *offset)
{

/*
 * Never write more than strlen(output) bytes.  If len < strlen(output)
 * write only len bytes.
 */
	size_t my_len = len < strlen(output) ? len : strlen(output);

	if (*offset > 0)
		return 0;

	if (copy_to_user(buffer, (char *)&output, my_len))
		return -EINVAL;

	*offset += my_len;

	return my_len;
}

static ssize_t firstchar_write(struct file *filp, const char __user *usr,
				size_t len, loff_t *offset)
{
	pr_alert("Unauthorized write to /dev/%s\n", MY_DEV_NAME);
	return -EINVAL;
}

static int firstchar_release(struct inode *inode, struct file *filp)
{
	occupied--;

	return 0;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
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

	pr_notice("Loaded %s successfully!\n", MY_DEV_NAME);
	pr_notice("%s got minor number %d\n", MY_DEV_NAME,
		firstchar_dev.minor);

	return 0;
}

/* All good things must come to an end */

void cleanup_module(void)
{
	pr_notice("I hate to see you leave, but...\n");
	misc_deregister(&firstchar_dev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonathan Bowie");
MODULE_DESCRIPTION("1be5456d23a8 misc_chrdev");
MODULE_VERSION("0.1");

module_init(firstchar_init);
