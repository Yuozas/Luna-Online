// UserInfoManager.cpp: implementation of the CUserInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserInfoManager.h"

#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"

#include "InventoryExDialog.h"
#include "OptionDialog.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "MHCamera.h"
#include "QuestTotalDialog.h"
#include "MinimapDlg.h"
#include "GMNotifyManager.h"
#include "PartyDialog.h"
#include "MHMap.h"

#include "QuestManager.h"

// 070202 LYW --- Include header file.
#include "../[CC]Header/GameResourceManager.h"

#include "QuestDialog.h"
#include "QuestQuickViewDialog.h"
#include "./interface/cListDialog.h"	

#include "Quest.h"

//#include "..\[CC]Quest\QuestScriptLoader.h"												// 퀘스트 스크립트 로더 헤더를 불러온다.
//#include "..\[CC]Quest\QuestString.h"													// 퀘스트 스트링 헤더를 불러온다.
#include "..\[CC]Quest\QuestInfo.h"														// 퀘스트 정보 헤더를 불러온다.
//#include "..\[CC]Quest\SubQuestInfo.h"													// 서브 퀘스트 정보 헤더를 불러온다.
//#include "..\[CC]Quest\QuestNpc.h"														// 퀘스트 npc 헤더를 불러온다.
//#include "..\[CC]Quest\QuestNpcData.h"													// 퀘스트 npc 데이터 헤더를 불러온다.



GLOBALTON(CUserInfoManager)


#define USERINFO_INTERFACE_VER 0x0006060801
#define USERINFO_USERSTATE_VER 0x0006060801

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserInfoManager::CUserInfoManager()
{
	m_strUserID[0] = 0;
	m_bMapChange = FALSE;
	m_bMinimapActive = TRUE;
	m_dwSaveFolderName = 0;
}

CUserInfoManager::~CUserInfoManager()
{
	
}

void CUserInfoManager::SetSaveFolderName( DWORD dwUserID )
{
	char* str = m_strUserID;
	DWORD Key = 740705;

	m_dwSaveFolderName = dwUserID + Key;
}

void CUserInfoManager::LoadUserInfo( DWORD dwFlag )
{
	char strFilePath[MAX_PATH];

	if( dwFlag & eUIK_INTERFACE )
	{
		wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UII", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, m_dwSaveFolderName );
		LoadInterfaceInfo( strFilePath  );
	}

	if( dwFlag & eUIK_USERSTATE )
	{
		wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UIS", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, m_dwSaveFolderName );
		LoadUserState( strFilePath );
	}

	if( HERO )
	{
		if( dwFlag & eUIK_USERQUEST )
		{
			wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UIQ", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, HERO->GetID());
			LoadUserQuestQuickView( strFilePath ) ;
		}
	}
}

void CUserInfoManager::SaveUserInfo( DWORD dwFlag )
{
	if( dwFlag == eUIK_NONE ) return;

	char strFilePath[MAX_PATH];

	//make directory
	wsprintf( strFilePath, "%s\\data\\interface\\log", DIRECTORYMGR->GetFullDirectoryName(eLM_Root) );
	CreateDirectory( strFilePath, NULL );
	wsprintf( strFilePath, "%s\\data\\interface\\log\\%d", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName );
	CreateDirectory( strFilePath, NULL );

	if( dwFlag & eUIK_INTERFACE )
	{
		wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UII", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, m_dwSaveFolderName );
		SaveInterfaceInfo( strFilePath );
	}

	if( dwFlag & eUIK_USERSTATE )
	{
		wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UIS", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, m_dwSaveFolderName );
		SaveUserState( strFilePath );
	}

	if( HERO )
	{
		if( dwFlag & eUIK_USERQUEST )
		{
			wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UIQ", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, HERO->GetID());
			SaveUserQuestQuickView( strFilePath ) ;
		}
	}
}

void CUserInfoManager::LoadInterfaceInfo( char* strFilePath )
{
	return ;

	HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	DWORD dwSize;
	DWORD dwVersion;
	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( dwVersion != USERINFO_INTERFACE_VER )
	{
		CloseHandle( hFile );
		return;
	}

	cDialog* pDlg;
	sUSERINFO_INTERFACE UII;
	if( !ReadFile( hFile, &UII, sizeof(UII), &dwSize, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}

	CloseHandle( hFile );

	pDlg = WINDOWMGR->GetWindowForID( CG_GUAGEDLG );
	if( m_bMapChange )
	{	
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_HeroGuage].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_HeroGuage].dwFlag & eUIF_ACTIVE );
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_HeroGuage].lPosX, UII.DIALOG_INFO[eSD_HeroGuage].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_HeroGuage].lPosX, UII.DIALOG_INFO[eSD_HeroGuage].lPosY );

/*	pDlg = WINDOWMGR->GetWindowForID( CG_MONSTERGUAGEDLG );
	if( m_bMapChange )
	{
		if( pDlg->IsActive() != UII.DIALOG_INFO[eSD_MonsterGuage].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_MonsterGuage].dwFlag & eUIF_ACTIVE );
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_MonsterGuage].lPosX, UII.DIALOG_INFO[eSD_MonsterGuage].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_MonsterGuage].lPosX, UII.DIALOG_INFO[eSD_MonsterGuage].lPosY );
*/	
	pDlg = WINDOWMGR->GetWindowForID( MNM_DIALOG );

	if( m_bMapChange )
	{
		pDlg->SetActive( UII.DIALOG_INFO[eSD_MiniMap].dwFlag & eUIF_ACTIVE );

		m_bMinimapActive = UII.DIALOG_INFO[eSD_MiniMap].dwFlag & eUIF_ACTIVE;

//		if( UII.DIALOG_INFO[eSD_MiniMap].dwFlag & eUIF_SECONDFORM )
//			((CMiniMapDlg*)pDlg)->ToggleMinimapMode();
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_MiniMap].lPosX, UII.DIALOG_INFO[eSD_MiniMap].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_MiniMap].lPosX, UII.DIALOG_INFO[eSD_MiniMap].lPosY );
/*
	pDlg = WINDOWMGR->GetWindowForID( QI_QUICKDLG );
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Quick].lPosX, UII.DIALOG_INFO[eSD_Quick].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Quick].lPosX, UII.DIALOG_INFO[eSD_Quick].lPosY );

//예전위치라면..잭팟을 가리게 되므로, 이동시켜주자.
	if( (int)pDlg->GetAbsX() == 977 && (int)pDlg->GetAbsY() == 187 )
		pDlg->SetAbsXY( 977, 209 );
//--
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Quick].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Quick].dwFlag & eUIF_ACTIVE );
		if( UII.DIALOG_INFO[eSD_Quick].dwFlag & eUIF_TABNUM2 )
			((CQuickDialog*)pDlg)->SelectPageDirect(1);
		else if( UII.DIALOG_INFO[eSD_Quick].dwFlag & eUIF_TABNUM3 )
			((CQuickDialog*)pDlg)->SelectPageDirect(2);
		else if( UII.DIALOG_INFO[eSD_Quick].dwFlag & eUIF_TABNUM4 )
			((CQuickDialog*)pDlg)->SelectPageDirect(3);
	}
*/
	// 061220 LYW --- Delete this code.
	/*
	pDlg = WINDOWMGR->GetWindowForID( MI_MAINDLG );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_MainBar].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_MainBar].dwFlag & eUIF_ACTIVE );
	}
	((CMainBarDialog*)pDlg)->Fold( UII.DIALOG_INFO[eSD_MainBar].dwFlag & eUIF_SECONDFORM );	////	
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_MainBar].lPosX, UII.DIALOG_INFO[eSD_MainBar].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_MainBar].lPosX, UII.DIALOG_INFO[eSD_MainBar].lPosY );
//임시.. 이거.. 헬프 아이콘 들어가면서..
//아이템몰 하나 더 들어갔당...	
	if( (int)pDlg->GetAbsX() == 464 && (int)pDlg->GetAbsY() == 726 )
		pDlg->SetAbsXY( 422, 726 );
		*/

	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	pDlg = WINDOWMGR->GetWindowForID( CTI_DLG );
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Chat].lPosX, UII.DIALOG_INFO[eSD_Chat].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Chat].lPosX, UII.DIALOG_INFO[eSD_Chat].lPosY );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_ACTIVE );
		if( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_TABNUM2 )
			((CChatDialog*)pDlg)->SelectMenu( 1 );
		else if( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_TABNUM3 )
			((CChatDialog*)pDlg)->SelectMenu( 2 );
		else if( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_TABNUM4 )
			((CChatDialog*)pDlg)->SelectMenu( 4 );
		
		int nLineNum = UII.DIALOG_INFO[eSD_Chat].dwParam;
		int nCurNum = ((CChatDialog*)pDlg)->GetLineNum();

		while( nLineNum != nCurNum )
		{
			if( nLineNum > nCurNum )
			{
				((CChatDialog*)pDlg)->OnActionEvent( CTI_BTN_TALLER, NULL, WE_BTNCLICK );
				++nCurNum;
			}
			else
			{
				((CChatDialog*)pDlg)->OnActionEvent( CTI_BTN_SMALLER, NULL, WE_BTNCLICK );
				--nCurNum;
			}
		}

		//
		if( !(UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_SUBACTIVE) )
			((CChatDialog*)pDlg)->SetAllShoutBtnPushed( FALSE );
	}
	*/

	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	pDlg = WINDOWMGR->GetWindowForID( CHA_DLG );
	if( pDlg )
	{
		if( m_bMapChange )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_SUBACTIVE );
	}
	*/

	pDlg = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Inventory].lPosX, UII.DIALOG_INFO[eSD_Inventory].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Inventory].lPosX, UII.DIALOG_INFO[eSD_Inventory].lPosY );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Inventory].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Inventory].dwFlag & eUIF_ACTIVE );
		if( UII.DIALOG_INFO[eSD_Inventory].dwFlag & eUIF_TABNUM2 )
			((CInventoryExDialog*)pDlg)->SelectTab( 1 );
		else if( UII.DIALOG_INFO[eSD_Inventory].dwFlag & eUIF_TABNUM3 )
			((CInventoryExDialog*)pDlg)->SelectTab( 2 );
		else if( UII.DIALOG_INFO[eSD_Inventory].dwFlag & eUIF_TABNUM4 )
			((CInventoryExDialog*)pDlg)->SelectTab( 3 );
	}

	// 070510 LYW --- UserInfoManager : Deactive inventory.
	pDlg->SetActive(FALSE) ;

	// 070113 LYW --- Delete this part for a moment.
	/*
	pDlg = WINDOWMGR->GetWindowForID( CI_CHARDLG );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Character].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Character].dwFlag & eUIF_ACTIVE );
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Character].lPosX, UII.DIALOG_INFO[eSD_Character].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Character].lPosX, UII.DIALOG_INFO[eSD_Character].lPosY );
		*/

//	pDlg = WINDOWMGR->GetWindowForID( MGI_SKILLSURYUNDLG );
//	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Skill].lPosX, UII.DIALOG_INFO[eSD_Skill].lPosY ) )
//		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Skill].lPosX, UII.DIALOG_INFO[eSD_Skill].lPosY );
	if( m_bMapChange )
	{
/*		((CSkillSuryunDialog*)pDlg)->GetSkillDialog()->SetSkillExtend( UII.DIALOG_INFO[eSD_Skill].dwParam );

		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_ACTIVE );
		if( UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_TABNUM2 )
		{
			((CSkillSuryunDialog*)pDlg)->SelectTab( 1 );
			if( UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_SUBTABNUM2 )
				GAMEIN->GetSuryunDialog()->SelectTab( 1 );
			else if( UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_SUBTABNUM3 )
				GAMEIN->GetSuryunDialog()->SelectTab( 2 );
			else if( UII.DIALOG_INFO[eSD_Skill].dwFlag & eUIF_SUBTABNUM4 )
				GAMEIN->GetSuryunDialog()->SelectTab( 3 );
		}
*/	}
/*
	pDlg = WINDOWMGR->GetWindowForID( NOTE_NOTEDLG );
	if( m_bMapChange )
	{
		if( pDlg->IsActive() != UII.DIALOG_INFO[eSD_Note].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Note].dwFlag & eUIF_ACTIVE );
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Note].lPosX, UII.DIALOG_INFO[eSD_Note].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Note].lPosX, UII.DIALOG_INFO[eSD_Note].lPosY );

	pDlg = WINDOWMGR->GetWindowForID( FRI_FRIENDDLG );
	if( m_bMapChange )
	{
		if( pDlg->IsActive() != UII.DIALOG_INFO[eSD_Friend].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Friend].dwFlag & eUIF_ACTIVE );
	}
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Friend].lPosX, UII.DIALOG_INFO[eSD_Friend].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Friend].lPosX, UII.DIALOG_INFO[eSD_Friend].lPosY );
*/
	pDlg = WINDOWMGR->GetWindowForID( OTI_TABDLG );
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Option].lPosX, UII.DIALOG_INFO[eSD_Option].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Option].lPosX, UII.DIALOG_INFO[eSD_Option].lPosY );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Option].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Option].dwFlag & eUIF_ACTIVE );
		if( UII.DIALOG_INFO[eSD_Option].dwFlag & eUIF_TABNUM2 )
			((COptionDialog*)pDlg)->SelectTab( 1 );
		else if( UII.DIALOG_INFO[eSD_Option].dwFlag & eUIF_TABNUM3 )
			((COptionDialog*)pDlg)->SelectTab( 2 );
		else if( UII.DIALOG_INFO[eSD_Option].dwFlag & eUIF_TABNUM4 )
			((COptionDialog*)pDlg)->SelectTab( 3 );	
	}

	pDlg = WINDOWMGR->GetWindowForID( QUE_TOTALDLG );
	if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Quest].lPosX, UII.DIALOG_INFO[eSD_Quest].lPosY ) )
		pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Quest].lPosX, UII.DIALOG_INFO[eSD_Quest].lPosY );
	if( m_bMapChange )
	{
		if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_Quest].dwFlag & eUIF_ACTIVE )
			pDlg->SetActive( UII.DIALOG_INFO[eSD_Quest].dwFlag & eUIF_ACTIVE );
//		if( UII.DIALOG_INFO[eSD_Quest].dwFlag & eUIF_TABNUM2 )
//			((CQuestTotalDialog*)pDlg)->SelectTab( 1 );
//		else if( UII.DIALOG_INFO[eSD_Quest].dwFlag & eUIF_TABNUM3 )
//			((CQuestTotalDialog*)pDlg)->SelectTab( 2 );
	}

	pDlg = WINDOWMGR->GetWindowForID( GMOT_OPENTALKDLG );
	if( pDlg )
	{
		if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_EventNotify].lPosX, UII.DIALOG_INFO[eSD_EventNotify].lPosY ) )
			pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_EventNotify].lPosX, UII.DIALOG_INFO[eSD_EventNotify].lPosY );
		if( m_bMapChange )
		{
			if( (DWORD)pDlg->IsActive() != UII.DIALOG_INFO[eSD_EventNotify].dwFlag & eUIF_ACTIVE )
				pDlg->SetActive( UII.DIALOG_INFO[eSD_EventNotify].dwFlag & eUIF_ACTIVE );
		}
	}

	pDlg = WINDOWMGR->GetWindowForID( PA_BTNDLG );
	CPartyDialog* pParty = GAMEIN->GetPartyDialog();
	if( pParty )
	{
		if( IsValidInterfacePos( pDlg->GetCaptionRect(), UII.DIALOG_INFO[eSD_Party].lPosX, UII.DIALOG_INFO[eSD_Party].lPosY ) )
			pDlg->SetAbsXY( UII.DIALOG_INFO[eSD_Party].lPosX, UII.DIALOG_INFO[eSD_Party].lPosY );
		if( m_bMapChange )
		{
			if( UII.DIALOG_INFO[eSD_Party].dwFlag & eUIF_ACTIVE )	// option
				pParty->SetOptionActive( FALSE );
			else
				pParty->SetOptionActive( TRUE );
			if( UII.DIALOG_INFO[eSD_Party].dwParam & eUIF_ACTIVE )	// member
				pParty->SetMemberActive( FALSE );
			else
				pParty->SetMemberActive( TRUE ); 
		}
	}
}

void CUserInfoManager::SaveInterfaceInfo( char* strFilePath )
{
	return ;

	cDialog* pDlg;
	sUSERINFO_INTERFACE UII;

	pDlg = WINDOWMGR->GetWindowForID( CG_GUAGEDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_HeroGuage].lPosX		= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_HeroGuage].lPosY		= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_HeroGuage].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_HeroGuage].dwFlag = 0;
	}

	pDlg = WINDOWMGR->GetWindowForID( CG_MONSTERGUAGEDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_MonsterGuage].lPosX		= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_MonsterGuage].lPosY		= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_MonsterGuage].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_MonsterGuage].dwFlag = 0;
	}

	pDlg = WINDOWMGR->GetWindowForID( MNM_DIALOG );
	
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_MiniMap].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_MiniMap].lPosY			= (LONG)pDlg->GetAbsY();

		if( ((CMiniMapDlg*)pDlg)->CanActive() )
		{
			if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_MiniMap].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
			else					UII.DIALOG_INFO[eSD_MiniMap].dwFlag = 0;
			
			//		if( ((CMiniMapDlg*)pDlg)->GetCurMode() == eMINIMAPMODE_SMALL )
			//			UII.DIALOG_INFO[eSD_MiniMap].dwFlag |= eUIF_SECONDFORM;
		}
		else 
		{
			UII.DIALOG_INFO[eSD_MiniMap].dwFlag = m_bMinimapActive;
		}
	}
/*
	pDlg = WINDOWMGR->GetWindowForID( QI_QUICKDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Quick].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Quick].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Quick].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Quick].dwFlag = 0;
		if(	((CQuickDialog*)pDlg)->GetSelectedPageNumber() == 1 )
			UII.DIALOG_INFO[eSD_Quick].dwFlag |= eUIF_TABNUM2;
		else if( ((CQuickDialog*)pDlg)->GetSelectedPageNumber() == 2 )
			UII.DIALOG_INFO[eSD_Quick].dwFlag |= eUIF_TABNUM3;
		else if( ((CQuickDialog*)pDlg)->GetSelectedPageNumber() == 3 )
			UII.DIALOG_INFO[eSD_Quick].dwFlag |= eUIF_TABNUM4;
	}
*/
	// 061220 LYW --- Delete this code.
	/*
	pDlg = WINDOWMGR->GetWindowForID( MI_MAINDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_MainBar].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_MainBar].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_MainBar].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_MainBar].dwFlag = 0;
		//접혔을 때
		if( ((CMainBarDialog*)pDlg)->IsFolded() ) UII.DIALOG_INFO[eSD_MainBar].dwFlag |= eUIF_SECONDFORM;
	}
	*/
	
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	pDlg = WINDOWMGR->GetWindowForID( CTI_DLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Chat].lPosX				= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Chat].lPosY				= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Chat].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Chat].dwFlag = 0;
		if( ((CChatDialog*)pDlg)->GetCurSheetNum() == 1 )
			UII.DIALOG_INFO[eSD_Chat].dwFlag |= eUIF_TABNUM2;
		else if( ((CChatDialog*)pDlg)->GetCurSheetNum() == 2 )
			UII.DIALOG_INFO[eSD_Chat].dwFlag |= eUIF_TABNUM3;
		else if( ((CChatDialog*)pDlg)->GetCurSheetNum() == 4 )
			UII.DIALOG_INFO[eSD_Chat].dwFlag |= eUIF_TABNUM4;		

		UII.DIALOG_INFO[eSD_Chat].dwParam = ((CChatDialog*)pDlg)->GetLineNum();
	}
	*/
	
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	pDlg = WINDOWMGR->GetWindowForID( CHA_DLG );
	if( pDlg )
	{
		if( pDlg->IsActive() )
			UII.DIALOG_INFO[eSD_Chat].dwFlag |= eUIF_SUBACTIVE;
		else if( UII.DIALOG_INFO[eSD_Chat].dwFlag & eUIF_SUBACTIVE )
			UII.DIALOG_INFO[eSD_Chat].dwFlag ^= eUIF_SUBACTIVE;
	}
	*/

	pDlg = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Inventory].lPosX		= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Inventory].lPosY		= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Inventory].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Inventory].dwFlag = 0;
		if( ((CInventoryExDialog*)pDlg)->GetCurTabNum() == 1 )
			UII.DIALOG_INFO[eSD_Inventory].dwFlag |= eUIF_TABNUM2;
		else if( ((CInventoryExDialog*)pDlg)->GetCurTabNum() == 2 )
			UII.DIALOG_INFO[eSD_Inventory].dwFlag |= eUIF_TABNUM3;
		else if( ((CInventoryExDialog*)pDlg)->GetCurTabNum() == 3 )
			UII.DIALOG_INFO[eSD_Inventory].dwFlag |= eUIF_TABNUM4;
	}

	pDlg = WINDOWMGR->GetWindowForID( CI_CHARDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Character].lPosX		= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Character].lPosY		= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Character].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Character].dwFlag = 0;
	}
/*
	pDlg = WINDOWMGR->GetWindowForID( MGI_SKILLSURYUNDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Skill].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Skill].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Skill].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Skill].dwFlag = 0;
		if( ((CSkillSuryunDialog*)pDlg)->GetCurTabNum() == 1 )
		{
			UII.DIALOG_INFO[eSD_Skill].dwFlag |= eUIF_TABNUM2;
			if( GAMEIN->GetSuryunDialog()->GetCurTabNum() == 1 )
				UII.DIALOG_INFO[eSD_Skill].dwFlag |= eUIF_SUBTABNUM2;
			else if( GAMEIN->GetSuryunDialog()->GetCurTabNum() == 2 )
				UII.DIALOG_INFO[eSD_Skill].dwFlag |= eUIF_SUBTABNUM3;
			else if( GAMEIN->GetSuryunDialog()->GetCurTabNum() == 3 )
				UII.DIALOG_INFO[eSD_Skill].dwFlag |= eUIF_SUBTABNUM4;
		}

		UII.DIALOG_INFO[eSD_Skill].dwParam = ((CSkillSuryunDialog*)pDlg)->GetSkillDialog()->IsSkillExtend();
	}
*/
/*
	pDlg = WINDOWMGR->GetWindowForID( NOTE_NOTEDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Note].lPosX				= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Note].lPosY				= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Note].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Note].dwFlag = 0;
	}

	pDlg = WINDOWMGR->GetWindowForID( FRI_FRIENDDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Friend].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Friend].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Friend].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Friend].dwFlag = 0;
	}
*/
	pDlg = WINDOWMGR->GetWindowForID( OTI_TABDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Option].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Option].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Option].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Option].dwFlag = 0;
		if( ((COptionDialog*)pDlg)->GetCurTabNum() == 1 )
			UII.DIALOG_INFO[eSD_Option].dwFlag |= eUIF_TABNUM2;
		if( ((COptionDialog*)pDlg)->GetCurTabNum() == 2 )
			UII.DIALOG_INFO[eSD_Option].dwFlag |= eUIF_TABNUM3;
		if( ((COptionDialog*)pDlg)->GetCurTabNum() == 3 )
			UII.DIALOG_INFO[eSD_Option].dwFlag |= eUIF_TABNUM4;
	}

	pDlg = WINDOWMGR->GetWindowForID( QUE_TOTALDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Quest].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Quest].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )	UII.DIALOG_INFO[eSD_Quest].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
		else					UII.DIALOG_INFO[eSD_Quest].dwFlag = 0;
//		if( ((CQuestTotalDialog*)pDlg)->GetCurTabNum() == 1 )
//			UII.DIALOG_INFO[eSD_Quest].dwFlag |= eUIF_TABNUM2;
//		if( ((CQuestTotalDialog*)pDlg)->GetCurTabNum() == 2 )
//			UII.DIALOG_INFO[eSD_Quest].dwFlag |= eUIF_TABNUM3;
	}

	pDlg = WINDOWMGR->GetWindowForID( GMOT_OPENTALKDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_EventNotify].lPosX			= (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_EventNotify].lPosY			= (LONG)pDlg->GetAbsY();
		if( pDlg->IsActive() )
		{
			UII.DIALOG_INFO[eSD_EventNotify].dwFlag = eUIF_ACTIVE; // '=' 초기화도 같이
			NOTIFYMGR->SetEventNotifyChanged( TRUE );
		}
		else
		{
			UII.DIALOG_INFO[eSD_EventNotify].dwFlag = 0;
		}		
	}	

	// party
	pDlg = WINDOWMGR->GetWindowForID( PA_BTNDLG );
	if( pDlg )
	{
		UII.DIALOG_INFO[eSD_Party].lPosX = (LONG)pDlg->GetAbsX();
		UII.DIALOG_INFO[eSD_Party].lPosY = (LONG)pDlg->GetAbsY();

		CPartyDialog* pParty = GAMEIN->GetPartyDialog();
		if( pParty )
		{
			if( pParty->IsOption() )	UII.DIALOG_INFO[eSD_Party].dwFlag = eUIF_ACTIVE;
			else						UII.DIALOG_INFO[eSD_Party].dwFlag = 0;

			if( pParty->IsMember() )	UII.DIALOG_INFO[eSD_Party].dwParam = eUIF_ACTIVE;
			else						UII.DIALOG_INFO[eSD_Party].dwParam = 0;
		}
	}

//----
	HANDLE hFile = CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	DWORD dwSize;
	DWORD dwVersion = USERINFO_INTERFACE_VER;
	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL );
	WriteFile( hFile, &UII, sizeof(UII), &dwSize, NULL );

	CloseHandle( hFile );
}


void CUserInfoManager::LoadUserState( char* strFilePath )
{
	HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	DWORD dwSize;
	DWORD dwVersion;
	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( dwVersion != USERINFO_USERSTATE_VER )
	{
		CloseHandle( hFile );
		return;
	}

	sUSERINFO_USERSTATE UIS;
	if( !ReadFile( hFile, &UIS, sizeof(UIS), &dwSize, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}

	CloseHandle( hFile );

	if( m_bMapChange && HERO )
	{
		HERO->SetKyungGongMode( UIS.bIsKyungGongMode );
		HERO->SetKyungGongGrade( UIS.wKyungGongIdx );	//security
		CAMERA->SetDistance( 0, UIS.fZoomDistance, 0 );
	}
}

void CUserInfoManager::SaveUserState( char* strFilePath )
{
	sUSERINFO_USERSTATE UIS;

	if( HERO == NULL )	return;

	UIS.bIsKyungGongMode	= HERO->GetKyungGongMode();
	UIS.wKyungGongIdx		= HERO->GetKyungGongGrade();
	UIS.fZoomDistance		= CAMERA->GetDistance(0);

	HANDLE hFile = CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	DWORD dwSize;
	DWORD dwVersion = USERINFO_USERSTATE_VER;
	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL );
	WriteFile( hFile, &UIS, sizeof(UIS), &dwSize, NULL );

	CloseHandle( hFile );
}

void CUserInfoManager::SaveUserQuestQuickView( char* strFilePath )						// 유저의 퀘스트 알림이 정보를 저장하는 함수.
{
	sUSERINFO_QUESTQUICKVIEW UIQ ;														// 퀘스트 알림이에 등록 된 퀘스트 아이디를 저장하는 구조체를 선언한다.

	if( HERO == NULL ) return ;															// 히어로 정보가 없으면 리턴 처리를 한다.

	/*CQuest* pQuest = NULL ;

	int nCount = 0 ;

	while(pQuest = QUESTMGR->GetQuest( nCount ))
	{
		++nCount ;

		UIQ.list.AddTail(pQuest) ;
	}*/

	CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.

	if( !pQuickView ) return ;

	cPtrList* pList = NULL ;
	pList = pQuickView->GetViewList() ;

	if( !pList ) return ;

	DWORD* pIdx ;

	int nCount = 0 ;

	PTRLISTPOS pos = NULL ;
	pos = pList->GetHeadPosition() ;

	while(pos)
	{
		pIdx = NULL ;
		pIdx = (DWORD*)pList->GetNext(pos) ;

		if( !pIdx ) continue ;

		UIQ.dwQuestID[nCount] = *pIdx ;

		++nCount ;
	}

	//for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )						// 퀘스트 알림이 최대 등록 가능 수 만큼 for문을 돌린다.
	//{
	//	UIQ.dwQuestID[count] = pQuickView->GetViewList() ;				// 퀘스트 매니져로 부터 카운트에 해당하는 인덱스를 받아서 세팅한다.
	//}

	//for( int count = 0 ; count < MAX_QUEST_COUNT ; ++count )							// 퀘스트 알림이 최대 등록 가능 수 만큼 for문을 돌린다.
	//{
	//	// 퀘스트 창의 정보를 저장한다.	
	//}

	//QUESTMGR->SaveQuestInfoFromQuickView(UIQ.dwQuestID) ;								// 퀘스트 매니져로 부터 카운트에 해당하는 인덱스를 받아서 세팅한다.

	HANDLE hFile = CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,		// 파일 패스에 따라 파일을 생성한다.
							   FILE_ATTRIBUTE_NORMAL, NULL ) ;

	if( hFile == INVALID_HANDLE_VALUE )													// 결과가 실패 했으면 리턴 처리를 한다.
	{
		return ;
	}

	DWORD dwSize ;																		// 사이즈를 담을 변수를 선언한다.
	//DWORD dwVersion = USERINFO_USERSTATE_VER ;											// 버전을 담을 변수를 선언하고 세팅한다.
	DWORD dwVersion = HERO->GetID() ;											// 버전을 담을 변수를 선언하고 세팅한다.

	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL ) ;						// 버전과 사이즈를 기록한다.
	WriteFile( hFile, &UIQ, sizeof(UIQ), &dwSize, NULL ) ;								// 퀘스트 인덱스 정보를 기록한다.

	CloseHandle( hFile ) ;																// 파일을 닫는다.
}

//void CUserInfoManager::LoadUserQuestQuickView( char* strFilePath ) 						// 유저의 퀘스트 알림이 정보를 로딩하는 함수.
//{
//	if( !HERO )
//	{
//		return ;
//	}
//
//	HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL,						// 지정된 파일 패스로 파일을 읽는다.
//							   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
//
//	if( hFile == INVALID_HANDLE_VALUE )													// 실패하면, 
//	{
//		return ;																		// 리턴 처리를 한다.
//	}
//
//	DWORD dwSize ;																		// 사이즈를 담을 변수를 선언한다.
//	DWORD dwVersion ;																	// 버전을 담을 변수를 선언한다.
//
//	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL ) )					// 사이즈와 버전을 읽어들인다. 실패하면,
//	{
//		CloseHandle( hFile ) ;															// 파일을 닫는다.
//		return ;																		// 리턴 처리를 한다.
//	}
//
//	if( dwVersion != HERO->GetID() )															// 버전이 같지 않으면,
//	{
//		CloseHandle( hFile ) ;															// 파일을 닫는다.
//		return ;																		// 리턴 처리를 한다.
//	}
//
//	sUSERINFO_QUESTQUICKVIEW UIQ ;														// 퀘스트 알림이에 등록 된 퀘스트 아이디를 저장하는 구조체.
//
//	if( !ReadFile( hFile, &UIQ, sizeof(UIQ), &dwSize, NULL ) )							// 퀘스트 아이디를 읽어들인다. 실패하면,
//	{
//		CloseHandle( hFile ) ;															// 파일을 닫는다.
//		return ;																		// 리턴 처리를 한다.
//	}
//
//	CloseHandle( hFile ) ;																// 파일을 닫는다.
//
//	//if( m_bMapChange )																	// 히어로 정보가 유효하고 맵 변경 중이면,
//	//{
//		QUESTMGR->LoadQuickViewInfoFromFile(UIQ.quickInfo) ;				// 퀘스트 매니져에 읽어들인 정보를 세팅한다.
//
//		//for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )					// 등록 가능 최대 수 만큼 for문을 돌린다.
//		//{
//		//	QUESTMGR->RegistQuestIDToQuickView(count, UIQ.dwQuestID[count]) ;			// 퀘스트 매니져에 읽어들인 정보를 세팅한다.
//		//}
//
//		//CQuestDialog* pDlg = GAMEIN->GetQuestDialog() ;
//
//		//if( pDlg )
//		//{
//		//	pDlg->GetQuestList()->RemoveAll() ;
//		//	pDlg->GetQuestDescList()->RemoveAll() ;
//
//		//	CQuest* pQuest ;
//
//		//	PTRLISTPOS pos = UIQ.list.GetHeadPosition() ;
//
//		//	while(pos)
//		//	{
//		//		pQuest = NULL ;
//		//		pQuest = (CQuest*)UIQ.list.GetNext(pos) ;
//
//		//		if( pQuest )
//		//		{
//		//			DWORD dwQuestIdx = pQuest->GetQuestIdx() ;
//
//		//			CQuestInfo* pQuestInfo =NULL ;														// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
//		//			pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;												// 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//		//			if( pQuestInfo )
//		//			{
//		//				int nMapNum = pQuestInfo->GetMapNum() ;												// 퀘스트 정보에 있는 맵 번호를 받는다.
//
//		//				QUESTMGR->AddMapTree(nMapNum) ;															// 퀘스트를 수락한 맵 번호에 따른 트리를 추가한다.
//
//		//				QUESTMGR->AddQuestTitle(nMapNum, dwQuestIdx) ;											// 퀘스트 제목을 퀘스트 리스트에 추가한다.
//		//			}
//		//		}
//		//	}	
//		//}
//	//}
//}

void CUserInfoManager::LoadUserQuestQuickView( char* strFilePath ) 						// 유저의 퀘스트 알림이 정보를 로딩하는 함수.
{
	if( !HERO )
	{
		return ;
	}

	HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL,						// 지정된 파일 패스로 파일을 읽는다.
							   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	if( hFile == INVALID_HANDLE_VALUE )													// 실패하면, 
	{
		return ;																		// 리턴 처리를 한다.
	}

	DWORD dwSize ;																		// 사이즈를 담을 변수를 선언한다.
	DWORD dwVersion ;																	// 버전을 담을 변수를 선언한다.

	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwSize, NULL ) )					// 사이즈와 버전을 읽어들인다. 실패하면,
	{
		CloseHandle( hFile ) ;															// 파일을 닫는다.
		return ;																		// 리턴 처리를 한다.
	}

	if( dwVersion != HERO->GetID() )															// 버전이 같지 않으면,
	{
		CloseHandle( hFile ) ;															// 파일을 닫는다.
		return ;																		// 리턴 처리를 한다.
	}

	sUSERINFO_QUESTQUICKVIEW UIQ ;														// 퀘스트 알림이에 등록 된 퀘스트 아이디를 저장하는 구조체.

	if( !ReadFile( hFile, &UIQ, sizeof(UIQ), &dwSize, NULL ) )							// 퀘스트 아이디를 읽어들인다. 실패하면,
	{
		CloseHandle( hFile ) ;															// 파일을 닫는다.
		return ;																		// 리턴 처리를 한다.
	}

	CloseHandle( hFile ) ;																// 파일을 닫는다.

	QUESTMGR->LoadQuickViewInfoFromFile(UIQ.dwQuestID) ;				// 퀘스트 매니져에 읽어들인 정보를 세팅한다.
}

void CUserInfoManager::DeleteUserQuestInfo(CPlayer* pPlayer)
{
	if( !pPlayer ) return ;

	char strFilePath[MAX_PATH];
	wsprintf( strFilePath, "%s\\data\\interface\\log\\%d\\%d.UIQ", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), m_dwSaveFolderName, pPlayer->GetID());
	DeleteFile(strFilePath) ;
}


void CUserInfoManager::SetUserID( char* strID )
{
	strcpy( m_strUserID, strID );
}


//for Interface
BOOL CUserInfoManager::IsValidInterfacePos( RECT* prcCaption, LONG lX, LONG lY )
{
	// 070202 LYW --- CUserInfoManager : Modified limited area.
	/*
	if( lX + prcCaption->left < 0 )		return FALSE;
	if( lX + prcCaption->right > 1024 ) return FALSE;
	if( lY + prcCaption->top < 0 )		return FALSE;
	if( lY + prcCaption->bottom > 768 ) return FALSE;
	*/

	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( lX + prcCaption->left < 0 )		return FALSE;
	if( lX + prcCaption->right > (LONG)(dispInfo.dwWidth) ) return FALSE;
	if( lY + prcCaption->top < 0 )		return FALSE;
	if( lY + prcCaption->bottom > (LONG)(dispInfo.dwHeight) ) return FALSE;

	// 070202 LYW --- End.

	return TRUE;
}
/*
void CUserInfoManager::AddSaveDialog( cDialog* pDlg )
{
	m_listInterface.AddTail( pDlg );
}

*/
