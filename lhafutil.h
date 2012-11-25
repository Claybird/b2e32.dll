#ifndef __LHAFUTIL_H__
#define __LHAFUTIL_H__

#define FILL_ZERO(x) ZeroMemory(x,sizeof(x))

bool UtilGetTemporaryFileName(LPTSTR lpszFileName,LPCTSTR lpszPrefix);
void UtilPrepareTemporaryDirectory(kiPath&);
bool UtilReadFromResponceFile(LPCSTR lpszRespFile,StrArray &fileList);
bool UtilPathCollapseCopy(const kiPath &from,const kiPath &to,int &iMode,bool &bContinue);

//���C���h�J�[�h�W�J���s��
bool UtilExpandWildcard(LPCSTR lpPath,StrArray &files,bool bSkipDir);

class CUtilCurrentDirectoryRestore{	//�J�����g�f�B���N�g���𕜋�����
protected:
	char m_szCurrentDir[_MAX_PATH+1];
public:
	CUtilCurrentDirectoryRestore(){
		::GetCurrentDirectory(_MAX_PATH,m_szCurrentDir);
	}
	virtual ~CUtilCurrentDirectoryRestore(){
		::SetCurrentDirectory(m_szCurrentDir);
	}
};

#endif //__LHAFUTIL_H__

