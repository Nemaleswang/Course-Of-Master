
#include "global.h"

int
setNoblocking(int fd)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    {
        pthread_mutex_lock(&stderrMutex);
        perror("fcntl getfl error");
        pthread_mutex_unlock(&stderrMutex);
        return -1;
    }
    val |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, val) < 0)
    {
        pthread_mutex_lock(&stderrMutex);
        perror("fcntl setfl error");
        pthread_mutex_unlock(&stderrMutex);
        return -1;
    }
    return 0;
}
