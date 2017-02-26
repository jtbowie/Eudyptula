#ifndef MOD_DEBUGFS_H
#define MOD_DEBUGFS_H

#define DEBUGFS_DIR     "eudyptula"                                         
#define EUD_ID  "1be5456d23a8"
#define EUD_ID_LEN      13

static char my_id[13] = EUD_ID;
static char *id_ptr = (char *)&my_id;
static rwlock_t lck;
static char foo_page[PAGE_SIZE+1];
static kobject *sysfs_kobj;
static kobj_attribute foo_attr;
static kobj_attribute id_attr;
static kobj_attribute jiff_attr;

static struct attributes *attrs[] = {
	&foo_attr.attr,
	&id_attr.attr,
	&jiff_attr.attr,
	NULL,
};

int make_endpoints(void);
#endif
