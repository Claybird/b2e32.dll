// NoahAM.h
//-- CNoahArchiverManager -- control many archiver routines --

#ifndef AFX_NOAHAM_H__CCE30761_D91B_4570_931A_6C165B19B22F__INCLUDED_
#define AFX_NOAHAM_H__CCE30761_D91B_4570_931A_6C165B19B22F__INCLUDED_

#include "Archiver.h"

class CNoahArchiverManager
{
public: //-- 外向けインターフェイス ------------

	// 初期化
	bool init(LPCSTR _szDirectory);

	//ディスク上のファイル情報を取得(戻り値はファイル数)
	static unsigned long get_files_info(const cCharArray& files,wfdArray &FName,kiPath &BasePath);
	static void get_file_info(const char *file,WIN32_FIND_DATA &wfd,kiPath &BasePath);
	CArchiver* get_compressor( const char* ext, const char* method, bool sfx ,int &iMethod);
	CArchiver* get_compressor_from_ext( const char* _ext, const char* method, bool sfx ,int &iMethod);

	// CheckArchive
	bool checkArchive( const kiPath &arcFile);
	// 解凍
	CArchiver* get_extractor(const kiPath &arcFile);
	int do_melting( const kiPath &arcFile,const kiPath &destdir,bool bIgnorePath,kiStr &strLog);
	int do_melting_each( const kiPath &arcFile,const kiPath &destdir,bool bIgnorePath,const cCharArray &files,kiStr &strLog);
	// 圧縮
	int do_compressing( const char* _destfile,const cCharArray& files, const char* ext, const char* method, bool sfx , kiStr &strLog);
	// アーカイブ中のファイル一覧取得
	bool do_filelist( const char* lpszArcFile, aflArray &files);
	// アーカイブにファイルを追加:added by claybird 2012.April.14
	int do_adding(const char* _arcfile,const cCharArray& files, const char* method, kiStr &strLog );
	// アーカイブからファイルを削除:added by claybird 2012.April.14
	int do_deleting(const char* _arcfile,const cCharArray& files,kiStr &strLog );

	// バージョン情報
	void get_version( kiStr& str );
	// 圧縮形式リスト
	void get_cmpmethod( const char* set, int& def_mhd, StrArray& mhd_list, bool need_ext=false, cCharArray* ext_list=NULL );
	// b2eが一個以上読み込まれていればtrue
	bool b2e_enabled() { return m_b2e; }

	int getItemCount(const char*);

	unsigned int getB2ECount(){return m_AList.len();}	//B2Eスクリプトの数
	UINT getB2EExtractorIndex(const kiPath&);		//指定ファイルを解凍するために使うB2Eスクリプトのインデックス
	WORD getB2EAbility(UINT index){return m_AList[index]->ability();}	//B2Eスクリプトの能力を取得
	LPCSTR getB2EMeltExt(UINT index){return m_AList[index]->mlt_ext();}	//GetB2EScriptName()のこと。
	const kiStr& getB2ECmprFmt(UINT index){return m_AList[index]->cmp_ext();}	//拡張子ではなく形式名が返されるので要注意:noted by Claybird 2012.April.15
	const StrArray& getB2ECmprMethod(UINT index){
		return m_AList[index]->cmp_mhd_list();
	}
	int getB2EDefaultCmprMethod(UINT index){
		return m_AList[index]->cmp_mhd_default();
	}
	//アーカイブ中のファイル一覧を文字列の形で取得する
	int listFileInArchives(StrArray&,kiStr &strOut);
private: //-- 内部処理 ---------------------------

	//解凍できるB2Eを取得
	CArchiver* fromExt( const char* ext );
	kiArray<CArchiver*> m_AList;	//使用可能なB2Eのリスト
	bool m_b2e;
	//終了処理
	void finish(){
		for( unsigned int i=0; i<m_AList.len(); i++ )
			delete m_AList[i];
		m_AList.empty();
		m_b2e=false;
	}
	//時間を整形
	bool formatTime(WORD wData,WORD wTime,kiStr &str);
	//単一アーカイブ中のファイル一覧を文字列の形で取得する
	bool listFileInSingleArchive(LPCSTR _szFileName,kiStr &strOut);
public:
	~CNoahArchiverManager(){
		finish();
	}

};

#endif
