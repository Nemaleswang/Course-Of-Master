#include <iostream>

using namespace std;

const int FUNC_ID = 1;

extern "C" void Print()
{
    cout << "Hello World!" << endl;
}

extern "C" void Help()
{
    cout << "Function ID " << FUNC_ID << " : This function will print hello world." << endl;
}
