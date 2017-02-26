#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include "sysfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(EUD_ID);
MODULE_DESCRIPTION("simple debugfs module");

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *kattr, char *buf)
{
	read_lock(&lck);
	memcpy(buf, (void *)&foo_page, strlen(foo_page));
	read_unlock(&lck);

	return strlen(buf);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *kattr, const char *buf, size_t cnt)
{
	if (cnt > PAGE_SIZE)
		return -EINVAL;
	
	memcpy((void *)&foo_page, buf, cnt);

	return cnt;
}

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *kattr, char *buf)
{
	sprintf(buf, "%s\n", my_id);

	return strlen(my_id);
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *kattr, const char *buf, size_t cnt) 
{
	char *ptr;

	ptr = strchr(buf, '\n');

	if (ptr) 
		*ptr = '\x00';
	
	if (!strcmp(buf, my_id))
		return cnt;
	else
		return -EINVAL;
}

ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *kattr, char *buf)
{
	sprintf(buf,"%ld\n",jiffies);
	return sizeof(jiffies);
}

int init_module(void)
{
	static int ret;

	eudy = kobject_create_and_add("eudyptula", kernel_kobj);

	if (sysfs_create_group(eudy, &attr))
		return -ENOMEM;

	sprintf(foo_page, "Initializing foo page...\n");

	rwlock_init(&lck);

	if (ret) {
		pr_alert("Making endpoints failed!\n");
		return ret;
	}

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_DEBUG "I hate to see you leave, but...\n");
	kobject_put(eudy);
}
