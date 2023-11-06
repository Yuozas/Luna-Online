//---BROWSER
#include "stdafx.h"
#include ".\cbrowser.h"
#include "../[CC]Header/GameResourceManager.h"

extern HWND _g_hWnd;

cBrowser::cBrowser(void)
{
	m_type			= WT_BROWSER;

	m_szURL[0]		= 0;
	m_hwndIE		= NULL;
}

cBrowser::~cBrowser(void)
{
	if( m_hwndIE )
	{
		SendMessage( m_hwndIE, WM_CLOSE, 0, 0 );
	}
}


void cBrowser::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * pBasicImage, LONG ID)
{
	if( m_hwndIE != NULL ) return;

	cWindow::Init( x, y, wid, hei, NULL, ID );

	DWORD dwStyle = WS_CHILD;
	DISPLAY_INFO disp;
	GAMERESRCMNGR->GetDispInfo( &disp );
	if( disp.dispType == FULLSCREEN_WITH_BLT )
		dwStyle = WS_POPUP;

	m_hwndIE = CreateWindow("AtlAxWin71", 
							"about:blank", dwStyle, 
							m_absPos.x, m_absPos.y, m_width, m_height, 
							_g_hWnd, (HMENU)0, GetModuleHandle(NULL), NULL);

	if( m_hwndIE )
	{
		CComPtr<IUnknown>    punkIE;
		if(AtlAxGetControl(m_hwndIE, &punkIE) == S_OK) 
			m_pWebBrowser = punkIE;
	}
}

void cBrowser::SetAbsXY(LONG x, LONG y)
{
	m_absPos.x=(float)x;
	m_absPos.y=(float)y;
	m_bIsMovedWnd = TRUE ;

//	SetWindowPos( m_hwndIE, NULL, x, y, m_width, m_height, SWP_NOSIZE );
	MoveWindow( m_hwndIE, x, y, m_width, m_height, TRUE );
}

void cBrowser::Navigate( char* pURL )
{
	SafeStrCpy( m_szURL, pURL, 1024 );
	
	if( m_pWebBrowser ) 
	{ 
		LPCONNECTIONPOINTCONTAINER pCPC = NULL;
		LPCONNECTIONPOINT pCP = NULL;

		m_pWebBrowser->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
		pCPC->FindConnectionPoint(__uuidof(SHDocVw::DWebBrowserEventsPtr), &pCP);

		DWORD dwCookie;
		pCP->Advise((LPUNKNOWN)&m_events, &dwCookie);

		pCPC->Release();

//		CComVariant vempty, vUrl(m_szURL);
		CComVariant vempty;

		m_pWebBrowser->Navigate( _bstr_t(m_szURL), &vempty, &vempty, &vempty, &vempty);             

		pCP->Unadvise(dwCookie);
		pCP->Release();
	}
}


//---�߿�!!! ���̾�αװ� ��Ƽ����°� �ƴ϶� �׷��� ������ ���� ���� ���������̵鵵 �׷����� ������,
//---�� �������� �����찡 �ߴ� ���̶� �� ��Ƽ����¸� ������Ѵ�.
//---���� cDialog::SetActiveRecursive() �� ����϶�. 
//---void CItemShopDlg::SetActive( BOOL val ) ����.
void cBrowser::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	
	m_bActive = val;

	if( m_bActive )
	{
		ShowWindow( m_hwndIE, SW_SHOW );
	}
	else
	{
		ShowWindow( m_hwndIE, SW_HIDE );
	}
}

