// main.cpp : DLL アプリケーション用のエントリ ポイントを定義します。

#include "stdafx.h"
#include "main.h"

extern "C" BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		//DLL が初めてプロセスのメモリ空間にマッピングされる時に渡されます。
		kilib_startUp((HMODULE)hModule);
		break;
	case DLL_THREAD_ATTACH:
		//DLLがアタッチしているプロセスで新しいスレッドが作成された時に渡されます。
		break;
	case DLL_THREAD_DETACH:
		//実行中のスレッドが終了する時に渡されます。
		break;
	case DLL_PROCESS_DETACH:
		//実行中のプロセスがDLLを解放する時に渡されます。
		kilib_shutDown();	//delete app()まで行う
		break;
	}
    return TRUE;
}

void kilib_create_new_app(HMODULE hModule)
{
	//-- kilib にアプリケーションを設定
	new CMain(hModule);
}
