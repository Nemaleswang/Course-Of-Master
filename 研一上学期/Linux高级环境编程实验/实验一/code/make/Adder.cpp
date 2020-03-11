#include <iostream>
#include "CAdder.h"

using namespace std;


int main()
{
    CAdder adder;
    int i = adder.Add(3, 4);
    cout << i << endl;
    return 0;
}
