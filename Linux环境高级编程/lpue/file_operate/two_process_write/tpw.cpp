#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main()
{
  
//  int fd = open("a.c", O_RDWR);
    int fd = open("a.c", O_RDWR | O_APPEND);
  if(-1 == fd)
  {
    cout << "open error" << endl;
    return 0;
  }

  char *buf = "11111";
  int n = 1000;
 
  while (n--){

    if(write(fd, buf, strlen(buf)) == -1)
      { 
        cout << "write error" << endl;
        close(fd);
        return 0;
      }
    usleep(100);
  }

  close(fd);

  return 0;
}


