//--- K.I.LIB ---
// kl_wnd.cpp : window information manager

#include "stdafx.h"
#include "kilib.h"


//-------- Window���쐬���� HWND �� kiWindow* ���Z�b�g���邽�߂̏��� -------//


kiWindow* kiWindow::st_pCurInit = NULL;
    HHOOK kiWindow::st_hHook    = NULL;

void kiWindow::init()
{
	// CreateWindow �p�t�b�N�ݒu
	st_hHook = ::SetWindowsHookEx( WH_CBT, &CBTProc, NULL, ::GetCurrentThreadId() );
}

void kiWindow::finish()
{
	// CreateWindow �p�t�b�N����
	::UnhookWindowsHookEx( st_hHook );
}

LRESULT CALLBACK kiWindow::CBTProc( int code, WPARAM wp, LPARAM lp )
{
	if( code == HCBT_CREATEWND )
	{
		if( st_pCurInit )
		{
			// k.i.lib �̃E�C���h�E�� CreateWindow ���ꂽ�ꍇ
			st_pCurInit->setHwnd( (HWND)wp );
			::SetWindowLong( (HWND)wp, GWL_USERDATA, (LONG)st_pCurInit );
			st_pCurInit = NULL;
		}
		else
			::SetWindowLong( (HWND)wp, GWL_USERDATA, 0 );
	}

	return ::CallNextHookEx( st_hHook, code, wp, lp );
}

void kiWindow::detachHwnd()
{
	::SetWindowLong( hwnd(), GWL_USERDATA, 0 );
	if( this == app()->mainwnd() )
		app()->setMainWnd( NULL );
	setHwnd( NULL );
}


//------------ Window �ɂ܂��G�g�Z�g���ȏ��� (static) ---------------//


bool kiWindow::loopbreaker = false;

void kiWindow::msg()
{
	for( MSG msg; ::PeekMessage( &msg,NULL,0,0,PM_REMOVE ); )
		::TranslateMessage( &msg ), ::DispatchMessage( &msg );
}

void kiWindow::msgLoop( msglooptype type )
{
	kiWindow* wnd;
	MSG msg;
	while( !loopbreaker &&
		  type==GET ?  ::GetMessage( &msg,NULL,0,0 )
					: ::PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )
	{
		if( wnd = app()->mainwnd() )
		{
			if( wnd->m_hAccel )
				if( ::TranslateAccelerator( wnd->hwnd(), wnd->m_hAccel, &msg ) )
					continue;
			if( wnd->isDlgMsg( &msg ) )
				continue;
		}
		::TranslateMessage( &msg ), ::DispatchMessage( &msg );
	}
	loopbreaker = false;
}

void kiWindow::setFront( HWND wnd )
{
	const OSVERSIONINFO& v = app()->osver();

	// Win2000 �ȏ� or Win98 �ȏ�
	if( ( v.dwPlatformId==VER_PLATFORM_WIN32_NT && v.dwMajorVersion>=5 )
	 || ( v.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS &&
							v.dwMajorVersion*100+v.dwMinorVersion>=410 ) )
	{
		DWORD pid;
		DWORD th1 = ::GetWindowThreadProcessId( ::GetForegroundWindow(), &pid );
		DWORD th2 = ::GetCurrentThreadId();
		::AttachThreadInput( th2, th1, TRUE );
		::SetForegroundWindow( wnd );
		::AttachThreadInput( th2, th1, FALSE );
		::BringWindowToTop( wnd );
	}
	else  // �Â�Win
		::SetForegroundWindow( wnd );

	// Special Thanks To kazubon !! ( the author of TClock )
}

void kiWindow::setCenter( HWND wnd, HWND rel )
{
	RECT rc,pr;
	::GetWindowRect( wnd, &rc );

	if( rel )
		::GetWindowRect( rel, &pr );
	else
		::SystemParametersInfo( SPI_GETWORKAREA, 0, &pr, 0 );

	::SetWindowPos( wnd, 0,
		pr.left + ( (pr.right-pr.left)-(rc.right-rc.left) )/2,
		pr.top  + ( (pr.bottom-pr.top)-(rc.bottom-rc.top) )/2,
		0, 0, SWP_NOSIZE|SWP_NOZORDER );
}


//------------------ Window�x�[�X�N���X�Ƃ��Ă̏��� ----------------------//


kiWindow::kiWindow()
{
	m_hWnd = NULL;
	m_hAccel = NULL;
	app()->shellInit();
}

kiWindow::~kiWindow()
{
	if( m_hWnd && ::IsWindow( m_hWnd ) )
	{
		::SetWindowLong( m_hWnd, GWL_USERDATA, 0 );
		::DestroyWindow( m_hWnd );
	}
}

/*void kiWindow::loadAccel( UINT id )
{
	m_hAccel = ::LoadAccelerators( app()->inst(), MAKEINTRESOURCE(id) );
}*/


//---------------- �X�^���h�A������Window�̏��� ---------------------//

// �c�������c

//---------------------------- Dialog -----------------------------//


kiDialog::kiDialog( UINT id )
{
	m_Rsrc = id;
}

void kiDialog::doModal( HWND parent )
{
	setState( true );
	preCreate( this );

	::DialogBoxParam( app()->inst(), MAKEINTRESOURCE(m_Rsrc),
						parent, commonDlg, (LPARAM)this );
}

void kiDialog::createModeless( HWND parent )
{
	setState( false );
	preCreate( this );

	::CreateDialogParam( app()->inst(), MAKEINTRESOURCE(m_Rsrc),
						parent, commonDlg, (LPARAM)this );

	::ShowWindow( hwnd(), SW_SHOW );
	::UpdateWindow( hwnd() );
}

void kiDialog::end( UINT endcode )
{
	setEndCode( endcode );

	if( isModal() )
		::EndDialog( hwnd(), getEndCode() );
	else
		::DestroyWindow( hwnd() );
}

BOOL kiDialog::commonDlg( HWND dlg, UINT msg, WPARAM wp, LPARAM lp )
{
	// kiDialog �C���^�[�t�F�C�X�ւ̃|�C���^���擾
	kiDialog* ptr = (kiDialog*)::GetWindowLong( dlg, GWL_USERDATA );
	if( !ptr ) return FALSE;

	// WM_INITDIALOG �Ȃ� onInit ���Ă�
	if( msg == WM_INITDIALOG )
		return ptr->onInit();

	// OK / Cancel ����
	else if( msg == WM_COMMAND )
	{
		switch( LOWORD(wp) )
		{
		case IDOK:
			if( ptr->onOK() )
				ptr->end( IDOK );
			return TRUE;
		case IDCANCEL:
			if( ptr->onCancel() )
				ptr->end( IDCANCEL );
			return TRUE;
		}
	}

	// ���ʂ̃��b�Z�[�W
	BOOL ans = ptr->proc( msg, wp, lp );

	// WM_DESTORY �Ȃ�E�C���h�E�n���h���؂藣��
	if( msg == WM_DESTROY )
		ptr->detachHwnd();

	return ans;
}


