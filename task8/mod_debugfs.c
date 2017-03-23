#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include "debugfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("1be5456d23a8");
MODULE_DESCRIPTION("simple debugfs module");

static struct dentry *dent;
static unsigned short success;

static ssize_t id_read(struct file *filp, char __user *buffer, size_t len,
	loff_t *offset)
{
	int ret;

	ret = simple_read_from_buffer(buffer, EUD_ID_LEN, offset, &my_id,
					EUD_ID_LEN);
	if (ret < 0)
		return -EINVAL;

	return ret;
}

static ssize_t id_write(struct file *filp, const char __user *buffer,
	size_t len, loff_t *offset)
{
	char *ptr;
	int ret;
	char input[EUD_ID_LEN];

	memset(&input, 0, EUD_ID_LEN);

	if (len > EUD_ID_LEN) {
		pr_alert("Buffer overflow attempt in eudyptula/id\n");
		return -EINVAL;
	}

	ret = simple_write_to_buffer(&input, EUD_ID_LEN, offset, buffer, len);

	if (strcmp(input, my_id)) {
		ptr = strchr(input, '\n');
		if (ptr)
			*ptr = '\x00';

		pr_notice("debugfs/eudyptula/id got bad input: %s\n", input);
		return -EINVAL;
	}

	return len;
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

int make_endpoints(void)
{
	static struct dentry *tmp;

	tmp = debugfs_create_file("id", 0644, dent, NULL, &id_fops);
	if (!tmp)
		return -EINVAL;
	return 0;
}

int init_module(void)
{
	static int ret;
	char *ptr = (char *)&foo_page;

	memset(ptr, '\x00', PAGE_SIZE+1);
	sprintf(foo_page, "Initializing foo page...\n");

	dent = debugfs_create_dir(DEBUGFS_DIR, NULL);
	if (dent == ERR_PTR(-ENODEV)) {
		pr_warn("Please configure kernel with CONFIG_DEBUGFS\n");
		pr_alert("debugfs_create_dir Failed!\n");
		return -ENODEV;
	}

	if (!dent) {
		pr_alert("debugfs_create_dir Failed!\n");
		return -ENODEV;
	}

	pr_notice("Created debugfs subdir %s\n", DEBUGFS_DIR);
	success = 1;

	ret = make_endpoints();

	rwlock_init(&lck);

	if (ret) {
		pr_alert("Making endpoints failed!\n");
		return ret;
	}

	return 0;
}

void cleanup_module(void)
{
	pr_notice("I hate to see you leave, but...\n");
	if (success)
		debugfs_remove_recursive(dent);
}
