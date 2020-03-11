#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main()
{
  int fd = creat("file.hole", S_IRWXU);

  if(-1 == fd)
  {
    cout << "creat error" << endl;
    return 0;
  }  


  char buf[100];
  for(int i = 0; i < 100; i++)
    buf[i] = 'a';

  if(write(fd, buf, 100) != 100)
  {
    cout << "write 1 error" << endl;
    return 0;
  }

  if(lseek(fd, 50, SEEK_END) == -1)  
  {
    cout << "lseek error" << endl;
    return 0;
  }

  if(write(fd, buf, 50) != 50)
  { 
    cout << "write 2 error" << endl;
    return 0;
  }

  return 0;
}
