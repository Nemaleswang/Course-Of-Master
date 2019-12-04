#include "CPluginController.h"
#include "CPluginEnumerator.h"
#include "dlfcn.h"

CPluginController::CPluginController(void)
{
}

CPluginController::~CPluginController(void)
{
}

bool CPluginController::InitializeController(void)
{
	std::vector<std::string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		void *hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY);
		if(hinstLib != NULL) 
		{ 
			m_vhForPlugin.push_back(hinstLib);

			PROC_PRINT DllPrint = (PROC_PRINT)dlsym(hinstLib, "Print");
			PROC_GETID DllGetID = (PROC_GETID)dlsym(hinstLib, "GetID");
			if((NULL != DllPrint) && (NULL != DllGetID))
			{
				m_vPrintFunc.push_back(DllPrint);
				m_vGetIDFunc.push_back(DllGetID);
			}
		}
	}

	return true;
}

bool CPluginController::ProcessRequest(int FunctionID)
{
	for(unsigned int i = 0; i < m_vGetIDFunc.size(); i++)
	{
		if((m_vGetIDFunc[i])() == FunctionID)
		{
			(m_vPrintFunc[i])();
			break;
		}
	}

	return true;
}

bool CPluginController::ProcessHelp(void)
{
	std::vector<std::string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		PROC_HELP DllProc; 

		void *hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY);
		if(hinstLib != NULL) 
		{ 
			DllProc = (PROC_HELP)dlsym(hinstLib, "Help"); 
			if(NULL != DllProc) 
			{
				(DllProc)();
			}

			dlclose(hinstLib);
		}
	}

	return true;
}

bool CPluginController::UninitializeController()
{
	for(unsigned int i = 0; i < m_vhForPlugin.size(); i++)
	{
		dlclose(m_vhForPlugin[i]);
	}

	return true;
}