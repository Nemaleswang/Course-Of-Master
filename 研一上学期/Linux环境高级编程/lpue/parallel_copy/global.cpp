
#include "global.h"


const int bufferSize = 10;
const int blockNum = 8092;
const int blockSize = 16;

int fileFd;
int pipeFd[2];
int blockRecv = 0;
int blockFinished = 0;
int blockCnt = 0;
int bufferSlot = bufferSize;
char *blockPtrs[blockNum];
int buffer[bufferSize];

void bufferInit(int);
int setblocking(int);
int setNoblocking(int);
void *blkProducer(void *);
void *blkConsumer(void *);

pthread_mutex_t stderrMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t freeMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t blockCntMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t blockRecvMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t blockFinishedMutex = PTHREAD_MUTEX_INITIALIZER; 

pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t bufferReadCond = PTHREAD_COND_INITIALIZER; 
pthread_cond_t bufferWriteCond = PTHREAD_COND_INITIALIZER; 
