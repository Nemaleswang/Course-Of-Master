#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>

using namespace std;

int main()
{
  struct stat buf;

  if(stat("a.txt", &buf) == 0)
  {
    cout << "UID: " << buf.st_uid << endl;
    cout << "Size: " << buf.st_size << endl;   
    cout << "Block Size: " << buf.st_blksize << endl;
  }

  return 0;
}
