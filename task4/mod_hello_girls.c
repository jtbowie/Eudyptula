#include <linux/module.h> /* We're a module */
#include <linux/kernel.h> /* Alex the Kidd in Kernel-Land */

/* Init our module. Constructor Power! */

int init_module(void)
{
	printk(KERN_DEBUG "Hello World!\n"); /* Let the world know it */

	return(0); /* Insmod will fail without return. */
}

/* All good things must come to an end */

void cleanup_module(void)
{
	printk(KERN_DEBUG "I hate to see you leave, but...\n");
}
