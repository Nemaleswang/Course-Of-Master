#include <dlfcn.h>
#include <iostream>
#include "CPluginEnumerator.h"
#include <string.h>

using namespace std;

int main(int argc, char **argv)
{
    if(argc != 2)
		return 0;

    if(strcmp(argv[1], "help") == 0)
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
	
			typedef void (*FUNC_HELP)();

			FUNC_HELP dl_help = (FUNC_HELP)dlsym(handle, "Help");
			if(dl_help == 0)
			{
		    	cout << "dlsym error" << endl;
	    		return 0;
			}
	
			(dl_help)();

			dlclose(handle);
    	}
    }

    return 0;
}
