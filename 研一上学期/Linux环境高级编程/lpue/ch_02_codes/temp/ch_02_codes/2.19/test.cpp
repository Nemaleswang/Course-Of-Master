#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main()
{
  int fd = creat("a.txt", O_RDWR);
  if(-1 == fd)
  {
     cout << "creat error" << endl;
  }
  else
  {
    cout << "creat success" << endl;
  }

  sleep(5);

  unlink("a.txt");

  return 0;
}
