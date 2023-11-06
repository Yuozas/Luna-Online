// GuageDialog.cpp: implementation of the CGuageDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuageDialog.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./interface/cStatic.h"
#include "MHTimeManager.h"
#include "MussangManager.h"
#include "ObjectManager.h"

// 070110 LYW --- Include header files to need.
#include "MonsterGuageDlg.h"
#include "Party.h"
#include "PartyManager.h"
#include "ChatManager.h"
#include "GameIn.h"

// 070118 LYW --- Include header file to need.
#include "./Interface/cResourceManager.h"

#include "cMsgBox.h"

#include "./Input/Mouse.h"
#include "./interface/cWindowManager.h"

// 071004 LYW --- GuageDialog : Include script manager.
#include "./Interface/cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuageDialog::CGuageDialog()
{
/*	m_pMussangBtn	= NULL;
	m_pFlicker01	= NULL;
	m_pFlicker02	= NULL;

	m_bFlicker		= FALSE;
	m_bFlActive		= FALSE;
	m_dwFlickerSwapTime	= 0;	*/

	// 070110 LYW --- Add controls for popup menu.
	int count = 0 ;

	m_pMenuButton = NULL ;

	for( count = 0 ; count < MAX_POPUP1_STATIC ; ++count )
	{
		m_pBGImage1[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_POPUP2_STATIC ; ++count )
	{
		m_pBGImage2[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_POPUP1_BUTTON ; ++count )
	{
		m_pPopupBtn1[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_POPUP2_BUTTON ; ++count )
	{
		m_pPopupBtn2[ count ] = NULL ;
	}

	// 070117 LYW --- Add static controls for charater image.
	for( count = 0 ; count < MAX_HERO_FACE ; ++count )
	{
		m_pHeroFace[ count ] = NULL ;
	}

	m_bShowPopupMenu1	= FALSE ; 
	m_bShowPopupMenu2	= FALSE ; 

	m_byHeroState		= e_NONE ;

	// 071004 LYW --- GuageDialog : Initialize party master part.
	m_bRenderMasterMark = FALSE ;
}

CGuageDialog::~CGuageDialog()
{

}

void CGuageDialog::Linking()
{
/*	m_pMussangBtn	= (cButton*)GetWindowForID(CG_MUSSANGSTART);
	m_pFlicker01	= (cStatic*)GetWindowForID(CG_MUSSANG1);
//	m_pFlicker02	= (cStatic*)GetWindowForID(CG_MUSSANG2);

	if( m_pMussangBtn )
	{
		DisableMussangBtn(TRUE);
	}
	((CObjectGuagen*)GetWindowForID(CG_GUAGEMUSSANG))->SetValue( 0, 0 );
	m_pFlicker01->SetActive(FALSE);
//	m_pFlicker02->SetActive(FALSE);*/

	// 070110 LYW --- Add controls for popup menu.
	int count = 0 ;
/*
	m_pMenuButton = ( cButton* )GetWindowForID( CG_MENUBOX ) ;

	for( count = 0 ; count < MAX_POPUP1_STATIC ; ++count )
	{
		m_pBGImage1[ count ] = ( cStatic* )GetWindowForID( CG_ST_MENU1BACK_TOP + count ) ;
		m_pBGImage1[ count ]->SetAlpha( 100 ) ;
	}

	for( count = 0 ; count < MAX_POPUP2_STATIC ; ++count )
	{
		m_pBGImage2[ count ] = ( cStatic* )GetWindowForID( CG_ST_MENU2BACK_TOP + count ) ;
		m_pBGImage2[ count ]->SetAlpha( 100 ) ;
	}

	for( count = 0 ; count < MAX_POPUP1_BUTTON ; ++count )
	{
		m_pPopupBtn1[ count ] = ( cButton* )GetWindowForID( CG_BT_MENU1_BUTTON1 + count ) ;
	}

	for( count = 0 ; count < MAX_POPUP2_BUTTON ; ++count )
	{
		m_pPopupBtn2[ count ] = ( cButton* )GetWindowForID( CG_BT_MENU2_BUTTON1 + count ) ;
	}
*/
	// 070117 LYW --- Add static controls for charater image.
	for( count = 0 ; count < MAX_HERO_FACE ; ++count )
	{
		m_pHeroFace[ count ] = ( cStatic* )GetWindowForID( CG_ST_HERO_ELF_MAN + count ) ;
		m_pHeroFace[ count ]->SetActive( FALSE ) ;
	}

	// 071004 LYW --- GuageDialog : Setting master mark.
	SCRIPTMGR->GetImage( MASTER_MARK_NUM, &m_MasterMark, PFT_HARDPATH ) ;
}

// 070110 LYW --- Delete this function.
/*
void CGuageDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
//	if( we & WE_BTNCLICK )
//	{
//		if( lId == CG_MUSSANGSTART )
//		{
//			//무쌍 발동 정보 보내기
//			if(!HERO->IsDied())
//				MUSSANGMGR->SendMsgMussangOn();
//		}
//	}
}
*/

void CGuageDialog::DisableMussangBtn(BOOL bDisable)
{
/*	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);
	
	if( bDisable )
	{
		FullColor = HalfColor;
	}

	m_pMussangBtn->SetDisable(bDisable);
	m_pMussangBtn->SetImageRGB(FullColor);*/
}

void CGuageDialog::SetFlicker( BOOL bFlicker )
{
/*	m_bFlicker = bFlicker;
	m_dwFlickerSwapTime = gCurTime;

	m_pFlicker01->SetActive(bFlicker);
//	m_pFlicker02->SetActive(bFlicker);*/
}

void CGuageDialog::FlickerMussangGuage()
{
/*	if(!m_bFlicker)	return;

	if( gCurTime - m_dwFlickerSwapTime > FLICKER_TIME )
	{
		m_bFlActive = !m_bFlActive;

		m_pFlicker01->SetActive(m_bFlActive);
//		m_pFlicker02->SetActive(!m_bFlActive);
		
		m_dwFlickerSwapTime = gCurTime;
	}*/
}

void CGuageDialog::Render()
{
//	FlickerMussangGuage();
	//cDialog::Render();

	if( !IsActive() ) return ;
	cDialog::RenderWindow();
	cDialog::RenderComponent();

	// 071004 LYW --- GuageDialog : Render master mark.
	if( m_bRenderMasterMark )
	{
		VECTOR2 scale;
		scale.x = 1.0f;
		scale.y = 1.0f;

		VECTOR2 vPos;
		/*vPos.x = (float)(GetAbsX() + 8) ;
		vPos.y = (float)(GetAbsY() + 8) ;*/

		int nExtraW = 11 ;
		int nMarkW = 14 ;

		vPos.x = (float)(GetAbsX() + GetWidth() - nExtraW - nMarkW ) ;
		vPos.y = (float)(GetAbsY() + 8) ;

		DWORD dwColor = 0xffffffff ;

		m_MasterMark.RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;
	}
}


// 070110 LYW --- Add function to processing event.
//=================================================================================================
// NAME			: ActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
DWORD CGuageDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL ;

	we |= cDialog::ActionEvent( mouseInfo ) ;

	if( we & WE_LBTNCLICK )
	{
		OBJECTMGR->SetSelectedObject( HERO );
	}
	/*
	if( we & WE_MOUSEOVER )
	{
		if( m_byHeroState == e_CAPTAIN )
		{
			int xPos = mouseInfo->GetMouseX() ;
			int yPos = mouseInfo->GetMouseY() ;
			if( m_pPopupBtn1[0]->PtInWindow( (LONG)xPos, (LONG)yPos ) || m_pBGImage2[0]->PtInWindow( (LONG)xPos, (LONG)yPos ) ||
				m_pBGImage2[1]->PtInWindow( (LONG)xPos, (LONG)yPos )  || m_pBGImage2[2]->PtInWindow( (LONG)xPos, (LONG)yPos ) || 
				m_pBGImage2[3]->PtInWindow( (LONG)xPos, (LONG)yPos ) )
			{
				if( m_bShowPopupMenu2 == FALSE )
				{
					m_bShowPopupMenu2 = TRUE ;
					SetSecondPopup() ;
				}
			}
		}
	}
	else
	{
		m_bShowPopupMenu2 = FALSE ;
		SetSecondPopup() ;
	}
	*/

	return we ;
}



//=================================================================================================
// NAME			: OnActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
void CGuageDialog::OnActionEvent( LONG lId, void* p, DWORD we )
{
	/*switch( lId )
	{
	case CG_MENUBOX :
		{
			if( m_bShowPopupMenu1 )
			{
				m_bShowPopupMenu1 = FALSE ;
				SetPopupMenu() ;

				m_bShowPopupMenu2 = FALSE ;
				SetSecondPopup() ;
			}
			else 
			{
				m_bShowPopupMenu1 = TRUE ;
				CheckHeroAtt() ;
				SetPopupMenu() ;
			}
		}
		break ;

	case CG_BT_MENU1_BUTTON1 :
		{
			if( m_byHeroState == e_CAPTAIN )
			{
				if( m_bShowPopupMenu2 )
				{
					m_bShowPopupMenu2 = FALSE ;
				}
				else
				{
					m_bShowPopupMenu2 = TRUE ;
				}
				SetSecondPopup() ;
			}
			else if( m_byHeroState == e_MEMBER )
			{
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));
			}
			else if( m_byHeroState == e_NONE )
			{
			}
		}
		break ;

	case CG_BT_MENU1_BUTTON2 :
		{
			if( m_byHeroState == e_CAPTAIN )
			{
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));
			}
		}
		break ;

	case CG_BT_MENU1_BUTTON3 :
		{
		}
		break ;

	case CG_BT_MENU2_BUTTON1 :
		{
			DWORD NewMasterID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(NewMasterID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->ChangeMasterPartySyn(HEROID, NewMasterID);
		}
		break ;

	case CG_BT_MENU2_BUTTON2 :
		{
			// 0 => Random
			PARTYMGR->SetPickupItem( 0 ) ;
		}
		break ;

	case CG_BT_MENU2_BUTTON3 :
		{
			// 1 => Damage
			PARTYMGR->SetPickupItem( 1 ) ;
		}
		break ;

	case CG_BT_MENU2_BUTTON4 :
		{
			// 2 => Sequence
			PARTYMGR->SetPickupItem( 2 ) ;
		}
		break ;
	}

	if( lId != CG_MENUBOX )
	{
		if( lId == CG_BT_MENU1_BUTTON1 )
		{
			if( m_byHeroState != e_CAPTAIN )
			{
				m_bShowPopupMenu1 = FALSE ;
				SetPopupMenu() ;

				m_bShowPopupMenu2 = FALSE ;
				SetSecondPopup() ;
			}
		}
		else
		{
			m_bShowPopupMenu1 = FALSE ;
			SetPopupMenu() ;

			m_bShowPopupMenu2 = FALSE ;
			SetSecondPopup() ;
		}
	}*/
}


//=================================================================================================
// NAME			: CheckHeroAtt
// PURPOSE		: Check and setting attribute of hero.
// ATTENTION	:
//=================================================================================================
void CGuageDialog::CheckHeroAtt()
{
	if( HERO->GetPartyIdx() == 0 )
	{
		m_byHeroState = e_NONE ;
	}
	else
	{
		CParty* pParty = PARTYMGR->GetParty();

		if( HEROID == pParty->GetMasterIdx() )
		{
			m_byHeroState = e_CAPTAIN ;
		}
		else
		{
			m_byHeroState = e_MEMBER ;
		}
	}
}


//=================================================================================================
// NAME			: SetPopupMenu
// PURPOSE		: Setting popup mode.
// ATTENTION	:
//=================================================================================================
void CGuageDialog::SetPopupMenu()
{
	int count = 0 ;

	DWORD basicColor = m_pPopupBtn1[0]->GetBasicColor() ;
	DWORD overColor  = m_pPopupBtn1[0]->GetOverColor() ;
	DWORD pressColor = m_pPopupBtn1[0]->GetPressColor() ;

	if( m_bShowPopupMenu1 )
	{
		switch( m_byHeroState )
		{
		case e_CAPTAIN :
			{
				for( count = 0 ; count < MAX_POPUP1_STATIC ; ++count )
				{
					m_pBGImage1[ count ]->SetActive( TRUE ) ;
				}

				for( count = 0 ; count < MAX_POPUP1_BUTTON ; ++count )
				{
					m_pPopupBtn1[ count ]->SetActive( TRUE ) ;
				}

				// 070118 LYW --- Modified this part.
				/*
				m_pPopupBtn1[0]->SetText("아이템 분배" , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[1]->SetText("파티 탈퇴" , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[2]->SetText("취소" , basicColor, overColor, pressColor ) ;
				*/

				m_pPopupBtn1[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[2]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
			}
			break ;

		case e_MEMBER :
			{
				//m_byCurPopupMode = e_STANDARD ;

				m_pBGImage1[0]->SetActive( TRUE ) ;
				m_pBGImage1[1]->SetActive( TRUE ) ;

				for( count = 0 ; count < MAX_POPUP1_BUTTON-1 ; ++count )
				{
					m_pPopupBtn1[ count ]->SetActive( TRUE ) ;
				}

				// 070118 LYW --- Modified this part.
				/*
				m_pPopupBtn1[0]->SetText("파티 탈퇴" , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[1]->SetText("취소" , basicColor, overColor, pressColor ) ;
				*/

				m_pPopupBtn1[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pPopupBtn1[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
			}
			break ;

		case e_NONE :
			{
				m_pBGImage1[0]->SetActive( TRUE ) ;
				m_pPopupBtn1[0]->SetActive( TRUE ) ;

				// 070118 LYW --- Modified this line.
                //m_pPopupBtn1[0]->SetText("취소" , basicColor, overColor, pressColor ) ;				
				m_pPopupBtn1[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;				
			}
			break ;
		}
	}
	else
	{
		// 070110 LYW --- Add controls for popup menu.
		for( count = 0 ; count < MAX_POPUP1_STATIC ; ++count )
		{
			m_pBGImage1[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_POPUP2_STATIC ; ++count )
		{
			m_pBGImage2[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_POPUP1_BUTTON ; ++count )
		{
			m_pPopupBtn1[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_POPUP2_BUTTON ; ++count )
		{
			m_pPopupBtn2[ count ]->SetActive( FALSE ) ;
		}
	}
}


//=================================================================================================
// NAME			: SetSecondPopup
// PURPOSE		: Setting  second popup mode.
// ATTENTION	:
//=================================================================================================
void CGuageDialog::SetSecondPopup()
{
	int count = 0 ;

	DWORD basicColor = m_pPopupBtn2[0]->GetBasicColor() ;
	DWORD overColor  = m_pPopupBtn2[0]->GetOverColor() ;
	DWORD pressColor = m_pPopupBtn2[0]->GetPressColor() ;

	if( m_bShowPopupMenu2 )
	{
		for( count = 0 ; count < MAX_POPUP2_STATIC ; ++count )
		{
			m_pBGImage2[ count ]->SetActive( TRUE ) ;
		}

		for( count = 0 ; count < MAX_POPUP2_BUTTON ; ++count )
		{
			m_pPopupBtn2[ count ]->SetActive( TRUE ) ;
		}

		// 070118 LYW --- Modified this part.
		/*
		m_pPopupBtn2[0]->SetText("파티장 분배"		, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[1]->SetText("순서대로 분배"	, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[2]->SetText("랜덤 분배"		, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[3]->SetText("데미지 우선 분배" , basicColor, overColor, pressColor ) ;
		*/

		m_pPopupBtn2[0]->SetText(RESRCMGR->GetMsg( 982)	, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[1]->SetText(RESRCMGR->GetMsg( 982)	, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[2]->SetText(RESRCMGR->GetMsg( 982)	, basicColor, overColor, pressColor ) ;
		m_pPopupBtn2[3]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
	}
	else
	{
		for( count = 0 ; count < MAX_POPUP2_STATIC ; ++count )
		{
			m_pBGImage2[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_POPUP2_BUTTON ; ++count )
		{
			m_pPopupBtn2[ count ]->SetActive( FALSE ) ;
		}
	}
}


//=================================================================================================
// NAME			: SecessionPartyCaptain
// PURPOSE		: Processing withdrawal captain from party.
// ATTENTION	:
//=================================================================================================
void CGuageDialog::SecessionPartyCaptain()
{
	int count = 0 ; 
	DWORD dwNextCaptain = 0 ;

	for( count = 1 ; count <  MAX_PARTY_LISTNUM ; ++count ) 
	{
		dwNextCaptain = PARTYMGR->GetParty()->GetMemberID(count) ;

		if( dwNextCaptain != 0 ) break ;
	}

	if( count < MAX_PARTY_LISTNUM )
	{
		dwNextCaptain = PARTYMGR->GetParty()->GetMemberID(count) ;

		MSG_DWORD2 msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CHANGEMASTER_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = dwNextCaptain;
		msg.dwData2 = HERO->GetPartyIdx();
		NETWORK->Send(&msg, sizeof(msg));
		
		//PARTYMGR->SetChangingMaster(TRUE);
	}
}


//=================================================================================================
//	NAME		: MakeHeroFace()
//	PURPOSE		: Add function for character face.
//	DATE		: January 17, 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CGuageDialog::MakeHeroFace()
{
	int count = 0 ;

	for( count = 0 ; count < MAX_HERO_FACE ; ++count )
	{
		m_pHeroFace[ count ]->SetActive( FALSE ) ;
	}

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( HERO->GetID() );

	BYTE racial		= pPlayer->GetCharacterTotalInfo()->Race ;
	BYTE gender		= pPlayer->GetCharacterTotalInfo()->Gender ;

	switch( racial )
	{
	case 0x00 :
		{
			if( gender == 0 )
			{
				m_pHeroFace[2]->SetActive( TRUE ) ;
			}
			else
			{
				m_pHeroFace[3]->SetActive( TRUE ) ;
			}
		}
		break ;

	case 0x01 :
		{
			if( gender == 0 )
			{
				m_pHeroFace[0]->SetActive( TRUE ) ;
			}
			else
			{
				m_pHeroFace[1]->SetActive( TRUE ) ;
			}
		}
		break ;
	}
}

void CGuageDialog::SetActive(BOOL val)
{
	if( val )
	{
		MakeHeroFace();
	}

	cDialog::SetActive( val );
}
