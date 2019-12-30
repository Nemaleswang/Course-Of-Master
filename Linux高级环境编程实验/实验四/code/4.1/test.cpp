#include"a.h"
#include<dlfcn.h>
#include<iostream>

using namespace std;

int main()
{
    void *handle = dlopen("./libtest.so", RTLD_LAZY);
    if(0 == handle)
    {
    	cout << "dlopen error" << endl;
    	return 0;
    }
    
    typedef void (*Fun)();

    Fun f1 = (Fun)dlsym(handle, "f");

    if(0 == f1)
    {
        cout << "f1 error" << endl;
        char *str = dlerror();
        cout << str << endl;
    }
	
    (*f1)();

    dlclose(handle);

    return 0;
}
