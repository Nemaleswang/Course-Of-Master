#include<pthread.h>
#include<stdio.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<signal.h>
#include<sys/shm.h>
#include<time.h>
#include<pthread.h>
//#include"apue.h"
#define NTHREAD 2 
#define BLOCKSIZE 4096
#define MYMODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

static unsigned long int blknum=0;
static unsigned long int bllknum=0;
static int outfd,infd;
static int pfd[2];

pthread_mutex_t wmutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rmutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pmutex=PTHREAD_MUTEX_INITIALIZER;
void *thr_w(void *arg)
{
    unsigned long int temp;

    int n=1;
    char tempbuf[BLOCKSIZE];
    while (1)
    {
        pthread_mutex_lock(&wmutex);
        n=read(pfd[0],tempbuf,BLOCKSIZE);
        if (n==0)
        {
            pthread_mutex_unlock(&wmutex);
            break;
        }
        temp=blknum++;
//		fprintf(stderr,"%lu\t%lu\n",temp,pthread_self());
        pthread_mutex_unlock(&wmutex);
        if (pwrite(outfd,tempbuf,n,temp*BLOCKSIZE)!=n)
        {
            printf("thread write file error!\n");
            pthread_exit((void *)0);
        }
        if (n<0)
        {
            printf("thread read pipe error!\n");
            pthread_exit((void *)0);
        }
    }
    pthread_exit((void *)0);
}
void *thr_r(void *arg)
{
    unsigned long int temp;
    int n=1;
    int i_rnum;
    char tempbuf[BLOCKSIZE];
    while (1)
    {
        pthread_mutex_lock(&rmutex);
        n=read(infd,tempbuf,BLOCKSIZE);
        if (n==0)
        {
            pthread_mutex_unlock(&rmutex);
            break;
        }
        pthread_mutex_lock(&pmutex);
        pthread_mutex_unlock(&rmutex);
//		temp=bllknum++;
//		fprintf(stderr,"%lu\t%lu\n",temp,pthread_self());
        i_rnum=write(pfd[1],tempbuf,n);
        pthread_mutex_unlock(&pmutex);
        if (i_rnum!=n)
        {
            printf("write pipe error!\n");
            pthread_exit((void *)0);
        }
        if (n<0)
        {
            printf("read infile.txt error!\n");
            pthread_exit((void *)0);
        }
    }
    pthread_exit((void *)0);
}
int main(int argc,char *argv[])
{
    clock_t start_time,finish_time;
    double total_time;
    start_time=clock();

    int n,err;
    pthread_t wtid[NTHREAD];
    pthread_t rtid[NTHREAD];

    if (argc != 3)
    {
        printf("usage:./task4.out src dst\n");
        return -1;
    }

    outfd=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,MYMODE);
    if (outfd<0)
    {
        printf(" create outfile.txt failed!\n");
        return -1;
    }

    infd=open(argv[1],O_RDONLY);
    if (infd<0)
    {
        printf("open infile.txt error!\n");
        return -1;
    }
    if (pipe(pfd)<0)
    {
        printf("create pipe error!\n");
        return -1;
    }

    for (n=0;n<NTHREAD;n++)
    {
        err=pthread_create(&rtid[n],NULL,thr_r,NULL);
        if (err!=0)
        {
            printf("create the %dth thread error!\n",n);
            return -1;
        }
    }
    for (n=0;n<NTHREAD;n++)
    {
        err=pthread_create(&wtid[n],NULL,thr_w,NULL);
        if (err!=0)
        {
            printf("create the %dth thread error!\n",n);
            return -1;
        }
    }
    for (n=0;n<NTHREAD;n++)
    {
        err=pthread_join(rtid[n],NULL);
        if (err!=0)
        {
            printf("join the %dth thread error!\n",n);
            return -1;
        }
    }
    close(pfd[1]);
    for (n=0;n<NTHREAD;n++)
    {
        err=pthread_join(wtid[n],NULL);
        if (err!=0)
        {
            printf("join the %dth thread error!\n",n);
            return -1;
        }
    }
    close(infd);
    close(outfd);
    close(pfd[0]);

    finish_time=clock();
    total_time=((double)(finish_time-start_time))/CLOCKS_PER_SEC;
    printf( "%f seconds/n", total_time);
    return 0;
}
