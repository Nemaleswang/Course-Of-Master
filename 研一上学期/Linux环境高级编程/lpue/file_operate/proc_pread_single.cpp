
/*
 * =====================================================================================
 *
 *       Filename:  proc_read_single.cpp
 *
 *    Description:  practise
 *
 *        Version:  1.0
 *        Created:  2010年10月18日 09时52分45秒
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
#include <sys/wait.h>

using std::cout;
using std::cerr;
using std::endl;


const int bufSize = 100;

int
main(int argc, char **argv)
{
	int   fd;
	off_t curPos;
	char buf[bufSize + 1];


	if ((fd = open("datafile", O_RDONLY)) == -1)
	{
		cerr << "creat file error: "
			<< strerror(errno) << endl;
		return -1;
	}

	if (pread(fd, buf, bufSize, 4 * 100) == -1)
	{
		cerr << "write error: "
			<< strerror(errno) << endl;
		return -1;
	}
	buf[bufSize] = '\0';

	cout << buf << endl;

    sleep(20);
	close(fd);
	return 0;
}
