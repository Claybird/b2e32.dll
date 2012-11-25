// main.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B

#include "stdafx.h"
#include "main.h"

extern "C" BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		//DLL �����߂ăv���Z�X�̃�������ԂɃ}�b�s���O����鎞�ɓn����܂��B
		kilib_startUp((HMODULE)hModule);
		break;
	case DLL_THREAD_ATTACH:
		//DLL���A�^�b�`���Ă���v���Z�X�ŐV�����X���b�h���쐬���ꂽ���ɓn����܂��B
		break;
	case DLL_THREAD_DETACH:
		//���s���̃X���b�h���I�����鎞�ɓn����܂��B
		break;
	case DLL_PROCESS_DETACH:
		//���s���̃v���Z�X��DLL��������鎞�ɓn����܂��B
		kilib_shutDown();	//delete app()�܂ōs��
		break;
	}
    return TRUE;
}

void kilib_create_new_app(HMODULE hModule)
{
	//-- kilib �ɃA�v���P�[�V������ݒ�
	new CMain(hModule);
}
