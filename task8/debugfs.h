#ifndef MOD_DEBUGFS_H
#define MOD_DEBUGFS_H

#define DEBUGFS_DIR     "eudyptula"                                         
#define EUD_ID  "1be5456d23a8"
#define EUD_ID_LEN      13

static char my_id[13] = EUD_ID;
static char *id_ptr = (char *)&my_id;
int make_endpoints(void);
#endif
