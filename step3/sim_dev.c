/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 05/09/19
**/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "sim_dev.h"

// arbitrary number
#define SIM_DEV_MAJOR 444
#define SIM_DEV_NAME "simdev"

// this space holds the data that device users send to the device
static disk_t *disk = NULL;

//Holds logical address
static int logAddr = 0;

// static unsigned long ioctl_control_data;
// static unsigned long ioctl_status_data = 0xAABBCCDD;
DISK_REGISTER sim_dev_register;

// open function - called when the "file" /dev/sim_dev is opened in userspace
static int sim_dev_open (struct inode *inode, struct file *file)
{
   // this is a special print functions that allows a user to print from the kernel
	printk("sim_dev_open\n");
	return 0;
}

// close function - called when the "file" /dev/sim_dev is closed in userspace  
static int sim_dev_release (struct inode *inode, struct file *file)
{
	printk("sim_dev_release\n");
	return 0;
}

// read function called when  /dev/sim_dev is read
static ssize_t sim_dev_read( struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char** buffer = kmalloc(count*SECT_SIZE + 1, GFP_KERNEL);

	read_disk(logAddr, count, buffer);
   	// a special copy function that allows to copy from kernel space to user space
   	if(copy_to_user(buf, buffer, count*SECT_SIZE + 1) != 0)
     	return -EFAULT;
    kfree(buffer);

   	return count;
}

// write function called when /dev/sim_dev is written to
static ssize_t sim_dev_write( struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
   	char* buffer;
    
	if (copy_from_user(buffer, buf, count) != 0)
		return -EFAULT;

	write_disk(logAddr, count, buffer);

	return count;
}

// ioctl function called when /dev/sim_dev receives an ioctl command
// Ubuntu 10.10: static int sim_dev_ioctl(struct inode *inode, struct file *file, unsigned int command, unsigned long arg)
// Ubuntu 11.04:
static long sim_dev_unlocked_ioctl(struct file *file, unsigned int command, unsigned long arg)
{
	switch ( command )
	{
		case IOCTL_SIM_DEV_WRITE:/* for writing data to arg */
			if (copy_from_user(&sim_dev_register, (int *)arg, sizeof(int)))
			   return -EFAULT;
			break;
			
		case IOCTL_SIM_DEV_READ:/* for reading data from arg */
			if (copy_to_user((int *)arg, &sim_dev_register, sizeof(int)))
			   return -EFAULT;
			break;
			
		default:
			return -EINVAL;
	}
	return -EINVAL;
}

//
// mapping of file operations to the driver functions
//

/* NEW VERSION -- OLD VERSION LOWER

struct file_operations {
        struct module *owner;
        loff_t (*llseek) (struct file *, loff_t, int);
        ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
        ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
        ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
        ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
        ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
        ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
        int (*iterate) (struct file *, struct dir_context *);
        unsigned int (*poll) (struct file *, struct poll_table_struct *);
        long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
        long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
        int (*mmap) (struct file *, struct vm_area_struct *);
        void (*mremap)(struct file *, struct vm_area_struct *);
        int (*open) (struct inode *, struct file *);
        int (*flush) (struct file *, fl_owner_t id);
        int (*release) (struct inode *, struct file *);
        int (*fsync) (struct file *, loff_t, loff_t, int datasync);
        int (*aio_fsync) (struct kiocb *, int datasync);
        int (*fasync) (int, struct file *, int);
        int (*lock) (struct file *, int, struct file_lock *);
        ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
        unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
        int (*check_flags)(int);
        int (*flock) (struct file *, int, struct file_lock *);
        ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
        ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
        int (*setlease)(struct file *, long, struct file_lock **, void **);
        long (*fallocate)(struct file *file, int mode, loff_t offset,
                          loff_t len);
        void (*show_fdinfo)(struct seq_file *m, struct file *f);
};

*/
struct file_operations sim_dev_file_operations = {
	.owner	=	THIS_MODULE,
	.llseek	=	NULL,
	.read	=	sim_dev_read,
	.write	=	sim_dev_write,
//	.readdir	=	NULL, // Ubuntu 14.04
	.poll	=	NULL,
//	.ioctl	=	sim_dev_ioctl, // Ubuntu 10.10
	.unlocked_ioctl	=	sim_dev_unlocked_ioctl, // Ubuntu 11.04
	.mmap	=	NULL,
	.open	=	sim_dev_open,
	.flush	=	NULL,
	.release	=	sim_dev_release,
	.fsync	=	NULL,
	.fasync	=	NULL,
	.lock	=	NULL,
};

// Loads a module in the kernel
static int sim_dev_init_module (void)
{
   // here we register sim_dev as a character device
	if (register_chrdev(SIM_DEV_MAJOR, SIM_DEV_NAME, &sim_dev_file_operations) != 0)
	   return -EIO;

	// reserve memory with kmalloc - Allocating Memory in the Kernel
	// GFP_KERNEL --> this does not have to be atomic, so kernel can sleep
	disk = kmalloc(sizeof(disk_t), GFP_KERNEL);
	if (!disk) {
		printk("kmalloc failed\n");
		return -1;
	}
   printk("Simulated Driver Module Installed\n");
   return 0;
}

//  Removes module from kernel
static void sim_dev_cleanup_module(void)
{
   // specialized function to free memeory in kernel
	kfree(disk);
	unregister_chrdev (SIM_DEV_MAJOR, SIM_DEV_NAME);
   printk("Simulated Driver Module Uninstalled\n");
}

// map the module initialization and cleanup functins
module_init(sim_dev_init_module);
module_exit(sim_dev_cleanup_module);

MODULE_AUTHOR("http://www.cs.csuci.edu/~ajbieszczad");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simulated Device Linux Device Driver");

