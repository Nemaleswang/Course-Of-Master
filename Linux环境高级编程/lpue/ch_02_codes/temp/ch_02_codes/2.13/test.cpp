#include <sys/stat.h>
#include <iostream>

using namespace std;

int main()
{
  struct stat buf;
  
  stat("a.txt", &buf);

  cout << buf.st_mode << endl;

  if(S_IWGRP & buf.st_mode)
    cout << "can read" << endl;
  else
    cout << "cannot write" << endl;
  
  return 0;
}
