#include <iostream>
#include <fcntl.h>

using namespace std;

int main()
{
  int fd = open("a.c", O_RDWR);   

  char buf = 'q';

  cout << write(fd, &buf, 1) << endl;

  return 0;
}