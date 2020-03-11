
#include <cstring>
#include "global.h"


using std::cout;
using std::cerr;
using std::endl;

int readPipeLocked(int);

void * blkProducer(void *arg)
{
    int blkId;
    char *ptr;

    if (setNoblocking(pipeFd[0]) == -1)
    {
        pthread_mutex_lock(&stderrMutex);
        cerr << "producer set no blocking error"
            << endl;
        pthread_mutex_unlock(&stderrMutex);
        return NULL;
    }

    for (;;)
    {
        pthread_mutex_lock(&blockCntMutex);
        if (blockCnt == blockNum)
        {
            pthread_mutex_unlock(&blockCntMutex);
            break;
        }
        if ((ptr = (char *)(malloc(blockSize))) == NULL)
        {
            pthread_mutex_unlock(&blockCntMutex);
            continue;
        }
        memset(ptr, '1', blockSize);
        blkId = blockCnt;
        blockPtrs[blockCnt++] = ptr;
        pthread_mutex_unlock(&blockCntMutex);

        pthread_mutex_lock(&bufferMutex);
        while (bufferSlot == 0)
            pthread_cond_wait(&bufferWriteCond, &bufferMutex);
        for (int i = 0; i < bufferSize; ++i)
        {
            if (buffer[i] == -1)
            {
                buffer[i] = blkId;
                break;
            }
        }
        --bufferSlot;


        pthread_mutex_lock(&stdoutMutex);
        cout << "post one block, block id is: " << blkId
            << endl;
        pthread_mutex_unlock(&stdoutMutex);

        pthread_cond_signal(&bufferReadCond);
        pthread_mutex_unlock(&bufferMutex);

        pthread_mutex_lock(&blockRecvMutex);
        readPipeLocked(pipeFd[0]);
        pthread_mutex_unlock(&blockRecvMutex);
    }

    for (;;)
    {
        pthread_mutex_lock(&blockRecvMutex);
        if (blockRecv == blockNum)
        {
            pthread_mutex_unlock(&blockRecvMutex);
            break;
        }
        readPipeLocked(pipeFd[0]);
        pthread_mutex_unlock(&blockRecvMutex);
    }
    return NULL;
}

    int 
readPipeLocked(int readfd)
{
    int num;
    struct Block block;
    char *ptr = (char *)(&block);
    char msg[8];
    num = read(readfd, msg, 8); 
    if (num == - 1 && errno == EAGAIN) 
        return 0;
    if (num != 8)
    {
        pthread_mutex_lock(&stderrMutex);
        cerr << "read pipe error" << endl; 
        pthread_mutex_unlock(&stderrMutex);
        return -1;
    }

    ++blockRecv;
    for (int i = 0; i < 8; ++i)
        *ptr++ = msg[i];

    pthread_mutex_lock(&stdoutMutex);
    cout << "block " <<  block.blk_id
        << " finished" << endl;
    cout << "the status is: " << block.blk_stat
        << endl;
    cout << "recv finished blocks is: " << blockRecv	<< endl;
    pthread_mutex_unlock(&stdoutMutex);

    free(blockPtrs[block.blk_id]);
    return 0;
}

