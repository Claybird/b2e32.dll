//--- K.I.LIB ---
// kl_cmd.h : commandline parser
//Customized for DLL Parameter Parsing by claybird.

#include "stdafx.h"
#include "kilib.h"


//------------------------ 文字列のメモリ処理など -----------------------//


kiCmdParser::kiCmdParser( char* cmd, bool ignoreFirst )
{
	m_Buffer = NULL;
	if( cmd )
		doit( cmd, ignoreFirst );
}

kiCmdParser::kiCmdParser( const char* cmd, bool ignoreFirst )
{
	m_Buffer=NULL;
	if( cmd )
	{
		m_Buffer = new char[ ki_strlen(cmd)+1 ];
		ki_strcpy( m_Buffer, cmd );
		doit( m_Buffer, ignoreFirst );
	}
}

kiCmdParser::~kiCmdParser()
{
	delete [] m_Buffer;
}


//---------------------------- 分割の処理 -----------------------------//


void kiCmdParser::doit( char* start, bool ignoreFirst )
{
	char* p=start;
	char endc;
	bool first = true;
	bool bSwitchEnd=false;	//trueならこれ以降の-で始まる文字列はスイッチではない

	while( *p!='\0' )
	{
		// 余分な空白はスキップ
		while( *p==' ' || *p=='\t' )//|| *p=='\r' || *p=='\n' )
			p++;

		// " だったら、その旨記録してさらに一個進める
		if( *p=='"' )
			endc='"', p++;
		else
			endc=' ';

		// end-of-text なら終了
		if( *p=='\0' )
			break;

		if( first && ignoreFirst ){	//多分一つ目のオプション(EXEなら自分自身の名前)を無視する
			first = false;
		}else{
			// 引数を保存
			if( *p=='-' && !bSwitchEnd){
				if('-'==*(p+1)){	//"--"が来たら以降に出現する文字列はスイッチではない
					bSwitchEnd=true;
				}/*else{*/
				m_Switch.add( p );
				/*}*/
			}
			else{
				m_Param.add( p );
			}
		}

		// 引数の終わりへ…
		while( *p!=endc && *p!='\0' )
			p++;

		// 終わりは'\0'にすることによって、引数を区切る
		if( *p!='\0' )
			*(p++) = '\0';
	}
}
