#ifndef __MAIN_H__
#define __MAIN_H__
#include "NoahAM.h"

#	define myapp() (*(CMain*)app())

#define DLL_NAME "B2E32.dll"
#define DLL_VERSION "B2E32.dll Ver.0.07"

class CMain:public kiApp
{
	//-- Main�I�u�W�F�N�g�擾�p�}�N��
public:
	CMain(HMODULE hModule){m_hInst=hModule;m_ArchiverManager.init(NULL);}
	virtual ~CMain(){}

	void run(){/*m_ArchiverManager.init(NULL);*/}

	bool init();
	CNoahArchiverManager m_ArchiverManager;
};

#endif //__MAIN_H__
