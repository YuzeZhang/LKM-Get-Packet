#pragma once

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>

static unsigned char *malloc_reserved_mem(unsigned int size);
int mem_mmap(struct file *filp, struct vm_area_struct *vma);
void mmap_init(void);