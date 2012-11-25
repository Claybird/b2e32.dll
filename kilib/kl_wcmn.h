//--- K.I.LIB ---
// kl_wcmn.h : windows-common-interface operatin

#ifndef AFX_KIWINCOMMON_H__0686721C_CAFB_4C2C_9FE5_0F482EA6A60B__INCLUDED_
#define AFX_KIWINCOMMON_H__0686721C_CAFB_4C2C_9FE5_0F482EA6A60B__INCLUDED_

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// シェルユーティリティークラス

class kiSUtil
{
public:

	// ファイルが存在する？
	static bool exist( const char* fname );
	static bool isdir( const char* fname );
};

#endif
