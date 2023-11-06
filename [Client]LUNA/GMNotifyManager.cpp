// GMNotifyManager.cpp: implementation of the CGMNotifyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "GMNotifyManager.h"															// GM���� �Ŵ��� Ŭ���� ����� �ҷ��´�.

#include "Interface/cFont.h"															// ��Ʈ Ŭ���� ����� �ҷ��´�.

#include "MainGame.h"																	// ���� ���� Ŭ���� ����� �ҷ��´�.

#include "GameResourceStruct.h"															// ���� ���ҽ� ����ü ����� �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CGMNotifyManager)																// GM�Ŵ��� Ŭ������ �۷ι� ������ �����Ѵ�.

CGMNotifyManager::CGMNotifyManager()													// GM�Ŵ��� Ŭ���� ������ �Լ�.
{
	Init( MAX_NOTIFYMSG_LINE );															// �ִ� ���� �޽��� �������� �ʱ�ȭ�� �Ѵ�.
	SetPosition( NOTIFYMSG_DEFAULT_X, NOTIFYMSG_DEFAULT_Y, NOTIFYMSG_DEFAULT_WIDTH );	// �⺻ ��� ��ġ�� ��ġ ������ �Ѵ�.
	SetLineSpace( NOTIFYMSG_DEFAULT_LINESPACE );										// �⺻ ���� �������� ���� ������ �����Ѵ�.
	SetFont( NOTIFYMSG_DEFAULT_FONT );													// �⺻ ��Ʈ�� ���� ��Ʈ�� �����Ѵ�.

	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );								// �̺�Ʈ ������ŭ �̺�Ʈ ����Ʈ�� �޸� �� �Ѵ�.

	// 070622 LYW --- GMNotifyManager : Add function to load display part.
	LoadGameDesc() ;																	// ���� ������ �ε��Ѵ�.

}

void CGMNotifyManager::LoadGameDesc()													// ���� ������ �ε��ϴ� �Լ�.
{
	GAMEDESC_INI GameDesc ;																// ���� ������ ��� ����ü�� �����Ѵ�.

	ZeroMemory(&GameDesc, sizeof(GAMEDESC_INI)) ;										// ���� ���� ����ü�� �޸� �� �Ѵ�.

	FILE* fp = fopen("system\\launcher.sav","r");										// ������ ����.

	if( fp )																			// ���� ���Ⱑ ���� �ߴٸ�,
	{
		char buffer[ 100 ];																// �ӽ� ���۸� �����Ѵ�.
		const char* separator = " =\n";													// �и� ��ȣ ������ �����ϰ� �����Ѵ�.

		while( fgets( buffer, sizeof( buffer ), fp ) )									// ��Ʈ���� �ӽ� ���۷� �о���δ�.
		{
			const char* token = strtok( buffer, separator );							// ���ڿ����� �и� ��ȣ�� ���� ��ū�� ã�´�.

			if( 0 == token || ';' == token[ 0 ] )										// ��ū�� �� �����̰ų�, �ڸ�Ʈ���,
			{
				continue;																// ��� �Ѵ�.
			}
			else if( ! strcmpi( "resolution",  token ) )								// ��ū�� ���ַ�ǰ� ���� ������,
			{
				token = strtok( 0, separator );											// ��ū�� �޴´�.

				switch( atoi( token ) )													// ��ū�� ���ڷ� ��ȯ�Ͽ� Ȯ���Ѵ�.
				{
				case 0:																	// 0�� ������,
					{
						GameDesc.dispInfo.dwWidth = 800;								// ���� ���÷��� ������ ���θ� 800���� �����Ѵ�.
						GameDesc.dispInfo.dwHeight = 600;								// ���� ���÷��� ������ ���θ� 600���� �����Ѵ�.
						break;
					}
				case 1:																	// 1�� ������,
					{
						GameDesc.dispInfo.dwWidth = 1024;								// ���� ���÷��� ������ ���θ� 1024���� �����Ѵ�.
						GameDesc.dispInfo.dwHeight = 768;								// ���� ���÷��� ������ ���θ� 768���� �����Ѵ�.
						break;
					}
				case 2:																	// 2�� ������,
					{
						GameDesc.dispInfo.dwWidth = 1280;								// ���� ���÷��� ������ ���θ� 1280���� �����Ѵ�.
						GameDesc.dispInfo.dwHeight = 1024;								// ���� ���÷��� ������ ���θ� 1024���� �����Ѵ�.
						break;
					}
				default:																// �� ���� ��찡 �־�� �ȵȴ�~!!
					{
						GameDesc.dispInfo.dwWidth = _ttoi(token);
						LPCTSTR textHeight = _tcstok(0, separator);
						GameDesc.dispInfo.dwHeight = _ttoi(textHeight ? textHeight : "");
						break;
					}
				}
			}
			else if( ! strcmpi( "windowMode", token ) )									// ��ū�� ������ ���� ������,
			{
				token = strtok( NULL, separator );										// ��ū�� �޴´�.

				GameDesc.dispInfo.dispType = ( ! strcmpi( "true", token ) ? WINDOW_WITH_BLT : FULLSCREEN_WITH_BLT );	// ���÷��� ��带 �����Ѵ�.
			}
		}

		fclose( fp );																	// ������ �ݴ´�.
	}

	m_rcPos.right = GameDesc.dispInfo.dwWidth ;											// ���� ��� ������ ���� ����� ���÷��� ���η� �����Ѵ�.
	m_rcPos.left = 0 ;																	// ���� ��� ������ ������ 0���� �����Ѵ�.
}

CGMNotifyManager::~CGMNotifyManager()													// GM���� �Ŵ��� Ŭ������ �Ҹ��� �Լ�.
{
	Release();																			// ���� �Լ��� ȣ���Ѵ�.
}

void CGMNotifyManager::Init( int nMaxLine )												// �ʱ�ȭ �Լ�.
{
	m_nMaxLine			= nMaxLine;														// �ִ� ���� ���� ���ڷ� �Ѿ�� ���� �����Ѵ�.

	m_bUseEventNotify	= FALSE;														// �̺�Ʈ ���� ��� ���θ� false�� �����Ѵ�.
	ZeroMemory( m_strEventNotifyTitle, 32 );											// �̺�Ʈ ���� ���� ���ڿ��� ���� �޸� �Ѵ�.
	ZeroMemory( m_strEventNotifyContext, 128 );											// �̺�Ʈ ���� ���� ���ڿ��� ���� �޸� �Ѵ�.

	m_bNotifyChanged	= FALSE;														// ���� ���� ���θ� false�� �����Ѵ�.
}

void CGMNotifyManager::Release()														// ���� �Լ�.
{
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();									// ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ ���� �����͸� �����ϰ� ��� ��ġ�� �޴´�.

	while(pos)																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);					// ���� ����ü �����͸� �����ϰ� ��ġ�� ���� ���� ����ü ������ �޴´�.
		SAFE_DELETE(pNotify);															// �����ϰ� ���� ������ �����Ѵ�.
	}
	
	m_ListNotify.RemoveAll();															// ���� ����Ʈ�� ��� ����.
}

void CGMNotifyManager::PushMsg( char* pMsg, DWORD dwColor )								// �޽��� �߰� �Լ�( �޽���, ���� )
{
	sGMNOTIFY* pNotify = new sGMNOTIFY;													// ���� ����ü �����͸� �����Ѵ�.
	
	pNotify->nStrLen = strlen(pMsg);													// ���ڿ� ���̸� �����Ѵ�.

	if( pNotify->nStrLen > MAX_NOTIFYMSG_LENGTH )										// ���ڿ� ���̰� �ִ� ���� ���� ���� ���,
	{
		strncpy( pNotify->strMsg, pMsg, MAX_NOTIFYMSG_LENGTH );							// ���� ��ŭ�� ���� �޽����� �����Ѵ�.
		pNotify->strMsg[MAX_NOTIFYMSG_LENGTH] = NULL;									// ���� �޽��� ���ڿ��� ������ ���� null�� �����Ѵ�.
		pNotify->nStrLen = MAX_NOTIFYMSG_LENGTH;										// ���� �޽��� ���̸� ���� �ְ� �޽��� ���̷� �����Ѵ�.
	}
	else																				// ���ڿ� ���̰� �ִ� ���� ���� ���� ������,
	{
		strcpy( pNotify->strMsg, pMsg );												// ���� �޽����� �����Ѵ�.
	}

	pNotify->dwReceiveTime	= gCurTime;													// ���� �޽��� ���� �ð��� �����Ѵ�.
	pNotify->dwColor		= dwColor;													// ���� �޽��� ������ �����Ѵ�.

	m_ListNotify.AddTail( pNotify );													// ���� �޽��� ����Ʈ�� ���� ����ü ������ �߰��Ѵ�.

	if( m_ListNotify.GetCount() > m_nMaxLine )											// ���� �޽����� �ִ� ������ ������, 
	{
		PopMsg();																		// ���� �޽����� ����.
	}
}

void CGMNotifyManager::PopMsg()															// ���� �޽����� ������ �Լ�.
{
	if( m_ListNotify.GetCount() )														// ���� ����Ʈ ī��Ʈ�� ������,
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();						// ���� ����ü ������ ���� �����͸� �����ϰ� ������ �޴´�.
		SAFE_DELETE( pNotify );															// �����ϰ� ���� ����ü ������ �����Ѵ�.
		m_ListNotify.RemoveHead();														// ���� ����Ʈ ��带 �����.
	}
}

void CGMNotifyManager::AddMsg( char* pMsg, int nClass )									// �޽��� �߰��Լ�( �޽���, ���� Ÿ�� )
{
	int nLen = strlen( pMsg );															// �޽��� ���̸� ���Ѵ�.

	char buf[128];																		// �ӽ� ���۸� �����Ѵ�.
	int nCpyNum = 0;																	// ���� ���̸� ���� ������ �����ϰ� 0���� �����Ѵ�.

	while( nLen > MAX_NOTIFYMSG_PER_LINE )												// �޽��� ���̰� �ִ� �޽��� ���̸� ������,
	{
		if( ( pMsg + MAX_NOTIFYMSG_PER_LINE ) !=										// �޽��� ���ϱ� ���� �� �޽��� �ִ� ���̰�
			CharNext( CharPrev( pMsg, pMsg + MAX_NOTIFYMSG_PER_LINE ) ) )				// ���� ������ ���� ���ڸ� üũ�� 2����Ʈ �����̸�,
		{
			nCpyNum = MAX_NOTIFYMSG_PER_LINE - 1;										// ������̸� ���� �� �ִ� �޽��� ���� -1�� �����Ѵ�.
		}
		else																			// �װ� �ƴϸ�,
		{
			nCpyNum = MAX_NOTIFYMSG_PER_LINE;											// ������̸� ���� �� �ִ� �޽��� ���̷� �����Ѵ�.
		}

		strncpy( buf, pMsg, nCpyNum );													// �ӽ� ���ۿ� ���� ���� ��ŭ �޽����� �����Ѵ�.
		buf[nCpyNum] = 0;																// �������� 0���� �����Ѵ�.
		PushMsg( buf, dwNotifyColor[nClass] );											// ���� ����Ʈ�� ������ �޽����� �߰��Ѵ�.
		nLen -= nCpyNum;																// ���� ���� ��ŭ �޽��� ���̸� ���δ�.
		pMsg += nCpyNum;																// �޽��� �����͸� �̵��Ѵ�.
		if( *pMsg == ' ' ) ++pMsg;														//���� ù���ڰ� �����̽��� �ƴϵ���...				
	}

	if( nLen > 0 )																		// ���̰� 0���� ũ��,
		PushMsg( pMsg, dwNotifyColor[nClass] );											// �޽����� �߰��Ѵ�.
}

void CGMNotifyManager::Render()															// ���� �Լ�.
{
	if( m_ListNotify.GetCount() == 0 ) return;											// ���� ����Ʈ ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )								// ���� ���� ���°� ������ ���°� �ƴϸ�,
	{
		return;																			// ���� ó���� �Ѵ�.
	}

	sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();							// ���� ����ü ������ ���� �����͸� �����ϰ� ����� ������ �޴´�.

	if( gCurTime - pNotify->dwReceiveTime > NOTIFYMSG_LAST_TIME )						// ���� �ð����� �����ð��� ������ 10�ʰ� ������,
	{
		PopMsg();																		// ����Ʈ���� �޽����� ������.
	}

	LONG	lPosY = m_rcPos.top;														// �����ġ ���̸� ���� ������ �����ϰ�, ��� ���� ��Ʈ�� ž���� �����Ѵ�.
	LONG	lPosX;																		// ��� ��ġ�� X��ǥ�� ���� ������ �����Ѵ�.
	LONG	lTextWidth;																	// �ؽ�Ʈ�� ���̸� ���� ������ �����Ѵ�.
	RECT	rect;																		// ��� ������ ���� ��Ʈ�� �����Ѵ�.

	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();									// ��ġ ������ ���� ������ ������ �����ϰ�, ���� ����Ʈ�� ��带 �޴´�.

	while(pos)																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);					// ���� ����ü ������ ���� �����͸� �����ϰ� ��ġ�� ���� ������ �޴´�.

		lTextWidth = CFONT_OBJ->GetTextExtentEx( 5, pNotify->strMsg, pNotify->nStrLen );// ��Ʈ��, �޽���, ���̿� ���� �ؽ�Ʈ ���� ���̸� ���Ѵ�.

		lPosX = m_rcPos.left + (m_rcPos.right - lTextWidth)/2 ;							// ��� ��ġ X��ǥ�� �޴´�.

		SetRect( &rect, lPosX, lPosY, lPosX + lTextWidth, lPosY + 1 );					// ��� ��ġ�� �����Ѵ�.

		CFONT_OBJ->RenderFont( 0, pNotify->strMsg, pNotify->nStrLen, &rect, RGB_HALF( 30, 30, 30 ) );	// ���ڸ� ����Ѵ�.(�׸���)
		OffsetRect( &rect, -1, -1 );													// �����ġ �ɼ��� �Ѵ�.
		CFONT_OBJ->RenderFont( 0, pNotify->strMsg, pNotify->nStrLen, &rect, RGB_HALF( 255, 255, 255 ) );// �ٽ� �ѹ� ���ڸ� ����Ѵ�.(�⺻ �ؽ�Ʈ)
		lPosY += m_nLineSpace + 10;														// ���� ���� 10���� ��Ų��.
	}
}

void CGMNotifyManager::SetPosition( LONG lx, LONG ly, LONG lWidth )						// ��ġ ���� �Լ�.
{
	m_rcPos.left	= lx;																// �����ġ left�� ���� lx�� �����Ѵ�.
	m_rcPos.right	= lx + lWidth;														// �����ġ right�� ���� lx + lWidth�� �����Ѵ�.
	m_rcPos.top		= ly;																// �����ġ top�� ���� ly�� �����Ѵ�.
	m_rcPos.bottom	= m_rcPos.top + 1;													// �����ġ bottom�� ž +1�� �����Ѵ�.
}

void CGMNotifyManager::SetEventNotifyStr( char* pStrTitle, char* pStrContext )			// �̺�Ʈ ���� ���ڿ��� �����ϴ� �Լ�.
{	
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );									// �̺�Ʈ ���� ������ �����Ѵ�.
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );							// �̺�Ʈ ���� ������ �����Ѵ�.
}

void CGMNotifyManager::SetEventNotify( BOOL bUse )										// �̺�Ʈ ������ ��� ���θ� �����ϴ� �Լ�.
{	
	m_bUseEventNotify = bUse;															// ���ڷ� �Ѿ�� ������ �̺�Ʈ ������ ��� ���θ� �����Ѵ�.
}
