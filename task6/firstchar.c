#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include "firstchar.h"

static int firstchar_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t firstchar_read(struct file *filp, char __user *buffer,
				size_t len, loff_t *offset)
{
	return simple_read_from_buffer(buffer, MAX_ID_LEN, offset, &my_id,
					MAX_ID_LEN);
}

static ssize_t firstchar_write(struct file *filp, const char __user *usr,
				size_t len, loff_t *offset)
{
	char buffer[MAX_ID_LEN];
	char *ptr;

	memset(&buffer, 0, MAX_ID_LEN);

	if (len > MAX_ID_LEN) {
		pr_notice("Buffer overflow attempted, len = %ld\n", len);
		return -EINVAL;
	}

	if (copy_from_user(&buffer, usr, len)) {
		pr_notice("Copy from user failed.\n");
		return -EINVAL;
	}

	if (strcmp(buffer, my_id)) {
		ptr = strchr(buffer, '\n');
		if (ptr)
			*ptr = '\x00';

		pr_notice("Got wrong id value %s\n", buffer);
		return -EINVAL;
	}

	return len;
}

static int firstchar_release(struct inode *inode, struct file *filp)
{
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
