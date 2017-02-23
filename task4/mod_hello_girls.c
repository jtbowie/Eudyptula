#include <linux/module.h>
#include <linux/kernel.h>

/*
 * Init our module. Constructor Power!
 */

int init_module(void)
{
	pr_debug("Hello World!\n");

	return 0;
}

/*
 * All good things must come to an end
 */

void cleanup_module(void)
{
	pr_debug("I hate to see you leave, but...\n");
}
