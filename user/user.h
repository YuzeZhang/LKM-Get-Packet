#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int check_input_ip(char *ip_str);
int open_dev_file();
int my_write(int fd, char *ip_str);
int my_read(int fd, char *buf);
char *get_c_buf();
void c_print(char *str);