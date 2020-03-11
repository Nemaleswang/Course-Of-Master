
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <pthread.h>
#include <fcntl.h>

#ifndef GLOBAL_HEADER_H
#define GLOBAL_HEADER_H

struct  Block
{
    int blk_id;
    int blk_stat;
};

extern const int bufferSize;
extern const int blockNum;
extern const int blockSize;

extern  int fileFd;
extern int pipeFd[];
extern int blockRecv;
extern int blockCnt;
extern int bufferSlot;
extern char *blockPtrs[];
extern int buffer[];

extern void bufferInit(int);
extern int setNoblocking(int);
extern void *blkProducer(void *);
extern void *blkConsumer(void *);

extern pthread_mutex_t stderrMutex;
extern pthread_mutex_t stdoutMutex;
extern pthread_mutex_t blockCntMutex;
extern pthread_mutex_t blockRecvMutex;

extern pthread_mutex_t bufferMutex;
extern pthread_cond_t bufferReadCond;
extern pthread_cond_t bufferWriteCond;

#endif
