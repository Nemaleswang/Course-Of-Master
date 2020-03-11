#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

int main()
{
  int fd = open("a.txt", O_RDONLY);
  if(-1 == fd)
  {     
    cout << "open error" << endl;
    cout << strerror(errno) << endl;
  }
  
  return 0;
}
