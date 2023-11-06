//=================================================================================================
//	FILE		: cMsgBox.cpp
//	PURPOSE		: Implemantacion part of message box interface.
//	PROGRAMMER	: Yongs Lee
//	DATE		: Modified January 19, 2008
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		필요한 헤더파일을 불러온다.
//=========================================================================
#include "stdafx.h"
#include "WindowIDEnum.h"

#include "../[CC]Header/GameResourceManager.h"
#include "ChatManager.h"

#include "./input/cImeWnd.h"
#include "./Input/Keyboard.h"
#include "cIMEex.h"

#include "./interface/cButton.h"
#include "./Interface/cFont.h"
#include "cMsgBox.h"
#include "./Interface/cResourceManager.h"
#include "./interface/cScriptManager.h"
#include "./interface/cStatic.h"
#include "./interface/cTextArea.h"
#include "./interface/cWindowManager.h"


cImage	cMsgBox::m_DlgImg;
cImage	cMsgBox::m_BtnImgBasic;
cImage	cMsgBox::m_BtnImgOver;
cImage	cMsgBox::m_BtnImgPress;
char	cMsgBox::m_BtnText[MB_BTN_COUNT][32]		= {0, };
BOOL	cMsgBox::m_bInit = FALSE;

// 080401 LYW --- cMsgBox : Change text colors of button.
//DWORD	cMsgBox::m_dwColor[3] = { 0x000a0a0a, 0x00ffff00, 0x00ffff00 };
DWORD	cMsgBox::m_dwColor[3] = { 0x00ffffff, 0x00ffff00, 0x00ffff00 };





//=========================================================================
//	NAME : cMsgBox
//	DESC : The function construc constructor.
//=========================================================================
cMsgBox::cMsgBox()
{
	m_type			= WT_MSGBOX ;											// 윈도우 타입을 세팅한다.
	m_nMBType		= MBT_NOBTN ;											// 메시지 박스 타입을 세팅한다.

	m_pMsg			= NULL ;
	m_pButton[0]	= NULL ;
	m_pButton[1]	= NULL ;

	m_dwMsgColor	= 0x00ffffff ;

	m_nDefaultBtn	= -1 ;
	m_dwParam		= 0 ;

	m_nWidth		= 200 ;
	m_nHeight		= 115 ;

	m_nBtn1_Y		= 78 ;
	m_nBtn2_Y		= 78 ;

	m_nBtnW			= 64 ;
	m_nBtnH			= 23 ;

	m_nText_X		= 15 ;
	m_nText_Y		= 42 ;
}





//=========================================================================
//	NAME : ~cMsgBox
//	DESC : The function construc destructor.
//=========================================================================
cMsgBox::~cMsgBox()
{
}





//=========================================================================
//	NAME : InitMsgBox
//	DESC : The function to initialize message box.
//=========================================================================
void cMsgBox::InitMsgBox()
{
	SCRIPTMGR->GetImage( 30, &m_DlgImg ) ;									// 다이얼로그 이미지 세팅.
	SCRIPTMGR->GetImage( 31, &m_BtnImgBasic ) ;								// 버튼 기본 이미지 세팅.
	SCRIPTMGR->GetImage( 32, &m_BtnImgOver ) ;								// 버튼 오버 이미지 세팅.
	SCRIPTMGR->GetImage( 33, &m_BtnImgPress ) ;								// 버튼 프레스 이미지 세팅.

	strcpy( m_BtnText[MB_BTN_OK],		RESRCMGR->GetMsg( 11 ) ) ;			// 버튼 [ 확인 ] 메시지 세팅.
	strcpy( m_BtnText[MB_BTN_YES],		RESRCMGR->GetMsg( 268 ) ) ;			// 버튼 [ 예 ] 메시지 세팅.
	strcpy( m_BtnText[MB_BTN_NO],		RESRCMGR->GetMsg( 269 ) ) ;			// 버튼 [ 아니오 ] 메시지 세팅.
	strcpy( m_BtnText[MB_BTN_CANCEL],	RESRCMGR->GetMsg( 12 ) ) ;			// 버튼 [ 취소 ] 메시지 세팅.

	m_bInit = TRUE ;														// 초기화 여부를 true로 세팅.
}





//=========================================================================
//	NAME : MsgBox
//	DESC : The function to setting information of message box.
//=========================================================================
void cMsgBox::MsgBox( LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc)
{
	//--------------------------------------------------------------------------
	// Check parameters.
	if( !strMsg ) return ;

	if( strlen(strMsg) == 0 ) return ;

	m_nMBType = nMBType ;

	//--------------------------------------------------------------------------
	// Setting display info.

	DISPLAY_INFO	dispInfo ;													// DISPLAY 정보 구조체를 선언.

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;									// DISPLAY 정보를 받는다.

	int nScrWidth		= dispInfo.dwWidth ;									// DISPLAY 가로, 세로 정보를 받는다.
	int nScrHeight		= dispInfo.dwHeight ;

	int nMsgBoxWidth	= m_nWidth  ;											// 메시지 박스의 가로, 세로 길이를 세팅한다.
	int nMsgBoxHeight	= m_nHeight ;

	int nBtn1_Y			= m_nBtn1_Y ;											// 버튼1, 2의 높이를 세팅한다.
	int nBtn2_Y			= m_nBtn2_Y ;

	int nTextLine		= 0 ;													// 텍스트 라인을 세팅한다.

	SCRIPTMGR->GetImage( 30, &m_DlgImg ) ;										// 기본 배경. AUTOSIZE 타입의 메세지 박스 출력 후에 항상 필요하기 때문에 여기서

	//--------------------------------------------------------------------------
	// Setting Text area.
	m_pMsg = new cTextArea ;													// 텍스트 라인 수를 구하기 위해 텍스트 먼저 처리

	int  nTextWidth	= nMsgBoxWidth - m_nText_X ;
	RECT rtText		= { 0, 0, 196 - m_nText_X * 2, 200 } ;

	m_pMsg->SetFontIdx( 0 ) ;
	m_pMsg->SetFontColor( RGB(96, 98, 1) ) ;

	m_pMsg->Init(m_nText_X, m_nText_Y, nTextWidth, 200, NULL ) ;

	m_pMsg->InitTextArea( &rtText, 256 ) ;

	m_pMsg->SetAlpha((BYTE)m_alpha) ;
	m_pMsg->SetScriptText( strMsg ) ;
	m_pMsg->SetActive(m_bActive) ;	

	nTextLine = m_pMsg->GetIMEex()->GetTotalLine() ;

	//--------------------------------------------------------------------------
	// Setting Dialog info.
	cImageRect* pRect = NULL ;

	cImage middleBg ;

	WORD wWidth	 = 0 ;
	WORD wHeight = 0 ;

	//--------------------------------------------------------------------------
	// ..상단 이미지
	SCRIPTMGR->GetImage( 119, &m_DlgImg ) ;
	pRect = m_DlgImg.GetImageRect() ;

	nMsgBoxHeight = (int)(pRect->bottom - pRect->top) ;

	//--------------------------------------------------------------------------
	// ..중간 이미지 - 상단 이미지, 중간 이미지, 텍스트 출력 y 좌표가 서로 딱 맞아 떨어지지가 않기 때문에 줄 수에 정확히 맞추기 위해서 따로 추가..
   	cStatic* pMiddle01Bg = new cStatic ;

	SCRIPTMGR->GetImage( 120, &middleBg ) ;
	pRect = middleBg.GetImageRect() ;

	wWidth  = (WORD)(pRect->right-pRect->left) ;
	wHeight = (WORD)(pRect->bottom-pRect->top) ;

	pMiddle01Bg->Init( 0, nMsgBoxHeight, wWidth, wHeight, &middleBg, -1 ) ;
	cImageScale scale0 = {1.0f, 3.0f} ;
 	pMiddle01Bg->SetScale(&scale0) ;
 	Add( pMiddle01Bg ) ;

	nMsgBoxHeight += (int)((pRect->bottom-pRect->top-1)*scale0.y) ;

	//--------------------------------------------------------------------------
	// ..중간 이미지 - 텍스트 공간 만큼의 크기로 설정
   	cStatic* pMiddle02Bg = new cStatic ;

	SCRIPTMGR->GetImage( 120, &middleBg ) ;
	pRect = middleBg.GetImageRect() ;

	wWidth  = (WORD)(pRect->right-pRect->left) ;
	wHeight = (WORD)(pRect->bottom-pRect->top) ;

	pMiddle02Bg->Init( 0, nMsgBoxHeight, wWidth, wHeight, &middleBg, -1 ) ;
	cImageScale scale = {1.0f, 4.0f} ;
	scale.y *= nTextLine ;
 	pMiddle02Bg->SetScale(&scale) ;
 	Add( pMiddle02Bg ) ;

	nMsgBoxHeight += (int)((pRect->bottom-pRect->top-1)*scale.y) ;

	nBtn1_Y = nMsgBoxHeight-7 ;
	nBtn2_Y = nMsgBoxHeight-7 ;

	//--------------------------------------------------------------------------
	// ..하단 이미지
	cStatic* pBottomBg = new cStatic ;

	SCRIPTMGR->GetImage( 121, &middleBg) ;
	pRect = middleBg.GetImageRect() ;

	wWidth  = (WORD)(pRect->right-pRect->left) ;
	wHeight = (WORD)(pRect->bottom-pRect->top) ;

	pBottomBg->Init( 0, nMsgBoxHeight, wWidth, wHeight, &middleBg, -1 );
	Add( pBottomBg ) ;

	nMsgBoxHeight += (int)(pRect->bottom-pRect->top) ;

	switch(nMBType)
	{
	case MBT_OK:
		{
			LONG btn1x = (nMsgBoxWidth - m_nBtnW)/2;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, nBtn1_Y, m_nBtnW, m_nBtnH, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_OK );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_OK], m_dwColor[0], m_dwColor[1], m_dwColor[2] );
			m_pButton[0]->SetTextXY(0, 6) ;
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
		}
		break;

	case MBT_YESNO:
		{
			LONG btn1x = (nMsgBoxWidth - (m_nBtnW*2) - 6)/2;
			LONG btn2x = btn1x + m_nBtnW + 6;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( 24, nBtn2_Y, m_nBtnW, m_nBtnH, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_YES );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_YES], m_dwColor[0], m_dwColor[1], m_dwColor[2] );
			m_pButton[0]->SetTextXY(0, 6) ;
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );

			m_pButton[1] = new cButton;
			m_pButton[1]->Init( 111, nBtn1_Y, m_nBtnW, m_nBtnH, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_NO );

			m_pButton[1]->SetText( m_BtnText[MB_BTN_NO], m_dwColor[0], m_dwColor[1], m_dwColor[2] );
			m_pButton[1]->SetTextXY(0, 6) ;
			m_pButton[1]->SetAlign( TXT_CENTER );
			m_pButton[1]->SetActive( m_bActive );
			m_pButton[1]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[1] );
		}
		break;

	case MBT_CANCEL:
		{
			LONG btn1x = (nMsgBoxWidth - m_nBtnW)/2;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, nBtn1_Y, m_nBtnW, m_nBtnH, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_CANCEL );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_CANCEL], m_dwColor[0], m_dwColor[1], m_dwColor[2] );
			m_pButton[0]->SetTextXY(0, 6) ;
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
		}
		break;

	default : break ;
	}

	// ..배경을 먼저 출력해야 하기 때문에 텍스트 추가는 여기서
	Add( m_pMsg );

	RECT rtCaption = { 0, 0, nMsgBoxWidth, 20 };
	cDialog::Init( ( nScrWidth - nMsgBoxWidth ) / 2, ( nScrHeight - nMsgBoxHeight ) / 2,
					nMsgBoxWidth, nMsgBoxHeight, &m_DlgImg, lId );

	SetMovable(TRUE);
	SetCaptionRect(&rtCaption);

	m_cbMBFunc = cbMBFunc;
	
	cStatic* pStatic = new cStatic;
	pStatic->Init( 15, 16, 0, 0, NULL, -1 );
	pStatic->SetStaticText( RESRCMGR->GetMsg( 261 ) );
	pStatic->SetShadow( TRUE );
	pStatic->SetFontIdx( 0 );
	pStatic->SetAlign( TXT_LEFT );
	Add( pStatic );

	m_type = WT_MSGBOX;
}






//=========================================================================
//	NAME : ActionEvent
//	DESC : The function to process mouse event.
//=========================================================================
DWORD cMsgBox::ActionEvent( CMouse * mouseInfo )
{
	DWORD we	= WE_NULL ;													// 다이얼로그 이벤트 변수 선언.
	DWORD weBtn = WE_NULL ;													// 버튼 이벤트 변수 선언.
	
	if( !m_bActive ) return we ;											// 메시지 박스가 비활성화 상태면, return we.

	we |= cWindow::ActionEvent( mouseInfo ) ;								// 다이얼로그의 이벤트를 받는다.
	we |= cDialog::ActionEventWindow( mouseInfo) ;							// 버튼 이벤트를 받는다.

	for( int i = 0 ; i < MB_MAX_BTN_COUNT ; ++ i )							// 최대 버튼 수 만큼 for문을 돌린다.
	{
		if( !m_pButton[i] ) continue ;										

		weBtn = m_pButton[i]->ActionEvent( mouseInfo ) ;					// 버튼의 이벤트를 받는다.

		if( weBtn & WE_BTNCLICK )											// 버튼을 클릭했다면,
		{
			ForcePressButton(m_pButton[i]->GetID()) ;						// 버튼 눌림 처리를 실행하고 for문을 빠져나간다.
			break ;
		}
	}

	return we ;																// 이벤트를 return 한다.
}





//=========================================================================
//	NAME : ActionKeyboardEvent
//	DESC : The function to process keyboard event.
//=========================================================================
DWORD cMsgBox::ActionKeyboardEvent( CKeyboard * keyInfo )
{
	DWORD we = WE_NULL ;													// 이벤트를 담을 변수를 선언한다.
	return we ;																// 이벤트를 return 한다.
}





//=========================================================================
//	NAME : PressDefaultBtn
//	DESC : The function to process press event of default button.
//=========================================================================
BOOL cMsgBox::PressDefaultBtn()
{
	if( m_nDefaultBtn < 0 || m_nDefaultBtn >= MB_MAX_BTN_COUNT )			// DefaultBtn의 아이디 범위를 체크한다.
	{
		return FALSE ;
	}

	if( m_pButton[m_nDefaultBtn] )											// 버튼의 정보가 유효하면,
	{
		return ForcePressButton( m_pButton[m_nDefaultBtn]->GetID() ) ;		// 해당 버튼의 Press 이벤트 처리를 한다.
	}

	return FALSE ;
}





//=========================================================================
//	NAME : ForcePressButton
//	DESC : The function to process press event of buttons.
//=========================================================================
BOOL cMsgBox::ForcePressButton( LONG lBtnID )
{
	if( lBtnID <= 0 ) return FALSE ;										// 버튼 아이디의 범위를 체크한다.

	if( !m_cbMBFunc ) return FALSE ;										// callback 함수의 정보를 체크한다.

	m_cbMBFunc( m_ID, this, lBtnID ) ;										// callback 함수를 실행한다.

	SetActive( FALSE ) ;													// 메시지 박스를 닫는다.
	SetDisable( TRUE ) ;													// 메시지 박스를 비활성화 한다.

	WINDOWMGR->AddListDestroyWindow( this ) ;								// 윈도우 매니져에 해제 윈도우로 등록한다.

	return TRUE ;
}





//=========================================================================
//	NAME : ForceClose
//	DESC : The function to process close event of buttons.
//=========================================================================
void cMsgBox::ForceClose()	
{
	switch( m_nMBType )														// 메시지 박스의 타입을 확인한다.
	{
	case MBT_NOBTN :	ForcePressButton( 0 ) ;				break ;
	case MBT_OK :		ForcePressButton( MBI_OK ) ;		break ;
	case MBT_YESNO :	ForcePressButton( MBI_NO ) ;		break ;
	case MBT_CANCEL :	ForcePressButton( MBI_CANCEL ) ;	break ;
	default :			/* default 체크를 꼭~!! 하자. */	break ;
	}
}
