//--- K.I.LIB ---
// kl_cmd.h : commandline parser
//Customized for DLL Parameter Parsing by claybird.

#include "stdafx.h"
#include "kilib.h"


//------------------------ ������̃����������Ȃ� -----------------------//


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


//---------------------------- �����̏��� -----------------------------//


void kiCmdParser::doit( char* start, bool ignoreFirst )
{
	char* p=start;
	char endc;
	bool first = true;
	bool bSwitchEnd=false;	//true�Ȃ炱��ȍ~��-�Ŏn�܂镶����̓X�C�b�`�ł͂Ȃ�

	while( *p!='\0' )
	{
		// �]���ȋ󔒂̓X�L�b�v
		while( *p==' ' || *p=='\t' )//|| *p=='\r' || *p=='\n' )
			p++;

		// " ��������A���̎|�L�^���Ă���Ɉ�i�߂�
		if( *p=='"' )
			endc='"', p++;
		else
			endc=' ';

		// end-of-text �Ȃ�I��
		if( *p=='\0' )
			break;

		if( first && ignoreFirst ){	//������ڂ̃I�v�V����(EXE�Ȃ玩�����g�̖��O)�𖳎�����
			first = false;
		}else{
			// ������ۑ�
			if( *p=='-' && !bSwitchEnd){
				if('-'==*(p+1)){	//"--"��������ȍ~�ɏo�����镶����̓X�C�b�`�ł͂Ȃ�
					bSwitchEnd=true;
				}/*else{*/
				m_Switch.add( p );
				/*}*/
			}
			else{
				m_Param.add( p );
			}
		}

		// �����̏I���ցc
		while( *p!=endc && *p!='\0' )
			p++;

		// �I����'\0'�ɂ��邱�Ƃɂ���āA��������؂�
		if( *p!='\0' )
			*(p++) = '\0';
	}
}
