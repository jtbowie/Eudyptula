#ifndef MOD_DEBUGFS_H
#define MOD_DEBUGFS_H

#define DEBUGFS_DIR     "eudyptula"                                         
#define EUD_ID  "1be5456d23a8"
#define EUD_ID_LEN      13

static ssize_t foo_show(struct kobject *, struct kobj_attribute *, char *);
static ssize_t foo_store(struct kobject *, struct kobj_attribute *, const char *, size_t);
static ssize_t id_show(struct kobject *, struct kobj_attribute *, char *);
static ssize_t id_store(struct kobject *, struct kobj_attribute *, const char *, size_t);
static ssize_t jiffies_show(struct kobject *, struct kobj_attribute *, char *);

static char my_id[13] = EUD_ID;
static rwlock_t lck;
static char foo_page[PAGE_SIZE+1];
static struct kobject *eudy;
static struct kobj_attribute foo_attr = __ATTR(foo, 0644, foo_show, foo_store);
/*
 * Since VERIFY_OCTAL_PERMISSIONS() fucking hates S_IWOTH we have to define this
 * manually.  GRRR.
 */

static struct kobj_attribute id_attr = {
	.attr = { .name = "id", .mode = 0664, },
	.show = id_show,
	.store = id_store,
};

static struct kobj_attribute jiff_attr = __ATTR_RO(jiffies);

static struct attribute *attrs[] = {
	&id_attr.attr,
	&foo_attr.attr,
	&jiff_attr.attr,
	NULL,
};

static struct attribute_group attr = {
	.attrs = attrs,
};

int make_endpoints(void);
#endif
