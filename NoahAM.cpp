// NoahAM.cpp
//-- control many archiver routines --

#include "stdafx.h"
#include "resource.h"
#include "Main.h"
#include "NoahAM.h"
//#include "ArcDLL.h"
#include "ArcB2e.h"
#include "lhafutil.h"
#include "OpenArchiveData.h"

//----------------------------------------------//
//------ ���������̃f�[�^�ŏ��������Ă��� ------//
//----------------------------------------------//

bool CNoahArchiverManager::init(LPCSTR _szDirectory)
{
	finish();
	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p
	// �g���X�N���v�g���[�h
	kiStr dir=CArcB2e::init_b2e_path(_szDirectory);
	if(!::SetCurrentDirectory( dir ))return false;
	char buffer[256];
	GetCurrentDirectory( 256,buffer );
	kiFindFile find;
	find.begin( "*.b2e" );
	WIN32_FIND_DATA fd;
	int t=0;
	for( ; find.next(&fd); t++ )
		m_AList.add( new CArcB2e(fd.cFileName) );
	m_b2e = (t>0);

	return true;
}

unsigned long CNoahArchiverManager::get_files_info(const cCharArray& _files,wfdArray &FName,kiPath &BasePath)
{
	StrArray fullfiles;
	FName.empty();
	//-- ���p�X���擾( �o���邾�����p�͈͂��L���邽�߁A8.3�`���� )
	if( _files.len() != 0 )
	{
		//���C���h�J�[�h�W�J
		//added by Claybird 2012.April.14
		for(unsigned long i=0;i<_files.len();i++){
			StrArray tmp;
			UtilExpandWildcard(_files[i],tmp,false);
			for(unsigned long j=0;j<tmp.len();j++){
				fullfiles.add(tmp[j]);
			}
		}

		char spath[MAX_PATH];
		BasePath =
			( 0!=::GetShortPathName( fullfiles[0], spath, MAX_PATH ) )
			? spath : "";
		if( !BasePath.beDirOnly() )
		{
			BasePath.beSpecialPath( kiPath::Cur );
			BasePath.beBackSlash( true );
		}
	}

	//-- �Z���t�@�C�����ƒ����̂𗼕��擾���Ă���
	FName.alloc( fullfiles.len() );
	unsigned int c=0;
	for( unsigned int i=0; i!=fullfiles.len(); i++ ){
		if( kiFindFile::findfirst( fullfiles[i], &FName[c] ) )
		{
			/*if( FName[c].cAlternateFileName[0] == '\0' )
				::lstrcpy(FName[c].cAlternateFileName,FName[c].cFileName);	DO NOT DO THIS: MAY CAUSE BUFFER OVERFLOW*/
			++c;
		}
	}
	FName.forcelen( c );

	return c;
}

void CNoahArchiverManager::get_file_info(const char *file,WIN32_FIND_DATA &wfd,kiPath& BasePath)
{
	//-- ���p�X���擾( �o���邾�����p�͈͂��L���邽�߁A8.3�`���� )
	char spath[MAX_PATH];
	BasePath =
		( 0!=::GetShortPathName( file, spath, MAX_PATH ) )
		? spath : "";
	if( !BasePath.beDirOnly() )
	{
		BasePath.beSpecialPath( kiPath::Cur );
		BasePath.beBackSlash( true );
	}

	//-- �Z���t�@�C�����ƒ����̂𗼕��擾���Ă���
	if( kiFindFile::findfirst( file, &wfd ) ){
		/*if( wfd.cAlternateFileName[0] == '\0' )
			::lstrcpy(wfd.cAlternateFileName,wfd.cFileName);	DO NOT DO THIS: MAY CAUSE BUFFER OVERFLOW*/
	}
}


// �w�肳�ꂽ�g���q�ɑΉ����Ă��郋�[�`������`�T��
CArchiver* CNoahArchiverManager::fromExt( const char* ext )
{
	kiStr tmp = ext;
	tmp.lower();

	int len=m_AList.len();
	for( unsigned int i=0; i<m_AList.len(); i++ ){
		if( m_AList[i]->extCheck( tmp )){
			if(m_AList[i]->ability() & aMelt){
				return m_AList[i];
			}
		}
	}
	return NULL;
}

bool CNoahArchiverManager::checkArchive( const kiPath &arcFile )
{
	return NULL!=get_extractor(arcFile);
}

//--------------------------//
//--- ���k���[�`���̎擾 ---//
//--------------------------//

CArchiver* CNoahArchiverManager::get_compressor( const char* fmt, const char* method, bool sfx ,int &iMethod)
{
	int m;
	CArchiver* lpCompressor=NULL;
	iMethod=-1;
	for( unsigned int i=0; i!=m_AList.len(); i++ ){
		if( -1 != (m=m_AList[i]->cancompressby(fmt,method,sfx)) ){
			if( m!=-2 ) // ���S��v
			{
				lpCompressor = m_AList[i];
				iMethod = m;
				break;
			}
			else if( iMethod == -1 ) // �`�����݈̂�v�����ŏ��̃��m
			{
				if(!method||0==strlen(method)){	//���\�b�h�ȗ����̂݃f�t�H���g���\�b�h�g�p
					lpCompressor = m_AList[i];
					iMethod = m_AList[i]->cmp_mhd_default();
				}
			}
		}
	}
	if(iMethod == -1)return NULL;
	return lpCompressor;
}

CArchiver* CNoahArchiverManager::get_compressor_from_ext( const char* _ext, const char* method, bool sfx ,int &iMethod)
{
	int m;
	CArchiver* lpCompressor=NULL;
	iMethod=-1;
	kiStr ext = _ext;
	ext.lower();
	for( unsigned int i=0; i!=m_AList.len(); i++ ){
		if( -1 != (m=m_AList[i]->cancompressby_ext(ext,method,sfx)) ){
			if( m!=-2 ) // ���S��v
			{
				lpCompressor = m_AList[i];
				iMethod = m;
				break;
			}
			else if( iMethod == -1 ) // �`�����݈̂�v�����ŏ��̃��m
			{
				if(!method||0==strlen(method)){	//���\�b�h�ȗ����̂݃f�t�H���g���\�b�h�g�p
					lpCompressor = m_AList[i];
					iMethod = m_AList[i]->cmp_mhd_default();
				}
			}
		}
	}
	if(iMethod == -1)return NULL;
	return lpCompressor;
}


//----------------------------------------------//
//------------ �o�[�W������񕶎��� ------------//
//----------------------------------------------//

void CNoahArchiverManager::get_version( kiStr& str )
{
	kiStr tmp;
	for( unsigned int i=0; i!=m_AList.len(); i++ )
		if( m_AList[i]->ver( tmp ) )
			str+=tmp, str+="\r\n";
}

//----------------------------------------------//
//--------------- ���k�`�����X�g ---------------//
//----------------------------------------------//

static unsigned int find( const cCharArray& x, const char* o )
{
	for( unsigned int i=0; i!=x.len(); i++ )
		if( 0==ki_strcmp( x[i], o ) )
			return i;
	return 0xffffffff;
}

static unsigned int find( const StrArray& x, const char* o )
{
	for( unsigned int i=0; i!=x.len(); i++ )
		if( x[i]==o )
			return i;
	return 0xffffffff;
}

void CNoahArchiverManager::get_cmpmethod(
		const char* set,
		int& def_mhd,
		StrArray& mhd_list,
		bool need_ext,
		cCharArray* ext_list )
{
	def_mhd = -1;

	const char* x;
	for( unsigned int i=0; i!=m_AList.len(); i++ )
	{
		if( *(x = m_AList[i]->cmp_ext())=='\0' )
			continue;
		if( need_ext )
		{
			if( -1 == find( *ext_list, x ) )
				ext_list->add( x );
		}
		if( 0 == ki_strcmp( set, x ) )
		{
			if( mhd_list.len()==0 )
			{
				def_mhd = m_AList[i]->cmp_mhd_default();
				for( unsigned int j=0; j!=m_AList[i]->cmp_mhd_list().len(); j++ )
					mhd_list.add( (m_AList[i]->cmp_mhd_list())[j] );
			}
			else
			{
				for( unsigned int j=0; j!=m_AList[i]->cmp_mhd_list().len(); j++ )
					if( -1 == find( mhd_list, (m_AList[i]->cmp_mhd_list())[j] ) )
						mhd_list.add( (m_AList[i]->cmp_mhd_list())[j] );
			}
		}
	}

	if( def_mhd == -1 )
		def_mhd = 0;
}


//----------------------------------------------//
//----------------- �𓀍�� -------------------//
//----------------------------------------------//
CArchiver* CNoahArchiverManager::get_extractor(const kiPath &arcFile)
{
	if( !kiSUtil::exist(arcFile) ){
		return NULL;
	}

	//-- 0byte�t�@�C�� / �f�B���N�g���͒e��
	WIN32_FIND_DATA wfd;
	kiPath BasePath;
	get_file_info(arcFile,wfd,BasePath);

	if( (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || 0==kiFile::getSize( arcFile, 0 ) ){
		return NULL;
	}

	//-- �܂��Ή��g���q���ǂ����Ō��`����I�o
	CArchiver* x = fromExt( kiPath::ext(wfd.cFileName) );
	CArchiver* bad=NULL;

	//-- ���`�ŁA�t�@�C�����e�ɂ��`�F�b�N
	if( x ){
		if(!x->check( arcFile ) ){
			//-- ���`�����e�`�F�b�N�s�Ȃ��̂Ŗ��������Ȃ�A����͎g���Ȃ�
			if( (x->ability() & aCheck) ){
				bad=x;
				x=NULL;
			}
		}
	}

	//-- ���`���_���Ȃ�A���̑��̓��e�`�F�b�N�\�ȃ��[�`���S�ĂŎ���
	if(!x){
		for( unsigned long j=0; j!=m_AList.len(); j++ )
			if( m_AList[j]!=bad/*x*/ && m_AList[j]->check( arcFile ) )
			{
				x=m_AList[j];
				break;
			}
	}
	return x;
}

int CNoahArchiverManager::do_melting( const kiPath &_arcFile,const kiPath &destdir,bool bIgnorePath,kiStr &strLog)
{
	kiPath arcFile(_arcFile);
	arcFile.beFullPath();

	if(bIgnorePath){
		//�p�X���𖳎����ĉ𓀂���
		//��U�e���|�����ɉ𓀂��Ă���ڕW�f�B���N�g���ɃR�s�[����
		strLog+="Extracting in Path-Ignore mode...\r\n";

		kiPath tmpdir;
		UtilPrepareTemporaryDirectory( tmpdir );
		tmpdir+="xtemp\\";	//eXtract TEMPorary

		do_melting(arcFile,tmpdir,false,strLog);

		int _i=-1;
		bool _bContinue=true;
		kiPath ddir( destdir );
		ddir.beBackSlash( true );
		ddir.mkdir();
		if(!UtilPathCollapseCopy(tmpdir,ddir,_i,_bContinue)){
			if(_bContinue)return ERROR_CANNOT_WRITE;
			else return ERROR_USER_SKIP;
		}
		else return 0;
	}

	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p
	if( !kiSUtil::exist(arcFile) ){
		strLog+="Extracting Error:\r\n"
			"Archive file \'";
		strLog+=arcFile;
		strLog+="' is not found\r\n";
		return ERROR_NOT_FIND_ARC_FILE;
	}

	//-- 0byte�t�@�C�� / �f�B���N�g���͒e��
	WIN32_FIND_DATA wfd;
	kiPath BasePath;
	get_file_info(arcFile,wfd,BasePath);

	if( (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || 0==kiFile::getSize( arcFile, 0 ) ){
		strLog+="Extracting Error:\r\n"
			"Archive file \'";
		strLog+=arcFile;
		strLog+="' is not an archive\r\n";
		return ERROR_NOT_ARC_FILE;
	}

	CArchiver* x=get_extractor(arcFile);
	if( x ){
		//-- �o�͐�

		kiPath ddir( destdir );
		ddir.beBackSlash( true );
		ddir.mkdir(), ddir.beShortPath();


		//-- �𓀁I

		arcname an( BasePath,
			wfd.cAlternateFileName[0]==0 ? wfd.cFileName : wfd.cAlternateFileName,
			wfd.cFileName );
		int result = x->melt( an, ddir );
		if( result</*0x8000*/ERROR_START ){
			//nothing
			strLog+="Extraction Successfully Completed.\r\n";
			return result;
		}
		else if(result==ERROR_USER_CANCEL){
			strLog+="Extraction Aborted:\r\n"
				"User Cancel\r\n";
			return result;
		}
		else
		{
			//�G���[�I
			char str[255];
			/*wsprintf( str, "%s\nError No: [%x]","Extract Error", result );
			app()->msgBox( str );*/
			wsprintf( str, "%s\r\nError No: [%x]","Extract Error", result );
			strLog+="Extraction Error:\r\n";
			strLog+=str;
			return result;
		}
	}
	strLog+="Extraction Error:\r\n"
		"Can't find Archive Handler\r\n";
	return ERROR_NOT_ARC_FILE;
}

//�ʉ�
int CNoahArchiverManager::do_melting_each( const kiPath &_arcFile,const kiPath &destdir,bool bIgnorePath,const cCharArray &files,kiStr &strLog)
{
	kiPath arcFile(_arcFile);
	arcFile.beFullPath();

	if(bIgnorePath){
		//�p�X���𖳎����ĉ𓀂���
		//��U�e���|�����ɉ𓀂��Ă���ڕW�f�B���N�g���ɃR�s�[����
		strLog+="Extracting in Path-Ignore mode...\r\n";

		kiPath tmpdir;
		UtilPrepareTemporaryDirectory( tmpdir );
		tmpdir+="xtemp\\";	//eXtract TEMPorary

		do_melting_each(arcFile,tmpdir,false,files,strLog);

		int _i=-1;
		bool _bContinue=true;
		kiPath ddir( destdir );
		ddir.beBackSlash( true );
		ddir.mkdir();
		if(!UtilPathCollapseCopy(tmpdir,ddir,_i,_bContinue)){
			if(_bContinue)return ERROR_CANNOT_WRITE;
			else return ERROR_USER_SKIP;
		}
		else return 0;
	}

	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p
	if( !kiSUtil::exist(arcFile) ){
		strLog+="Extracting Error:\r\n"
			"Archive file \'";
		strLog+=arcFile;
		strLog+="' is not found\r\n";
		return ERROR_NOT_FIND_ARC_FILE;
	}
	//-- 0byte�t�@�C�� / �f�B���N�g���͒e��
	WIN32_FIND_DATA wfd;
	kiPath BasePath;
	get_file_info(arcFile,wfd,BasePath);

	if( (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || 0==kiFile::getSize( arcFile, 0 ) ){
		strLog+="Extracting Error:\r\n"
			"Archive file \'";
		strLog+=arcFile;
		strLog+="' is not an archive\r\n";
		return ERROR_NOT_ARC_FILE;
	}

	CArchiver* x=get_extractor(arcFile);

	if(files.len()==0){
		strLog+="Extraction Error:\r\n"
			"File list is empty.\r\n"
			"This error can be a bug of "DLL_NAME"\r\n";
		return ERROR_NOT_FILENAME;
	}

	if( x ){
		if(!(x->ability()&aMeltEach)){
			strLog+="Extraction Error:\r\n"
				"This ArchiveHandler does not have ability to melt only specified file.\r\n";
			return ERROR_NOT_SUPPORT;
		}
		//-- �o�͐�

		kiPath ddir( destdir );
		ddir.beBackSlash( true );
		ddir.mkdir(), ddir.beShortPath();


		//-- �𓀁I

		arcname an( BasePath,
			wfd.cAlternateFileName[0]==0 ? wfd.cFileName : wfd.cAlternateFileName,
			wfd.cFileName );
		aflArray temp(files.len());
		//�t�@�C�����X�g�쐬
		for(unsigned int i=0;i<files.len();i++){
			strncpy_s(temp[i].inf.szFileName,files[i],MAX_PATH);
		}
		temp.forcelen(files.len());
		int result = x->melt( an, ddir, &temp);
		if( result</*0x8000*/ERROR_START ){
			//nothing
			strLog+="Extraction Successfully Completed.\r\n";
			return result;
		}
		else if(result==ERROR_USER_CANCEL){
			strLog+="Extraction Aborted:\r\n"
				"User Cancel\r\n";
			return result;
		}
		else
		{
			//�G���[�I
			char str[255];
			/*wsprintf( str, "%s\nError No: [%x]","Extract Error", result );
			app()->msgBox( str );*/
			wsprintf( str, "%s\r\nError No: [%x]","Extract Error", result );
			strLog+="Extraction Error:\r\n";
			strLog+=str;
			return result;
		}
	}
	strLog+="Extraction Error:\r\n"
		"Can't find Archive Handler\r\n";
	return ERROR_NOT_ARC_FILE;
}

//----------------------------------------------//
//----------------- ���k��� -------------------//
//----------------------------------------------//

int CNoahArchiverManager::do_compressing(const char* _destfile,const cCharArray& files, const char* ext, const char* method, bool sfx ,kiStr &strLog )
{
	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p

	//�o�͐���t���p�X�Ŏ擾
	kiPath destfile(_destfile);
	destfile.beFullPath();

	int iMethod;
	CArchiver *Compressor=get_compressor(ext,method,sfx,iMethod);
	if(!Compressor){
		strLog+="Compression Error:\r\n"
			"Compression type or method is not supported\r\n";
		return ERROR_NOT_SUPPORT;	//�擾���s
	}

	int result = 0xffff, tr;

	// �t�@�C�����
	wfdArray FName;
	kiPath BasePath;
	get_files_info(files,FName,BasePath);

	// �o�͐���m���ɍ���Ă���
	kiPath destdir(destfile);
	destdir.beDirOnly();
	destdir.beBackSlash( true );
	destdir.mkdir();
	destdir.beShortPath();

	// Archiving�s�̌`���Ȃ�����
	if( !(Compressor->ability() & aArchive) )
	{
		wfdArray templist;

		for( unsigned int i=0; i!=FName.len(); i++ )
		{
			templist.empty();
			templist.add( FName[i] );
			tr = Compressor->compress( BasePath,templist,destdir,iMethod,sfx,destfile);
			if( tr</*0x8000*/ERROR_START || tr==ERROR_USER_CANCEL/*0x8020*/ )
				result = tr;
		}
	}
	else
		result = Compressor->compress( BasePath,FName,destdir,iMethod,sfx,destfile);

	// �J������
	if( result</*0x8000*/ERROR_START ){
		//nothing
		strLog+="Compression Successfully Completed.\r\n";
		return result;
	}
	else if(result==ERROR_USER_CANCEL){
		strLog+="Compression Error:\r\n"
			"User Cancel\r\n";
			return result;
	}
	else
	{
		//�G���[�I
		char str[255];
		/*wsprintf( str, "%s\nError No: [%x]", "Compression Error", result );
		app()->msgBox( str );*/
		wsprintf( str, "%s\r\nError No: [%x]", "Compression Error", result );
		strLog+="Compression Error:\r\n";
		strLog+=str;
		return result;
	}
	return result;
}

//added by claybird 2012.April.14
int CNoahArchiverManager::do_adding(const char* _arcfile,const cCharArray& files, const char* method, kiStr &strLog )
{
	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p

	const char* ext=PathFindExtension(_arcfile);
	if(!ext)return -1;
	ext+=1;

	//�A�[�J�C�u�����t���p�X�Ŏ擾
	kiPath arcFile(_arcfile);
	arcFile.beFullPath();

	kiFindFile f;
	WIN32_FIND_DATA fd;
	f.begin( arcFile );
	if( !f.next( &fd ) )return -1;
	arcFile.beDirOnly();
	arcname aname( arcFile, fd.cAlternateFileName[0] ? fd.cAlternateFileName : fd.cFileName, fd.cFileName );

	int iMethod;
	CArchiver *Compressor=get_compressor_from_ext(ext,method,false,iMethod);
	if(!Compressor){
		strLog+="Addition Error:\r\n"
			"Compression type or method is not supported\r\n";
		return ERROR_NOT_SUPPORT;	//�擾���s
	}

	int result = 0xffff;

	// �t�@�C�����
	wfdArray FName;
	kiPath BasePath;
	get_files_info(files,FName,BasePath);

	result = Compressor->add( aname,BasePath,FName,iMethod);

	// �J������
	if( result</*0x8000*/ERROR_START ){
		//nothing
		strLog+="Addition Successfully Completed.\r\n";
		return result;
	}
	else if(result==ERROR_USER_CANCEL){
		strLog+="Addition Error:\r\n"
			"User Cancel\r\n";
			return result;
	}
	else
	{
		//�G���[�I
		char str[255];
		/*wsprintf( str, "%s\nError No: [%x]", "Compression Error", result );
		app()->msgBox( str );*/
		wsprintf( str, "%s\r\nError No: [%x]", "Addition Error", result );
		strLog+="Addition Error:\r\n";
		strLog+=str;
		return result;
	}
	return result;
}

//added by claybird 2012.April.14
int CNoahArchiverManager::do_deleting(const char* _arcfile,const cCharArray& files,kiStr &strLog )
{
	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p

	const char* ext=PathFindExtension(_arcfile);
	if(!ext)return -1;
	ext+=1;

	//�A�[�J�C�u�����t���p�X�Ŏ擾
	kiPath arcFile(_arcfile);
	arcFile.beFullPath();

	kiFindFile f;
	WIN32_FIND_DATA fd;
	f.begin( arcFile );
	if( !f.next( &fd ) )return -1;
	arcFile.beDirOnly();
	arcname aname( arcFile, fd.cAlternateFileName[0] ? fd.cAlternateFileName : fd.cFileName, fd.cFileName );

	int iMethod;
	CArchiver *Compressor=get_compressor_from_ext(ext,NULL,false,iMethod);
	if(!Compressor){
		strLog+="Deletion Error:\r\n"
			"Compression type or method is not supported\r\n";
		return ERROR_NOT_SUPPORT;	//�擾���s
	}

	int result = 0xffff;

	// �t�@�C�����
	aflArray FName;

	if(!(Compressor->ability()&aDel)){
		strLog+="Deletion Error:\r\n"
			"This ArchiveHandler does not have ability to delete files.\r\n";
		return ERROR_NOT_SUPPORT;
	}

	aflArray temp(files.len());
	//�t�@�C�����X�g�쐬
	for(unsigned int i=0;i<files.len();i++){
		strncpy_s(temp[i].inf.szFileName,files[i],MAX_PATH);
	}
	temp.forcelen(files.len());
	result = Compressor->del( aname,temp);

	// �J������
	if( result</*0x8000*/ERROR_START ){
		//nothing
		strLog+="Deletion Successfully Completed.\r\n";
		return result;
	}
	else if(result==ERROR_USER_CANCEL){
		strLog+="Deletion Error:\r\n"
			"User Cancel\r\n";
			return result;
	}
	else
	{
		//�G���[�I
		char str[255];
		/*wsprintf( str, "%s\nError No: [%x]", "Compression Error", result );
		app()->msgBox( str );*/
		wsprintf( str, "%s\r\nError No: [%x]", "Deletion Error", result );
		strLog+="Deletion Error:\r\n";
		strLog+=str;
		return result;
	}
	return result;
}


int CNoahArchiverManager::getItemCount(const char* lpszArcFile)
{
	kiArray<arcfile> files;
	if(!do_filelist(lpszArcFile,files))return -1;

	return files.len();
}

bool CNoahArchiverManager::do_filelist( const char* lpszArcFile, aflArray &files)
{
	CUtilCurrentDirectoryRestore restore;	//�J�����g�f�B���N�g�������p
	kiPath arcFile(lpszArcFile);
	arcFile.beFullPath();

	CArchiver *x=get_extractor(arcFile);

	if(!x)return false;

	kiFindFile f;
	WIN32_FIND_DATA fd;
	f.begin( arcFile );
	if( !f.next( &fd ) )return false;
	arcFile.beDirOnly();
	arcname aname( arcFile, fd.cAlternateFileName[0] ? fd.cAlternateFileName : fd.cFileName, fd.cFileName );

	return x->v_list(aname,files);
}

//�w��t�@�C�����𓀂��邽�߂Ɏg��B2E�X�N���v�g�̃C���f�b�N�X
UINT CNoahArchiverManager::getB2EExtractorIndex(const kiPath &arcFile)
{
	CArchiver *x=get_extractor(arcFile);

	//�C���f�b�N�X��Ԃ�
	if(!x)return 0xFFFFFFFF;

	for(UINT i=0;i<m_AList.len();i++){
		if( x==m_AList[i] ){
			return i;
		}
	}
	return 0xFFFFFFFF;
}

//--------

//�w�b�_(7-Zip32.dll�̌`���𗬗p)
LPCSTR LIST_HEADER		="   Date      Time      Attr         Size   Compressed  Name\r\n";
//��؂�
LPCSTR LIST_SPLITTER	="------------------- ------- ------------ ------------  ------------------------\r\n";


//���Ԃ𐮌`
bool CNoahArchiverManager::formatTime(WORD wDate,WORD wTime,kiStr &str)
{
	LPCSTR lpDefault="XXXX-XX-XX XX:XX:XX";
	if(wDate==-1||wTime==-1){	//�����擾�Ɏ��s
		str=lpDefault;
		return false;
	}
	FILETIME TempTime;
	if(!DosDateTimeToFileTime(wDate,wTime,&TempTime)){
		str=lpDefault;
		return false;
	}
	SYSTEMTIME SystemTime;
	if(!FileTimeToSystemTime(&TempTime,&SystemTime)){
		str=lpDefault;
		return false;
	}

	char Buffer[64];
	wsprintf(Buffer,"%04d-%02d-%02d %02d:%02d:%02d",
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
			SystemTime.wHour, SystemTime.wMinute,SystemTime.wSecond);

	str=Buffer;
	return true;
}



//�P��A�[�J�C�u���̃t�@�C���ꗗ�𕶎���̌`�Ŏ擾����
//B2E32.dll���O�������ɃG�N�X�|�[�g���Ă���֐��͎g��Ȃ�;B2EGetRunning()�̉e�������邽��
bool CNoahArchiverManager::listFileInSingleArchive(LPCSTR _szFileName,kiStr &strOut)
{
	if(!_szFileName||strlen(_szFileName)==0){
		//�t�@�C�����w�薳��
		strOut="Listing Error:File not Specified.";
		return false;
	}

	if(!kiSUtil::exist(_szFileName)){
		//�t�@�C�����݃`�F�b�N
		strOut="Listing Error:File not found : ";
		strOut+=_szFileName;
		return false;
	}

	//�t�@�C������\��
	strOut="Listing of archive : ";
	strOut+=_szFileName;
	strOut+="\r\n\r\n";

	//�`���`�F�b�N
	//UINT nScriptIdx=B2EScriptGetExtractorIndex(_szArcFileName);
	UINT nScriptIdx=myapp().m_ArchiverManager.getB2EExtractorIndex(kiPath(_szFileName));
	if(0xFFFFFFFF==nScriptIdx||myapp().m_ArchiverManager.getB2ECount()<=nScriptIdx){
		//���Ή��`��
		strOut+="Listing Error:Not a valid archive.\r\n";
		return false;
	}else{
		//�\�̓`�F�b�N
		//B2EScriptGetAbility()
		WORD ability=myapp().m_ArchiverManager.getB2EAbility(nScriptIdx);
		if(!(ability&ABILITY_LIST)){
			strOut+="Listing Error:File listing is not available in this archive.\r\n";
			return false;
		}
	}

	//���X�e�B���O�J�n
	COpenArchiveData *lpArc=new COpenArchiveData(myapp().m_ArchiverManager);
	if(!lpArc->ScanArchive(_szFileName)){
		//���ɓ��e�擾�Ɏ��s
		delete lpArc;
		strOut+="Listing Error:Failed to get file list.\r\n";
		return false;
	}

	//�w�b�_�\��
	strOut+=LIST_HEADER;
	strOut+=LIST_SPLITTER;

	//�t�@�C���T�C�Y���v
	DWORD dwTotalOrgSize=0,dwTotalCompressedSize=0;
	if(lpArc->FindFirstFile()){	//���擾
		arcfile af;
		//���`���ĕ\��
		do{
			lpArc->GetItemData(af);

			//---����
			kiStr strDateTime;
			formatTime(af.inf.wDate,af.inf.wTime,strDateTime);
			strOut+=strDateTime;

			strOut+=" ";
			//---����
			char buf[32];
			//�������܂�悤�ɐ��`
			wsprintf(buf,"%7s",af.inf.szAttribute);
			strOut+=buf;

			strOut+=" ";
			//---���k�O�T�C�Y
			dwTotalOrgSize+=af.inf.dwOriginalSize;
			wsprintf(buf,"%12u",af.inf.dwOriginalSize);
			strOut+=buf;

			strOut+=" ";
			//---���k��T�C�Y
			dwTotalCompressedSize+=af.inf.dwCompressedSize;
			wsprintf(buf,"%12u",af.inf.dwCompressedSize);
			strOut+=buf;

			strOut+="  ";

			//---�t�@�C����
			strOut+=af.inf.szFileName;

			if(!af.isfile){
				//�f�B���N�g��
				strOut+="<DIR>";
			}
			strOut+="\r\n";
		}while(lpArc->FindNextFile());
	}
	//��؂��\��
	strOut+=LIST_SPLITTER;
	//---�t�@�C�����v��\��
	char buf[64];
	//���k�O
	wsprintf(buf,"%40u",dwTotalOrgSize);
	strOut+=buf;
	//���k��
	wsprintf(buf,"%13u",dwTotalCompressedSize);
	strOut+=buf;
	strOut+="  ";

	//�t�@�C����
	wsprintf(buf,"%u items",lpArc->GetItemCount());
	strOut+=buf;
	strOut+="\r\n\r\n";

	return true;
}

//�A�[�J�C�u���̃t�@�C���ꗗ�𕶎���̌`�Ŏ擾����
int CNoahArchiverManager::listFileInArchives(StrArray &files,kiStr &strOut)
{
	strOut="";
	UINT size=files.len();
	for(UINT i=0;i<size;i++){
		StrArray expanded_files;
		//����
		UtilExpandWildcard(files[i],expanded_files,true);
		for(UINT j=0;j<expanded_files.len();j++){
			kiStr strTmp;
			listFileInSingleArchive(expanded_files[j],strTmp);
			strOut+=strTmp;
		}
	}

	return 0;
}

