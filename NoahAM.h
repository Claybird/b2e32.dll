// NoahAM.h
//-- CNoahArchiverManager -- control many archiver routines --

#ifndef AFX_NOAHAM_H__CCE30761_D91B_4570_931A_6C165B19B22F__INCLUDED_
#define AFX_NOAHAM_H__CCE30761_D91B_4570_931A_6C165B19B22F__INCLUDED_

#include "Archiver.h"

class CNoahArchiverManager
{
public: //-- �O�����C���^�[�t�F�C�X ------------

	// ������
	bool init(LPCSTR _szDirectory);

	//�f�B�X�N��̃t�@�C�������擾(�߂�l�̓t�@�C����)
	static unsigned long get_files_info(const cCharArray& files,wfdArray &FName,kiPath &BasePath);
	static void get_file_info(const char *file,WIN32_FIND_DATA &wfd,kiPath &BasePath);
	CArchiver* get_compressor( const char* ext, const char* method, bool sfx ,int &iMethod);
	CArchiver* get_compressor_from_ext( const char* _ext, const char* method, bool sfx ,int &iMethod);

	// CheckArchive
	bool checkArchive( const kiPath &arcFile);
	// ��
	CArchiver* get_extractor(const kiPath &arcFile);
	int do_melting( const kiPath &arcFile,const kiPath &destdir,bool bIgnorePath,kiStr &strLog);
	int do_melting_each( const kiPath &arcFile,const kiPath &destdir,bool bIgnorePath,const cCharArray &files,kiStr &strLog);
	// ���k
	int do_compressing( const char* _destfile,const cCharArray& files, const char* ext, const char* method, bool sfx , kiStr &strLog);
	// �A�[�J�C�u���̃t�@�C���ꗗ�擾
	bool do_filelist( const char* lpszArcFile, aflArray &files);
	// �A�[�J�C�u�Ƀt�@�C����ǉ�:added by claybird 2012.April.14
	int do_adding(const char* _arcfile,const cCharArray& files, const char* method, kiStr &strLog );
	// �A�[�J�C�u����t�@�C�����폜:added by claybird 2012.April.14
	int do_deleting(const char* _arcfile,const cCharArray& files,kiStr &strLog );

	// �o�[�W�������
	void get_version( kiStr& str );
	// ���k�`�����X�g
	void get_cmpmethod( const char* set, int& def_mhd, StrArray& mhd_list, bool need_ext=false, cCharArray* ext_list=NULL );
	// b2e����ȏ�ǂݍ��܂�Ă����true
	bool b2e_enabled() { return m_b2e; }

	int getItemCount(const char*);

	unsigned int getB2ECount(){return m_AList.len();}	//B2E�X�N���v�g�̐�
	UINT getB2EExtractorIndex(const kiPath&);		//�w��t�@�C�����𓀂��邽�߂Ɏg��B2E�X�N���v�g�̃C���f�b�N�X
	WORD getB2EAbility(UINT index){return m_AList[index]->ability();}	//B2E�X�N���v�g�̔\�͂��擾
	LPCSTR getB2EMeltExt(UINT index){return m_AList[index]->mlt_ext();}	//GetB2EScriptName()�̂��ƁB
	const kiStr& getB2ECmprFmt(UINT index){return m_AList[index]->cmp_ext();}	//�g���q�ł͂Ȃ��`�������Ԃ����̂ŗv����:noted by Claybird 2012.April.15
	const StrArray& getB2ECmprMethod(UINT index){
		return m_AList[index]->cmp_mhd_list();
	}
	int getB2EDefaultCmprMethod(UINT index){
		return m_AList[index]->cmp_mhd_default();
	}
	//�A�[�J�C�u���̃t�@�C���ꗗ�𕶎���̌`�Ŏ擾����
	int listFileInArchives(StrArray&,kiStr &strOut);
private: //-- �������� ---------------------------

	//�𓀂ł���B2E���擾
	CArchiver* fromExt( const char* ext );
	kiArray<CArchiver*> m_AList;	//�g�p�\��B2E�̃��X�g
	bool m_b2e;
	//�I������
	void finish(){
		for( unsigned int i=0; i<m_AList.len(); i++ )
			delete m_AList[i];
		m_AList.empty();
		m_b2e=false;
	}
	//���Ԃ𐮌`
	bool formatTime(WORD wData,WORD wTime,kiStr &str);
	//�P��A�[�J�C�u���̃t�@�C���ꗗ�𕶎���̌`�Ŏ擾����
	bool listFileInSingleArchive(LPCSTR _szFileName,kiStr &strOut);
public:
	~CNoahArchiverManager(){
		finish();
	}

};

#endif
