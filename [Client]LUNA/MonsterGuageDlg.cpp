// MonsterGuageDlg.cpp: implementation of the CMonsterGuageDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterGuageDlg.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "Monster.h"
#include "CheatMsgParser.h"
#include "ChatManager.h"
#include "./Interface/cWindowManager.h"

// 070109 LYW --- Include header files to need.
#include "ObjectManager.h"
#include "Party.h"
#include "PartyManager.h"
#include "GameIn.h"
#include "ExchangeManager.h"
#include "ShowdownManager.h"

// 070118 LYW --- Include header file to need.
#include "./Interface/cResourceManager.h"

// 070418 LYW --- MonsterGuageDlg : Include header file of CTimeDelay class.
#include "../[cc]skill/client/delay/timedelay.h"

// 070418 LYW --- MonsterGuageDlg : Add for test.
#include "cFont.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.08
#include "GameResourceManager.h"
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.05.08

#include "pet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterGuageDlg::CMonsterGuageDlg()
{
	// 070123 LYW --- Modified this part.
	/*
	// 070109 LYW --- Add variable for count.
	int count = 0 ;

	m_pName = NULL;
	m_pNpcName = NULL;
	m_pLifeText = NULL;
	m_pLifeGuage = NULL;
	m_type = WT_MONSTERGUAGEDLG;
	m_CurMode = -1;
	m_Type = eObjectKind_Monster;

	// 070109 LYW --- Add new controls.
	m_pMenuBox			= NULL ;
	m_pHPMark			= NULL ;
	m_pCharLife			= NULL ;
	m_pCharGuageLife	= NULL ;

	// 070109 LYW --- Add variable whether show popup menu or not.
	m_bShowPopupMenu = FALSE ;

	m_byCurPopupMode = e_STANDARD ;

	m_byTargetState	 = e_NONE ;
	m_byHeroState	 = e_NONE ;

	// 070109 LYW --- Initialize to NULL.
	for( count = 0 ; count < MAX_POPUP_BGCOUNT ; ++count )
	{
		m_pST_BGImage[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
	{
		m_pBT_PopupMenu[ count ] = NULL ;
	}
	*/

	int count = 0 ; 

	m_pHPMark					= NULL ;

	m_byCurMode					= 0x00 ;
    

	/// POPUP MENU PART.
	m_pMenuCallBtn				= NULL ;
	for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
	{
		m_pMenuBtn[ count ]		= NULL ;
	}

	for( count = 0 ;  count < MAX_POPUP_BGCOUNT ; ++count )
	{
		m_pMenuBG[ count ]		= NULL ;
	}

	m_bShowMenu					= FALSE ;


	/// MONSTER PART.
	m_pMonsterFace				= NULL ;
	m_pMonsterName				= NULL ;
	m_pMonsterGuage				= NULL ;


	/// PLAYER PART.
	m_pPlayerFace				= NULL ;
	m_pGuildName				= NULL ;
	m_pUnionName				= NULL ;
	m_pPlayerName				= NULL ;
	m_playerLife				= NULL ;
	m_pPlayerGuage				= NULL ;

	/// NPC PART.
	m_pNPCFace					= NULL ;
	m_pNPCName					= NULL ;
	m_pNPCOccupation			= NULL ;

	m_byCurObjectKind = 0 ;
}

CMonsterGuageDlg::~CMonsterGuageDlg()
{
	m_pCurMonster = NULL;
	for(int i=0; i<eMonGuage_Max; ++i)
		m_MonGuageCtlListArray[i].RemoveAll();
}

void CMonsterGuageDlg::Linking()
{
	// 070123 LYW --- Modified this part.
	/*
	m_pName			= (cStatic *)GetWindowForID(CG_MONSTERGUAGENAME);
	m_pLifeText		= (cStatic *)GetWindowForID(CG_MONSTERGUAGELIFETEXT);
	m_pLifeGuage	= (CObjectGuagen *)GetWindowForID(CG_MONSTERGUAGELIFE);
	m_pGuildName	= (cStatic *)GetWindowForID(CG_GUILDNAME);
	m_pGuildUnionName	= (cStatic*)GetWindowForID(CG_GUILDUNION_NAME);
	// 070109 LYW --- DeActive this control.
	m_pGuildUnionName->SetActive( FALSE ) ;
	m_pNpcName		= (cStatic *)GetWindowForID(CG_NPCNAME);

	// 070109 LYW --- Add new controls.
	m_pMenuBox			= ( cButton* )GetWindowForID( TG_MENUBOX ) ;
	m_pHPMark			= ( cStatic* )GetWindowForID( CG_LIFEIMAGE ) ;
	m_pCharLife			= ( cStatic* )GetWindowForID( CG_CRTLIFE ) ;
	m_pCharGuageLife	= ( CObjectGuagen* )GetWindowForID( CG_CRTGUAGELIFE ) ;

	
	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail(m_pLifeGuage);
	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail(m_pLifeText);
//	m_MonGuageCtlListArray[eMonGuage_Monster].AddTail((cStatic *)GetWindowForID(CG_MONSTERGUAGELIFEBASE));

	m_MonGuageCtlListArray[eMonGuage_Character].AddTail(m_pGuildName);
	m_MonGuageCtlListArray[eMonGuage_Character].AddTail(m_pGuildUnionName);

	m_MonGuageCtlListArray[eMonGuage_Npc].AddTail(m_pNpcName);

	// 070109 LYW --- Linking controls.
	int count = 0 ;
	for( count = 0 ; count < MAX_POPUP_BGCOUNT ; ++count )
	{
		m_pST_BGImage[ count ] = ( cStatic* )GetWindowForID( CG_ST_BACK_TOP + count ) ;
		m_pST_BGImage[ count ]->SetAlpha( 100 ) ;
	}

	for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
	{
		m_pBT_PopupMenu[ count ] = ( cButton* )GetWindowForID( CG_BT_MENU_BUTTON1 + count ) ;
	}
	*/
	int count = 0 ; 

	m_pHPMark					= ( cStatic* )GetWindowForID( CG_LIFEIMAGE ) ;

	/// POPUP MENU PART.
	m_pMenuCallBtn				= ( cButton* )GetWindowForID( TG_MENUBOX ) ;
	for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
	{
		m_pMenuBtn[ count ]		= ( cButton* )GetWindowForID( CG_BT_MENU_BUTTON1 + count ) ;
	}

	for( count = 0 ;  count < MAX_POPUP_BGCOUNT ; ++count )
	{
		m_pMenuBG[ count ]		= ( cStatic* )GetWindowForID( CG_ST_BACK_TOP + count ) ;
	}

	/// MONSTER PART.
	m_pMonsterFace				= ( cStatic* )GetWindowForID( CG_MONSTERRACE ) ;
	// 070125 LYW --- Deactive monster face.
	m_pMonsterFace->SetActive( FALSE ) ;
	m_pMonsterName				= ( cStatic* )GetWindowForID( CG_MONSTERGUAGENAME ) ;
	m_pMonsterGuage				= ( CObjectGuagen* )GetWindowForID( CG_MONSTERGUAGELIFE ) ;
    
	/// PLAYER PART.
	m_pPlayerFace				= ( cStatic* )GetWindowForID( CG_PLAYERRACE ) ;
	// 070125 LYW --- Deactive player face.
	m_pPlayerFace->SetActive( FALSE ) ;
	m_pGuildName				= ( cStatic* )GetWindowForID( CG_GUILDNAME ) ;
	m_pUnionName				= ( cStatic* )GetWindowForID( CG_GUILDUNION_NAME ) ;
	m_pPlayerName				= ( cStatic* )GetWindowForID( CG_CRTGUAGENAME ) ;
	m_playerLife				= ( cStatic* )GetWindowForID( CG_CRTLIFE ) ;
	m_pPlayerGuage				= ( CObjectGuagen* )GetWindowForID( CG_CRTGUAGELIFE ) ;

	/// NPC PART.
	m_pNPCFace					= ( cStatic* )GetWindowForID( CG_NPCRACE ) ;
	// 070125 LYW --- Deactive npc face.
	m_pNPCFace->SetActive( FALSE ) ;
	m_pNPCName					= ( cStatic* )GetWindowForID( CG_NPCNAME ) ;
	m_pNPCOccupation			= ( cStatic* )GetWindowForID( CG_NPCOCCUPATION ) ;

	m_pMasterName				= ( cStatic* )GetWindowForID( CG_PETMASTERNAME ) ;
	m_pPetName 					= ( cStatic* )GetWindowForID( CG_PETNAME ) ;

}
// 070123 LYW --- Delete this function.
/*
void CMonsterGuageDlg::SetMonsterName( char * szMonsterName )
{
	if( m_pName )
	m_pName->SetStaticText(szMonsterName);

	m_pCurMonster = NULL;
	// 070109 LYW --- Modified this code.
	//m_pLifeGuage->SetValue(0,0);
	// 파티원일 경우와 일반 경우로 바뀐다.
	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();

	CheckTargetAtt( (CPlayer*)pOpPlayer ) ;

	if( pOpPlayer->GetObjectKind() == eMonGuage_Character )
	{
		if( m_byTargetState != e_NONE )
		{
			SetMonsterLife( pOpPlayer->GetLife(), pOpPlayer->GetMaxLife(), 0 ) ;
			m_pLifeGuage->SetActive( TRUE ) ;
			m_pHPMark->SetActive( TRUE ) ;

			m_pGuildName->SetActive( FALSE ) ;
		}
		else
		{
			m_pLifeGuage->SetValue(0,0);
			m_pLifeGuage->SetActive( FALSE ) ;
			m_pLifeText->SetActive( FALSE ) ;
			m_pHPMark->SetActive( FALSE ) ;

			m_pGuildName->SetActive( TRUE ) ;
		}
	}
	// 070122 LYW --- Add npc part.
	else if( pOpPlayer->GetObjectKind() == eMonGuage_Npc )
	{
		m_pLifeGuage->SetValue(0,0);
		m_pLifeGuage->SetActive( FALSE ) ;
		m_pLifeText->SetActive( FALSE ) ;
		m_pHPMark->SetActive( FALSE ) ;

		m_pGuildName->SetActive( TRUE ) ;
	}
}
*/

// 070123 LYW --- Delete this function.
/*
// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
void CMonsterGuageDlg::SetNpcName( char * szNpcName )
{
	if( m_pNpcName )
	m_pNpcName->SetStaticText(szNpcName);
}
*/

// 070123 LYW --- Delete this function.

void CMonsterGuageDlg::SetMonsterLife( CMonster * pMonster )
{
	// 070122 LYW --- Check monster type.
	// 070124 LYW --- Delete this part.
	/*
	WORD monsterType = pMonster->GetObjectKind() ;
	if( monsterType >= eObjectKind_Monster && monsterType <= eObjectKind_ToghterPlayMonster )
	{
		m_pLifeText->SetActive( FALSE ) ;
	}
	else
	{
		char szValue[50];

		if(pMonster->GetLife() > pMonster->GetSInfo()->Life)
			pMonster->SetLife(pMonster->GetSInfo()->Life, 0) ;
		sprintf( szValue, "%d/%d", pMonster->GetLife(), pMonster->GetSInfo()->Life );

		if( m_pLifeText )
		{
			m_pLifeText->SetStaticText(szValue);
			m_pLifeText->SetActive( TRUE ) ;
		}
	}
	*/

	if( pMonster->GetSInfo() )
		SetMonsterLife( pMonster->GetLife(), pMonster->GetSInfo()->Life, 0 );

	m_pCurMonster = pMonster;
	m_Type = eObjectKind_Monster;
}


void CMonsterGuageDlg::SetMonsterLife( DWORD curVal, DWORD maxVal, int type )
{
	ASSERT(maxVal>0);
	if(maxVal == 0)
		maxVal = 1;

	// 070122 LYW --- Delete this part.
	/*
	char szValue[50];

	//GuageOverCheck
	if(curVal > maxVal)
		curVal = maxVal;
	sprintf( szValue, "%d / %d", curVal, maxVal );
	*/
// 070109 LYW --- Modified this code.
	/*
#ifdef _CHEATENABLE_		
	if( m_pLifeText )
	{
		if(CHEATMGR->IsCheatEnable())
			m_pLifeText->SetStaticText(szValue);
	}
#endif
	*/
	// 070122 LYW --- Delete this part.
	/*
	if( m_pLifeText )
	{
		m_pLifeText->SetStaticText(szValue);
		m_pLifeText->SetActive( TRUE ) ;
	}
	*/

	// 070123 LYW --- Modified this part.
	/*
	if( m_pLifeGuage )
	{
		if(type == 0)
			m_pLifeGuage->SetValue((float)curVal/(float)maxVal, 0);
		else
			m_pLifeGuage->SetValue((float)curVal/(float)maxVal, (1500/maxVal)*curVal);
	}
	*/
	// 070124 LYW --- Modified this part.
	/*
	if( m_byCurMode == eMonGuage_Character )
	{
		m_pPlayerGuage->SetValue((float)curVal/(float)maxVal, 0);
	}
	else if( m_byCurMode == eMonGuage_Monster )
	{
		m_pMonsterGuage->SetValue((float)curVal/(float)maxVal, 0);
	}
	*/
	if( m_byCurObjectKind == eObjectKind_Player )
	{
		m_pPlayerGuage->SetValue((float)curVal/(float)maxVal, 0);
	}
	else if( m_byCurObjectKind >= eObjectKind_Monster && m_byCurObjectKind <= eObjectKind_ToghterPlayMonster )
	{
		m_pMonsterGuage->SetValue((float)curVal/(float)maxVal, 0);
	}
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.22
	else if( m_byCurObjectKind == eObjectKind_FarmObj)
	{
		m_pMonsterGuage->SetValue((float)curVal/(float)maxVal, 0);
	}
	// E 농장시스템 추가 added by hseos 2007.08.22
}

// 070123 LYW --- Delete this function.
/*
void CMonsterGuageDlg::SetGuildUnionName(char* GuildName, char* GuildUnionName)
{
//	char buf[MAX_GUILD_NAME+6];
	char buf[128];

	if( GuildName[0] )
		sprintf(buf, CHATMGR->GetChatMsg(783), GuildName);
	else
		buf[0] = 0;

	m_pGuildName->SetStaticText(buf);

	if( GuildUnionName[0] )
		sprintf(buf, CHATMGR->GetChatMsg(1137), GuildUnionName);
	else
		buf[0] = 0;

	m_pGuildUnionName->SetStaticText(buf);
}
*/

// 070123 LYW --- Delete this function.
/*
void CMonsterGuageDlg::ShowMonsterGuageMode(int mode)
{
	if(m_CurMode == mode)
		return;
	
	if(m_CurMode != -1)
	{
		SetActiveMonGuageMode(m_CurMode, FALSE);
	}

	m_CurMode = mode;

	//pet
	if(eMonGuage_Pet == mode)	return;
	
	SetActiveMonGuageMode(mode, TRUE);
}
*/
// 070123 LYW --- Delete this function.
/*
void CMonsterGuageDlg::SetActiveMonGuageMode(int mode, BOOL bActive)
{
	PTRLISTSEARCHSTART(m_MonGuageCtlListArray[mode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}
*/

void CMonsterGuageDlg::Render()
{
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() ) return;
#endif

	// 070123 LYW --- Modified this part.
	/*
	if( m_CurMode == eMonGuage_Character )
	{
		if( 0 < strlen(m_pGuildUnionName->GetStaticText()) )
			m_pGuildName->SetRelXY(0, 18 );
		else
			m_pGuildName->SetRelXY(0, 26 );
//		m_pGuildName->SetAbsXY( m_pGuildName->GetAbsX(), m_pGuildName->GetAbsY() );
		this->SetAbsXY( (LONG)this->GetAbsX(), (LONG)this->GetAbsY() );
	}
	*/

	if( m_byCurObjectKind >= eObjectKind_Monster &&  m_byCurObjectKind <= eObjectKind_ToghterPlayMonster )
	{
		if(IsStartTimer())
		{
			FadeAlpha() ;
		}
	}

	/*
	char tempStr[ 128 ] ;
	RECT rc ;
	rc.left = 10 ;
	rc.top = 300 ;
	rc.right = 1000 ;
	rc.bottom = 700 ;

	sprintf( tempStr, "RemainTime = %d", m_FOTimer.Check() ) ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	*/

	cDialog::Render();
}


// 070109 LYW --- Add function that is ActionEvent.
//=================================================================================================
// NAME			: ActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
DWORD CMonsterGuageDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL ;

	we |= cDialog::ActionEvent( mouseInfo ) ;

	/*
	if( we & WE_LBTNCLICK )
	{
		CheckTargetKind() ;
	}
	*/

	if( we & WE_RBTNCLICK )
	{
		CloseTargetDlg() ;
	}

	return we ;
}

void CMonsterGuageDlg::CloseTargetDlg()
{
	SetActive(FALSE ) ;
	OBJECTMGR->SetSelectedObjectID(0) ;
	m_bShowMenu = FALSE ;
}


//=================================================================================================
// NAME			: OnActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::OnActionEvent( LONG lId, void* p, DWORD we )
{
	//switch( lId )
	//{
	//	case TG_MENUBOX :
	//		{
	//			// 070123 LYW --- Modified this line.
	//			//if( m_bShowPopupMenu )
	//			if( m_bShowMenu )
	//			{
	//				// 070123 LYW --- Modified this part.
	//				/*
	//				m_bShowPopupMenu = FALSE ;
	//				SetPopupMenu() ;
	//				*/
	//				m_bShowMenu = FALSE ;
	//				InitPopupMenu() ;
	//			}
	//			else 
	//			{
	//				// 070315 LYW --- MonsterGuageDlg : Delete this function for a moment.
	//				//CheckTargetKind() ;

	//				CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();

	//				CheckTargetAtt( (CPlayer*)pOpPlayer ) ;
	//			}
	//		}
	//		break ;

	//	case CG_BT_MENU_BUTTON1 :
	//		{
	//			// 070123 LYW --- Modified this line.
	//			//GAMEIN->GetChattingDlg()->GetWhisperNameEditBox()->SetEditText( m_pName->GetStaticText() ) ;
	//			// 070321 LYW --- ChattingDlg : Delete.
	//			//GAMEIN->GetChattingDlg()->GetWhisperNameEditBox()->SetEditText( m_pPlayerName->GetStaticText() ) ;

	//			/*
	//			GAMEIN->GetChattingDlg()->SetCurChatMode( e_CM_EDITBOX ) ;
	//			GAMEIN->GetChattingDlg()->SetCurCommand( e_CMD_NORMAL ) ;
	//			GAMEIN->GetChattingDlg()->SetInputFocus( TRUE ) ;
 //               GAMEIN->GetChattingDlg()->GetInputEditBox()->SetFocusEdit( TRUE ) ;
	//			*/
	//		}
	//		break ;

	//	case CG_BT_MENU_BUTTON2 :
	//		{
	//			if( m_byCurPopupMode == e_STANDARD )
	//			{
	//				EXCHANGEMGR->ApplyExchange() ;
	//			}
	//			else
	//			{
	//				if( m_byHeroState == e_CAPTAIN )
	//				{
	//					PARTYMGR->AddPartyMemberSyn( OBJECTMGR->GetSelectedObject()->GetID() ) ;
	//				}
	//				else
	//				{
	//					PARTYMGR->CreatePartySyn(OBJECTMGR->GetSelectedObject()->GetID()) ;
	//				}
	//			}

	//			// 070123 LYW --- Modified this line.
	//			//m_bShowPopupMenu = FALSE ;
	//			m_bShowMenu = FALSE ;
	//		}
	//		break ;

	//	case CG_BT_MENU_BUTTON3 :
	//		{
	//			if( m_byCurPopupMode == e_STANDARD )
	//			{
	//				SHOWDOWNMGR->ApplyShowdown() ;
	//			}
	//			else
	//			{
	//				EXCHANGEMGR->ApplyExchange() ;
	//			}

	//			// 070123 LYW --- Modified this line.
	//			//m_bShowPopupMenu = FALSE ;
	//			m_bShowMenu = FALSE ;
	//		}
	//		break ;

	//	case CG_BT_MENU_BUTTON4 :
	//		{
	//			if( m_byCurPopupMode != e_STANDARD )
	//			{
	//				SHOWDOWNMGR->ApplyShowdown() ;
	//			}

	//			// 070123 LYW --- Modified this line.
	//			//m_bShowPopupMenu = FALSE ;
	//			m_bShowMenu = FALSE ;
	//		}
	//		break ;

	//	case CG_BT_MENU_BUTTON5 :
	//		{
	//			// 070123 LYW --- Modified this line.
	//			//m_bShowPopupMenu = FALSE ;
	//			m_bShowMenu = FALSE ;
	//		}
	//		break ;
	//}

	//if( lId != TG_MENUBOX )
	//{
	//	// 070123 LYW --- Modified this part.
	//	/*
	//	m_bShowPopupMenu = FALSE ;
	//	SetPopupMenu() ;
	//	*/
	//	m_bShowMenu = FALSE ;
	//	InitPopupMenu() ;
	//}
}


//=================================================================================================
// NAME			: CheckTargetKind
// PURPOSE		: Check kind of target.
// ATTENTION	:
//=================================================================================================
// 070315 LYW --- MonsterGuageDlg : Modified function CheckTargetKind() ;
//void CMonsterGuageDlg::CheckTargetKind()
void CMonsterGuageDlg::CheckTargetKind(CObject* pObject)
{
	// 070123 LYW --- Modified this line.
	//if( m_CurMode == eMonGuage_Character )
	///if( m_byCurMode == eMonGuage_Character )
	///{
	///	CheckHeroAtt() ;
		// 070123 LYW --- Modified this part.
		/*
		m_bShowPopupMenu = TRUE ;
		SetPopupMenu() ;
		*/
		// 070123 LYW --- Modified this line.
	///	m_bShowMenu = TRUE ;
	///	InitPopupMenu() ;
	///}

	DWORD targetID = pObject->GetID() ;
	if( m_byCurMode == eMonGuage_Character )
	{
		CParty* pParty = PARTYMGR->GetParty();

		if( PARTYMGR->IsPartyMember(targetID) )
		{
			m_byTargetState = e_MEMBER ;
		}
	}
}


//=================================================================================================
// NAME			: CheckHeroAtt
// PURPOSE		: Check and setting attribute of hero.
// ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::CheckHeroAtt()
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
// NAME			: CheckTargetAtt
// PURPOSE		: Check target type and setting attribute of target player.
// ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::CheckTargetAtt( CPlayer* pPlayer )
{
	int count = 0 ;

	BOOL bPartyMember = FALSE ;

	for( count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	{
		if( pPlayer->GetID() == PARTYMGR->GetPartyMemberInfo( count )->dwMemberID )
		{
			bPartyMember = TRUE ;
			break ;
		}
	}

	if( !bPartyMember )
	{
		m_byTargetState = e_NONE ;
	}
	else
	{
		m_byTargetState = e_MEMBER ;
	}
}


//=================================================================================================
//	NAME		: SetFace
//	PURPOSE		: Setting face image of target.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::SetFace()
{
	m_pMonsterFace->SetActive( FALSE ) ;
	m_pPlayerFace->SetActive( FALSE ) ;
	m_pNPCFace->SetActive( FALSE ) ;

	switch( m_byCurMode )
	{
	case eObjectKind_Monster :	
	case eObjectKind_BossMonster :
	case eObjectKind_SpecialMonster :
	case eObjectKind_FieldBossMonster :
	case eObjectKind_FieldSubMonster :
	case eObjectKind_ToghterPlayMonster :
		{
			m_pMonsterFace->SetActive( TRUE ) ;
		}
		break ;

	case eObjectKind_Player :
		{
			m_pPlayerFace->SetActive( TRUE ) ;
		}
		break ;

	case eObjectKind_Npc :
		{
			m_pNPCFace->SetActive( TRUE ) ;
		}
		break ;

	}
}


//=================================================================================================
//	NAME		: SetName
//	PURPOSE		: Setting name of target.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::SetName( CObject* pObject, char* name )
{
	m_pMonsterName->SetActive( FALSE ) ;
	m_pPlayerName->SetActive( FALSE ) ;
	m_pNPCName->SetActive( FALSE ) ;
	m_pNPCOccupation->SetActive( FALSE ) ;
	m_pMasterName->SetActive( FALSE ) ;	
	m_pPetName->SetActive( FALSE ) ;		

	char tempName[128] = { 0, } ;
	char realName[128] = { 0, } ;

	int msgNum = 0 ;

	WORD monsterKind = ((CMonster*)pObject)->GetObjectKind() ;

	WORD wLevel = 0x0000 ;

	switch( monsterKind )
	{
	case eObjectKind_Monster :
	case eObjectKind_BossMonster :
	case eObjectKind_SpecialMonster :
	case eObjectKind_FieldBossMonster :
	case eObjectKind_FieldSubMonster :
	case eObjectKind_ToghterPlayMonster :
		{
			m_pMonsterName->SetActive( TRUE ) ;

			wLevel = ((CMonster*)pObject)->GetSInfo()->Level ;
			strcpy(tempName, ((CMonster*)pObject)->GetSInfo()->Name ) ;
			sprintf( realName, "L%d %s", wLevel, tempName ) ;

			LEVELTYPE monsterLevel = ((CMonster*)pObject)->GetSInfo()->Level ;
			LEVELTYPE playerLevel  = HERO->GetLevel() ;
			LEVELTYPE levelGap	   = 0 ;

			if( playerLevel > monsterLevel )
			{
				levelGap = playerLevel - monsterLevel ;

				if( levelGap >= 1 && levelGap <= 5 )
				{
					m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(3) ) ;
				}
				else if( levelGap >= 6 && levelGap <= 10 )
				{
					m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(4) ) ;
				}
				else
				{
					m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(5) ) ;
				}
			}
			else if( monsterLevel > playerLevel )
			{
				levelGap = monsterLevel - playerLevel ;

				if( levelGap >= 1 && levelGap <= 5 )
				{
					m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(1) ) ;
				}
				else
				{
					m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(0) ) ;
				}
			}
			else
			{
				m_pMonsterName->SetFGColor( m_pMonsterName->GetExtendColor(2) ) ;
			}

			m_pMonsterName->SetStaticText( realName ) ;
		}
		break ;

	case eObjectKind_Player :
		{
			m_pPlayerName->SetActive( TRUE ) ;
			m_pPlayerName->SetStaticText( name ) ;
		}
		break ;

	case eObjectKind_Npc :
		{ 
			m_pNPCName->SetActive( TRUE ) ;
			m_pNPCName->SetStaticText( name ) ;
			// 070612 LYW --- MonsterGuageDlg : Check showjob.
			if( ((CNpc*)pObject)->GetSInfo()->ShowJob )
			{
				m_pNPCOccupation->SetActive( TRUE ) ;
				m_pNPCOccupation->SetStaticText(((CNpc*)pObject)->GetSInfo()->Name) ;
			}
		}
		break ;

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.08
	case eObjectKind_FarmObj:
		{
			m_pNPCName->SetActive( TRUE ) ;
			m_pNPCName->SetStaticText( name ) ;
			if( g_csFarmManager.CLI_GetFarmRenderObjList(((CSHFarmRenderObj*)pObject)->GetInfo()->nKind)->bShowGroupName )
			{
				m_pNPCOccupation->SetActive( TRUE ) ;
				m_pNPCOccupation->SetStaticText(g_csFarmManager.CLI_GetFarmRenderObjList(((CSHFarmRenderObj*)pObject)->GetInfo()->nKind)->szGroupName);
			}
		}
		break ;
	// E 농장시스템 추가 added by hseos 2007.05.08
	case eObjectKind_Pet:
		{
			m_pMasterName->SetActive( TRUE ) ;	
			m_pPetName->SetActive( TRUE ) ;

			CPet* pPet = (CPet*)pObject;
			m_pMasterName->SetStaticText( pPet->GetMasterName() );
			m_pPetName->SetStaticText( name ) ;
		}	
		break;
	}
}


//=================================================================================================
//	NAME		: SetGuildName
//	PURPOSE		: Setting guild name and union name of guild.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::SetGuildName( char* guildName, char* unionName )
{
	char tempName[128] = {0, } ;

	m_pGuildName->SetActive( TRUE ) ;
	sprintf( tempName, "%s%s", CHATMGR->GetChatMsg(0), guildName ) ;
	m_pGuildName->SetStaticText( tempName ) ;

	memset( tempName, 0, strlen( tempName ) ) ;

	m_pUnionName->SetActive( TRUE ) ;
	sprintf( tempName, "%s%s", CHATMGR->GetChatMsg(0), unionName ) ;
	m_pUnionName->SetStaticText( tempName ) ;
}


//=================================================================================================
//	NAME		: CheckPartyIdx
//	PURPOSE		: Check party index of target.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
BOOL CMonsterGuageDlg::CheckPartyIdx( CPlayer* pPlayer )
{
	DWORD heroIdx	= 0x00000000 ;
	DWORD targetIdx	= 0x00000000 ;

	heroIdx		= HERO->GetPartyIdx() ;

	//targetIdx	= pPlayer->GetPartyIdx() ;

	if( heroIdx == targetIdx )
	{
		return TRUE ;
	}
	else
	{
		return FALSE ;
	}
}

//=================================================================================================
//	NAME		: SetLife
//	PURPOSE		: Setting lif of target.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
// 070315 LYW --- MonsterGuageDlg : Modified function SetLife()
//void CMonsterGuageDlg::SetLife( CMonster* pPlayer )
void CMonsterGuageDlg::SetLife( CPlayer* pPlayer )
{
	/*
	char szValue[50];

	if(pPlayer->GetLife() > pPlayer->GetSInfo()->Life)
			pPlayer->SetLife(pPlayer->GetSInfo()->Life, 0) ;

	sprintf( szValue, "%d/%d", pPlayer->GetLife(), pPlayer->GetSInfo()->Life );

	if( m_playerLife )
	{
		m_playerLife->SetStaticText(szValue);
		m_playerLife->SetActive( TRUE ) ;
	}
	*/

	char szValue[50];

	CHARACTER_TOTALINFO* pInfo = pPlayer->GetCharacterTotalInfo() ;

	if( pInfo->Life > pInfo->MaxLife )
	{
		pInfo->Life = pInfo->MaxLife ;
	}
	
	sprintf( szValue, "%d/%d", pInfo->Life, pInfo->MaxLife );

	if( m_playerLife )
	{
		m_playerLife->SetStaticText(szValue);
		m_playerLife->SetActive( TRUE ) ;
	}
}


//=================================================================================================
//	NAME		: SetGuage
//	PURPOSE		: Setting guage of target.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::SetGuage( CObject* pObject )
{
	m_pHPMark->SetActive( FALSE ) ;
	m_pMonsterGuage->SetActive( FALSE ) ;
	m_pPlayerGuage->SetActive( FALSE ) ;

	WORD monsterKind = ((CMonster*)pObject)->GetObjectKind() ;

	switch( monsterKind )
	{
	case eObjectKind_Monster :	
	case eObjectKind_BossMonster :
	case eObjectKind_SpecialMonster :
	case eObjectKind_FieldBossMonster :
	case eObjectKind_FieldSubMonster :
	case eObjectKind_ToghterPlayMonster :
		{
			m_pHPMark->SetActive( TRUE ) ;
			m_pMonsterGuage->SetActive( TRUE ) ;
			m_pMonsterGuage->SetValue((float)((CMonster*)pObject)->GetLife()/(float)((CMonster*)pObject)->GetSInfo()->Life, 0);

			m_pCurMonster = ((CMonster*)pObject) ;
			m_Type = eObjectKind_Monster;
		}
		break ;

	case eObjectKind_Player :
		{
			m_pHPMark->SetActive( TRUE ) ;
			m_pPlayerGuage->SetActive( TRUE ) ;
			m_pPlayerGuage->SetValue((float)((CMonster*)pObject)->GetLife()/1, 0);
		}
		break ;

	case eObjectKind_Npc :
		{			
			m_pHPMark->SetActive( FALSE ) ;
		}
		break ;

	}
}


//=================================================================================================
//	NAME		: InitPopupMenu
//	PURPOSE		: Setting popup menu.
//	DATE		: January 23 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMonsterGuageDlg::InitPopupMenu()
{
	int count = 0 ;

	DWORD basicColor = m_pMenuBtn[0]->GetBasicColor() ;
	DWORD overColor  = m_pMenuBtn[0]->GetOverColor() ;
	DWORD pressColor = m_pMenuBtn[0]->GetPressColor() ;

	if( m_bShowMenu )
	{
		m_pMenuCallBtn->SetActive( TRUE ) ;

		switch( m_byHeroState )
		{
		case e_NONE :
			{
				m_byCurPopupMode = e_INVITE ;

				for( count = 0 ; count < MAX_POPUP_BGCOUNT ; ++count )
				{
					m_pMenuBG[ count ]->SetActive( TRUE ) ;
				}
				m_pMenuBG[4]->SetRelXY( (LONG)140, (LONG)68 ) ;

				for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
				{
					m_pMenuBtn[ count ]->SetActive( TRUE ) ;
				}

				// 070118 LYW --- Modified this part.
				/*
				m_pMenuBtn[0]->SetText("귓속말" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[1]->SetText("파티 초대" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[2]->SetText("거래" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[3]->SetText("결투" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[4]->SetText("취소" , basicColor, overColor, pressColor ) ;
				*/

				m_pMenuBtn[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[2]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[3]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[4]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
			}
			break ;

		case e_CAPTAIN :
			{
				if( m_byTargetState == e_MEMBER )
				{
					m_byCurPopupMode = e_STANDARD ;

					m_pMenuBG[0]->SetActive( TRUE ) ;
					m_pMenuBG[1]->SetActive( TRUE ) ;
					m_pMenuBG[2]->SetActive( TRUE ) ;
					m_pMenuBG[4]->SetActive( TRUE ) ;
					m_pMenuBG[4]->SetRelXY( (LONG)140, (LONG)52 ) ;

					for( count = 0 ; count < MAX_MENU_COUNT-1 ; ++count )
					{
						m_pMenuBtn[ count ]->SetActive( TRUE ) ;
					}

					// 070118 LYW --- Modified this part.
					/*
					m_pMenuBtn[0]->SetText("귓속말" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[1]->SetText("거래" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[2]->SetText("결투" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[3]->SetText("취소" , basicColor, overColor, pressColor ) ;
					*/
					m_pMenuBtn[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[2]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[3]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				}
				else
				{
					m_byCurPopupMode = e_INVITE ;

					for( count = 0 ; count < MAX_POPUP_BGCOUNT ; ++count )
					{
						m_pMenuBG[ count ]->SetActive( TRUE ) ;
					}
					m_pMenuBG[4]->SetRelXY( (LONG)140, (LONG)68 ) ;

					for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
					{
						m_pMenuBtn[ count ]->SetActive( TRUE ) ;
					}

					// 070118 LYW --- Modified this part.
					/*
					m_pMenuBtn[0]->SetText("귓속말" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[1]->SetText("파티 초대" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[2]->SetText("거래" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[3]->SetText("결투" , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[4]->SetText("취소" , basicColor, overColor, pressColor ) ;
					*/
					m_pMenuBtn[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[2]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[3]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
					m_pMenuBtn[4]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				}
			}
			break ;

		case e_MEMBER :
			{
				m_byCurPopupMode = e_STANDARD ;

				m_pMenuBG[0]->SetActive( TRUE ) ;
				m_pMenuBG[1]->SetActive( TRUE ) ;
				m_pMenuBG[2]->SetActive( TRUE ) ;
				m_pMenuBG[4]->SetActive( TRUE ) ;
				m_pMenuBG[4]->SetRelXY( (LONG)140, (LONG)52 ) ;

				for( count = 0 ; count < MAX_MENU_COUNT-1 ; ++count )
				{
					m_pMenuBtn[ count ]->SetActive( TRUE ) ;
				}

				// 070118 LYW --- Modified this part.
				/*
				m_pMenuBtn[0]->SetText("귓속말" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[1]->SetText("거래" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[2]->SetText("결투" , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[3]->SetText("취소" , basicColor, overColor, pressColor ) ;
				*/
				m_pMenuBtn[0]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[1]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[2]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
				m_pMenuBtn[3]->SetText(RESRCMGR->GetMsg( 982) , basicColor, overColor, pressColor ) ;
			}
			break ;
		}
	}
	else
	{
		for( count = 0 ; count < MAX_POPUP_BGCOUNT ; ++count )
		{
			m_pMenuBG[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_MENU_COUNT ; ++count )
		{
			m_pMenuBtn[ count ]->SetActive( FALSE ) ;
		}
	}
}

// 070418 LYW --- MonsterGuageDlg : Add function to control fade out timer.
BOOL CMonsterGuageDlg::IsStartTimer()
{
	if( m_FOTimer.Check() > 0 ) return TRUE ;
	
	return FALSE ;
}

void CMonsterGuageDlg::ResetDlg()
{
	m_FOTimer.End() ;
	SetAlpha(255) ;	

	m_pGuildName->SetActive( TRUE ) ;
}

void CMonsterGuageDlg::StartFadeOut()
{
	m_FOTimer.Init( 5000 ) ;
	m_FOTimer.Start() ;
}

void CMonsterGuageDlg::FadeAlpha()
{
	DWORD dwRemainTime = m_FOTimer.Check() ;
	BYTE byAlpha = (BYTE)(dwRemainTime /20) + 5 ;

	SetAlpha(byAlpha) ;	
	m_pMonsterName->SetTextAlpha(TRUE) ;
	m_pMonsterName->SetAlpha(byAlpha) ;

	if( byAlpha <= 30 || dwRemainTime <= 0 )
	{
		OBJECTMGR->SetSelectedObjectID(0);
	}
}