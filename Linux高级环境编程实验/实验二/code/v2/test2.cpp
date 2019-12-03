#include "A.h"
#include <iostream>
#include <vector>
#include "SerializerForAs.h"

using namespace std;

int main(){
    A a1(100);
    A a2(200);

    vector<A *> v1;
    v1.push_back(&a1);
    v1.push_back(&a2);
    SerializerForAs::Serialize("data2", v1);

    A a3;
    A a4;

    vector<A *> v2;
    v2.push_back(&a3);
    v2.push_back(&a4);
    SerializerForAs::Deserialize("data2", v2);
    a3.f();
    a4.f();

}