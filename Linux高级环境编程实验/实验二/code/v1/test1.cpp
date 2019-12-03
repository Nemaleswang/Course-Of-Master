#include "A.h"

int main(){
<<<<<<< HEAD
    A a(100);
=======
    A a(50);
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
    a.Serialize("data");
    A b = A::Deserialize("data");
    b.f();
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
