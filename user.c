#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

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
