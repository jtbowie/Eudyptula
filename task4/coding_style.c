#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/delay.h>
#include <linux/slab.h>

#define DELAY_DUR	10
#define WORK_SEED	10

int do_work(int *my_int)
{
	int x;
	int y = *my_int;
	int z;
	
	for (x = 0; x < y; ++x) {
		udelay(DELAY_DUR);
	}

	if (y < WORK_SEED ) {

		/*
		 * That was a long sleep, tell userspace about it
		 */

		pr_debug("We slept a long time!");
	}

	z = x * y;

	return z;
}

int my_init(void)
{
	int x = WORK_SEED;

	x = do_work(&x);

	return x;
}

void my_exit(void)
{
	return;
}

module_init(my_init);
module_exit(my_exit);
