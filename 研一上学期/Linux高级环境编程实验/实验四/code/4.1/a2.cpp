#include<iostream>
#include"a.h"

using namespace std;

extern "C" void g()
{
    cout << "g()" << endl;
}
