//--- K.I.LIB ---
// kl_carc.h : handling "common archivers' dll"

#ifndef AFX_KIARCDLLRAW_H__C94DE2A0_4292_49CE_8471_2CAA1340D216__INCLUDED_
#define AFX_KIARCDLLRAW_H__C94DE2A0_4292_49CE_8471_2CAA1340D216__INCLUDED_

#include "../b2e32.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// �����A�[�J�C�oDLL����ɔ����������C��

class kiArcDLLRaw  
{
public: //-- �O�����C���^�[�t�F�C�X --------------------------

	// DLL���ŏ������B
	kiArcDLLRaw( const char* dllname );
	kiPath& name()
		{ return m_DllPath; }

	// DLL�������Ă��邩�ǂ����B
	bool isAlive();

	// API �̃��b�p [ DLL��API�������Ƃ��̕Ԓl ]

		// �o�[�W����x100 [ 0 ]
		WORD getVer();
		// �o�[�W�����T�ux100 [ 0 ]
		WORD getVerSub();
		// �R�}���h���� [ ERROR_NOT_SUPPORT ]
		int command( const HWND wnd, LPCSTR cmd, LPSTR buf, const DWORD siz );
		// ���Ƀ`�F�b�N [ FALSE ]
		BOOL check( LPCSTR filename, const int mode );
		// ���ɂ̎�� [ 0 ]
		int getArcType( LPCSTR filename );

		// �J�� [ NULL ]
		HARC openArc( const HWND wnd, LPCSTR arcname, const DWORD flag );
		// ����
		void closeArc( HARC arc );
		// ��ڂ����� [ ERROR_NOT_SUPPORT ]
		int findfirst( HARC arc, LPCSTR wildname, INDIVIDUALINFO* inf );
		// ��ڈȍ~������ [ ERROR_NOT_SUPPORT ]
		int findnext( HARC arc, INDIVIDUALINFO* inf );
		// �����擾 [ 0 ]
		int getAttr( HARC arc );

		// �I�[�i�[�w�� [ FALSE ]
		BOOL setOwner( HWND wnd );
		// �I�[�i�[���� [ FALSE ]
		BOOL clearOwner();

	// ��load�n�͌Ă΂Ȃ��Ă������ŉ��Ƃ�����̂łł���Ύg��Ȃ����ƁB
	bool load();
	void unload();

private: //-- �������� -----------------------------------

	HINSTANCE m_hDLL;
	bool not_loaded_yet;

	kiPath m_DllPath;
	kiStr m_DllNameBody;
	FARPROC getProc( const char* procname );

	FARPROC m_Proc[ ISARC_FUNCTION_END ];
	FARPROC f_VSb;
#define f_Cmd m_Proc[ ISARC ]
#define f_Chk m_Proc[ ISARC_GET_VERSION ]
#define f_Gat m_Proc[ 13 ]
#define f_Ver m_Proc[ ISARC_CHECK_ARCHIVE ]
#define f_Opn m_Proc[ ISARC_OPEN_ARCHIVE ]
#define f_Cls m_Proc[ ISARC_CLOSE_ARCHIVE ]
#define f_Ffs m_Proc[ ISARC_FIND_FIRST ]
#define f_Fnx m_Proc[ ISARC_FIND_NEXT ]
#define f_GAr m_Proc[ ISARC_GET_ATTRIBUTE ]
#define f_SOw m_Proc[ ISARC_SETOWNERWINDOW ]
#define f_COw m_Proc[ ISARC_CLEAROWNERWINDOW ]

public:
	virtual ~kiArcDLLRaw();
};

#endif
