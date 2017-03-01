#ifndef FIRSTCHAR_H
#define FIRSTCHAR_H

#define MY_DEV_NAME "eudyptula"
#define MAX_ID_LEN 14
#define GET_LEN(x, y)    (x < strlen(y) ? x : strlen(y))

static int occupied;
static struct miscdevice firstchar_dev;
static char my_id[MAX_ID_LEN] = "1be5456d23a8\n";

static int firstchar_open(struct inode *, struct file *);
static ssize_t firstchar_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t firstchar_write(struct file *, const char __user *, size_t,
loff_t *);
static int firstchar_release(struct inode *, struct file *);

#endif