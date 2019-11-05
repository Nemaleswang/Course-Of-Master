#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

using namespace std;

int main()
{
  cout << chown("a.txt", 1001, 1001) << endl;
  cout << strerror(errno) << endl;
  return 0;
}