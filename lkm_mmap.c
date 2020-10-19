#include "lkm_mmap.h"

/*
struct file_operations Fops = {
    .read = char_read,
    .write = char_write,
    .open = char_open,
    .release = char_release,
    .mmap = mem_mmap,
};
*/

static unsigned long gsize = 4096 * 10;
static unsigned char *mem_msg_buf = NULL;

static unsigned char *malloc_reserved_mem(unsigned int size)
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

int mem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    printk("inmem_mmap\n");
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

void mmap_init(void)
{
    mem_msg_buf = malloc_reserved_mem(gsize);
}
