#include <iostream>

#include "CPluginController.h"

#include <stdlib.h>

#include <string.h>

using namespace std;

int main(int argc, char **argv)

{

	if(argc == 2)
	{
		if(strcmp(argv[1], "help") == 0)//帮助功能
		{
			CPluginController pc;

			pc.ProcessHelp();

			return 0;
		}
		else
		{
			int FunctionID = atoi(argv[1]);

			CPluginController pc;

			pc.InitializeController();

			pc.ProcessRequest(FunctionID);

			pc.UninitializeController();

			return 0;
		}
	}
	else if(argc == 3)
	{
		CPluginController pc;

		char *Function = argv[1];

		//Function += ".so";

		char *Document = argv[2];//操作的文件名

		pc.InitializeController();

		if(pc.IfProcess(Function) == false)//判断插件是否存在
		{
			cout << "No this plugin!" << endl;
		}
		else
		{
			pc.ProcessFunction(Function,Document);
		};

		pc.UninitializeController();
		return 0;
	}
	else
	{
		cout << "Parameters error" << endl;
		return 0;
	}

}
