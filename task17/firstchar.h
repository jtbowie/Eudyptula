#ifndef FIRSTCHAR_H
#define FIRSTCHAR_H

#define MY_DEV_NAME "eudyptula"
#define MAX_ID_LEN 14

static struct miscdevice firstchar_dev;
static char my_id[MAX_ID_LEN] = "1be5456d23a8\n";

static ssize_t firstchar_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t firstchar_write(struct file *, const char __user *, size_t,
loff_t *);

#endif
