#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
	int32_t ret;

	ret = syscall(332, 0x1be5, 0x456d23a8);

	if (ret) {
		printf("Got error: %d :(\n", ret);
		return ret;
	}

	printf("You chose wisely.\n");

	ret = syscall(332, 0x1be4, 0x456d23a8);

	if (!ret)
		printf("The fucking thing is broken.\n");
	else
		printf("None shall pass without a shubbery.\n");

	return 0;
}
