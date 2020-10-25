#include "user.h"

void c_print(char *str)
{
    printf("%s\n", str);
}

char *get_c_buf()
{
    char *buf = malloc(sizeof(char) * 1024);
    return buf;
}

int check_input_ip(char *ip_str)
{
    unsigned int val = 0, part = 0;
    int i = 0;
    char c;
    for (i = 0; i < 4; ++i)
    {
        part = 0;
        while ((c = *ip_str++) != '\0' && c != '.')
        {
            if (c < '0' || c > '9')
                return -1;
            part = part * 10 + (c - '0');
        }
        if (part > 255)
            return -1;
        val = ((val << 8) | part);
        if (i == 3)
        {
            if (c != '\0')
                return -1;
        }
        else
        {
            if (c == '\0')
                return -1;
        }
    }
    return val;
}

int open_dev_file()
{
    int fd = open("../mychardev0", O_RDWR);
    if (fd < 0)
    {
        return -1;
    }
    return fd;
}

int my_write(int fd, char *ip_str)
{
    if (write(fd, ip_str, strlen(ip_str)) == -1)
    {
        return -1;
    }
    return 1;
}

int my_read(int fd, char *buf)
{
    if (read(fd, buf, strlen(buf)) == -1)
    {
        return -1;
    }
    return 1;
}

/*
int main()
{
    int fd;
    char buffer_write[20] = {};
    char buffer_read[4096] = {};

    fd = open("./mychardev0", O_RDWR);
    if (fd < 0)
    {
        printf("open dev error!\n");
        exit(fd);
    }

    //向指定设备写入用户输入文本
    // printf("Please input the text:\n");
    // scanf("%s", buffer_write);
    write(fd, "172.21.0.7", 20);

    sleep(3);

    //输出设备中的内容
    read(fd, buffer_read, 20);
    printf(buffer_read);

    sleep(3);
    memset(buffer_read, 0, 4096);

    printf("!!!!!!!!!!!!!!!!");
    //输出设备中的内容
    read(fd, buffer_read, 20);
    printf(buffer_read);

    close(fd);
    return 0;
}
*/