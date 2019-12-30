#include <iostream>
#include "IPrintPlugin.h"
#include <unistd.h>//read(file)
#include <fcntl.h>
#include <string.h>

using namespace std;

const int FUNC_ID = 3;

const char FUNC_NAME[]="sl";//statistics line!

class CPrintPlugin : public IPrintPlugin

{

public:

	CPrintPlugin()
	{
	}

	virtual ~CPrintPlugin()
	{
	}

	virtual void Print()
	{
		cout << "Statistics the document line!" << endl;
	}

	virtual void Help()
	{
		cout << "Function ID " << FUNC_ID << " : This function will statistics the document line." << endl;
	}

	virtual int GetID(void)
	{
		return FUNC_ID;
	}

	virtual char *GetName()
	{
		return FUNC_NAME;
	}

	virtual void Fun(char *Document)
	{

		int fp;
		char temp;
		long num=0;//统计

		//open file

		if((fp=open(Document,O_RDONLY))==-1)
		{
			cout << "Can not open: " << Document << endl;
			return;
		};

		while(read(fp,&temp,1))
		{
			if(temp=='\n')
			{
				num++;
			}
		};

		close(fp);

		cout << Document << "lines is: " << num << endl;
	}
};

extern "C" void CreateObj(IPrintPlugin **ppPlugin)
{

	static CPrintPlugin plugin;

	*ppPlugin = &plugin;

}
