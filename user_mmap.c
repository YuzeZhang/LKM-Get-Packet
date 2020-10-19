#include "user_mmap.h"

void *g_pBuffer = NULL;
int g_fd = -1;

void *getMemMsgBuf()
{
    unsigned char *ret;
    if (NULL != g_pBuffer)
    {
        return g_pBuffer;
    }

    if (g_fd < 0)
    {
        g_fd = open("/dev/memMsgDev", O_RDWR);
        if (0 > g_fd)
        {
            printf("Error : getMemMsgBuf() open /dev/memMsgDev failed , errno = %d \n", errno);
            return NULL;
        }
    }

    g_pBuffer = mmap(NULL, 4096 * 10, PROT_READ | PROT_WRITE, MAP_SHARED, g_fd, 0);
    if (g_pBuffer == MAP_FAILED)
    {
        printf("Error : getMemMsgBuf() failmmap!\n");
        return NULL;
    }

    return g_pBuffer;
}