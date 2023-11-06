// GMToolManager.cpp: implementation of the CGMToolManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _GMTOOL_

#include "GMToolManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "CheatMsgParser.h"
#include "FilteringTable.h"
#include "ChatManager.h"

#include "MHMap.h"
#include "Commdlg.h"
#include "CommCtrl.h"
#include "MHFile.h"
#include "cSkillTreeManager.h"
#include "MainGame.h"
#include "WeatherManager.h"

extern HWND _g_hWnd;
extern HINSTANCE g_hInst;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CGMToolManager)

//extern HWND ghWnd;

extern int g_MapChannelNum[100];




CGMToolManager::CGMToolManager()
{
	m_hWndDlg = NULL;
	ZeroMemory( m_hWndSub, sizeof( m_hWndSub ) );

	m_nSelectMenu	= eMenu_Count;
	m_bShow			= FALSE;
	m_bCanUse		= FALSE;
	m_bSubShow		= FALSE;
	m_cbChannelCount	= 0;

	m_lBossStartIndex	= 0;
	m_hWndChatList		= NULL;

	m_nNumNow			= 0;
	m_nNumWait			= 0;
	m_nBufLen			= 0;

	m_bLogin			= FALSE;
	m_nPower			= eGM_POWER_MAX;	//3
	m_hWndLoginDlg		= NULL;
//ÀÓ½Ã
	m_nNullMove			= 0;
}

CGMToolManager::~CGMToolManager()
{

}

BOOL CGMToolManager::CreateGMDialog()
{
	if( m_hWndDlg ) return FALSE;

	m_hWndDlg = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GMDIALOG), _g_hWnd, GMDlgProc );	
	m_hWndLoginDlg = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GMLOGIN), _g_hWnd, GMLoginDlgProc );

	CreateGMSubDialog();

//	CHEATMGR->SetCheatEnable( TRUE );
	return TRUE;
}

BOOL CGMToolManager::CreateGMSubDialog()
{
	m_hWndSub[eMenu_Move]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_MOVE_DIALOG), m_hWndDlg, GMSubMoveDlgProc );
	m_hWndSub[eMenu_Where]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_WHERE_DIALOG), m_hWndDlg, GMSubWhereDlgProc );
	m_hWndSub[eMenu_Item]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_ITEM_DIALOG), m_hWndDlg, GMSubItemDlgProc );
	m_hWndSub[eMenu_Hide]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_HIDE_DIALOG), m_hWndDlg, GMSubHideDlgProc );

	m_hWndSub[eMenu_Chat]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_CHAT_DIALOG), m_hWndDlg, GMSubChatDlgProc );
	m_hWndSub[eMenu_PK]		= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_PK_DIALOG), m_hWndDlg, GMSubPKDlgProc );
	m_hWndSub[eMenu_Discon] = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_DISCON_DIALOG), m_hWndDlg, GMSubDisconDlgProc );
	m_hWndSub[eMenu_Block]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_BLOCK_DIALOG), m_hWndDlg, GMSubBlockDlgProc );
	
	m_hWndSub[eMenu_Mob]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_REGEN_DIALOG), m_hWndDlg, GMSubRegenDlgProc );
	m_hWndSub[eMenu_Counsel]= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_COUNSEL_DIALOG), m_hWndDlg, GMSubCounselDlgProc );
	m_hWndSub[eMenu_Notice]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_NOTICE_DIALOG), m_hWndDlg, GMSubNoticeDlgProc );

	m_hWndSub[eMenu_Event]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENT_DIALOG), m_hWndDlg, GMSubEventDlgProc );
	m_hWndSub[eMenu_EventMap]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENTMAP_DIALOG), m_hWndDlg, GMSubEventMapDlgProc );
	m_hWndSub[eMenu_EventNotify]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENTNOTIFY_DIALOG), m_hWndDlg, GMSubEventNotifyDlgProc );
	
	return TRUE;
}

BOOL CGMToolManager::DestroyGMDialog()
{
	if( !m_hWndDlg ) return FALSE;

	SaveChatList();

//	DestroyWindow( m_hWndDlg );
	m_hWndDlg = NULL;
	m_hWndLoginDlg = NULL;

	DestroyGMSubDialog();

	m_nSelectMenu = eMenu_Count;

	return TRUE;
}

BOOL CGMToolManager::DestroyGMSubDialog()
{
	for( int i = 0 ; i < eMenu_Count ; ++i )
	{
		if( m_hWndSub[i] )
		{
//			DestroyWindow( m_hWndSub[i] );
			m_hWndSub[i] = NULL;
		}
	}

	return TRUE;
}

void CGMToolManager::ShowGMDialog( BOOL bShow, BOOL bUpdate )
{
	if( !m_hWndDlg )		return;
	if( !m_bCanUse )		return;


//¿©±â¼­ºÎÅÍ ·Î±×ÀÎ(Login) Ã¼Å© ÇÏ´Â°÷
	if( bShow && !m_bLogin && MAINGAME->GetUserLevel() == eUSERLEVEL_GM )
	{
		ShowWindow( m_hWndLoginDlg, SW_SHOW );

		return;
	}
	else if( MAINGAME->GetUserLevel() > eUSERLEVEL_GM )
	{
		return;
	}
//¿©±â±îÁö


	if( bShow )
	{
		ShowWindow( m_hWndDlg, SW_SHOWNA );
		SetPositionByMainWindow();
	}
	else
	{
		GMTOOLMGR->OnClickMenu( eMenu_Count );
		ShowWindow( m_hWndDlg, SW_HIDE );		
	}

	if( bUpdate )
		m_bShow = bShow;

//ÀÓ½Ã·Î ¿©±âÀ§Ä¡
	static int bRegist = FALSE;
	int i;
	char bufNum[20];
	//comboboxµî·Ï
	if( !bRegist )
	{
//		if( m_hWndSub[eMenu_Item] )
		{
			ITEMMGR->SetItemIfoPositionHead();
			ITEM_INFO* pInfo = NULL;
			while( pInfo = ITEMMGR->GetItemInfoData() )
			{
				if( pInfo->Category == eItemCategory_Equip )
				{
					switch( pInfo->EquipType )
					{
					case eEquipType_Weapon:
						{
//							if( pInfo->Grade == 0 )
								SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					case eEquipType_Armor:			
						{
//							if( pInfo->Grade == 0 )
								SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					case eEquipType_Accessary:
					case eEquipType_Pet:
						{
							SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					}
				}
				else if( pInfo->Category == eItemCategory_Expend )
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
				}
				else
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
				}
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLBOOK, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILL, CB_SETCURSEL, 0, 0 );

//Grade
			for( i = 0 ; i <= 9 ; ++i )
			{
				wsprintf( bufNum, "+%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			for( i = 0 ; i <= 12 ; ++i )
			{
				wsprintf( bufNum, "%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_SETCURSEL, 0, 0 );
			SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_POTION, 1, TRUE );
			SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ETC, 1, TRUE );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_SPN_POTION, UDM_SETRANGE, 0, (LPARAM)MAKELONG(50, 1) );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_SPN_ETC, UDM_SETRANGE, 0, (LPARAM)MAKELONG(50, 1) );

			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
		}

//		if( m_hWndSub[eMenu_Mob] )
		{
//¸ó½ºÅÍ µî·Ï
			BASE_MONSTER_LIST* pList = NULL;
			for( i = 1 ; pList = GAMERESRCMNGR->GetMonsterListInfo( i ) ; ++i )
			{
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_ADDSTRING, 0, (LPARAM)pList->Name );
			}
			m_lBossStartIndex = i - 1;

		/*	//º¸½º : È²¸ª¼öÈ£Àå±º
			pList = GAMERESRCMNGR->GetMonsterListInfo( 1 );
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_ADDSTRING, 0, (LPARAM)pList->Name );
			///////
*/
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_SETCURSEL, 0, 0 );
//ITEM µî·Ï
			ITEMMGR->SetItemIfoPositionHead();
			ITEM_INFO* pInfo = NULL;
			while( pInfo = ITEMMGR->GetItemInfoData() )
			{
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, CB_SETCURSEL, 0, 0 );

//Map µî·Ï
			char* pStrMap = NULL;			
			for( i = 1 ; i < MAX_MAP_ENUMCOUNT ; ++i )
			{
				if( *( pStrMap = GetMapName(i) ) )
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Mob],		IDC_GMREGEN_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Move],		IDC_GMMOVE_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Discon],	IDC_GMDISCON_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Chat],		IDC_GMCHAT_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_PK],		IDC_GMPK_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Notice],	IDC_GMNOTICE_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
				}
			}

			SendDlgItemMessage( m_hWndSub[eMenu_Mob],		IDC_GMREGEN_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Move],		IDC_GMMOVE_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Discon],	IDC_GMDISCON_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat],		IDC_GMCHAT_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_PK],		IDC_GMPK_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Notice],	IDC_GMNOTICE_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			
			/*
			// Ã¤³ÎÀº ¸ÊÀ» ¼±ÅÃÇÑ ÈÄ¿¡ ÇØ´ç ¸Ê¿¡ ¼³Á¤µÈ Ã¤³ÎÀ» ÀÐ¾î¿É´Ï´Ù.
//Channel µî·Ï
			SendDlgItemMessage( m_hWndSub[eMenu_Mob],	IDC_GMREGEN_CMB_CHANNEL,	CB_ADDSTRING, 0, (LPARAM)"ALL" );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat],	IDC_GMCHAT_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"ALL" );
			SendDlgItemMessage( m_hWndSub[eMenu_PK],	IDC_GMPK_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"ALL" );
			SendDlgItemMessage( m_hWndSub[eMenu_Weather],	IDC_GMWEATHER_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"ALL" );
			SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"ALL" );

			for( i = 1 ; i <= m_cbChannelCount ; ++i )
			{
				wsprintf( bufNum, "%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Chat], IDC_GMCHAT_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				
				SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,	CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat], IDC_GMCHAT_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,	CB_SETCURSEL, gChannelNum, 0 );
			*/
		}


//event µî·Ï
/*
float gExpRate		= 1.0f;		// °æÇèÄ¡ ¾ò´Â ºñÀ²
float gItemRate		= 1.0f;		// ¾ÆÀÌÅÛ µå¶ø ºñÀ²
float gMoneyRate	= 1.0f;		// µ· ¾ò´Â ºñÀ²
float gDamageReciveRate	= 1.0f;		// ¹Þ´Â µ¥¹ÌÁö ºñÀ²
float gDamageRate	= 1.0f;		// ÁÖ´Â µ¥¹ÌÁö ºñÀ²
float gNaeRuykRate	= 1.0f;		// ³»·Â¼Ò¸ð ºñÀ²
float gUngiSpeed	= 1.0f;		// ¿î±âÁ¶½Ä ½ºÇÇµå
float gPartyExpRate	= 1.0f;		// ÆÄÆ¼°æÇèÄ¡ ¾ò´Â ºñÀ²
float gAbilRate		= 1.0f;		// Æ¯±âÄ¡ ¾ò´Â ºñÀ²
float gGetMoney		= 1.0f;		// ¾ò´Âµ·ÀÇ ¾ç

¡°¾­ÑéÖµ»ñµÃÂÊ¡±
¡°ÎïÆ·µôÂäÂÊ¡±
¡°½ðÇ®µôÂäÂÊ¡±
¡°ÊÜµ½µÄÉËº¦Öµ¡±
¡°Ôì³ÉµÄÉËº¦Öµ¡±
¡°ÄÚÁ¦ÏûºÄ¡±
¡°´ò×ø»Ø¸´ËÙ¶È¡±
¡°×é¶Ó¾­ÑéÖµ»ñµÃ¡±
¡°¼¼ÄÜµã»ñµÃ¡±
¡°½ðÇ®µôÂä¶îÊý¡±
*/
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"EXP rate" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Item drop rate" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Money drop rate" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"Not Used" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_SETCURSEL, 0, 0 );
		bRegist = TRUE;
	}
}

BOOL CGMToolManager::IsGMDialogMessage( LPMSG pMessage)
{
	if( IsWindow( m_hWndDlg ) && IsDialogMessage( m_hWndDlg, pMessage ) )
		return TRUE;

	if( m_nSelectMenu < eMenu_Count )
	if( m_hWndSub[m_nSelectMenu] )
	{
		if( IsWindow( m_hWndSub[m_nSelectMenu] ) && IsDialogMessage( m_hWndSub[m_nSelectMenu], pMessage ) )
			return TRUE;
	}

	return FALSE;
}

void CGMToolManager::SetPositionByMainWindow()
{
	RECT rcMain, rcDlg;
	GetWindowRect( _g_hWnd, &rcMain );
	GetWindowRect( m_hWndDlg, &rcDlg );
	int nDlgWidth = rcDlg.right - rcDlg.left;
	int X = rcMain.left - nDlgWidth;
	if( X < 0 ) X = rcMain.left;
	int Y = rcMain.top+GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION);
	
	SetWindowPos( m_hWndDlg, NULL, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetPositionSubDlg();
}

void CGMToolManager::SetPositionSubDlg()
{
	RECT rcDlg;
	GetWindowRect( m_hWndDlg, &rcDlg );

	if( m_nSelectMenu < eMenu_Count )
	{
		if( m_hWndSub[m_nSelectMenu] )
		{
			int subX = rcDlg.right;
			int subY = rcDlg.top + 36 + m_nSelectMenu * 28;

			SetWindowPos( m_hWndSub[m_nSelectMenu], NULL, subX, subY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		}
	}
}

void CGMToolManager::OnClickMenu( int nMenu )
{
	switch( nMenu )
	{
	case eMenu_Move:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Where:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Item:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		return;	break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		return;	break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			return;	break;
			}
		}
		break;
	case eMenu_Hide:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Chat:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_PK:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		return;	break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		return;	break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Discon:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Block:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			return;	break;
			}
		}
		break;
	case eMenu_Mob:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Counsel:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			return;	break;
			}
		}
		break;
	case eMenu_Notice:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Event:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		return;	break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_EventMap:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_EventNotify:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	case eMenu_Weather:
		{
			switch( m_nPower )
			{
			case eGM_POWER_MONITOR:		break;
			case eGM_POWER_PATROLLER:	return;	break;
			case eGM_POWER_AUDITOR:		break;
			case eGM_POWER_EVENTER:		break;
			case eGM_POWER_QA:			break;
			}
		}
		break;
	}

	if( nMenu >= eMenu_Count )
	{
		if( m_nSelectMenu < eMenu_Count )
		{
			if( m_hWndSub[m_nSelectMenu] )
				ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );

			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );			
		}

		m_nSelectMenu = eMenu_Count;
		CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_NONE, BST_CHECKED );

		m_bSubShow = FALSE;
		SetFocus( _g_hWnd );
	}
	else
	{
		if( m_nSelectMenu == nMenu )
		{
			if( m_hWndSub[m_nSelectMenu] )
				ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );

			m_nSelectMenu = eMenu_Count;
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_NONE, BST_CHECKED );

			m_bSubShow = FALSE;
			SetFocus( _g_hWnd );
		}
		else
		{
			ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );
			
			m_nSelectMenu = nMenu;
			SetPositionSubDlg();
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_CHECKED );

			if( m_hWndSub[m_nSelectMenu] )
				AnimateWindow( m_hWndSub[m_nSelectMenu], 200, AW_SLIDE | AW_HOR_POSITIVE );

			m_bSubShow = TRUE;
		}
	}	
}

BOOL CGMToolManager::OnGMMoveCommand( int nMethod )
{
	if( !HERO ) return FALSE;
	if( !m_hWndSub[eMenu_Move] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nMethod == 0 )
	{
		BOOL	bRt;
		VECTOR3 pos;
		pos.x = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_X, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;
		pos.y = 0;
		pos.z = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_Y, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;

		MOVE_POS msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVE_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwMoverID	= HEROID;
		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( nMethod == 1 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_NAME, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//ÀÚ±â ÀÚ½Å
			return FALSE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 0;		//Ä³¸¯ÅÍ·Î °¡±â
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));
	}
	else if( nMethod == 2)
	{
		char buf[MAX_MAP_NAME_LENGTH+1] = {0,};
		//int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_MAP, buf, MAX_NAME_LENGTH+1);
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_MAP, buf, MAX_MAP_NAME_LENGTH+1 );
		if( nLen > MAX_MAP_NAME_LENGTH ) return FALSE;

		WORD wMapNum = GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;

		//BOOL bRt;
		//int nChannel = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_CHANNEL, &bRt, TRUE );
		//int nChannel = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, &bRt, TRUE );
		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;

		//if( bRt )
		//if( m_cbChannelCount > 0 && nChannel > 0 && nChannel <= m_cbChannelCount )
		if(0<nChannel &&nChannel<=g_MapChannelNum[wMapNum])
		{
			nChannel = nChannel - 1; //0ºÎÅÍ ½ÃÀÛÇÏ´Â ÀÎµ¦½º¶ó -1
		}

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
//		msg.Category	= MP_USERCONN;
//		msg.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)wMapNum;
		msg.dwData2		= (DWORD)nChannel;
		msg.Name[0]		= 0;	//hero
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnUserMoveCommand( int nMethod )
{
	if( !m_hWndSub[eMenu_Move] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nMethod == 0 )
	{
		BOOL	bRt;
		VECTOR3 pos;
		pos.x = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_X, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;
		pos.y = 0;
		pos.z = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_Y, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;

		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4  ) return FALSE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return FALSE;

		MOVE_POS_USER msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVEUSER_SYN;	//
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( nMethod == 1 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;		

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )	//ÀÚ±â ÀÚ½Å
			return FALSE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return FALSE;
		
		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 1;		//µ¥·Á¿À±â
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));

	}
	else if( nMethod == 2 )
	{
		//username

		char buf[MAX_MAP_NAME_LENGTH+1] = {0,};
		int nLen;
		//nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_MAP, buf, MAX_NAME_LENGTH+1 );
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_MAP, buf, MAX_MAP_NAME_LENGTH+1 );
		if( nLen > MAX_MAP_NAME_LENGTH ) return FALSE;
		WORD wMapNum = GetMapNumForName( buf );
//		BOOL bRt;
		//int nChannel = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_CHANNEL, &bRt, TRUE );
		//int nChannel = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, &bRt, TRUE );
		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;
//		if( bRt )
		//if( m_cbChannelCount > 0 && nChannel > 0 && nChannel <= m_cbChannelCount )
		if(0<nChannel &&nChannel<=g_MapChannelNum[wMapNum])
		{
			nChannel = nChannel - 1;
		}
		
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)wMapNum;
		msg.dwData2		= (DWORD)nChannel;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnItemCommand()
{
	if( !m_hWndSub[eMenu_Item] ) return FALSE;
	SetFocus( _g_hWnd );

	int nResult = 0;
	WORD wCount = 1;
	char buf[64];
	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_WEAPON ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_GETCURSEL, 0, 0 );
		if( nSel > 0 && nSel <= 9 )
			wsprintf( buf, "%s+%d", buf, nSel );

		nResult = 1;	//1:Item
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_CLOTHES ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_GETCURSEL, 0, 0 );
		if( nSel > 0 && nSel <= 9 )
			wsprintf( buf, "%s+%d", buf, nSel );
		nResult = 1;	//1:Item
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_ACCESSORY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, buf, MAX_ITEMNAME_LENGTH+1 );
		nResult = 1;	//1:Item
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_SKILLBOOK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLBOOK, buf, MAX_ITEMNAME_LENGTH+1 );
		nResult = 1;	//1:Item
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_POTION ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, buf, MAX_ITEMNAME_LENGTH+1 );
		BOOL rt;
		int nCount = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_POTION, &rt, TRUE );
		if( rt )
		{
			if( nCount < 0 )			nCount = 0;
			else if( nCount > 99 )		nCount = 99;

			wCount = (WORD)nCount;
			nResult = 1;	//1:Item;
		}
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_ETC ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, buf, MAX_ITEMNAME_LENGTH+1 );
		BOOL rt;
		int nCount = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ETC, &rt, TRUE );
		if( rt )
		{
			if( nCount < 0 )			nCount = 0;
			else if( nCount > 50 )		nCount = 50;

			wCount = (WORD)nCount;
			nResult = 1;	//1:Item;
		}
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_MONEY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		DWORD dwMoney = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_MONEY, &rt, TRUE );
		if( !rt )	return FALSE;

		SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_MONEY, 0, TRUE );

		MSG_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MONEY_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= dwMoney;
		NETWORK->Send( &msg, sizeof(msg) );		
	}
/*	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_SKILL ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILL, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_GETCURSEL, 0, 0 );
		
		if( nSel < 0 )			nSel = 0;
		else if( nSel > 12 )	nSel = 12;

		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;

		cSkillBase* pBase = SKILLTREEMGR->GetSkill( pInfo->SkillNum );
		if( pBase == NULL )
		{
			MSG_WORD3 msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_ADDSKILL_SYN;
			msg.dwObjectID	= HEROID;
			msg.wData1		= pInfo->SkillNum;
			msg.wData2		= pInfo->ItemKind;
			msg.wData3		= nSel;
		
			NETWORK->Send(&msg,sizeof(msg));
			EnableWindow( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG ), TRUE );
		}
		else
		{
			if( pBase->GetLevel() != nSel )
			{
				MSG_WORD2 msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_SKILLLEVEL_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pInfo->SkillNum;
				msg.wData2 = nSel;
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
	}
*/	if( nResult == 1 )
	{
		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;

		MSG_DWORD_WORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_ITEM_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= pInfo->ItemIdx;
		msg.wData		= wCount;

		NETWORK->Send(&msg,sizeof(msg));
	}

	return TRUE;
}

BOOL CGMToolManager::OnDisconCommand()
{
	if( !m_hWndSub[eMenu_Block] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = { 0, };

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Discon], IDC_GMDISCON_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Discon], IDC_GMDISCON_EDT_CHAR, buf, MAX_NAME_LENGTH+1 );
		if( nLen < 4 || nLen > MAX_NAME_LENGTH )
			return FALSE;

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )
			return FALSE;
		
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return FALSE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send( &msg,sizeof(msg) );
	}
	else	//mapÀüÃ¼
	{
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Discon], IDC_GMDISCON_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH )
			return FALSE;
		
		WORD wMapNum = GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;

		WORD wExceptSelf = 0;
		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Discon], IDC_GMDISCON_BTN_EXCEPTSELF ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
			wExceptSelf = 1;
		
		//channelµµ ±¸ºÐÇÒ ÇÊ¿ä°¡ ÀÖ´Â°¡... Agent°¡ Ã¤³ÎÀ» ¾Ë°í ÀÖ³ª?
		MSG_WORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1		= wMapNum;
		msg.wData2		= wExceptSelf;
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnBlockCommand()
{
	if( !HERO ) return FALSE;
	if( !m_hWndSub[eMenu_Block] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = { 0, };
	int nLen = GetDlgItemText( m_hWndSub[eMenu_Block], IDC_GMBLOCK_EDT_CHAR, buf, MAX_NAME_LENGTH+1 );
	if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )
		return FALSE;
	if( nLen < 4 || nLen > MAX_NAME_LENGTH ) return FALSE;
	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
		return FALSE;

	BOOL bBlock;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Block], IDC_GMBLOCK_BTN_BLOCK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		bBlock = TRUE;
	else
		bBlock = FALSE;

	MSG_NAME_DWORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData		= (DWORD)bBlock;
	SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
	NETWORK->Send(&msg,sizeof(msg));

	if( bBlock )
	{	//disconnect

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}

	return TRUE;
}

BOOL CGMToolManager::OnWhereCommand()
{
	if( !m_hWndSub[eMenu_Where] ) return FALSE;
	SetFocus( _g_hWnd );

	MSG_NAME msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
	msg.dwObjectID	= HEROID;

	int nLen = GetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_USER, msg.Name, MAX_NAME_LENGTH+1 );

	if( nLen < 4 || nLen > MAX_NAME_LENGTH ) return FALSE;

	msg.Name[MAX_NAME_LENGTH] = 0;
	
	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)msg.Name)) == TRUE )
	{
		return TRUE;
	}
	NETWORK->Send( &msg, sizeof(msg) );
	
//ÃÊ±âÈ­
	SetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_MAP, "" );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_CHANNEL, 0, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_X, 0, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_Y, 0, TRUE );
//
	return TRUE;
}

void CGMToolManager::DisplayWhereInfo( char* MapName, int nChannel, int nX, int nY )
{
	if( !m_hWndSub[eMenu_Where] ) return;
	SetFocus( _g_hWnd );

	SetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_MAP, MapName );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_CHANNEL, nChannel, FALSE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_X, nX, FALSE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_Y, nY, FALSE );
}

BOOL CGMToolManager::OnHideCommand()
{
	if( !m_hWndSub[eMenu_Hide] ) return FALSE;
	SetFocus( _g_hWnd );

//	char buf[MAX_NAME_LENGTH+1] = {0,};
	int  nLen	= 0;
	BOOL bHide = TRUE;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_HIDE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		bHide = TRUE;
	else
	{
		bHide = FALSE;
	}
	// 06. 05 HIDE NPC - ÀÌ¿µÁØ
	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_NPC ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		CObject* pObject = OBJECTMGR->GetSelectedObject();
		if( !pObject )	return FALSE;
		if( pObject->GetObjectKind() != eObjectKind_Npc )	return FALSE;

		MSG_WORD3 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_NPCHIDE_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1 = ((CNpc*)pObject)->GetNpcUniqueIdx();
		msg.wData2 = (WORD)SendDlgItemMessage( m_hWndSub[eMenu_Hide], IDC_GMHIDE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 );
		msg.wData3 = !bHide;

		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else
	{
		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_HIDE_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= (DWORD)bHide;

		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )	
		{
			nLen = GetDlgItemText( m_hWndSub[eMenu_Hide], IDC_GMHIDE_EDT_USER, msg.Name, MAX_NAME_LENGTH+1 );
			if( nLen < 4 || nLen > MAX_NAME_LENGTH )
				return FALSE;
			
			switch( m_nPower )
			{
			case eGM_POWER_MASTER: break;
			case eGM_POWER_MONITOR:
			case eGM_POWER_PATROLLER:
			case eGM_POWER_AUDITOR:
			case eGM_POWER_EVENTER:
			case eGM_POWER_QA:
				return FALSE;
			}		
		}
		else
		{
			msg.Name[0] = 0;	//hero
		}

		msg.Name[MAX_NAME_LENGTH] = 0;
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
}

BOOL CGMToolManager::OnChatCommand()
{
	if( !m_hWndSub[eMenu_Chat] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = {0,};
	int nLen	= 0;
	BOOL bAllow = TRUE;
	int nTime	= 0;
	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Move], IDC_GMCHAT_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMCHAT_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Move], IDC_GMCHAT_BTN_NOCHAT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		{
			bAllow = FALSE;
			BOOL bRt;
			nTime = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMCHAT_EDT_TIME, &bRt, TRUE );
			if( !bRt ) return FALSE;
		}
		else
		{
			bAllow = TRUE;
		}
		//msg

		//username;
		//time;
		//
	}
	else
	{

	}

	return TRUE;
}


BOOL CGMToolManager::OnPKCommand()
{
	if( !m_hWndSub[eMenu_PK] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = {0,};
	int nLen = GetDlgItemText( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
	if( nLen > MAX_NAME_LENGTH ) return FALSE;

	WORD wMapNum = GetMapNumForName( buf );
	if( wMapNum == 0 ) return FALSE;

	int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;

	MSG_WORD2 msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_PKALLOW_SYN;
	msg.dwObjectID	= HEROID;
	msg.wData1		= wMapNum;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_PK], IDC_GMPK_BTN_NOPK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		msg.wData2	= 0;		//not allow
	}
	else
	{
		msg.wData2	= 1;		//allow		
	}

	NETWORK->Send(&msg,sizeof(msg));	

	return TRUE;
}

BOOL CGMToolManager::OnRegenCommand()
{
	if( !m_hWndSub[eMenu_Mob] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_ITEMNAME_LENGTH+1] = {0,};

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_MODEDIRECT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
//mob
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_GETCURSEL, 0, 0 );
//		BYTE bBoss = 0;
		BASE_MONSTER_LIST* pList = NULL;
		if( nSel < m_lBossStartIndex )
		{
			pList = GAMERESRCMNGR->GetMonsterListInfo( nSel + 1 );
		}
		else
		{
			pList = GAMERESRCMNGR->GetMonsterListInfo( 1000 + nSel - m_lBossStartIndex );
//			bBoss = 1;
		}

		if( pList == NULL ) return FALSE;
//mob count
		BOOL rt;
		int nMobCount = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_MOBNUM, &rt, TRUE );
		if( !rt ) return FALSE;
		if( nMobCount <= 0 ) return FALSE;
		if( nMobCount > 100 ) return FALSE;	//ÀÓ½Ã·Î ÇÑ¹ø¿¡ 100°³ ±îÁö¸¸
//map
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MAP, buf, MAX_MAP_NAME_LENGTH+1 );
		if( nLen == 0 ) return FALSE;
		buf[MAX_MAP_NAME_LENGTH] = 0;
		WORD wMapNum = GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;
//channel
		//int nChannel = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_CHANNEL, &rt, TRUE );
		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_GETCURSEL, 0, 0 );
		//if( !rt ) return FALSE;
		if( nChannel < 0 ) return FALSE;
		//if( nChannel > m_cbChannelCount ) return FALSE;		
		if(nChannel > g_MapChannelNum[wMapNum]) return FALSE;		
//pos & radius
		int nX = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_X, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;
		int nY = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_Y, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;
		int nRad = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_RAD, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;

		if( nX < 0 ) nX = 0;	//ÃÖ´ë°ª Ã¼Å©Ãß°¡
		if( nY < 0 ) nY = 0;	
		if( nRad < 0 ) nRad = 0;
		else if( nRad > 10000 ) nRad = 10000;	//¹Ý°æ 100m·Î Á¦ÇÑ

		VECTOR3 vPos = { (float)nX, 0.0f, (float)nY };
//Item
		nLen = GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, buf, MAX_ITEMNAME_LENGTH+1 );
		if( nLen == 0 ) return FALSE;
		buf[MAX_ITEMNAME_LENGTH] = 0;
		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;
//Drop Ratio
		int nDropRatio = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_ITEM, &rt, TRUE );
		if( !rt ) return FALSE;
		if( nDropRatio < 0 )	return FALSE;
		if( nDropRatio > 100 )	return FALSE;

		MSG_EVENT_MONSTERREGEN msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
		msg.dwObjectID	= HEROID;
		msg.MonsterKind = pList->MonsterKind;
		msg.cbMobCount	= nMobCount;
		msg.wMap		= wMapNum;
		msg.cbChannel	= (BYTE)nChannel;
		msg.Pos			= vPos;
		msg.wRadius		= (WORD)nRad;
		msg.ItemID		= pInfo->ItemIdx;
		msg.dwDropRatio	= (DWORD)nDropRatio;
//		msg.bBoss		= bBoss;

		NETWORK->Send( &msg,sizeof(msg) );
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_MODEFILE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		char lpstrFile[MAX_PATH] = {0,};
		GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_FILE, lpstrFile, MAX_PATH );
	
		if( lpstrFile[0] == 0 )
			return FALSE;
		
		CMHFile fp;		
		if( !fp.Init( lpstrFile, "rt" ) )
			return FALSE;
		char buff[256]={0,};

		while(1)
		{
			fp.GetString(buff);
			if(fp.IsEOF())
				break;
			if(buff[0] == '@')
			{
				fp.GetLineX(buff, 256);
				continue;
			}
			if( strcmp( buff, "$REGEN" ) == 0 )
			{
				DWORD dwGroup = fp.GetDword();
				if( dwGroup == 0 )
				{
					fp.Release();
					return FALSE;
				}
				if((fp.GetString())[0] == '{')
				{
					MSG_EVENT_MONSTERREGEN msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
					msg.dwObjectID	= HEROID;
					msg.MonsterKind = GAMERESRCMNGR->GetMonsterIndexForName( fp.GetString() );
					msg.cbMobCount	= fp.GetByte();
					msg.wMap		= GetMapNumForName( fp.GetString() );
					msg.cbChannel	= fp.GetByte();
					msg.Pos.x		= (float)fp.GetWord() * 100.0f;
					msg.Pos.z		= (float)fp.GetWord() * 100.0f;
					msg.wRadius		= fp.GetWord()*100;
					ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( fp.GetString() );
					if( pInfo )
						msg.ItemID		= pInfo->ItemIdx;
					else
						msg.ItemID		= 0;
					msg.dwDropRatio	= fp.GetDword();

					//if( msg.MonsterKind == 0 || msg.wMap == 0 || msg.cbChannel > m_cbChannelCount
					if( msg.MonsterKind == 0 || msg.wMap == 0 || msg.cbChannel > g_MapChannelNum[msg.wMap]
						|| ( msg.ItemID == 0 && msg.dwDropRatio != 0 ) )
					{
						ASSERT(0);
					}
					else
					{
						NETWORK->Send( &msg,sizeof(msg) );
					}					
				}
				else
				{
					fp.Release();
					return FALSE;
				}

				if((fp.GetString())[0] != '}')
				{
					fp.Release();
					return FALSE;
				}
			}
		}
		fp.Release();		
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_DELETE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENT_MONSTER_DELETE;
		msg.dwObjectID	= HEROID;

		NETWORK->Send( &msg, sizeof(msg) );
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

#define MAX_PREFACE_LENGTH	30

BOOL CGMToolManager::OnNoticeCommand()
{
	if( !m_hWndSub[eMenu_Notice] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_CHAT_LENGTH+1] = {0,};
	char bufMap[MAX_NAME_LENGTH+1] = {0,};

	int nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_PREFACE ), buf, MAX_PREFACE_LENGTH+1 );
	int nLen2 = GetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ), buf+nLen, MAX_CHAT_LENGTH-MAX_PREFACE_LENGTH );
	
	if( nLen2 == 0 ) return FALSE;
//	if( strlen( buf ) == 0 ) return FALSE;

	MSG_CHAT_WORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_NOTICE_SYN;
	msg.dwObjectID	= HEROID;
	SafeStrCpy( msg.Msg, buf, MAX_CHAT_LENGTH+1 );

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_BTN_ALLMAP ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		msg.wData = 0;
	}
	else
	{
		GetDlgItemText( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_CMB_MAP, bufMap, MAX_NAME_LENGTH+1 );
		msg.wData = GetMapNumForName(bufMap);
	}

	NETWORK->Send( &msg, msg.GetMsgLength() );

	SetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ), "" );
	SetFocus( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ) );

	return TRUE;
}

BOOL CGMToolManager::OnEventCommand()
{
	if( !m_hWndSub[eMenu_Notice] ) return FALSE;
	SetFocus( _g_hWnd );


	//Á¤º¸ ÀÐ¾î ¿À±â
	int nEvent = SendDlgItemMessage( m_hWndSub[eMenu_Event], IDC_GMEVENT_CMB_EVENT, CB_GETCURSEL, 0, 0 )+1;
	BOOL rt;
	int nRatio = GetDlgItemInt( m_hWndSub[eMenu_Event], IDC_GMEVENT_EDT_RATE, &rt, TRUE );

	// 071210 LYW --- GMToolManager : ÀÌº¥Æ® °æÇèÄ¡¸¸ ±â´ÉÀ» »ì¸°´Ù.
	// °æÇèÄ¡ ÃÖ°í 30¹è·Î Á¦ÇÑÀ» µÐ´Ù.
	if( nRatio > 3000 ) 
	{
		//MessageBox( NULL, "ÀÌº¥Æ® °æÇèÄ¡´Â ÃÖ´ë 30¹è ±îÁö¸¸ Àû¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù.", "GM-Tool", MB_OK) ;
		MessageBox( NULL, CHATMGR->GetChatMsg(1476), "GM-Tool", MB_OK) ;
		return FALSE ;
	}

	MSG_WORD2 msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_EVENT_SYN;
	msg.dwObjectID	= HEROID;
	msg.wData1		= nEvent;
	msg.wData2		= nRatio;
	NETWORK->Send( &msg, sizeof(msg) );
	CHATMGR->AddMsg( CTC_TOGM, "Event Applied!" );

	return TRUE;
}


BOOL CGMToolManager::OnEventMapCommand( int nKind )
{
	if( !m_hWndSub[eMenu_EventMap] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nKind == 1 )
	{
		//Á¤º¸ ÀÐ¾î ¿À±â
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHAR ), buf, MAX_NAME_LENGTH+1 );

		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Input User Name!" );
			return FALSE;
		}
		BOOL rt;
		int nChannel	= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHANNEL, &rt, TRUE );
		if( nChannel <= 0 || nChannel > 10 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Invalid Channel Number!" );
			return FALSE;
		}
		int nTeam		= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_TEAM, &rt, TRUE );
		if( nTeam != 1 && nTeam != 2 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Invalid Team Number!" );
			return FALSE;
		}
		
		MSG_NAME_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHANGE_EVENTMAP_SYN;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
		msg.dwData1 = 58;
		msg.dwData2 = nChannel;
		msg.dwData3 = nTeam;

		NETWORK->Send( &msg, sizeof(msg) );
		
		return TRUE;		
	}
	else if( nKind == 2 )
	{
		BOOL rt;
		int nChannel	= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHANNEL2, &rt, TRUE );
		if( nChannel <= 0 || nChannel > 10 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Invalid Channel Number!" );
			return FALSE;
		}

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_READY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 58;
		msg.dwData2 = nChannel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;		
	}

	return FALSE;
}

BOOL CGMToolManager::OnEventNotifyCommand( BOOL bOn )
{
	if( !m_hWndSub[eMenu_EventNotify] ) return FALSE;
	SetFocus( _g_hWnd );

	if( bOn )
	{
		//Á¤º¸ ÀÐ¾î ¿À±â
		char bufTitle[32] = {0,};
		char bufContext[128] = {0,};
		int nLen = 0;

		nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventNotify], IDC_GMEVENTNOTIFY_EDT_TITLE ), bufTitle, 32 );
		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Input Notify Title!" );
			return FALSE;
		}
		nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventNotify], IDC_GMEVENTNOTIFY_EDT_CONTEXT ), bufContext, 128 );
		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "Input Notify Context!" );
			return FALSE;
		}

		if( !m_hWndSub[eMenu_Notice] ) return FALSE;
		SetFocus( _g_hWnd );
		//Á¤º¸ ÀÐ¾î ¿À±â
		int nEvent = SendDlgItemMessage( m_hWndSub[eMenu_Event], IDC_GMEVENT_CMB_EVENT, CB_GETCURSEL, 0, 0 )+1;
	
		bufTitle[31] = 0;
		bufContext[127] = 0;

		MSG_EVENTNOTIFY_ON msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFY_ON;
		msg.dwObjectID = HEROID;
		memset( msg.EventList, 0, sizeof(BYTE)*eEvent_Max );
		msg.EventList[nEvent] = 1;
		SafeStrCpy( msg.strTitle, bufTitle, 32 );
		SafeStrCpy( msg.strContext, bufContext, 128 );

		NETWORK->Send( &msg, sizeof(msg) );
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFY_OFF;
		msg.dwObjectID = HEROID;

		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

//---KES GMÅø ·Î±×¾Æ¿ô Ã³¸® Ãß°¡
void CGMToolManager::LogOut()
{
	m_bLogin = FALSE;
	m_nPower = eGM_POWER_MAX;
}
//-------------------------

void CGMToolManager::Login( BOOL bLogin, int nGMPower )
{
	static char power_str[6][16] = { "Master", "Monitor", "Patroller", "Auditor", "Eventer", "QA" };

	char buf[128];
	if( bLogin )
	{
		wsprintf( buf, "< GM Power : %s >", power_str[nGMPower] );
		MessageBox( m_hWndLoginDlg, buf, "Login OK!", MB_OK );
		m_bLogin = TRUE;
		m_nPower = nGMPower;
		ShowWindow( m_hWndLoginDlg, SW_HIDE );
	}
	else
	{
		if( nGMPower == 1)		//nGMPower = error code : if bLogin == FALSE
		{
			MessageBox( m_hWndLoginDlg, "Invalid ID/PW", "Login Failed!", MB_OK );
		}
		else
		{
			MessageBox( m_hWndLoginDlg, "Error!", "Login Failed!", MB_OK );
		}		
	}
}

void SetChannelFromMap(HWND hWnd, int nMapCBoxID, int nChannelCBoxID)
{
	char buf[MAX_MAP_NAME_LENGTH+1] = {0,};
	int nSel = SendDlgItemMessage( hWnd, nMapCBoxID, CB_GETCURSEL, 0, 0 );
	SendDlgItemMessage( hWnd, nMapCBoxID, CB_GETLBTEXT, nSel, (LPARAM)buf );

	WORD wMapNum = GetMapNumForName( buf );
	if( wMapNum == 0 ) return;

	WORD wChannel = g_MapChannelNum[wMapNum];

	char bufNum[16] = {0,};
	SendDlgItemMessage( hWnd, nChannelCBoxID, CB_RESETCONTENT, 0, 0 );


	switch(nMapCBoxID)
	{
	case IDC_GMREGEN_CMB_MAP:
	//case IDC_GMCHAT_CMB_MAP:
	//case IDC_GMPK_CMB_MAP:
	//case IDC_GMWEATHER_CMB_MAP:
	//case IDC_GMHIDE_CMB_MAP:
		SendDlgItemMessage( hWnd, nChannelCBoxID, CB_ADDSTRING, 0, (LPARAM)"ALL" );
		break;

	default:
		break;
	}
	
	int i;
	for( i = 1 ; i<=wChannel ; i++ )
	{
		wsprintf( bufNum, "%d", i );
		SendDlgItemMessage( hWnd, nChannelCBoxID, CB_ADDSTRING, 0, (LPARAM)bufNum );
	}

	switch(nMapCBoxID)
	{
	case IDC_GMREGEN_CMB_MAP:
	//case IDC_GMCHAT_CMB_MAP:
	//case IDC_GMPK_CMB_MAP:
	//case IDC_GMWEATHER_CMB_MAP:
	//case IDC_GMHIDE_CMB_MAP:
		SendDlgItemMessage( hWnd, nChannelCBoxID, CB_SETCURSEL, gChannelNum-1, 0 );
		break;

	default:
		SendDlgItemMessage( hWnd, nChannelCBoxID, CB_SETCURSEL, gChannelNum, 0 );
		break;
	}
}

INT_PTR CALLBACK GMLoginDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			RECT rcGame;
			RECT rcDlg;
			POINT pt;
			GetClientRect( hWnd, &rcDlg );
			GetClientRect( _g_hWnd, &rcGame );
			pt.x = rcGame.left;
			pt.y = rcGame.top;
			ClientToScreen( _g_hWnd, &pt );

			// 070202 LYW --- GMLoginDlgProc : Modified limited area.
			/*
			int x = ( ( 1024 - ( rcDlg.right - rcDlg.left ) ) / 2 ) + pt.x;
			int y = ( ( 768 - ( rcDlg.bottom - rcDlg.top ) ) / 2 ) + pt.y;
			*/
			DISPLAY_INFO	dispInfo ;
			GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
			int x = ( ( dispInfo.dwWidth - ( rcDlg.right - rcDlg.left ) ) / 2 ) + pt.x;
			int y = ( ( dispInfo.dwHeight - ( rcDlg.bottom - rcDlg.top ) ) / 2 ) + pt.y;
			// 070202 LYW --- End.

			SetWindowPos( hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

			SendMessage( GetDlgItem( hWnd, IDC_GMLOGIN_EDT_ID ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMLOGIN_EDT_PW ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
		}
		return TRUE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				if( LOWORD( wParam ) == IDC_GMLOGIN_BTN_CONNECT )
				{
					char strID[MAX_NAME_LENGTH+1];
					char strPW[MAX_NAME_LENGTH+1];
					GetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), strID, MAX_NAME_LENGTH+1 );
					GetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), strPW, MAX_NAME_LENGTH+1 );
					
					if( strID[0] == 0 || strPW[0] == 0 )
					{
						MessageBox( hWnd, "Input ID/PW", "Error", MB_OK );
						return TRUE;
					}
				
					MSG_NAME2 msg;
					msg.Category = MP_CHEAT;
					msg.Protocol = MP_CHEAT_GM_LOGIN_SYN;
					msg.dwObjectID = HEROID;
					SafeStrCpy( msg.str1, strID, MAX_NAME_LENGTH+1 );
					SafeStrCpy( msg.str2, strPW, MAX_NAME_LENGTH+1 );

					NETWORK->Send( &msg, sizeof(msg) );

					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), "" );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), "" );
				}
				else if( LOWORD( wParam ) == IDC_GMLOGIN_BTN_CANCEL )
				{
					ShowWindow( hWnd, SW_HIDE );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), "" );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), "" );
				}

			}
		}
		return TRUE;
	}
    return FALSE;
}


INT_PTR CALLBACK GMDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GM_MNBTN_NONE, BST_CHECKED );

//#ifdef TAIWAN_LOCAL	//Áß±¹Àº ÀÌº¥Æ® È°¼ºÈ­ ¾ÈµÊ
//			EnableWindow( GetDlgItem( hWnd, IDC_GM_MNBTN_EVENT ), FALSE );
//#endif
		}
		return FALSE;	//not active

	case WM_NCHITTEST:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			SetFocus( _g_hWnd );
		}
		break;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
/*
				if( LOWORD( wParam ) == IDC_GM_MNBTN_EVENT1_ON )
				{
					MSG_WORD2 msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_EVENT_SYN;
					msg.dwObjectID	= HEROID;
					msg.wData1		= 1;	//event1
					msg.wData2		= 200;	//on
					NETWORK->Send( &msg, sizeof(msg) );
					CHATMGR->AddMsg( CTC_SYSMSG, "Event On!" );
				}
				else if( LOWORD( wParam ) == IDC_GM_MNBTN_EVENT1_OFF )
				{
					MSG_WORD2 msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_EVENT_SYN;
					msg.dwObjectID	= HEROID;
					msg.wData1		= 1;	//event1
					msg.wData2		= 100;	//off
					NETWORK->Send( &msg, sizeof(msg) );
					CHATMGR->AddMsg( CTC_SYSMSG, "Event Off!" );
				}
				else
*/
				{
					GMTOOLMGR->OnClickMenu( LOWORD( wParam ) - IDC_GM_MNBTN_MOVE );
				}
			}
		}
		return TRUE;

	case WM_MOVE:
		{
			GMTOOLMGR->SetPositionSubDlg();
		}
		break;

	case WM_CLOSE:
		{
//			GMTOOLMGR->DestroyGMDialog();
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );
		}
		return TRUE;
	}


    return FALSE;
}

INT_PTR CALLBACK GMSubMoveDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMMOVE_BTN_ME, IDC_GMMOVE_BTN_USER, IDC_GMMOVE_BTN_ME );
			CheckRadioButton( hWnd, IDC_GMMOVE_BTN_XY, IDC_GMMOVE_BTN_MAP, IDC_GMMOVE_BTN_XY );			
			//SetDlgItemInt( hWnd, IDC_GMMOVE_EDT_X, 0, TRUE );
			//SetDlgItemInt( hWnd, IDC_GMMOVE_EDT_Y, 0, TRUE );

			SetChannelFromMap(hWnd, IDC_GMMOVE_CMB_MAP, IDC_GMMOVE_CMB_CHANNEL);
		}
		return FALSE;	//not active

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMMOVE_OK:	//Do Move
					{
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_XY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{							
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 0 );
							else
								GMTOOLMGR->OnUserMoveCommand( 0 );
						}
						else if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 1 );
							else
								GMTOOLMGR->OnUserMoveCommand( 1 );
						}
						else if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_MAP ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 2 );
							else
								GMTOOLMGR->OnUserMoveCommand( 2 );
						}
					}
					break;

				case IDC_GMMOVE_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMMOVE_BTN_ME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ), FALSE );

//						SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, "" );
//						if( HERO )
//							SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, HERO->GetObjectName() );

						//IDC_GMMOVE_BTN_NAMEÀÌ Ã¼Å©µÇ¾î ÀÖ°í
						//IDC_GMMOVE_EDT_NAME°¡ disableÀÌ¸é enable·Î
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );
						}

						SetWindowText( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), "Character" );
					}
					break;

				case IDC_GMMOVE_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ), TRUE );
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, pObject->GetObjectName() );
						}

						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ) );

						//IDC_GMMOVE_BTN_NAMEÀÌ Ã¼Å©µÇ¾î ÀÖ°í
						//IDC_GMMOVE_EDT_NAME°¡ enableÀÌ¸é disable·Î
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						}

						SetWindowText( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), "To GM" );

						//character±Û¾¾¸¦ To GM À¸·Î?
					}
					break;

				case IDC_GMMOVE_BTN_XY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), FALSE );

						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ) );
					}
					break;

				case IDC_GMMOVE_BTN_NAME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), FALSE );
						
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );

							CObject* pObject = OBJECTMGR->GetSelectedObject();
							if( pObject )
							if( pObject->GetObjectKind() == eObjectKind_Player )
							{
								SetDlgItemText( hWnd, IDC_GMMOVE_EDT_NAME, pObject->GetObjectName() );
							}

							SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ) );
						}
					}
					break;

				case IDC_GMMOVE_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), TRUE );

						//SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ) );
						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ) );

						SetChannelFromMap(hWnd, IDC_GMMOVE_CMB_MAP, IDC_GMMOVE_CMB_CHANNEL);
					}
					break;
				}
			}
			else if( HIWORD( wParam ) == CBN_SELENDOK )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMMOVE_CMB_MAP:
					{
						SetChannelFromMap(hWnd, IDC_GMMOVE_CMB_MAP, IDC_GMMOVE_CMB_CHANNEL);
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}

INT_PTR CALLBACK GMSubChatDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_USER, IDC_GMCHAT_BTN_MAP, IDC_GMCHAT_BTN_USER );
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_NOCHAT, IDC_GMCHAT_BTN_ALLOW, IDC_GMCHAT_BTN_NOCHAT );
///			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_AUTO, IDC_GMCHAT_BTN_MANUAL, IDC_GMCHAT_BTN_AUTO );
		}
		return FALSE;	//not acitve

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMCHAT_BTN_APPLY:
					{
						GMTOOLMGR->OnChatCommand();
					}
					break;

				case IDC_GMCHAT_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMCHAT_BTN_LOOKUP:
					{

					}
					break;

				case IDC_GMCHAT_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_LOOKUP ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_CHANNEL ),	FALSE );

///						if( SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_NOCHAT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
///						{
///							if( SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ), BM_GETCHECK, 0, 0 ) != BST_CHECKED 
///								&& SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ), BM_GETCHECK, 0, 0 ) != BST_CHECKED )
///							{
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),		TRUE );
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	TRUE );
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		TRUE );
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		TRUE );
///
///								SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),	BM_SETCHECK, 1, 0 );
///
///							}
///						}

						//¼±ÅÃµÈ À¯Àú¸¦ ¾ò¾î¿ÀÀÚ.
						SetFocus( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ) );
					}
					break;

				case IDC_GMCHAT_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_LOOKUP ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_MAP ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_CHANNEL ),	TRUE );

///						if( SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_NOCHAT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
///						{
///							SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),	BM_SETCHECK, 0, 0 );
///							SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	BM_SETCHECK, 0, 0 );
///
///							EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),		FALSE );
///							EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	FALSE );							
///							EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		FALSE );
///							EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		FALSE );
///						}
					}
					break;

				case IDC_GMCHAT_BTN_NOCHAT:
					{
///						if( SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ), BM_GETCHECK, 0, 0 ) != BST_CHECKED 
///							&& SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ), BM_GETCHECK, 0, 0 ) != BST_CHECKED )
///						{
///							if( SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
///							{
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),		TRUE );
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	TRUE );							
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		TRUE );
///								EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		TRUE );
///								
///								SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),	BM_SETCHECK, 1, 0 );
///							}
///						}

						//ÇöÀç½Ã°£ ¾ò¾î¿À±â
					}
					break;

				case IDC_GMCHAT_BTN_ALLOW:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),		FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		FALSE );
///
///						SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_AUTO ),	BM_SETCHECK, 0, 0 );
///						SendMessage( GetDlgItem( hWnd, IDC_GMCHAT_BTN_MANUAL ),	BM_SETCHECK, 0, 0 );
					}
					break;

				case IDC_GMCHAT_BTN_AUTO:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		TRUE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		TRUE );
					}
					break;

				case IDC_GMCHAT_BTN_MANUAL:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_DATE_DATE ),		FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_TIME_TIME ),		FALSE );
					}
					break;
				}
			}
		}
		return TRUE;
	
	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK GMSubBlockDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMBLOCK_BTN_USER, IDC_GMBLOCK_BTN_USER, IDC_GMBLOCK_BTN_USER );
			CheckRadioButton( hWnd, IDC_GMBLOCK_BTN_BLOCK, IDC_GMBLOCK_BTN_UNBLOCK, IDC_GMBLOCK_BTN_BLOCK );
///			CheckRadioButton( hWnd, IDC_GMBLOCK_BTN_AUTO, IDC_GMBLOCK_BTN_MANUAL, IDC_GMBLOCK_BTN_AUTO );

		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMBLOCK_BTN_APPLY:
					{
						GMTOOLMGR->OnBlockCommand();
					}
					break;
					
				case IDC_GMBLOCK_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMBLOCK_BTN_USER:
					{
						SetFocus( GetDlgItem( hWnd, IDC_GMBLOCK_EDT_CHAR ) );

						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMBLOCK_EDT_CHAR, pObject->GetObjectName() );
						}
					}
					break;
				case IDC_GMBLOCK_BTN_BLOCK:
					{
///						if( SendMessage( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_AUTO ), BM_GETCHECK, 0, 0 ) != BST_CHECKED 
///							&& SendMessage( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_MANUAL ), BM_GETCHECK, 0, 0 ) != BST_CHECKED )
///						{
///							EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_AUTO ),		TRUE );
///							EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_MANUAL ),	TRUE );
///							EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_DATE_DATE ),	TRUE );
///							EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_TIME_TIME ),	TRUE );
///
///							SendMessage( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_AUTO ),	BM_SETCHECK, 1, 0 );
//						}

					}
					break;
				case IDC_GMBLOCK_BTN_UNBLOCK:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_AUTO ),		FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_MANUAL ),	FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_DATE_DATE ),	FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_TIME_TIME ),	FALSE );
///
///						SendMessage( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_AUTO ),		BM_SETCHECK, 0, 0 );
///						SendMessage( GetDlgItem( hWnd, IDC_GMBLOCK_BTN_MANUAL ),	BM_SETCHECK, 0, 0 );
					}
					break;
				case IDC_GMBLOCK_BTN_AUTO:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_DATE_DATE ),		TRUE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_TIME_TIME ),		TRUE );
					}
					break;
				case IDC_GMBLOCK_BTN_MANUAL:
					{
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_DATE_DATE ),		FALSE );
///						EnableWindow( GetDlgItem( hWnd, IDC_GMBLOCK_TIME_TIME ),		FALSE );
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif

		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubHideDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMHIDE_BTN_ME, IDC_GMHIDE_BTN_NPC, IDC_GMHIDE_BTN_ME );
			CheckRadioButton( hWnd, IDC_GMHIDE_BTN_HIDE, IDC_GMHIDE_BTN_SHOW, IDC_GMHIDE_BTN_HIDE );

			//SetChannelFromMap(hWnd, IDC_GMHIDE_CMB_MA, IDC_GMHIDE_CMB_CHANNEL);
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMHIDE_BTN_APPLY:
					{
						GMTOOLMGR->OnHideCommand();
					}
					break;
				case IDC_GMHIDE_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMHIDE_BTN_ME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), FALSE );
						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_NPC, FALSE );
					}
					break;
				case IDC_GMHIDE_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), FALSE );
						SetFocus( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ) );

						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMHIDE_EDT_USER, pObject->GetObjectName() );
						}

						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_NPC, FALSE );
					}
					break;
				case IDC_GMHIDE_BTN_NPC:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), TRUE );

						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_ME, FALSE );
						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_USER, FALSE );
					}
					break;
				}
			}
		}
		return TRUE;


	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}
	return FALSE;
}


INT_PTR CALLBACK GMSubWhereDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_USER, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_MAP, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_XY, BST_CHECKED );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMWHERE_BTN_WHERE:
					{
						GMTOOLMGR->OnWhereCommand();
					}
					break;
				case IDC_GMWHERE_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMWHERE_BTN_MAP:
					{
					}
					break;
				case IDC_GMWHERE_BTN_XY:
					{
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}

INT_PTR CALLBACK GMSubItemDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			//comboboxµî·Ï
/*
			ITEMMGR->SetItemIfoPositionHead();
			ITEM_INFO* pInfo = NULL;
			while( pInfo = ITEMMGR->GetItemInfoData() )
			{
				SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_ITEM, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
			}
			SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_ITEM, CB_SETCURSEL, 0, 0 );
*/
			CheckRadioButton( hWnd, IDC_GMITEM_BTN_WEAPON, IDC_GMITEM_BTN_ABILITY, IDC_GMITEM_BTN_WEAPON );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMITEM_BTN_GET:
					{
						GMTOOLMGR->OnItemCommand();
					}
					break;
				case IDC_GMITEM_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMITEM_BTN_WEAPON:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_CLOTHES:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_ACCESSORY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_SKILLBOOK:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
					case IDC_GMITEM_BTN_POTION:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_ETC:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_MONEY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_SKILL:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						//
						char buf[64];
						GetDlgItemText( hWnd, IDC_GMITEM_CMB_SKILL, buf, MAX_ITEMNAME_LENGTH+1 );
		
						ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
						if( pInfo )
						{
/*							cSkillBase* pBase = SKILLTREEMGR->GetSkill( pInfo->SkillNum );
							if( pBase )
							{
								EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	TRUE );
								SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, pBase->GetLevel(), 0 );
							}
							else
							{
								EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	FALSE );
								SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
							}
*/						}
					}
					break;
				case IDC_GMITEM_BTN_ABILITY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		TRUE );
					}
					break;
				}
			}
			else if( HIWORD( wParam ) == CBN_SELCHANGE )
			{
				if( LOWORD( wParam ) == IDC_GMITEM_CMB_SKILL )
				{
					char buf[64];
					
					//GetDlgItemText( hWnd, IDC_GMITEM_CMB_SKILL, buf, MAX_ITEMNAME_LENGTH+1 );
					int nSel = SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILL, CB_GETCURSEL, 0, 0 );

					SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILL, CB_GETLBTEXT, nSel, (LPARAM)buf );
					
					ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
					if( pInfo )
					{
/*						cSkillBase* pBase = SKILLTREEMGR->GetSkill( pInfo->SkillNum );
						if( pBase )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	TRUE );
							SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, pBase->GetLevel(), 0 );
						}
						else
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	FALSE );
							SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
						}
*/					}
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubRegenDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMREGEN_BTN_MODEDIRECT, IDC_GMREGEN_BTN_DELETE, IDC_GMREGEN_BTN_MODEDIRECT );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_MOBNUM, 1, TRUE );
			//SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_CHANNEL, 0, TRUE );
			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_RAD, 10, TRUE );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_X, 0, TRUE );
			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_Y, 0, TRUE );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_ITEM, 0, TRUE );

			SetChannelFromMap(hWnd, IDC_GMREGEN_CMB_MAP, IDC_GMREGEN_CMB_CHANNEL);
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMREGEN_BTN_APPLY:
					{
						GMTOOLMGR->OnRegenCommand();
					}
					break;
				case IDC_GMREGEN_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMREGEN_BTN_MODEDIRECT:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		TRUE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		FALSE );
					}
					break;
				case IDC_GMREGEN_BTN_MODEFILE:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		FALSE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		TRUE );
					}
					break;
				case IDC_GMREGEN_BTN_DELETE:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		FALSE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		FALSE );
					}
					break;
				case IDC_GMREGEN_BTN_MYPOS:
					{
						if( HERO )
						{
							VECTOR3 vPos;
							HERO->GetPosition( &vPos );
							SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_X, (UINT)(vPos.x / 100.0f), TRUE );
							SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_Y, (UINT)(vPos.z / 100.0f), TRUE );
						}

						SetDlgItemText( hWnd, IDC_GMREGEN_CMB_MAP, GetMapName( MAP->GetMapNum() ) );

						//SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_CHANNEL, gChannelNum+1, TRUE );
						SendDlgItemMessage( hWnd, IDC_GMREGEN_CMB_CHANNEL, CB_SETCURSEL, gChannelNum+1, 0 );
					}
					break;

				case IDC_GMREGEN_BTN_OPEN:
					{
						//ÆÄÀÏ ´ÙÀÌ¾ó·Î±×
						char lpstrFile[MAX_PATH] = "";
						OPENFILENAME OFN;
						ZeroMemory( &OFN, sizeof( OPENFILENAME ) );
						OFN.lStructSize = sizeof( OPENFILENAME );
						OFN.hwndOwner	= hWnd;
						OFN.lpstrFilter	= "Text Files (.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						OFN.lpstrFile	= lpstrFile;
						OFN.nMaxFile	= MAX_PATH;						
						OFN.Flags		= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST; 
						
						if( GetOpenFileName( &OFN ) != 0 )
						{
							SetDlgItemText( hWnd, IDC_GMREGEN_EDT_FILE, OFN.lpstrFile );
						}
					}
					break;
				}
			}
			else if( HIWORD( wParam ) == CBN_SELENDOK )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMREGEN_CMB_MAP:
					{
						SetChannelFromMap(hWnd, IDC_GMREGEN_CMB_MAP, IDC_GMREGEN_CMB_CHANNEL);
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubDisconDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMDISCON_BTN_USER, IDC_GMDISCON_BTN_MAP, IDC_GMDISCON_BTN_USER );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMDISCON_BTN_APPLY:
					{
						GMTOOLMGR->OnDisconCommand();
					}
					break;
				case IDC_GMDISCON_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMDISCON_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_EDT_CHAR ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_CMB_MAP ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_BTN_EXCEPTSELF ),	FALSE );
					}
					break;
				case IDC_GMDISCON_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_EDT_CHAR ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_CMB_MAP ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_BTN_EXCEPTSELF ),	TRUE );
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubPKDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMPK_BTN_MAP, IDC_GMPK_BTN_MAP, IDC_GMPK_BTN_MAP );
			CheckRadioButton( hWnd, IDC_GMPK_BTN_NOPK, IDC_GMPK_BTN_ALLOW, IDC_GMPK_BTN_NOPK );

			SetChannelFromMap(hWnd, IDC_GMPK_CMB_MAP, IDC_GMPK_CMB_CHANNEL);
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMPK_BTN_APPLY:
					{
						GMTOOLMGR->OnPKCommand();
					}
					break;
				case IDC_GMPK_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
			else if( HIWORD( wParam ) == CBN_SELENDOK )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMPK_CMB_MAP:
					{
						SetChannelFromMap(hWnd, IDC_GMREGEN_CMB_MAP, IDC_GMPK_CMB_CHANNEL);
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK GMSubEventDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_USER, IDC_GMCHAT_BTN_MAP, IDC_GMCHAT_BTN_USER );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMEVENT_BTN_APPLY:
					{
						GMTOOLMGR->OnEventCommand();
					}
					break;
				case IDC_GMEVENT_BTN_AUTO_ON:
					{
						// 080118 LYW --- GMToolManager : ÇöÀç »ç¿ëÇÏÁö ¾Ê´Â Auto Event On ÀÇ ±â´ÉÀ» ¸·¾Æ ¹ö¸².
						MSGBASE msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_PLUSTIME_ON;
						msg.dwObjectID = HEROID;						
						NETWORK->Send( &msg, sizeof(msg) );
					}
					break;
				case IDC_GMEVENT_BTN_AUTO_OFF:
					{
						MSGBASE msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_PLUSTIME_ALLOFF;
						msg.dwObjectID = HEROID;						
						NETWORK->Send( &msg, sizeof(msg) );
					}
					break;
				case IDC_GMEVENT_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}

INT_PTR CALLBACK GMSubNoticeDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMNOTICE_BTN_ALLMAP, IDC_GMNOTICE_BTN_MAP, IDC_GMNOTICE_BTN_ALLMAP );
			//ÀÔ·Â¿¡µðÆ® Á¦ÇÑ±ÛÀÚ ¼³Á¤ //focus
			SendMessage( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_PREFACE ), EM_LIMITTEXT, MAX_PREFACE_LENGTH, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_NOTICE ), EM_LIMITTEXT, MAX_CHAT_LENGTH - MAX_PREFACE_LENGTH, 0 );
			SetFocus( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_PREFACE ) );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMNOTICE_OK:
					{
						GMTOOLMGR->OnNoticeCommand();
					}
					break;
				case IDC_GMNOTICE_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMNOTICE_BTN_ALLMAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMNOTICE_CMB_MAP ), FALSE );
					}
					break;
				case IDC_GMNOTICE_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMNOTICE_CMB_MAP ), TRUE );
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_CLOSE:
		{
			GMTOOLMGR->OnClickMenu( eMenu_Count );
			SetFocus( _g_hWnd );
		}
		return TRUE;

	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif
		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubEventMapDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_USER, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_CHNANNEL, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_TEAM, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_CHNANNEL2, BST_CHECKED );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTMAP_EDT_CHAR ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMEVENTMAP_BTN_SEND:
					{
						GMTOOLMGR->OnEventMapCommand(1);
					}
					break;
				case IDC_GMEVENTMAP_BTN_STARTEVENT:
					{
						GMTOOLMGR->OnEventMapCommand(2);
					}
					break;
				case IDC_GMEVENTMAP_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMEVENTMAP_BTN_USER:
					{
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMEVENTMAP_EDT_CHAR, pObject->GetObjectName() );
						}
					}
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubEventNotifyDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMEVENTNOTIFY_BTN_NOTIFY, BST_CHECKED );
//			CheckDlgButton( hWnd, IDC_GMEVENTNOTIFY_BTN_SOUND, BST_CHECKED );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTNOTIFY_EDT_TITLE ), EM_LIMITTEXT, 10, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTNOTIFY_EDT_CONTEXT ), EM_LIMITTEXT, 40, 0 );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMEVENTNOTIFY_BTN_ON:
					{
						GMTOOLMGR->OnEventNotifyCommand( TRUE );
					}
					break;
				case IDC_GMEVENTNOTIFY_BTN_OFF:
					{
						GMTOOLMGR->OnEventNotifyCommand( FALSE );
					}
					break;
				case IDC_GMEVENTNOTIFY_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}



WNDPROC OldEditProc;
WNDPROC OldChatProc;
HWND hWndComboID;

INT_PTR CALLBACK GMSubCounselDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			//ÀÔ·Â¿¡µðÆ® Á¦ÇÑ±ÛÀÚ ¼³Á¤ //focus
			SendMessage( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_INPUT ), EM_LIMITTEXT, 127, 0 );
			SetFocus( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_INPUT ) );

			OldEditProc = (WNDPROC)SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_INPUT ), 
												  GWL_WNDPROC, (LONG)GMCounselEditProc);

			OldChatProc = (WNDPROC)SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_CHAT ), 
												  GWL_WNDPROC, (LONG)GMCounselChatProc);

			hWndComboID = GetDlgItem( hWnd, IDC_GMCOUNSEL_CMB_ID );
			SendMessage( hWndComboID, CB_INSERTSTRING, 0, (LPARAM)"----------------" );

			GMTOOLMGR->SetChatListHwnd( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_CHAT ) );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMCOUNSEL_BTN_SAVE:
					{
						char FileName[MAX_PATH] = "";
						OPENFILENAME OFN;
						memset( &OFN, 0, sizeof(OPENFILENAME) );
						OFN.lStructSize = sizeof(OPENFILENAME);
						OFN.hwndOwner	= hWnd;					
						OFN.lpstrFilter = "Text Files(*.txt)\0*.txt\0";
						OFN.lpstrFile	= FileName;
						OFN.nMaxFile	= MAX_PATH;
						OFN.lpstrDefExt = "txt";
						OFN.Flags		= OFN_OVERWRITEPROMPT;
						if( GetSaveFileName(&OFN) != 0 )
						{
							GMTOOLMGR->CaptureChat( FileName );
						}
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_CLOSE:
		{

			GMTOOLMGR->OnClickMenu( eMenu_Count );
			SetFocus( _g_hWnd );
	
		}
		return TRUE;
	case WM_DESTROY:
		{
			SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_INPUT ), GWL_WNDPROC, (LONG)OldEditProc );
			SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_CHAT ), GWL_WNDPROC, (LONG)OldChatProc );
		}
		return TRUE;
	case WM_SETFOCUS:
		{
#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#endif

		}
		break;
	case WM_KILLFOCUS:
		{
#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK GMCounselEditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_KEYDOWN:
		{
			if( wParam == VK_RETURN )
			{
				char buf[128];
				GetWindowText( hWnd, buf, 127 );
				if( buf[0] == 0 ) return TRUE;
				
				char Name[MAX_NAME_LENGTH+2] = {0,};

				int nLen = GetWindowText( hWndComboID, Name, MAX_NAME_LENGTH+1 );
				if( nLen < 4 || nLen > MAX_NAME_LENGTH )
					return TRUE;
		
				// ¾ÆÀÌµðÈ®ÀÎ - RaMa 04.10.20
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)Name)) == TRUE )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(679));
					return TRUE;
				}
					
				//---±Ó¸» º¸³»±â
				MSG_CHAT data;
				data.Category	= MP_CHAT;
				data.Protocol	= MP_CHAT_WHISPER_SYN;
				data.dwObjectID	= HEROID;
				SafeStrCpy( data.Name, Name, MAX_NAME_LENGTH+1 );
				SafeStrCpy( data.Msg, buf, MAX_CHAT_LENGTH+1 );
				//		NETWORK->Send(&data,sizeof(data));
				NETWORK->Send(&data,data.GetMsgLength());	//CHATMSG 040324
				
				SetWindowText( hWnd, "" );

				return TRUE;
			}
			else if( wParam == VK_F8 )
			{
				GMTOOLMGR->RemoveIdFromList();
			}
		}
		break;
	}

	return CallWindowProc( OldEditProc, hWnd, message, wParam, lParam );
}


LRESULT CALLBACK GMCounselChatProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_CHAR:
	case WM_LBUTTONDOWN:
		return TRUE;
	}

	return CallWindowProc( OldChatProc, hWnd, message, wParam, lParam );
}

void CGMToolManager::RemoveIdFromList()
{
	char Name[MAX_NAME_LENGTH+2] = {0,};
	int nLen = GetWindowText( hWndComboID, Name, MAX_NAME_LENGTH+1 );
	if( nLen < 4 || nLen > MAX_NAME_LENGTH )
		return;
				
	int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)Name );
	if( nIndex != CB_ERR && nIndex != m_nNumNow )
	{
		SendMessage( hWndComboID, CB_DELETESTRING, nIndex, 0 );
		SendMessage( hWndComboID, CB_SETCURSEL, 0, 0 );
				
		if( nIndex > m_nNumNow )
			--m_nNumWait;
		else
			--m_nNumNow;
	}
}

void CGMToolManager::AddChatMsg( char* pName, char* pMsg, int nKind )
{
	if( !m_hWndChatList ) return;

	char buf[160];

	if( nKind == 1 )	//Çß´Ù.
	{
		wsprintf( buf, "*%-20s: %s", pName, pMsg );
		
		int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)pName );
		if( nIndex == CB_ERR )
		{
			SendMessage( hWndComboID, CB_INSERTSTRING, 0, (LPARAM)pName );
			++m_nNumNow;			
		}
		else if( nIndex != 0 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, nIndex, 0 );
			SendMessage( hWndComboID, CB_INSERTSTRING, 0, (LPARAM)pName );
			SendMessage( hWndComboID, CB_SETCURSEL, 0, 0 );
		}
		
		if( m_nNumNow > 10 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, 10, 0 );	//Á¦ÀÏ ¾Æ·¡°Í Áö¿ì±â
			--m_nNumNow;
		}
		
	}
	else if( nKind == 2 )	//¹Þ¾Ò´Ù.
	{
		wsprintf( buf, "FROM %-16s: %s", pName, pMsg );

		int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)pName );
		if( nIndex == CB_ERR )
		{
			SendMessage( hWndComboID, CB_INSERTSTRING, m_nNumNow, (LPARAM)pName );
			++m_nNumWait;
		}

		int nCount = SendMessage( hWndComboID, CB_GETCOUNT, 0, 0 );

		if( m_nNumWait > 10 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, nCount-1, 0 );	//Á¦ÀÏ ¾Æ·¡°Í Áö¿ì±â
			--m_nNumWait;
		}
	}
	else if( nKind == 3 )	//¿¡·¯
	{
		wsprintf( buf, "%22s - %s -", pName, pMsg );
	}

	int nLen = strlen( buf );
	buf[nLen]	= 13;
	buf[nLen+1] = 10;
	buf[nLen+2] = 0;

	SendMessage( m_hWndChatList, EM_REPLACESEL, FALSE, (LPARAM)buf );

	strcpy( m_strChatList+m_nBufLen, buf );
	m_nBufLen += (nLen+2);

	//³Ê¹« ¸¹ÀÌ Â÷¸é Áö¿ìÀÚ.
	if( m_nBufLen > 1024*10 )
	{
		char FileName[MAX_PATH];
		SYSTEMTIME ti;
		GetLocalTime( &ti );
		wsprintf( FileName, "GM_Counsel/%s%02d%02d%02d.txt", HERO->GetObjectName(), ti.wYear, ti.wMonth, ti.wDay );
		m_nBufLen -= 1024;

		DIRECTORYMGR->SetLoadMode(eLM_Root);
		CreateDirectory( "GM_Counsel", NULL );

		HANDLE	hFile;
		hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

		if( hFile == INVALID_HANDLE_VALUE )
			return;

		DWORD dwWritten;
		SetFilePointer( hFile, 0, NULL, FILE_END );
		WriteFile( hFile, m_strChatList, 1024, &dwWritten, NULL );
		CloseHandle( hFile );
		
		memmove( m_strChatList, m_strChatList+1024, m_nBufLen );
		m_strChatList[m_nBufLen] = 0;
		SetWindowText( m_hWndChatList, m_strChatList );
		SendMessage( m_hWndChatList, EM_SETSEL, m_nBufLen, m_nBufLen );
	}
}

void CGMToolManager::CaptureChat( char* FileName )
{
	HANDLE	hFile;
	hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
								
	if( hFile == INVALID_HANDLE_VALUE )
		return;
								
	DWORD dwWritten;
	WriteFile( hFile, m_strChatList, m_nBufLen, &dwWritten, NULL );
	CloseHandle( hFile );
}

void CGMToolManager::SaveChatList()
{
	if( m_strChatList[0] == 0 ) return;

	char FileName[MAX_PATH];
	SYSTEMTIME ti;
	GetLocalTime( &ti );
	
	CHero* hero = OBJECTMGR->GetHero();

	if( hero )
	{
		wsprintf( FileName, "GM_Counsel/%s%02d%02d%02d.txt", hero->GetObjectName(), ti.wYear, ti.wMonth, ti.wDay );
	}
	
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	CreateDirectory( "GM_Counsel", NULL );
	
	HANDLE	hFile;
	hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( hFile == INVALID_HANDLE_VALUE )
		return;
	
	DWORD dwWritten;
	SetFilePointer( hFile, 0, NULL, FILE_END );
	WriteFile( hFile, m_strChatList, m_nBufLen, &dwWritten, NULL );
	CloseHandle( hFile );

	m_strChatList[0] = 0;
	m_nBufLen = 0;
	
	SetWindowText( m_hWndChatList, "" );
/*
	HANDLE	hFile2;
	hFile2 = CreateFile( "debugNullMove.txt", GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	WriteFile( hFile, &m_nNullMove, 4, &dwWritten, NULL );
	CloseHandle( hFile2 );
*/
}

#endif //_GMTOOL_
