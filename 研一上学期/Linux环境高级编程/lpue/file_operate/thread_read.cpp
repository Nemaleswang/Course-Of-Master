
/*
 * =====================================================================================
 *
 *       Filename:  thread_read.cpp
 *
 *    Description:  practise
 *
 *        Version:  1.0
 *        Created:  2010年10月17日 16时45分26秒
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

using std::cout;
using std::cerr;
using std::endl;


const int bufSize = 100;

int   fd;
pthread_mutex_t io_err_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t io_out_mutex = PTHREAD_MUTEX_INITIALIZER;

void *
thrd_fn(void * arg)
{

	int index = *(int*)(arg);
	off_t curPos;
	char buf[bufSize + 1];
	
 	if ( (curPos = lseek(fd, index * 100, SEEK_SET) ) == -1)
	{
		pthread_mutex_lock(&io_err_mutex);
		cerr << "lseek error: "
			<< strerror(errno) << endl;
		pthread_mutex_unlock(&io_err_mutex);
		return (void *)-1;
	}
	usleep(1000);
	if (read(fd, buf, bufSize) == -1)
	{
		pthread_mutex_lock(&io_err_mutex);
		cerr << "read error: "
			<< strerror(errno) << endl;
		pthread_mutex_unlock(&io_err_mutex);
		return (void *)-1;
	}
    buf[bufSize] = '\0';

	pthread_mutex_lock(&io_out_mutex);
    cout << "thread " << index
        << ":" << buf << endl;
	pthread_mutex_unlock(&io_out_mutex);

    delete (int *)arg; 
	return (void*)0;
}

int
main(void)
{
    int *addr;
	pthread_t tid[10];
    if ((fd = open("datafile", O_RDONLY)) == -1)
	{
		cerr << "open file error: "
			<< strerror(errno) << endl;
		return -1;
	}
	for (int index = 0; index < 10; ++index)
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

	for(int index = 0; index < 10; ++index)
		pthread_join(tid[index], NULL);
	close(fd);
	return 0;
}
