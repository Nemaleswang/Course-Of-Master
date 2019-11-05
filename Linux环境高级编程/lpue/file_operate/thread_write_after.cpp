/*
 * =====================================================================================
 *
 *       Filename:  thread_write_after.cpp
 *
 *    Description:  practise
 *
 *        Version:  1.0
 *        Created:  2010年10月17日 18时37分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), YOUR EMAIL
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */


#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

using std::cerr;
using std::endl;


const int bufSize = 100;
const int times = 1000;

int   fd;
pthread_mutex_t io_err_mutex = PTHREAD_MUTEX_INITIALIZER;

void *
thrd_fn(void * arg)
{

	int index = *(int*)arg;
	off_t endPos;
	char buf[bufSize];

	for (int i = 0; i < bufSize - 1; ++i)
		buf[i] = index + '0';
	buf[bufSize - 1] = '\n';

	sleep(3);

	for (int i = 0; i < times; ++i)	
	{

 		if ( (endPos = lseek(fd, 0, SEEK_END) ) == -1)
		{
			pthread_mutex_lock(&io_err_mutex);
			cerr << "lseek error: "
				<< strerror(errno) << endl;
			pthread_mutex_unlock(&io_err_mutex);
			return (void *)-1;
		}
		usleep(100);
		if (write(fd, buf, bufSize) == -1)
		{
			pthread_mutex_lock(&io_err_mutex);
			cerr << "write error: "
				<< strerror(errno) << endl;
			pthread_mutex_unlock(&io_err_mutex);
			return (void *)-1;
		}
	}
    delete (int *)arg;
	return (void*)0;
}

int
main(void)
{
    int *addr;
	pthread_t tid[3];
	for (int index = 0; index < 3; ++index)
	{
        addr = new(int);
        *addr = index;
		if (pthread_create(&tid[index], NULL, thrd_fn, (void *)addr) != 0)
		{
			pthread_mutex_lock(&io_err_mutex);
			cerr << "creat thread error: "
				<< strerror(errno) << endl;
			pthread_mutex_unlock(&io_err_mutex);
			return -1;
		}
	}

	if ((fd = creat("thread_after_file", S_IRUSR | S_IWUSR | 
				   	S_IRGRP | S_IROTH)) == -1)
	{
		pthread_mutex_lock(&io_err_mutex);
		cerr << "creat file error: "
			<< strerror(errno) << endl;
		pthread_mutex_unlock(&io_err_mutex);
		return -1;
	}

	for(int index = 0; index < 3; ++index)
		pthread_join(tid[index], NULL);
	close(fd);
	return 0;
}
