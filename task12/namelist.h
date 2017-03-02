#ifndef NAMELIST_H
#define NAMELIST_H

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

#endif
