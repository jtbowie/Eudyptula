#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include "firstchar.h"

static ssize_t firstchar_read(struct file *filp, char __user *buffer,
				size_t len, loff_t *offset)
{
	ssize_t ret;

	ret = simple_read_from_buffer(buffer, MAX_ID_LEN, offset, &my_id,
					MAX_ID_LEN);

	if (ret < 0)
		pr_notice("simple_read_from_buffer failed: %ld\n",
			PTR_ERR(&ret));

	return ret;
}

static ssize_t firstchar_write(struct file *filp, const char __user *usr,
				size_t len, loff_t *offset)
{
	char buffer[MAX_ID_LEN];
	char *ptr;
	ssize_t ret;

	memset(&buffer, 0, MAX_ID_LEN);

	ret = simple_write_to_buffer(&buffer, MAX_ID_LEN, offset, usr,
					len);
	if (ret < 0) {
		pr_notice("simple_write_to_buffer_failed: %ld\n",
			PTR_ERR(&ret));
		return ret;
	}

	if (strcmp(buffer, my_id)) {
		ptr = memchr(buffer, '\n', MAX_ID_LEN);
		if (ptr)
			*ptr = '\x00';
		else
			buffer[MAX_ID_LEN-1] = '\x00';

		pr_notice("Got wrong id value %s\n", buffer);
		return -EINVAL;
	}

	return ret;
}

static const struct file_operations fops = {
	.read = firstchar_read,
	.write = firstchar_write,
};

static int __init firstchar_init(void)
{
	static int ret;

	firstchar_dev.minor = MISC_DYNAMIC_MINOR;
	firstchar_dev.name = MY_DEV_NAME;
	firstchar_dev.fops = &fops;
	firstchar_dev.mode = 0664;

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
MODULE_VERSION("1.0");

module_init(firstchar_init);
