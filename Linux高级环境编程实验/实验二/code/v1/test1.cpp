#include "A.h"

int main(){
    A a(100);
    a.Serialize("data");
    A b = A::Deserialize("data");
    b.f();
    return 0;
}