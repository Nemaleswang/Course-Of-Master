#include<pthread.h>
#include<stdio.h>
#include<malloc.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<signal.h>
#include<sys/shm.h>
#include<time.h>
#define NTHREAD 10
#define BLOCKSIZE 4096
#define headlong 4
#define MYMODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

int outfd,infd;
int pfd[2];
struct data
{
	int ioffset;
	int ilen;
	char buf[BLOCKSIZE];
};

void *thr_w(void *arg)
{
	int n = 0;
	struct data *tempdataptr;
	for(;;)
	{
		if((n = read(pfd[0], &tempdataptr, sizeof(struct data *))) < 0)
			printf("read pipe error\n");
		else if(n == 0)
			break;
		else
		{
			pwrite(outfd, tempdataptr->buf, tempdataptr->ilen, tempdataptr->ioffset);
			free(tempdataptr);
		}
	}
	pthread_exit((void *)0);
}
void *thr_r(void *arg)
{
	int thr_num = (int)arg;
	int n = 0;
	struct data *tempdataptr;
	for(;;)
	{
		tempdataptr = malloc(sizeof(struct data));
		if((n = pread(infd, tempdataptr->buf, BLOCKSIZE, BLOCKSIZE*thr_num)) < 0)
			printf("read infd error\n");
		else if(n == 0)
		{
			free(tempdataptr);
			break;
		}
		else
		{
			tempdataptr->ioffset = BLOCKSIZE*thr_num;
			tempdataptr->ilen = n;
			write(pfd[1], &tempdataptr, sizeof(struct data *));
			thr_num += NTHREAD;
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

	if(argc != 3)
	{
		printf("usage:./task4.out src dst\n");
		return -1;
	}

	outfd=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,MYMODE);
	if(outfd<0)
	{
		printf(" create outfile.txt failed!\n");
		return -1;
	}

	infd=open(argv[1],O_RDONLY);
	if(infd<0)
	{
		printf("open infile.txt error!\n");
		return -1;
	}
	if(pipe(pfd)<0)
	{
		printf("create pipe error!\n");
		return -1;
	}

	for(n=0;n<NTHREAD;n++)
	{
		err=pthread_create(&rtid[n],NULL,thr_r,(void *)n);
		if(err!=0)
		{
			printf("create the %dth thread error!\n",n);
			return -1;
		}
	}
	for(n=0;n<NTHREAD;n++)
	{
		err=pthread_create(&wtid[n],NULL,thr_w,NULL);
		if(err!=0)
		{
			printf("create the %dth thread error!\n",n);
			return -1;
		}
	}
	for(n=0;n<NTHREAD;n++)	
	{
		err=pthread_join(rtid[n],NULL);
		if(err!=0)
		{
			printf("join the %dth thread error!\n",n);
			return -1;
		}
	}
	close(pfd[1]);
	for(n=0;n<NTHREAD;n++)	
	{
		err=pthread_join(wtid[n],NULL);
		if(err!=0)
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
	printf( "%f seconds\n", total_time);
	return 0;
}
