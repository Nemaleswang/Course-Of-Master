#include <dlfcn.h>
#include <iostream>
#include "CPluginEnumerator.h"

using namespace std;

int main()
{
    vector<string> vstrPluginNames;
    
    CPluginEnumerator enumerator;
    if(!enumerator.GetPluginNames(vstrPluginNames))
    {
		cout << "GetPluginNames error" << endl;
		return 0;
    }

    for(int i = 0; i< vstrPluginNames.size(); i++)
    {
		void *handle = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY);
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
    }

    return 0;
}
