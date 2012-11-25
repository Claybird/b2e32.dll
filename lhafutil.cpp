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

// �S�V�X�e�����ň�ӂȃe���|�����t�H���_������ĕԂ�
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
		while(true){	//�ꎞ�𓀐�m��
			TCHAR Buffer[_MAX_PATH+1];
			FILL_ZERO(Buffer);
			wsprintf(Buffer,"%sb2e%d",szTempPath,Count);
			Count++;

			//���݃`�F�b�N
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

//���X�|���X�t�@�C����ǂݎ��
bool UtilReadFromResponceFile(LPCSTR lpszRespFile,StrArray &fileList)
{
	fileList.empty();

	kiFile file;
	if(!file.open(lpszRespFile,true,false))return false;	//�ǂݎ��A�쐬�͂��Ȃ�

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

// from\�ɂ���t�@�C�����T�u�f�B���N�g���܂Ő����ė񋓂��Ato�Ƀp�X���𖳎����ăR�s�[����
// from/to�����ɂ�\�����Ă������Ƃ��K�{
bool UtilPathCollapseCopy(const kiPath &from,const kiPath &to,int &iMode,bool &bContinue)
{
	enum {UNKNOWN=0,OVERWRITEALL,SKIPALL};	//���Ƀf�t�H���g�A�S�ď㏑���A�S�ăX�L�b�v(���݂��Ȃ��t�@�C�������R�s�[)
	if(-1==iMode){
		iMode=UNKNOWN;
		bContinue=true;
	}
	//�㏑���m�F�_�C�A���O�̃N���X
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
		if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){	//�T�u�f�B���N�g���̏���
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
				if( !CopyFile( src, dest, FALSE ) ){	//�����㏑��
					bRet = false;
				}
				break;
			case SKIPALL:
				if(!kiSUtil::exist(dest))
					CopyFile( src, dest, TRUE );	//�����t�@�C���̓X�L�b�v
				break;
			default:
				if(!kiSUtil::exist(dest)){
					if( !CopyFile( src, dest, TRUE ) )bRet=false;	//�����t�@�C���̏㏑���Ɏ��s
				}else{//�t�@�C�������ɑ���
					COverwriteConfirm dlg(fd.cFileName,dest);
					dlg.doModal();
					switch(dlg.getEndCode()){
					case IDC_OVERWRITEALL:	//FALLTHROUGH
						iMode=OVERWRITEALL;
					case IDYES:
						if( !CopyFile( src, dest, FALSE ) ){	//�����㏑��
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

//���C���h�J�[�h�W�J���s��
bool UtilExpandWildcard(LPCSTR lpPath,StrArray &files,bool bSkipDir)
{
	kiFindFile f;
	WIN32_FIND_DATA fd;
	//�t�@�C�������t���p�X��
	kiPath fname(lpPath);
	fname.beFullPath();

	//�f�B���N�g�����̎��o��
	kiPath dir(fname);
	dir.beDirOnly();
	dir.beBackSlash(true);

	//����
	files.empty();
	f.begin( fname );
	while(f.next( &fd )){	//�e�t�@�C�����ƂɁA���C���h�J�[�h�W�J���s���ǉ�
		if(bSkipDir&&(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))continue;	//�f�B���N�g���̓X�L�b�v
		files.add(dir+fd.cFileName);
	}
	return files.len()>0;
}
