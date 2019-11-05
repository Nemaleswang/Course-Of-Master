#include<iostream>
#include<sys/types.h>
#include<unistd.h>

using namespace std;

int main()
{
  cout << truncate("a.txt", 10) << endl;
  return 0;
}