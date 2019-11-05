#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>

using namespace std;

int main()
{
  int fd = open("a.txt", O_WRONLY | O_APPEND);

  int val = fcntl(fd, F_GETFL);
  if(-1 == val)
  {
    cout << "fcntl error" << endl;
    return 0; 
  }

  int accessmode = val & O_ACCMODE; //why?
  switch(accessmode)    
  {
  case O_RDONLY:
    cout << "Read only" << endl;
    break;

  case O_WRONLY:
    cout << "Write only" << endl;
    break;

  case O_RDWR:
    cout << "Read and Write" << endl;
    break;

  default:
    break;
  }

  if(val & O_APPEND)
    cout << "O_APPEND " << O_APPEND << endl;

  if(val & O_NONBLOCK)
    cout << "O_NONBLOCK " << O_NONBLOCK << endl;

  return 0;
}

