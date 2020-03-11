#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main()
{
  int fd = dup2(1, 6);
  cout << "After dup2: " << fd << endl;

  int fd1 = dup(6);
  cout << "After dup: " << fd1 << endl;

  return 0;
}
