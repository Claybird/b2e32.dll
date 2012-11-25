//--- K.I.LIB ---
// kl_wnd.h : window information manager

#ifndef AFX_KIWINDOW_H__26105B94_1E36_42FA_8916_C2F7FB9EF994__INCLUDED_
#define AFX_KIWINDOW_H__26105B94_1E36_42FA_8916_C2F7FB9EF994__INCLUDED_

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// kiWindow : Windowの簡単な管理

class kiWindow
{
friend void kilib_startUp(HMODULE hModule);
friend void kilib_shutDown();

private: //-- グローバルな初期化処理など ---------------------

	static void init();
	static void finish();

public: //-- 外向きインターフェイス --------------------------

	// 関連付けられているHWND
	HWND hwnd()
		{
			return m_hWnd;
		}

/*	// そのWindow用にアクセラレータをロード
	void loadAccel( UINT id );*/

	// ウインドウがまだ残っているかどうかチェック
	bool isAlive()
		{
			if( !m_hWnd )
				return false;
			if( ::IsWindow(m_hWnd) )
				return true;
			m_hWnd = NULL;
			return false;
		}

	// 親
	kiWindow* parent()
		{
			return kiwnd( ::GetParent( hwnd() ) );
		}

	// メッセージ送信
	int sendMsg( UINT msg, WPARAM wp=0, LPARAM lp=0 )
		{
			return ::SendMessage( hwnd(), msg, wp, lp );
		}

	// [static] キューにあるメッセージを全て処理
	static void msg();

	// [static] メッセージループをまわす。
	enum msglooptype {PEEK, GET};
	static void msgLoop( msglooptype type = GET );

	// [static] Windowを強制的に front へ
	static void setFront( HWND wnd );

	// [static] Windowを中央へ
	static void setCenter( HWND wnd, HWND rel=NULL );

	// [static] HWND -> kiWindow ( もしあれば )
	static kiWindow* kiwnd( HWND wnd )
		{
			kiWindow* ptr = (kiWindow*)::GetWindowLong( wnd, GWL_USERDATA );
			if( !ptr ) return NULL;
			if( ::IsBadCodePtr((FARPROC)&ptr) ) return NULL;
			return ptr;
		}

protected: //-- 派生クラス向け -----------------------------

	// 派生クラスは、作成直前にコレを呼ぶこと。
	static void preCreate( kiWindow* wnd )
		{ st_pCurInit = wnd; }
	// 破棄直前にコレを呼ぶこと。
	void detachHwnd();
	// いったんGET/POSTメッセージループを停止
	static void loopbreak()
		{
			loopbreaker = true;
		}

private: //-- 内部処理 -------------------------------------

	// ウインドウハンドル設定
	static LRESULT CALLBACK CBTProc( int code, WPARAM wp, LPARAM lp );
	static HHOOK st_hHook;
	static kiWindow* st_pCurInit;
	void setHwnd( HWND wnd )
		{
			m_hWnd = wnd;
		}

	// ウインドウ情報保持用変数
	HWND m_hWnd;
	HACCEL m_hAccel;
	// ダイアログメッセージ
	virtual bool isDlgMsg( MSG* msg )
		{ return false; }
	// GETループ一時抜けだし
	static bool loopbreaker;

protected:
	kiWindow();
public:
	virtual ~kiWindow();
};

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// kiDialog : DialogをkiWindowとして管理する

class kiDialog : public kiWindow
{
public: //-- 外向きインターフェイス --------------------------

	// モーダルダイアログとして実行
	virtual void doModal( HWND parent=NULL );

	// モードレスダイアログとして作成
	virtual void createModeless( HWND parent=NULL );

	// 終了コード取得
	UINT getEndCode()
		{
			return m_EndCode;
		}

	// モーダルか否か
	bool isModal()
		{
			return m_bStateModal;
		}

	// ダイアログアイテム
	int sendMsgToItem( UINT id, UINT msg, WPARAM wp=0, LPARAM lp=0 )
		{
			return ::SendDlgItemMessage( hwnd(), id, msg, wp, lp );
		}
	HWND item( UINT id )
		{
			return ::GetDlgItem( hwnd(), id );
		}

protected: //-- 派生クラス向け -----------------------------

	// リソースIDで初期化
	kiDialog( UINT id );

	// リソースID取得
	UINT getRsrcID()
		{
			return m_Rsrc;
		}

	// 終了コードをセット
	void setEndCode( UINT endcode )
		{
			m_EndCode = endcode;
		}

	// モーダルか否かのフラグ"のみ"を切替
	void setState( bool modal )
		{
			m_bStateModal = modal;
		}

	// 終了コードをセットして、終了する( IDOK を渡しても onOK() は呼ばれないことに注意！ )
	virtual void end( UINT endcode );

	// コマンド・メッセージ発生時に呼ばれる

		// ＯＫ -> onOK     -> if true end(IDOK)
		virtual bool onOK() {return true;}
		// 取消 -> onCancel -> if true end(IDCANCEL)
		virtual bool onCancel() {return true;}
		// WM_INITDIALOG      -> onInit
		virtual BOOL onInit() {return FALSE;}
		// WM_????            -> proc
		virtual BOOL CALLBACK proc( UINT msg, WPARAM wp, LPARAM lp ) {return FALSE;}

private: //-- 内部処理 -------------------------------------

	UINT m_EndCode;
	UINT m_Rsrc;
	bool m_bStateModal;
	bool isDlgMsg( MSG* msg )
		{
			return (FALSE!=::IsDialogMessage( hwnd(), msg ));
		}
	static BOOL CALLBACK commonDlg( HWND dlg, UINT msg, WPARAM wp, LPARAM lp );
};


#endif
