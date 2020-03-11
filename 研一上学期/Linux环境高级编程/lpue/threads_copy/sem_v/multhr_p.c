#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<pthread.h>
#include<fcntl.h>
#include<semaphore.h>
#include<string.h>
#define MAXLINE 1024
#define bufferNum 10


sem_t  gv_semProducer;
sem_t  gv_semCustomer;
sem_t  gv_semmutexProducer;
sem_t  gv_semmutexCustomer;
sem_t  gv_semMutex;

int gv_ifileIn, gv_ifileOut;
char gv_cBuffer[bufferNum][MAXLINE + 1];
int gv_iptrIn, gv_iptrOut;
int gv_iproCount;
int count;
int  gv_iproducerNum, gv_icustomerNum, gv_iprocessNum;
unsigned long gv_ulOffset, MAXBYTES;
struct stat gv_stfileStat;

void
*thrCustomer(void *arg)
{
    char lv_cBuffer[MAXLINE+1];
    int lv_iPtr;
    unsigned long lv_lOffset;
    int lv_idataLen;
    int lv_isemValue;
    int lv_inWrite;

    while (1)
    {
        sem_wait(&gv_semCustomer);
        sem_wait(&gv_semmutexCustomer);
        lv_iPtr = gv_iptrOut;
        lv_lOffset = gv_ulOffset;
        lv_idataLen = strlen(gv_cBuffer[lv_iPtr]);
        gv_ulOffset+=lv_idataLen;
        gv_iptrOut = (gv_iptrOut + 1)  % 10;
        strcpy(lv_cBuffer,gv_cBuffer[lv_iPtr]);
        memset(gv_cBuffer[lv_iPtr],0,MAXLINE+1);
        sem_post(&gv_semmutexCustomer);

        sem_getvalue(&gv_semProducer,&lv_isemValue);
        sem_post(&gv_semProducer);

        lv_inWrite = pwrite(gv_ifileOut, lv_cBuffer, lv_idataLen, lv_lOffset);

        if ((lv_inWrite <= 0) && (gv_iproCount == gv_iproducerNum))
        {
            fputs("customer quit\n",stderr);
            fprintf(stderr, "offset: %u   empty: %d \n", lv_lOffset, lv_isemValue);
            sem_post(&gv_semCustomer);
            pthread_exit(NULL);
        }

        // fprintf(stderr,"proqNum: %d\n ", gv_iproCount);

        //fprintf(stderr, "offset: %u\n", lv_lOffset);
    }
}

void
*thrProducer(void *arg)
{
    char lv_cBuffer[MAXLINE+1];
    int lv_iPtr;
    int lv_ireadNum;

    while (1)
    {
        sem_wait(&gv_semProducer);
        sem_wait(&gv_semmutexProducer);
        if  ((lv_ireadNum=read(gv_ifileIn, lv_cBuffer, MAXLINE)) <= 0)
        {
            fputs("producer quit\n",stderr);
            gv_iproCount++;
            sem_post(&gv_semCustomer);
            sem_post(&gv_semmutexProducer);
            pthread_exit(NULL);
        }
        lv_cBuffer[lv_ireadNum]='\0';
        lv_iPtr=gv_iptrIn;
        gv_iptrIn=(gv_iptrIn+1) % 10;
        count++;
        strcpy(gv_cBuffer[lv_iPtr], lv_cBuffer);
        sem_post(&gv_semmutexProducer);

        fprintf(stderr, "num: %d\n", count);
        sem_post(&gv_semCustomer);
    }
}

int
main()
{
    int i;
    struct timeval lv_tstartTime, lv_tendTime;
    double timeuse;
    pthread_t lv_pid[100];

    gettimeofday(&lv_tstartTime, NULL);
    gv_ifileIn = open("textin", O_RDONLY|O_NONBLOCK);
    gv_ifileOut = open("textout", O_WRONLY);
    fstat(gv_ifileIn, &gv_stfileStat);
    MAXBYTES=(unsigned long)(gv_stfileStat.st_size);

    sem_init(&gv_semProducer, 0, 10);
    sem_init(&gv_semCustomer, 0, 0);
    sem_init(&gv_semmutexProducer, 0, 1);
    sem_init(&gv_semmutexCustomer, 0, 1);
    gv_iptrIn = 0;
    gv_iptrOut = 0;
    gv_ulOffset = 0;
    gv_iproCount = 0;
    count = 0;

    gv_iproducerNum  = 5;
    gv_icustomerNum  = 10;

    for (i=0;i< gv_iproducerNum;i++)
        pthread_create(&lv_pid[i], NULL, thrProducer, NULL);
    for (i=0;i< gv_icustomerNum;i++)
        pthread_create(&lv_pid[i+ gv_iproducerNum], NULL, thrCustomer, NULL);

    gv_iprocessNum = gv_iproducerNum + gv_icustomerNum;
    for (i = 0;i < gv_iprocessNum; i++)
        pthread_join(lv_pid[i],NULL);
    // sleep(10);
    fputs("main stop\n",stderr);
    gettimeofday(&lv_tendTime,NULL);
    timeuse =  100000 * (lv_tendTime.tv_sec-lv_tstartTime.tv_sec)+(lv_tendTime.tv_usec-lv_tstartTime.tv_usec);
    timeuse /= 1000;
    fprintf(stderr,"timeuse: %lf\n", timeuse);
    sem_destroy(&gv_semProducer);
    sem_destroy(&gv_semCustomer);
    sem_destroy(&gv_semmutexProducer);
    sem_destroy(&gv_semmutexCustomer);
    close(gv_ifileIn);
    close(gv_ifileOut);
    return 0;
}
