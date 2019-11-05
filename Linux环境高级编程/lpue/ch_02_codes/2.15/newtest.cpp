#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
  cout << "/usr/bin/passwd" << endl;

  cout << "X_OK  " << access("/usr/bin/passwd", X_OK) << endl;
  cout << "R_OK  " << access("/usr/bin/passwd", R_OK) << endl;
  cout << "W_OK  " << access("/usr/bin/passwd", W_OK) << endl;
  cout << "F_OK  " << access("/usr/bin/passwd", F_OK) << endl;

  return 0;
}
