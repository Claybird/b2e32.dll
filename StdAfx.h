#if !defined(__STDAFX_H__)
#define __STDAFX_H__

//define for STLport
#ifdef NDEBUG
    #undef _STLP_DEBUG
#else
    #pragma warning(disable:4786)
    #define _STLP_DEBUG 1
#endif

// この位置にヘッダーを挿入してください
#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

// windows general header
#undef  WINVER
#define WINVER    0x0500
//#undef  _WIN32_IE
//#define _WIN32_IE 0x0200
#include <windows.h>
#include <time.h>
// shell header
#include <shlobj.h>
#include <shlwapi.h>

// k.i.lib
#include "kilib/kilibext.h"


#endif // !defined(__STDAFX_H__)
