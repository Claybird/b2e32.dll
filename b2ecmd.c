/*
  B2ECmd.c : B2E()を呼び出すだけの単純なB2E32.dllのラッパ。
  NYSL ver 0.9982 ( http://www.kmonos.net/nysl/ )を適用します。
*/

#define STRICT
#include <stdio.h>
#include <windows.h>

#define PARAM_LENGTH	65535	/* B2E()に渡すコマンドライン文字列のサイズ */
#define BUFFER_LENGTH	65535	/* B2E()の結果を受けとるバッファのサイズ */

#define ERROR_START				0x8000

typedef int   (WINAPI *_B2EDLL_HANDLER)(const HWND,LPCSTR,LPSTR,const DWORD);
typedef WORD  (WINAPI *_B2EDLL_GETVERSION)(VOID);
typedef BOOL  (WINAPI *_B2EDLL_SETSCRIPTDIRECTORY)(LPCSTR);

void dispUsage(void);

int main(int argc,char *argv[])
{
	HINSTANCE	hInstDLL;		/* B2E32.dllのインスタンス */
	int		i;
	int		iRet;								/* B2E()の戻り値 */
	BOOL	bSearchFlag;						/* B2ECmd.exe独自のコマンドラインオプションを探すならTRUE */
	char	szCmdLine[PARAM_LENGTH+1]={0};		/* B2E()に渡すコマンドライン文字列 */
	char	szScriptDirectory[_MAX_PATH+1]={0};	/* B2Eスクリプトを探すパス */
	char	szReturnBuffer[BUFFER_LENGTH+1]={0};/* B2E()の結果を受けとるバッファ */

	/*----------------*/
	/* 関数のアドレス */
	/*----------------*/
	_B2EDLL_HANDLER				funcB2E;				/* B2E() */
	_B2EDLL_GETVERSION			funcGetVersion;			/* B2EGetVersion()*/
	_B2EDLL_GETVERSION			funcGetSubVersion;		/* B2EGetSubVersion() */
	_B2EDLL_SETSCRIPTDIRECTORY	funcSetScriptDirectory;	/* B2ESetScriptDirectory() */

	/* 引数チェック */
	if(argc<2){
		/* 引数が足りないので使用法を表示して終了 */
		dispUsage();
		return 1;
	}

	/* オプション解釈 & コマンドライン文字列組み立て */
	bSearchFlag=TRUE;
	for(i=1;i<argc;i++){
		/*------------------------------------*/
		/* 独自コマンドラインオプションを探す */
		/*------------------------------------*/
		if(bSearchFlag && '-'==argv[i][0]){
			if(strncmp("-dir",argv[i]+1,3)==0){
				strncpy(szScriptDirectory,argv[i]+3,_MAX_PATH);
				bSearchFlag=FALSE;	/* これ以降は独自オプションを探す必要はない */
				continue;
			}else if('-'==argv[i][1]){
				bSearchFlag=FALSE;	/* これ以降はコマンドラインオプションではない */
			}
		}
		/*------------------------------------*/
		/*       コマンドライン組み立て       */
		/*------------------------------------*/
		strncat(szCmdLine,"\"",PARAM_LENGTH);
		strncat(szCmdLine,argv[i],PARAM_LENGTH);
		strncat(szCmdLine,"\" ",PARAM_LENGTH);
	}

	/* DLLのロード */
	hInstDLL=LoadLibrary("B2E32.dll");
	if(!hInstDLL){
		/* ロードに失敗 */
		fprintf(stderr,"Error:B2E32.dllのロードに失敗しました\n");
		return -1;
	}
	/* 関数を探す */
	funcB2E=(_B2EDLL_HANDLER)GetProcAddress(hInstDLL,"B2E");
	funcGetVersion=(_B2EDLL_GETVERSION)GetProcAddress(hInstDLL,"B2EGetVersion");
	funcGetSubVersion=(_B2EDLL_GETVERSION)GetProcAddress(hInstDLL,"B2EGetSubVersion");
	funcSetScriptDirectory=(_B2EDLL_SETSCRIPTDIRECTORY)GetProcAddress(hInstDLL,"B2ESetScriptDirectory");
	if(!funcB2E||!funcGetVersion||!funcGetSubVersion||!funcSetScriptDirectory){
		/* いずれかの関数が見つからない */
		fprintf(stderr,"Error:関数が見つかりません\n");
		FreeLibrary(hInstDLL);
		return -2;
	}

	/* B2E32.dllのバージョン表示 */
	printf("B2E32.dll ver.%d.%02d.%d.%02d\n",funcGetVersion()/100,funcGetVersion()%100,funcGetSubVersion()/100,funcGetSubVersion()%100);

	/* 必要ならB2Eスクリプトのディレクトリを指定 */
	if(szScriptDirectory[0]){
		if(!funcSetScriptDirectory(szScriptDirectory)){
			fprintf(stderr,"Error:B2Eディレクトリの指定に失敗しました\n");
			FreeLibrary(hInstDLL);
			return -3;
		}
		printf("B2Eのディレクトリは \'%s\'\n",szScriptDirectory);
	}

	/* 呼び出しコマンドライン確認 */
	printf("CmdLine=%s\n",szCmdLine);

	/*---------------*/
	/* B2E()呼び出し */
	/*---------------*/
	iRet=funcB2E(NULL,szCmdLine,szReturnBuffer,BUFFER_LENGTH);


	printf("---Log---\n"
		"%s\n"
		"---End---\n"
		"Return Value=0x%x\n",szReturnBuffer,iRet);
	if(iRet>=ERROR_START){
		printf("B2E32.dllでエラーが起きたようです。\n");
	}

	/* 後始末 */
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
		"  -c<format> : 圧縮を指示。形式指定必須\n"
		"    ex : -cDGCA\n"
		"  -a : ファイルの追加を指示。形式指定なし\n"
		"    ex : -a archive.zip *.txt\n"
		"  -d : ファイルの削除を指示\n"
		"    ex : -d archive.zip *.html\n"
		"  -m<method> : [圧縮時/追加時のみ]圧縮メソッドを指定。省略可\n"
		"    ex : -mRecoveryData\n"
		"  -s : [圧縮時のみ]可能なら自己解凍形式で圧縮\n"
		"  -e : 解凍\n"
		"  -g : [解凍時のみ]出力ファイルのパス情報を無視\n"
		"  -l : 圧縮ファイル中のファイルを列挙\n"
		"  -o<dir> : [解凍時のみ]出力先指定。指定を推奨\n"
		"    ex : -oC:\\Outputs\\\n"
		"  -@<resp> : レスポンスファイル指定\n"
		"    ex : -@C:\\temp\\resp.txt\n"
		"  -- : 以降のパラメータを全てファイル名と見なす\n\n"

		"空白文字を含むパス名などを指定する際には、オプション全体を\"\"で括ってください。\n"
		"    ex : \"-@C:\\test file\\resp.txt\"\n\n"

		"B2ECmd.exe 独自コマンド:\n"
		"  --dir<dir> : B2Eのディレクトリを指定\n"

		);
}
