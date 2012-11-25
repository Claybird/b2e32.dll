/*
  B2ECmd.c : B2E()���Ăяo�������̒P����B2E32.dll�̃��b�p�B
  NYSL ver 0.9982 ( http://www.kmonos.net/nysl/ )��K�p���܂��B
*/

#define STRICT
#include <stdio.h>
#include <windows.h>

#define PARAM_LENGTH	65535	/* B2E()�ɓn���R�}���h���C��������̃T�C�Y */
#define BUFFER_LENGTH	65535	/* B2E()�̌��ʂ��󂯂Ƃ�o�b�t�@�̃T�C�Y */

#define ERROR_START				0x8000

typedef int   (WINAPI *_B2EDLL_HANDLER)(const HWND,LPCSTR,LPSTR,const DWORD);
typedef WORD  (WINAPI *_B2EDLL_GETVERSION)(VOID);
typedef BOOL  (WINAPI *_B2EDLL_SETSCRIPTDIRECTORY)(LPCSTR);

void dispUsage(void);

int main(int argc,char *argv[])
{
	HINSTANCE	hInstDLL;		/* B2E32.dll�̃C���X�^���X */
	int		i;
	int		iRet;								/* B2E()�̖߂�l */
	BOOL	bSearchFlag;						/* B2ECmd.exe�Ǝ��̃R�}���h���C���I�v�V������T���Ȃ�TRUE */
	char	szCmdLine[PARAM_LENGTH+1]={0};		/* B2E()�ɓn���R�}���h���C�������� */
	char	szScriptDirectory[_MAX_PATH+1]={0};	/* B2E�X�N���v�g��T���p�X */
	char	szReturnBuffer[BUFFER_LENGTH+1]={0};/* B2E()�̌��ʂ��󂯂Ƃ�o�b�t�@ */

	/*----------------*/
	/* �֐��̃A�h���X */
	/*----------------*/
	_B2EDLL_HANDLER				funcB2E;				/* B2E() */
	_B2EDLL_GETVERSION			funcGetVersion;			/* B2EGetVersion()*/
	_B2EDLL_GETVERSION			funcGetSubVersion;		/* B2EGetSubVersion() */
	_B2EDLL_SETSCRIPTDIRECTORY	funcSetScriptDirectory;	/* B2ESetScriptDirectory() */

	/* �����`�F�b�N */
	if(argc<2){
		/* ����������Ȃ��̂Ŏg�p�@��\�����ďI�� */
		dispUsage();
		return 1;
	}

	/* �I�v�V�������� & �R�}���h���C��������g�ݗ��� */
	bSearchFlag=TRUE;
	for(i=1;i<argc;i++){
		/*------------------------------------*/
		/* �Ǝ��R�}���h���C���I�v�V������T�� */
		/*------------------------------------*/
		if(bSearchFlag && '-'==argv[i][0]){
			if(strncmp("-dir",argv[i]+1,3)==0){
				strncpy(szScriptDirectory,argv[i]+3,_MAX_PATH);
				bSearchFlag=FALSE;	/* ����ȍ~�͓Ǝ��I�v�V������T���K�v�͂Ȃ� */
				continue;
			}else if('-'==argv[i][1]){
				bSearchFlag=FALSE;	/* ����ȍ~�̓R�}���h���C���I�v�V�����ł͂Ȃ� */
			}
		}
		/*------------------------------------*/
		/*       �R�}���h���C���g�ݗ���       */
		/*------------------------------------*/
		strncat(szCmdLine,"\"",PARAM_LENGTH);
		strncat(szCmdLine,argv[i],PARAM_LENGTH);
		strncat(szCmdLine,"\" ",PARAM_LENGTH);
	}

	/* DLL�̃��[�h */
	hInstDLL=LoadLibrary("B2E32.dll");
	if(!hInstDLL){
		/* ���[�h�Ɏ��s */
		fprintf(stderr,"Error:B2E32.dll�̃��[�h�Ɏ��s���܂���\n");
		return -1;
	}
	/* �֐���T�� */
	funcB2E=(_B2EDLL_HANDLER)GetProcAddress(hInstDLL,"B2E");
	funcGetVersion=(_B2EDLL_GETVERSION)GetProcAddress(hInstDLL,"B2EGetVersion");
	funcGetSubVersion=(_B2EDLL_GETVERSION)GetProcAddress(hInstDLL,"B2EGetSubVersion");
	funcSetScriptDirectory=(_B2EDLL_SETSCRIPTDIRECTORY)GetProcAddress(hInstDLL,"B2ESetScriptDirectory");
	if(!funcB2E||!funcGetVersion||!funcGetSubVersion||!funcSetScriptDirectory){
		/* �����ꂩ�̊֐���������Ȃ� */
		fprintf(stderr,"Error:�֐���������܂���\n");
		FreeLibrary(hInstDLL);
		return -2;
	}

	/* B2E32.dll�̃o�[�W�����\�� */
	printf("B2E32.dll ver.%d.%02d.%d.%02d\n",funcGetVersion()/100,funcGetVersion()%100,funcGetSubVersion()/100,funcGetSubVersion()%100);

	/* �K�v�Ȃ�B2E�X�N���v�g�̃f�B���N�g�����w�� */
	if(szScriptDirectory[0]){
		if(!funcSetScriptDirectory(szScriptDirectory)){
			fprintf(stderr,"Error:B2E�f�B���N�g���̎w��Ɏ��s���܂���\n");
			FreeLibrary(hInstDLL);
			return -3;
		}
		printf("B2E�̃f�B���N�g���� \'%s\'\n",szScriptDirectory);
	}

	/* �Ăяo���R�}���h���C���m�F */
	printf("CmdLine=%s\n",szCmdLine);

	/*---------------*/
	/* B2E()�Ăяo�� */
	/*---------------*/
	iRet=funcB2E(NULL,szCmdLine,szReturnBuffer,BUFFER_LENGTH);


	printf("---Log---\n"
		"%s\n"
		"---End---\n"
		"Return Value=0x%x\n",szReturnBuffer,iRet);
	if(iRet>=ERROR_START){
		printf("B2E32.dll�ŃG���[���N�����悤�ł��B\n");
	}

	/* ��n�� */
	FreeLibrary(hInstDLL);

	return 0;
}

void dispUsage(void)
{
	printf(
		"B2ECmd.exe -- A Command Line Wrapper for B2E32.dll\n"
		"Usage:\n"
		"B2ECmd.exe -<command> <archive_file_name> [<path_name>|-@<resp>]\n\n"

		"-<command>:\n"
		"  -c<format> : ���k���w���B�`���w��K�{\n"
		"    ex : -cDGCA\n"
		"  -a : �t�@�C���̒ǉ����w���B�`���w��Ȃ�\n"
		"    ex : -a archive.zip *.txt\n"
		"  -d : �t�@�C���̍폜���w��\n"
		"    ex : -d archive.zip *.html\n"
		"  -m<method> : [���k��/�ǉ����̂�]���k���\�b�h���w��B�ȗ���\n"
		"    ex : -mRecoveryData\n"
		"  -s : [���k���̂�]�\�Ȃ玩�ȉ𓀌`���ň��k\n"
		"  -e : ��\n"
		"  -g : [�𓀎��̂�]�o�̓t�@�C���̃p�X���𖳎�\n"
		"  -l : ���k�t�@�C�����̃t�@�C�����\n"
		"  -o<dir> : [�𓀎��̂�]�o�͐�w��B�w��𐄏�\n"
		"    ex : -oC:\\Outputs\\\n"
		"  -@<resp> : ���X�|���X�t�@�C���w��\n"
		"    ex : -@C:\\temp\\resp.txt\n"
		"  -- : �ȍ~�̃p�����[�^��S�ăt�@�C�����ƌ��Ȃ�\n\n"

		"�󔒕������܂ރp�X���Ȃǂ��w�肷��ۂɂ́A�I�v�V�����S�̂�\"\"�Ŋ����Ă��������B\n"
		"    ex : \"-@C:\\test file\\resp.txt\"\n\n"

		"B2ECmd.exe �Ǝ��R�}���h:\n"
		"  --dir<dir> : B2E�̃f�B���N�g�����w��\n"

		);
}
