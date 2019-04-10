#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#define DEVSIZE 512
MODULE_LICENSE("GPL");
unsigned int test_major=0;
unsigned char buffer[1024];
static loff_t my_llseek(struct file *filp, loff_t offset, int orig);
static ssize_t my_open(struct inode *inode,struct file *file);
static ssize_t my_write(struct file *filp,const char __user *buf,size_t size,loff_t *pos);
static ssize_t my_release(struct inode *inode,struct file *file);
static ssize_t my_read(struct file *filp,char __user *buf,size_t size,loff_t *pos);
static struct file_operations mydev_fops={
	owner:THIS_MODULE,
	read:my_read,
	write:my_write,
	open:my_open,
    release:my_release,
    llseek:my_llseek
};
int init_module(void)
{
	int result;
	result=register_chrdev(0,"wangsen_dev",&mydev_fops);
	if(result<0) return result;
	if(test_major==0) test_major=result;
	printk("init mydev succeed!\n");
	return 0;
}
void cleanup_module(void)
{
	unregister_chrdev(test_major,"wangsen_dev");
	printk("unloading my device!\n");
}
static ssize_t my_open(struct inode *inode,struct file *file)
{
	printk("open success!\n");
	try_module_get(THIS_MODULE);
	return 0;
}
static ssize_t my_release(struct inode *inode,struct file *file)
{
	printk("device released\n");
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t my_write(struct file *filp,const char __user *buf,size_t size,loff_t *pos)
{
	int s = size;
	if(size>1024){
		printk("size is too big,only 1024 characters can be written!\n");
	}
	if(copy_from_user(buffer+filp->f_pos, buf,sizeof(buffer))) return -1;
	return s;
}

static ssize_t my_read(struct file *filp,char __user *buf,size_t size,loff_t *pos)
{
	int s=size;
	if(s>1024){
		printk("size is too big!\n");
		s=1024;
	}
	if(copy_to_user(buf,buffer+filp->f_pos, s)) return -1;
	return s;
}

/* seek文件定位函数 */
static loff_t my_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	loff_t old_pos = filp->f_pos;

	switch (orig)    //开始位置
	{
	case SEEK_SET:  //从头开始
		ret = offset;
		break;
	case SEEK_CUR:  //从当前开始
		ret = old_pos + offset;
		break;
	case SEEK_END:  //从末尾
		ret = old_pos + DEVSIZE;
		break;
	default:
		printk("orig error\n");
		return -EINVAL;
	}
	if (ret<0 || ret>DEVSIZE)
	{
		printk("f_pos over failed\n");
		return -EINVAL;
	}
	filp->f_pos = ret;
	return ret;
}
