// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Console.h"
#include <stdio.H>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConsole g_Console;

CConsole::CConsole()
{
	m_pIConsole = NULL;
//	CoInitialize(NULL);
}

CConsole::~CConsole()
{
//	CoUninitialize();
}

BOOL CConsole::Init(int MaxButton,MENU_CUSTOM_INFO* pMenuInfo, cbRetrnFunc commandFunc)
{
	HRESULT hr;
	
	hr = CoCreateInstance(
           CLSID_ULTRA_TCONSOLE,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_ITConsole,
           (void**)&m_pIConsole);

	
	if(FAILED(hr))
		return FALSE;

	LOGFONT logFont;
	logFont.lfHeight		= 17; 
	logFont.lfWidth			= 0; 
	logFont.lfEscapement	= 0; 
	logFont.lfOrientation	= 0; 
	logFont.lfWeight		= FW_BOLD; 
	logFont.lfItalic		= 0; 
	logFont.lfUnderline		= 0; 
	logFont.lfStrikeOut		= 0; 
	logFont.lfCharSet		= HANGUL_CHARSET; 
	logFont.lfOutPrecision	= 0; 
	logFont.lfClipPrecision	= 0; 
	logFont.lfQuality		= 0; 
	logFont.lfPitchAndFamily	= 0; 
	strcpy(logFont.lfFaceName, "Microsoft Jhenghei"); 

	HWND hWnd;
	MHTCONSOLE_DESC	desc;
	desc.szConsoleName = "Darkstory";
	desc.dwRefreshRate = 1000;
	desc.wLogFileType = LFILE_DESTROYLOGFILEOUT;//0;//LFILE_LOGOVERFLOWFILEOUT;

//---��¥���� ������
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/ConsoleLog_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );

	desc.szLogFileName = szFile; //"./Log/ConsoleLog.txt";
	desc.dwFlushFileBufferSize = 10000;
	desc.dwDrawTimeOut	= 1000*60*3;
	desc.wMaxLineNum = 1000;
	desc.dwListStyle = TLO_NOTMESSAGECLOSE|TLO_LINENUMBER|TLO_SCROLLTUMBTRACKUPDATE;
	desc.Width	= 800;
	desc.Height = 400;
	desc.pFont = &logFont;

	desc.nCustomMunuNum = MaxButton;
	desc.cbReturnFunc = commandFunc;
	desc.pCustomMenu = pMenuInfo;
	m_pIConsole->CreateConsole(&desc, &hWnd);

	return TRUE;
}

void CConsole::Release()
{
	if(m_pIConsole)
	{
		m_pIConsole->Release();
		m_pIConsole = NULL;
	}
	
}

void CConsole::Log(int LogType,int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];

	//---LogMsg üũ	//�α� ��½� ������ ��찡 �ִ�. üũ�� ����.
	if( strlen( LogMsg ) > 512 )	// OutputFile()�� ��� �ִ� ���̰� ������? �켱 512�Ѵ°� �ִ��� ����.
	{
		char buff[512] = {0,};
		wsprintf( buff, "%s : %d", "CConsole::Log() LogMsg - too long message : ", strlen( LogMsg ) );
		if( m_pIConsole )
			m_pIConsole->OutputDisplay( buff, strlen( buff ) );

		return;
	}
	//--------------


	va_list vl;

	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	//---va_Temp_Buff üũ	//�α� ��½� ������ ��찡 �ִ�. üũ�� ����.
	if( strlen( va_Temp_Buff ) > 512 )	// OutputFile()�� ��� �ִ� ���̰� ������? �켱 512�Ѵ°� �ִ��� ����.
	{
		char buff[512] = {0,};
		wsprintf( buff, "%s : %d", "CConsole::Log() va_Temp_Buff - too long message : ", strlen( va_Temp_Buff ) );
		if( m_pIConsole )
			m_pIConsole->OutputDisplay( buff, strlen( buff ) );

		return;
	}
	//--------------

	switch(LogType) 
	{
	case eLogDisplay:
		{
			// 080130 LUJ, 	���ڿ��� NULL�� ������ ���� ��� �ܼ��� �������鼭
			//				�������� ����Ǵ� ���� �����ϱ� ���� �ִ� ���ڿ� ����
			//				ũ�⸸ŭ�� ����ϵ��� ��
			if( m_pIConsole )
				m_pIConsole->OutputDisplay(va_Temp_Buff, strlen( va_Temp_Buff ) );
			break;
		}		
	case eLogFile:
		{
			//m_pIConsole->WriteFile(va_Temp_Buff,strlen(va_Temp_Buff),MsgLevel);
		}
		break;
	default:
		MessageBox(NULL,"Not Defined LogType",0,0);
	}
}

void CConsole::LOG(int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];

	//---LogMsg üũ	//�α� ��½� ������ ��찡 �ִ�. üũ�� ����.
	if( strlen( LogMsg ) > 512 )	// OutputFile()�� ��� �ִ� ���̰� ������? �켱 512�Ѵ°� �ִ��� ����.
	{
		char buff[512] = {0,};
		wsprintf( buff, "%s : %d", "CConsole::Log() LogMsg - too long message : ", strlen( LogMsg ) );
		if( m_pIConsole )
			m_pIConsole->OutputDisplay( buff, strlen( buff ) );

		return;
	}
	//--------------
	
	va_list vl;

	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	//---va_Temp_Buff üũ	//�α� ��½� ������ ��찡 �ִ�. üũ�� ����.
	if( strlen( va_Temp_Buff ) > 512 )	// OutputFile()�� ��� �ִ� ���̰� ������? �켱 512�Ѵ°� �ִ��� ����.
	{
		char buff[512] = {0,};
		wsprintf( buff, "%s : %d", "CConsole::Log() va_Temp_Buff - too long message : ", strlen( va_Temp_Buff ) );
		if( m_pIConsole )
			m_pIConsole->OutputDisplay( buff, strlen( buff ) );

		return;
	}
	//--------------


	if( m_pIConsole )
	{
		// 080130 LUJ, 	���ڿ��� NULL�� ������ ���� ��� �ܼ��� �������鼭
		//				�������� ����Ǵ� ���� �����ϱ� ���� �ִ� ���ڿ� ����
		//				ũ�⸸ŭ�� ����ϵ��� ��
		m_pIConsole->OutputDisplay(va_Temp_Buff, strlen( va_Temp_Buff ) );
	}
	
}

void CConsole::WaitMessage()
{
	m_pIConsole->MessageLoop();
}
