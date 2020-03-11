#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(int argc, char **argv)
{
  cout << argv[0] << endl;

  unlink(argv[0]);

  return 0;
}
