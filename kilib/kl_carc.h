//--- K.I.LIB ---
// kl_carc.h : handling "common archivers' dll"

#ifndef AFX_KIARCDLLRAW_H__C94DE2A0_4292_49CE_8471_2CAA1340D216__INCLUDED_
#define AFX_KIARCDLLRAW_H__C94DE2A0_4292_49CE_8471_2CAA1340D216__INCLUDED_

#include "../b2e32.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// 統合アーカイバDLLを非常に薄く覆うレイヤ

class kiArcDLLRaw  
{
public: //-- 外向きインターフェイス --------------------------

	// DLL名で初期化。
	kiArcDLLRaw( const char* dllname );
	kiPath& name()
		{ return m_DllPath; }

	// DLLが生きているかどうか。
	bool isAlive();

	// API のラッパ [ DLLやAPIが無いときの返値 ]

		// バージョンx100 [ 0 ]
		WORD getVer();
		// バージョンサブx100 [ 0 ]
		WORD getVerSub();
		// コマンド送り [ ERROR_NOT_SUPPORT ]
		int command( const HWND wnd, LPCSTR cmd, LPSTR buf, const DWORD siz );
		// 書庫チェック [ FALSE ]
		BOOL check( LPCSTR filename, const int mode );
		// 書庫の種類 [ 0 ]
		int getArcType( LPCSTR filename );

		// 開く [ NULL ]
		HARC openArc( const HWND wnd, LPCSTR arcname, const DWORD flag );
		// 閉じる
		void closeArc( HARC arc );
		// 一つ目を検索 [ ERROR_NOT_SUPPORT ]
		int findfirst( HARC arc, LPCSTR wildname, INDIVIDUALINFO* inf );
		// 二つ目以降を検索 [ ERROR_NOT_SUPPORT ]
		int findnext( HARC arc, INDIVIDUALINFO* inf );
		// 属性取得 [ 0 ]
		int getAttr( HARC arc );

		// オーナー指定 [ FALSE ]
		BOOL setOwner( HWND wnd );
		// オーナー解除 [ FALSE ]
		BOOL clearOwner();

	// ※load系は呼ばなくても内部で何とかするのでできれば使わないこと。
	bool load();
	void unload();

private: //-- 内部処理 -----------------------------------

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
