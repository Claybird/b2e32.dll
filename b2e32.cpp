// WINAPI _export B2E32.cpp : エクスポートされた関数の実体を記述します
//

#include "stdafx.h"
#include "b2e32.h"
#include "main.h"
#include "OpenArchiveData.h"
#include "apisupport.h"
#include "resource.h"
#include "lhafutil.h"

WORD WINAPI _export B2EGetVersion(VOID){
	return B2E32_VERSION;
}

static LONG s_RunningCount=0;

BOOL WINAPI _export B2EGetRunning(VOID)
{
	return 0!=s_RunningCount;
}

//--------------------------------
// 一応値は保存するが、意味はない
//--------------------------------
static BOOL s_BackGroundMode=FALSE;
BOOL WINAPI _export B2EGetBackGroundMode(VOID){return s_BackGroundMode;}
BOOL WINAPI _export B2ESetBackGroundMode(const BOOL _BackGroundMode){s_BackGroundMode=_BackGroundMode;return TRUE;}
static BOOL s_CursorMode=FALSE;
BOOL WINAPI _export B2EGetCursorMode(VOID){return s_CursorMode;}
BOOL WINAPI _export B2ESetCursorMode(const BOOL _CursorMode){s_CursorMode=_CursorMode;return TRUE;}
static WORD s_Interval=80;
WORD WINAPI _export B2EGetCursorInterval(VOID){return s_Interval;}
BOOL WINAPI _export B2ESetCursorInterval(const WORD _Interval){s_Interval=_Interval;return TRUE;}

//------
// 本体
//------
int WINAPI _export B2E(const HWND _hwnd, LPCSTR _szCmdLine,LPSTR _szOutput, const DWORD _dwSize)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;

	InterlockedIncrement(&s_RunningCount);
	int ret=0;

	if(!_szCmdLine){
		ret=ERROR_COMMAND_NAME;
	}else{
		//通常処理
		kiCmdParser Parser(_szCmdLine,false);
		kiStr strLog(DLL_VERSION "\r\n");
		bool bForceBreak=false;

		enum{UNKNOWN,COMPRESS,EXTRACT,LIST,ADD,DEL};
		int mode=UNKNOWN;		//圧縮モード
		bool bSFX=false;		//自己解凍形式で圧縮するならtrue
		bool bIgnorePath=false;	//展開時パス情報を無視するならtrue
		LPCSTR pCompressType=NULL;	//圧縮タイプ
		LPCSTR pOutputDir=NULL;		//出力先ディレクトリ
		LPCSTR pMethod=NULL;			//圧縮メソッド
		LPCSTR pResponceFile=NULL;	//レスポンスファイル
		for(unsigned int i=0;i<Parser.option().len();i++){
			const char* pOpt=&Parser.option()[i][1];
			switch(*pOpt){
			case 'c':				//FALLTHROUGH
			case 'C':	//圧縮(形式指定)
				pOpt++;
				if('\0'==*pOpt||' '==*pOpt||'\t'==*pOpt){
					//圧縮形式未指定
					strLog+="Option Error:\r\n"
						"Compression type is not specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else if(mode!=UNKNOWN){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pCompressType=pOpt;
					mode=COMPRESS;
				}
				break;
			case 'a':				//FALLTHROUGH
			case 'A':	//追加(形式指定)
				pOpt++;
				if(mode!=UNKNOWN){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pCompressType=NULL;
					mode=ADD;
				}
				break;
			case 'd':				//FALLTHROUGH
			case 'D':	//削除
				pOpt++;
				if(mode!=UNKNOWN){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pCompressType=NULL;
					mode=DEL;
				}
				break;
			case 'e':				//FALLTHROUGH
			case 'E':	//解凍
				 if(mode!=UNKNOWN){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					mode=EXTRACT;
				}
				break;
			case 'l':				//FALLTHROUGH
			case 'L':	//ファイルリスト表示
				 if(mode!=UNKNOWN){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					mode=LIST;
				}
				break;
			case 'o':				//FALLTHROUGH
			case 'O':	//出力先ディレクトリ指定
				pOpt++;
				if('\0'==*pOpt||' '==*pOpt||'\t'==*pOpt){
					//ディレクトリ未指定
					strLog+="Option Error:\r\n"
						"Output directory is not specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pOutputDir=pOpt;
				}
				break;
			case 'm':				//FALLTHROUGH
			case 'M':	//圧縮メソッド指定
				pOpt++;
				if('\0'==*pOpt||' '==*pOpt||'\t'==*pOpt){
					//圧縮メソッド未指定
					strLog+="Option Error:\r\n"
						"Compression method is not specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else if(mode!=COMPRESS && mode!=ADD){	//モード異常
					strLog+="Option Error:\r\n"
						"Action type is not specified or is not \'compress\' mode\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pMethod=pOpt;
				}
				break;
			case 's':				//FALLTHROUGH
			case 'S':	//自己解凍
				if(bSFX){
					//既に自己解凍のスイッチがついている
					strLog+="Option Error:\r\n"
						"Self-Extract mode has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}
				else bSFX=true;
				break;
			case 'g':				//FALLTHROUGH
			case 'G':	//パス情報指定無視
				if(bIgnorePath){
					//既にパス情報無視のスイッチがついている
					strLog+="Option Error:\r\n"
						"Path-Ignore mode has been already specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}
				else bIgnorePath=true;
				break;
			case '@':	//レスポンスファイル指定(-@)
				pOpt++;
				if('\0'==*pOpt||' '==*pOpt||'\t'==*pOpt){
					//ファイル未指定
					strLog+="Option Error:\r\n"
						"Responce file is not specified\r\n";
					bForceBreak=true;
					ret=ERROR_COMMAND_NAME;
				}else{
					//正常
					pResponceFile=pOpt;
				}
				break;
			case '-':	//End of Switch
				//Do nothing
				break;
			default:
				//不明なコマンドライン
				strLog+="Illegal Option:\r\n"
					"Option \'-";
				strLog+=pOpt;
				strLog+="\' is unknown\r\n";
				bForceBreak=true;
				ret=ERROR_COMMAND_NAME;
				break;
			}
			if(bForceBreak)break;
		}
		if(!bForceBreak){
			StrArray fileList;
			if(pResponceFile){
				if(!UtilReadFromResponceFile(pResponceFile,fileList)){
					strLog+="Read Error:\r\n"
						"An error occured while reading responce file\r\n";
					ret=ERROR_RESPONSE_READ;
				}
			}
			if(ret==0){
				//対象ファイルの列挙など
				const int totallen=Parser.param().len()+fileList.len();	//処理対象ファイルの数
				if(0==totallen){
					strLog+="Option Error:\r\n"
						"No file is specified\r\n";
					ret=ERROR_COMMAND_NAME;
				}else if(1==totallen&&mode==COMPRESS){
					strLog+="Option Error:\r\n"
						"Source file is not specified\r\n";
					ret=ERROR_COMMAND_NAME;
				}else if(mode==LIST){
					//指定されたファイル名は全てアーカイブ名と見なせる
					StrArray fileArray(Parser.param().len()+fileList.len());
					for(unsigned int i=0;i<Parser.param().len()+fileList.len();i++){
						if(i<Parser.param().len()){
							fileArray[i]=Parser.param()[i];
						}else{
							fileArray[i]=fileList[i-Parser.param().len()];
						}
					}
					fileArray.forcelen(Parser.param().len()+fileList.len());
					if(fileArray.len()==0){
						strLog+="Listing Error:\r\n"
							"Archive file is not specified\r\n";
						ret=ERROR_COMMAND_NAME;
					}else{
						ret=myapp().m_ArchiverManager.listFileInArchives(fileArray,strLog);
					}
				}else{
					LPCSTR pArc=NULL;		//アーカイブファイル名
					if(Parser.param().len()>0)pArc=Parser.param()[0];
					else pArc=fileList[0];

					//targetArrayには圧縮/部分解凍対象のファイル名を格納する
					cCharArray targetArray(Parser.param().len()+fileList.len()-1);
					for(unsigned int i=1;i<Parser.param().len()+fileList.len();i++){
						if(i<Parser.param().len()){
							targetArray[i-1]=Parser.param()[i];
						}else{
							targetArray[i-1]=fileList[i-Parser.param().len()];
						}
					}
					targetArray.forcelen(Parser.param().len()+fileList.len()-1);
					//ファイルを処理
					switch(mode){
					case UNKNOWN:
						strLog+="Option Error:\r\n"
							"Action type is not specified\r\n";
						ret=ERROR_COMMAND_NAME;
						break;
					case COMPRESS:
						ret=myapp().m_ArchiverManager.do_compressing(pArc,targetArray,pCompressType,pMethod,bSFX,strLog);
						break;
					case ADD:
						ret=myapp().m_ArchiverManager.do_adding(pArc,targetArray,pMethod,strLog);
						break;
					case DEL:
						ret=myapp().m_ArchiverManager.do_deleting(pArc,targetArray,strLog);
						break;
					case EXTRACT:
						if(pArc){
							if(targetArray.len()>0){	//アーカイブ内の指定ファイルだけ解凍
								ret=myapp().m_ArchiverManager.do_melting_each(kiPath(pArc),kiPath(pOutputDir?pOutputDir:""),bIgnorePath,targetArray,strLog);
							}
							else{	//アーカイブを全部解凍:ワイルドカード展開を行う
								StrArray files;
								if(!UtilExpandWildcard(pArc,files,true)){	//ワイルドカード展開に失敗
									strLog+="Extraction Error:\r\n"
										"Archive file '";
									strLog+=pArc;
									strLog+="' is not found\r\n";
									ret=ERROR_NOT_FIND_ARC_FILE;
								}else{
									UINT size=files.len();
									for(UINT i=0;i<size;i++){
										strLog+="Extracting Archive : ";
										strLog+=files[i];
										strLog+="\r\n";
										ret=myapp().m_ArchiverManager.do_melting(kiPath(files[i]),kiPath(pOutputDir?pOutputDir:""),bIgnorePath,strLog);
										if(ret){	//エラーor警告が出た
											strLog+="Something is wrong.\r\n";
											break;
										}
										strLog+="\r\n";
									}
								}
							}
						}
						else{
							strLog+="Extraction Error:\r\n"
								"Archive file is not specified\r\n";
							ret=ERROR_COMMAND_NAME;
						}
						break;
					}
				}
			}
		}
		if(_szOutput){
			strncpy_s(_szOutput,_dwSize,strLog,_dwSize);
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

BOOL WINAPI _export B2ECheckArchive(LPCSTR _szFileName, const int _iMode)
{
	if(B2EGetRunning())return FALSE;

	InterlockedIncrement(&s_RunningCount);
	BOOL ret;
	if(!_szFileName||strlen(_szFileName)==0||!kiSUtil::exist(_szFileName)){
		ret=FALSE;
	}else{
		ret=myapp().m_ArchiverManager.checkArchive(kiPath(_szFileName));
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EGetFileCount(LPCSTR _szArcFile)
{
	if(B2EGetRunning()||!_szArcFile)return -1;

	InterlockedIncrement(&s_RunningCount);
	int ret=myapp().m_ArchiverManager.getItemCount(_szArcFile);
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

BOOL WINAPI _export B2EConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,const int _iMode)
{
	if(B2EGetRunning())return FALSE;

	InterlockedIncrement(&s_RunningCount);

	kiStr ver_str;
	myapp().m_ArchiverManager.get_version(ver_str);
	struct CVersionDialog : public kiDialog
	{
		const LPCSTR str;
		CVersionDialog(const kiStr &s)
			: kiDialog( IDD_VERSION ),str(s){}
		BOOL onInit()
			{
				sendMsgToItem( IDC_EDIT_VERSION, WM_SETTEXT, 0, (LPARAM)str );
				return TRUE;
			}
	}VersionDlg(ver_str);
	VersionDlg.doModal(_hwnd);

	InterlockedDecrement(&s_RunningCount);
	return TRUE;
}

BOOL WINAPI _export B2EQueryFunctionList(const int _iFunction)
{
	if(_iFunction<ISARC_FUNCTION_START||ISARC_FUNCTION_END<_iFunction)return false;
	return s_bAPISupportTable[_iFunction];
}

BOOL WINAPI _export B2ESetOwnerWindow(const HWND _hwnd)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EClearOwnerWindow(VOID)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2ESetOwnerWindowEx(const HWND _hwnd,LPARCHIVERPROC _lpArcProc)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EKillOwnerWindowEx(const HWND _hwnd)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2ESetOwnerWindowEx64(const HWND _hwnd,LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EKillOwnerWindowEx64(const HWND _hwnd)
{
	//中身無し
	return FALSE;
}

//OpenArchive系

HARC WINAPI _export B2EOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode)
{
	if(B2EGetRunning())return (HARC)NULL;
	if(!_szFileName)return (HARC)NULL;

	InterlockedIncrement(&s_RunningCount);

	COpenArchiveData *lpData=new COpenArchiveData(myapp().m_ArchiverManager);
	if(!lpData->ScanArchive(_szFileName)){
		//書庫内容取得
		delete lpData;
		lpData=NULL;
	}

	InterlockedDecrement(&s_RunningCount);
	return (HARC)lpData;
}

HARC WINAPI _export B2EOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode, LPCSTR _szOption)
{
	//中身無し
	return NULL;
}

int WINAPI _export B2ECloseArchive(HARC _harc)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		delete lpData;
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EFindFirst(HARC _harc, LPCSTR _szWildName,INDIVIDUALINFO *_lpSubInfo)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		if(!lpData->FindFirstFile()){
			ret=-1;	//NOT FOUND
		}else{
			if(_lpSubInfo){
				arcfile afile;
				if(!lpData->GetItemData(afile)){
					ret=ERROR_HARC_ISNOT_OPENED;
				}else{
					memcpy(_lpSubInfo,&afile.inf,sizeof(afile.inf));
				}
			}
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo)
{
	if(B2EGetRunning())return -1;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		if(!lpData->FindNextFile()){
			ret=-1;	//NOT FOUND
		}else{
			if(_lpSubInfo){
				arcfile afile;
				if(!lpData->GetItemData(afile)){
					ret=ERROR_HARC_ISNOT_OPENED;
				}else{
					memcpy(_lpSubInfo,&afile.inf,sizeof(afile.inf));
				}
			}
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EGetArcFileName(HARC _harc, LPSTR _lpBuffer,const int _nSize)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;
	if(!_lpBuffer)return ERROR_ENOUGH_MEMORY;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		strncpy_s(_lpBuffer,_nSize,lpData->GetArchiveName(),_nSize);
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

DWORD WINAPI _export B2EGetArcFileSize(HARC _harc)
{
	//中身無し
	return -1;
}

DWORD WINAPI _export B2EGetArcOriginalSize(HARC _harc)
{
	//中身無し
	return -1;
}

DWORD WINAPI _export B2EGetArcCompressedSize(HARC _harc)
{
	//中身無し
	return -1;
}

BOOL WINAPI _export B2EGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//中身無し
	return FALSE;
}

DWORD WINAPI _export B2EGetArcReadSize(HARC _harc)
{
	//中身無し
	return -1;
}

BOOL WINAPI _export B2EGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//中身無し
	return FALSE;
}

WORD WINAPI _export B2EGetArcRatio(HARC _harc)
{
	//中身無し
	return -1;
}

WORD WINAPI _export B2EGetArcDate(HARC _harc)
{
	//中身無し
	return -1;
}

WORD WINAPI _export B2EGetArcTime(HARC _harc)
{
	//中身無し
	return -1;
}
BOOL WINAPI _export B2EGetArcWriteTimeEx(HARC _harc,FILETIME *_lpftLastWriteTime)
{
	//中身無し
	return FALSE;
}
BOOL WINAPI _export B2EGetArcCreateTimeEx(HARC _harc,FILETIME *_lpftCreationTime)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EGetArcAccessTimeEx(HARC _harc,FILETIME *_lpftLastAccessTime)
{
	//中身無し
	return FALSE;
}

UINT WINAPI _export B2EGetArcOSType(HARC _harc)
{
	//中身無し
	return -1;
}

int WINAPI _export B2EIsSFXFile(HARC _harc)
{
	//中身無し
	return -1;
}

int WINAPI _export B2EGetFileName(HARC _harc, LPSTR _lpBuffer,const int _nSize)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;
	if(!_lpBuffer)return ERROR_ENOUGH_MEMORY;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		bool r=lpData->GetItemData(afile);
		if(!r){
			ret=ERROR_HARC_ISNOT_OPENED;
		}else{
			strncpy_s(_lpBuffer,_nSize,afile.inf.szFileName,_nSize);
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EGetMethod(HARC _harc, LPSTR _lpBuffer,const int _nSize)
{
	if(B2EGetRunning())return ERROR_ALREADY_RUNNING;
	if(!_lpBuffer)return ERROR_ENOUGH_MEMORY;

	int ret=0;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=ERROR_HARC_ISNOT_OPENED;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=ERROR_HARC_ISNOT_OPENED;
		}else{
			ret=0;
			strncpy_s(_lpBuffer,_nSize,afile.inf.szMode,_nSize);
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

DWORD WINAPI _export B2EGetOriginalSize(HARC _harc)
{
	if(B2EGetRunning())return -1;

	DWORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.dwOriginalSize;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

DWORD WINAPI _export B2EGetCompressedSize(HARC _harc)
{
	if(B2EGetRunning())return -1;

	DWORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.dwCompressedSize;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

BOOL WINAPI _export B2EGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//実装しない
	return FALSE;
}

BOOL WINAPI _export B2EGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	//実装しない
	return FALSE;
}

WORD WINAPI _export B2EGetRatio(HARC _harc)
{
	if(B2EGetRunning())return -1;

	WORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.wRatio;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

WORD WINAPI _export B2EGetDate(HARC _harc)
{
	if(B2EGetRunning())return -1;

	WORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.wDate;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

WORD WINAPI _export B2EGetTime(HARC _harc)
{
	if(B2EGetRunning())return -1;

	WORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.wTime;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

DWORD WINAPI _export B2EGetWriteTime(HARC _harc)
{
	return -1;
}

DWORD WINAPI _export B2EGetAccessTime(HARC _harc)
{
	return -1;
}

DWORD WINAPI _export B2EGetCreateTime(HARC _harc)
{
	return -1;
}

BOOL WINAPI _export B2EGetWriteTimeEx(HARC _harc,FILETIME *_lpftLastWriteTime)
{
	//実装しない
	return FALSE;
}

BOOL WINAPI _export B2EGetCreateTimeEx(HARC _harc,FILETIME *_lpftCreationTime)
{
	//実装しない
	return FALSE;
}

BOOL WINAPI _export B2EGetAccessTimeEx(HARC _harc,FILETIME *_lpftLastAccessTime)
{
	//実装しない
	return FALSE;
}

DWORD WINAPI _export B2EGetCRC(HARC _harc)
{
	if(B2EGetRunning())return -1;

	DWORD ret=-1;
	InterlockedIncrement(&s_RunningCount);
	if(!_harc){
		//無効なハンドル
		ret=-1;
	}else{
		COpenArchiveData *lpData=(COpenArchiveData*)_harc;
		arcfile afile;
		if(!lpData->GetItemData(afile)){
			ret=-1;
		}else{
			ret=afile.inf.dwCRC;
		}
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

int WINAPI _export B2EGetAttribute(HARC _harc)
{
	//丸投げ
	return B2EGetAttributes(_harc);
}

int WINAPI _export B2EGetAttributes(HARC _harc)
{
	//属性が文字で格納されているので判定できない！
	return -1;
}

UINT WINAPI _export B2EGetOSType(HARC _harc)
{
	//中身無し
	return -1;
}

WORD WINAPI _export B2EGetSubVersion(VOID){
	return B2E32_SUB_VERSION;
}

int WINAPI _export B2EExtractMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,
		time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize)
{
	//中身無し
	return ERROR_NOT_SUPPORT;
}
int WINAPI _export B2ECompressMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,
		const time_t *_lpTime, const LPWORD _lpwAttr,
		LPDWORD _lpdwWriteSize)
{
	//中身無し
	return ERROR_NOT_SUPPORT;
}

BOOL WINAPI _export B2ESetEnumMembersProc(B2E_WND_ENUMMEMBPROC _lpEnumProc)
{
	//中身無し
	return FALSE;
}
BOOL WINAPI _export B2EClearEnumMembersProc(VOID)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2ESetEnumMembersProc64(B2E_WND_ENUMMEMBPROC _lpEnumProc, DWORD _dwStructSize)
{
	//中身無し
	return FALSE;
}

BOOL WINAPI _export B2EClearEnumMembersProc64(VOID)
{
	//中身無し
	return FALSE;
}



/**************************/
/* 互換性のための別名関数 */
/**************************/

/* ### LHA.DLL Ver 1.1 と互換性のある API 群です。### */
/* LHA.DLL compatible API */

WORD WINAPI _export BtoEGetVersion(VOID){return B2EGetVersion();}
BOOL WINAPI _export BtoEGetRunning(VOID){return B2EGetRunning();}
BOOL WINAPI _export BtoEGetBackGroundMode(VOID){return B2EGetBackGroundMode();}
BOOL WINAPI _export BtoESetBackGroundMode(const BOOL _BackGroundMode){return B2ESetBackGroundMode(_BackGroundMode);}
BOOL WINAPI _export BtoEGetCursorMode(VOID){return B2EGetCursorMode();}
BOOL WINAPI _export BtoESetCursorMode(const BOOL _CursorMode){return B2ESetCursorMode(_CursorMode);}
WORD WINAPI _export BtoEGetCursorInterval(VOID){return B2EGetCursorInterval();}
BOOL WINAPI _export BtoESetCursorInterval(const WORD _Interval){return B2ESetCursorInterval(_Interval);}
int WINAPI _export BtoE(const HWND _hwnd, LPCSTR _szCmdLine,LPSTR _szOutput, const DWORD _dwSize){return B2E(_hwnd,_szCmdLine,_szOutput,_dwSize);}
BOOL WINAPI _export BtoECheckArchive(LPCSTR _szFileName, const int _iMode){return B2ECheckArchive(_szFileName,_iMode);}
int WINAPI _export BtoEGetFileCount(LPCSTR _szArcFile){return B2EGetFileCount(_szArcFile);}
BOOL WINAPI _export BtoEConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,const int _iMode){return B2EConfigDialog(_hwnd,_lpszComBuffer,_iMode);}
BOOL WINAPI _export BtoEQueryFunctionList(const int _iFunction){return B2EQueryFunctionList(_iFunction);}
BOOL WINAPI _export BtoESetOwnerWindow(const HWND _hwnd){return B2ESetOwnerWindow(_hwnd);}
BOOL WINAPI _export BtoEClearOwnerWindow(VOID){return B2EClearOwnerWindow();}
BOOL WINAPI _export BtoESetOwnerWindowEx(const HWND _hwnd,LPARCHIVERPROC _lpArcProc){return B2ESetOwnerWindowEx(_hwnd,_lpArcProc);}
BOOL WINAPI _export BtoEKillOwnerWindowEx(const HWND _hwnd){return B2EKillOwnerWindowEx(_hwnd);}
BOOL WINAPI _export BtoESetOwnerWindowEx64(const HWND _hwnd,LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize){return B2ESetOwnerWindowEx64(_hwnd,_lpArcProc,_dwStructSize);}
BOOL WINAPI _export BtoEKillOwnerWindowEx64(const HWND _hwnd){return B2EKillOwnerWindowEx64(_hwnd);}

/* OpenArchive 系 API です。 */

HARC WINAPI _export BtoEOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode){return B2EOpenArchive(_hwnd,_szFileName,_dwMode);}
HARC WINAPI _export BtoEOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode, LPCSTR _szOption){return B2EOpenArchive2(_hwnd,_szFileName,_dwMode,_szOption);}
int WINAPI _export BtoECloseArchive(HARC _harc){return B2ECloseArchive(_harc);}
int WINAPI _export BtoEFindFirst(HARC _harc, LPCSTR _szWildName,INDIVIDUALINFO *_lpSubInfo){return B2EFindFirst(_harc, _szWildName,_lpSubInfo);}
int WINAPI _export BtoEFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo){return B2EFindNext(_harc,_lpSubInfo);}
int WINAPI _export BtoEGetArcFileName(HARC _harc, LPSTR _lpBuffer,const int _nSize){return B2EGetArcFileName(_harc,_lpBuffer,_nSize);}
DWORD WINAPI _export BtoEGetArcFileSize(HARC _harc){return B2EGetArcFileSize(_harc);}
DWORD WINAPI _export BtoEGetArcOriginalSize(HARC _harc){return B2EGetArcOriginalSize(_harc);}
DWORD WINAPI _export BtoEGetArcCompressedSize(HARC _harc){return B2EGetArcCompressedSize(_harc);}
BOOL WINAPI _export BtoEGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetArcFileSizeEx(_harc,_lpllSize);}
BOOL WINAPI _export BtoEGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetArcOriginalSizeEx(_harc,_lpllSize);}
BOOL WINAPI _export BtoEGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetArcCompressedSizeEx(_harc,_lpllSize);}
DWORD WINAPI _export BtoEGetArcReadSize(HARC _harc){return B2EGetArcReadSize(_harc);}
BOOL WINAPI _export BtoEGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetArcReadSizeEx(_harc,_lpllSize);}
WORD WINAPI _export BtoEGetArcRatio(HARC _harc){return B2EGetArcRatio(_harc);}
WORD WINAPI _export BtoEGetArcDate(HARC _harc){return B2EGetArcDate(_harc);}
WORD WINAPI _export BtoEGetArcTime(HARC _harc){return B2EGetArcTime(_harc);}
BOOL WINAPI _export BtoEGetArcWriteTimeEx(HARC _harc,FILETIME *_lpftLastWriteTime){return B2EGetArcWriteTimeEx(_harc,_lpftLastWriteTime);}
BOOL WINAPI _export BtoEGetArcCreateTimeEx(HARC _harc,FILETIME *_lpftCreationTime){return B2EGetArcCreateTimeEx(_harc,_lpftCreationTime);}
BOOL WINAPI _export BtoEGetArcAccessTimeEx(HARC _harc,FILETIME *_lpftLastAccessTime){return B2EGetArcAccessTimeEx(_harc,_lpftLastAccessTime);}
UINT WINAPI _export BtoEGetArcOSType(HARC _harc){return B2EGetArcOSType(_harc);}
int WINAPI _export BtoEIsSFXFile(HARC _harc){return B2EIsSFXFile(_harc);}
int WINAPI _export BtoEGetFileName(HARC _harc, LPSTR _lpBuffer,const int _nSize){return B2EGetFileName(_harc,_lpBuffer,_nSize);}
int WINAPI _export BtoEGetMethod(HARC _harc, LPSTR _lpBuffer,const int _nSize){return B2EGetMethod(_harc,_lpBuffer,_nSize);}
DWORD WINAPI _export BtoEGetOriginalSize(HARC _harc){return B2EGetOriginalSize(_harc);}
DWORD WINAPI _export BtoEGetCompressedSize(HARC _harc){return B2EGetCompressedSize(_harc);}
BOOL WINAPI _export BtoEGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetOriginalSizeEx(_harc,_lpllSize);}
BOOL WINAPI _export BtoEGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize){return B2EGetCompressedSizeEx(_harc,_lpllSize);}
WORD WINAPI _export BtoEGetRatio(HARC _harc){return B2EGetRatio(_harc);}
WORD WINAPI _export BtoEGetDate(HARC _harc){return B2EGetDate(_harc);}
WORD WINAPI _export BtoEGetTime(HARC _harc){return B2EGetTime(_harc);}
DWORD WINAPI _export BtoEGetWriteTime(HARC _harc){return B2EGetWriteTime(_harc);}
DWORD WINAPI _export BtoEGetAccessTime(HARC _harc){return B2EGetAccessTime(_harc);}
DWORD WINAPI _export BtoEGetCreateTime(HARC _harc){return B2EGetCreateTime(_harc);}
BOOL WINAPI _export BtoEGetWriteTimeEx(HARC _harc,FILETIME *_lpftLastWriteTime){return B2EGetWriteTimeEx(_harc,_lpftLastWriteTime);}
BOOL WINAPI _export BtoEGetCreateTimeEx(HARC _harc,FILETIME *_lpftCreationTime){return B2EGetCreateTimeEx(_harc,_lpftCreationTime);}
BOOL WINAPI _export BtoEGetAccessTimeEx(HARC _harc,FILETIME *_lpftLastAccessTime){return B2EGetAccessTimeEx(_harc,_lpftLastAccessTime);}
DWORD WINAPI _export BtoEGetCRC(HARC _harc){return B2EGetCRC(_harc);}
int WINAPI _export BtoEGetAttribute(HARC _harc){return B2EGetAttribute(_harc);}
int WINAPI _export BtoEGetAttributes(HARC _harc){return B2EGetAttributes(_harc);}
UINT WINAPI _export BtoEGetOSType(HARC _harc){return B2EGetOSType(_harc);}
WORD WINAPI _export BtoEGetSubVersion(VOID){return B2EGetSubVersion();}
int WINAPI _export BtoEExtractMem(const HWND _hwndParent,LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize)
{
	return B2EExtractMem(_hwndParent,_szCmdLine,_lpBuffer,_dwSize,_lpTime,_lpwAttr,_lpdwWriteSize);
}
int WINAPI _export BtoECompressMem(const HWND _hwndParent,LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,const time_t *_lpTime, const LPWORD _lpwAttr,LPDWORD _lpdwWriteSize)
{
	return B2ECompressMem(_hwndParent,_szCmdLine,_lpBuffer,_dwSize,_lpTime,_lpwAttr,_lpdwWriteSize);
}
BOOL WINAPI _export BtoESetEnumMembersProc(B2E_WND_ENUMMEMBPROC _lpEnumProc){return B2ESetEnumMembersProc(_lpEnumProc);}
BOOL WINAPI _export BtoEClearEnumMembersProc(VOID){return B2EClearEnumMembersProc();}
BOOL WINAPI _export BtoESetEnumMembersProc64(B2E_WND_ENUMMEMBPROC _lpEnumProc, DWORD _dwStructSize){return B2ESetEnumMembersProc64(_lpEnumProc,_dwStructSize);}
BOOL WINAPI _export BtoEClearEnumMembersProc64(VOID){return B2EClearEnumMembersProc64();}


//----------------------
// 独自API
//----------------------
BOOL WINAPI _export B2ESetScriptDirectory(LPCSTR _szDirectory)
{
	if(B2EGetRunning())return FALSE;

	InterlockedIncrement(&s_RunningCount);

	BOOL bRet=TRUE;

	if(!myapp().m_ArchiverManager.init(_szDirectory))bRet=FALSE;

	InterlockedDecrement(&s_RunningCount);
	return bRet;
}

/*B2Eスクリプトの数を取得*/
int WINAPI _export B2EScriptGetCount()
{
	if(B2EGetRunning())return -1;
	return myapp().m_ArchiverManager.getB2ECount();
}

/*B2Eスクリプト名を取得*/
BOOL WINAPI _export B2EScriptGetName(const UINT _nIndex,LPSTR _szName,const DWORD _dwSize)
{
	if(B2EGetRunning())return FALSE;
	if(!_szName){
		return FALSE;	//異常
	}
	InterlockedIncrement(&s_RunningCount);

	BOOL ret=TRUE;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()<=_nIndex){
		ret=FALSE;
	}else{
		//拡張子取得(ext1.ext2.b2e)
		LPCSTR x=myapp().m_ArchiverManager.getB2EMeltExt(_nIndex);
		if(!x)ret=FALSE;
		strncpy_s(_szName,_dwSize,x,_dwSize);
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
B2Eスクリプトの能力を列挙:
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _lpAbility:能力
*/
BOOL WINAPI _export B2EScriptGetAbility(const UINT _nIndex,LPWORD _lpAbility)
{
	if(B2EGetRunning())return FALSE;
	if(!_lpAbility){
		return FALSE;	//異常
	}
	InterlockedIncrement(&s_RunningCount);

	BOOL ret=TRUE;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()<=_nIndex){
		ret=FALSE;
	}else{
		//能力
		*_lpAbility=myapp().m_ArchiverManager.getB2EAbility(_nIndex);
	}
	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
解凍できる拡張子の_nExtIndex番目のものを取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _nExtIndex:対応拡張子のインデックス(0オリジン)
*/
BOOL WINAPI _export B2EScriptGetExtractExtensions(const UINT _nIndex,const UINT _nExtIndex,LPSTR _szExt,const DWORD _dwSize)
{
	if(B2EGetRunning())return FALSE;
	if(!_szExt){
		return FALSE;	//異常
	}
	InterlockedIncrement(&s_RunningCount);

	BOOL ret=TRUE;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()<=_nIndex){
		ret=FALSE;
	}else{
		//拡張子取得(ext1.ext2.b2e)
		LPCSTR x=myapp().m_ArchiverManager.getB2EMeltExt(_nIndex),y=NULL;
		if(!x)ret=FALSE;
		else if(x[0]=='#'){	//圧縮のみ
			ret=FALSE;
		}
		else{
			for(UINT i=0;i<=_nExtIndex;i++){
				for( y=x+1; *y && *y!='.'; y++ );
				if('\0'==*y||i==_nExtIndex){
					break;
				}else{
					x=y+1;
				}
			}
			if(!y||'\0'==*y){
				ret=FALSE;
			}else{
				memset(_szExt,'\0',_dwSize);
				int len=y-x;
				if(len>(signed)_dwSize)len=_dwSize;
				strncpy_s(_szExt,_dwSize,x,len);
			}
		}
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
B2Eの圧縮対応形式を取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _szType:形式名
*/
BOOL WINAPI _export B2EScriptGetCompressType(const UINT _nIndex,LPSTR _szType,const DWORD _dwSize)
{
	if(B2EGetRunning())return FALSE;
	if(!_szType){
		return FALSE;	//異常
	}
	InterlockedIncrement(&s_RunningCount);

	BOOL ret=TRUE;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()<=_nIndex){
		ret=FALSE;
	}else{
		const kiStr& type=myapp().m_ArchiverManager.getB2ECmprFmt(_nIndex);
		strncpy_s(_szType,_dwSize,type,_dwSize);
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
圧縮用B2Eの対応メソッドを取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _nMethodIndex:メソッドの番号
  _szMethod:メソッド名
*/
BOOL WINAPI _export B2EScriptGetCompressMethod(const UINT _nIndex,const UINT _nMthdIndex,LPSTR _szMethod,const DWORD _dwSize)
{
	if(B2EGetRunning())return FALSE;
	if(!_szMethod){
		return FALSE;	//異常
	}
	InterlockedIncrement(&s_RunningCount);

	BOOL ret=TRUE;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()<=_nIndex){
		ret=FALSE;
	}else{
		const StrArray& methodArray=myapp().m_ArchiverManager.getB2ECmprMethod(_nIndex);
		if(methodArray.len()<=_nMthdIndex)ret=FALSE;
		else{
			strncpy_s(_szMethod,_dwSize,methodArray[_nMthdIndex],_dwSize);
		}
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
圧縮用B2Eのデフォルトメソッド番号を取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _szMethod:メソッド名
*/
int WINAPI _export B2EScriptGetDefaultCompressMethod(const UINT _nIndex)
{
	if(B2EGetRunning())return -1;
	InterlockedIncrement(&s_RunningCount);

	int ret=-1;
	//範囲チェック
	if(myapp().m_ArchiverManager.getB2ECount()>_nIndex){
		ret=myapp().m_ArchiverManager.getB2EDefaultCmprMethod(_nIndex);
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

/*
  指定したアーカイブファイルの解凍に使うB2Eのインデックスを取得
    _szFileName  チェックすべきアーカイブファイルのファイル名
戻り値
    非負                 B2Eスクリプトのインデックス
    0xFFFFFFFF           エラー、もしくは現在利用可能なB2Eスクリプトで扱えないファイル
*/
UINT WINAPI _export B2EScriptGetExtractorIndex(LPCSTR _szFileName)
{
	if(B2EGetRunning())return 0xFFFFFFFF;
	InterlockedIncrement(&s_RunningCount);

	UINT ret;
	if(!_szFileName||strlen(_szFileName)==0||!kiSUtil::exist(_szFileName)){
		ret=0xFFFFFFFF;
	}else{
		ret=myapp().m_ArchiverManager.getB2EExtractorIndex(kiPath(_szFileName));
	}

	InterlockedDecrement(&s_RunningCount);
	return ret;
}

