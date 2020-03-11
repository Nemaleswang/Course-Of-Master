/*
 * =====================================================================================
 *
 *       Filename:  proc_pread.cpp
 *
 *    Description:  practise
 *
 *        Version:  1.0
 *        Created:  2010年10月18日 10时06分20秒
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
main(void)
{
	int   fd;
	pid_t pid;
	if ((fd = open("datafile", O_RDONLY)) == -1)
	{
		cerr << "creat file error: "
			<< strerror(errno) << endl;
		return -1;
	}
	for (int index = 0; index < 10; ++index)
	{
		if ((pid = fork()) < 0)
		{
			cerr << "fork error: "
			   << strerror(errno) << endl;
			return -1;
		}
		else if (pid == 0)
		{
			off_t rightPos = index * 100;
			char buf[bufSize + 1];

			usleep(1000);
			if (pread(fd, buf, bufSize, rightPos) == -1)
			{
				cerr << "read error: "
					<< strerror(errno) << endl;
				return -1;
			}
			buf[bufSize] = '\0';

			cout << "proc " << index << ":"
				<< buf << endl;

			close(fd);
			return 0;
		}
	}

	for(int index = 0; index < 10; ++index)
		waitpid(-1, NULL, 0);
	close(fd);
	return 0;
}
