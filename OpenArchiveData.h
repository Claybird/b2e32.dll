#ifndef __OPENARCHIVEDATA_H__
#define __OPENARCHIVEDATA_H__

class COpenArchiveData{
public:
	COpenArchiveData(CNoahArchiverManager&);
	virtual ~COpenArchiveData();
	bool ScanArchive(const LPCTSTR);
	int GetItemCount();
	LPCTSTR GetArchiveName();
	bool GetItemData(arcfile&);
	bool FindFirstFile();
	bool FindNextFile();
protected:
	aflArray m_ItemArray;	//OpenArchive�Ŏ擾�����t�@�C�����ꗗ
	kiPath m_arcFile;	//�A�[�J�C�u��
	unsigned int m_itemIndex;
	CNoahArchiverManager &m_ArchiveManager;
};

#endif //__OPENARCHIVEDATA_H__
