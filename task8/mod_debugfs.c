#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include "debugfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(EUD_ID);
MODULE_DESCRIPTION("simple debugfs module");

static struct dentry *dent;
static unsigned short success = 0;

static ssize_t foo_read(struct file *filp, char __user *buffer, size_t len,
	loff_t *offset)
{
	return 0;
}

static ssize_t foo_write(struct file *filp, const char __user *buffer,
	size_t len, loff_t *offset)
{
	return 0;
}

static ssize_t id_read(struct file *filp, char __user *buffer, size_t len,
	loff_t *offset)
{

	if (*offset != 0)
		return 0;

	if ((len < EUD_ID_LEN) || copy_to_user(buffer, id_ptr, EUD_ID_LEN))
		return -EINVAL;

	*offset += len;
	return len;
}

static ssize_t id_write(struct file *filp, const char __user *buffer, 
	size_t len, loff_t *offset)
{
	char *ptr;
	char input[EUD_ID_LEN];

	if (len > EUD_ID_LEN) {
		pr_alert("Buffer overflow attempt in eudyptula/id\n");
		return -EINVAL;
	}

	copy_from_user(&input, buffer, len);

	ptr = strchr(input, '\n');
	if (ptr)
		*ptr = '\x00';

	if (strcmp(input, my_id))
		return -EINVAL;

	return len;
}

static struct file_operations id_fops = {
        .owner = THIS_MODULE,
        .read = id_read,
        .write = id_write,
};

static struct file_operations foo_ops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

int make_endpoints(void)
{
	static struct dentry *tmp;

	tmp = debugfs_create_file("id", 0666, dent, NULL, &id_fops);
	if (!tmp) 
		return -EINVAL;
	tmp = debugfs_create_u32("jiffies", 0444, dent, (unsigned int *)&jiffies);
	if (!tmp)
		return -EINVAL;
	tmp = debugfs_create_file("foo", 0644, dent, NULL, &foo_ops);
	if (!tmp)
		return -EINVAL;
	return 0;	
}

int init_module(void)
{
	static int ret;

	dent = debugfs_create_dir(DEBUGFS_DIR, NULL);
	if (dent == ERR_PTR(-ENODEV)) {
		pr_warn("Please configure kernel with CONFIG_DEBUGFS\n");
		pr_alert("debugfs_create_dir Failed!\n");
		return -ENODEV;
	} 

	if (!dent) {
		pr_alert("debugfs_create_dir Failed!\n");
		return -ENODEV;
	} else {
		pr_notice("Created debugfs subdir %s\n", DEBUGFS_DIR);
		success = 1;
	}

	ret = make_endpoints();

	if (ret) {
		pr_alert("Making endpoints failed!\n");
		return ret;
	}

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_DEBUG "I hate to see you leave, but...\n");
	if (success) 
		debugfs_remove_recursive(dent);
}
