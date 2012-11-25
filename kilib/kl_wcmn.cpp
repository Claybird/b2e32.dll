//--- K.I.LIB ---
// kl_wcmn.h : windows-common-interface operatin

#include "stdafx.h"
#include "kilib.h"


bool kiSUtil::exist( const char* fname )
{
	return 0xffffffff != ::GetFileAttributes( fname );
}

bool kiSUtil::isdir( const char* fname )
{
	DWORD attr = ::GetFileAttributes( fname );
	return attr!=0xffffffff && (attr&FILE_ATTRIBUTE_DIRECTORY);
}
