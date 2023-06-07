#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Volkvo");

#define BUF_SIZE PAGE_SIZE

#define DIRNAME "seq_dir"
#define FILENAME "single"
#define SYMLINK "seq_ln"
#define FILEPATH DIRNAME

static struct proc_dir_entry *dir = NULL;
static struct proc_dir_entry *afile = NULL;
static struct proc_dir_entry *link = NULL;

static char *cookie_pot;
static int cookie_index;
static int next_fortune;

static char tmp[BUF_SIZE];

ssize_t my_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{	
	printk("+ single: read called\n");
	return seq_read(file, buf, size, ppos);
}

int my_show(struct seq_file *m, void *v)
{
	int len;
	if (!cookie_index)
		return 0;
	seq_printf(m, "%s", cookie_pot);

	printk("+ single: show was called\n");

	return 0;
}

ssize_t my_func(struct file *file, const char __user *buf, size_t len, loff_t *offp)
{
	printk("+ single: write called\n");
	if (len > BUF_SIZE - cookie_index + 1)
	{
		printk(KERN_ERR"+ buffer overflow!\n");
		return -ENOSPC;
	}

	if (copy_from_user(cookie_pot + cookie_index, buf, len))
	{
		printk(KERN_ERR"+ copy_from_user() failed\n");
		return -EFAULT;
	}

	cookie_index += len;
	cookie_pot[cookie_index++] = '\n';
	return len;
}

int my_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO"+ single: open called\n");
	return single_open(file, my_show, NULL);
}

int my_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO"+ single: release called\n");
	return single_release(inode, file);
}


static struct proc_ops fops = {
	.proc_read = my_read,
	.proc_write = my_func,
	.proc_open = my_open,
	.proc_release = my_release
};

static void freemem(void)
{
	if (link)
		remove_proc_entry(SYMLINK, NULL);
	if (afile)
		remove_proc_entry(FILENAME, NULL);
	if (dir)
		remove_proc_entry(DIRNAME, NULL);
	if (cookie_pot)
		vfree(cookie_pot);
}

static int __init mod_init(void)
{
	if (!(cookie_pot = vmalloc(BUF_SIZE)))
	{
		printk(KERN_ERR"+ malloc failed!\n");
		return -1;
	}
	
	memset(cookie_pot, 0, BUF_SIZE);
	if (!(dir = proc_mkdir(DIRNAME, NULL)))
	{
		freemem();
		printk(KERN_ERR"+ mkdir failed!\n");
		return -1;
	}
	if (!(afile = proc_create(FILENAME, 0666, NULL, &fops)))
	{
		freemem();
		printk(KERN_ERR"+ file create failed!\n");
		return - 1;
	}
	if (!(link = proc_symlink(SYMLINK, NULL, FILEPATH)))
	{
		freemem();
		printk(KERN_ERR"+ failed to create symlink!\n");
		return -1;
	}

	cookie_index = 0;
	next_fortune = 0;
	printk(KERN_INFO"+ single: module loaded!\n");
	return 0;
}

static void __exit mod_exit(void)
{
	freemem();
	printk(KERN_INFO"+ single: module unloaded!\n");
}

module_init(mod_init);
module_exit(mod_exit);
