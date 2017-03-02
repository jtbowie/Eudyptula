#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "namelist.h"

static struct identity idlist;
static struct kmem_cache *cache_pool;

static struct identity *identity_find(int id)
{
	struct identity *tmp;

	list_for_each_entry(tmp, &idlist.list, list) {
		if (tmp->id == id)
			return tmp;
	}

	return NULL;
}

static int identity_create(const char *name, int id)
{
	struct identity *newid;

	newid = kmem_cache_alloc(cache_pool, GFP_KERNEL);
	if (!newid)
		return -ENOMEM;

	strncpy((char *)&(newid->name), name, sizeof(newid->name));
	newid->id = id;
	newid->busy = 0;

	list_add(&(newid->list), &idlist.list);

	return 0;
}

static void identity_del(int id)
{
	struct identity *tmp;

	tmp = identity_find(id);
	if (!tmp)
		return;

	list_del(&(tmp->list));
	kmem_cache_free(cache_pool, tmp);
}

int namelist_init(void)
{
	struct identity *tmp;
	int i;
	int ret;

	cache_pool = kmem_cache_create("eudyptula", sizeof(struct identity),
					0, SLAB_RED_ZONE, NULL);

	INIT_LIST_HEAD(&idlist.list);

	ret = identity_create("Alice", 1);
	if (ret < 0)
		pr_debug("Identity creation failed: %d\n", ret);
	ret = identity_create("Bob", 2);
	if (ret < 0)
		pr_debug("Identity creation failed: %d\n", ret);
	ret = identity_create("Dave", 3);
	if (ret < 0)
		pr_debug("Identity creation failed: %d\n", ret);
	ret = identity_create("Gena", 10);
	if (ret < 0)
		pr_debug("Identity creation failed: %d\n", ret);

	for (i = 1; i <= 10; i++) {
		tmp = identity_find(i);
		if (tmp)
			pr_debug("id %d = %s\n", tmp->id, tmp->name);
		else
			pr_debug("id %d not found!\n", i);
	}

	/*
	 * Ideally we should also implement a cleanup function which iterates
	 * through the list with list_for_each_safe() and destroys outstanding
	 * items in the list.  Since we are "self-managing" the list in this
	 * exercise it's unnecessary; but, it is a good idea in general
	 * practice.
	 */

	identity_del(2);
	identity_del(1);
	identity_del(3);
	identity_del(42);
	identity_del(10);

	return 0;
}

void namelist_exit(void)
{
	pr_debug("I hate to see you leave, but...\n");
	kmem_cache_destroy(cache_pool);
}

module_init(namelist_init);
module_exit(namelist_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("1be5456d23a8");
MODULE_DESCRIPTION("First kernel linked list module");
