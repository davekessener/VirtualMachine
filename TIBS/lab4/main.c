#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

#include "speed.h"

#define MXT_STR_DEVICENAME "tzm"
#define MXT_BUFSIZE 32

#define FALSE 0
#define TRUE (!FALSE)

MODULE_LICENSE("Dual BSD/GPL");

typedef struct
{
	int64_t speed;
	int setup_status, is_open;
	dev_t dev;
	struct semaphore lock;
	struct cdev cdev;
} TZM;

long default_speed = MXT_DEFAULT_SPEED;
TZM tzm;

module_param(default_speed, long, S_IRUGO);

void update_speed(int64_t);
static int initialize(void);
static void cleanup(void);

int tzm_fops_open(struct inode *, struct file *);
int tzm_fops_release(struct inode *, struct file *);
ssize_t tzm_fops_read(struct file *, char *, size_t, loff_t *);
ssize_t tzm_fops_write(struct file *, const char *, size_t, loff_t *);

struct file_operations tzm_fops = {
	.owner   = THIS_MODULE,
	.open    = tzm_fops_open,
	.release = tzm_fops_release,
	.read    = tzm_fops_read,
	.write   = tzm_fops_write
};

// # ---------------------------------------------------------------------------

void TZM_init(TZM *this, int64_t speed)
{
	this->speed = speed;
	this->setup_status = 0;
	this->is_open = FALSE;
	sema_init(&this->lock, 1);
}

void TZM_setup(TZM *this)
{
	++this->setup_status;
}

int TZM_register_device(TZM *this, const char *name)
{
	TZM_setup(&tzm);

	return alloc_chrdev_region(&this->dev, 0, 1, name);
}

int TZM_setup_cdev(TZM *this, struct file_operations *fops)
{
	TZM_setup(&tzm);

	cdev_init(&this->cdev, fops);

	this->cdev.owner = THIS_MODULE;
	this->cdev.ops = fops;

	return cdev_add(&this->cdev, this->dev, 1);
}

// # ---------------------------------------------------------------------------

static int __init initialize(void)
{
	int r = 0;

	printk(KERN_INFO "Loading tzm module with default speed %li ...\n", default_speed);

	tzm_initialize(&update_speed);
	TZM_init(&tzm, default_speed);

	if((r = TZM_register_device(&tzm, MXT_STR_DEVICENAME)))
	{
		printk(KERN_ALERT "tzm CAN'T REGISTER A MAJOR (%i)!\n", r);
	}
	else if((r = TZM_setup_cdev(&tzm, &tzm_fops)))
	{
		printk(KERN_ALERT "tzm CAN'T SETUP CDEV (%i)!\n", r);
	}

	if(r) cleanup(); else TZM_setup(&tzm);

	return r;
}

static void cleanup(void)
{
	printk(KERN_INFO "Unloading tzm module ... Goodbye.\n");

	switch(tzm.setup_status)
	{
		case 3:
			cdev_del(&tzm.cdev);
		case 2:
			unregister_chrdev_region(tzm.dev, 1);
		case 1:
		default:
			;
	}

	tzm_cleanup();
}

// # ---------------------------------------------------------------------------

void update_speed(int64_t v)
{
	tzm.speed = v;
}

int tzm_fops_open(struct inode *n, struct file *f)
{
	int r = 0;

	if(down_interruptible(&tzm.lock))
		return -ERESTARTSYS;

	if(tzm.is_open)
	{
		r = -EBUSY;
	}
	else
	{
		tzm.is_open = TRUE;
	}
 
	up(&tzm.lock);
	
	return r;
}

int tzm_fops_release(struct inode *n, struct file *f)
{
	if(down_interruptible(&tzm.lock))
		return -ERESTARTSYS;
	
 	tzm.is_open = FALSE;
 
	up(&tzm.lock);
	
	return 0;
}

ssize_t tzm_fops_read(struct file *f, char *p, size_t l, loff_t *loff)
{
	if(down_interruptible(&tzm.lock))
		return -ERESTARTSYS;
 
	copy_to_user(p, &tzm.speed, sizeof(tzm.speed));


	up(&tzm.lock);
	
	return l;
}

ssize_t tzm_fops_write(struct file *f, const char *p, size_t l, loff_t *loff)
{
	uint64_t t;
	char buf[MXT_BUFSIZE];
	size_t ll = l;
	int i;

	if(down_interruptible(&tzm.lock))
		return -ERESTARTSYS;
 
 	t = get_jiffies_64();

#define MIN(a,b) ((a)>(b)?(b):(a))
	while(ll)
	{
		size_t lll = MIN(ll, MXT_BUFSIZE);
		copy_from_user(buf, p, lll);

		for(i = 0 ; i < lll ; ++i)
		{
			tzm_pressed(buf[i], t);
		}

		ll -= lll;
	}
#undef MIN

	up(&tzm.lock);

	return l;
}

// # ---------------------------------------------------------------------------

module_init(initialize);
module_exit(cleanup);

