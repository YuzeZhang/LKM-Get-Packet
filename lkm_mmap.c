#include "lkm_mmap.h"

int dev_major = 696;
int dev_minor = 0;

// dev storage size
unsigned long gsize = 4096 * 10;
// dev storage ptr
unsigned char *mem_msg_buf = NULL;

struct info *infop = NULL;

struct char_dev
{
    char *data;
    struct cdev cdev;
};

struct char_dev *char_device;

module_param(dev_major, int, S_IRUGO);
module_param(dev_minor, int, S_IRUGO);

int char_open(struct inode *inode, struct file *fp)
{
    struct char_dev *dev;
    // get the dev structure from inode
    dev = container_of(inode->i_cdev, struct char_dev, cdev);
    fp->private_data = dev;

    return 0;
}

static ssize_t char_read(struct file *fp, char __user *buf, size_t count, loff_t *offset)
{
    char *buffer;
    ssize_t size;
    fp->private_data = infop->data;
    buffer = (char *)fp->private_data;
    size = infop->size;

    if (copy_to_user(buf, buffer, infop->size))
    {
        printk("copy_to_user error!\n");
        return -EFAULT;
    }
    memset(infop->data, 0, infop->size);
    infop->size = 0;
    printk("%s\n", buf);
    printk("User are using the read func!\n");
    return size;
}

static ssize_t char_write(struct file *fp, const char __user *buf, size_t count, loff_t *offset)
{
    unsigned int ip = ip_atoi(buf);
    printk("User are using the write func!\n");
    if (ip != -1)
    {
        filter_ip = ip;
        printk("lkm successfully obtained the IP address sent by the user.\n");
    }
    else
    {
        printk("Error: Wrong IP address was entered!\n");
        return -EFAULT;
    }

    // if (copy_from_user(buffer, buf, count))
    // {
    //     printk("copy_from_user error!\n");
    //     return -EFAULT;
    // }
    return strlen(buf);
}

int char_release(struct inode *inode, struct file *fp)
{
    return 0;
}

int mem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long physics = ((unsigned long)mem_msg_buf) - PAGE_OFFSET;
    unsigned long mypfn = physics >> PAGE_SHIFT;

    unsigned long vmsize = vma->vm_end - vma->vm_start;
    unsigned long psize = gsize - offset;
    if (vmsize > psize)
        return -ENXIO;
    if (remap_pfn_range(vma, vma->vm_start, mypfn, vmsize, vma->vm_page_prot))
        return -EAGAIN;
    return 0;
}

struct file_operations Fops = {
    .read = char_read,
    .write = char_write,
    .open = char_open,
    .release = char_release,
    .mmap = mem_mmap,
};

unsigned char *malloc_reserved_mem(unsigned int size)
{
    unsigned char *p = kmalloc(size, GFP_KERNEL);
    unsigned char *tmp = p;
    unsigned int i, n;
    if (NULL == p)
    {
        printk("Error : malloc_reserved_mem kmalloc failed!\n");
        return NULL;
    }
    n = size / 4096 + 1;
    if (0 == size % 4096)
    {
        --n;
    }
    for (i = 0; i < n; ++i)
    {
        SetPageReserved(virt_to_page(tmp));
        tmp += 4096;
    }
    return p;
}

int lkm_mmap_init(void)
{
    int res = 0;
    dev_t dev = 0, devno = 0;
    int err = 0;
    mem_msg_buf = malloc_reserved_mem(gsize);

    if (dev_major)
    {
        dev = MKDEV(dev_major, dev_minor);
        res = register_chrdev_region(dev, 1, "get_packet");
    }
    else
    {
        res = alloc_chrdev_region(&dev, dev_minor, 1, "get_packet");
        dev_major = MAJOR(dev);
    }

    if (res < 0)
    {
        printk("Can't get major %d!\n", dev_major);
        return res;
    }

    char_device = kmalloc(sizeof(struct char_dev), GFP_KERNEL);

    if (!char_device)
    {
        res = -ENOMEM;
        return res;
    }

    memset(char_device, 0, sizeof(struct char_dev));

    cdev_init(&char_device->cdev, &Fops);
    char_device->cdev.owner = THIS_MODULE;
    char_device->cdev.ops = &Fops;

    devno = MKDEV(dev_major, dev_minor);

    err = cdev_add(&char_device->cdev, devno, 1);
    char_device->data = mem_msg_buf;

    infop = kmalloc(sizeof(struct info), GFP_KERNEL);
    infop->data = malloc_reserved_mem(gsize);
    infop->cap = gsize;
    infop->size = 0;

    return 0;
}

void lkm_mmap_exit(void)
{
    dev_t devno = 0;
    devno = MKDEV(dev_major, dev_minor);
    cdev_del(&char_device->cdev);
    kfree(char_device);
    // kfree(mem_msg_buf);
    unregister_chrdev_region(devno, 1);
}