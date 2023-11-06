#include "stdafx.h"
#include "OptionDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cCheckBox.h"
#include "./interface/cStatic.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "ObjectManager.h"
#include "./interface/cScriptManager.h"
#include "ChatManager.h"

#include "TutorialManager.h"


COptionDialog::COptionDialog()
{
}

COptionDialog::~COptionDialog()
{
}

void COptionDialog::Add( cWindow* window )
{
	WORD wWindowType = window->GetType() ;																		// 윈도우 타입을 받아온다.

	if( wWindowType == WT_PUSHUPBUTTON )																		// 윈도우 타입이 푸쉬업 버튼이면,
	{
		AddTabBtn( curIdx1++, (cPushupButton*)window ) ;														// 현재 인덱스1로 탭 버튼을 추가한다.
	}
	else if( wWindowType == WT_DIALOG )																			// 윈도우 타입이 다이얼로그이면,
	{
		AddTabSheet( curIdx2++, window ) ;																		// 현재 인덱스2로 쉬트를 추가한다.
	}
	else																										// 이도 저도 아니면,
	{
		cTabDialog::Add( window ) ;																				// 윈도우를 추가한다.
	}
}

void COptionDialog::SetActive( BOOL val )
{
	if( !m_bDisable )																							// 비활성화 상태가 아니면,
	{
		if( val )																								// val 값이 TRUE 이면,
		{
			m_GameOption = *( OPTIONMGR->GetGameOption() ) ;													// 옵션 매니져의 게임 옵션을 받아온다.

			UpdateData( FALSE ) ;																				// FALSE 모드로 업데이트를 한다.					
		}

		cTabDialog::SetActive( val ) ;																			// 탭다이얼 로그를 val 세팅한다.
	}
}

void COptionDialog::Linking()
{
}

void COptionDialog::UpdateData( BOOL bSave )
{
	cDialog* pDlg ;

	if( bSave )
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	

		m_GameOption.bNoDeal		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->IsChecked();
		m_GameOption.bNoParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->IsChecked();
		m_GameOption.bNoFriend		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->IsChecked();
		m_GameOption.bNoShowdown	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->IsChecked();
		m_GameOption.bNameMunpa		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->IsChecked();
		m_GameOption.bNameParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->IsChecked();
		m_GameOption.bNameOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->IsChecked();
		m_GameOption.bNoMemberDamage	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->IsChecked();
		m_GameOption.bNoGameTip		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->IsChecked();
		m_GameOption.nMacroMode		= ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->IsPushed();
		
		pDlg = (cDialog*)GetTabSheet( 1 );

		m_GameOption.bNoWhisper		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->IsChecked();
		m_GameOption.bNoChatting	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->IsChecked();
		m_GameOption.bNoBalloon		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->IsChecked();
		m_GameOption.bNoPartyChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTYCHAT ))->IsChecked() ;
		m_GameOption.bNoFamilyChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFAMILYCHAT ))->IsChecked() ;
		m_GameOption.bNoShoutChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->IsChecked();
		m_GameOption.bNoGuildChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->IsChecked();
		m_GameOption.bNoAllianceChat= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->IsChecked();
		m_GameOption.bNoSystemMsg	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->IsChecked();
		m_GameOption.bNoExpMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->IsChecked();
		m_GameOption.bNoItemMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->IsChecked();

		pDlg = (cDialog*)GetTabSheet( 2 ); // 옵션에서 그래픽설정부분이다.

		m_GameOption.nSightDistance = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->GetCurValue(); // 시야거리
		m_GameOption.bShadowHero	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->IsChecked(  ); // 자신
		m_GameOption.bShadowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->IsChecked(  ); // 몬스터/NPC
		m_GameOption.bShadowOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->IsChecked( ); // 다른케릭터

#ifndef _JAPAN_LOCAL_
		m_GameOption.bAutoCtrl		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->IsChecked() ;

		m_GameOption.nLODMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed(); 
		m_GameOption.nEffectMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->IsPushed();
#endif //_JAPAN_LOCAL_

		pDlg = (cDialog*)GetTabSheet( 3 );

		m_GameOption.bSoundBGM			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->IsChecked();
		m_GameOption.bSoundEnvironment	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->IsChecked();
		m_GameOption.nVolumnBGM			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->GetCurValue();
		m_GameOption.nVolumnEnvironment = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->GetCurValue();


		// 071025 LYW --- OptionDialog : Setting tutorial option.
		m_GameOption.bShowTutorial		= TUTORIALMGR->Is_ActiveTutorial() ;				// 튜토리얼 옵션을 세팅한다.

		OPTIONMGR->SetGameOption( &m_GameOption );
	}
	else	//정보 가지고 와서 창 세팅 (기본설정이 창이 뜸)
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->SetChecked( m_GameOption.bNoDeal );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->SetChecked( m_GameOption.bNoParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->SetChecked( m_GameOption.bNoFriend );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->SetChecked( m_GameOption.bNoShowdown );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->SetChecked( m_GameOption.bNameMunpa );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->SetChecked( m_GameOption.bNameParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->SetChecked( m_GameOption.bNameOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->SetChecked( m_GameOption.bNoMemberDamage );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->SetChecked( m_GameOption.bNoGameTip );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( !m_GameOption.nMacroMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( m_GameOption.nMacroMode );
		
		pDlg = (cDialog*)GetTabSheet( 1 );	

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->SetChecked( m_GameOption.bNoWhisper );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->SetChecked( m_GameOption.bNoChatting );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->SetChecked( m_GameOption.bNoBalloon );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTYCHAT ))->SetChecked( m_GameOption.bNoPartyChat ) ;
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFAMILYCHAT ))->SetChecked( m_GameOption.bNoFamilyChat ) ;
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->SetChecked( m_GameOption.bNoShoutChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->SetChecked( m_GameOption.bNoGuildChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->SetChecked( m_GameOption.bNoAllianceChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->SetChecked( m_GameOption.bNoSystemMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->SetChecked( m_GameOption.bNoExpMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->SetChecked( m_GameOption.bNoItemMsg );

		pDlg = (cDialog*)GetTabSheet( 2 );

		if( m_GameOption.nSightDistance <= 55 )																// 게이지 값이 최소값인 55보다 작으면,
		{
			m_GameOption.nSightDistance = 55 ;																// 게이지 값을 55로 세팅해, 0이되거나, 
		}																									// 완전 시야를 가리는 일을 막는다.

		//m_GameOption.nSightDistance = 155 ;
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->SetCurValue( m_GameOption.nSightDistance );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->SetChecked( m_GameOption.bShadowHero );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetChecked( m_GameOption.bShadowMonster );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetChecked( m_GameOption.bShadowOthers );

#ifndef _JAPAN_LOCAL_
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->SetChecked( m_GameOption.bAutoCtrl );

		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( !m_GameOption.nLODMode ) ;
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( m_GameOption.nLODMode ) ;
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( !m_GameOption.nEffectMode ) ;
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( m_GameOption.nEffectMode ) ;

		DisableGraphicTab( m_GameOption.bAutoCtrl ) ;
#endif	//_JAPAN_LOCAL_

		pDlg = (cDialog*)GetTabSheet( 3 );

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->SetChecked( m_GameOption.bSoundBGM );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->SetChecked( m_GameOption.bSoundEnvironment );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->SetCurValue( m_GameOption.nVolumnBGM );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->SetCurValue( m_GameOption.nVolumnEnvironment );

		// 071025 LYW --- OptionDialog : Setting tutorial option.
		TUTORIALMGR->Set_ActiveTutorial(m_GameOption.bShowTutorial) ;				// 튜토리얼 옵션을 세팅한다.
	}

	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);

	BOOL bDisable = TRUE ;

	if( bDisable )
	{
		FullColor = HalfColor;
	}

	pDlg = (cDialog*)GetTabSheet( 0 );
	cCheckBox* pCheckBox = NULL ;

	pCheckBox = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND )) ;
	if( pCheckBox )
	{
		pCheckBox->SetChecked(FALSE) ;

		pCheckBox->SetDisable(bDisable) ;

		pCheckBox->SetImageRGB( FullColor ) ;
	}

/*	pCheckBox = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP )) ;
	if( pCheckBox )
	{
		pCheckBox->SetDisable(bDisable) ;

		pCheckBox->SetImageRGB( FullColor ) ;
	}
*/
	pCheckBox = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR )) ;
	if( pCheckBox )
	{
		pCheckBox->SetDisable(bDisable) ;

		pCheckBox->SetImageRGB( FullColor ) ;
	}

	//pDlg = (cDialog*)GetTabSheet(2) ;
	//cGuageBar* pGuageBar = (cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ) ;

	//if( pGuageBar )
	//{
	//	//pGuageBar->SetCurValue(155) ;

	//	pGuageBar->SetDisable(TRUE) ;

	//	pGuageBar->SetImageRGB( FullColor ) ;

	//	pGuageBar->SetGuageLock(TRUE, FullColor);

	//	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(FullColor);
	//}
}

void COptionDialog::OnActionEvent( LONG lId, void* p, DWORD we )
{
	if( we & WE_BTNCLICK )
	{
		if( lId == OTI_BTN_OK )
		{
			UpdateData( TRUE );
			OPTIONMGR->ApplySettings();
			OPTIONMGR->SaveGameOption();
			OPTIONMGR->SendOptionMsg();	
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_CANCEL )
		{
			OPTIONMGR->CancelSettings();
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_RESET )
		{
			OPTIONMGR->SetDefaultOption();
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			UpdateData( FALSE );
		}
		else if( lId == OTI_BTN_PREVIEW )
		{
			UpdateData( TRUE );
			OPTIONMGR->ApplySettings();
			OPTIONMGR->SaveGameOption();
			OPTIONMGR->SendOptionMsg();		
		}
	}
	
	if( we & WE_PUSHUP )		//라디오 버튼 처리
	{
		if( lId == OTI_PB_CHATMODE || lId == OTI_PB_MACROMODE )
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( lId ))->SetPush( TRUE );

#ifndef _JAPAN_LOCAL_
		if( lId == OTI_PB_BASICGRAPHIC || lId == OTI_PB_DOWNGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet(2))->GetWindowForID(lId))->SetPush(TRUE) ;
		}
		if( lId == OTI_PB_BASICEFFECT || lId == OTI_PB_ONEEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
		}
#endif //_JAPAN_LOCAL_
	}

	if( we & WE_PUSHDOWN )		//
	{
		if( lId == OTI_PB_CHATMODE )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_MACROMODE )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( FALSE );
		}

#ifndef _JAPNA_LOCAL_
		if( lId == OTI_PB_BASICGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_DOWNGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( FALSE );
		}
		if( lId == OTI_PB_BASICEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_ONEEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( FALSE );
		}
#endif //_JAPNA_LOCAL_
	}

#ifndef _JAPAN_LOCAL_
	if( we & WE_CHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(TRUE) ;
		}
	}
	else if( we & WE_NOTCHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(FALSE) ;
		}
	}
#endif //_JAPAN_LOCAL_
}

#ifndef _JAPAN_LOCAL_
void COptionDialog::DisableGraphicTab(BOOL bDisable)
{
			DWORD FullColor = 0xffffffff;
			DWORD HalfColor = RGBA_MAKE(200,200,200,255);

			DWORD StaticColor = RGBA_MAKE(10, 10, 10,255);

			if( bDisable )
			{
				FullColor = HalfColor;
				StaticColor = HalfColor ;
			}

			cWindow* pWnd = NULL;
			cGuageBar* pGuag = NULL;

			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ));
			pGuag->SetDisable(bDisable);
			pGuag->SetImageRGB(FullColor);
			pGuag->SetGuageLock(bDisable, FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_HEROSHADOW ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_MONSTERSHADOW ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_OTHERSSHADOW ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICGRAPHIC ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_DOWNGRAPHIC ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICEFFECT ))->SetFGColor(StaticColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_ONEEFFECT ))->SetFGColor(StaticColor);	
}

#endif //_JAPAN_LOCAL_




// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"

// 061206 LYW --- Include Outside Chatting Dialog.


//COptionDialog::COptionDialog()
//{
////	m_pBtnPreview = NULL;
//
//	// 061206 LYW --- Add Flag For CheckBox. ( Chatting Option Part )
//	m_bCheckSystem = FALSE ;
//}
//
//COptionDialog::~COptionDialog()
//{
//
//}
//
//void COptionDialog::Add(cWindow * window)
//{
//	if(window->GetType() == WT_PUSHUPBUTTON)
//		AddTabBtn(curIdx1++, (cPushupButton * )window);
//	else if(window->GetType() == WT_DIALOG)
//		AddTabSheet(curIdx2++, window);
//	else 
//		cTabDialog::Add(window);
//}
//
//void COptionDialog::Linking()
//{
////	m_pBtnPreview = (cButton*)GetWindowForID( OTI_BTN_PREVIEW );
////	m_pBtnPreview->SetDisable( TRUE );
//	//SW050824 체크박스 툴팁 설명
//	cWindow* pWnd = ((cWindow*)((cDialog*)GetTabSheet(2))->GetWindowForID( OTI_CB_AUTOCONTROL ));
//	cImage ToolTipImage;
//	SCRIPTMGR->GetImage( 0, &ToolTipImage, PFT_HARDPATH );
//	pWnd->SetToolTip( CHATMGR->GetChatMsg(1098), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );
//}
//
//void COptionDialog::SetActive(BOOL val)
//{
//	if( !m_bDisable )
//	{
//		if( val )
//		{
//			m_GameOption = *( OPTIONMGR->GetGameOption() );
//			UpdateData( FALSE );
//		}
//
//		cTabDialog::SetActive(val);
//	}
//
////--- main bar icon
//	// 061220 LYW --- Delete this code.
//	/*
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_OPTIONDLGICON, m_bActive );
//		*/
//}
//
//void COptionDialog::UpdateData( BOOL bSave )
//{
//	cDialog* pDlg;
//
//	if( bSave )	// 세팅된 옵션 가져오기 
//	{
//		pDlg = (cDialog*)GetTabSheet( 0 );	
//		// 061204 LYW --- MODIFIED UPDATE DATA FOR CHATTING.
//		// before modified.
//		/*
//		m_GameOption.bNoDeal		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->IsChecked();
//		m_GameOption.bNoParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->IsChecked();
//		m_GameOption.bNoFriend		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->IsChecked();
//		m_GameOption.bNoShowdown		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->IsChecked();
//		m_GameOption.bNameMunpa		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->IsChecked();
//		m_GameOption.bNameParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->IsChecked();
//		m_GameOption.bNameOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->IsChecked();
//		m_GameOption.bNoMemberDamage	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->IsChecked();
//		m_GameOption.bNoGameTip		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->IsChecked();
//		m_GameOption.nMacroMode		= ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->IsPushed();
//		*/
//		// after modified.
//		m_GameOption.bWhisper		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_WHISPER ) )->IsChecked() ;
//		m_GameOption.bTalk			= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_TALK ) )->IsChecked() ;
//		m_GameOption.bBalloon		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_BALLOON ) )->IsChecked() ;
//		m_GameOption.bSystem		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_SYSTEM ) )->IsChecked() ;
//		//m_GameOption.bNotify		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_NOTIFY ) )->IsChecked() ;
//		m_GameOption.bExperience1	= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE1 ) )->IsChecked() ;
//		//m_GameOption.bExperience2	= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE2 ) )->IsChecked() ;
//		m_GameOption.bItem1			= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM1 ) )->IsChecked() ;
//		//m_GameOption.bItem2			= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM2 ) )->IsChecked() ;
//		m_GameOption.bMoney1		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY1 ) )->IsChecked() ;
//		//m_GameOption.bMoney2		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY2 ) )->IsChecked() ;
//		m_GameOption.bCombat1		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT1 ) )->IsChecked() ;
//		//m_GameOption.bCombat2		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT2 ) )->IsChecked() ;
//		m_GameOption.bEmoticon		= (( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EMOTICON ) )->IsChecked() ;
//		// 061206 LYW --- Add Flag For CheckBox. ( Chatting Option Part )
//		if( m_GameOption.bSystem )
//		{
//			m_bCheckSystem = TRUE ;
//		}
//		else
//		{
//			m_bCheckSystem = FALSE ;
//
//			cDialog* pDlg = ( cDialog* )GetTabSheet(0) ;
//            
//			cCheckBox* pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_SYSTEM ) ;
//			pCheckBox->SetChecked( FALSE ) ;
//
//			pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE1 ) ;
//			pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//			pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM1 ) ;
//			pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//			pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY1 ) ;
//			pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//			pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT1 ) ;
//			pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//		}
//		// end modify.
//		
//		pDlg = (cDialog*)GetTabSheet( 1 );
//		m_GameOption.bNoWhisper		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->IsChecked();
//		m_GameOption.bNoChatting	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->IsChecked();
//		m_GameOption.bNoBalloon		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->IsChecked();
//		m_GameOption.bAutoHide		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->IsChecked();
//		m_GameOption.bNoShoutChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->IsChecked();
//		m_GameOption.bNoGuildChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->IsChecked();
//		m_GameOption.bNoAllianceChat= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->IsChecked();
//		m_GameOption.bNoSystemMsg	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->IsChecked();
//		m_GameOption.bNoExpMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->IsChecked();
//		m_GameOption.bNoItemMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->IsChecked();
//
//		pDlg = (cDialog*)GetTabSheet( 2 ); // 옵션에서 그래픽설정부분이다.
//		m_GameOption.nGamma			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->GetCurValue(); // 게임밝기
//		m_GameOption.nSightDistance = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->GetCurValue(); // 시야거리
////		m_GameOption.bGraphicCursor = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_CURSOR ))->IsChecked();
//		m_GameOption.bShadowHero	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->IsChecked(  ); // 자신
//		m_GameOption.bShadowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->IsChecked(  ); // 몬스터/NPC
//		m_GameOption.bShadowOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->IsChecked( ); // 다른케릭터
//	
//
//
//		//SW050822 Add RadioBtn for GraphicCtrl
//		m_GameOption.nEffectSnow    = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->IsChecked( ); // 눈 On/Off 2005.12.28일추가	
//
//		m_GameOption.bAutoCtrl		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->IsChecked();
//		// 061204 LYW --- MODIFIED UPDATE DATA FOR CHATTING.
//		// before modified.
//		/*
//		m_GameOption.bAmbientMax	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->IsChecked();
//		*/
//		
////		if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->IsPushed() )
////		{
////			m_GameOption.nLODMode = MODEL_LOD_USING_MODE_DEFAULT;
////		}
////		else if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed() )
////		{
////			m_GameOption.nLODMode = MODEL_LOD_USING_MODE_SET_LEVEL;
////		}
//
//		m_GameOption.nLODMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed(); // 그랙픽 상태
//		m_GameOption.nEffectMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->IsPushed();// 이펙트 표시	
//
//
//
//		
//		   
//
//
////		else if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ALLOFFEFFECT ))->IsPushed() )
////		{
////			m_GameOption.nEffectMode = eOO_EFFECT_NOT_USE;
////		}
//		pDlg = (cDialog*)GetTabSheet( 3 );
//		m_GameOption.bSoundBGM			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->IsChecked();
//		m_GameOption.bSoundEnvironment	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->IsChecked();
//		m_GameOption.nVolumnBGM			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->GetCurValue();
//		m_GameOption.nVolumnEnvironment = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->GetCurValue();
//
//		OPTIONMGR->SetGameOption( &m_GameOption );
//	}
//	else	//정보 가지고 와서 창 세팅 (기본설정이 창이 뜸)
//	{
//		pDlg = (cDialog*)GetTabSheet( 0 );	
//		// 061204 LYW --- MODIFIED UPDATE DATA FOR CHATTING.
//		// before modified.
//		/*
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->SetChecked( m_GameOption.bNoDeal );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->SetChecked( m_GameOption.bNoParty );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->SetChecked( m_GameOption.bNoFriend );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->SetChecked( m_GameOption.bNoShowdown );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->SetChecked( m_GameOption.bNameMunpa );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->SetChecked( m_GameOption.bNameParty );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->SetChecked( m_GameOption.bNameOthers );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->SetChecked( m_GameOption.bNoMemberDamage );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->SetChecked( m_GameOption.bNoGameTip );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( !m_GameOption.nMacroMode );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( m_GameOption.nMacroMode );
//		*/
//		// after modified.
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_WHISPER ) )->SetChecked( m_GameOption.bWhisper ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_TALK ) )->SetChecked( m_GameOption.bTalk ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_BALLOON ) )->SetChecked( m_GameOption.bBalloon ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_SYSTEM ) )->SetChecked( m_GameOption.bSystem ) ;
//		//(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_NOTIFY ) )->SetChecked( m_GameOption.bNotify ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE1 ) )->SetChecked( m_GameOption.bExperience1 ) ;
//		//(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE2 ) )->SetChecked( m_GameOption.bExperience2 ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM1 ) )->SetChecked( m_GameOption.bItem1 ) ;
//		//(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM2 ) )->SetChecked( m_GameOption.bItem2 ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY1 ) )->SetChecked( m_GameOption.bMoney1 ) ;
//		//(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY2 ) )->SetChecked( m_GameOption.bMoney2 ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT1 ) )->SetChecked( m_GameOption.bCombat1 ) ;
//		//(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT2 ) )->SetChecked( m_GameOption.bCombat2 ) ;
//		(( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EMOTICON ) )->SetChecked( m_GameOption.bEmoticon ) ;
//		// end modify.
//		
//		pDlg = (cDialog*)GetTabSheet( 1 );	
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->SetChecked( m_GameOption.bNoWhisper );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->SetChecked( m_GameOption.bNoChatting );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->SetChecked( m_GameOption.bNoBalloon );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->SetChecked( m_GameOption.bAutoHide );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->SetChecked( m_GameOption.bNoShoutChat );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->SetChecked( m_GameOption.bNoGuildChat );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->SetChecked( m_GameOption.bNoAllianceChat );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->SetChecked( m_GameOption.bNoSystemMsg );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->SetChecked( m_GameOption.bNoExpMsg );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->SetChecked( m_GameOption.bNoItemMsg );
//
//		pDlg = (cDialog*)GetTabSheet( 2 );
//		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->SetCurValue( m_GameOption.nGamma );
//		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->SetCurValue( m_GameOption.nSightDistance );
////		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_CURSOR ))->SetChecked( m_GameOption.bGraphicCursor );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->SetChecked( m_GameOption.bShadowHero );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetChecked( m_GameOption.bShadowMonster );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetChecked( m_GameOption.bShadowOthers );
//
//
//		// 눈 이펙트 추가 예정 2005.12.28 눈 On/Off 추가 
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->SetChecked( m_GameOption.nEffectSnow );
//
//		//SW050822 Add RadioBtn for GraphicCtrl
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->SetChecked( m_GameOption.bAutoCtrl );
//		//SW060904 Add MapAmbientCtrlOptn
//		// 061204 LYW --- Delete Some Code For A Moment.
//		/*
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->SetChecked( m_GameOption.bAmbientMax );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( !m_GameOption.nLODMode );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( m_GameOption.nLODMode );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( !m_GameOption.nEffectMode );
//		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( m_GameOption.nEffectMode );
//		*/
//
//		DisableGraphicTab(m_GameOption.bAutoCtrl);
////		else if( m_GameOption.nEffectMode == eOO_EFFECT_NOT_USE )
////		{
////			((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ALLOFFEFFECT ))->SetPush( TRUE );
////		}
//		pDlg = (cDialog*)GetTabSheet( 3 );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->SetChecked( m_GameOption.bSoundBGM );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->SetChecked( m_GameOption.bSoundEnvironment );
//		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->SetCurValue( m_GameOption.nVolumnBGM );
//		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->SetCurValue( m_GameOption.nVolumnEnvironment );
//	}
//}
//
///*
////=================================================================================================
//// NAME			: ActionEvent
//// PURPOSE		: Processing envet function.
//// ATTENTION	:
////=================================================================================================
//DWORD COptionDialog::ActionEvent( CMouse* mouseInfo )
//{
//	DWORD we = WE_NULL ;
//
//	we |= cDialog::ActionEvent( mouseInfo ) ;
//
//	if(we & WE_LBTNCLICK)
//	{
//		cDialog* pDlg = ( cDialog* )GetTabSheet(0) ;
//
//		cGuageBar* pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA ) ;
//
//		if( pGuageBar->IsDrag() )
//		{
//			CHATMGR->GetChattingDlg()->SetAlpha( (BYTE)pGuageBar->GetCurRate() ) ;
//		}
//
//		pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA2 ) ;
//
//		if( pGuageBar->IsDrag() )
//		{
//			CHATMGR->GetOSChattingDlg()->SetAlpha( (BYTE)pGuageBar->GetCurRate() ) ;
//		}
//	}
//
//	retur%n we ;
//}
//*/
//
//void COptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
//{
//	if( we & WE_BTNCLICK )
//	{
//		if( lId == OTI_BTN_OK )
//		{
//			UpdateData( TRUE );
//			OPTIONMGR->ApplySettings();
//			OPTIONMGR->SaveGameOption();
//			OPTIONMGR->SendOptionMsg();		
//			SetActive( FALSE );
//			// 061209 LYW --- Save option.
//			// 070102 LYW --- Delete this code.
//			//CHATMGR->GetChattingDlg()->InitOptionValue() ;
//		}
////		else if( lId == OTI_BTN_PREVIEW )
////		{
////			UpdateData( TRUE );
////			OPTIONMGR->ApplySettings();
////			OPTIONMGR->SaveGameOption();
////			m_pBtnPreview->SetDisable( TRUE );
////		}
//		else if( lId == OTI_BTN_CANCEL )
//		{
//			OPTIONMGR->CancelSettings();
//			SetActive( FALSE );
//		}
//		else if( lId == OTI_BTN_RESET )
//		{
//			OPTIONMGR->SetDefaultOption();
//			m_GameOption = *( OPTIONMGR->GetGameOption() );
//			UpdateData( FALSE );
//		}
//		else if( lId == OTI_BTN_SETCHAT )
//		{
//
//		}
//		else if( lId == OTI_BTN_SETMACRO )
//		{
//
//		}
//	}
//	
//	if( we & WE_PUSHUP )		//라디오 버튼 처리
//	{
//		// 061204 LYW --- Delete This Code For A Moment.
//		/*
//		if( lId == OTI_PB_CHATMODE || lId == OTI_PB_MACROMODE )
//			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( lId ))->SetPush( TRUE );
//		//SW050822 Add GraphicCtrl
//		if( lId == OTI_PB_BASICGRAPHIC || lId == OTI_PB_DOWNGRAPHIC )
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
//		if( lId == OTI_PB_BASICEFFECT || lId == OTI_PB_ONEEFFECT )
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
//			*/
//	}
//
//	if( we & WE_PUSHDOWN )		//
//	{
//		// 061204 LYW --- Delete This Code For A Moment.
//		/*
//		if( lId == OTI_PB_CHATMODE )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( FALSE );
//		}
//		else if( lId == OTI_PB_MACROMODE )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( FALSE );
//		}
//		*/
//		//SW050822 Add GraphicCtrl
//		if( lId == OTI_PB_BASICGRAPHIC )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( FALSE );
//		}
//		else if( lId == OTI_PB_DOWNGRAPHIC )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( FALSE );
//		}
//		if( lId == OTI_PB_BASICEFFECT )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( FALSE );
//		}
//		else if( lId == OTI_PB_ONEEFFECT )
//		{
//			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( FALSE );
//		}
//	}
//	if( we & WE_CHECKED )	//SW 자동 옵션을 키면 옵션버튼 비활성화 & 글자 흐리기로 비활성 표시
//	{
//
//		// 061204 LYW --- Modified Preexistence Chatting Dialog.
//		/*
//		if( lId == OTI_CB_AUTOCONTROL )
//		{
//			DisableGraphicTab(TRUE);
//		}
//		*/
//		switch( lId )
//		{
//		case OTI_CB_AUTOCONTROL :
//			DisableGraphicTab( TRUE ) ;
//			break ;
//
//		case OTI_CB_SYSTEM :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				// 061206 LYW --- modified.
//				// before
//				/*
//				if( !pOption->bSystem )
//				{
//					pOption->bSystem = TRUE ;
//
//					cCheckBox* pCheckBox ;
//
//					pCheckBox = ( cCheckBox* )GetWindowForID( OTI_CB_EXPERIENCE1 ) ;
//					pCheckBox->SetActive( FALSE ) ;
//				}
//				*/
//				// after
//				cDialog* pDlg = (cDialog*)GetTabSheet(0);
//				
//				cCheckBox* pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_SYSTEM ) ;
//
//				pOption->bSystem = TRUE ;
//
//				pCheckBox->SetChecked( TRUE ) ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE1 ) ;
//				pCheckBox->SetImageRGB( RGB( 255, 255, 255 ) ) ;
//				pOption->bExperience1 = pCheckBox->IsChecked() ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM1 ) ;
//				pCheckBox->SetImageRGB( RGB( 255, 255, 255 ) ) ;
//				pOption->bItem1 = pCheckBox->IsChecked() ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY1 ) ;
//				pCheckBox->SetImageRGB( RGB( 255, 255, 255 ) ) ;
//				pOption->bMoney1 = pCheckBox->IsChecked() ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT1 ) ;
//				pCheckBox->SetImageRGB( RGB( 255, 255, 255 ) ) ;
//				pOption->bCombat1 = pCheckBox->IsChecked() ;
//
//				//CHATMGR->GetChattingDlg()->InitOptionValue() ;
//
//				// 061206 LYW --- Add Flag For CheckBox. ( Chatting Option Part )
//				m_bCheckSystem = FALSE ;
//				// end
//			}
//			break ;
//
//		case OTI_CB_EXPERIENCE1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_EXPERIENCE1, &pOption->bExperience1 ) ;
//			}
//			break ;
//
//		case OTI_CB_ITEM1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_ITEM1, &pOption->bExperience1 ) ;
//			}
//			break ;
//
//		case OTI_CB_MONEY1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_MONEY1, &pOption->bMoney1 ) ;
//			}
//			break ;
//
//		case OTI_CB_COMBAT1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_COMBAT1, &pOption->bCombat1 ) ;
//			}
//			break ;
//		}
//	}
//	else if( we & WE_NOTCHECKED )
//	{
//		// 061204 LYW --- Modified Preexistence Chatting Dialog.
//		/*
//		if( lId == OTI_CB_AUTOCONTROL )
//		{
//			DisableGraphicTab(FALSE);
//		}
//		*/
//		switch( lId )
//		{
//		case OTI_CB_AUTOCONTROL :
//			DisableGraphicTab( FALSE ) ;
//			break ;
//
//		case OTI_CB_SYSTEM :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//
//				cDialog* pDlg = ( cDialog* )GetTabSheet(0) ;
//
//				cCheckBox* pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_SYSTEM ) ;
//
//				pOption->bSystem = FALSE ;
//
//				pCheckBox->SetChecked( FALSE ) ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_EXPERIENCE1 ) ;
//				pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_ITEM1 ) ;
//				pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_MONEY1 ) ;
//				pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//				pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( OTI_CB_COMBAT1 ) ;
//				pCheckBox->SetImageRGB( RGB( 100, 100, 100 ) ) ;
//
//				pOption->bExperience1	= FALSE ;
//				pOption->bItem1			= FALSE ;
//				pOption->bMoney1		= FALSE ;
//				pOption->bCombat1		= FALSE ;
//
//				//CHATMGR->GetChattingDlg()->InitOptionValue() ;
//
//				// 061206 LYW --- Add Flag For CheckBox. ( Chatting Option Part )
//				m_bCheckSystem = TRUE ;
//			}
//			break ;
//
//		case OTI_CB_EXPERIENCE1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_EXPERIENCE1, &pOption->bExperience1 ) ;
//			}
//			break ;
//
//		case OTI_CB_ITEM1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_ITEM1, &pOption->bExperience1 ) ;
//			}
//			break ;
//
//		case OTI_CB_MONEY1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_MONEY1, &pOption->bMoney1 ) ;
//			}
//			break ;
//
//		case OTI_CB_COMBAT1 :
//			{
//				sGAMEOPTION* pOption ;
//				pOption = OPTIONMGR->GetGameOption() ;
//				SetCheckBox( 0, OTI_CB_COMBAT1, &pOption->bCombat1 ) ;
//			}
//			break ;
//		}
//	}
////	if( m_pBtnPreview->IsDisable() && lId != OTI_BTN_PREVIEW )
////		m_pBtnPreview->SetDisable( FALSE );
//	/*
//	if( we & WE_LBTNCLICK )
//	{
//		if( lId == OTI_GB_CHATALPHA )
//		{
//			cDialog* pDlg = ( cDialog* )GetTabSheet(0) ;
//
//			cGuageBar* pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA ) ;
//
//			if( pGuageBar->IsDrag() )
//			{
//				CHATMGR->GetChattingDlg()->SetAlpha( (BYTE)pGuageBar->GetCurRate() ) ;
//			}
//		}
//		else if( lId == OTI_GB_CHATALPHA2 )
//		{
//			cDialog* pDlg = ( cDialog* )GetTabSheet(0) ;
//
//			cGuageBar* pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA2 ) ;
//
//			if( pGuageBar->IsDrag() )
//			{
//				CHATMGR->GetOSChattingDlg()->SetAlpha( (BYTE)pGuageBar->GetCurRate() ) ;
//			}
//		}
//	}
//	*/
//}
//
//
////=================================================================================================
//// NAME			: SetCheckBox()
//// PURPOSE		: Add Function to Setting checkbox.
//// ATTENTION	:
////=================================================================================================
//void COptionDialog::SetCheckBox( int nTabNum, LONG lId, BOOL* bOptionValue )
//{
//	cDialog* pDlg = ( cDialog* )GetTabSheet(nTabNum) ;
//
//	cCheckBox* pCheckBox = ( cCheckBox* )pDlg->GetWindowForID( lId ) ;
//
//	if( !m_bCheckSystem )
//	{
//		pCheckBox->SetChecked( pCheckBox->IsChecked() ) ;
//	}
//	else
//	{
//		if( pCheckBox->IsChecked() )
//		{
//			pCheckBox->SetChecked( FALSE ) ;
//			*bOptionValue = FALSE ;
//		}
//		else
//		{
//			pCheckBox->SetChecked( TRUE ) ;
//			*bOptionValue = TRUE ;
//		}
//		//CHATMGR->GetChattingDlg()->InitOptionValue() ;
//	}
//}
//
//void COptionDialog::DisableGraphicTab(BOOL bDisable)
//{
//			DWORD FullColor = 0xffffffff;
//			DWORD HalfColor = RGBA_MAKE(200,200,200,255);
//
//			if( bDisable )
//			{
//				FullColor = HalfColor;
//			}
//
//			cWindow* pWnd = NULL;
//			cGuageBar* pGuag = NULL;
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ))->SetDisable(FALSE);
//			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ));
//			pGuag->SetDisable(bDisable);
//			pGuag->SetImageRGB(FullColor);
//			pGuag->SetGuageLock(bDisable, FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_GAMMA ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ))->SetDisable(FALSE);
//			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ));
//			pGuag->SetDisable(bDisable);
//			pGuag->SetImageRGB(FullColor);
//			pGuag->SetGuageLock(bDisable, FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_HEROSHADOW ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_MONSTERSHADOW ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_OTHERSSHADOW ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICGRAPHIC ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_DOWNGRAPHIC ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICEFFECT ))->SetFGColor(FullColor);
////			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetDisable(FALSE);
//			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ));
//			pWnd->SetDisable(bDisable);
//			pWnd->SetImageRGB(FullColor);
//			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_ONEEFFECT ))->SetFGColor(FullColor);	
//}

