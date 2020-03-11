#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
  cout << "a.txt" << endl;

  cout << "X_OK  " << access("a.txt", X_OK) << endl;
  cout << "R_OK  " << access("a.txt", R_OK) << endl;
  cout << "W_OK  " << access("a.txt", W_OK) << endl;
  cout << "F_OK  " << access("a.txt", F_OK) << endl;

  return 0;
}
