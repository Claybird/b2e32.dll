#include "stdafx.h"
#include "lhafutil.h"
#include "resource.h"

bool UtilGetTemporaryFileName(LPTSTR lpszFileName,LPCTSTR lpszPrefix)
{
	TCHAR szTempDirPath[_MAX_PATH+1];
	FILL_ZERO(szTempDirPath);
	GetTempPath(_MAX_PATH,szTempDirPath);
	if(!GetTempFileName(szTempDirPath,lpszPrefix,0,lpszFileName)){
		return false;
	}
	return true;
}

// 全システム中で一意なテンポラリフォルダを作って返す
void UtilPrepareTemporaryDirectory(kiPath& tmp)
{
	static kiPath s_Path;
	if(0==s_Path.len()){
		//%TEMP%\%prefix%0000\filename...
		TCHAR szTempPath[_MAX_PATH+1];
		FILL_ZERO(szTempPath);
		GetTempPath(_MAX_PATH,szTempPath);
		PathAddBackslash(szTempPath);

		int Count=0;
		while(true){	//一時解凍先確保
			TCHAR Buffer[_MAX_PATH+1];
			FILL_ZERO(Buffer);
			wsprintf(Buffer,"%sb2e%d",szTempPath,Count);
			Count++;

			//存在チェック
			if(!PathFileExists(Buffer)){
				PathAddBackslash(Buffer);
				if(!PathIsDirectory(Buffer)){
					s_Path=Buffer;
					break;
				}
			}
		}
	}
	tmp=s_Path;
	tmp.beBackSlash( true );
	tmp.mkdir();
}

//レスポンスファイルを読み取る
bool UtilReadFromResponceFile(LPCSTR lpszRespFile,StrArray &fileList)
{
	fileList.empty();

	kiFile file;
	if(!file.open(lpszRespFile,true,false))return false;	//読み取り、作成はしない

	const DWORD dwSize=file.getSize();
	char *Buffer=new char[dwSize+2];
	if(dwSize!=file.read((LPBYTE)Buffer,dwSize)){
		delete [] Buffer;
		return false;
	}
	Buffer[dwSize]='\0';
	LPCSTR p=&Buffer[0];
	const LPCSTR end=&Buffer[dwSize+1];
	kiStr Line;
	for(;p!=end;p++){
		if(*p=='\n'||*p=='\r'||*p=='\0'){
			if(Line.len()>0){
				char tmp[_MAX_PATH+1];
				strncpy_s(tmp,Line,_MAX_PATH);
				PathUnquoteSpaces(tmp);
				fileList.add(tmp);
			}
			Line="";
		}
		else Line+=*p;
	}
	delete [] Buffer;
	return true;
}

// from\にあるファイルをサブディレクトリまで潜って列挙し、toにパス情報を無視してコピーする
// from/to末尾には\をつけておくことが必須
bool UtilPathCollapseCopy(const kiPath &from,const kiPath &to,int &iMode,bool &bContinue)
{
	enum {UNKNOWN=0,OVERWRITEALL,SKIPALL};	//順にデフォルト、全て上書き、全てスキップ(存在しないファイルだけコピー)
	if(-1==iMode){
		iMode=UNKNOWN;
		bContinue=true;
	}
	//上書き確認ダイアログのクラス
	struct COverwriteConfirm : public kiDialog
	{
		const LPCSTR lpszNewFile,lpszOldFile;
		kiStr msg;
		COverwriteConfirm( LPCSTR _lpszNew, LPCSTR _lpszOld)
			: kiDialog( IDD_DIALOG_OVERWRITE ), lpszNewFile(_lpszNew), lpszOldFile(_lpszOld) {}
		BOOL onInit()
			{
				const int Length=_MAX_PATH*2+256;
				char Buffer[Length]={0};
				sendMsgToItem( IDC_STATIC_MSG,WM_GETTEXT,Length-1,(LPARAM)Buffer);
				msg=Buffer;
				wsprintf(Buffer,msg,lpszNewFile,lpszOldFile);
				msg=Buffer;
				sendMsgToItem( IDC_STATIC_MSG, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)msg );
				return TRUE;
			}
		virtual BOOL CALLBACK proc( UINT msg, WPARAM wp, LPARAM lp ) {
			if(WM_COMMAND==msg){
				end(LOWORD(wp));
				return TRUE;
			}
			return FALSE;
		}
	};

	kiFindFile f;
	WIN32_FIND_DATA fd;
	kiPath wild( from );
	wild.beDirOnly();
	wild += "*";

	f.begin( wild );
	bool bRet=true;
	while( f.next( &fd ) && bContinue)
	{
		if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){	//サブディレクトリの処理
			kiPath dir(from);
			dir+=fd.cFileName;
			dir.beBackSlash(true);
			bRet=bRet&&UtilPathCollapseCopy(dir,to,iMode,bContinue);
		}
		else{
			kiPath src(from);
			src += fd.cFileName;
			kiPath dest(to);
			dest.beDirOnly(); dest+= fd.cFileName;
			switch(iMode){
			case OVERWRITEALL:
				if( !CopyFile( src, dest, FALSE ) ){	//強制上書き
					bRet = false;
				}
				break;
			case SKIPALL:
				if(!kiSUtil::exist(dest))
					CopyFile( src, dest, TRUE );	//既存ファイルはスキップ
				break;
			default:
				if(!kiSUtil::exist(dest)){
					if( !CopyFile( src, dest, TRUE ) )bRet=false;	//既存ファイルの上書きに失敗
				}else{//ファイルが既に存在
					COverwriteConfirm dlg(fd.cFileName,dest);
					dlg.doModal();
					switch(dlg.getEndCode()){
					case IDC_OVERWRITEALL:	//FALLTHROUGH
						iMode=OVERWRITEALL;
					case IDYES:
						if( !CopyFile( src, dest, FALSE ) ){	//強制上書き
							bRet = false;
						}
						break;
					case IDC_SKIPALL:	//FALLTHROUGH
						iMode=SKIPALL;
					case IDNO:
						bRet=false;
						break;
					case IDCANCEL:	//FALLTHROUGH
					default:
						bRet=false;
						bContinue=false;
						break;
					}
				}
				break;
			}
		}
	}
	return bRet;
}

//ワイルドカード展開を行う
bool UtilExpandWildcard(LPCSTR lpPath,StrArray &files,bool bSkipDir)
{
	kiFindFile f;
	WIN32_FIND_DATA fd;
	//ファイル名をフルパスで
	kiPath fname(lpPath);
	fname.beFullPath();

	//ディレクトリ名の取り出し
	kiPath dir(fname);
	dir.beDirOnly();
	dir.beBackSlash(true);

	//検索
	files.empty();
	f.begin( fname );
	while(f.next( &fd )){	//各ファイルごとに、ワイルドカード展開も行いつつ追加
		if(bSkipDir&&(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))continue;	//ディレクトリはスキップ
		files.add(dir+fd.cFileName);
	}
	return files.len()>0;
}
