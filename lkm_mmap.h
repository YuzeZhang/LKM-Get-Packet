#pragma once

#include "lkm.h"

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// dev storage size
extern unsigned long gsize;
// dev storage ptr
extern unsigned char *mem_msg_buf;
extern unsigned char *packetInfo;

extern unsigned int filter_ip;

extern unsigned int ip_atoi(char *ip_str);

struct info
{
    unsigned char *data;
    size_t cap;
    size_t size;
};
// struct info *infop;

// unsigned char *malloc_reserved_mem(unsigned int size);
// int mem_mmap(struct file *filp, struct vm_area_struct *vma);
int lkm_mmap_init(void);
void lkm_mmap_exit(void);
unsigned char *malloc_reserved_mem(unsigned int size);