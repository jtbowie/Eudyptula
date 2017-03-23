#ifndef MOD_DEBUGFS_H
#define MOD_DEBUGFS_H

#define DEBUGFS_DIR     "eudyptula"
#define EUD_ID  "1be5456d23a8\n"
#define EUD_ID_LEN      14

static char my_id[EUD_ID_LEN] = EUD_ID;

int make_endpoints(void);

#endif
