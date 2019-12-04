#include <dlfcn.h>
#include <iostream>

using namespace std;

int main()
{
    void *handle = dlopen("./libfun.so", RTLD_LAZY);
    if(handle == 0)
    {
	cout << "dlopen error" << endl;
	return 0;
    }

    typedef void (*FUNC_PRINT)();

    FUNC_PRINT dl_print = (FUNC_PRINT)dlsym(handle, "Print");
    if(dl_print == 0)
    {
	cout << "dlsym error" << endl;
	return 0;
    }

    (dl_print)();

    dlclose(handle);

    return 0;
}
