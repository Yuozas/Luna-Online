#include "stdafx.h"
#include "ItemManager.h"
#include "ItemShow.h"
#include "cQuickItem.h"
#include "DealDialog.h"
#include "ExchangeDialog.h"
#include "MixDialog.h"
#include "ProgressDialog.h"
#include "GameIn.h"
#include "WindowIDEnum.h"
#include "Interface/cWindowManager.h"
#include "Interface/cScriptManager.h"
#include "cResourceManager.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
//#include "DebugDlg.h"

#include "AppearanceManager.h"
//#include "GameDef.h"
#include "QuickManager.h"
#include "ExchangeItem.h"

#include "ChatManager.h"
//#include "./Audio/MHAudioManager.h"
#include "mhFile.h"

#include "GuildWarehouseDialog.h"
#include "InventoryExDialog.h"
#include "StorageDialog.h"
#include "GameEventManager.h"
#include "EnchantDialog.h"
#include "ReinforceDlg.h"

// 080218 LUJ
#include "ApplyOptionDialog.h"

#include "MHMap.h"
#include "DissolveDialog.h"
#include "QuickSlotDlg.h"
#include "cSkillTreeManager.h"
#include "../[cc]skill/client/info/SkillInfo.h"
#include "../[cc]skill/client/manager/Skillmanager.h"
#include "ItemShopDialog.h"
//#include "StatusIconDlg.h"
//#include "MoveDialog.h"
//#include "CharacterDialog.h"
//#include "cMsgBox.h"
//#include "ObjectStateManager.h"
//#include "MoveManager.h"
#include "EventMapInfo.h"

//#include "ChaseDialog.h"
//#include "NameChangeDialog.h"
//#include "ReviveDialog.h"

//#include "GMNotifyManager.h"
#include "SealDialog.h"
#include "SiegeWarMgr.h"
//#include "ChangeJobDialog.h"


// 070329 LYW --- ItemManager : Include NpcImageDlg.
#include "NpcImageDlg.h"
#include "NpcScriptDialog.h"
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.24
#include "../hseos/Farm/SHFarmManageDlg.h"
// E 농장시스템 추가 added by hseos 2007.10.24

// 071022 LYW --- ItemManager : Include header file of tutorial manager.
#include "TutorialManager.h"

// 071203 LYW --- ItemManager : Include map move dialog.
#include "cMapMoveDialog.h"

// 071204 LYW --- ItemManager : Include character dialog.
#include "../CharacterDialog.h"

// 071206 LYW --- InventoryExDialog : 
#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/info/ActiveSkillInfo.h"

#include "cMsgBox.h"

// 071231 LYW --- ItemManager : 이름 변경 창 헤더 포함.
#include "cChangeNameDialog.h"
#include "../FilteringTable.h"
#include "./interface/cEditBox.h"

#include "../ConductInfo.h"
#include "ConductManager.h"



#include "../hseos/Farm/FarmAnimalDlg.h"

#include "BuyItem.h"

#include "ShoutDialog.h"
#include "FishingDialog.h"
#include "FishingManager.h"

// 080320 LUJ, 인챈트 수치 함수를 위해 참조
#include "CommonCalcFunc.h"


#include "../hseos/Monstermeter/SHMonstermeterDlg.h"
#include "ItemPopupDlg.h"
#include "petweareddialog.h"
#include "petmanager.h"

GLOBALTON(CItemManager)
CItemManager::CItemManager()
{
	m_ItemHash.Initialize(128);
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
	m_UnfitItemInfoListForHide.Initialize(100);
	m_ItemToolTipTextList.Initialize(64);		//confirm
	//m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	//m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	// 071121 LYW - Modified.
	//m_UsedItemList.Initialize(32);
	//m_TimeItemList.Initialize(100) ;
	mEffectList.Initialize( 100 );
	mEventInfoTable.Initialize( 3 );

	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_ITEM_STARTINDEX);
	m_bAddPrice = FALSE;
	//m_RareItemInfoTable.Initialize(64);
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));

	LoadSetScript();
	LoadMixScript();
	LoadDissolveScript();
	LoadReinforceScript();
	LoadEnchantScript();
	LoadApplyOptionScript();
	LoadCoolTimeScript();
	LoadChangeItem();
	
	// 080414 LUJ, 외양 변경 스크립트 로딩
	LoadBodyChangeScript();

	// 071127 LYW --- ItemManager : 삭제 아이템 ( 봉인 관련 ) 위치 저장 변수 추가.
	m_nSealItemDelPos = -1 ;
}

CItemManager::~CItemManager()
{
	//m_GroundHash.R
	Release();
	m_IconIndexCreator.Release();

	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();
	m_UnfitItemInfoListForHide.RemoveAll();	

	TOOLTIP_TEXT* pToolTipText = NULL;
	TOOLTIP_TEXT* pTemp = NULL;

	m_ItemToolTipTextList.SetPositionHead();
	while( pToolTipText = m_ItemToolTipTextList.GetData() )
	{
		do
		{
			pTemp = pToolTipText->pNext;
			if( pToolTipText->strToolTip )
				delete[] pToolTipText->strToolTip;
			delete pToolTipText;
			pToolTipText = pTemp;

		} while( pToolTipText );
	}
	m_ItemToolTipTextList.RemoveAll();

	EnchantEffect * pEffect = NULL;
	mEffectList.SetPositionHead();
	while(pEffect = mEffectList.GetData())
		delete pEffect;
	mEffectList.RemoveAll();

	EVENTINFO* pEvent = NULL;
	mEventInfoTable.SetPositionHead();
	while( pEvent = mEventInfoTable.GetData() )
	{
		delete[] pEvent->Msg;
		delete pEvent;
	}
	mEventInfoTable.RemoveAll();

	/*SHOPITEMBASE* pShopItemInfo = NULL;
	m_UsedItemList.SetPositionHead();
	while(pShopItemInfo = m_UsedItemList.GetData())
		delete pShopItemInfo;
	m_UsedItemList.RemoveAll();*/

	//sRareItemInfo* pList = NULL;
	//m_RareItemInfoTable.SetPositionHead();
	//while( pList = m_RareItemInfoTable.GetData() )
	//{
	//	delete pList;
	//	pList = NULL;
	//}
	//m_RareItemInfoTable.RemoveAll();

	//for(	std::list< SetScript* >::iterator it = mSetScript.begin();
	//		mSetScript.end() != it;
	//		++it )
	//{
	//	SetScript* script = *it;

	//	SAFE_DELETE( script );
	//}

	m_GiftEventTime = 0;
}


cImage * CItemManager::GetIconImage(DWORD ItemIdx, cImage * pImageGet)
{
	//char buff[128];
	//wsprintf(buff, "Data/Interface/2DImage/item/%d.tif",GetItemInfo(ItemIdx)->Image2DNum);
	//cImage* pImage = WINDOWMGR->GetImageOfFile(buff, 40);
	//return pImage;
	SCRIPTMGR->GetImage( GetItemInfo(ItemIdx)->Image2DNum, pImageGet, PFT_ITEMPATH );	
	return pImageGet;
}

CItem* CItemManager::MakeNewItem( const ITEMBASE* pBaseInfo, const char* strKind)
{
	ITEM_INFO* pItemInfo = GetItemInfo(pBaseInfo->wIconIdx);
	
	if(pItemInfo == NULL)
	{
		char temp[256];
		sprintf(temp,"NoItemInfo %d,  %d,  %s",HEROID,pBaseInfo->wIconIdx,strKind);
		ASSERTMSG(pItemInfo, temp);
		return NULL;
	}
	
	if(pBaseInfo->dwDBIdx == 0)
		return NULL;

	CItem* pItem = NULL;
	if(!(pItem = GetItem(pBaseInfo->dwDBIdx)))
	{
		pItem = new CItem(pBaseInfo);
		m_ItemHash.Add(pItem,pItem->GetDBIdx());						// HASH¿¡ ¾ÆAIAU ³O±a
		cImage tmpIconImage;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE,
					GetIconImage(pItem->GetItemIdx(), &tmpIconImage),
					m_IconIndexCreator.GenerateIndex());
		pItem->SetIconType(eIconType_AllItem);
		pItem->SetData(pItem->GetItemIdx());
		pItem->SetLinkPosition( 0 );

		WINDOWMGR->AddWindow(pItem);
		RefreshItem( pItem );
	}
	else
	{
		char buf[128];
		sprintf(buf, "중복된 DB인덱스를 사용! DBIndex: %d", pBaseInfo->dwDBIdx);
		ASSERTMSG(0, buf);
	}

	return pItem;
}


void CItemManager::AddToolTip( cWindow* window, DWORD toolTipIndex, DWORD color )
{
	if( ! window )
	{
		return;
	}

	const TOOLTIP_TEXT* text = 0;

	for(
		text = m_ItemToolTipTextList.GetData( toolTipIndex );
		text;
		text = text->pNext )
	{
		if( *text->strToolTip )
		{
			window->AddToolTipLine( text->strToolTip, color );
		}
	}
}


void CItemManager::AddToolTip( CVirtualItem* icon )
{
	if( ! icon )
	{
		return;
	}

	CBaseItem* linkedItem = icon->GetLinkItem();

	if( ! linkedItem )
	{
		return;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		item.wIconIdx	= linkedItem->GetItemIdx();
		item.dwDBIdx	= linkedItem->GetDBIdx();
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );
}


void CItemManager::AddToolTip( cIcon* icon )
{
	if( ! icon )
	{
		return;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		item.wIconIdx	= icon->GetData();
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );
}


void CItemManager::AddToolTip( CItemShow* icon )
{
	if( ! icon )
	{
		return;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		item.wIconIdx	= icon->GetItemIdx();
		item.dwDBIdx	= icon->GetDBIdx();
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );
}

// 071215 LYW --- ItemManager : 퀵슬롯의 소셜아이콘 툴팀 처리 함수 추가.
void CItemManager::SetSocialIconTooltip(cQuickItem* pItem, SLOT_INFO* pSlotInfo)
{
	if( !pItem ) return ;
	if( !pSlotInfo ) return ;

	cConductInfo* pConductInfo = NULL ;
	pConductInfo = CONDUCTMGR->GetConductInfo( pSlotInfo->idx ) ;

	if( !pConductInfo ) return ;

	cImage image;
	SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &image, TTCLR_ITEM_CANEQUIP );

	pItem->AddToolTipLine( pConductInfo->GetConductName(), RGB_HALF( 255, 255, 255 ) );
}


void CItemManager::AddToolTip( CItem* icon )
{
	if( ! icon )
	{
		return;
	}

	const ITEMBASE& item = icon->GetItemBaseInfo();

	BOOL bFirst = TRUE ;

	if( eITEM_TYPE_UNSEAL == item.nSealed )
	{
		bFirst = FALSE ;
		icon->SetLastCheckTime( gCurTime );
	}
		
	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );

	// 071115 LYW --- ItemManager : 남은 사용시간 툴팁 추가.
	AddRemainTimeToolTip( *icon, item, bFirst );

	if( icon->IsNoDefenceTooltip() )
	{
		icon->AddToolTipLine( "" );
		icon->AddToolTipLine( CHATMGR->GetChatMsg( 1195 ), ICONCLR_DISABLE );
		icon->AddToolTipLine( CHATMGR->GetChatMsg( 1196 ), ICONCLR_DISABLE );
	}
}


void CItemManager::AddToolTip( cQuickItem* icon )
{
	if( !	icon  )
	{
		return;
	}

	const SLOT_INFO* pSlotInfo = icon->GetSlotInfo() ;

	if( !	pSlotInfo ||
			pSlotInfo->kind == QuickKind_Skill )
	{
		return ;
	}
	else if( pSlotInfo->kind == QuickKind_Conduct )
	{
		cConductInfo* pConductInfo = CONDUCTMGR->GetConductInfo( pSlotInfo->idx ) ;

		if( !pConductInfo ) return ;

		cImage image;
		SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

		icon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &image, TTCLR_ITEM_CANEQUIP );
		icon->AddToolTipLine( pConductInfo->GetConductName(), RGB_HALF( 255, 255, 255 ) );
		return ;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		item.wIconIdx	= icon->GetSlotInfo()->idx;

		if( icon->GetItem() )
		{
			item.dwDBIdx	= icon->GetItem()->GetDBIdx();
			item.nSealed	= icon->GetItem()->IsSeal();
		}		
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );
	AddUseableTimeToolTip( *icon, item );

	/*
	// TODO: 리팩토링
	if( icon->GetIconType() == eIconType_AllItem )
	{
		CItem* pItem = NULL ;

		if( icon->GetIconType() == eIconType_AllItem )
		{
			CBaseItem* pItemBaseInfo = icon->GetItem();

			if( pItemBaseInfo )
			{
				pItem = GetItem( pItemBaseInfo->GetDBIdx() ) ;
			}
			else
			{
				pItem = GetItem( pSlotInfo->dbIdx ) ;
			}

			if( !pItem ) return ;
		}

		const ITEMBASE& pItemBase = pItem->GetItemBaseInfo() ;

		if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
		{
			char tempBuf[512] = {0, } ;

			DWORD dwRemainTime = pItemBase.nRemainSecond ;

			int nDayValue = 86400 ;
			int nHourValue = 3600 ;

			int nDay = dwRemainTime/(nDayValue) ;

			int nHour = ( dwRemainTime - (nDay * nDayValue) ) / nHourValue ;

			int nMinuteValue = (dwRemainTime - (nDay * nDayValue)) - (nHour * nHourValue) ;

			int nMinute = nMinuteValue / 60;

			int nSecond = nMinuteValue % 60;

			if( nSecond > 0 )
			{
				++nMinute ;
			}

			icon->AddToolTipLine(CHATMGR->GetChatMsg( 1179 ), RGB_HALF( 255, 255, 0 )) ;

			if( nDay <= 0 )
			{
				if( nHour <= 0 )
				{
					if( nMinute <= 0 ) 
					{
						sprintf(tempBuf, CHATMGR->GetChatMsg( 1181 ), 1) ;
					}
					else
					{
						sprintf(tempBuf, CHATMGR->GetChatMsg( 1181 ), nMinute) ;
					}
				}
				else
				{
					sprintf(tempBuf, CHATMGR->GetChatMsg( 1182 ), nHour, nMinute) ;
				}
			}
			else
			{
				sprintf(tempBuf, CHATMGR->GetChatMsg( 1183 ), nDay, nHour, nMinute) ;
			}

			icon->AddToolTipLine(tempBuf, RGB_HALF( 255, 255, 0 )) ;
		}
		else
		{
			AddUseableTimeToolTip( *icon, item ) ;
		}
	}
	*/
}


void CItemManager::AddToolTip( CExchangeItem* icon )
{
	if( ! icon  )
	{
		return;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		if( icon->GetLinkItem() )
		{
			item = icon->GetLinkItem()->GetItemBaseInfo();
		}
		else
		{
			item.wIconIdx	= icon->GetData();
			item.dwDBIdx	= icon->GetDBId();
			item.nSealed	= icon->IsSeal();
		}		
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip(* icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );

	if( item.nSealed )
	{
		AddUseableTimeToolTip( *icon, item ) ;
	}
}


void CItemManager::AddTimeToolTip( cIcon* icon, const ITEM_OPTION& option, BOOL bFirst)
{
	if( ! icon )
	{
		ASSERT( 0 );
		return;
	}
	
	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		if( WT_ITEM == icon->GetType() )
		{
			item = ( ( CItem* )icon )->GetItemBaseInfo();
		}
		else
		{
			item.wIconIdx	= icon->GetData();
		}		
	}

	const ITEM_INFO* info = GetItemInfo( icon->GetData() );

	if( ! info )
	{
		return;
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip( *icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );

	// 071115 LYW --- ItemManager : 남은 시간 출력.
	if( icon->GetType() == WT_ITEM )
	{
		AddRemainTimeToolTip( *icon, item, bFirst) ;
	}	
}

void CItemManager::AddToolTip(CBuyItem* icon)
{
	if( ! icon  )
	{
		return;
	}

	ITEMBASE item;
	{
		ZeroMemory( &item, sizeof( item ) );

		/*if( icon->GetLinkItem() )
		{
			item = icon->GetLinkItem()->GetItemBaseInfo();
		}
		else*/
		{
			item.wIconIdx	= icon->GetItemIdx();
			item.nSealed	= icon->GetSeal();
		}		
	}

	AddNametoolTip( *icon, item );
	AddPetToolTip( *icon, item );
	AddOptionToolTip( *icon, item );
	AddDescriptionToolTip( *icon, item );
	AddSetToolTip(* icon, item );
	AddFunctionToolTip( *icon, item );
	AddMakerToolTip( *icon, item );

	if( item.nSealed )
	{
		AddUseableTimeToolTip( *icon, item ) ;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
// 071126 LYW --- ItemManager : 남은 사용시간 툴팁 추가.
//---------------------------------------------------------------------------------------------------------------------------
void CItemManager::AddUseableTimeToolTip(cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* pItemInfo = GetItemInfo( item.wIconIdx ) ;

	if( !pItemInfo ||
		pItemInfo->wSeal != eITEM_TYPE_SEAL ||
		pItemInfo->SupplyType == ITEM_KIND_PET )
	{
		return ;
	}

	const DWORD dwRemainTime = pItemInfo->dwUseTime ;

	const int nDayValue		= 86400 ;
	const int nHourValue	= 3600 ;

	int nDay = dwRemainTime/(nDayValue) ;

	int nHour = ( dwRemainTime - (nDay * nDayValue) ) / nHourValue ;
	
	int nMinute = ( (dwRemainTime - (nDay * nDayValue)) - (nHour * nHourValue) ) / 60;

	char tempBuf[512] = {0, } ;

	icon.AddToolTipLine(CHATMGR->GetChatMsg( 1184 ), RGB_HALF( 143, 205, 217 )) ;

	if( nDay <= 0 )
	{
		if( nHour <= 0 )
		{
			if( nMinute <= 0 ) nMinute = 1 ;
			sprintf(tempBuf, CHATMGR->GetChatMsg( 1185 ), nMinute) ;
		}
		else
		{
			sprintf(tempBuf, CHATMGR->GetChatMsg( 1186 ), nHour, nMinute) ;
		}
	}
	else
	{
		sprintf(tempBuf, CHATMGR->GetChatMsg( 1187 ), nDay, nHour, nMinute) ;
	}

	icon.AddToolTipLine(tempBuf, RGB_HALF( 143, 205, 217 )) ;
}


//---------------------------------------------------------------------------------------------------------------------------
// 071115 LYW --- ItemManager : 남은 사용시간 툴팁 추가.
//---------------------------------------------------------------------------------------------------------------------------
void CItemManager::AddRemainTimeToolTip( cIcon& icon, const ITEMBASE& item, BOOL bFirst)
{
	const ITEM_INFO* pItemInfo = GetItemInfo( item.wIconIdx );

	if( !pItemInfo )
	{
		return;
	}

	if( pItemInfo->SupplyType == ITEM_KIND_PET )
	{
		return;
	}

	DWORD dwRemainTime = 0;
	char tempBuf[512] = { 0 } ;

	switch( item.nSealed )
	{		
	case eITEM_TYPE_UNSEAL:
		{
			icon.AddToolTipLine( "" );

			if( bFirst )
			{
				dwRemainTime = pItemInfo->dwUseTime ;
			}
			else
			{
				dwRemainTime = item.nRemainSecond ;
			}

			int nDayValue = 86400 ;
			int nHourValue = 3600 ;

			int nDay = dwRemainTime/(nDayValue) ;
			int nHour = ( dwRemainTime - (nDay * nDayValue) ) / nHourValue ;		
			int nMinuteValue = (dwRemainTime - (nDay * nDayValue)) - (nHour * nHourValue) ;		
			int nMinute = nMinuteValue / 60;
			int nSecond = nMinuteValue % 60;

			if( nSecond > 0 )
			{
				++nMinute ;
			}

			icon.AddToolTipLine(CHATMGR->GetChatMsg( 1179 ), RGB_HALF( 255, 255, 0 )) ;

			if( nDay <= 0 )
			{
				if( nHour <= 0 )
				{
					if( nMinute <= 0 ) 
					{
						sprintf(tempBuf, CHATMGR->GetChatMsg( 1181 ), 1) ;
					}
					else
					{
						sprintf(tempBuf, CHATMGR->GetChatMsg( 1181 ), nMinute) ;
					}

				}
				else
				{
					sprintf(tempBuf, CHATMGR->GetChatMsg( 1182 ), nHour, nMinute) ;
				}
			}
			else
			{
				sprintf(tempBuf, CHATMGR->GetChatMsg( 1183 ), nDay, nHour, nMinute) ;
			}

			icon.AddToolTipLine(tempBuf, RGB_HALF( 255, 255, 0 )) ;
			break;
		}
	case eITEM_TYPE_SEAL:
		{
			icon.AddToolTipLine( "" );

			dwRemainTime = pItemInfo->dwUseTime ;

			int nDayValue = 86400 ;
			int nHourValue = 3600 ;

			int nDay = dwRemainTime/(nDayValue) ;

			int nHour = ( dwRemainTime - (nDay * nDayValue) ) / nHourValue ;

			int nMinute = ( (dwRemainTime - (nDay * nDayValue)) - (nHour * nHourValue) ) / 60;

			icon.AddToolTipLine(CHATMGR->GetChatMsg( 1184 ), RGB_HALF( 143, 205, 217 )) ;

			if( nDay <= 0 )
			{
				if( nHour <= 0 )
				{
					if( nMinute <= 0 ) nMinute = 1 ;
					sprintf(tempBuf, CHATMGR->GetChatMsg( 1185 ), nMinute) ;
				}
				else
				{
					sprintf(tempBuf, CHATMGR->GetChatMsg( 1186 ), nHour, nMinute) ;
				}
			}
			else
			{
				sprintf(tempBuf, CHATMGR->GetChatMsg( 1187 ), nDay, nHour, nMinute) ;
			}

			icon.AddToolTipLine(tempBuf, RGB_HALF( 143, 205, 217 )) ;
			break;
		}
	}
}


void CItemManager::AddMakerToolTip( cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	// 080226 LUJ, 장착 불가능한 아이템은 제작자 정보를 표시하지 않는다
	if( !	info ||
			eEquipType_None == info->EquipType )
	{
		return;
	}

	const ITEM_OPTION& option = GetOption( item.dwDBIdx );

	const char* reinforceMaker	= option.mReinforce.mMadeBy;
	const char* mixMaker		= option.mMix.mMadeBy;
	const char* enchantMaker	= option.mEnchant.mMadeBy;

	if( ! *reinforceMaker	&&
		! *mixMaker			&&
		! *enchantMaker )
	{
		return;
	}

	char text[ MAX_PATH ];

	icon.AddToolTipLine( "" );

	if( *reinforceMaker )
	{
		sprintf( text, CHATMGR->GetChatMsg( 1170 ), reinforceMaker );
		icon.AddToolTipLine( text, RGB_HALF( 255, 255, 0 ) );
	}

	if( *mixMaker )
	{
		sprintf( text, CHATMGR->GetChatMsg( 1169 ), mixMaker );
		icon.AddToolTipLine( text, RGB_HALF( 255, 255, 0 ) );
	}

	if( *enchantMaker )
	{
		sprintf( text, CHATMGR->GetChatMsg( 1171 ), enchantMaker );
		icon.AddToolTipLine( text, RGB_HALF( 255, 255, 0 ) );
	}
}


void CItemManager::AddFunctionToolTip( cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	// 조합, 분해 가능 태그 추가
	{
		typedef std::list< std::string >	Tag;
		Tag									tag;

		if( GetMixScript( info->ItemIdx ) )
		{
			// 조합 가능
			tag.push_back( RESRCMGR->GetMsg( 637 ) );	
		}

		if( GetDissolveScript( info->ItemIdx ) )
		{
			// 분해 가능
			tag.push_back( RESRCMGR->GetMsg( 638 ) );
		}

		if( info->Improvement )
		{
			// 강화 가능ㅡ
			tag.push_back( RESRCMGR->GetMsg( 639 ) );
		}

		if( ! tag.empty() )
		{
			icon.AddToolTipLine( "" );

			for(
				Tag::const_iterator it = tag.begin();
				tag.end() != it;
				++it )
			{
				icon.AddToolTipLine( it->c_str() );
			}
		}
	}
}


void CItemManager::AddNametoolTip( cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	// 초기화
	{
		cImage image;
		SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

		icon.SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &image, TTCLR_ITEM_CANEQUIP );
	}	

	char line[ MAX_PATH ] = { 0 };

	// 이름 표시
	{
		char isSealing[ MAX_PATH ] = { 0 };

		DWORD color = GetItemNameColor( *info );

		if( //eIconType_AllItem	== icon.GetIconType() &&
			eITEM_TYPE_SEAL		== item.nSealed )
		{
			sprintf( isSealing, "[%s] ", CHATMGR->GetChatMsg( 1393 ) );

			color = RGB_HALF( 255, 0, 0 );
		}

		char isEnchant[ MAX_PATH ] = { 0 };

		// 080601 LUJ, 중첩되지 않는 아이템만 인챈트 정보를 표시한다
		if( ! info->Stack )
		{
			const ITEM_OPTION& option = GetOption( item.dwDBIdx );

			if( option.mEnchant.mLevel )
			{
				sprintf( isEnchant, "+%d ", option.mEnchant.mLevel );
			}
		}

		sprintf( line, "%s%s%s", isSealing, isEnchant, info->ItemName );

		icon.AddToolTipLine( line, color );
	}	

	// 장비 종류
	switch( eEquipType( info->EquipType ) )
	{
	case eEquipType_Weapon:
		{
			switch( eWeaponType( info->WeaponType ) )
			{
			case eWeaponType_Sword:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 660 ) );
					break;
				}				
			case eWeaponType_Mace:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 662 ) );
					break;
				}				
			case eWeaponType_Axe:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 661 ) );
					break;
				}
			case eWeaponType_Dagger:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 663 ) );
					break;
				}
			case eWeaponType_Staff:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 665 ) );
					break;
				}
			case eWeaponType_Bow:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 664 ) );
					break;
				}
			case eWeaponType_FishingPole:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 1542 ) );
					break;
				}
			}

			switch( eWeaponAnimationType( info->WeaponAnimation ) )
			{
			case eWeaponAnimationType_OneHand:
				{
					sprintf( line, "%s / %s", line, CHATMGR->GetChatMsg( 658 ) );
					break;
				}
			case eWeaponAnimationType_TwoHand:
				{
					sprintf( line, "%s / %s", line, CHATMGR->GetChatMsg( 659 ) );
					break;
				}
			}

			icon.AddToolTipLine( line, TTTC_ITEM_NAME );
			break;
		}
	case eEquipType_Armor:
		{
			switch( eEquipType( info->EquipSlot + 1 ) )
			{
			case eEquipSlot_Shield:	// 방패
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 695 ) );
					break;
				}
			case eEquipSlot_Armor: // 갑옷
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 683 ) );
					break;
				}
			case eEquipSlot_Helmet: // 투구
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 684 ) );
					break;
				}
			case eEquipSlot_Glove: // 장갑
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 685 ) );
					break;
				}
			case eEquipSlot_Boots: // 신발
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 686 ) );
					break;
				}
			case eEquipSlot_Ring1: // 반지
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 687 ) );
					break;
				}
			case eEquipSlot_Necklace: // 목걸이
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 688 ) );
					break;
				}
			case eEquipSlot_Earring1: // 귀걸이
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 689 ) );
					break;
				}
			case eEquipSlot_Belt: // 벨트
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 690 ) );
					break;
				}
			case eEquipSlot_Band: // 머리장식
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 691 ) );
					break;
				}
			case eEquipSlot_Glasses:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 832 ) );
					break;
				}
			case eEquipSlot_Mask:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 833 ) );
					break;
				}
			case eEquipSlot_Wing:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 834 ) );
					break;
				}
			case eEquipSlot_Costume_Head:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 835 ) );
					break;
				}
			case eEquipSlot_Costume_Dress:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 836 ) );
					break;
				}
			case eEquipSlot_Costume_Glove:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 837 ) );
					break;
				}
			case eEquipSlot_Costume_Shoes:
				{
					sprintf( line, "%s", CHATMGR->GetChatMsg( 838 ) );
					break;
				}
			}

			// D : 종류
			switch( eArmorType( info->ArmorType ) )
			{
			case eArmorType_Robe:
				{
					sprintf( line, "%s / %s", line, CHATMGR->GetChatMsg( 692 ) );
					break;
				}
				break;
			case eArmorType_Leather:
				{
					sprintf( line, "%s / %s", line, CHATMGR->GetChatMsg( 693 ) );
					break;
				}
			case eArmorType_Metal:
				{
					sprintf( line, "%s / %s", line, CHATMGR->GetChatMsg( 694 ) );
					break;
				}				
			}

			icon.AddToolTipLine( line, TTTC_ITEM_NAME );
			break;
		}
	}
}


void CItemManager::AddDescriptionToolTip( cIcon& icon, const ITEMBASE& item, DWORD color )
{
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	const TOOLTIP_TEXT* text = m_ItemToolTipTextList.GetData( info->ItemTooltipIdx );

	// 080227 LUJ, 옵션이 있거나 장비 아이템인 경우 한 줄 띄도록 한다
	if( text && 
		( GetOption( item.dwDBIdx ).mItemDbIndex || eEquipType_None != info->EquipType ) )
	{
		icon.AddToolTipLine( "" );
	}

	for(
		;
		text;
		text = text->pNext )
	{
		if( *( text->strToolTip ) )
		{
			icon.AddToolTipLine( text->strToolTip, color );
		}
	}
}


void CItemManager::ItemDelete(CItem* pItem)
{
	m_ItemHash.Remove(pItem->GetDBIdx());	
	//ICFreeIndex(&m_IconIndexCreator,pItem->GetID()-START_ITEMTCON_ID);
	m_IconIndexCreator.ReleaseIndex(pItem->GetID());
	
//KES EXCHANGE 031002
	if( pItem->GetLinkItem() )
	{
		GAMEIN->GetExchangeDialog()->DelItem( 0, pItem->GetLinkItem()->GetPosition() );
	}
//

//	WINDOWMGR->DeleteWindowForID(pItem->GetID());	//KES 즉시지우지마시오. 031128
	WINDOWMGR->AddListDestroyWindow( pItem );
}


void CItemManager::Release()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	while(pItem = m_ItemHash.GetData())
	{
		ItemDelete(pItem);
		//ICFreeIndex(&m_IconIndexCreator,pItem->GetID()-START_ITEMTCON_ID);		
	}
	m_ItemHash.RemoveAll();

	{
		for(
			OptionMap::iterator it = mOptionMap.begin();
			mOptionMap.end() != it;
			++it )
		{
			SAFE_DELETE( it->second );
		}

		mOptionMap.clear();
	}	
	
	// 윈도우 메니져에서 Delete해주기 때문에 따로 할필요가 없다

	// 윈도우를 지우지 않기 때문에 따로 delete를 해주어야 한다. KES 040316
	GAMEIN->GetInventoryDialog()->ReleaseInventory();

	// 071218 LUJ
	for(
		PreIconList::iterator it = mPreIconList.begin();
		mPreIconList.end() != it;
		++it )
	{
		CItem* item = *it;

		SAFE_DELETE( item );
	}
		
	mPreIconList.clear();
}


CItem* CItemManager::GetItem(DWORD dwDBIdx)
{
	return m_ItemHash.GetData( dwDBIdx );
}


void CItemManager::CombineItem( MSG_ITEM_COMBINE_ACK * pMsg )
{	
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);

	CItem * pFromItem = GetItemofTable(FromTableIdx, pMsg->FromPos);
	CItem * pToItem = GetItemofTable(ToTableIdx, pMsg->ToPos);

	if( !pFromItem || !pToItem )
	{
		ASSERTMSG(0, "Item Combine Failed : pToItem=NULL or pFromItem=NULL");
		return;
	}
	if( ( pFromItem->GetItemIdx() != pToItem->GetItemIdx() ) || ( pFromItem->GetItemIdx() != pMsg->wItemIdx) )
//	if( ( pFromItem->GetItemIdx() != pToItem->GetItemIdx() ) || ( pFromItem->GetItemIdx() != pMsg->CombineItemIdx ) )
	{
		ASSERTMSG(0, "Item Combine Failed : FromItemIdx != ToItemIdx");
		return;
	}
	pFromItem->SetDurability( pMsg->FromDur );
	pToItem->SetDurability( pMsg->ToDur );
	
	if( pMsg->FromDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition(), FALSE);
	//	DeleteItem( pFromItem->GetPosition() );
	}

	if( pMsg->ToDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition(), FALSE);
	//	DeleteItem( pToItem->GetPosition() );
	}
}

void CItemManager::MoveItem( MSG_ITEM_MOVE_ACK * pMsg )
{
	BYTE tableIdx = GetTableIdxForAbsPos(pMsg->ToPos);
	if(tableIdx == eItemTable_Storage)
		MoveItemToStorage(pMsg);
	else if(tableIdx == eItemTable_GuildWarehouse)
		MoveItemToGuild(pMsg);
	else if(tableIdx == eItemTable_Shop)
		MoveItemToShop(pMsg);
	else if(tableIdx == eItemTable_ShopInven)
		MoveItemToShopInven(pMsg);
	else if(tableIdx == eItemTable_Inventory || tableIdx == eItemTable_Weared)
	{
		CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
		CStorageDialog* pStorage = GAMEIN->GetStorageDialog();
		CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
		CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
		
		CItem * FromItem = NULL;
		CItem * ToItem = NULL;
		
		pInven->DeleteItem(pMsg->ToPos, &ToItem);

		WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
		if(FromTableIdx == eItemTable_Storage)
		{
			pStorage->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_GuildWarehouse)
		{
			pGuild->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_Shop)
		{
			// 후에 샵아이템 인벤에만 들어가도록 추가

			pShop->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else 
			pInven->DeleteItem( pMsg->FromPos , &FromItem );

		if(ToItem)
		{
			ToItem->SetPosition( pMsg->FromPos );

			ITEM_INFO* pinfo = GetItemInfo( ToItem->GetItemBaseInfo().wIconIdx );
			if( pinfo )
			{
				POSTYPE pos = 0;
/*				if( (pinfo->ItemKind & eEQUIP_ITEM) && pInven->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_SHOPINVEN_START <= pMsg->FromPos && pMsg->FromPos < TP_SHOPINVEN_END )
					{
						FromTableIdx = eItemTable_Inventory;                        
						ToItem->SetPosition( pos );						
					}
				}
				else if( (pinfo->ItemKind == eSHOP_ITEM_EQUIP) && pInven->GetShopInven()->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_INVENTORY_START <= pMsg->FromPos && pMsg->FromPos < TP_INVENTORY_END )
					{
						FromTableIdx = eItemTable_ShopInven;
						ToItem->SetPosition( pos );
					}
				}
*/			}
			if(FromTableIdx == eItemTable_Storage)
			{
				if( ToItem->IsQuickLink() )
				{
					ToItem->RemoveQuickLink();
				}
				pStorage->AddItem(ToItem);
			}
			else if(FromTableIdx == eItemTable_GuildWarehouse)
			{
				if( ToItem->IsQuickLink() )
				{
					ToItem->RemoveQuickLink();
				}
				pGuild->AddItem(ToItem);
				QUICKMGR->RefreshQickItem();
			}
			else
				pInven->AddItem(ToItem);
		}
		
		if(FromItem == 0)
		{
			ASSERT(0);
			return;
		}
		FromItem->SetPosition(pMsg->ToPos);
		pInven->AddItem(FromItem); 
		QUICKMGR->RefreshQickItem();
	}
}

void CItemManager::PetMoveItem( MSG_ITEM_MOVE_ACK * pMsg )
{
	CPetWearedDialog* pPetDlg = GAMEIN->GetPetWearedDlg();
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CStorageDialog* pStorage = GAMEIN->GetStorageDialog();

	CItem * FromItem = NULL;
	cIcon * buf = NULL;
	CItem * ToItem = NULL;

	pPetDlg->DeleteItem(pMsg->ToPos, &buf);
	ToItem = ( CItem* )buf;

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	if(FromTableIdx == eItemTable_Storage)
	{
		pStorage->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else 
		pInven->DeleteItem( pMsg->FromPos , &FromItem );

	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );

		ITEM_INFO* pinfo = GetItemInfo( ToItem->GetItemBaseInfo().wIconIdx );
		if( pinfo )
		{
			POSTYPE pos = 0;

			if(FromTableIdx == eItemTable_Storage)
			{
				if( ToItem->IsQuickLink() )
				{
					ToItem->RemoveQuickLink();
				}
				pStorage->AddItem(ToItem);
			}
			else
				pInven->AddItem(ToItem);
		}
	}	

	if(FromItem)
	{
		FromItem->SetPosition( pMsg->ToPos );
		pPetDlg->AddItem( pMsg->ToPos, (cIcon*)FromItem );
	}

	QUICKMGR->RefreshQickItem();
}

void CItemManager::MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg )
{	
	CGuildWarehouseDialog * pGuildDlg	= GAMEIN->GetGuildWarehouseDlg();
	CInventoryExDialog * pInvenDlg = GAMEIN->GetInventoryDialog();
		
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);

	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pGuildDlg->DeleteItem(pMsg->ToPos , &ToItem);

	if(FromTableIdx == eItemTable_GuildWarehouse)
	{
		pGuildDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInvenDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else 
		ASSERT(0);
	
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory)
			pInvenDlg->AddItem(ToItem);
		else
			pGuildDlg->AddItem(ToItem);
	}

	ReLinkQuickPosition(FromItem);
	if( FromItem->IsQuickLink() )
	{
		FromItem->RemoveQuickLink();
	}
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	pGuildDlg->AddItem(FromItem);
	
	QUICKMGR->RefreshQickItem();

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
}

void CItemManager::MoveItemToStorage(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CStorageDialog* pStorage = GAMEIN->GetStorageDialog();
	
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pStorage->DeleteItem(pMsg->ToPos, &ToItem);
	
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );				
	}
		
	else if(FromTableIdx == eItemTable_Storage)
	{
		pStorage->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}
		
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
			pInven->AddItem(ToItem);
		else
			pStorage->AddItem(ToItem);
		if( ToItem->IsQuickLink() )
		{
			ToItem->RemoveQuickLink();
		}
	}

	ReLinkQuickPosition(FromItem);
	
	if( FromItem->IsQuickLink() )
	{
		FromItem->RemoveQuickLink();
	}
	ASSERT(FromItem);

	// 071125 LYW --- ItemManager : 시간 아이템 이동 처리 추가.
	const ITEMBASE& pItemBase = FromItem->GetItemBaseInfo() ;

	if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
	{
		const ITEM_OPTION& pOption = GetOption( FromItem->GetItemBaseInfo() ) ;

		AddTimeToolTip(((CItem*)FromItem), pOption, FALSE );
	}

	FromItem->SetPosition(pMsg->ToPos);
	pStorage->AddItem(FromItem);
	
	QUICKMGR->RefreshQickItem();
}



void CItemManager::MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
	
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pShop->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if( FromTableIdx == eItemTable_Shop )
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );				
	}
	else if( FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
		
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );		

		if( FromTableIdx == eItemTable_Shop )		
			pShop->AddItem(ToItem);	
		else if( FromTableIdx == eItemTable_ShopInven )		
			pInven->AddItem(ToItem);
		if( ToItem->IsQuickLink() )
		{
			ToItem->RemoveQuickLink();
		}
	}

	ReLinkQuickPosition(FromItem);
	if( FromItem->IsQuickLink() )
	{
		FromItem->RemoveQuickLink();
	}
	ASSERT(FromItem);


	FromItem->SetPosition(pMsg->ToPos);
	pShop->AddItem(FromItem);

	QUICKMGR->RefreshQickItem();
}



void CItemManager::MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();	

	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pInven->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if(FromTableIdx == eItemTable_Shop)
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );
	}	
	else if(FromTableIdx == eItemTable_ShopInven)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_Weared )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	
	if(ToItem)
	{
		// ShopInven내에서만 이동한다.
		ToItem->SetPosition( pMsg->FromPos );

		if(FromTableIdx == eItemTable_Shop)		
			pShop->AddItem(ToItem);
		else if(FromTableIdx == eItemTable_ShopInven)		
			pInven->AddItem(ToItem);
		if( ToItem->IsQuickLink() )
		{
			ToItem->RemoveQuickLink();
		}
	}

	ReLinkQuickPosition(FromItem);
	if( FromItem->IsQuickLink() )
	{
		FromItem->RemoveQuickLink();
	}
	ASSERT(FromItem);

	FromItem->SetPosition(pMsg->ToPos);
	pInven->AddItem(FromItem);	

	QUICKMGR->RefreshQickItem();
}

void CItemManager::DeleteItem( POSTYPE absPos, CItem** ppItemOut )
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);

	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
			pInven->DeleteItem( absPos, ppItemOut );

		}break;
	case eItemTable_Storage:
		{
			CStorageDialog* pStorage = GAMEIN->GetStorageDialog();
			pStorage->DeleteItem( absPos, ppItemOut );
			
		}break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
			pGuild->DeleteItem( absPos, ppItemOut );
		}break;
	}

	if((*ppItemOut))
	{
		RemoveOption((*ppItemOut)->GetDBIdx());
		
		if( (*ppItemOut)->IsQuickLink() )
		{
			(*ppItemOut)->RemoveQuickLink();
		}

		ItemDelete( (*ppItemOut) );
	}
		
}

//void CItemManager::DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION * pItemOptionInfoOut/*, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL*/ )
// 071202 LYW --- ItemManager : 봉인 아이템이 삭제 될 때, 순수 삭제될 때와 상점에 팔 때에도 같은 이 함수를 사용하기 때문에, 
//								함수 뒤쪽 인자로 팔때인지 삭제인지 여부를 체크하도록 수정함.
void CItemManager::DeleteItemofTable(WORD TableIdx, POSTYPE absPos, BOOL bSellDel )
{
	CItem * pItemOut = NULL;
	
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}

			if(pItemOut)
			{
/*				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					if( !(pItemOut->GetItemParam() & ITEM_PARAM_SEAL) )
						RemoveUsedItemInfo( pItemOut->GetItemIdx(), pItemOut->GetItemBaseInfo().dwDBIdx );
					if( pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend || 
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMove2 )
						HERO->CalcShopItemOption( eIncantation_MemoryMoveExtend, FALSE );
				}
*/			}

			// 071124 LYW --- ItemManager : 시간 완료 아이템 삭제 메시지 추가.
			const ITEMBASE& pItemBase = pItemOut->GetItemBaseInfo() ;
			
			if( pItemBase.nSealed == eITEM_TYPE_UNSEAL && !bSellDel )
			{
				if( m_nSealItemDelPos != (int)pItemBase.Position )
				{
					CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg( 1192 ), CHATMGR->GetChatMsg( 1189 ), pItemOut->GetItemInfo()->ItemName );
				}
			}

			// 071127 LYW --- ItemManager : 삭제 아이템 ( 봉인 관련 ) 위치 저장 변수 추가.
			m_nSealItemDelPos = -1 ;
		}
		break;
	case eItemTable_Storage:
		{
			CStorageDialog * pDlg = GAMEIN->GetStorageDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}

			// 071124 LYW --- ItemManager : 시간 완료 아이템 삭제 메시지 추가.
			const ITEMBASE& pItemBase = pItemOut->GetItemBaseInfo() ;
			
			if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
			{
				if( m_nSealItemDelPos != (int)pItemBase.Position )
				{
					CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg( 1192 ), CHATMGR->GetChatMsg( 1188 ), pItemOut->GetItemInfo()->ItemName );
				}
			}

			// 071127 LYW --- ItemManager : 삭제 아이템 ( 봉인 관련 ) 위치 저장 변수 추가.
			m_nSealItemDelPos = -1 ;
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}

			if(pItemOut)
			{
				/*if( pItemOut->GetItemInfo()->ItemKind == eSHOP_ITEM_MAKEUP || 
					pItemOut->GetItemInfo()->ItemKind == eSHOP_ITEM_DECORATION )*/
/*				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					RemoveUsedItemInfo( pItemOut->GetItemInfo()->ItemIdx, pItemOut->GetItemBaseInfo().dwDBIdx );
				}
*/			}
		}
		break;
	}
	if(pItemOut)
	{
		RemoveOption(pItemOut->GetDBIdx() );

		if( pItemOut->IsQuickLink() )
		{
			pItemOut->RemoveQuickLink();
		}
		ItemDelete( pItemOut );
	}
}


void CItemManager::RemoveOption( DWORD dwOptionIdx )
{
	OptionMap::iterator it = mOptionMap.find( dwOptionIdx );

	if( mOptionMap.end() == it )
	{
		return;
	}

	SAFE_DELETE( it->second );

	mOptionMap.erase( it );
}


void CItemManager::AddOption( const ITEM_OPTION& option )
{
	OptionMap::iterator it = mOptionMap.find( option.mItemDbIndex );

	// 이미 할당 공간이 있으면 해당 메모리에 복사해버린다
	if( mOptionMap.end() != it )
	{
		*( it->second ) = option;
		return;
	}

	mOptionMap.insert( std::make_pair( option.mItemDbIndex, new ITEM_OPTION( option ) ) );
}



const ITEM_OPTION& CItemManager::GetOption( const ITEMBASE& item ) const
{
	return GetOption( item.dwDBIdx );
}


const ITEM_OPTION& CItemManager::GetOption( DWORD itemDbIndex ) const
{
	OptionMap::const_iterator it = mOptionMap.find( itemDbIndex );

	if( mOptionMap.end() == it )
	{
		const static ITEM_OPTION emptyOption;

		return emptyOption;
	}

	return *( it->second );
}


void CItemManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_ITEM_TOTALINFO_LOCAL:					Item_TotalInfo_Local( pMsg ) ;					break;
	case MP_ITEM_STORAGE_ITEM_INFO_ACK:				Item_Storage_Item_Info_Ack( pMsg ) ;			break;
	case MP_ITEM_MOVE_ACK:							Item_Move_Ack( pMsg ) ;							break;
	case MP_ITEM_COMBINE_ACK:						Item_Combine_Ack( pMsg ) ;						break;
	case MP_ITEM_DIVIDE_ACK:						Item_Divide_Ack( pMsg ) ;						break;
	case MP_ITEM_MONSTER_OBTAIN_NOTIFY:				Item_Monster_Obtain_Notify( pMsg ) ;			break;
	case MP_ITEM_MOVE_NACK:							Item_Move_Nack( pMsg ) ;						break;
	case MP_ITEM_DIVIDE_NACK:						Item_Divite_Nack( pMsg ) ;						break;
	case MP_ITEM_COMBINE_NACK:						Item_Combine_Nack( pMsg ) ;						break;
	case MP_ITEM_DISCARD_NACK:						Item_Discard_Nack( pMsg ) ;						break;
	case MP_ITEM_DISCARD_ACK:						Item_Discard_Ack( pMsg ) ;						break;
	case MP_ITEM_USE_NOTIFY:						Item_Use_Notify( pMsg ) ;						break;
	case MP_ITEM_USE_ACK:							Item_Use_Ack( pMsg ) ;							break;
	case MP_ITEM_REINFORCE_OPTION_ACK:				Item_Reinforce_Option_Ack( pMsg );				break;
	case MP_ITEM_REINFORCE_NACK:					Item_ReInForce_Nack( pMsg ) ;					break;
	case MP_ITEM_REINFORCE_SUCCEED_ACK:
	case MP_ITEM_REINFORCE_FAILED_ACK:
		{
			Item_ReInForce_Ack( pMsg );
			break;
		}	
	case MP_ITEM_MIX_SUCCEED_ACK:
	case MP_ITEM_MIX_FAILED_ACK:
	case MP_ITEM_MIX_BIGFAILED_ACK:
		{
			Item_Mix_Ack( pMsg );
			break;
		}
	case MP_ITEM_MIX_NACK:							Item_Mix_Nack( pMsg ) ;							break;
	case MP_ITEM_MIX_GETITEM:						Item_Mix_GetItem( pMsg );						break;
	case MP_ITEM_ENCHANT_SUCCESS_ACK:				Item_Enchant_Success_Ack( pMsg ) ;				break;
	case MP_ITEM_ENCHANT_NACK:						Item_Enchant_Nack( pMsg ) ;						break;
	case MP_ITEM_ENCHANT_FAILED_ACK:				Item_Enchant_Failed_Ack( pMsg );				break;
	case MP_ITEM_WORKING_SUCCESS:					Item_Working_Success( pMsg ) ;					break;
	case MP_ITEM_WORKING_FAIL:						Item_Working_Fail( pMsg ) ;						break;
	case MP_ITEM_WORKING_START:						Item_Working_Start( pMsg ) ;					break;
	case MP_ITEM_WORKING_STOP:						Item_Working_Stop( pMsg ) ;						break;
	case MP_ITEM_USE_NACK:							Item_Use_Nack( pMsg ) ;							break;
	case MP_ITEM_SELL_ACK:							Item_Sell_Ack( pMsg ) ;							break;
	case MP_ITEM_SELL_NACK:							Item_Sell_Nack( pMsg ) ;						break;
	case MP_ITEM_BUY_ACK:							Item_Buy_Ack( pMsg ) ;							break;
	case MP_ITEM_BUY_NACK:							Item_Buy_Nack( pMsg ) ;							break;
	case MP_ITEM_DEALER_ACK:						Item_Dealer_Ack( pMsg ) ;						break;
	case MP_ITEM_APPEARANCE_ADD:					Item_Appearance_Add( pMsg );					break;
	case MP_ITEM_APPEARANCE_REMOVE:					Item_Appearance_Remove( pMsg );					break;
	case MP_ITEM_MONEY:								Item_Money( pMsg ) ;							break;
	case MP_ITEM_MONEY_ERROR:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(115) );		break;
	case MP_ITEM_OBTAIN_MONEY:						Item_Obtain_Money( pMsg ) ;						break;
	case MP_ITEM_DIVIDE_NEWITEM_NOTIFY:				Item_Divide_NewItem_Notify( pMsg ) ;			break;
	case MP_ITEM_GUILD_MOVE_ACK:					Item_Guild_Move_Ack( pMsg ) ;					break;
	case MP_ITEM_GUILD_MOVE_NACK:					Item_Guild_Move_Nack( pMsg ) ;					break;
	case MP_ITEM_DELETEFROM_GUILDWARE_NOTIFY:		Item_DeleteFrom_GuildWare_Notify( pMsg ) ;		break;
	case MP_ITEM_ADDTO_GUILDWARE_NOTIFY:			Item_AddTo_GuildWare_Notify( pMsg ) ;			break;
	case MP_ITEM_STORAGE_MOVE_ACK:					Item_Storage_Move_Ack( pMsg ) ;					break;
	case MP_ITEM_DISSOLVE_GETITEM:					Item_Dissolve_GetItem( pMsg ) ;					break;
	case MP_ITEM_DISSOLVE_ACK:						Item_Dissolve_Ack( pMsg ) ;						break;
	case MP_ITEM_DISSOLVE_NACK:						Item_Dissolve_Nack( pMsg ) ;					break;	
	case MP_ITEM_ERROR_NACK:						Item_Error_Nack( pMsg ) ;						break;
	case MP_ITEM_CHANGEITEM_NACK:					Item_ChangeItem_Nack( pMsg ) ;					break;
	case MP_ITEM_USE_CHANGEITEM_ACK:				Item_Use_ChangeItem_Ack( pMsg ) ;				break;
	case MP_ITEM_TIMELIMT_ITEM_ONEMINUTE :			Item_TimeLimit_Item_OneMinute( pMsg ) ;			break;
	case MP_ITEM_SHOPITEM_USE_ACK:					Item_ShopItem_Use_Ack( pMsg ) ;					break;
	case MP_ITEM_SHOPITEM_USE_NACK:					Item_ShopItem_Use_Nack( pMsg ) ;				break;
	case MP_ITEM_EVENTITEM_USE:						Item_EventItem_Use( pMsg ) ;					break;
	case MP_ITEM_SHOPITEM_INFO_ACK:					Item_ShopItem_Info_Ack( pMsg ) ;				break;
	case MP_ITEM_SKILL_ACK :						Item_Skill_Ack( pMsg ) ;						break;
	case MP_ITEM_RESET_STATS_ACK :					Item_Reset_Stats_Ack( pMsg ) ;					break;
	case MP_ITEM_RESET_STATS_NACK :					Item_Reset_Stats_Nack( pMsg ) ;					break;
	case MP_ITEM_MAPMOVE_SCROLL_NACK :				Item_MapMoveScroll_Nack( pMsg ) ;				break;
	case MP_ITEM_INCREASE_INVENTORY_ACK :			Item_Increase_Inventory_Ack( pMsg ) ;			break;
	case MP_ITEM_INCREASE_INVENTORY_NACK :			Item_Increase_Inventory_Nack( pMsg ) ;			break;
	case MP_ITEM_CHANGE_CHARACTER_NAME_ACK :		Item_Change_Character_Name_Ack( pMsg ) ;		break;
	case MP_ITEM_CHANGE_CHARACTER_NAME_NACK :		Item_Change_Character_Name_Nack( pMsg ) ;		break;
	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.13
	case MP_ITEM_MARRIAGE_ACK:						Item_Monster_Obtain_Notify( pMsg ) ;			break;
	// E 결혼 추가 added by hseos 2007.12.13
	case MP_ITEM_GIFT_EVENT_NOTIFY:					Item_Gift_Event_Notify( pMsg );					break;

	// 080124 LUJ, 옵션 추가
	case MP_ITEM_OPTION_ADD_ACK:
		{
			const MSG_ITEM_OPTION* message = ( MSG_ITEM_OPTION* )pMsg;

			for( DWORD i = 0; i < message->mSize; ++i )
			{
				const ITEM_OPTION& option = message->mOption[ i ];

				AddOption( option );
			}
			break;
		}
	// 080218 LUJ, 아이템 옵션 부여
	case MP_ITEM_APPLY_OPTION_ACK:
		{
			CApplyOptionDialog* dialog = ( CApplyOptionDialog* )WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );

			if( dialog )
			{
				dialog->Succeed();
			}

			break;
		}
	// 080218 LUJ, 아이템 옵션 부여
	case MP_ITEM_APPLY_OPTION_NACK:
		{
			CApplyOptionDialog* dialog = ( CApplyOptionDialog* )WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );

			if( dialog )
			{
				dialog->Fail();
			}

			break;
		}

	// 080317 NYJ --- 전체외치기 아이템 사용실패
	case MP_ITEM_SHOPITEM_SHOUT_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1461 ) );
			break;
		}

	case MP_ITEM_ENCHANT_EFFECT_NOTIFY:
		{
			MSG_DWORDBYTE* pmsg = ( MSG_DWORDBYTE* )pMsg;

			CObject* pTarget = OBJECTMGR->GetObject( pmsg->dwData );

			if( !pTarget || pTarget->GetObjectKind() != eObjectKind_Player )
				break;

			CPlayer* pPlayer = ( CPlayer* )pTarget;

			pPlayer->GetCharacterTotalInfo()->WeaponEnchant = pmsg->bData;
		}
		break;
	case MP_ITEM_PET_MOVE_ACK:	Item_Pet_Move_Ack( pMsg ) ;	break;

	//case MP_ITEM_DEALER_NACK:																		break;
	//case MP_ITEM_APPEARANCE_CHANGE:					Item_Appearance_Change( pMsg ) ;				break;
/*
	case MP_ITEM_STORAGE_INFO_ACK:
		{
			SEND_STORAGE_ITEM_INFO* pmsg = (SEND_STORAGE_ITEM_INFO*)pMsg;
			//혹시나 해서 문파 창 닫음.
			GAMEIN->GetMunpaDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			GAMEIN->GetStorageDialog()->SetActive(TRUE);
			GAMEIN->GetStorageDialog()->SetStorageItem(pmsg);
			GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1);
			GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1);
			RefreshAllItem();
		}
		break;*/

	//////////////////////////////////////////////////////////////////////////
	//nack
/*
	case MP_ITEM_STORAGE_INFO_NACK:
		GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare); break;
*/

		// 071121 LYW - Modified.
	/*case MP_ITEM_SHOPITEM_UNSEAL_ACK:
		{
			ASSERT(pMsg) ;

			MSG_DWORD* pmsg = NULL ;
			pmsg = (MSG_DWORD*)pMsg ;

			ASSERT(pmsg) ;

			CInventoryExDialog* pInven = NULL ;
			pInven = GAMEIN->GetInventoryDialog() ;

			ASSERT(pInven) ;
			
			pInven->ItemUnSealing(pmsg->dwData) ;

			QUICKMGR->RefreshQickItem() ;

			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pmsg->dwData;
			msg.UseBaseInfo.TargetItemIdx = pmsg->dwData;

			NETWORK->Send(&msg,sizeof(msg));
		}
		break;
	case MP_ITEM_SHOPITEM_UNSEAL_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, "잠김 해제에 실패하였습니다." );
		}
		break;*/

	//case MP_ITEM_SHOPITEM_USEEND:
	//	{
	//		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	//		//// 기간제 이동주문서를 지워줘야 한다.
	//		//ITEM_INFO* pInfo = GetItemInfo((WORD)pmsg->dwData );
	//		//if( !pInfo )		return;
	//		//SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData((WORD)pmsg->dwData);
	//		//if( !pShopItemBase )		return;

	//		//if( pInfo->ItemType == 11 )
	//		//{
	//		//	BOOL bfind = FALSE;
	//		//	for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
	//		//	{
	//		//		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );					
	//		//		if( pItemBase && pItemBase.dwDBIdx == pShopItemBase.ItemBase.dwDBIdx )
	//		//		{
	//		//			CItem* pOutItem = NULL;
	//		//			DeleteItem( i+TP_SHOPINVEN_START, &pOutItem );

	//		//			if( pOutItem )
	//		//			{
	//		//				char buf[256] = { 0, };
	//		//				sprintf( buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName );
	//		//				CHATMGR->AddMsg(CTC_SYSMSG, buf);
	//		//			}
	//		//			bfind = TRUE;
	//		//			break;
	//		//		}
	//		//	}
	//		//	if( !bfind )
	//		//	{
	//		//		for(int i=0; i<SLOT_WEAR_NUM; i++)
	//		//		{
	//		//			ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_WEAR_START );					
	//		//			if( pItemBase && pItemBase.dwDBIdx == pShopItemBase.ItemBase.dwDBIdx )
	//		//			{
	//		//				CItem* pOutItem = NULL;
	//		//				DeleteItem( i+TP_WEAR_START, &pOutItem );

	//		//				if( pOutItem )
	//		//				{
	//		//					char buf[256] = { 0, };
	//		//					sprintf( buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName );
	//		//					CHATMGR->AddMsg(CTC_SYSMSG, buf);
	//		//				}

	//		//				bfind = TRUE;
	//		//				break;
	//		//			}
	//		//		}
	//		//	}
	//		//}
	//		//
	//		//RemoveUsedItemInfo((WORD)pmsg->dwData, pShopItemBase.ItemBase.dwDBIdx);
	//		//HERO->CalcShopItemOption((WORD)pmsg->dwData, FALSE);
	//	}
	//	break;
	//case MP_ITEM_SHOPITEM_ONEMINUTE:
	//	{
	//		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	//		//STATUSICONDLG->SetOneMinuteToShopItem(pmsg->dwData);
	//	}
	//	break;		
/*
	case MP_ITEM_SHOPITEM_INFO_ACK:
		{
			SEND_SHOPITEM_INFO* pmsg = (SEND_SHOPITEM_INFO*)pMsg;

			if( pmsg->ItemCount )
			{
				GAMEIN->GetItemShopDialog()->SetItemInfo(pmsg);
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(785) );
			}

			GAMEIN->GetItemShopDialog()->SetActive(TRUE);
			GAMEIN->GetInventoryDialog()->SetActive( TRUE );
//			GAMEIN->GetInventoryDialog()->OnShopItemBtn();
//			GAMEIN->GetInventoryDialog()->SetShopItemInvenBtn( TRUE );			
		}
		break;

	case MP_ITEM_SHOPITEM_MPINFO:
		{
			SEND_MOVEDATA_INFO* pmsg = (SEND_MOVEDATA_INFO*)pMsg;			
			
			if( pmsg->Count == 0 && pmsg->bInited )
			{
				SetDisableDialog(FALSE, eItemTable_Inventory);
				SetDisableDialog(FALSE, eItemTable_Storage);
				SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				SetDisableDialog(FALSE, eItemTable_Shop);

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(794) );

				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}

			GAMEIN->GetMoveDialog()->SetMoveInfo( pmsg );
			if( pmsg->bInited )
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
		}
		break;
	
	case MP_ITEM_SHOPITEM_MSG:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
		}
		break;
	
	case MP_ITEM_SHOPITEM_MONEYPROTECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(753) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_EXPPROTECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(754) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_MIXUP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(755) );
			RemoveUsedItemInfo( (WORD)pmsg->dwData );
		}
		break;
	case MP_ITEM_SHOPITEM_STATEMINUS:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( pmsg->dwData > 0 )
				GAMEIN->GetCharacterDialog()->SetMinusPoint(TRUE);
			else
				GAMEIN->GetCharacterDialog()->SetMinusPoint(FALSE);
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Storage);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetMoveDialog()->AddMoveInfo( &pmsg->Data );
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(792) );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_NACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Storage);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(793) );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

			GAMEIN->GetMoveDialog()->UpdateMoveInfo( &pmsg->Data );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_DEL_ACK:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

			GAMEIN->GetMoveDialog()->DelMoveInfo( &pmsg->Data );
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_SYN:
		{
			SEND_REVIVEOTHER* pmsg = (SEND_REVIVEOTHER*)pMsg;
			
			if( !GAMEIN->GetReviveDialog()->IsActive() )
			{
				MSG_DWORD2 msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.dwObjectID = HEROID;
				msg.dwData1 = pmsg->TargetID;
				msg.dwData2 = eShopItemErr_Revive_NotReady;			// Not Ready
				NETWORK->Send( &msg, sizeof(msg) );
				return;
			}

			GAMEIN->GetInventoryDialog()->SetReviveData( pmsg->TargetID, 0, 0 );

			char buf[128] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(762), pmsg->TargetName );
			WINDOWMGR->MsgBox( MBI_REVIVECONFIRM, MBT_YESNO, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Storage);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);	
			
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
			if(!pPlayer)		return;
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			SetDisableDialog(FALSE, eItemTable_Inventory);
			SetDisableDialog(FALSE, eItemTable_Storage);
			SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
			SetDisableDialog(FALSE, eItemTable_Shop);

			switch( pmsg->dwData )
			{
			case eShopItemErr_Revive_Fail:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(765) );
				break;
			case eShopItemErr_Revive_NotDead:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(765) );
				break;
			case eShopItemErr_Revive_NotUse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
				break;
			case eShopItemErr_Revive_Refuse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(787) );
				break;
			case eShopItemErr_Revive_TooFar:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(789) );
				break;
			case eShopItemErr_Revive_TimeOver:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(790) );
				break;
			case eShopItemErr_Revive_NotReady:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(920) );
				break;
			}
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_ITEM_SHOPITEM_AVATAR_PUTON:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				APPEARANCEMGR->SetAvatarItem( (CPlayer*)pObject, (WORD)pmsg->dwData2 );				
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_TAKEOFF:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
			if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
			{
				APPEARANCEMGR->SetAvatarItem( (CPlayer*)pObject, (WORD)pmsg->dwData2, FALSE );
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE:	
		{
			SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;

			AddUsedItemInfo(&pmsg->ShopItemBase);
		}
		break;


	case MP_ITEM_SHOPITEM_AVATAR_INFO:
		{
			SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
			if(!pPlayer)		return;

			AVATARITEM* pInfo= GAMERESRCMNGR->m_AvatarEquipTable.GetData( pmsg->ItemIdx );
			if(!pInfo)		return;	
			
			if( pmsg->PlayerId == HERO->GetID() )
				HERO->CheckShopItemUseInfoToInventory( pmsg->ItemIdx, pmsg->ItemPos );

			if( pInfo->Position == eAvatar_Hair )
			{
				int MotionIdx = -1;
				WORD Weapontype = pPlayer->GetWeaponEquipType();
				if( Weapontype==WP_GUM || Weapontype==WP_GWUN || Weapontype==WP_GUNG || Weapontype==WP_AMGI ||
					Weapontype==WP_EVENT )
					MotionIdx = eMotion_Item_ChangeHair_1;
				else if( Weapontype==WP_DO )
					MotionIdx = eMotion_Item_ChangeHair_2;
				else if( Weapontype==WP_CHANG )
					MotionIdx = eMotion_Item_ChangeHair_3;

				if( pPlayer->GetID() == HERO->GetID() )
					MOVEMGR->HeroMoveStop();

				if( pPlayer->GetState() == eObjectState_None )
				{
					pPlayer->SetItemUseMotionIdx(MotionIdx);
					pPlayer->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);
				}

				OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_ItemUse);
				OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_ItemUse, pPlayer->GetEngineObject()->GetAnimationTime(MotionIdx));				
				memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
			}
			else
			{
				memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
				APPEARANCEMGR->InitAppearance( pPlayer );
			}

			pPlayer->CalcAvatarOption();
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_ACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wData2 );
			if( !pItem )		return;
			pItem->SetUseParam( 1 );

			char buf[256] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(779), pItem->GetItemInfo()->ItemName );
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_NACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wData2 );
			if( !pItem )		return;
			pItem->SetUseParam( 0 );

			char buf[256] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(780), pItem->GetItemInfo()->ItemName );
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_DISCARD:
		{
			SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
			if(!pPlayer)		return;

			memcpy( pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max );
			APPEARANCEMGR->InitAppearance( pPlayer );
		}
		break;
	case MP_ITEM_SHOPITEM_PET_USE:
		{
			SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;

			//!!!클라이언트에서 펫생성. 또는 맵서버 생성된 펫 리턴후 생성
			//!!!해제후 아이콘 툴팁등 변화.
			AddUsedItemInfo(&pmsg->ShopItemBase);
		}
		break;
	case MP_ITEM_SHOPITEM_EFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
			if(!pPlayer)		return;

			EFFECTMGR->StartPlayerEffectProcess( pPlayer, pmsg->dwData2 );
		}
		break;
	case MP_ITEM_SHOPITEM_SLOTOVERITEM:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( pmsg->dwData > SLOT_SHOPITEM_IMSI )
			{				
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(796) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_PROTECTALL:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(729) );								
			HERO->GetShopItemStats()->ProtectCount = (char)pmsg->dwData;
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:
		{
			SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;

			VECTOR3 pos;
			pmsg->Pos.Decompress( &pos );
			if( GAMEIN->GetChaseDlg()->InitMiniMap( pmsg->MapNum, (WORD)pos.x, (WORD)pos.z, pmsg->WandtedName, pmsg->EventMapNum ) )
			{
				GAMEIN->GetChaseDlg()->SetActive( TRUE );
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(908) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			GAMEIN->GetChaseDlg()->SetActive( FALSE );

			if(pmsg->dwData == 1)
				WINDOWMGR->MsgBox( MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(907) );
			else if(pmsg->dwData == 2)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(908) );
			else if(pmsg->dwData == 3 )
				WINDOWMGR->MsgBox( MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(921) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_TRACKING:
		{
//			CHATMGR->AddMsg( CTC_TRACKING, CHATMGR->GetChatMsg(910) );
		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			WINDOWMGR->MsgBox( MBI_CHANGENAME_ACK, MBT_OK, CHATMGR->GetChatMsg(917) );

			// Item Position 확인
			for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				
				if( pItemBase.dwDBIdx == GAMEIN->GetNameChangeDlg()->GetItemDBIdx() )
				{
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case 6:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(912) );
				break;
			case 5:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(916) );
				break;
			case 4:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(913) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(914) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(915) );
				break;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_SEAL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( GAMEIN->GetSealDlg() )
				GAMEIN->GetSealDlg()->ItemSealAck();
		}
		break;
	case MP_ITEM_SHOPITEM_SEAL_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( GAMEIN->GetSealDlg() )
				GAMEIN->GetSealDlg()->ItemSealNAck();
		}
		break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			WINDOWMGR->MsgBox( MBI_JOBCHANGE_ACK, MBT_OK, CHATMGR->GetChatMsg(1143) );
			CChangeJobDialog* pDlg = GAMEIN->GetChangeJobDlg();
			if( !pDlg )		return;

			// Item Position 확인
			for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn( HERO, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				
				if( pItemBase.dwDBIdx == pDlg->GetItemDBIdx() && pItemBase.Position == pDlg->GetItemPos() )
				{
					CItem* pItem = NULL;
					DeleteItem( i+TP_SHOPINVEN_START, &pItem );
					break;
				}
			}
		}
		break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case 1:			// 화경이나 극마가 아님
				break;
			case 2:			// 아이템을 사용할 수 없음
				break;
			}

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
			GAMEIN->GetChangeJobDlg()->CancelChangeJob();
		}
		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( GAMEIN->GetReinforceResetDlg() )
				GAMEIN->GetReinforceResetDlg()->ItemResetAck();
		}
		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( GAMEIN->GetReinforceResetDlg() )
				GAMEIN->GetReinforceResetDlg()->ItemResetNAck();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1233) );
		}
		break;
	case MP_ITEM_SHOPITEM_RARECREATE_ACK:
		{
			MSG_ITEM_RAREITEM_GET* pmsg = (MSG_ITEM_RAREITEM_GET*)pMsg;
			
			CItem* pItem = NULL;
			pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->RareItemBase.Position);

			if( !pItem )
			{
				GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
				return;
			}

			ITEMBASE* pItemBase = (ITEMBASE*)pItem->GetItemBaseInfo();
			if( !pItemBase )
			{
				GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
				return;
			}

			if( pItemBase.RareIdx )
				RemoveItemRareOption( pItem->GetItemBaseInfo().RareIdx );			
            pItemBase.RareIdx = pmsg->RareInfo.dwRareOptionIdx;
			AddItemRareOption(&pmsg->RareInfo);

			//
			GAMEIN->GetRareCreateDlg()->ItemRareCreateAck();
			QUICKMGR->RefreshQickItem();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1235) );
		}
		break;
	case MP_ITEM_SHOPITEM_RARECREATE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1236) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDACK:
		{
			SEND_SHOUTRECEIVE* pmsg = (SEND_SHOUTRECEIVE*)pMsg;

			WINDOWMGR->MsgBox( MBI_SHOUT_ACK, MBT_OK, CHATMGR->GetChatMsg(905), pmsg->Receive.Time/60+1, pmsg->Receive.Count );
		}
		break;

	case MP_ITEM_SHOPITEM_SHOUT_NACK:
		{
			WINDOWMGR->MsgBox( MBI_SHOUT_NACK, MBT_OK, CHATMGR->GetChatMsg(904) );
		}
		break;
*/
	}
}
BOOL CItemManager::IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos )
{
	if(TP_INVENTORY_START <= absPos && absPos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		return (TableIdx == eItemTable_Inventory);
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return (TableIdx == eItemTable_Weared);
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return (TableIdx == eItemTable_GuildWarehouse);
	}
	else if(TP_STORAGE_START <= absPos && absPos < TP_STORAGE_END)
	{
		return (TableIdx == eItemTable_Storage);
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return (TableIdx == eItemTable_ShopInven);
	}
	else
	{
		return FALSE;
	}
}
BYTE CItemManager::GetTableIdxForAbsPos(POSTYPE absPos)
{
	if(TP_INVENTORY_START <= absPos && absPos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return eItemTable_GuildWarehouse;
	}
	else if(TP_STORAGE_START <= absPos && absPos < TP_STORAGE_END)
	{
		return eItemTable_Storage;
	}
	else if(TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END)
	{
		return eItemTable_Shop;
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return eItemTable_ShopInven;
	}
	else
	{
		return 255;
	}
/*
	if(INVENTORY_STARTPOSITION <= absPos && absPos < INVENTORY_ENDPOSITION)
	{
		return eItemTable_Inventory;
	}
	else if(WEAR_STARTPOSITION <= absPos && absPos < WEAR_ENDPOSITION)
	{
		return eItemTable_Weared;
	}
	else if(STALL_STARTPOSITION <= absPos && absPos < STALL_ENDPOSITION)
	{
		return eItemTable_StreetStall;
	}
	else if(MUNPAITEM_STARTPOSITION <= absPos && absPos < MUNPAITEM_ENDPOSITION)
	{
		return eItemTable_MunpaWarehouse;
	}
	else if(STORAGEITEM_STARTPOSITION <= absPos && absPos < STORAGEITEM_ENDPOSITION)
	{
		return eItemTable_Storage;
	}
	else
	{
		return 255;
	}
*/
}


void CItemManager::RefreshAllItem()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	
	while(pItem = m_ItemHash.GetData())
	{
		RefreshItem( pItem );

/*		if(pItem->GetItemInfo()->Category == eItemCategory_Equip)
		{
			if(CanEquip(pItem->GetItemIdx()))
			{
				pItem->SetUseParam( 0 );
				//			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
			}
			else
			{
				pItem->SetUseParam( 1 );
				//			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
			}
		}
/*		else if(pItem->GetItemKind() & eSKILL_ITEM)
		{
			if(CanConvertToSkill(pItem->GetItemIdx()))
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
				pItem->SetToolTipImageRGB( TTCLR_SKILLBOOK_CANCONVERT );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
				pItem->SetToolTipImageRGB( TTCLR_SKILLBOOK_CANNOTCONVERT );
			}
		}
*/
//		if(pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()))
//			AddToolTip(pItem, GetOption((pItem->GetDurability()));

/*		if( m_bAddPrice )
		{
			pItem->AddToolTipLine( "" );
			char buf[128];
//			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(pItem->GetItemInfo()->SellPrice) );
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			wsprintf( buf, CHATMGR->GetChatMsg(35), AddComma(SellPrice) );
			pItem->AddToolTipLine( buf, TTTC_SELLPRICE );
		}
*/	}
}

void CItemManager::RefreshItem( CItem* pItem )
{
	if(pItem->GetItemInfo()->Category == eItemCategory_Equip)
	{
		if(CanEquip(pItem->GetItemIdx()))
		{
			pItem->SetUseParam( 0 );
//			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
		}
		else
		{
			pItem->SetUseParam( 1 );
//			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
		}
	}
	else if(pItem->GetItemInfo()->Category == eItemCategory_Expend)
	{
		if(CanUse(pItem->GetItemIdx()))
		{
			pItem->SetUseParam( 0 );
		}
		else
		{
			pItem->SetUseParam( 1 );
		}
	}
/*	else if(pItem->GetItemKind() & eSKILL_ITEM)
	{
		if(CanConvertToSkill(pItem->GetItemIdx()))
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
			pItem->SetToolTipImageRGB( TTCLR_SKILLBOOK_CANCONVERT );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetToolTipImageRGB( TTCLR_SKILLBOOK_CANNOTCONVERT );
		}
	}*/

	AddToolTip( pItem );

	if( m_bAddPrice )
	{
		char buf[128];
//		wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(pItem->GetItemInfo()->SellPrice) );
		DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			pItem->AddToolTipLine( "" );
		wsprintf( buf, CHATMGR->GetChatMsg(35), AddComma(SellPrice) );
		pItem->AddToolTipLine( buf, TTTC_SELLPRICE );
	}	
}

void CItemManager::RefreshItemToolTip( DWORD ItemDBIdx )
{
	CItem* pItem = GetItem(ItemDBIdx);

	ASSERT(pItem);

	AddToolTip( pItem );
}
/*
// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2008.01.31
void CItemManager::RefreshItemToolTipByItemID(DWORD nItemID)
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	
	while(pItem = m_ItemHash.GetData())
	{
		if (pItem->GetItemIdx() == nItemID)
		{
			AddToolTip( pItem );
		}
	}
}
// E 결혼 추가 added by hseos 2008.01.31
*/
void CItemManager::SetPriceToItem( BOOL bAddPrice )
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();

	if( bAddPrice )
	{
		char buf[128];

		while(pItem = m_ItemHash.GetData())
		{
//			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(pItem->GetItemInfo()->SellPrice) );
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			pItem->AddToolTipLine( "" );
			wsprintf( buf, CHATMGR->GetChatMsg(35), AddComma(SellPrice) );
			pItem->AddToolTipLine( buf, TTTC_SELLPRICE );			
		}
	}
	else
	{
		while(pItem = m_ItemHash.GetData())
		{
			AddToolTip( pItem );
		}
	}

	m_bAddPrice = bAddPrice;
}



// 무공서적을 무공으로 변환
BOOL CItemManager::CanConvertToSkill(DWORD wItemIdx,SKILL_TYPE SkillType)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);
	if(pInfo == NULL)
	{
		ASSERT(pInfo);
		return FALSE;
	}	
/*	
	//////////////////////////////////////////////////////////////////////////	
	// YH2DO  ItemKind 에 따라 제대로 정리 필요
	if(SkillType == SKILLTYPE_NORMAL)
	{
		if(1025 <= pInfo->ItemKind && pInfo->ItemKind <= 1036)
		{
		}
		else
			return FALSE;
	}
	if(SkillType == SKILLTYPE_JINBUB)
	{
		if(1038 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(SkillType == SKILLTYPE_SIMBUB)
	{
		if(1037 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	
	
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
		return FALSE;
	}

	// 근력.민첩,심맥,체력
	if(pInfo->LimitStr > HERO->GetStrength())
	{
		return FALSE;
	}
	if(pInfo->LimitDex > HERO->GetDexterity())
	{
		return FALSE;
	}
	if(pInfo->LimitWis > HERO->GetWisdom())
	{
		return FALSE;
	}
	if(pInfo->LimitVit > HERO->GetVitality())
	{
		return FALSE;
	}

	// stage
/*	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	
*/
	return TRUE;
}
// 현재 장착 아이템만¸
BOOL CItemManager::CanEquip(DWORD wItemIdx)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);

	// 장착가능 남여
	/*
	전체:0
	남:1
	여:2
	*/
	if(pInfo->LimitRace)
	{
		if( pInfo->LimitRace != HERO->GetRace() + 1 )
		{
			return FALSE;
		}
	}

	if(pInfo->LimitGender)
	if(pInfo->LimitGender != HERO->GetGender()+1)
	{
		return FALSE;
	}
	
	// 장착가능 레벨
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
/*		switch(pInfo->ItemKind)
		{
		case 1: // 무공서
			break;
		case 2: // 장착 아이템
			break;
		default:
			break;
		}
*/
		int lev = 0;
		
		if( HERO->GetShopItemStats()->EquipLevelFree )
		{
			int gap = pInfo->LimitLevel - HERO->GetLevel();
			gap -= HERO->GetShopItemStats()->EquipLevelFree;
			if( gap > 0 )
			{
				return FALSE;
			}
		}
		else
		{			
			return FALSE;
		}
	}

	// 낚시대는 숙련등급을 비교함.
	WORD wFishingLevel = FISHINGMGR->GetFishingLevel();
	if(pInfo->WeaponType==eWeaponType_FishingPole && wFishingLevel<pInfo->kind)
	{
		return FALSE;
	}
	
/*
	// 근력.민첩,심맥,체력
	if(pInfo->LimitStr > HERO->GetStrength())
	{
		return FALSE;
	}
	if(pInfo->LimitDex > HERO->GetDexterity())
	{
		return FALSE;
	}
	if(pInfo->LimitWis > HERO->GetWisdom())
	{
		return FALSE;
	}
	if(pInfo->LimitVit > HERO->GetVitality())
	{
		return FALSE;
	}

	// 장착가능 내공
	/*
	무:0
	양:1
	음:2
	/*
	
	// 장착가능 속성
	/*
	무:0
	화:1
	수:2
	목:3
	금:4
	토:5
	독:6
	 */

	// 장착가능 경지
	// stage
/*	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	
*/
/*	switch( pInfo->ItemKind )
	{
	case eEQUIP_ITEM_TWOHANDED:
	case eEQUIP_ITEM_BOW:
	case eEQUIP_ITEM_STAFF:
		{
			CItem* pShield = GAMEIN->GetInventoryDialog()->GetItemForPos( TP_WEAR_START + eWearedItem_Shield );

			if( pShield )
			{
				GAMEIN->GetInventoryDialog()->UseItem( pShield );
			}
		}
		break;
	case eEQUIP_ITEM_SHIELD:
		{
			CItem* pWeapon = GAMEIN->GetInventoryDialog()->GetItemForPos( TP_WEAR_START + eWearedItem_Weapon );

			if( pWeapon )
			{
				switch( pWeapon->GetItemInfo()->ItemKind )
				{
				case eEQUIP_ITEM_TWOHANDED:
				case eEQUIP_ITEM_BOW:
				case eEQUIP_ITEM_STAFF:
					{
						return FALSE;
					}
					break;
				}
			}
		}
		break;
	}

*/	return TRUE;
}

BOOL CItemManager::CanUse(DWORD wItemIdx)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);


	// 미끼는 숙련등급을 비교함.
	if(pInfo->SupplyType == ITEM_KIND_FISHING_BAIT)
	{
		WORD wFishingLevel = FISHINGMGR->GetFishingLevel();

		if(wFishingLevel < pInfo->kind)
		{
			return FALSE;
		}
	}

	return TRUE;
}

ITEM_INFO * CItemManager::GetItemInfo(DWORD wItemIdx)
{
	return m_ItemInfoList.GetData(wItemIdx);
}

#define TOOLTIPINFO_LEN		24

void CItemManager::LoadItemToolTipList()
{
	CMHFile file;
	// 070116 LYW --- Modified bin file name.
	//if( !file.Init( "System/Resource/TooltipInfo.bin", "rb" ) )
	if( !file.Init( "Data/Interface/Windows/ToolTipMsg.bin", "rb" ) )
		return;

	char buf[512];	//툴팁의 길이제한...22		//256까지 지원하고..
	TOOLTIP_TEXT* pToolTipText;
	TOOLTIP_TEXT* pTemp;
	DWORD dwIndex;
	int nCpyNum = 0;
	int nLen;
	char* str;
	while( 1 )
	{
		if( file.IsEOF() ) break;

		pToolTipText = NULL;

//		file.GetString( buf );
		SafeStrCpy( buf, file.GetString(), 512 );

		if( strcmp( buf, "#Msg" ) == 0 )
		{
			dwIndex = file.GetDword();	//msg index
//			ASSERT( dwIndex >= 0 && dwIndex <= 10000 );	//index가 올바른가?//confirm

			//file.GetString( buf );
			SafeStrCpy( buf, file.GetString(), 512 );
			
			if( strcmp( buf, "{" ) == 0 )
			{
				file.GetLine( buf, 512 );
				str = buf;
				nLen = strlen( buf );

				while( *buf != '}' )
				{
					while( nLen > TOOLTIPINFO_LEN )
					{
						if( ( str + TOOLTIPINFO_LEN ) != CharNext( CharPrev( str, str + TOOLTIPINFO_LEN ) ) )
							nCpyNum = TOOLTIPINFO_LEN - 1;
						else
							nCpyNum = TOOLTIPINFO_LEN;
						
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nCpyNum+1];
						strncpy( pTemp->strToolTip, str, nCpyNum );
						pTemp->strToolTip[nCpyNum] = 0;
						nLen -= nCpyNum;
						str += nCpyNum;
						if( *str == ' ' ) ++str;

						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );

						pToolTipText = pTemp;
					}

					if( nLen > 0 )
					{
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nLen+1];
						strncpy( pTemp->strToolTip, str, nLen );
						pTemp->strToolTip[nLen] = 0;

						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );

						pToolTipText = pTemp;
					}

					file.GetLine( buf, 512 );
					str = buf;
					nLen = strlen( buf );
				}
			}
		}		
	}	
}

TOOLTIP_TEXT* CItemManager::GetItemToolTipInfo( DWORD wIdx )
{
	return m_ItemToolTipTextList.GetData( wIdx );
}

void CItemManager::LoadItemList()
{
#ifdef _DEBUG
	const DWORD beginTime = GetTickCount();	
#endif
	{
		CMHFile file;

		if( ! file.Init("System/Resource/ItemList.bin", "rb" ) )
		{
			return;
		}

		while( ! file.IsEOF() )
		{
			ITEM_INFO* pInfo = new ITEM_INFO;
			ASSERT( pInfo );

			pInfo->ItemIdx = file.GetDword();

			// desc_hseos_ASSERT수정_01
			// S ASSERT수정 추가 added by hseos 2007.05.22
			// ..bin파일에 아이템ID 가 중복되는 경우가 있다. 디버그 모드일 떄는 바로 에러 메세지 보여주도록..
#if defined(_DEBUG)
			if (m_ItemInfoList.GetData(pInfo->ItemIdx))
			{
				char szText[256];
				sprintf(szText, "이미 존재하는 아이템ID: %d", pInfo->ItemIdx);
				char	szCaption[512];														// 아래 두줄 살짝 추가. -_-; 2007.05.28 이진영.
				sprintf( szCaption, "%s, %s() %d", __FILE__, __FUNCTION__, __LINE__); 
				MessageBox(NULL, szText, szCaption, NULL);
			}
#endif
			// E ASSERT수정 추가 added by hseos 2007.05.22

			SafeStrCpy( pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1 );

			pInfo->ItemTooltipIdx = file.GetDword();
			pInfo->Image2DNum = file.GetWord();	
			pInfo->Part3DType = file.GetWord();
			pInfo->Part3DModelNum = file.GetWord();	

			pInfo->Shop = file.GetWord(); // 아이템몰 여부
			pInfo->Category = file.GetWord();			

			pInfo->Grade = file.GetWord();			

			pInfo->LimitRace = file.GetWord();
			pInfo->LimitGender = file.GetWord();		
			pInfo->LimitLevel = file.GetLevel();	

			pInfo->Stack = file.GetWord();
			pInfo->Trade = file.GetWord();
			pInfo->Deposit = file.GetWord();
			pInfo->Sell = file.GetWord();
			pInfo->Decompose = file.GetWord();

			pInfo->DecomposeIdx = file.GetDword();
			pInfo->Time = file.GetDword();

			pInfo->BuyPrice = file.GetDword();			
			pInfo->SellPrice = file.GetDword();			

			pInfo->EquipType = file.GetWord();
			pInfo->EquipSlot = file.GetWord() - 1;

			pInfo->Durability = file.GetDword();
			file.GetDword();

			pInfo->Repair = file.GetWord();

			pInfo->WeaponAnimation = file.GetWord();
			pInfo->WeaponType = file.GetWord();		

			pInfo->PhysicAttack = file.GetInt();
			pInfo->MagicAttack = file.GetInt();

			pInfo->ArmorType = file.GetWord();
			pInfo->AccessaryType = file.GetWord();

			pInfo->PhysicDefense = file.GetInt();
			pInfo->MagicDefense = file.GetInt();

			pInfo->IsEnchant = file.GetBool();
			pInfo->Improvement = file.GetWord();

			pInfo->EnchantValue		= file.GetDword();
			pInfo->EnchantDeterm	= file.GetDword();

			pInfo->ImprovementStr = file.GetInt();			
			pInfo->ImprovementDex = file.GetInt();			
			pInfo->ImprovementVit = file.GetInt();		//10
			pInfo->ImprovementInt = file.GetInt();
			pInfo->ImprovementWis = file.GetInt();			
			pInfo->ImprovementLife = file.GetInt();			
			pInfo->ImprovementMana = file.GetInt();			

			pInfo->ImprovementSkill = file.GetDword();
			pInfo->Skill		= file.GetDword();
			pInfo->SupplyType	= file.GetWord();			
			pInfo->SupplyValue	= file.GetDword();
			pInfo->kind			= ITEM_INFO::eKind( file.GetDword() );

			pInfo->RequiredBuff = file.GetDword();
			pInfo->Battle = file.GetBool();
			pInfo->Peace = file.GetBool();
			pInfo->Move = file.GetBool();
			pInfo->Stop = file.GetBool();
			pInfo->Rest = file.GetBool();

			// 071121 LYW - Modified.
			pInfo->wSeal = file.GetWord() ;
			pInfo->nTimeKind = file.GetInt() ;
			pInfo->dwUseTime = file.GetDword() ;

			//// 080415 NYJ --- 낚시 포인트 추가
			pInfo->dwBuyFishPoint = file.GetDword() ;
		//	pInfo->EquipSlot = file.GetWord() - 1;

			// desc_hseos_농장시스템_01
			// S 농장시스템 추가 added by hseos 2007.10.24
			// ..농작물 아이템의 이미지를 저장하기 위해
			if (pInfo->SupplyType == ITEM_KIND_FARM_PLANT)
			{
				static BOOL bInit = TRUE;
				if (bInit)
				{
					CSHFarmManageGardenDlg::SetCropImgInfo(0, NULL, NULL);
					bInit = FALSE;
				}
				CSHFarmManageGardenDlg::SetCropImgInfo(1, pInfo->SupplyValue, pInfo->Image2DNum);
			}
			// E 농장시스템 추가 added by hseos 2007.10.24

			if (pInfo->SupplyType == ITEM_KIND_ANIMAL)
			{
				static BOOL bInit = TRUE;
				if( bInit)
				{
					//CAnimalDialog::SetAnimalImgInfo(0, NULL, NULL);
					GAMEIN->GetAnimalDialog()->SetAnimalImgInfo(0, NULL, NULL);
					bInit = FALSE;
				}
				GAMEIN->GetAnimalDialog()->SetAnimalImgInfo(1, pInfo->SupplyValue, pInfo->Image2DNum);
			}

			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
	}
	
	//{
	//	CMHFile file;

	//	if(!file.Init("System/Resource/HideItemLock.bin", "rb"))
	//	{
	//		return;
	//	}

	//	while( ! file.IsEOF() )
	//	{
	//		WORD Index = file.GetWord();
	//		ITEM_INFO* pInfo = m_ItemInfoList.GetData(Index);

	//		if(pInfo)
	//		{
	//			m_UnfitItemInfoListForHide.Add(pInfo, pInfo->ItemIdx);
	//		}
	//	}
	//}
#ifdef _DEBUG
	char line[ MAX_TEXT_SIZE ];
	const float second = float( GetTickCount() - beginTime ) / 1000.0f;

	sprintf( line, "itemList.bin: cost time to parse: %0.5f sec\n", second );

	OutputDebugStr( line );
#endif
}

/*
BOOL CItemManager::CanGradeUp(CItem * pItem)
{
	eITEM_KINDBIT bits = pItem->GetItemKind();
	if(bits & eEQUIP_ITEM)
	{
		if(m_dwStateParam == 50700)
		{
			// 무기
			if((bits >= eEQUIP_ITEM_WEAPON && bits < eEQUIP_ITEM_WEAPON+6))
			{
				return TRUE;
			}

		}
		else if(m_dwStateParam == 50701)
		{
			// 방어구  
			if(bits == eEQUIP_ITEM_DRESS ||
				bits == eEQUIP_ITEM_HAT ||
				bits == eEQUIP_ITEM_SHOES)
			{
				return TRUE;
			}
		}
		
	}
	return FALSE;
}*/

BOOL CItemManager::IsDupItem( DWORD wItemIdx )
{
	const ITEM_INFO* info = GetItemInfo( wItemIdx );

	return info ? info->Stack : 0;
}


CItem* CItemManager::GetItemofTable(WORD TableIDX, POSTYPE absPos)
{
	CItem * pItem = NULL;
	switch(TableIDX)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Storage:
		{
			CStorageDialog * pDlg = GAMEIN->GetStorageDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	}
	return pItem;
}

// yunho StatsCalcManager 공유를 위해
const ITEMBASE* CItemManager::GetItemInfoAbsIn( CPlayer*, POSTYPE absPos)
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);

	if(TableIdx == 255)
	{
		return NULL;
	}

	CItem* pItem = GetItemofTable(TableIdx,absPos);
	
	return pItem ? &( pItem->GetItemBaseInfo() ) : 0;
}

void CItemManager::SetDisableDialog(BOOL val, BYTE TableIdx)
{
	switch(TableIdx)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Storage:
		{
			CStorageDialog * pDlg = GAMEIN->GetStorageDialog();
			if( pDlg )
				pDlg->SetDisable( val );			
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Deal:
		{
			CDealDialog * pDlg = GAMEIN->GetDealDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog(); 
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	default:
		{
			ASSERTMSG(0, "테이블 인덱스가 벗어났음다.DisableDlg()-Taiyo. Invalid Table Index.");
			break;
		}
	}
}

void CItemManager::DivideItem(ITEMOBTAINARRAY * pMsg )
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
	
	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);

		pItem = MakeNewItem(pmsg->GetItem(i),"DivideItem");	
	//	BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if(!AddItem(pItem))
		{
			ASSERT(0);
		}
	}
}

BOOL CItemManager::AddItem(CItem* pItem)
{
	ASSERT( pItem );

	switch( GetTableIdxForAbsPos( pItem->GetPosition() ) )
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		return GAMEIN->GetInventoryDialog()->AddItem(pItem);
	case eItemTable_Storage:
		return GAMEIN->GetStorageDialog()->AddItem(pItem);
	case eItemTable_GuildWarehouse:
		return GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);
	}
	
	return FALSE;
}

ITEM_INFO* CItemManager::FindItemInfoForName( char* strName )
{
	m_ItemInfoList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;
	while( pInfo = m_ItemInfoList.GetData() )
	{
		if( strcmp( strName, pInfo->ItemName ) == 0 )
		{
			return pInfo;
		}
	}

	return NULL;
}

void CItemManager::ReLinkQuickPosition(CItem* pItem)
{
	if(IsDupItem(pItem->GetItemIdx()))
	{
		QUICKMGR->RefreshQickItem();
	}
}

void CItemManager::SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count)
{
	ITEM_INFO* pItemInfo = NULL;
	int ItemIndex = 0;

	m_ItemInfoList.SetPositionHead();
	while( pItemInfo = m_ItemInfoList.GetData() )
	{
		for(int i=0; i<Count; i++)
		{
			if(pItemInfo->LimitLevel == Level[i])
			{
				// 나머지가 있으면 기본무기에 +가 붙은 아이템이다. 그래서 패스~
				if( pItemInfo->ItemIdx%10 )			continue;
				
				ItemIndex = pPreLoadInfo->Count[ePreLoad_Item];
				pPreLoadInfo->Kind[ePreLoad_Item][ItemIndex] = pItemInfo->ItemIdx;
				++pPreLoadInfo->Count[ePreLoad_Item];
				
				// Max를 넘어가면 그마안~~
				if(pPreLoadInfo->Count[ePreLoad_Item] >= MAX_KIND_PERMAP)		return;
			}
		}

		/*
		if(pItemInfo->LimitLevel == Level)					
		{
			// 나머지가 있으면 기본무기에 +가 붙은 아이템이다. 그래서 패스~
			if( pItemInfo->ItemIdx%10 )			continue;

			ItemIndex = pPreLoadInfo->Count[ePreLoad_Item];
			pPreLoadInfo->Kind[ePreLoad_Item][ItemIndex] = pItemInfo->ItemIdx;
			++pPreLoadInfo->Count[ePreLoad_Item];

			// Max를 넘어가면 그마안~~
			if(pPreLoadInfo->Count[ePreLoad_Item] >= MAX_KIND_PERMAP)		break;
		}
		*/
	}
}

void CItemManager::ItemDropEffect( DWORD wItemIdx )
{
	BYTE kind = ( BYTE )( GetItemInfo(wItemIdx)->kind );
	WORD Category = GetItemInfo(wItemIdx)->Category;
	WORD Equip = GetItemInfo(wItemIdx)->EquipType;

	switch( kind )
	{
	case ITEM_INFO::eKindRare:
	case ITEM_INFO::eKindSetRare:
		{
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Rare);
		}
		break;
	case ITEM_INFO::eKindUnique:
	case ITEM_INFO::eKindSetUnique:
		{
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Unique);
		}
		break;
	}

	if( Category == eItemCategory_Equip )
	{
		switch( Equip )
		{
		case eEquipType_Weapon:
			{
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Weapon);
			}
			break;
		case eEquipType_Armor:
			{
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Dress);
			}
			break;
		}
	}
	else
	{
		EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Youngyak);
	}
}

void CItemManager::MoneyDropEffect()
{
	EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
}




// 071121 LYW - Modified.
void CItemManager::AddUsedItemInfo(ITEMBASE* pInfo)
{
	/*if(!pInfo) return ;
	if( pInfo->dwDBIdx == 0 ) return ;

	CItem * item = NULL ;
	item = GetItemofTable( eItemTable_Inventory, pInfo->Position );
	if( !item ) return ;
    
	AddToolTip( item ) ;*/
}


//void CItemManager::AddUsedItemInfo(SHOPITEMBASE* pInfo)
//{
//	if(!pInfo)		return;
//	if( pInfo->ItemBase.dwDBIdx == 0 )		return;
//	
//	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
////	if( !pItemInfo || (!(pItemInfo->ItemKind & eSHOP_ITEM)) )
//		return;
///*	
//	SHOPITEMBASE* pShopItemBase = NULL;
//	pShopItemBase = m_UsedItemList.GetData(pInfo->ItemBase.wIconIdx );
//	if( pShopItemBase )
//	{
//		// 개인플러스타임체크
//		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
//		{
//			SAFE_DELETE( pShopItemBase );
//			m_UsedItemList.Remove( pItemInfo->ItemIdx );
//		}
//		else
//			return;
//	}
//	
//	pShopItemBase = new SHOPITEMBASE;
//	memcpy(pShopItemBase, pInfo, sizeof(SHOPITEMBASE));
//	m_UsedItemList.Add(pShopItemBase, pShopItemBase.ItemBase.wIconIdx);
//
//	// 개인플러스타임체크
//	if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
//	{
//		if( pShopItemBase.Remaintime == 0 )
//			return;
//	}
//	//
//
//	if( pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->BuyPrice )
//		STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pShopItemBase.ItemBase.wIconIdx);
//	
//	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
//	if( pShopItemBase.Remaintime <= MINUTETOSECOND*1000 )
//		STATUSICONDLG->SetOneMinuteToShopItem( pShopItemBase.ItemBase.wIconIdx );
//
//	// 아바타 아이템은 툴팁을 따로 설정
//	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
//	{
//		AddUsedAvatarItemToolTip( pInfo );
//	}
//	else if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
//	{
//		AddUsedShopEquipItemToolTip( pInfo );
//	}
//	// 기간제 툴팁 따로 설정
//	else if( pItemInfo->ItemType == 11 )
//	{
//		CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
//		if( pItem )
//			AddToolTip( (cIcon*)pItem );
//	}
//*/
//}

// 071121 LYW - Modified.
//void CItemManager::RemoveUsedItemInfo(DWORD wIndex, DWORD dwDBIdx)
//{
//	/*ITEMBASE* pItemBase = NULL ;
//	pItemBase = m_TimeItemList.GetData(wIndex) ;
//
//	if( !pItemBase ) return ;
//
//	if( dwDBIdx <= 0 ) return ;
//
//	if( pItemBase.dwDBIdx != dwDBIdx )	return ;
//	
//	SAFE_DELETE(pItemBase) ;
//	m_TimeItemList.Remove(wIndex) ;*/
//}

//void CItemManager::RemoveUsedItemInfo(DWORD wIndex, DWORD dwDBIdx)
//{
//	SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(wIndex);
//	if( !pShopItemBase )		return;
//	if( dwDBIdx != 0 && pShopItemBase.ItemBase.dwDBIdx != dwDBIdx )		return;
//	
//	SAFE_DELETE(pShopItemBase);
//	m_UsedItemList.Remove(wIndex);
//
//	ITEM_INFO* pItemInfo = GetItemInfo(wIndex);	
////	if( pItemInfo &&  pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->BuyPrice )
////		STATUSICONDLG->RemoveIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), wIndex);
//}


//// 071121 LYW - Modified.
//ITEMBASE* CItemManager::GetUsedItemInfo(DWORD Index)
//{
//	return m_TimeItemList.GetData(Index) ;
//}

//SHOPITEMBASE* CItemManager::GetUsedItemInfo(DWORD Index)
//{
//	return m_UsedItemList.GetData(Index);
//}


void CItemManager::Process()
{
	ProcessCoolTime();

	if( m_GiftEventTime )
	{
		if( gCurTime - m_GifeEventCheckTime < m_GiftEventTime )
		{
			m_GiftEventTime -= ( gCurTime - m_GifeEventCheckTime );

			CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg();

			pDlg->SetGiftEventTime( m_GiftEventTime );
		}
	}

	m_GifeEventCheckTime = gCurTime;
}



//// 071121 LYW - Modified.
//void CItemManager::DeleteShopItemInfo()
//{
//	ITEMBASE* pItemBase = NULL ;
//
//	m_TimeItemList.SetPositionHead() ;
//
//	while(	pItemBase = m_TimeItemList.GetData() )
//	{
//		RemoveUsedItemInfo( pItemBase.wIconIdx, pItemBase.dwDBIdx ) ;
//	}
//}


//void CItemManager::DeleteShopItemInfo()
//{
//	SHOPITEMBASE* pShopItem = NULL;
//
//	m_UsedItemList.SetPositionHead();
//
//	while(	pShopItem = m_UsedItemList.GetData() )
//	{
//		RemoveUsedItemInfo( pShopItem->ItemBase.wIconIdx, pShopItem->ItemBase.dwDBIdx );
//	}
//}


void CItemManager::ReviveOtherOK()
{
}


void CItemManager::ReviveOtherSync()
{
	CObject* pObject = OBJECTMGR->GetSelectedObject();
	if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
	{
		SetDisableDialog(FALSE, eItemTable_Inventory);
		SetDisableDialog(FALSE, eItemTable_Storage);
		SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		SetDisableDialog(FALSE, eItemTable_Shop);

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
		return;
	}
	/*
	if( MOVEMGR->CalcDistance( HERO, pObject ) > REVIVE_OTHER_DIST )
	{
		SetDisableDialog(FALSE, eItemTable_Inventory);
		SetDisableDialog(FALSE, eItemTable_Storage);
		SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		SetDisableDialog(FALSE, eItemTable_Shop);

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(764) );
		return;
	}
	*/

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();
	if( !pItem )
	{
		SetDisableDialog(FALSE, eItemTable_Inventory);
		SetDisableDialog(FALSE, eItemTable_Storage);
		SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		SetDisableDialog(FALSE, eItemTable_Shop);
		
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		return;
	}

	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = OBJECTMGR->GetSelectedObject()->GetID();	
	msg.dwData2 = pItem->GetItemIdx();
	msg.dwData3 = pItem->GetPosition();
	NETWORK->Send( &msg, sizeof(msg) );

	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}


void CItemManager::ReviveOtherCancel()
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}


void CItemManager::ReviveOtherConfirm( BOOL bRevive )
{
	if( GAMEIN->GetInventoryDialog()->GetReviveData().TargetID == 0 )
		return;

	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = GAMEIN->GetInventoryDialog()->GetReviveData().TargetID;
	msg.dwData2 = eShopItemErr_Revive_Refuse;
	
	if( bRevive )
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_ACK;	
	else
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;

	NETWORK->Send( &msg, sizeof(msg) );

	//
	GAMEIN->GetInventoryDialog()->SetReviveData( 0, 0, 0 );
}

void CItemManager::FakeDeleteItem(CItem* pItem)
{
	m_TempDeleteItem.Durability = pItem->GetDurability();
	m_TempDeleteItem.Position = pItem->GetPosition();
	m_TempDeleteItem.wIconIdx = pItem->GetItemIdx();
}

void CItemManager::SendDeleteItem()
{
	if( m_TempDeleteItem.wIconIdx == 0 ) return;

	CItem* pItem = GetItemofTable(GetTableIdxForAbsPos(m_TempDeleteItem.Position), m_TempDeleteItem.Position);
	if((pItem == 0) || (pItem->GetItemIdx() != m_TempDeleteItem.wIconIdx))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(153) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	// 071127 LYW --- ItemManager : 삭제 아이템 ( 봉인 관련 ) 위치 저장 변수 추가.
	m_nSealItemDelPos = m_TempDeleteItem.Position ;
			
	MSG_ITEM_DISCARD_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_DISCARD_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos =m_TempDeleteItem.Position;
	msg.wItemIdx = m_TempDeleteItem.wIconIdx;
	msg.ItemNum = m_TempDeleteItem.Durability;

	NETWORK->Send(&msg,sizeof(msg));
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
}

// 060911 KKR 
// 샵 아이템 사용시간 뿌리기 
void CItemManager::PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo )
{
	// 사용기간 표시.
	pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );

	// 현재시간 계산되는것
	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	// 시간제한 없음
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1443), TTTC_QUESTITEM );
	}
	// 플레이시간 
	else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	// 사용해 놓고 나중에 효과가 발동되는것 
	else if( eShopItemUseParam_Continue )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
}

// 071121 LYW - Modified.
//void CItemManager::AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo )
void CItemManager::AddUsedAvatarItemToolTip( ITEMBASE* pInfo )
{
	//if( !pInfo )		return;

	//ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	//if( !pItemInfo )	return;

	//CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	//if( !pItem )		return;
	//stTIME time;
	//time.value = pInfo->Remaintime;
	//
	//// 초기화
	//cImage imgToolTip;
	//SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );		
	//pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	//
	//// 아이템 이름
	//char buf[256] = { 0, };
	//wsprintf( buf, "[%s]", pItemInfo->ItemName );
	//pItem->AddToolTipLine( buf );

	//// 060908 KKR
	//AddShopItemToolTip( (cIcon*)pItem, pItemInfo );
	//
	//// 아이템 옵션  05.02.17
	////AddAvatarItemOptionTooltip( (cIcon*)pItem, pItemInfo );
	//
	//
	//
	//// 아이템 설명
	//AddDescriptionToolTip( pItem, pItemInfo->ItemTooltipIdx );
	//
	//// 종료시간
	//pItem->AddToolTipLine("");
	//pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );
	//
	//// 현재시간 계산되는것
	//if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	//{
	//	wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );			
	//	pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	//}
	//// 시간제한 없음
	//else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	//{
	//	pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
	//}
}

void CItemManager::AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo )
{
	//CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	//ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	//if( !pItem || !pItemInfo )		return;

	//// 초기화
	//cImage imgToolTip;
	//SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );		
	//pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	//// KKR
	//AddEquipItemToolTip( pItem, pItemInfo, NULL/*, NULL*/ );
	////AddShopItemToolTip( (cIcon*)pItem, pItemInfo );

	//char buf[256]= { 0, };
	//stTIME time;
	//time.value = pInfo->Remaintime;
	//
	//// 종료시간
	//pItem->AddToolTipLine("");
	//pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

	//// 현재시간 계산되는것
	//if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	//{
	//	wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );			
	//	pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	//}
	//// 시간제한 없음
	//else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	//{
	//	pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
	//}
	
}

//// 071121 LYW - Modified.
//void CItemManager::RefreshStatsBuffIcon()
//{
//	/*ITEMBASE* pItemBase = NULL ;
//
//	m_TimeItemList.SetPositionHead() ;
//
//	while( pItemBase = m_TimeItemList.GetData() )
//	{
//		ITEM_INFO* pItemInfo = NULL ;
//		pItemInfo = GetItemInfo( pItemBase.wIconIdx ) ;
//
//		if( !pItemInfo) continue ;
//	}*/
//}

//void CItemManager::RefreshStatsBuffIcon()
//{
//	SHOPITEMBASE* pItemBase = NULL;
//
//	m_UsedItemList.SetPositionHead();
//
//	while( pItemBase = m_UsedItemList.GetData() )
//	{
//		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase.ItemBase.wIconIdx );
//		if( !pItemInfo)		continue;
//
////		if(  pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->BuyPrice )
////			STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pItemBase.ItemBase.wIconIdx);
//	}
//}


void CItemManager::AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{

}
/*
BOOL CItemManager::CheckItemStage( BYTE bItemStage )
{
	// stage
	if( bItemStage == eItemStage_Normal )		return TRUE;
	
	BYTE stage = HERO->GetStage();
	if( stage == eStage_Normal )				return FALSE;
	if( bItemStage == eItemStage_ChangeStage )	return TRUE;
	
	switch( stage )
	{
	case eStage_Hwa:
		{
			if( bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hwa )
				return TRUE;
		}
		break;
	case eStage_Hyun:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hyun )
				return TRUE;
		}
		break;
	case eStage_Geuk:
		{
			if( bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Geuk )
				return TRUE;
		}
		break;
	case eStage_Tal:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Tal )
				return TRUE;
		}
		break;
	}

	return FALSE;
}
*/
//BOOL CItemManager::LoadRareItemInfo()
//{
//	CMHFile fp;
//
//	char szBuf[256] = {0,};
//	char line[512];
//
//	char FileName[256];
//
//	sprintf(FileName, "System/Resource/Item_RareItemInfo.bin");
//	if( !fp.Init(FileName,"rb") ) return FALSE;
//
//	while( !fp.IsEOF() )
//	{
//		fp.GetString(szBuf);
//
//		if( szBuf[0] == '@' )
//		{
//			fp.GetLineX(line,512);
//			continue;
//		}
//
//		//		strupr(szBuf);
//
//		if( szBuf[0] == '*' )
//		{
//			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
//			pRareItemInfo->ItemIdx = fp.GetDword();
//			pRareItemInfo->RareProb = fp.GetDword();
//
//			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
//			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
//		}
//	}
//
//	return TRUE;
//}

void CItemManager::SetIconColorFilter( DWORD dwDBIdx, DWORD clrRGBA )
{
	CItem* pItem = GetItem(dwDBIdx);
	pItem->SetImageRGB(clrRGBA);
}

void CItemManager::SetIconColorFilter( cIcon* pIcon, DWORD clrRGBA )
{
	pIcon->SetImageRGB(clrRGBA);
}


void CItemManager::Item_TotalInfo_Local( void* pMsg )
{
	if( !pMsg ) return ;

	ITEM_TOTALINFO * msg = (ITEM_TOTALINFO *)pMsg;

	for(int i = 0 ; i < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )) ; i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&msg->Inventory[i]);
		if(msg->Inventory[i].QuickPosition != 0)
		{
			if( EVENTMAP->IsEventMap() )
			{
				ITEM_INFO* pItemInfo = GetItemInfo(msg->Inventory[i].wIconIdx);
				//if( pItemInfo )
				//if( pItemInfo->ItemKind != eYOUNGYAK_ITEM )
					// 06. 01 이영준 - 단축창 변경
					// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
					//QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition,msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
			}
			else
			{
				// 06. 01 이영준 - 단축창 변경
				// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
				//QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition,msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
			}
		}
	}
	//for(i = 0 ; i < SLOT_SHOPINVEN_NUM/2 ; i++)
	//{
	//	GAMEIN->GetInventoryDialog()->AddItem(&msg->ShopInventory[i]);
	//	if(msg->ShopInventory[i].QuickPosition != 0)
	//	{
	//		// 06. 01 이영준 - 단축창 변경
	//		// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
	//		//QUICKMGR->SetQuickItemReal(msg->ShopInventory[i].QuickPosition,msg->ShopInventory[i].Position, msg->ShopInventory[i].wIconIdx);
	//	}
	//}
	for(i = 0 ; i < eWearedItem_Max ; i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&msg->WearedItem[i]);
		if(msg->WearedItem[i].QuickPosition != 0)
		{
			// 06. 01 이영준 - 단축창 변경
			// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
			//QUICKMGR->SetQuickItemReal(msg->WearedItem[i].QuickPosition,msg->WearedItem[i].Position,msg->WearedItem[i].wIconIdx);
		}
	}

	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_Storage_Item_Info_Ack( void* pMsg )
{
	SEND_STORAGE_ITEM_INFO * pmsg = (SEND_STORAGE_ITEM_INFO*)pMsg;

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				ITEM_OPTION OptionInfo[SLOT_STORAGE_NUM];
				iter.GetInfoData(&OptionInfo);
				
				for( WORD i = 0; i < pmsg->wOptionCount; ++i )
				{
					ITEM_OPTION& option = OptionInfo[ i ];

					AddOption( option );
				}
			}
			break;
		}
		iter.ShiftToNextData();
	}

	for(int i=0; i<SLOT_STORAGE_NUM; ++i)
	{
		if(pmsg->StorageItem[i].dwDBIdx != 0)
			GAMEIN->GetStorageDialog()->AddItem(&pmsg->StorageItem[i]);
	}
	GAMEIN->GetStorageDialog()->SetStorageMoney(pmsg->money);		
	
	GAMEIN->GetStorageDialog()->SetItemInit(TRUE);
	// 070326 LYW --- GlobalEventFunc : Modified active function.
	/*
	GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	GAMEIN->GetStorageDialog()->SetActive(TRUE);
	*/
	GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;
	if(HERO->GetStorageNum() == 0)
	{

	}
	else
	{
		GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1);
		GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1);
		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// 스크립트 다이얼로그를 비활성화 한다.
	}
}

void CItemManager::Item_Reinforce_Option_Ack( void* pMsg )
{
	const MSG_ITEM_REINFORCE_OPTION_ACK* message = ( MSG_ITEM_REINFORCE_OPTION_ACK* )pMsg;
	
	AddOption( message->mOption );

	{
		CReinforceDlg* dialog = GAMEIN->GetReinforceDialog();
		ASSERT( dialog );

		dialog->Succeed( *message );
	}

	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_ReInForce_Ack( void* pMsg )
{
	const MSG_ITEM_REINFORCE_ACK*	message		= ( MSG_ITEM_REINFORCE_ACK* )pMsg;
	
	if( MP_ITEM_REINFORCE_FAILED_ACK == message->Protocol )
	{
		CReinforceDlg* dialog = GAMEIN->GetReinforceDialog();
		ASSERT( dialog );

		dialog->Fail();
	}
	// 071226 LUJ, 강화 옵션이 붙을 때까지만 달아놓자...
	else
	{
		CItem* item = GetItem( message->mSourceItemDbIndex );

		if( item )
		{
			// 080108 LUJ, 서버 응답이 올때까지 다시 강화할 수 없게 한다
			item->SetLock( TRUE );
		}
	}

	CItem*				deletedItem		= 0;
	CInventoryExDialog*	inventoryDialog = GAMEIN->GetInventoryDialog();
	ASSERT( inventoryDialog );

	// 아이템 업데이트
	for( DWORD i = 0; i < message->mSize; ++i )
	{
		const ITEMBASE& updatedItem = message->mItem[ i ];

		CItem* item	= GetItem( updatedItem.dwDBIdx );

		if( item )
		{
			if( updatedItem.wIconIdx )
			{
				cImage image;

				item->SetBasicImage( GetIconImage( updatedItem.wIconIdx, &image ) );
				item->SetItemBaseInfo( updatedItem );
				item->SetVisibleDurability( IsDupItem( updatedItem.wIconIdx ) );
				item->SetData( updatedItem.wIconIdx );

				AddToolTip( item );
			}
			else
			{
				DeleteItem( item->GetPosition(), &deletedItem );

				//inventoryDialog->DeleteItem( updatedItem.Position, &deletedItem );
			}

			item->SetLock( FALSE );
		}
		// 071218 LUJ,	클라이언트에 없는 아이템이면 임시에 생성해 놓고, DB 인덱스가 서버에서 올때까지 잠궈놓는다
		//				DB랙으로 인해 아이템이 사라졌다고 오해하거나, 해당 공간을 사용하는 것을 막기 위함이다
		else
		{
			CItem* item = new CItem( &updatedItem );

			mPreIconList.push_back( item );

			cImage image;

			item->Init(
				0,
				0,
				DEFAULT_ICONSIZE,
				DEFAULT_ICONSIZE,
				GetIconImage( updatedItem.wIconIdx, &image ),
				m_IconIndexCreator.GenerateIndex() );

			item->SetVisibleDurability( IsDupItem( updatedItem.wIconIdx ) );
			item->SetLock( TRUE );
			item->SetData( updatedItem.wIconIdx );

			inventoryDialog->AddItem( item );
		}
	}
}


void CItemManager::Item_ReInForce_Nack( void* pMsg )
{
	{
		CReinforceDlg* dialog = GAMEIN->GetReinforceDialog();
		ASSERT( dialog );

		dialog->Restore();
	}

	const MSG_ITEM_ERROR* message = ( MSG_ITEM_ERROR* )pMsg;

	switch( message->ECode )
	{
	case eLog_ItemReinforceDataHaveTooManyMaterials:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Data is too big" );
			break;
		}
	case eLog_ItemReinforceMaterialIsInvalid:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Material data is invalid" );
			break;
		}
	case eLog_ItemReinforceWhichMaterialHasZero:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Some material has zero" );
			break;
		}
	case eLog_ItemReinfoceYouCannotOver100ToReinforce:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: You cannot over 100 materials to reinforce" );
			break;
		}
	case eLog_ItemReinforceSourceItemNotExist:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Source item not exist" );
			break;
		}
	case eLog_ItemRerinfoceServerHasNoReinforceScript:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Server has no reinforce script" );
			break;
		}
	case eLog_ItemReinforceMaterialSourceItemCannnotReinforce:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Source item cannot reinforce" );
			break;
		}
	case eLog_ItemReinforceSourceItemCanDuplicate:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Source item can duplicate" );
			break;
		}
	case eLog_ItemReinforceRemovingMaterialIsFailed:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Removing material is failed" );
			break;
		}
	case eLog_ItemReinforceUpdatingItemIsFailed:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Update item is failed" );
			break;
		}
	default:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Undefined result detected" );
			break;
		}
	}
}


void CItemManager::Item_Move_Ack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	MoveItem( (MSG_ITEM_MOVE_ACK *)pMsg );			
	
	WINDOWMGR->DragWindowNull();
}

void CItemManager::Item_Pet_Move_Ack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	PetMoveItem( (MSG_ITEM_MOVE_ACK *)pMsg );			
	
	WINDOWMGR->DragWindowNull();
}

void CItemManager::Item_Combine_Ack( void* pMsg )
{
	CombineItem( (MSG_ITEM_COMBINE_ACK *)pMsg );	

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	
	WINDOWMGR->BackDragWindow();
}


void CItemManager::Item_Divide_Ack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_ShopInven);

	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		//---KES ItemDivide Fix 071020
		//---Comment
		/*
		pPreItem = NULL;
		DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
		*/
		//----------------------------

		pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_DIVIDE_ACK");	
		if(!AddItem(pItem))
		{
			ASSERT(0);
		}
	}
			
	//---KES ItemDivide Fix 071020
	GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd( GetDivideItemInfoPos(), -GetDivideItemInfoDur() );
	//----------------------------

	// QuickDur다시 계산
	QUICKMGR->RefreshQickItem();

	WINDOWMGR->DragWindowNull();
}


void CItemManager::Item_Monster_Obtain_Notify( void* pMsg )
{
	ITEMOBTAINARRAY *	pmsg	= (ITEMOBTAINARRAY *)pMsg;
	CItem*				pItem	= NULL;
		
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		ITEMBASE& itemBase = pmsg->ItemInfo[ i ];
		
		pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( itemBase.Position );
		
		ITEMBASE prevItemBase;
		ZeroMemory( &prevItemBase, sizeof( prevItemBase ) );

		if( pItem )
		{
			if( pItem->GetDBIdx() == itemBase.dwDBIdx )
			{
				prevItemBase = pItem->GetItemBaseInfo();
			}
			
			pItem->SetItemBaseInfo( itemBase ) ;
		}
		else
		{
			pItem = MakeNewItem( &itemBase, "MP_ITEM_MONSTER_OBTAIN_NOTIFY" ) ;

			// 071127 LYW --- ItemManager : 봉인 아이템 처리.
			const ITEM_INFO* pInfo = GetItemInfo( pItem->GetItemIdx() ) ;

			if( !pInfo ) continue;

			// 071127 LYW --- ItemManager : 아이템 습득 처리에 봉인 관련 처리 추가.
			if( pInfo->wSeal == eITEM_TYPE_SEAL )
			{
				itemBase.nSealed = eITEM_TYPE_SEAL ;
				itemBase.nRemainSecond = pInfo->dwUseTime ;

				pItem->SetItemBaseInfo( itemBase ) ;
			}

			if( !GAMEIN->GetInventoryDialog()->AddItem(pItem) )
			{
				ASSERT(0) ;
			}
		}

		// 071220 LUJ, 개수가 음수로 나오는 문제 수정
		if( IsDupItem( itemBase.wIconIdx ) )
		{
			const DURTYPE quantity = abs( int( prevItemBase.Durability ) - int( itemBase.Durability ) );

			// 080526 LUJ, 획득 개수가 0인 경우 표시하지 않도록 함
			if( quantity )
			{
				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 1112 ), pItem->GetItemInfo()->ItemName, quantity );
			}
		}
		else
		{
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 101 ), pItem->GetItemInfo()->ItemName );
		}

		GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem);
	}

	if( pItem )
	{
		ItemDropEffect( pItem->GetItemIdx() );
	}

	//나누기창이 있으면 지운다.
	cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
	if( pDlg )
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	//취소버튼 누르기
	}

	QUICKMGR->RefreshQickItem();

	//071022 LYW --- ItemManager : Check item tutorial.
	TUTORIALMGR->Check_GetItem(pItem) ;
}


void CItemManager::Item_Move_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	
	switch(pmsg->wData)
	{
	case 7:
		break;
	default:
		ASSERT(pmsg->wData == 3);	
	}
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	
	WINDOWMGR->BackDragWindow();
}


void CItemManager::Item_Divite_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_ShopInven);
	WINDOWMGR->BackDragWindow();
}


void CItemManager::Item_Combine_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	ASSERT(pmsg->wData == 0);
	ASSERT(0);
	
	WINDOWMGR->BackDragWindow();
}


void CItemManager::Item_Discard_Nack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	ASSERT(0);
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
	MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	if(pmsg->ECode != 4)
		WINDOWMGR->BackDragWindow();
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(799));
}


void CItemManager::Item_Discard_Ack( void* pMsg )
{
	MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;

	// 071210 LYW --- ItemManager : 퀵 슬롯에 등록 된 아이템이 인벤토리에서 삭제될 때, 
	// 퀵슬롯에 등록된 퀵 아이템도 삭제해야 한다. 봉인관련 아이템이나, 인챈트, 강화에 의해
	// 기본 아이템정보에서 변경이 된 아이템들의 툴팁이 뿌려지고 있는상황에서, 
	// 인벤에서만 아이템이 삭제될 경우, 이전에 진행하고 있던 처리과정의 정보들을 얻지 못해, 
	// 에러가 발생하기 때문이다.
	if( !pmsg ) return ;

	const ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->TargetPos) ;

	ASSERTMSG( !pItemBase, "ItemDelete-Invalid itembase" ) ;

	QUICKMGR->DeleteLinkdedQuickItem(pItemBase->dwDBIdx) ;
	// 퀵 아이콘 삭제 처리 완료.


	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);

	DeleteItemofTable(wTableIdx, pmsg->TargetPos, FALSE);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_Use_Notify( void* pMsg )
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject( pmsg->dwData );
	if( pObject == NULL ) return;
	if( pObject->GetObjectKind() != eObjectKind_Player ) return;

	CPlayer* pPlayer = (CPlayer*)pObject;

	switch( pmsg->bData )
	{
	case 1:	//생명력 회복 이펙트
		{
			OBJECTEFFECTDESC desc(FindEffectNum("ItemUSE_PotionHP_little.beff"));
			pPlayer->AddObjectEffect( LIFE_RECOVER_EFFECTID, &desc, 1 );
		}
		break;
	case 2:	//내력 회복 이펙트
		{
			OBJECTEFFECTDESC desc(FindEffectNum("ItemUSE_PotionMP_little.beff"));
			pPlayer->AddObjectEffect( MANA_RECOVER_EFFECTID, &desc, 1 );
		}
		break;
	case 3:	//올 회복 이펙트
		{
			//---KES 나중엔 이펙트 하나로 만들어서 그걸로 뿌려주자!
			OBJECTEFFECTDESC desc1(FindEffectNum("ItemUSE_PotionHP_little.beff"));
			pPlayer->AddObjectEffect( LIFE_RECOVER_EFFECTID, &desc1, 1 );
			OBJECTEFFECTDESC desc2(FindEffectNum("ItemUSE_PotionMP_little.beff"));
			pPlayer->AddObjectEffect( MANA_RECOVER_EFFECTID, &desc2, 1 );
//			OBJECTEFFECTDESC desc(FindEffectNum("ItemUSE_PotionHP_little.beff"));
//			pPlayer->AddObjectEffect( ALL_RECOVER_EFFECTID, &desc, 1 );
		}
		break;
	}
}


void CItemManager::Item_Use_Ack( void* pMsg )
{
	MSG_ITEM_USE_ACK* pmsg = (MSG_ITEM_USE_ACK*)pMsg;

	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);
	CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
	WORD Category = item->GetItemInfo()->Category;

	if( !item )
	{
		return;
	}

	// 쿨 타임 처리
	if( AddCoolTime( pmsg->dwObjectID, item->GetItemIdx() ) )
	{
		CoolTimeScript::ItemData::const_iterator item_it = mCoolTimeScript.mItemData.find( item->GetItemIdx() );

		if( mCoolTimeScript.mItemData.end() == item_it )
		{
			ASSERT( 0 );
			return;
		}

		const CoolTimeScript::Unit& unit		= item_it->second;
		const DWORD					itemIndex	= item->GetItemIdx();

		CoolTimeScript::GroupData::const_iterator group_it = mCoolTimeScript.mGroupData.find( GetCoolTimeGroupIndex( itemIndex ) );

		if( mCoolTimeScript.mGroupData.end() == group_it )
		{
			ASSERT( 0 );
			return;
		}

		const CoolTimeScript::ItemIndex& indexSet = group_it->second;

		// 인벤토리 아이콘 쿨타임 처리
		{
			CInventoryExDialog* dialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( dialog );

			dialog->SetCoolTime( itemIndex, unit.mTime );
		}

		// 단축창 아이콘 쿨타임 처리
		{
			cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
			ASSERT( dialog );

			dialog->SetCoolTime( itemIndex, unit.mTime );

			// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
			cQuickSlotDlg* dialog2 = NULL ;
			dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
			ASSERT( dialog2 ) ;

			if(dialog2)
			{
				dialog2->SetCoolTime( itemIndex, unit.mTime ) ;
			}
		}
	}

	if( IsChangeItem( pmsg->wItemIdx ) )
	{
		//{
		//	CInventoryExDialog* dialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
		//	ASSERT( dialog );

		//	dialog->Restore();
		//}
	}

	// 소모성 아이템일 때는 메시지 표시
	{
		const ITEM_INFO* info = item->GetItemInfo();
		ASSERT( info );

		if( eItemCategory_Expend == info->Category )
		{
			CHATMGR->AddMsg( CTC_EXPENDEDITEM, CHATMGR->GetChatMsg( 1111 ), info->ItemName );

			//// 물고기 사용시 포인트메시지 표시
			//if(info->SupplyType == ITEM_KIND_FISHING_FISH)
			//{
			//	CHATMGR->AddMsg( CTC_EXPENDEDITEM, CHATMGR->GetChatMsg( 1539 ), info->SupplyValue );
			//}
		}
	}
	
	switch(wTableIdx)//			switch(pmsg->wTableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
		{
			BOOL bDelItem = FALSE;

			switch( Category )
			{
			case eItemCategory_Expend:
				{
					if( item->GetItemInfo()->SupplyType == ITEM_KIND_PET )
					{
						break;
					}

					if(item->GetDurability() > 1)
					{
						//개수 처리
						GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
					}
					else
					{
						// 071212 LYW --- ItemManager : 봉인 된 아이템이라면, 사용 허가 처리가 왔을 때, 
						// 아이템을 소모하여 바로 없어지는 것이 아니라, 봉인이 해제가 되어야 하기 때문에, 
						// 아이템을 삭제하는 처리를 건너띈다.
						const ITEMBASE& pItemBase = item->GetItemBaseInfo() ;
						
						if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
						{
							QUICKMGR->RefreshQickItem();
							return ;
						}

						CItem* pOutItem = NULL;
						DeleteItem( pmsg->TargetPos, &pOutItem);
						if( pOutItem )
						{
							bDelItem = TRUE;
							ReLinkQuickPosition( pOutItem );
						}
					}

					if( item->GetItemInfo()->SupplyType == 1 || item->GetItemInfo()->SupplyType == 3 )
					{
						EFFECTMGR->StartHeroEffectProcess(FindEffectNum("ItemUSE_PotionHP_little.beff"));
					}
					else if( item->GetItemInfo()->SupplyType == 2 || item->GetItemInfo()->SupplyType == 4 )
					{
						EFFECTMGR->StartHeroEffectProcess(FindEffectNum("ItemUSE_PotionMP_little.beff"));
					}
					else if( item->GetItemInfo()->SupplyType == 5 || item->GetItemInfo()->SupplyType == 6 )
					{
						//---KES 나중엔 이펙트 하나로 만들어서 그걸 뿌려주자.
						EFFECTMGR->StartHeroEffectProcess(FindEffectNum("ItemUSE_PotionHP_little.beff"));
						EFFECTMGR->StartHeroEffectProcess(FindEffectNum("ItemUSE_PotionMP_little.beff"));
					}
					else if( item->GetItemInfo()->SupplyType == ITEM_KIND_FISHING_BAIT)
					{
						if(bDelItem)
						{
							CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForIdx(item->GetItemIdx());
							if(pItem)
								GAMEIN->GetFishingDlg()->UpdateBaitItem();
							else
								GAMEIN->GetFishingDlg()->SetBaitItem(NULL);
						}
						else
							GAMEIN->GetFishingDlg()->UpdateBaitItem();
					}

				}
				break;

			}





/*
			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			eITEM_KINDBIT bits = item->GetItemKind();
			if(bits & eYOUNGYAK_ITEM)
			{
			
				if(item->GetDurability() > 1)
				{
					//개수 처리
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
				}
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem( pmsg->TargetPos, &pOutItem);
					if( pOutItem )
						ReLinkQuickPosition( pOutItem );
				}
				
				// use sound
				//AUDIOMGR->Play(68, HERO);
				if( item )
				{
					if( item->GetItemIdx() == 53032 || item->GetItemIdx() == 53095
						|| item->GetItemIdx() == 53103 || item->GetItemIdx() == 53102 )
					{
						EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_078.beff"));
					}
					else
					{
						//if( item->GetItemKind() != eYOUNGYAK_ITEM )	return;
						if( !(item->GetItemKind() & eYOUNGYAK_ITEM) )	return;

						if( item->GetItemKind() == eYOUNGYAK_ITEM )	//펫 아닌 플레이어에게만 해당
						{
							if( item->GetItemInfo()->LifeRecover || item->GetItemInfo()->LifeRecoverRate )
							{
								EFFECTMGR->StartHeroEffectProcess(FindEffectNum("eff_PotionHP.beff"));
							}
							if( item->GetItemInfo()->ManaRecover || item->GetItemInfo()->ManaRecoverRate )
							{
								EFFECTMGR->StartHeroEffectProcess(FindEffectNum("eff_PotionMP.beff"));
							}								
							//========================= 달걀 하드코딩
							if( item->GetItemIdx() == 53031 || item->GetItemIdx() == 53094 || item->GetItemIdx() == 53109 )
							{
								EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_077.beff"));
							}
						}
						else if( item->GetItemKind() == eYOUNGYAK_ITEM_PET )
						{
							//펫 먹이 이펙트
// 									TARGETSET set;
// 									set.pTarget = HEROPET;
// 									EFFECTMGR->StartEffectProcess(eEffect_FeedUp,HEROPET,&set,0,HEROPET->GetID());

							CPet* pPet = PETMGR->GetCurSummonPet();
							if(pPet)
							{
								TARGETSET set;
								set.pTarget = pPet;
								EFFECTMGR->StartEffectProcess(eEffect_FeedUp,pPet,&set,0,pPet->GetID());
							}
						}
					}
		
//==========================

				}
			}
			else if(bits & eCHANGE_ITEM)
			{
				if(item->GetDurability() > 1)
				{
					//°³¼o A³¸®
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
				}
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem( pmsg->TargetPos, &pOutItem);
					if( pOutItem )
						ReLinkQuickPosition( pOutItem );
				}
			}
			else if(bits & eEXTRA_ITEM)
			{
//						ASSERT(0);
				
				if(item->GetDurability() > 1)
				{
					//개수 처리
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
				}
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem( pmsg->TargetPos, &pOutItem);
					if( pOutItem )
						ReLinkQuickPosition( pOutItem );
				}
			}
			else if(bits & eSKILL_ITEM)
			{
				CItem* pOutItem = NULL;
				DeleteItem( pmsg->TargetPos, &pOutItem);
			
				// use sound
				AUDIOMGR->Play(56, HERO);
			}
			else if( bits & eQUEST_ITEM )
			{
				if( bits == eQUEST_ITEM_START )
				{
					if( item->GetDurability() > 1 )
						GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
					else
					{
						CItem* pOutItem = NULL;
						DeleteItem( pmsg->TargetPos, &pOutItem);
						if( pOutItem )
							ReLinkQuickPosition( pOutItem );
					}
				}
			}
			else
			{
				ASSERT(0);
			}
*/		}
		break;
	case eItemTable_StreetStall:
		{
			ASSERT(0);
		}
		break;
	case eItemTable_ShopInven:
		{
/*			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			eITEM_KINDBIT bits = item->GetItemKind();

			if( bits & eSHOP_ITEM )
			{
				//if( bits == eSHOP_ITEM_HERB || bits == eSHOP_ITEM_INCANTATION )
				//{
				if(item->GetDurability() > 1)
				{
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos,-1);
				}
				//}
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem( pmsg->TargetPos, &pOutItem);
					if( pOutItem )
						ReLinkQuickPosition( pOutItem );
				}
				
				// use sound
				AUDIOMGR->Play(68, HERO);
			}
*/		}
		break;				
	default:
		{
			ASSERT(0);
		}
	}
	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_Mix_Ack( void* pMsg ) 
{
	const MSG_ITEM_MIX_ACK* message		= ( MSG_ITEM_MIX_ACK *)pMsg;
	const BOOL				isFailed	= ( message->Protocol != MP_ITEM_MIX_SUCCEED_ACK );
	CMixDialog*				mixDialog	= GAMEIN->GetMixDialog();

	ASSERT( mixDialog );

	// 결과 알림
	if( isFailed )
	{
		mixDialog->Fail( *message );

		QUICKMGR->RefreshQickItem();
	}
	else
	{
		CItem* item	= GetItem( message->mSourceItemDbIndex );

		if( ! item )
		{
			ASSERT( 0 );
			return;
		}

		const ItemMixResult*	mixResult	= GetMixResult( item->GetItemIdx(), message->mResultIndex );
		const ITEM_INFO*		info		= GetItemInfo( mixResult->mItemIndex );
		ASSERT( info );

		if( info )
		{
			CHATMGR->AddMsg(
				CTC_OPERATEITEM,
				CHATMGR->GetChatMsg( 814 ),
				info->ItemName,
				message->mMixSize );

			QUICKMGR->RefreshQickItem();
		}

		mixDialog->Succeed( *message );
	}
	
	CItem*				deletedItem		= 0;
	CInventoryExDialog*	inventoryDialog = GAMEIN->GetInventoryDialog();

	// 아이템 업데이트
	for( DWORD i = 0; i < message->mSize; ++i )
	{
		const ITEMBASE& updatedItem = message->mItem[ i ];

		CItem* item	= GetItem( updatedItem.dwDBIdx );
		
		if( item )
		{
			item->SetLock( FALSE );

			const BOOL isDuplicated = IsDupItem( updatedItem.wIconIdx );

			if( updatedItem.wIconIdx )
			{
				cImage image;

				item->SetBasicImage( GetIconImage( updatedItem.wIconIdx, &image ) );
				item->SetItemBaseInfo( updatedItem );
				item->SetVisibleDurability( isDuplicated );
				item->SetData( updatedItem.wIconIdx );

				RefreshItem( item );
			}
			else
			{
				DeleteItem( item->GetPosition(), &deletedItem );
			}
		}
		// 071218 LUJ,	클라이언트에 없는 아이템이면 임시에 생성해 놓고, DB 인덱스가 서버에서 올때까지 잠궈놓는다
		//				DB랙으로 인해 아이템이 사라졌다고 오해하거나, 해당 공간을 사용하는 것을 막기 위함이다
		else
		{
			CItem* item = new CItem( &updatedItem );

			mPreIconList.push_back( item );

			cImage image;

			item->Init(
				0,
				0,
				DEFAULT_ICONSIZE,
				DEFAULT_ICONSIZE,
				GetIconImage( updatedItem.wIconIdx, &image ),
				m_IconIndexCreator.GenerateIndex() );

			item->SetVisibleDurability( IsDupItem( updatedItem.wIconIdx ) );
			item->SetLock( TRUE );
			item->SetData( updatedItem.wIconIdx );

			inventoryDialog->AddItem( item );

			item->AddToolTipLine( "" );
			item->AddToolTipLine( "ing..." );

			//AddToolTip( &item );
		}
	}

	// 080228 LUJ, 창이 열려있을 경우에만 자동 등록하도록 한다
	if( isFailed &&
		mixDialog->IsActive() )
	{
		// 실패 후에도 원본 아이템이 여전히 남아있으면 조합창에 자동 등록시킨다
		{
			CItem* sourceItem = GetItem( message->mSourceItemDbIndex );

			if( sourceItem )
			{
				mixDialog->FakeMoveIcon( 0, 0, sourceItem );
			}
		}
	}
}


void CItemManager::Item_Mix_GetItem( void* p )
{
	ITEMOBTAINARRAY*	message = ( ITEMOBTAINARRAY* )p;

	// 인벤토리 갱신
	{
		CInventoryExDialog* dialog	= GAMEIN->GetInventoryDialog();
		ASSERT( dialog );

		for( int i = 0; i < message->ItemNum ; ++i )
		{
			const ITEMBASE* itemBase = message->GetItem( i );

			{
				CItem* item = dialog->GetItemForPos( itemBase->Position );

				if( item )
				{
					CItem*	deletedItem	= 0;

					//dialog->DeleteItem( itemBase->Position, 0 );
					DeleteItem( item->GetPosition(), &deletedItem );
				}
			}

			{
				CItem* item = MakeNewItem( itemBase, "" );

				if( ! dialog->AddItem( item ) )
				{
					ASSERT( 0 );
				}

				item->SetLock( FALSE );
			}
		}	
	}

	// 080227 LUJ, 조합창 갱신. 결과를 한 종류만 처리할 수 있도록 설계되어 있음을 유의하자
	{
		CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );

		if( dialog )
		{
			const ITEMBASE* itemBase = message->GetItem( 0 );

			if( itemBase )
			{
				dialog->RefreshResult( *itemBase );
			}
		}
	}
}


void CItemManager::Item_Mix_Nack( void* pMsg )
{
	const MSG_DWORD*	message = ( MSG_DWORD* )	pMsg;		

	switch( eLogitemmoney( message->dwData ) )
	{
	case eLog_ItemMixInsufficentSpace:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 910 ) );
			break;
		}
	case eLog_ItemMixSystemFault:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "시스템 오류가 발생했습니다. 안전한 게임 진행을 위해 재접속하세요" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	case eLog_ItemMixNotFound:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "조합할 아이템이 부족합니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 784 ) );
			break;
		}
	case eLog_itemMixBadRequest:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "잘못된 조합 요청입니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1276 ) );
			break;
		}
	case eLog_ItemMixBadCondition:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "잘못된 조합 조건입니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1277 ) );
			break;
		}
	case eLog_ItemMixBadQuantity:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "최대 개수를 넘어선 아이템이 발견되었습니다. 운영팀에 알려주세요" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1278 ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "조합 중에 시스템 오류가 생겼습니다. 올바른 진행을 위해 재접속을 권유합니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	}
}


void CItemManager::Item_Working_Success( void* pMsg ) 
{
	const MSG_DWORD*	message		= ( MSG_DWORD* )pMsg;
	const DWORD			playerIndex = message->dwData;
	CObject*			object		= OBJECTMGR->GetObject( playerIndex );

	if( object )
	{
		OBJECTEFFECTDESC desc( 28 );

		object->RemoveObjectEffect( ITEM_WORKING_EFFECT );
		object->RemoveObjectEffect( ITEM_WORKING_SUCCESS_EFFECT );
		object->AddObjectEffect( ITEM_WORKING_SUCCESS_EFFECT, &desc, 1 );
	}
}

void CItemManager::Item_Working_Fail( void* pMsg )
{
	const MSG_DWORD*	message		= ( MSG_DWORD* )pMsg;
	const DWORD			playerIndex = message->dwData;
	CObject*			object		= OBJECTMGR->GetObject( playerIndex );

	if( object )
	{
		OBJECTEFFECTDESC desc( 29 );

		object->RemoveObjectEffect( ITEM_WORKING_EFFECT );
		object->RemoveObjectEffect( ITEM_WORKING_FAIL_EFFECT );
		object->AddObjectEffect( ITEM_WORKING_FAIL_EFFECT, &desc, 1 );
	}
}

void CItemManager::Item_Working_Start( void* pMsg )
{
	const MSG_DWORD*	message		= ( MSG_DWORD* )pMsg;
	const DWORD			playerIndex = message->dwData;
	CObject*			object		= OBJECTMGR->GetObject( playerIndex );

	if( object )
	{
		OBJECTEFFECTDESC desc( 30 );

		object->RemoveObjectEffect( ITEM_WORKING_EFFECT );
		object->AddObjectEffect( ITEM_WORKING_EFFECT, &desc, 1 );
	}
}

void CItemManager::Item_Working_Stop( void* pMsg )
{
	const MSG_DWORD*	message		= ( MSG_DWORD* )pMsg;
	const DWORD			playerIndex = message->dwData;
	CObject*			object		= OBJECTMGR->GetObject( playerIndex );

	if( object )
	{
		object->RemoveObjectEffect( ITEM_WORKING_EFFECT );
	}
}

void CItemManager::Item_TimeLimit_Item_OneMinute( void* pMsg )
{
	if( !pMsg ) return ;
	
	MSG_DWORD2* pmsg = NULL ;
	pmsg = (MSG_DWORD2*)pMsg ;

	if( !pmsg ) return ;

	ITEM_INFO* pItemInfo = NULL ;
	pItemInfo = GetItemInfo( pmsg->dwData1 ) ;

	if( !pItemInfo ) return ;

	POSTYPE pos = (POSTYPE)pmsg->dwData2 ;

	const ITEMBASE* pItemBase = GetItemInfoAbsIn( HERO, pos ) ;

	if( pItemBase )
	{
		CItem* pItem = NULL ;
		pItem = GetItem( pItemBase->dwDBIdx ) ;

		if( pItem )
		{
			pItem->SetItemBaseInfo( *pItemBase ) ;
			pItem->SetLastCheckTime(gCurTime) ;
			pItem->SetRemainTime( 50 ) ;
		}
	}

	WORD wTableIdx = GetTableIdxForAbsPos( pos ) ;

	if( wTableIdx == eItemTable_Inventory )
	{
		CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg( 1191 ), CHATMGR->GetChatMsg( 1189 ), pItemInfo->ItemName ) ;
	}
	else if( wTableIdx == eItemTable_Storage )
	{
		CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg( 1191 ), CHATMGR->GetChatMsg( 1188 ), pItemInfo->ItemName ) ;
	}
	else if( wTableIdx == eItemTable_Weared )
	{
		CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg( 1191 ), CHATMGR->GetChatMsg( 1190 ), pItemInfo->ItemName ) ;
	}
}

void CItemManager::Item_ShopItem_Use_Ack( void* pMsg )
{
	if( !pMsg ) return ;

	SEND_ITEM_BASEINFO* pmsg = NULL ;
	pmsg = (SEND_ITEM_BASEINFO*)pMsg ;
	if( !pmsg ) return ;

	CInventoryExDialog* pInven = NULL ;
	pInven = GAMEIN->GetInventoryDialog() ;
	if( !pInven ) return ;

	CItem* pItem = NULL ;
	pItem = pInven->GetItemForPos( pmsg->ItemPos ) ;
	if( !pItem ) return ;

	const ITEM_INFO* pInfo = pItem->GetItemInfo() ;
	if( !pInfo ) return ;

	/*pmsg->ItemBase.nRemainSecond = 0 ;
	pmsg->ItemBase.nRemainSecond = pInfo->dwUseTime ;*/

	//pmsg->ItemBase.LastCheckTime = gCurTime ;

	pItem->SetItemBaseInfo(pmsg->ItemBase) ;
	pItem->SetLastCheckTime(gCurTime) ;

	//HERO->Add_htTimeCheckItem(&pmsg->ItemBase) ;

	AddToolTip( pItem ) ;

	////AddUsedItemInfo(&pmsg->ItemBase) ;

	// 071215 LYW --- ItemManager : 퀵 슬롯에 봉인된 아이템의 링크가 걸려 있으면, 봉인을 풀어준다.
	QUICKMGR->UnsealQuickItem(pmsg->ItemBase.dwDBIdx) ;

	QUICKMGR->RefreshQickItem() ;

	CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg(366) ) ;

	char buf[256] = { 0, } ;
	wsprintf( buf, CHATMGR->GetChatMsg(1176), pItem->GetItemInfo()->ItemName ) ;
	CHATMGR->AddMsg( CTC_CHEAT_1, buf ) ;
}

void CItemManager::Item_ShopItem_Use_Nack( void* pMsg )
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	//SetDisableDialog(FALSE, eItemTable_Inventory);
	//SetDisableDialog(FALSE, eItemTable_Storage);
	//SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	//SetDisableDialog(FALSE, eItemTable_Shop);

	//if( HERO->GetState() == eObjectState_Deal )			
	//	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	//switch(pmsg->ECode)
	//{
	//case eItemUseErr_Lock:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(751) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "봉인을 해제한 후 사용하십시요." );
	//	break;
	//case eItemUseErr_AlreadyUse:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "이미 사용중인 아이템 입니다." );
	//	break;
	//case eItemUseErr_Unabletime:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(757) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "이전 사용일에서 30일이 지난 후에 사용할 수 있습니다." );
	//	break;
	//case eItemUseErr_AlredyChange:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(912) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "이름변경에 실패하였습니다." );
	//	break;
	//case eItemuseErr_DontUseToday:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1079) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "오늘은 더이상 사용하실 수 없습니다." );
	//	break;
	//case eItemUseErr_DontDupUse:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1076) );
	//	CHATMGR->AddMsg( CTC_SYSMSG, "중복해서 사용할수 없는 아이템입니다." );
	//	break;
	//case eItemUseErr_UseFull:
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1435) );
	//	//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1079) );
	//	break;
	//}
}


void CItemManager::Item_Enchant_Success_Ack( void* pMsg )
{
	const MSG_ITEM_ENCHANT_ACK* message = ( MSG_ITEM_ENCHANT_ACK* )pMsg;

	{
		CEnchantDialog* dialog = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG	);
		ASSERT( dialog );

		dialog->Succeed( *message );
	}

	//{
	//	CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
	//	ASSERT( dialog );

	//	dialog->Succeed();
	//}
	
	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_Enchant_Nack( void* pMsg )
{
	const MSG_DWORD* message = ( MSG_DWORD* )pMsg;

	switch( eLogitemmoney( message->dwData ) )
	{
	case eLog_ItemEnchantItemInvalid:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "Report to LUNA homepage, please: Enchanting item is invalid" );
			break;
		}
	case eLog_ItemEnchantServerError:
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "인챈트 중에 시스템 오류가 생겼습니다. 올바른 진행을 위해 다시 접속하시기 바랍니다" );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	case eLog_ItemEnchantHacking:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "Report to LUNA homepage, please: your apply to server was abnormal" );
			break;
		}
	case eLog_ItemEnchantItemUpdateFailed:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "Report to LUNA homepage, please: After enchanting update data is failed" );
			break;
		}
	default:
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "인챈트 중에 알 수 없는 시스템 오류가 생겼습니다. 올바른 진행을 위해 다시 접속하시기 바랍니다" );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	}

	{
		CEnchantDialog* dialog = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG	);
		ASSERT( dialog );

		dialog->Restore();
	}
}


void CItemManager::Item_Enchant_Failed_Ack( void* pMsg )
{
	CEnchantDialog* dialog = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG	);
	ASSERT( dialog );

	dialog->Fail();
}


void CItemManager::Item_Use_Nack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.29
	/*
	MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	if(pmsg->ECode == eItemUseErr_PreInsert)
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(678));
	else
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
	*/

	// 080607 LYW --- ItemManager : 아이템 사용 실패 에러 메시지 처리 수정.
	//MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	//if(pmsg->ECode == eItemUseErr_PreInsert)
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(678));
	//// 인벤이 꽉 찼을 때 사용이 불가능한 아이템 일 경우 (EX:체인지아이템)
	//else if (pmsg->ECode == eItemUseErr_FullInven)
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
	//// 아이템 에러 메세지가 필요없을 경우(EX:농장 아이템. 농장 처리 루틴에서 에러 메세지를 처리함)
	//else if (pmsg->ECode == eItemUseErr_WithNoneMsg)
	//	;
	//else
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
	//// E 농장시스템 추가 added by hseos 2007.08.29

	////// 변환 아이템 사용시에는 실패를 알려야 한다.
	////{
	////	CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
	////	ASSERT( dialog );

	////	if( dialog->IsActive() )
	////	{
	////		dialog->Fail();
	////	}
	////}

	MSG_ITEM_ERROR * pmsg = NULL ;
	pmsg = (MSG_ITEM_ERROR*)pMsg ;

	if(!pmsg) return ;

	switch(pmsg->ECode)
	{
	case eItemUseErr_PreInsert :
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(678)) ;
		}
		break ;

	case eItemUseErr_FullInven : 
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) ) ;
		}
		break ;

	case eItemUse_OpenStorage_byItem_Failed_Storage_IsZero : 
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1551)) ;
		}
		break ;

	case eItemUseErr_WithNoneMsg : break ;

	default :
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) ) ;
		}
		break ;
	}
}


void CItemManager::Item_Sell_Ack( void* pMsg )
{
	MSG_ITEM_SELL_ACK * pmsg = ( MSG_ITEM_SELL_ACK * )pMsg;	
	CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
	if(IsDupItem(pmsg->wSellItemIdx) && (pItem->GetDurability() - pmsg->SellItemNum) > 0)
	{
		// update
		pItem->SetDurability( pItem->GetDurability() - pmsg->SellItemNum );
		DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
		CHATMGR->AddMsg( CTC_SELLITEM, CHATMGR->GetChatMsg(39), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma( SellPrice*pmsg->SellItemNum ) );
	}
	else
	{
		// delete
//				if( pmsg->SellItemNum == 0 || IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(217), pItem->GetItemInfo()->ItemName, AddComma( pItem->GetItemInfo()->SellPrice ) );
//				else
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(218), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma( pItem->GetItemInfo()->SellPrice*pmsg->SellItemNum ) );
		DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
		if( pmsg->SellItemNum == 0 || ! IsDupItem( pItem->GetItemIdx() ) )
		{
			CHATMGR->AddMsg( CTC_SELLITEM, CHATMGR->GetChatMsg(38), pItem->GetItemInfo()->ItemName, AddComma( SellPrice ) );
		}
		else
		{
			CHATMGR->AddMsg( CTC_SELLITEM, CHATMGR->GetChatMsg(39), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma( SellPrice*pmsg->SellItemNum ) );
		}

		//DeleteItemofTable(GetTableIdxForAbsPos(pmsg->TargetPos), pmsg->TargetPos);
		// 071202 LYW --- ItemManager : 봉인 아이템이 삭제 될 때, 순수 삭제될 때와 상점에 팔 때에도 같은 이 함수를 사용하기 때문에, 
		//								함수 뒤쪽 인자로 팔때인지 삭제인지 여부를 체크하도록 수정함.
		DeleteItemofTable(GetTableIdxForAbsPos(pmsg->TargetPos), pmsg->TargetPos, TRUE) ;
	}
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
	GAMEIN->GetDealDialog()->SetDisable( FALSE );
	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_Sell_Nack( void* pMsg )
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	if( pmsg->ECode == 9 )
		// 070329 LYW --- ItemManager : Modified message number.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(459) );

	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
	GAMEIN->GetDealDialog()->SetDisable( FALSE );
}


void CItemManager::Item_Buy_Ack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Deal);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	BOOL SetQuickLink = FALSE; //TEST code
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		if( pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
		{
			DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
			ASSERT(pPreItem);
		}
		pItem = MakeNewItem(pmsg->GetItem(i),"MP_ITEM_BUY_ACK");	
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if(!rt)
		{
			ASSERT(0);					
		}
	}
	MONEYTYPE BuyPrice;
	BuyPrice = pItem->GetItemInfo()->BuyPrice;
//	if(MAP->IsVillage() == FALSE)
//	{
//		BuyPrice = (MONEYTYPE)(BuyPrice * 1.2);
//	}
	BuyPrice = SWPROFIT->CalTexRateForBuy( BuyPrice );
	if( pmsg->wObtainCount > 1 )
	{
		CHATMGR->AddMsg( CTC_BUYITEM, CHATMGR->GetChatMsg(37), pItem->GetItemInfo()->ItemName, pmsg->wObtainCount,
						AddComma( BuyPrice * pmsg->wObtainCount ) );	//confirm
	}
	else
	{
		CHATMGR->AddMsg( CTC_BUYITEM, CHATMGR->GetChatMsg(36), pItem->GetItemInfo()->ItemName, AddComma( BuyPrice ) );
	}

	QUICKMGR->RefreshQickItem();
}

// 070329 LYW --- ItemManager : Add function to make string.
void CItemManager::CompositString( char* inputStr, char* str, ... )
{
	if( inputStr == NULL ) return ;
	if( str == NULL ) return;

	va_list argList;

	va_start(argList, str);
	vsprintf(inputStr,str,argList);
	va_end(argList);
}


void CItemManager::Item_Buy_Nack( void* pMsg )
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	//most of all, inventory full process : taiyo
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_Deal);
	//GAMEIN->GetDealDialog()->SetDisable( FALSE );

	switch(pmsg->ECode)
	{
	case NOT_MONEY:	
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		break;
	case NOT_SPACE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		break;
	case NOT_EXIST:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(476) );
		break;
	case NOT_PLAYER:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(477) );
		break;
	case NOT_FISHINGPOINT:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1533 ) );
		break;
	default:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(478) );
		break;
	}

	//ASSERT(0);
}


void CItemManager::Item_Dealer_Ack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg; 
	if( HERO->GetState() != eObjectState_Die)
		GAMEIN->GetDealDialog()->ShowDealer(pmsg->wData);

	// 070329 LYW --- ItemManager : Setting position deal dialog and inventory dialog.
	GAMEIN->GetDealDialog()->ShowDealDialog(TRUE) ;
}


//void CItemManager::Item_Appearance_Change( void* pMsg )
//{
//	APPEARANCE_INFO* pmsg = (APPEARANCE_INFO*)pMsg;
//	CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->PlayerID);
//	if( pObject && pObject->GetObjectKind() == eObjectKind_Player )
//	{
//		CPlayer* pPlayer = (CPlayer*)pObject;
//		for(int n=0;n<eWearedItem_Max;++n)
//			pPlayer->SetWearedItemIdx(n,pmsg->WearedItem[n]);
//		APPEARANCEMGR->AddCharacterPartChange(pObject->GetID());
//	}
//	else
//	{
//		ASSERT(0);
//	}
//}


void CItemManager::Item_Appearance_Add( void* p )
{
	const MSG_APPERANCE_ADD*	message		= ( MSG_APPERANCE_ADD* )p;
	const DWORD					playerIndex = message->dwObjectID;

	CPlayer* player = ( CPlayer* )OBJECTMGR->GetObject( playerIndex );

	if( player &&
		player->GetObjectKind() == eObjectKind_Player )
	{
		player->SetWearedItemIdx( message->mSlotIndex, message->mItemIndex );
		APPEARANCEMGR->AddCharacterPartChange( playerIndex );
	}
	else
	{
		ASSERT( 0 );
	}
}


void CItemManager::Item_Appearance_Remove( void* p )
{
	const MSG_APPERANCE_REMOVE*	message		= ( MSG_APPERANCE_REMOVE* )p;
	const DWORD					playerIndex = message->dwObjectID;

	CPlayer* player = ( CPlayer* )OBJECTMGR->GetObject( playerIndex );

	if( player &&
		player->GetObjectKind() == eObjectKind_Player )
	{
		player->SetWearedItemIdx( message->mSlotIndex, 0 );
		APPEARANCEMGR->AddCharacterPartChange( playerIndex );
	}
	else
	{
		ASSERT( 0 );
	}
}


void CItemManager::Item_Money( void* pMsg )
{
	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
			
	MONEYTYPE OriMoney = HERO->GetMoney();
	MONEYTYPE PrintMoney = 0;

	switch(pmsg->bFlag)
	{
	case 64:
		{
			PrintMoney = pmsg->dwTotalMoney - OriMoney;
			if( PrintMoney )
			{
				// 070126 LYW --- GETMONEY : Modified message number.
				//CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), AddComma( PrintMoney ) );
				CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 301 ), AddComma( PrintMoney ) );
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
			}
		}
		break;
	case MF_NOMAL:
		{
			break;
		}
	case MF_OBTAIN: // Obtain Money;
		{
			PrintMoney = pmsg->dwTotalMoney - OriMoney;
			if( PrintMoney )
			{
				// 070126 LYW --- GETMONEY : Modified message number.
				//CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), AddComma( PrintMoney ) );
				CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 301 ), AddComma( PrintMoney ) );
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
			}
		}
		break;
	case MF_LOST:
		{
			PrintMoney = OriMoney - pmsg->dwTotalMoney;
			if( PrintMoney )
			{
				// 070126 LYW --- GETMONEY : Modified message number.
				//CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 118 ), AddComma( PrintMoney) );
				CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 378 ), AddComma( PrintMoney) );
			}
		}
		break;
	default:
		break;
	}

	HERO->SetMoney(pmsg->dwTotalMoney);

	// 조합은 히어로의 소지금/레벨에 영향받는데, 현재 상태에 기반해서 표시해주는 항목이 있으므로 정보가 변경될때마다 갱신이 필요하다
	{
		CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
		ASSERT( dialog );

		dialog->Refresh();
	}
}


void CItemManager::Item_Obtain_Money( void* pMsg )
{
	ASSERT(0);
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	/*
	CChatDialog * dlg = GAMEIN->GetChatDialog();
	dlg->AddChatMsg(WHOLE,RGBA_MAKE(0,0,255,255),
		"[%s] 돈을 %d 얻었습니다.", HERO->GetObjectName(), pmsg->dwData - HERO->GetMoney());// pjslocal [5/29/2003]
	*/

	DWORD OriMoney = HERO->GetMoney();
	DWORD PrintMoney = pmsg->dwData - OriMoney;

	if( PrintMoney )
	{
		// 070126 LYW --- GETMONEY : Modified message number.
		//CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 120 ), PrintMoney );
		CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg( 301 ), PrintMoney );
		EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
	}
	
	HERO->SetMoney(pmsg->dwData);
}


void CItemManager::Item_Divide_NewItem_Notify( void* pMsg )
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CItem* pItem = MakeNewItem(&pmsg->ItemInfo,"MP_ITEM_DIVIDE_NEWITEM_NOTIFY");
	int tbidx = GetTableIdxForAbsPos(pItem->GetItemBaseInfo().Position);
	if(tbidx == eItemTable_Inventory || tbidx == eItemTable_ShopInven)
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
	else if(tbidx == eItemTable_Storage)
		BOOL rt = GAMEIN->GetStorageDialog()->AddItem(pItem);
	else if(tbidx == eItemTable_GuildWarehouse)
		BOOL rt = GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);

	CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 123 ));
}


void CItemManager::Item_Guild_Move_Ack( void* pMsg )
{
	MoveItemToGuild((MSG_ITEM_MOVE_ACK*)pMsg);
	WINDOWMGR->DragWindowNull();
}


void CItemManager::Item_Guild_Move_Nack( void* pMsg )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	WINDOWMGR->BackDragWindow();
}


void CItemManager::Item_DeleteFrom_GuildWare_Notify( void* pMsg )
{
	MSG_ITEM * pItembase = (MSG_ITEM*)pMsg;
	CItem* pItem = NULL;
	if(GAMEIN->GetGuildWarehouseDlg()->DeleteItem(pItembase->ItemInfo.Position, &pItem) == TRUE)
		ItemDelete(pItem);
	else
	{
		ASSERT(0);
	}
}


void CItemManager::Item_AddTo_GuildWare_Notify( void* pMsg )
{
	const MSG_ITEM_WITH_OPTION * pItem = (MSG_ITEM_WITH_OPTION*)pMsg;

	AddOption( pItem->OptionInfo );
	
	GAMEIN->GetGuildWarehouseDlg()->AddItem(&pItem->ItemInfo);
}


void CItemManager::Item_Storage_Move_Ack( void* pMsg )
{
	MoveItemToStorage( (MSG_ITEM_MOVE_ACK*)pMsg );
	WINDOWMGR->DragWindowNull();
}


void CItemManager::Item_Dissolve_GetItem( void* pMsg )
{
	ITEMOBTAINARRAY*	message = (ITEMOBTAINARRAY *)pMsg;
	CInventoryExDialog* dialog	= GAMEIN->GetInventoryDialog();
	ASSERT( dialog );

	for( int i = 0 ; i < message->ItemNum ; ++i )
	{
		const ITEMBASE* itemBase = message->GetItem( i );

		{
			CItem* item = dialog->GetItemForPos( itemBase->Position );

			if( item )
			{
				CItem*	deletedItem	= 0;

				DeleteItem( item->GetPosition(), &deletedItem );
			}
		}

		{
			CItem* item = MakeNewItem( itemBase, "" );

			if( ! dialog->AddItem( item ) )
			{
				continue;
			}

			//const ITEM_INFO* itemInfo = item->GetItemInfo();

			//if( ! itemInfo )
			//{
			//	continue;
			//}
		}
	}
}


void CItemManager::Item_Dissolve_Ack( void* pMsg )
{
	const MSG_ITEM_DISSOLVE_ACK* message = ( MSG_ITEM_DISSOLVE_ACK *)pMsg;

	{
		CDissolveDialog* dialog = GAMEIN->GetDissolveDialog();
		ASSERT( dialog );

		dialog->Succeed( *message );

		// 080314 LUJ, 분해 대상이 가진 옵션을 삭제한다
		RemoveOption( message->mSourceItemDbIndex );
	}

	typedef std::map< DWORD, DURTYPE >	DissolveResult;
	DissolveResult						dissolveResult;

	CItem*				deletedItem		= 0;
	CInventoryExDialog*	inventoryDialog = GAMEIN->GetInventoryDialog();

	// 아이템 업데이트
	for( DWORD i = 0; i < message->mSize; ++i )
	{
		const ITEMBASE& updatedItem = message->mItem[ i ];

		CItem* item	= GetItem( updatedItem.dwDBIdx );

		if( item )
		{
			if( updatedItem.wIconIdx )
			{
				// 080610 LUJ, 분해 아이템에 옵션이 남지 않게 삭제한다
				RemoveOption( updatedItem.dwDBIdx );

				cImage image;

				item->SetBasicImage( GetIconImage( updatedItem.wIconIdx, &image ) );
				item->SetItemBaseInfo( updatedItem );
				item->SetVisibleDurability( IsDupItem( updatedItem.wIconIdx ) );
				item->SetData( updatedItem.wIconIdx );

				AddToolTip( item );
			}
			else
			{
				DeleteItem( item->GetPosition(), &deletedItem );
			}

			item->SetLock( FALSE );
		}
		// 071218 LUJ,	클라이언트에 없는 아이템이면 임시에 생성해 놓고, DB 인덱스가 서버에서 올때까지 잠궈놓는다
		//				DB랙으로 인해 아이템이 사라졌다고 오해하거나, 해당 공간을 사용하는 것을 막기 위함이다
		else
		{
			CItem* item = new CItem( &updatedItem );

			mPreIconList.push_back( item );

			cImage image;

			item->Init(
				0,
				0,
				DEFAULT_ICONSIZE,
				DEFAULT_ICONSIZE,
				GetIconImage( updatedItem.wIconIdx, &image ),
				m_IconIndexCreator.GenerateIndex() );

			item->SetVisibleDurability( IsDupItem( updatedItem.wIconIdx ) );
			item->SetLock( TRUE );
			item->SetData( updatedItem.wIconIdx );

			inventoryDialog->AddItem( item );
		}
	}
}


void CItemManager::Item_Dissolve_Nack( void* pMsg )
{
	const MSG_DWORD* message = ( MSG_DWORD* )pMsg;

	switch( eLogitemmoney( message->dwData ) )
	{
	case eLog_ItemDissolvePlayerNotExist:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Server have no such player data" );
			break;
		}
	case eLog_ItemDissolveSourceItemNotExist:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "분해할 아이템이 존재하지 않습니다. 올바른 진행을 위해 다시 접속하시기 바랍니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	case eLog_ItemDissolveItemDataNotMatched:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Server item data do not matched with yours" );
			break;
		}
	case eLog_ItemDissolveServerHaveNoDissolvingData:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Server has no data about it's dissolution" );
			break;
		}
	case eLog_ItemDissolveItNeedMoreLevel:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: You need more level to dissolve it" );
			break;
		}
	case eLog_ItemDissolveRemovingItemIsFailed:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Removing item is failed" );
			break;
		}
	case eLog_ItemDissolveAddingItemIsFailed:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: Adding item is failed" );
			break;
		}
	case eLog_ItemDissolveInsufficentSpace:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 839 ) );
			break;
		}
	case eLog_ItemDissolveNoResult:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: No result remained" );
			break;
		}
	case eLog_ItemDissolvePlayserHasNoInventory:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: You have no inventory" );
			break;
		}
	case eLog_ItemDissolveServerError:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "분해 중에 시스템 오류가 생겼습니다. 올바른 진행을 위해 다시 접속하시기 바랍니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	case eLog_ItemDissolveScriptInvalid:
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, "Report to LUNA homepage, please: dissolving script is invalid" );
			break;
		}
	default:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, "분해 중에 아이템 정보 이상이 감지되었습니다. 올바른 진행을 위해 다시 접속하시기 바랍니다" );
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1275 ) );
			break;
		}
	}
}


void CItemManager::Item_Error_Nack( void* p )
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Storage);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	WINDOWMGR->BackDragWindow();

	const MSG_ITEM_ERROR* message = ( MSG_ITEM_ERROR* )p;

	switch( message->ECode )
	{
	case eItemUseErr_PreInsert:
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "이 슬롯은 아이템이 들어가기로 예약되어 있습니다" );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1279 ) );
			break;
		}
	case eItemUseErr_Discard:
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "이미 버린 아이템입니다" );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1280 ) );
			break;
		}
	case eItemUseErr_AlreadyUse:
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "써버린 아이템입니다" );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1281 ) );
			break;
		}
	}
}


void CItemManager::Item_ChangeItem_Nack( void* pMsg )
{
	MSG_WORD * pmsg = (MSG_WORD*)pMsg;

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );

	//{
	//	CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
	//	ASSERT( dialog );

	//	dialog->Fail();
	//}
}


void CItemManager::Item_Use_ChangeItem_Ack( void* pMsg )
{
	ITEMOBTAINARRAY* pmsg = (ITEMOBTAINARRAY *)pMsg;
		
	CItem* pItem = NULL;

	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		pItem								= GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position );
		const DURTYPE	previousQuantity	= ( pItem ? pItem->GetDurability() : 0 );

		// 071127 LYW --- ItemManager : 아이템 습득 처리에 봉인 관련 처리 추가.
		ITEMBASE* pItemBase = pmsg->GetItem(i) ;

		if( !pItemBase ) return ;

		if( pItem )
		{
			pItem->SetItemBaseInfo(*pItemBase);
		}
		else
		{
			pItem = MakeNewItem(pItemBase,"MP_ITEM_MONSTER_OBTAIN_NOTIFY");	

			ITEM_INFO* pInfo = GetItemInfo( pItem->GetItemIdx() ) ;

			if( !pInfo ) continue ;

			if( pInfo->wSeal == eITEM_TYPE_SEAL )
			{
				pItemBase->nSealed = eITEM_TYPE_SEAL ;
				pItemBase->nRemainSecond = pInfo->dwUseTime ;

				pItem->SetItemBaseInfo( *pItemBase ) ;
			}
			
			if(! GAMEIN->GetInventoryDialog()->AddItem(pItem) )
			{
				ASSERT(0);
			}
		}

		if( IsDupItem( pItem->GetItemIdx() ) )
		{
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 1112 ), pItem->GetItemInfo()->ItemName, pItem->GetDurability() - previousQuantity );
		}
		else
		{
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 101 ), pItem->GetItemInfo()->ItemName );
		}
	}

	if( pItem )
	{
		ItemDropEffect( pItem->GetItemIdx() );
	}
	
	//³ª´ⓒ±aA￠AI AOA¸¸e Ao¿i´U.
	cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
	if( pDlg )
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	//Ae¼O¹oÆ° ´ⓒ¸￡±a
	}

	QUICKMGR->RefreshQickItem();
}


void CItemManager::Item_EventItem_Use( void* pMsg )
{
#ifdef TAIWAN_LOCAL
	if( pmsg->dwData2 == 53074 )
	{
		EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_079.beff") );
	}

	// 06. 03. 화이트데이 이벤트
	if( pmsg->dwData2 = 53151 )
	{
		EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_082.beff") );
	}
#else
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);

	// 06. 03. 화이트데이 이벤트
	if( pmsg->dwData2 == 53151 )
	{
		EFFECTMGR->StartPlayerEffectProcess( pPlayer, FindEffectNum("m_ba_082.beff") );
	}
#endif
}


// 071128 LYW --- ItemManager : 아이템몰 창고 추가 처리.
void CItemManager::Item_ShopItem_Info_Ack( void* pMsg ) 
{
	SEND_SHOPITEM_INFO* pmsg = (SEND_SHOPITEM_INFO*)pMsg;

	if( pmsg->ItemCount )
	{
		GAMEIN->GetItemShopDialog()->SetItemInfo(pmsg);
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1193) );
	}

	GAMEIN->GetItemShopDialog()->SetActive(TRUE);
	GAMEIN->GetInventoryDialog()->SetActive( TRUE );
}





// 071203 LYW --- ItemManager
void CItemManager::Item_Skill_Ack( void* pMsg )
{
	if( !pMsg ) return ;

	MSG_ITEM_USE_ACK* pmsg = NULL ;
	pmsg = (MSG_ITEM_USE_ACK*)pMsg ;

	if( !pmsg ) return ;

	Item_Use_Ack(pMsg);
}

// 071204 LYW --- ItemManager :
void CItemManager::Item_Reset_Stats_Ack( void* pMsg )
{
	if( !pMsg ) return ;
			
	SEND_DEFAULT_STATS* pmsg = NULL ;
	pmsg = (SEND_DEFAULT_STATS*)pMsg ;

	if( !pmsg ) return ;

	if( !HERO ) return ;

	HERO->SetStrength((DWORD)pmsg->nStr) ;
	HERO->SetDexterity((DWORD)pmsg->nDex) ;
	HERO->SetVitality((DWORD)pmsg->nVit) ;
	HERO->SetIntelligence((DWORD)pmsg->nInt) ;
	HERO->SetWisdom((DWORD)pmsg->nWis) ;
	GAMEIN->GetCharacterDialog()->SetPointLeveling(TRUE, (WORD)pmsg->nPoint);
	//CHATMGR->AddMsg(CTC_SYSMSG, "스탯이 초기화 되었습니다.") ;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1283 ) );
}


// 071208 LYW --- ItemManager : 스탯 초기화 실패 처리.
void CItemManager::Item_Reset_Stats_Nack( void* pMsg ) 
{
	//WINDOWMGR->MsgBox( NULL, MBT_OK, "스탯 초기화에 실패하였습니다." ) ;	
	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg( 1282 ) );	
}


// 071208 LYW --- ItemManager : 맵 이동 실패 처리.
void CItemManager::Item_MapMoveScroll_Nack( void* pMsg )
{
	if( !pMsg ) return ;
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	if( !pmsg ) return ;

	cMapMoveDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetMapMoveDlg() ;

	if( !pDlg ) return ;

	pDlg->SetDisableState(FALSE) ;

	WINDOWMGR->MsgBox( MBI_MAPMOVE_NOTICE, MBT_OK, CHATMGR->GetChatMsg(56) ) ;	
}


// 080312 NYJ --- ItemManager : 전체외치기 아이템사용(클릭) 처리.
void CItemManager::UseItem_WorldShout(CItem* pItem)
{
	if( !pItem ) return ;

	pItem->SetLock(TRUE) ;

	CShoutDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetShoutDlg();

	if( !pDlg ) return ;
	pDlg->SetItem(pItem);
	pDlg->SetActive(TRUE) ;
}

// 080312 NYJ --- ItemManager : 전체외치기 아이템사용 결과 처리.
//void CItemManager::Item_WorldShout_ack( void* pMsg ) 
//{
//	if(!pMsg) return;
//	SEND_ITEM_BASEINFO* pmsg = (SEND_ITEM_BASEINFO*)pMsg;
//}
void CItemManager::Item_WorldShout_Nack( void* pMsg )
{
	if(!pMsg) return ;
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	if( !pmsg ) return ;
}


void CItemManager::LoadSetScript()
{
	SetItemName setName;

	// 세트 이름을 로드하자
	{
		CMHFile file;

		if( file.Init( "data/interface/windows/setItemName.bin", "rb" ) )
		{
			for(	TCHAR line[ MAX_PATH ];
				! file.IsEOF();
				ZeroMemory( line, sizeof( line ) ) )
			{
				file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

				const TCHAR*	separator	= _T( " \"\t\n" );
				const TCHAR*	token		= _tcstok( line, separator );
				const DWORD		index		= _tcstoul( token, 0, 10 );

				if( setName.end() != setName.find( index ) )
				{
					TCHAR buffer[ 128 ];
					_stprintf( buffer, "%d번이 중복되었습니다. 나중에 나온 동일 번호의 이름은 무시됩니다", index );

					ASSERT( 0 && buffer );
					continue;
				}

				std::string &	name		= setName[ index ];
				const TCHAR*	position = line;

				while( token = _tcstok( 0, separator ) )
				{
					name		+= token;
					position	+= _tcslen( token );

					if( 0 == *( position + 1 ) )
					{
						break;
					}

					name += ' ';
				}
			}
		}
	}

	CMHFile file;

	if( ! file.Init( "system/resource/setItem.bin", "rb" ) )
	{
		ASSERT( 0 && "세트 아이템 스크립트가 없습니다: setItem.bin" );
		return;
	}

	std::list< std::string >	text;

	SetScript*		script		= 0;
	int				openCount	= 0;

	for(	TCHAR line[ MAX_PATH ];
		! file.IsEOF();
		ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( 0 == length )
		{
			continue;
		}

		// 중간의 주석 제거
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == token )
		{
			text.pop_back();
		}		
		else if( 0 == _tcsicmp( token, "set" ) )
		{
			if( script )
			{
				ASSERT( 0 && "세트 스크립트가 올바로 파싱되지 못했습니다" );
				return;
			}

			mSetScript.push_back( SetScript() );

			script = &( mSetScript.back() );

			while( token = _tcstok( 0, separator ) )
			{
				const int index = _ttoi( token );

				script->mItem.insert( index );

				if( mSetScriptMap.end() != mSetScriptMap.find( index ) )
				{
					_stprintf( line, "아이템 인덱스가 중복되었습니다. %s", text.back().c_str() );
					ASSERT( 0 && line );
					return;
				}

				mSetScriptMap[ index ] = script;
			}

			text.pop_back();
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			if( 0 == --openCount )
			{
				if( ! script )
				{
					ASSERT( 0 && "세트 스크립트에서 괄호가 맞지 않는 라인이 발견되었습니다" );
					return;
				}

				ParseSetScript( text, *script, setName );

				// 적법성 검사
				{
					if( script->mName.empty() )
					{
						ASSERT( 0 && "세트 스크립트에서 세트 이름이 지정되지 않았습니다" );
					}

					if( script->mAbility.empty() )
					{
						ASSERT( 0 && "세트 스크립트에서 부여된 능력이 없는 세트가 있습니다" );
					}

					if( script->mItem.empty() )
					{
						ASSERT( 0 && "세트 스크립트에서 아이템 인덱스 없이 지정된 세트가 있습니다" );
					}
				}

				script = 0;
				text.clear();
			}
		}
	}

	if( openCount )
	{
		ASSERT( 0 && "세트 스크립트의 {} 블록이 올바로 닫히지 않았습니다" );
	}
}


void CItemManager::ParseSetScript(std::list< std::string >& text, SetScript& data, const SetItemName& name )
{
	int					openCount	= 0;
	SetScript::Element*	element		= 0;
	TCHAR				line[ MAX_PATH ];

	for(	std::list< std::string >::const_iterator it = text.begin();
		text.end() != it;
		++it )
	{
		_tcscpy( line, it->c_str() );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );

		// 이름 파싱
		if( 0 == token )
		{
			continue;
		}

		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == _tcsicmp( "name", token ) )
		{
			token					= _tcstok( 0, separator );
			const DWORD nameIndex	= _tcstoul( token, 0, 10 );

			SetItemName::const_iterator name_it = name.find( nameIndex );

			if( name.end() != name_it )
			{
				data.mName = name_it->second;
			}
		}
		// 개수별 능력 파싱
		else if( 0 == _tcsicmp( "size", token ) )
		{
			const size_t size = _tcstoul( _tcstok( 0, separator ), 0, 10 );

			if( element ) 
			{
				ASSERT( 0 && "세트 스크립트의 블록이 올바로 닫히지 않았습니다" );
				return;
			}
			else if( data.mItem.size() < size )
			{
				_stprintf( line, "세트로 지정된 아이템 개수보다 더 많이 size가 지정되었습니다. %s", it->c_str() );
				ASSERT( 0 && line );
				continue;
			}

			element = &( data.mAbility[ size ] );

			{
				PlayerStat& stat = element->mStat;
				ZeroMemory( &stat, sizeof( stat ) );
			}
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			--openCount;

			element = 0;
		}
		else if( element )
		{
			PlayerStat& stat = element->mStat;

			if( 0 == _tcsicmp( token, "physic_attack" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mPhysicAttack.mPercent = value / 100.0f;
				}
				else
				{
					stat.mPhysicAttack.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "magic_attack" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMagicAttack.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMagicAttack.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "physic_defense" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mPhysicDefense.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mPhysicDefense.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "magic_defense" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMagicDefense.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMagicDefense.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "strength" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mStrength.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mStrength.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "dexterity" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mDexterity.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mDexterity.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "vitality" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mVitality.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mVitality.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "intelligence" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mIntelligence.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mIntelligence.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "wisdom" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mWisdom.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mWisdom.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "life" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mLife.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mLife.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "mana" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMana.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMana.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "recovery_life" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mRecoveryLife.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mRecoveryLife.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "recovery_mana" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mRecoveryMana.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mRecoveryMana.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "accuracy" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mAccuracy.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mAccuracy.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "evade" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mEvade.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mEvade.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "critical_rate" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mCriticalRate.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mCriticalRate.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "critical_damage" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mCriticalDamage.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mCriticalDamage.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "move_speed" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMoveSpeed.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMoveSpeed.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "skill" ) )
			{
				while( token = _tcstok( 0, separator ) )
				{
					const DWORD index	= _tcstoul( token, 0, 10 );
					const BYTE level	= BYTE( _ttoi( _tcstok( 0, separator ) ) );

					if( element->mSkill.end() != element->mSkill.find( index ) )
					{
						_stprintf( line, "세트 스크립트에서 스킬이 중복되었습니다. %s", it->c_str() );
						ASSERT( 0 && line );
					}

					element->mSkill[ index ] = level;
				}
			}
			else
			{
				_stprintf( line, "세트 스크립트에서 쓰지 않는 키워드가 발견되었습니다. %s", it->c_str() );
				ASSERT( 0 && line );
			}
		}
	}

	if( openCount || element )
	{
		ASSERT( 0 && "세트 스크립트의 {} 블록이 올바로 닫히지 않았습니다" );
	}
}


// 080319 LUJ, 추가 인챈트 수치를 표시함
// 080320 LUJ, 추가 인챈트 수치를 함수로 가져옴
void CItemManager::AddOptionToolTip( cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( !	info ||
			eEquipType_None == info->EquipType )
	{
		return;
	}

	// 성별제한
	switch( info->LimitGender )
	{
	case 1:
		{
			icon.AddToolTipLine( CHATMGR->GetChatMsg( 666 ), TTTC_ITEM_NAME );
			break;
		}
	case 2:
		{
			icon.AddToolTipLine( CHATMGR->GetChatMsg( 667 ), TTTC_ITEM_NAME );
			break;
		}
	}

	// 종족제한
	switch( info->LimitRace )
	{
	case 1:
		{
			icon.AddToolTipLine( CHATMGR->GetChatMsg( 668 ), TTTC_ITEM_NAME );
			break;
		}
	case 2:
		{
			icon.AddToolTipLine( CHATMGR->GetChatMsg( 669 ), TTTC_ITEM_NAME );
			break;
		}
	}

	char line[ MAX_PATH ];

	// 레벨제한
	if( info->LimitLevel )
	{
		sprintf( line, "%s%d", CHATMGR->GetChatMsg( 670 ), info->LimitLevel );
		icon.AddToolTipLine( line, TTTC_ITEM_NAME );
	}

	ITEM_OPTION base	= { 0 };
	ITEM_OPTION option	= GetOption( item.dwDBIdx );

	// 인챈트된 수치는 괄호안에 표시되지 않는다. 또한 저장될 때는 메모리 절약 및 계산 속도 향상을 위해 옵션 정보와 같이 저장된다.
	// 따라서 일단 수치를 별도로 저장한다. 그리고, 인챈트되었으면 그 만큼을 기본 수치에 더해주고, 옵션 수치에서는 빼준다.
	{
		base.mReinforce.mPhysicAttack	= info->PhysicAttack;
		base.mReinforce.mMagicAttack	= info->MagicAttack;
		base.mReinforce.mPhysicDefence	= info->PhysicDefense;
		base.mReinforce.mMagicDefence	= info->MagicDefense;
		base.mReinforce.mStrength		= info->ImprovementStr;
		base.mReinforce.mDexterity		= info->ImprovementDex;
		base.mReinforce.mVitality		= info->ImprovementVit;
		base.mReinforce.mIntelligence	= info->ImprovementInt;
		base.mReinforce.mWisdom			= info->ImprovementWis;
		base.mReinforce.mLife			= info->ImprovementLife;
		base.mReinforce.mMana			= info->ImprovementMana;

		const EnchantScript* script = GetEnchantScript( option.mEnchant.mIndex );

		if( script && option.mEnchant.mLevel )
		{
			const int value = info->EnchantDeterm * option.mEnchant.mLevel;

			AddEnchantValue( base,		script->mAbility, value );
			AddEnchantValue( option,	script->mAbility, -value );

			// 080320 LUJ,	추가 인챈트 능력치를 부여한다
			// 080421 LUJ,	추가 인챈트 수치 적용을 단순화 
			{
				ITEM_OPTION::Reinforce& reinforceOption = base.mReinforce;

				for(
					EnchantScript::Ability::const_iterator it = script->mAbility.begin();
					script->mAbility.end() != it;
					++it )
				{
					switch( EnchantScript::eType( *it ) )
					{
					case ReinforceScript::eTypeStrength:
						{
							reinforceOption.mStrength += GetBonusEnchantValue( *info, option, info->ImprovementStr );
							break;
						}
					case ReinforceScript::eTypeDexterity:
						{
							reinforceOption.mDexterity += GetBonusEnchantValue( *info, option, info->ImprovementDex );
							break;
						}
					case ReinforceScript::eTypeVitality:
						{
							reinforceOption.mVitality += GetBonusEnchantValue( *info, option, info->ImprovementVit );
							break;
						}
					case ReinforceScript::eTypeWisdom:
						{
							reinforceOption.mWisdom += GetBonusEnchantValue( *info, option, info->ImprovementWis );
							break;
						}
					case ReinforceScript::eTypeIntelligence:
						{
							reinforceOption.mIntelligence += GetBonusEnchantValue( *info, option, info->ImprovementInt );
							break;
						}
					case ReinforceScript::eTypeLife:
						{
							reinforceOption.mLife += GetBonusEnchantValue( *info, option, info->ImprovementLife );
							break;
						}
					case ReinforceScript::eTypeMana:
						{
							reinforceOption.mMana += GetBonusEnchantValue( *info, option, info->ImprovementMana );
							break;
						}
					case ReinforceScript::eTypePhysicAttack:
						{
							reinforceOption.mPhysicAttack += GetBonusEnchantValue( *info, option, info->PhysicAttack );
							break;
						}
					case ReinforceScript::eTypePhysicDefence:
						{
							reinforceOption.mPhysicDefence += GetBonusEnchantValue( *info, option, info->PhysicDefense );
							break;
						}
					case ReinforceScript::eTypeMagicAttack:
						{
							reinforceOption.mMagicAttack += GetBonusEnchantValue( *info, option, info->MagicAttack );
							break;
						}
					case ReinforceScript::eTypeMagicDefence:
						{
							reinforceOption.mMagicDefence += GetBonusEnchantValue( *info, option, info->MagicDefense );
							break;
						}
					// 080319 LUJ, 추가 인챈트 수치를 부여할 수 없는 속성들
					case ReinforceScript::eTypeMoveSpeed:
					case ReinforceScript::eTypeEvade:
					case ReinforceScript::eTypeAccuracy:
					case ReinforceScript::eTypeLifeRecovery:
					case ReinforceScript::eTypeManaRecovery:
					case ReinforceScript::eTypeCriticalRate:
					case ReinforceScript::eTypeCriticalDamage:
						{
							break;
						}
					}
				}
			}
		}		
	}

	// second: 색상
	typedef std::pair< std::string, DWORD >	TextUnit;
	typedef std::list< TextUnit >	TextList;
	TextList						firstTextList;
	
	struct
	{
		void operator() ( TextList& list, const char* title, DWORD baseValue, DWORD optionValue )
		{
			char text[ MAX_PATH ] = { 0 };

			if( optionValue )
			{
				sprintf( text, "%s %d (%+d)", title, baseValue, optionValue  );

				list.push_back( TextUnit( text, NAMECOLOR_WANTED ) );
			}
			else if( baseValue )
			{
				sprintf( text, "%s %d", title, baseValue  );
				
				list.push_back( TextUnit( text, TTTC_ITEM_NAME ) );
			}
		}
	}
	AddDefaultOption;

	AddDefaultOption( firstTextList, CHATMGR->GetChatMsg( 671 ), base.mReinforce.mPhysicAttack,		option.mReinforce.mPhysicAttack );
	AddDefaultOption( firstTextList, CHATMGR->GetChatMsg( 673 ), base.mReinforce.mPhysicDefence,	option.mReinforce.mPhysicDefence );
	AddDefaultOption( firstTextList, CHATMGR->GetChatMsg( 672 ), base.mReinforce.mMagicAttack,		option.mReinforce.mMagicAttack );
	AddDefaultOption( firstTextList, CHATMGR->GetChatMsg( 674 ), base.mReinforce.mMagicDefence,		option.mReinforce.mMagicDefence );

	// 드롭 옵션은 항목을 분리해서 표시
	{
		const ITEM_OPTION empytOption = { 0 };

		if( memcmp( &empytOption.mDrop, &option.mDrop, sizeof( empytOption.mDrop ) ) )
		{
			firstTextList.push_back( TextUnit( "", TTTC_DEFAULT ) );
		}

		const DWORD size = sizeof( option.mDrop.mValue ) / sizeof( *option.mDrop.mValue );

		for( DWORD i = 0; i < size; ++i )
		{
			const ITEM_OPTION::Drop::Value& value = option.mDrop.mValue[ i ];

			switch( value.mKey )
			{
			case ITEM_OPTION::Drop::KeyPlusStrength:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 157 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusIntelligence:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 160 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusDexterity:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 158 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusWisdom:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 161 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusVitality:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 678 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusPhysicalAttack:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 152 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 153 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusMagicalAttack:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 155 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusMagicalDefence:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 156 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusCriticalRate:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 842 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusCriticalDamage:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 843 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusAccuracy:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 162 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusEvade:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 841 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusMoveSpeed:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 840 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusLife:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 164 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusMana:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 165 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusLifeRecovery:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 846 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPlusManaRecovery:
				{
					sprintf( line, "%s %+.1f", CHATMGR->GetChatMsg( 847 ), value.mValue );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentStrength:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 157 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentIntelligence:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 160 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentDexterity:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 158 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentWisdom:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 161 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentVitality:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 678 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentPhysicalAttack:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 152 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 153 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentMagicalAttack:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 155 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentMagicalDefence:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 156 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentCriticalRate:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 842 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentCriticalDamage:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 843 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentAccuracy:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 162 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentEvade:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 841 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentMoveSpeed:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 840 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentLife:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 164 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentMana:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 165 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentLifeRecovery:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 846 ), value.mValue * 100.0f );
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentManaRecovery:
				{
					sprintf( line, "%s %+.1f%%", CHATMGR->GetChatMsg( 847 ), value.mValue * 100.0f );
					break;
				}
			default:
				{
					continue;
				}
			}

			firstTextList.push_back( TextUnit( line, RGB_HALF( 57, 187, 225 ) ) );
		}
	}

	struct
	{
		void operator() ( TextList& list, const char* title, DWORD baseValue, DWORD optionValue, DWORD mixValue )
		{
			baseValue += mixValue;

			char text[ MAX_PATH ] = { 0 };

			if( optionValue )
			{
				sprintf( text, "%s %+d (%+d)", title, baseValue, optionValue  );

				list.push_back( TextUnit( text, NAMECOLOR_WANTED ) );
			}
			else if( baseValue )
			{
				sprintf( text, "%s %+d", title, baseValue  );

				list.push_back( TextUnit( text, mixValue ? RGB_HALF( 255, 255, 0 ) : TTTC_ITEM_VALUE ) );
			}
		}
	}
	AddExtendedOption;

	TextList secondTextList;

	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 676 ), base.mReinforce.mStrength,		option.mReinforce.mStrength,		option.mMix.mStrength );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 677 ), base.mReinforce.mDexterity,		option.mReinforce.mDexterity,		option.mMix.mDexterity );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 678 ), base.mReinforce.mVitality,		option.mReinforce.mVitality,		option.mMix.mVitality );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 679 ), base.mReinforce.mIntelligence,	option.mReinforce.mIntelligence,	option.mMix.mIntelligence );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 680 ), base.mReinforce.mWisdom,			option.mReinforce.mWisdom,			option.mMix.mWisdom );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 681 ), base.mReinforce.mLife,			option.mReinforce.mLife,			0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 682 ), base.mReinforce.mMana,			option.mReinforce.mMana,			0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 846 ), base.mReinforce.mLifeRecovery,	option.mReinforce.mLifeRecovery,	0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 847 ), base.mReinforce.mManaRecovery,	option.mReinforce.mManaRecovery,	0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 840 ), base.mReinforce.mMoveSpeed,		option.mReinforce.mMoveSpeed,		0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 841 ), base.mReinforce.mEvade,			option.mReinforce.mEvade,			0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 162 ), base.mReinforce.mAccuracy,		option.mReinforce.mAccuracy,		0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 842 ), base.mReinforce.mCriticalRate,	option.mReinforce.mCriticalRate,	0 );
	AddExtendedOption( secondTextList, CHATMGR->GetChatMsg( 843 ), base.mReinforce.mCriticalDamage,	option.mReinforce.mCriticalDamage,	0 );

	// 아이콘에 툴팁 표시
	{
		for(
			TextList::const_iterator it = firstTextList.begin();
			firstTextList.end() != it;
			++it )
		{
			const std::string&	text	= it->first;
			const DWORD			color	= it->second;

			icon.AddToolTipLine( text.c_str(), color );
		}

		if( ! secondTextList.empty() )
		{
			icon.AddToolTipLine( "" );
		}

		for(
			TextList::const_iterator it = secondTextList.begin();
			secondTextList.end() != it;
			++it )
		{
			const std::string&	text	= it->first;
			const DWORD			color	= it->second;

			icon.AddToolTipLine( text.c_str(), color );
		}
	}	
}

void CItemManager::AddPetToolTip( cIcon& icon, const ITEMBASE& item )
{
	PET_OBJECT_INFO* pPetObjectInfo = PETMGR->GetPetObjectInfo( item.dwDBIdx );

	if( !pPetObjectInfo )
		return;

	icon.AddToolTipLine( "" );

	char buf[256] = {0,};

	if( pPetObjectInfo->State == ePetState_Die )
	{
		cImage dieimage;
		SCRIPTMGR->GetImage( 141, &dieimage );
		icon.SetStateImage( &dieimage );
	}
	else
	{
		icon.ClearStateImage();
	}

	sprintf( buf, CHATMGR->GetChatMsg( 1586 ), pPetObjectInfo->Level );
	icon.AddToolTipLine( buf );

	if( pPetObjectInfo->Type == ePetType_Basic )
	{
		sprintf( buf, CHATMGR->GetChatMsg( 1587 ), RESRCMGR->GetMsg( 1005 ) );
		icon.AddToolTipLine( buf );
	}
	else
	{
		sprintf( buf, CHATMGR->GetChatMsg( 1587 ), RESRCMGR->GetMsg( pPetObjectInfo->Type + 996 ) );
		icon.AddToolTipLine( buf );
	}

	sprintf( buf, CHATMGR->GetChatMsg( 1588 ), RESRCMGR->GetMsg( pPetObjectInfo->Grade + 1000 ) );			
	icon.AddToolTipLine( buf );

	sprintf( buf, CHATMGR->GetChatMsg( 1589 ), pPetObjectInfo->SkillSlot );
	icon.AddToolTipLine( buf );
}

void CItemManager::AddSetToolTip( cIcon& icon, const ITEMBASE& item )
{
	const ITEM_INFO* info	= GetItemInfo( item.wIconIdx );
	const SetScript* script = GetSetScript( item.wIconIdx );

	if( ! info ||
		! script )
	{
		return;
	}

	icon.AddToolTipLine( "" );

	CHero* hero = OBJECTMGR->GetHero();

	// 보유한 아이템을 여기에 넣는다
	std::set< DWORD > itemIndexSet;

	// 현재 보유한 아이템 인덱스를 셋에 넣는다.
	{
		BOOL isWearItem = FALSE;
		
		for( POSTYPE position = TP_WEAR_START; position < TP_WEAR_END; ++position )
		{		
			const ITEMBASE* equippedItem = GetItemInfoAbsIn( hero, position );

			if( equippedItem  )
			{
				itemIndexSet.insert( equippedItem->wIconIdx );
				
				if( position == item.Position )
				{
					isWearItem = TRUE;
				}
			}
		}

		// 착용하지 않은 아이템은 세트 아이템 목록만 보여주도록 장비 셋을 클리어한다.
		if( ! isWearItem )
		{
			itemIndexSet.clear();
		}
	}

	// 갖춰진 세트 아이템 개수
	int currentSize = 0;

	// 완성된 세트 구성을 가져오고, 장비 유무에 따라 표시한다.
	{
		typedef std::pair< std::string, DWORD > TextUnit;
		typedef std::list< TextUnit >			TextList;
		TextList								textList;

		// 일단 툴팁 메시지를 저장한다.
		for(
			SetScript::Item::const_iterator it = script->mItem.begin();
			script->mItem.end() != it;
			++it )
		{
			const DWORD			index		= *it;
			const ITEM_INFO*	itemInfo	= GetItemInfo( index );
			
			if( ! itemInfo )
			{
				continue;
			}

			const std::string name( std::string( "  " ) + itemInfo->ItemName );

			if( itemIndexSet.end() == itemIndexSet.find( index ) )
			{				
				const DWORD dissatisfiedColor	= TTTC_ITEM_LIMIT;

				textList.push_back( TextUnit( name.c_str(), dissatisfiedColor ) );
				
			}
			else
			{
				const DWORD satisfiedColor	= RGB_HALF( 0, 255, 0 );

				textList.push_back( TextUnit( name.c_str(), satisfiedColor ) );

				++currentSize;
			}
		}

		// 세트 구성 개수도 저장한다. 
		{
			char buffer[ 128 ];
			sprintf( buffer, "%s(%d/%d)" , script->mName.c_str(), currentSize, script->mItem.size() );

			const DWORD setTitleColor = RGB_HALF( 255, 255, 0 );

			textList.push_front( TextUnit( buffer, setTitleColor ) );
		}

		// 일단 이름과 구성된 아이템을 표시한다

		for(
			TextList::const_iterator it = textList.begin();
			textList.end() != it; 
			++it )
		{
			const std::string&	text	= it->first;
			const DWORD			color	= it->second;

			icon.AddToolTipLine( text.c_str(), color );
		}
	}

	// 능력을 표시한다.
	{
		icon.AddToolTipLine( "" );

		char buffer	[ MAX_TEXT_SIZE ];
		char line	[ MAX_TEXT_SIZE ] = { 0, };
		
		/// 세트효과 글자 만큼 태핑할 수 있도록 공백 문자열을 만든다
		std::string tabString;

		// 메시지 [0]세트 효과
		for(
			size_t tabSize = strlen( RESRCMGR->GetMsg( 632 ) );
			tabSize--;
			)
		{
			tabString += " ";
		}

		for(
			SetScript::Ability::const_iterator it = script->mAbility.begin();
			script->mAbility.end() != it;
			++it )
		{
			const int					size	= it->first;
			const SetScript::Element&	element = it->second;
			const DWORD					color	= size > currentSize ? TTTC_ITEM_LIMIT : RGB_HALF( 0, 255, 0 );
			
			//sprintf( buffer, "[%d]세트효과: ", size );
			sprintf( buffer, RESRCMGR->GetMsg( 633 ), size );
			const char* prefix = buffer;

			// 물리 공격
			{
				const PlayerStat::Value& value = element.mStat.mPhysicAttack;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 671 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 671 ), value.mPlus );
					strcat( line, " " );	// 파싱 버그일까? 웬일인지 위의 sprintf에서 뒤의 여백은 무시가 된다...
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}
			
			// 마법 공격
			{
				const PlayerStat::Value& value = element.mStat.mMagicAttack;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 672 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 672 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 물리 방어
			{
				const PlayerStat::Value& value = element.mStat.mPhysicDefense;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 673 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f ", prefix, CHATMGR->GetChatMsg( 673 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 마법 방어
			{
				const PlayerStat::Value& value = element.mStat.mMagicDefense;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 674 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 674 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 힘
			{
				const PlayerStat::Value& value = element.mStat.mStrength;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 676 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 676 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 민첩
			{
				const PlayerStat::Value& value = element.mStat.mDexterity;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 677 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 677 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 생명
			{
				const PlayerStat::Value& value = element.mStat.mVitality;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 678 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 678 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 지능
			{
				const PlayerStat::Value& value = element.mStat.mIntelligence;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 679 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 679 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 지혜
			{
				const PlayerStat::Value& value = element.mStat.mWisdom;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 680 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 680 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 생명
			{
				const PlayerStat::Value& value = element.mStat.mLife;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 681 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 681 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 마나
			{
				const PlayerStat::Value& value = element.mStat.mMana;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 682 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 682 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 생명 회복
			{
				const PlayerStat::Value& value = element.mStat.mRecoveryLife;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, RESRCMGR->GetMsg( 634 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, RESRCMGR->GetMsg( 634 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 마나 회복
			{
				const PlayerStat::Value& value = element.mStat.mRecoveryMana;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, RESRCMGR->GetMsg( 635 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, RESRCMGR->GetMsg( 635 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 명중
			{
				const PlayerStat::Value& value = element.mStat.mAccuracy;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, RESRCMGR->GetMsg( 282 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, RESRCMGR->GetMsg( 282 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 회피
			{
				const PlayerStat::Value& value = element.mStat.mEvade;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, RESRCMGR->GetMsg( 281 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, RESRCMGR->GetMsg( 281 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 크리티컬 확률
			{
				const PlayerStat::Value& value = element.mStat.mCriticalRate;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 842 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 842 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 크리티컬 피해
			{
				const PlayerStat::Value& value = element.mStat.mCriticalDamage;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 843 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 843 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			// 080226 LUJ, 이동 속도
			{
				const PlayerStat::Value& value = element.mStat.mMoveSpeed;

				if( value.mPercent )
				{
					sprintf( line, "%s%s %+.0f%% ", prefix, CHATMGR->GetChatMsg( 840 ), value.mPercent * 100 );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
				else if( value.mPlus )
				{
					sprintf( line, "%s%s %+.0f%", prefix, CHATMGR->GetChatMsg( 840 ), value.mPlus );
					strcat( line, " " );
					icon.AddToolTipLine( line, color );

					prefix = tabString.c_str();
				}
			}

			for(
				SetScript::Element::Skill::const_iterator it = element.mSkill.begin();
				element.mSkill.end() != it;
				++it )
			{
				const short	level	= it->second;
				const DWORD	index	= it->first / 100 * 100 + level;	
				cSkillInfo* info	=  SKILLMGR->GetSkillInfo( index );
				
				if( ! info )
				{
					continue;
				}
				else if( info->GetLevel() )
				{
					sprintf( line, "%s[%s] %s +%d ", prefix, info->GetName(), RESRCMGR->GetMsg( 636 ), info->GetLevel() );
				}
				else
				{
					sprintf( line, "%s[%s] ", prefix, info->GetName() );
				}

				icon.AddToolTipLine( line, color );
				
				prefix = tabString.c_str();
			}
		}
	}
}


int CItemManager::GetSetItemSize( CPlayer* player, const SetScript* script )
{
	if( ! script || ! player )
	{
		ASSERT( 0 );
		return 0;
	}

	int size = 0;

	for( POSTYPE part = TP_WEAR_START; part < TP_WEAR_END; ++part )
	{
		const ITEMBASE * itemBase = GetItemInfoAbsIn(player, part );

		if( itemBase &&
			itemBase->dwDBIdx )
		{
			const ITEM_INFO * itemInfo = GetItemInfo( itemBase->wIconIdx );

			if( itemInfo &&
				script->mItem.end() != script->mItem.find( itemInfo->ItemIdx ) )
			{
				++size;
			}
		}
	}

	return size;
}


const SetScript* CItemManager::GetSetScript( DWORD itemIndex ) const
{
	SetScriptMap::const_iterator it = mSetScriptMap.find( itemIndex );

	return mSetScriptMap.end() == it ? 0 : it->second;
}


// 080228 LUJ, 보호 아이템을 위해 스크립트 파싱 변경
void CItemManager::LoadMixScript()
{
	CMHFile file;

	if( ! file.Init( "System/Resource/ItemMix.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	enum Type
	{
		TypeNone,
		TypeProtection,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), ";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtection;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}

		switch( type )
		{
		case TypeProtection:
			{
				const DWORD itemIndex = atoi( token );

				mMixProtectionItemSet.insert( itemIndex );
				break;
			}
		case TypeResult:
			{
				const DWORD		itemIndex	= atoi( token );
				MixScript&		data		= mMixScriptMap[ itemIndex ];

				data.push_back( ItemMixResult() );

				ItemMixResult& result = data.back();

				result.mMaterial[ itemIndex ] = 1;

				{
					result.mItemIndex		= strtoul( strtok( 0, separator ), 0, 10 );
					result.mRequiredLevel	= WORD( atoi( strtok( 0, separator ) ) );
					result.mMoney			= strtoul( strtok( 0, separator ), 0, 10 );

					const float rate		= float( atof( strtok( 0, separator ) ) / 10000 );
					result.mSuccessRate		= min( 1.0f, rate );
				}

				while( true )
				{
					const char* token1 = strtok( 0, separator );
					const char* token2 = strtok( 0, separator );

					if( ! token1 || ! token2 )
					{
						break;
					}

					const DWORD index		= strtoul( token1, 0, 10 );
					const DWORD	quantity	= strtoul( token2, 0, 10 );

					if( 0 < quantity )
					{
						result.mMaterial[ index ] += quantity;
					}
				}

				break;
			}
		}
	}
}


const MixScript* CItemManager::GetMixScript( DWORD itemIndex ) const
{
	MixScriptMap::const_iterator it = mMixScriptMap.find( itemIndex );

	return mMixScriptMap.end() == it ? 0 : &( it->second );
};


const ItemMixResult* CItemManager::GetMixResult( DWORD itemIndex, DWORD level ) const
{
	MixScriptMap::const_iterator it = mMixScriptMap.find( itemIndex );

	if( mMixScriptMap.end() != it )
	{
		const MixScript&			mixScript	= it->second;
		MixScript::const_iterator	inner		= mixScript.begin();

		if( mixScript.size() > level )
		{
			std::advance( inner, level );

			const ItemMixResult& mixResult = *inner;

			return &mixResult;
		}
	}

	return 0;
}


DWORD CItemManager::GetItemNameColor( const ITEM_INFO& info ) const
{
	switch( info.kind )
	{
	case ITEM_INFO::eKindNormal:
	case ITEM_INFO::eKindSetNormal:
		{
			return RGB( 254, 254, 254 );
		}
	case ITEM_INFO::eKindHighClass:
	case ITEM_INFO::eKindSetHighClass:
		{
			return RGB( 0, 255, 0 );
		}
	case ITEM_INFO::eKindMasterPiece:
	case ITEM_INFO::eKindSetMasterPiece:
		{
			return RGB( 254, 150, 20 );
		}
	case ITEM_INFO::eKindRare:
	case ITEM_INFO::eKindSetRare:
		{
			return RGB( 50, 135, 245 );
		}
	case ITEM_INFO::eKindUnique:
	case ITEM_INFO::eKindSetUnique:
		{
			return RGB( 230, 50, 230 );
		}
	default:
		{
			ASSERTMSG( 0, "지정되지 않은 종류가 있습니다. 일단 툴팁 색상을 백색으로 표시합니다" );
			return RGB( 254, 254, 254 );
		}
	}
}


const DissolveScript* CItemManager::GetDissolveScript( DWORD itemIndex ) const
{
	DissolveScriptMap::const_iterator it = mDissolveScriptMap.find( itemIndex );

	return mDissolveScriptMap.end() == it ? 0 : &( it->second );
}


void CItemManager::LoadDissolveScript()
{
#ifdef _DEBUG
	const DWORD beginTime = GetTickCount();	
#endif

	CMHFile file;

	if( ! file.Init( "System/Resource/itemDissolve.bin", "rb" ) )
	{
		ASSERTMSG( 0, "분해스크립트(itemDissolve.bin) 가 없습니다" );
		return;
	}

	char buffer[ 1024 ];

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const char* separator	= "\t\n ,";
		const char* token		= strtok( buffer, separator );
		const DWORD itemIndex	= strtoul( token, 0, 10 );

		DissolveScript& script = mDissolveScriptMap[ itemIndex ];

		script.mLevel			= strtoul( strtok( 0, separator ), 0, 10 ) ;

        const DWORD staticResultSize = 5;

        for( DWORD i = 0; i < staticResultSize; ++i )
		{
			const char* token1		= strtok( 0, separator );
			const char* token2		= strtok( 0, separator );
			
			ASSERT( token1 && token2 );

			const DWORD index		= strtoul( token1, 0, 10 );
			const DWORD quantity	= strtoul( token2, 0, 10 );

			if( index && quantity )
			{
				script.mStaticResult.push_back( DissolveScript::Result() );

				DissolveScript::Result& result = script.mStaticResult.back();

				result.mItemIndex	= index;
				result.mQuantity	= quantity;
			}
		}

		// 확률에 따라 아이템이 출현하는 동적 결과는 처리하기 조금 까다롭다.
		// 다음과 같이 진행할 것이다. 먼저 파싱 결과를 저장한다. 그 값을 바탕으로 
		// 루프를 돌면서 맵에 누적시킨 값들을 저장한다(값이 10000이 넘지 않도록 주의).
		// 예를 들어 10%, 30%, 60% 확률이 있다면, 맵에 1000, 4000, 10000으로 저장한다.
		// 그리고 결과를 얻을 때는 랜덤값을 얻은 후 맵의 lower_bound()로 해당되는 값을 찾아낸다.
		{
			const DWORD maxRate = 10000;

			for( DWORD rate = 0; rate < maxRate; )
			{
				const char* token1	= strtok( 0, separator );
				const char* token2	= strtok( 0, separator );
				const char* token3	= strtok( 0, separator );

				if( ! token1 || ! token2 || ! token3 )
				{
					break;
				}

				const DWORD itemIndex	= strtoul( token1, 0, 10 );
				const DWORD quantity	= strtoul( token2, 0, 10 );
				const DWORD percent		= strtoul( token3, 0, 10 );

				if( ! itemIndex || ! quantity || ! percent )
				{
					break;
				}

				rate = min( maxRate, rate + percent );
				
				DissolveScript::Result& result = script.mDynamicResult[ rate ];

				result.mItemIndex	= itemIndex;
				result.mQuantity	= quantity;
			}

			if( !	script.mDynamicResult.empty()	&&
					script.mDynamicResult.end() == script.mDynamicResult.find( maxRate ) )
			{
				ASSERT( 0 && "분해 스크립트의 확률 값의 합이 10000이 되지 않습니다. 올바른 결과를 보장할 수 없습니다" );
			}
		}
	}

#ifdef _DEBUG
	char line[ MAX_TEXT_SIZE ];
	const float second = float( GetTickCount() - beginTime ) / 1000.0f;

	sprintf( line, "itemDissolve.bin: cost time to parse: %0.5f sec\n", second );

	OutputDebugStr( line );
#endif
}


const ReinforceScript* CItemManager::GetReinforceScript( DWORD itemIndex ) const
{
	ReinforceScriptMap::const_iterator it = mReinforceScriptMap.find( itemIndex );

	return mReinforceScriptMap.end() == it ? 0 : &( it->second );
}


const CItemManager::ReinforceScriptMap& CItemManager::GetReinforceScript() const
{
	return mReinforceScriptMap;
}


// 080228 LUJ, 보호 아이템을 위해 스크립트 파싱 변경
void CItemManager::LoadReinforceScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemReinforce.bin", "rb" ) )
	{
		ASSERTMSG( 0, "강화 아이템 스크립트(itemReinforce.bin)가 없어요" );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	enum Type
	{
		TypeNone,
		TypeProtection,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), ";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtection;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}

		switch( type )
		{
		case TypeProtection:
			{
				const DWORD itemIndex = atoi( token );
                
				mReinforceProtectionItemSet.insert( itemIndex );
				break;
			}
		case TypeResult:
			{
				const DWORD itemIndex = atoi( token );
				ASSERTMSG( mReinforceScriptMap.end() == mReinforceScriptMap.find( itemIndex ), "it's duplicated item index" );

				ReinforceScript& script = mReinforceScriptMap[ itemIndex ];

				const std::string reinforce = strtok( 0, separator );

				script.mMax		=		 atoi( strtok( 0, separator ) );
				script.mBias	= float( atof( strtok( 0, separator ) ) );
				script.mForRare	= ! strcmpi( "true", strtok( 0, separator ) );

				// 강화 가능한 아이템 종류 세팅
				{
					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_hand );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Shield );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Armor );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Helmet );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Glove );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Boots );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Ring1 );
						script.mSlot.insert( eEquipSlot_Ring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Necklace );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Earring1 );
						script.mSlot.insert( eEquipSlot_Earring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Belt );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Band );
					}
				}

				// 강화 가능한 능력 설정
				{
					if( ! strcmpi( "strength", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeStrength;
					}
					else if( ! strcmpi( "dexterity", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeDexterity;
					}
					else if( ! strcmpi( "vitality", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeVitality;
					}
					else if( ! strcmpi( "intelligence", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeIntelligence;
					}
					else if( ! strcmpi( "wisdom", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeWisdom;
					}
					else if( ! strcmpi( "mana", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMana;
					}
					else if( ! strcmpi( "recovery_mana", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeManaRecovery;
					}
					else if( ! strcmpi( "life", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeLife;
					}
					else if( ! strcmpi( "recovery_life", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeLifeRecovery;
					}
					else if( ! strcmpi( "physic_defense", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypePhysicDefence;
					}
					else if( ! strcmpi( "physic_attack", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypePhysicAttack;
					}
					else if( ! strcmpi( "magic_defense", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMagicDefence;
					}
					else if( ! strcmpi( "magic_attack", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMagicAttack;
					}
					else if( ! strcmpi( "move_speed", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMoveSpeed;
					}
					else if( ! strcmpi( "evade", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeEvade;
					}
					else if( ! strcmpi( "accuracy", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeAccuracy;
					}
					else if( ! strcmpi( "critical_rate", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeCriticalRate;
					}
					else if( ! strcmpi( "critical_damage", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeCriticalDamage;
					}
					else
					{
						ASSERTMSG( 0, "itemReinforce.bin에 정의되어 있지 않은 키워드가 있습니다" );

						mReinforceScriptMap.erase( itemIndex );
					}
				}

				break;
			}
		}
	}
}


BOOL CItemManager::IsRare( const ITEM_INFO* info ) const
{
	if( info )
	{
		switch( info->kind )
		{
		case ITEM_INFO::eKindMasterPiece:
		case ITEM_INFO::eKindSetMasterPiece:
		case ITEM_INFO::eKindRare:
		case ITEM_INFO::eKindSetRare:
		case ITEM_INFO::eKindUnique:
		case ITEM_INFO::eKindSetUnique:
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


const EnchantScript* CItemManager::GetEnchantScript( DWORD itemIndex ) const
{
	EnchantScriptMap::const_iterator it = mEnchantScriptMap.find( itemIndex );

	return mEnchantScriptMap.end() == it ? 0 : &( it->second );
}


// 080228 LUJ, 보호 아이템을 위해 스크립트 파싱 변경
// 080320 LUJ, 인챈트 수치를 제외한 보호 모드 추가
// 080328 LUJ, 인챈트 보호 아이템 레벨 및 플러스/마이너스 보너스 추가
void CItemManager::LoadEnchantScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemEnchant.bin", "rb" ) )
	{
		ASSERTMSG( 0, "아이템 업글 스크립트(itemReinforce.bin)가 없어요" );
		return;
	}

	char buffer[ MAX_PATH * 5 ] = { 0 };

	enum Type
	{
		TypeNone,
		TypeProtectionAll,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), :";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtectionAll;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}		

		switch( type )
		{
			// 080320 LUJ, 인챈트 수치를 제외하고 보호하는 상태
			// 080328 LUJ, 아이템 장착 레벨을 보호 조건에 포함시키고, 플러스/마이너스 보너스 확률맵 추가
		case TypeProtectionAll:
			{
				const DWORD itemIndex = atoi( token );

				EnchantProtection::Data& data = mEnchantProtection.mItemMap[ itemIndex ];
				ZeroMemory( &data.mEnchantLevelRange,	sizeof( data.mEnchantLevelRange ) );
				ZeroMemory( &data.mItemLimitLevelRange, sizeof( data.mItemLimitLevelRange ) );

				data.mEnchantLevelRange.mMin	= 		atoi( strtok( 0, separator ) );
				data.mEnchantLevelRange.mMax	= 		atoi( strtok( 0, separator ) );

				data.mItemLimitLevelRange.mMin	= 		atoi( strtok( 0, separator ) );
				data.mItemLimitLevelRange.mMax	= 		atoi( strtok( 0, separator ) );

				data.mMinimumEnchantLevel		=		atoi( strtok( 0, separator ) );
				data.mAddedPercent				= int(	atof( strtok( 0, separator ) ) / 100.0f * 1000.0f );

				// 080328 LUJ, 인챈트 플러스/마이너스 보너스 확률 맵을 만든다
				;
				for(
					float plusRate	= 0, minusRate = 0;
					; )
				{
					const char* token1	= strtok( 0, separator );
					const char* token2	= strtok( 0, separator );

					if( ! token1 ||
						! token2 )
					{
						break;
					}

					const int	level	=			atoi( token1 );
					const float	rate	= float(	atof( token2 ) / 100.0f );

					if( 0 < level )
					{
						data.mPlusBonusMap[ plusRate += rate ] = level;
					}
					else if( 0 > level )
					{
						data.mMinusBonusMap[ minusRate += rate ] = level;
					}
				}

				break;
			}
		case TypeResult:
			{
				const DWORD itemIndex = atoi( token );

				ASSERT( mEnchantScriptMap.end() == mEnchantScriptMap.find( itemIndex ) );

				EnchantScript& script = mEnchantScriptMap[ itemIndex ];

				script.mItemIndex		= itemIndex;
				script.mItemLevel.mMin	= atoi( strtok( 0, separator ) );
				script.mItemLevel.mMax	= atoi( strtok( 0, separator ) );
				script.mEnchantMaxLevel	= atoi( strtok( 0, separator ) );

				for(
					DWORD abilitySize = 2;
					abilitySize--;
				)
					// 강화 가능한 능력 설정
				{
					token = strtok( 0, separator );

					if( ! strcmpi( "strength", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeStrength );
					}
					else if( ! strcmpi( "dexterity", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeDexterity );
					}
					else if( ! strcmpi( "vitality", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeVitality );
					}
					else if( ! strcmpi( "intelligence", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeIntelligence );
					}
					else if( ! strcmpi( "wisdom", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeWisdom );
					}
					else if( ! strcmpi( "mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMana );
					}
					else if( ! strcmpi( "recovery_mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeManaRecovery );
					}
					else if( ! strcmpi( "life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLife );
					}
					else if( ! strcmpi( "recovery_life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLifeRecovery );
					}
					else if( ! strcmpi( "physic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicDefence );
					}
					else if( ! strcmpi( "physic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicAttack );
					}
					else if( ! strcmpi( "magic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicDefence );
					}
					else if( ! strcmpi( "magic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicAttack );
					}
					else if( ! strcmpi( "move_speed", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMoveSpeed );
					}
					else if( ! strcmpi( "evade", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeEvade );
					}
					else if( ! strcmpi( "accuracy", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeAccuracy );
					}
					else if( ! strcmpi( "critical_rate", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalRate );
					}
					else if( ! strcmpi( "critical_damage", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalDamage );
					}
					else if( ! strcmpi( "none", token ) )
					{
						// to do nothing
					}
					else
					{
						ASSERTMSG( 0, "itemEnchant.bin에 정의되어 있지 않은 키워드가 있습니다" );

						mEnchantScriptMap.erase( itemIndex );
					}
				}

				// 업글 가능한 아이템 종류 세팅
				{
					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_hand );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Shield );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Armor );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Helmet );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Glove );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Boots );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Ring1 );
						script.mSlot.insert( eEquipSlot_Ring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Necklace );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Earring1 );
						script.mSlot.insert( eEquipSlot_Earring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Belt );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Band );
					}
				}

				break;
			}
		}
	}
}


void CItemManager::AddEnchantValue( ITEM_OPTION& option, const EnchantScript::Ability& ability, int value ) const
{
	ITEM_OPTION::Reinforce& reinforce = option.mReinforce;

	for(	EnchantScript::Ability::const_iterator it = ability.begin();
			ability.end() != it;
			++it )
	{
		switch( EnchantScript::eType( *it ) )
		{
		case ReinforceScript::eTypeStrength:
			{
				reinforce.mStrength	+= value;
				break;
			}
		case ReinforceScript::eTypeDexterity:
			{
				reinforce.mDexterity	+= value;
				break;
			}
		case ReinforceScript::eTypeVitality:
			{
				reinforce.mVitality	+= value;
				break;
			}
		case ReinforceScript::eTypeWisdom:
			{
				reinforce.mWisdom	+= value;
				break;
			}
		case ReinforceScript::eTypeIntelligence:
			{
				reinforce.mIntelligence	+= value;
				break;
			}
		case ReinforceScript::eTypeLife:
			{
				reinforce.mLife	+= value;
				break;
			}
		case ReinforceScript::eTypeLifeRecovery:
			{
				reinforce.mLifeRecovery	+= value;
				break;
			}
		case ReinforceScript::eTypeMana:
			{
				reinforce.mMana	+= value;
				break;
			}
		case ReinforceScript::eTypeManaRecovery:
			{
				reinforce.mManaRecovery	+= value;
				break;
			}
		case ReinforceScript::eTypePhysicAttack:
			{
				reinforce.mPhysicAttack	+= value;
				break;
			}
		case ReinforceScript::eTypePhysicDefence:
			{
				reinforce.mPhysicDefence	+= value;
				break;
			}
		case ReinforceScript::eTypeMagicAttack:
			{
				reinforce.mMagicAttack		+= value;
				break;
			}
		case ReinforceScript::eTypeMagicDefence:
			{
				reinforce.mMagicDefence	+= value;
				break;
			}
		case ReinforceScript::eTypeMoveSpeed:
			{
				reinforce.mMoveSpeed		+= value;
				break;
			}
		case ReinforceScript::eTypeEvade:
			{
				reinforce.mEvade			+= value;
				break;
			}
		case ReinforceScript::eTypeAccuracy:
			{
				reinforce.mAccuracy		+= value;
				break;
			}
		case ReinforceScript::eTypeCriticalRate:
			{
				reinforce.mCriticalRate	+= value;
				break;
			}
		case ReinforceScript::eTypeCriticalDamage:
			{
				reinforce.mCriticalDamage	+= value;
				break;
			}
		default:
			{
				ASSERT( 0 && "invaild script. check it now" );
				break;
			}
		}
	}
}


void CItemManager::LoadApplyOptionScript()
{
	/*
	080215 LUJ, 스크립트 파싱 방식은 LoadDropOptionScrit() 참조
	*/

	CMHFile file;

	if( ! file.Init( "system/resource/ItemApplyOption.bin", "rb" ) )
	{
		return;
	}

	char line[ 1024 ] = { 0 };

	typedef std::list< std::string >	TextList;
	TextList							textList;

	enum ScriptState
	{
		ScriptNone,
		ScriptItem,
		ScriptTarget,
		ScriptTargetLevel,
		ScriptTargetType,
		ScriptOption,
		ScriptOptionSize,
		ScriptOptionType,
	}
	scriptState = ScriptNone;

	int openCount = 0;

	ApplyOptionScript* script = 0;

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = strlen( line );

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = line[ i ];
			const char b = line[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				line[ i ] = 0;
				break;
			}
		}

		textList.push_back( line );

		const char* separator	= " \n\t=,+\"~()";
		const char* token		= strtok( line, separator );
		const char* markBegin	= "{";
		const char* markEnd		= "}";

		if( ! token )
		{
			textList.pop_back();
		}
		// 새로운 클로저가 열리면 헤드를 파싱한다
		else if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			textList.pop_back();

			switch( scriptState )
			{
			case ScriptNone:
				{
					++openCount;

					scriptState = ScriptItem;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "item" ) )
						{
							const char* itemIndex = strtok( 0, separator );

							script = &( mApplyOptionScriptMap[ atoi( itemIndex ) ] );

							ZeroMemory( &( script->mLevel ), sizeof( script->mLevel ) );
						}
					}

					break;
				}
			case ScriptItem:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "target" ) )
						{
							scriptState = ScriptTarget;
						}
						else if( ! stricmp( token, "option" ) )
						{
							scriptState = ScriptOption;
						}
					}

					break;
				}
			case ScriptTarget:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "level" ) )
						{
							scriptState = ScriptTargetLevel;
						}
						else if( ! stricmp( token, "type" ) )
						{
							scriptState = ScriptTargetType;
						}
					}

					break;
				}
			case ScriptOption:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "size" ) )
						{
							scriptState = ScriptOptionSize;
						}
						else if( ! stricmp( token, "type" ) )
						{
							scriptState = ScriptOptionType;
						}
					}

					break;
				}
			}

			textList.clear();
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			textList.pop_back();

			--openCount;

			switch( scriptState )
			{
			case ScriptItem:
				{
					script		= 0;
					scriptState	= ScriptNone;
					break;
				}
			case ScriptTarget:
				{
					scriptState	= ScriptItem;
					break;
				}
			case ScriptOption:
				{
					scriptState = ScriptItem;
					break;
				}
			case ScriptTargetLevel:
				{
					scriptState	= ScriptTarget;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						script->mLevel.mMin	= atoi( strtok( line, separator ) );
						script->mLevel.mMax	= atoi( strtok( 0, separator ) );
					}

					break;
				}
			case ScriptTargetType:
				{
					scriptState	= ScriptTarget;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						eArmorType	armorType	= eArmorType_None;
						eWeaponType	weaponType	= eWeaponType_None;
						eEquipType	equipType	= eEquipType_None;
						eEquipSlot	equipSlot	= eEquipSlot_None;

						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						for(
							token = strtok( line, separator );
							token;
						token = strtok( 0, separator ) )
						{
							if( ! stricmp( token, "sword" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Sword;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Sword, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "axe" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Axe;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Axe, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "mace" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Mace;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Mace, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Dagger;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Dagger, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Bow;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Bow, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Staff;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Staff, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "armor" ) )
							{
								equipSlot	= eEquipSlot_Armor;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Armor, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "helmet" ) )
							{
								equipSlot	= eEquipSlot_Helmet;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Helmet, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "gloves" ) )
							{
								equipSlot	= eEquipSlot_Glove;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "shoes" ) )
							{
								equipSlot	= eEquipSlot_Boots;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Boots, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "shield" ) )
							{
								equipSlot	= eEquipSlot_Shield;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Shield, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "ring" ) )
							{
								equipSlot	= eEquipSlot_Ring1;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Ring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Ring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "necklace" ) )
							{
								equipSlot	= eEquipSlot_Necklace;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Necklace, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "earring" ) )
							{
								equipSlot	= eEquipSlot_Earring1;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Earring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Earring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "belt" ) )
							{
								equipSlot	= eEquipSlot_Belt;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Belt, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "band" ) )
							{
								equipSlot	= eEquipSlot_Band;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Band, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}						
							else if( ! stricmp( token, "glasses" ) )
							{
								equipSlot	= eEquipSlot_Glasses;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Glasses, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "mask" ) )
							{
								equipSlot	= eEquipSlot_Mask;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Mask, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_body" ) )
							{
								equipSlot	= eEquipSlot_Costume_Dress;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Dress, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_foot" ) )
							{
								equipSlot	= eEquipSlot_Costume_Shoes;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Shoes, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_hand" ) )
							{
								equipSlot	= eEquipSlot_Costume_Glove;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_head" ) )
							{
								equipSlot	= eEquipSlot_Costume_Head;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Head, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							// 080602 LUJ, 누락된 '날개' 아이템 설정 추가
							else if( ! stricmp( token, "wing" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Wing, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "one_hand" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ) );
							}
							else if( ! stricmp( token, "two_hand" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Staff ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Bow ) );
							}
							else if( ! stricmp( token, "gun" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Gun ) );
							}
							else if( ! stricmp( token, "two_blade" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
							}
							else if( ! stricmp( token, "robe" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Robe, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "light_armor" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Leather, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "heavy_armor" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Metal, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "all" ) )
							{
								switch( equipSlot )
								{
									// 무기
								case eEquipSlot_hand:
									{
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Bow ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Staff ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Gun ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Dagger ) );
										break;
									}
								case eEquipSlot_Armor:
								case eEquipSlot_Helmet:
								case eEquipSlot_Glove:
								case eEquipSlot_Boots:								
									{
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Robe, eWeaponAnimationType_None ) );
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Leather, eWeaponAnimationType_None ) );
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Metal, eWeaponAnimationType_None ) );
										break;
									}
								}
							}
						}
					}					

					break;
				}
			case ScriptOptionSize:
				{
					scriptState	= ScriptOption;

					float step = 0.0f;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* size	= strtok( line, separator );
						const float rate	= float( atof( strtok( 0, separator ) ) );

						if( ! size ||
							! rate )
						{
							continue;
						}

						step = step + rate / 100.0f;

						script->mOptionSizeMap.insert( std::make_pair( step, atoi( size ) ) );
					}

					break;
				}
			case ScriptOptionType:
				{
					scriptState = ScriptOption;

					ApplyOptionScript::Option option;
					ZeroMemory( &option, sizeof( option ) );

					float step = 0;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* type		= strtok( line, separator );
						const float rate		= float( atof( strtok( 0,	separator ) ) );
						const char* beginValue	= strtok( 0,	separator );
						const char* endValue	= strtok( 0,	separator );

						if( ! type		||
							! rate		||
							! beginValue	||
							! endValue	)
						{
							continue;
						}

						step = step + rate / 100.0f;

						{							
							if( '%' == beginValue[ strlen( beginValue ) - 1 ] )
							{
								option.mBeginValue	= float( atof( beginValue ) ) / 100.0f;
							}
							else
							{
								option.mBeginValue	= float( atof( beginValue ) );
							}

							if( '%' == endValue[ strlen( endValue ) - 1 ] )
							{
								option.mEndValue	= float( atof( endValue ) ) / 100.0F;
							}
							else
							{
								option.mEndValue	= float( atof( endValue ) );
							}
						}						

						if( ! stricmp( type, "plus_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusStrength;
						}
						else if( ! stricmp( type, "plus_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusIntelligence;
						}
						else if( ! stricmp( type, "plus_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusDexterity;
						}
						else if( ! stricmp( type, "plus_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusWisdom;
						}
						else if( ! stricmp( type, "plus_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusVitality;
						}
						else if( ! stricmp( type, "plus_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalAttack;
						}
						else if( ! stricmp( type, "plus_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalDefence;
						}
						else if( ! stricmp( type, "plus_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalAttack;
						}
						else if( ! stricmp( type, "plus_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalDefence;
						}
						else if( ! stricmp( type, "plus_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalRate;
						}
						else if( ! stricmp( type, "plus_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalDamage;
						}
						else if( ! stricmp( type, "plus_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusAccuracy;
						}
						else if( ! stricmp( type, "plus_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusEvade;
						}
						else if( ! stricmp( type, "plus_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMoveSpeed;
						}
						else if( ! stricmp( type, "plus_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLife;
						}
						else if( ! stricmp( type, "plus_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMana;
						}
						else if( ! stricmp( type, "plus_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLifeRecovery;
						}
						else if( ! stricmp( type, "plus_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusManaRecovery;
						}
						else if( ! stricmp( type, "percent_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentStrength;
						}
						else if( ! stricmp( type, "percent_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentIntelligence;
						}
						else if( ! stricmp( type, "percent_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentDexterity;
						}
						else if( ! stricmp( type, "percent_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentWisdom;
						}
						else if( ! stricmp( type, "percent_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentVitality;
						}
						else if( ! stricmp( type, "percent_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalAttack;
						}
						else if( ! stricmp( type, "percent_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalDefence;
						}
						else if( ! stricmp( type, "percent_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalAttack;
						}
						else if( ! stricmp( type, "percent_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalDefence;
						}
						else if( ! stricmp( type, "percent_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalRate;
						}
						else if( ! stricmp( type, "percent_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalDamage;
						}
						else if( ! stricmp( type, "percent_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentAccuracy;
						}
						else if( ! stricmp( type, "percent_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentEvade;
						}
						else if( ! stricmp( type, "percent_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMoveSpeed;
						}
						else if( ! stricmp( type, "percent_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLife;
						}
						else if( ! stricmp( type, "percent_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMana;
						}
						else if( ! stricmp( type, "percent_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLifeRecovery;
						}
						else if( ! stricmp( type, "percent_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentManaRecovery;
						}

						script->mOptionTypeMap.insert( std::make_pair( step, option ) );
					}

					break;
				}
				
			}

			textList.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}

	// 080611 LUJ, GM툴 상태에서만 표시하도록 함
#ifdef _GMTOOL_
	for(
		ApplyOptionScriptMap::const_iterator script_it = mApplyOptionScriptMap.begin();
		mApplyOptionScriptMap.end() != script_it;
	++script_it )
	{
		const DWORD					itemIndex	= script_it->first;
		const ApplyOptionScript&	script		= script_it->second;

		cprintf( "itemIndex: %d\n", itemIndex );
		cprintf( "\t target level: %d ~ %d\n", script.mLevel.mMin, script.mLevel.mMax );
		cprintf( "\t target type:\n" );


		for(
			ApplyOptionScript::TargetTypeSet::const_iterator it = script.mTargetTypeSet.begin();
			script.mTargetTypeSet.end() != it;
		++it )
		{
			cprintf( "\t\t %u\n", *it );
		}

		cprintf( "\n" );
		cprintf( "\t option size:\n" );

		for(
			ApplyOptionScript::OptionSizeMap::const_iterator it = script.mOptionSizeMap.begin();
			script.mOptionSizeMap.end() != it;
		++it )
		{
			cprintf( "\t\t %f\t %d\n", it->first, it->second );
		}

		cprintf( "\n" );
		cprintf( "\t option rate:\n" );
		cprintf( "\t\t rate\t key\t min ~ max:\n" );

		for(
			ApplyOptionScript::OptionTypeMap::const_iterator it = script.mOptionTypeMap.begin();
			script.mOptionTypeMap.end() != it;
		++it )
		{
			const ApplyOptionScript::Option& option = it->second;

			cprintf(
				"\t\t %f\t %d\t %f~%f\n",
				it->first,
				option.mKey,
				option.mBeginValue,
				option.mEndValue );
		}
	}
#endif
}


LONGLONG CItemManager::GetItemKey( eEquipSlot slot, eWeaponType weapon, eArmorType armor, eWeaponAnimationType animation ) const
{
	const short key1	= short( slot );
	const short key2	= short( weapon );
	const short key3	= short( armor );
	const short key4	= short( animation );

	LONGLONG key = 0;

	ASSERT( sizeof( key ) == sizeof( key1 ) + sizeof( key2 ) + sizeof( key3 ) + sizeof( key4 ) );

	memcpy(	( short* )&key + 0, &key1, sizeof( key1 ) );
	memcpy(	( short* )&key + 1, &key2, sizeof( key2 ) );
	memcpy(	( short* )&key + 2, &key3, sizeof( key3 ) );
	memcpy(	( short* )&key + 3, &key4, sizeof( key4 ) );

	return key;
}


BOOL CItemManager::IsApplyOptionItem( DWORD itemIndex ) const
{
	return mApplyOptionScriptMap.end() != mApplyOptionScriptMap.find( itemIndex );
}


const ApplyOptionScript* CItemManager::GetApplyOptionScript( DWORD itemIndex ) const
{
	ApplyOptionScriptMap::const_iterator it = mApplyOptionScriptMap.find( itemIndex );

	return mApplyOptionScriptMap.end() == it ? 0 : &( it->second );
}


void CItemManager::LoadCoolTimeScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemCoolTime.bin", "rb" ) )
	{
		ASSERTMSG( 0, "아이템 쿨타임 스크립트(itemCoolTime.bin)가 없어요" );
		return;
	}

	int		openCount	= 0;
	DWORD	groupIndex	= 1;

	std::list< std::string >	text;

	for(	TCHAR line[ MAX_PATH ];
			! file.IsEOF();
			ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " ~\n\t=,+\"()%" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == token ||
			0 == _tcsicmp( "group", token ) )
		{
			text.pop_back();
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			text.pop_back();

			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			text.pop_back();

			if( ! --openCount )
			{
				for(	std::list< std::string >::const_iterator it = text.begin();
						text.end() != it;
						++it )
				{
					// 파싱하자... 첫번째는 아이템 인덱스, 다음은 쿨타임
					char buffer[ MAX_PATH ];

					_tcscpy( buffer, it->c_str() );

					const DWORD itemIndex	= strtoul( _tcstok( buffer, separator ), 0, 10 );
                    const DWORD coolTime	= atoi( _tcstok( 0, separator ) ) * 1000;

					CoolTimeScript::Unit& unit = mCoolTimeScript.mItemData[ itemIndex ];

					unit.mIndex	= groupIndex;
					unit.mTime	= coolTime;

					{
						CoolTimeScript::ItemIndex& indexSet =  mCoolTimeScript.mGroupData[ groupIndex ];

						indexSet.insert( itemIndex );
					}
				}

				text.clear();
				++groupIndex;
			}
			else
			{
				ASSERT( 0 );
				return;
			}
		}
	}
}


BOOL CItemManager::IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const
{
	UsingCoolTimeGroup::const_iterator it =  mUsingCoolTimeGroup.find( playerIndex );

	if( mUsingCoolTimeGroup.end() == it )
	{
		return FALSE;
	}

	const CoolTimeGroup& group = it->second;

	return group.end() != group.find( GetCoolTimeGroupIndex( itemIndex ) );
}


BOOL CItemManager::AddCoolTime( DWORD playerIndex, DWORD itemIndex )
{
	const DWORD groupIndex = GetCoolTimeGroupIndex( itemIndex );

	if( ! groupIndex )
	{
		return FALSE;
	}

	// 해당 쿨타임 그룹을 사용중으로 처리한다.
	{
		CoolTimeGroup& group = mUsingCoolTimeGroup[ playerIndex ];

		group.insert( groupIndex );
	}

	// 큐에 넣어 사용 가능 시점이 자동으로 설정되도록 한다.
	{
		CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find( itemIndex );

		if( mCoolTimeScript.mItemData.end() == it )
		{
			ASSERT( 0 );
			return FALSE;
		}

		const CoolTimeScript::Unit& unit = it->second;

		mCoolTimeQueue.push( CoolTimeSort::Tick() );

		CoolTimeSort::Tick& tick		= mCoolTimeQueue.top();
		const DWORD			tickCount	= GetTickCount();

		tick.mCoolTimeGroup	= groupIndex;
		tick.mBegin			= tickCount;
		tick.mEnd			= tickCount + unit.mTime;
		tick.mPlayerIndex	= playerIndex;


		std::pair<DWORD, CoolTimeSort::Tick> pairData;
		pairData.first = groupIndex;
		pairData.second = tick;
		m_mapCoolTime.insert(pairData);
	}

	return TRUE;
}


DWORD CItemManager::GetCoolTimeGroupIndex( DWORD itemIndex ) const
{
	CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find( itemIndex );

	if( mCoolTimeScript.mItemData.end() == it )
	{
		return 0;
	}

	const CoolTimeScript::Unit& unit = it->second;

	return unit.mIndex;
}


void CItemManager::ProcessCoolTime()
{
	// 쿨타임 체크. 남은 시간 별로 정렬되어 있으므로 가장 상위에 있는 것만 체크한다.
	if( mCoolTimeQueue.empty() )
	{
		return;
	}

	const DWORD					tickCount	= GetTickCount();
	const CoolTimeSort::Tick&	tick		= mCoolTimeQueue.top();

	// 남은 시간 별로 정렬된 우선순위 큐의 가장 상위에 있는 유닛의 시간이 만료되었는지 체크하자
	if( ( tick.mBegin > tick.mEnd && tickCount < tick.mBegin && tickCount > tick.mEnd ) ||	// 오버플로된 상황
		( tick.mBegin < tick.mEnd && tick.mEnd < tickCount ) )								// 일반적인 경우
	{
		UsingCoolTimeGroup::iterator it = mUsingCoolTimeGroup.find( tick.mPlayerIndex );

		if( mUsingCoolTimeGroup.end() != it )
		{
			CoolTimeGroup& group = it->second;

			group.erase( tick.mCoolTimeGroup );

			if( group.empty() )
			{
				mUsingCoolTimeGroup.erase( it );
			}
		}

		// 쿨타임 해제할 아이템 그룹을 가져오자...
		CoolTimeScript::GroupData::const_iterator group_it = mCoolTimeScript.mGroupData.find( tick.mCoolTimeGroup );

		if( mCoolTimeScript.mGroupData.end() == group_it )
		{
			ASSERT( 0 );
			return;
		}

		const CoolTimeScript::ItemIndex& itemIndex = group_it->second;

		// 인벤토리와 단축창의 쿨타임 아이템을 해제한다.
		{
			CInventoryExDialog* inventoryDialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( inventoryDialog );

			cQuickSlotDlg* quickDialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
			ASSERT( quickDialog );

			// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
			cQuickSlotDlg* dialog2 = NULL ;
			dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
			ASSERT( dialog2 ) ;

			for( CoolTimeScript::ItemIndex::const_iterator it = itemIndex.begin(); itemIndex.end() != it; ++it )
			{
				const DWORD index = *it;

				inventoryDialog->SetCoolTime( index, 0 );
				quickDialog->SetCoolTime( index, 0 );

				// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
				if(dialog2)
				{
					dialog2->SetCoolTime( index, 0 ) ;
				}
			}
		}

		if(m_mapCoolTime.end() != m_mapCoolTime.find(tick.mCoolTimeGroup))
		{
			m_mapCoolTime.erase(tick.mCoolTimeGroup);
		}

		mCoolTimeQueue.pop();
	}
}


void CItemManager::RemoveCoolTime( DWORD playerIndex )
{
	mUsingCoolTimeGroup.erase( playerIndex );
}

void CItemManager::UpdateDlgCoolTime(DWORD itemIndex)
{
	if(IsCoolTime(HEROID, itemIndex))
	{
		CoolTimeScript::ItemData::const_iterator item_it = mCoolTimeScript.mItemData.find( itemIndex );

		if( mCoolTimeScript.mItemData.end() == item_it )
		{
			ASSERT( 0 );
			return;
		}

		const CoolTimeScript::Unit& unit		= item_it->second;

		CoolTimeScript::GroupData::const_iterator group_it = mCoolTimeScript.mGroupData.find( GetCoolTimeGroupIndex( itemIndex ) );

		if( mCoolTimeScript.mGroupData.end() == group_it )
		{
			ASSERT( 0 );
			return;
		}

		//const CoolTimeScript::ItemIndex& indexSet = group_it->second;

		std::map<DWORD, CoolTimeSort::Tick>::iterator iter;
		iter = m_mapCoolTime.find(GetCoolTimeGroupIndex( itemIndex ));

		if(iter != m_mapCoolTime.end())
		{
			// 인벤토리 아이콘 쿨타임 처리
			{
				CInventoryExDialog* dialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
				ASSERT( dialog );
				dialog->SetCoolTimeAbs( itemIndex, iter->second.mBegin, iter->second.mEnd, unit.mTime);
			}

			// 단축창 아이콘 쿨타임 처리
			{
				cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
				ASSERT( dialog );
				dialog->SetCoolTimeAbs( itemIndex, iter->second.mBegin, iter->second.mEnd, unit.mTime);

				// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
				cQuickSlotDlg* dialog2 = NULL ;
				dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
				ASSERT( dialog2 ) ;

				if(dialog2)
				{
					dialog2->SetCoolTimeAbs( itemIndex, iter->second.mBegin, iter->second.mEnd, unit.mTime) ;
				}
			}
		}
	}
}


void CItemManager::LoadChangeItem()
{
	CMHFile file;

	if( ! file.Init( "system/resource/changeitem.bin", "rb") )
	{
		ASSERT( 0 );
		return;
	}

	char line[ MAX_PATH ];

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = _tcslen( line );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		const char* separator	= " \t";
		const char* token		= strtok( line, separator );

		if (	token						&&
				stricmp( token, "money" )	&&
				stricmp( token, "item" )	&&
				stricmp( token, "end_kind" ) )
		{
			mChangeItemSet.insert( atoi( token ) );
		}
	}
}


BOOL CItemManager::IsChangeItem( DWORD itemIndex ) const
{
	return mChangeItemSet.end() != mChangeItemSet.find( itemIndex );
}


//=========================================================================
//	NAME : UseItem_ResetStats
//	DESC : 071203 LYW --- 스탯 초기화 아이템 처리.
//=========================================================================
void CItemManager::UseItem_ResetStats(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성을 체크한다.

	MSG_ITEM_USE_SYN msg ;																// 메시지 구조체를 선언하고 초기화 한다.
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category	= MP_ITEM ;															// 카테고리, 프로토콜, 오브젝트 아이디 세팅.
	msg.Protocol	= MP_ITEM_USE_SYN ;
	msg.dwObjectID	= HEROID ;

	msg.wItemIdx	= pItem->GetItemIdx() ;												// 아이템 인덱스와 포지션을 세팅한다.
	msg.TargetPos	= pItem->GetItemBaseInfo().Position ;

	NETWORK->Send( &msg, sizeof(MSG_ITEM_USE_SYN) ) ;									// 메시지 전송.
}





//=========================================================================
//	NAME : UseItem_ResetSkill
//	DESC : 071203 LYW --- 스킬 초기화 아이템 처리.
//=========================================================================
void CItemManager::UseItem_ResetSkill(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성 체크.

	MSG_ITEM_USE_SYN msg ;																// 메시지 구조체를 선언하고 초기화 한다.
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category	= MP_ITEM ;															// 카테고리, 프로토콜, 오브젝트 아이디를 세팅한다.
	msg.Protocol	= MP_ITEM_USE_SYN ;
	msg.dwObjectID	= HEROID ;

	msg.wItemIdx	= pItem->GetItemIdx() ;												// 아이템의 인덱스와 포지션을 세팅한다.
	msg.TargetPos	= pItem->GetItemBaseInfo().Position ;

	NETWORK->Send( &msg, sizeof(MSG_ITEM_USE_SYN) ) ;									// 메시지 전송.
}

// 080218 KTH -- Reset RegidentRegist
void CItemManager::UseItem_ResetRegistResident(CItem* pItem)
{
	if( !pItem ) return;

	MSG_ITEM_USE_SYN msg;

	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN));

	msg.Category	= MP_ITEM;
	msg.Protocol	= MP_ITEM_USE_SYN;
	msg.dwObjectID	= HEROID;
	msg.wItemIdx	= pItem->GetItemIdx();
	msg.TargetPos	= pItem->GetItemBaseInfo().Position;

	NETWORK->Send( &msg, sizeof(MSG_ITEM_USE_SYN) );
}


//=========================================================================
//	NAME : UseItem_MapMoveScroll
//	DESC : 071203 LYW --- 맵 이동 스크롤 처리.
//=========================================================================
void CItemManager::UseItem_MapMoveScroll(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성 체크.

	cMapMoveDialog* pDlg = NULL ;														// 맵 이동 창을 받는다.
	pDlg = GAMEIN->GetMapMoveDlg() ;

	if( !pDlg ) return ;																// 맵 이동 창의 무결성 체크.

	CItem* pDeleteItem = NULL ;
	pDeleteItem = pDlg->GetItemInfo() ;													// 사용 중이던 아이템 정보를 받는다.

	if( pDeleteItem )																	// 사용 중이던 아이템이 있다면,
	{
		pDeleteItem->SetLock(FALSE) ;													// 사용 중이던 아이템의 잠김을 해제한다.
	}

	pItem->SetLock(TRUE) ;																// 사용할 아이템을 잠근다.
    
	pDlg->SetItemInfo(pItem) ;															// 맵 이동창에 아이템 정보를 세팅한다.
	pDlg->SetActive(TRUE) ;																// 맵 이동창을 활성화한다.

	pDlg->GetMapListDlg()->ResetGuageBarPos() ;											// 맵 리스트를 다시 정렬한다.
}





//=========================================================================
//	NAME : UseItem_ExtendStorage
//	DESC : 071206 LYW --- 창고 확장 아이템 처리.
//=========================================================================
void CItemManager::UseItem_ExtendStorage(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성 체크.

	CInventoryExDialog* pDlg = NULL ;													// 인벤토리를 얻어온다.
	pDlg = GAMEIN->GetInventoryDialog() ;

	if( !pDlg ) return ;																// 인벤토리 무결성 체크.

	MSG_ITEM_USE_SYN msg ;																// 메시지 구조체를 선언하고 초기화 한다.
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category = MP_ITEM;																// 카테고리 및, 기타 메시지 구조체를 세팅한다.
	msg.Protocol = MP_ITEM_USE_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.TargetPos = pItem->GetPosition();
	msg.wItemIdx = pItem->GetItemIdx();

	if( IsChangeItem( pItem->GetItemIdx() ) )											// 변환 아이템이면,
	{
		pDlg->SetDisable( TRUE );														// 인벤토리를 고정시킨다.

		const DWORD changeSecond = WAITMILISECOND;										// 변환 시간을 세팅한다.

		pDlg->SetCoolTime( pItem->GetItemIdx(), changeSecond );							// 아이템에 쿨타임을 세팅한다.

		{
			cQuickSlotDlg* dialog = NULL ;												// 퀵 슬롯 다이얼로그를 받는다.
			dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG ) ;

			ASSERT( dialog ) ;															// 퀵 슬롯 다이얼로그의 무결성 체크.

			dialog->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;					// 퀵 슬롯에 쿨타임을 세팅한다.

			// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
			cQuickSlotDlg* dialog2 = NULL ;
			dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
			ASSERT( dialog2 ) ;

			if(dialog2)
			{
				dialog2->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;
			}
		}
	}
	else																				// 변환아이템이 아니면, 
	{
		NETWORK->Send( &msg, sizeof( msg ) ) ;											// 메시지를 전송한다.
	}
}




//=========================================================================
// NAME : OpenStorageByItem
// DESC : 080607 LYW --- ItemManager : 창고 소환 함수 추가.
//=========================================================================
void CItemManager::OpenStorageByItem(CItem* pItem)
{
	// 아이템 유효성 체크.
	ASSERT(pItem) ;
	if(!pItem) return ;


	// 창고 정보를 받아온다.
	CStorageDialog* pStorageDlg = NULL ;
	pStorageDlg = GAMEIN->GetStorageDialog() ;
	ASSERT(pStorageDlg) ;
	if(!pStorageDlg) return ;


	// 창고가 열린 상태인지 체크한다.
	if(pStorageDlg->IsActive())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1552) ) ;
		return ;
	}


	// 인벤토리 정보를 얻어온다.
	CInventoryExDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetInventoryDialog() ;
	ASSERT(pDlg) ;
	if(!pDlg) return ;


	// 아이템 사용 요청.
	MSG_ITEM_USE_SYN msg ;
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category = MP_ITEM ;
	msg.Protocol = MP_ITEM_USE_SYN ;
	msg.dwObjectID = HERO->GetID() ;
	msg.TargetPos = pItem->GetPosition() ;
	msg.wItemIdx = pItem->GetItemIdx() ;


	// 변환 아이템인지 체크하고, 쿨타임 등을 세팅한다.
	if( IsChangeItem( pItem->GetItemIdx() ) )
	{
		pDlg->SetDisable( TRUE ) ;

		const DWORD changeSecond = WAITMILISECOND ;

		pDlg->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;

		cQuickSlotDlg* dialog = NULL ;
		dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG ) ;

		ASSERT( dialog ) ;
		if(!dialog) return ;

		dialog->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;

		// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
		cQuickSlotDlg* dialog2 = NULL ;
		dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
		ASSERT( dialog2 ) ;
		if(!dialog2) return ;

		dialog2->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;
	}
	else
	{
		NETWORK->Send( &msg, sizeof( msg ) ) ;
	}
}





//=========================================================================
//	NAME : GetSkillPointByItem
//	DESC : 080611 LYW --- 스킬 포인트 추가 주문서 처리.
//=========================================================================
void CItemManager::GetSkillPointByItem(CItem* pItem)
{
	ASSERT(pItem) ;
	if( !pItem ) return ;																// 아이템의 무결성 체크.

	MSG_ITEM_USE_SYN msg ;																// 메시지 구조체를 선언하고 초기화 한다.
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category	= MP_ITEM ;															// 카테고리, 프로토콜, 오브젝트 아이디를 세팅한다.
	msg.Protocol	= MP_ITEM_USE_SYN ;
	msg.dwObjectID	= HEROID ;

	msg.wItemIdx	= pItem->GetItemIdx() ;												// 아이템의 인덱스와 포지션을 세팅한다.
	msg.TargetPos	= pItem->GetItemBaseInfo().Position ;

	NETWORK->Send( &msg, sizeof(MSG_ITEM_USE_SYN) ) ;									// 메시지 전송.
}





//=========================================================================
//	NAME : IsUsingItem
//	DESC : 071206 LYW --- 이미 사용하여 버프가 활성화 중인지 체크하는 함수 추가.
//=========================================================================
BOOL CItemManager::IsUsingItem(CItem* pItem)
{
	BOOL bUsing = FALSE ;																// 사용여부 결과를 담는 변수 선언 및 세팅.

	const ITEM_INFO* pInfo = pItem->GetItemInfo() ;										// 아이템 정보를 받는다.

	if( !pInfo ) return bUsing ;														// 아이템 정보의 무결성 체크.

	cActiveSkillInfo* pActiveSkillInfo = NULL ;
	pActiveSkillInfo = (cActiveSkillInfo*)SKILLMGR->GetSkillInfo(pInfo->SupplyValue) ;	// 아이템이 지원하는 액티브 스킬 정보를 받는다.

	if( !pActiveSkillInfo ) return bUsing ;												// 액티브 스킬의 무결성 체크.

	ACTIVE_SKILL_INFO* pActive_Skill_Info = NULL ;										// 액티브 스킬 구조체의 정보를 받는다.
	pActive_Skill_Info = pActiveSkillInfo->GetSkillInfo() ;

	if( !pActive_Skill_Info ) return bUsing ;											// 액티브 스킬 구조체의 무결성 체크.

	DWORD dwBufIdx = pActive_Skill_Info->Buff[0] ;										// 버프 인덱스를 받는다.

	cSkillObject* pSkill = NULL ;														// HERO의 버프스킬 정보를 받는다.
	pSkill = HERO->GetBuffSkillObject(dwBufIdx/100) ;

	if( pSkill ) bUsing = TRUE ;														// 버프 스킬 정보가 유요하면, 사용중으로 세팅.

	return bUsing ;																		// 결과 리턴.
}





//=========================================================================
//	NAME : UseItem_ItemMallItem
//	DESC : 071206 LYW --- 아이템몰 아이템 사용 함수 추가.
//=========================================================================
void CItemManager::UseItem_ItemMallItem(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성 체크.

	MAINTARGET MainTarget;																// 주 타겟을 선언한다.

	CActionTarget Target ;																// 액션타겟을 HERO로 하고, 주 타겟으로 처리한다.
	Target.InitActionTarget(HERO,NULL) ;		
	Target.GetMainTarget( &MainTarget ) ;

	DWORD dwSupplyValue = 0 ;
	dwSupplyValue = pItem->GetItemInfo()->SupplyValue ;									// 아이템이 지원하는 스킬 인덱스를 받는다.

	float fAngleDeg = 0.0f ;
	fAngleDeg = HERO->GetAngleDeg() ;													// HERO의 각도를 받는다.

	MSG_ITEM_SKILL_START_SYN msg ;														
	memset(&msg, 0, sizeof(MSG_ITEM_SKILL_START_SYN)) ;									// 메시지 구조체를 선언하고 초기화 한다.

	msg.Category	= MP_ITEM ;
	msg.Protocol	= MP_ITEM_SKILL_SYN ;												// 카테도리 등, 각 메시지 정보를 세팅한다.
	msg.dwObjectID	= HEROID ;
	msg.Idx			= pItem->GetItemIdx() ;
	msg.Pos			= pItem->GetItemBaseInfo().Position ;

	msg.SkillStartSyn.InitMsg( dwSupplyValue, &MainTarget, fAngleDeg, HEROID ) ;		// 스킬 시작 요청 메시지를 초기화한다.

	NETWORK->Send(&msg,sizeof(MSG_ITEM_SKILL_START_SYN)) ;								// 메시지를 전송한다.
}





//=========================================================================
//	NAME : UseItem_ExtendInventory
//	DESC : 071210 LYW --- 인벤토리 확장 아이템 처리.
//=========================================================================
void CItemManager::UseItem_ExtendInventory(CItem* pItem)
{
	if( !pItem ) return ;																// 아이템의 무결성을 체크한다.

	CInventoryExDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetInventoryDialog() ;												// 인벤토리를 받고, 인벤토리의 무결성을 체크한다.

	if( !pDlg ) return ;

	MSG_ITEM_USE_SYN msg ;																// 메시지 구조체를 선언하고 초기화 한다.
	memset(&msg, 0, sizeof(MSG_ITEM_USE_SYN)) ;

	msg.Category = MP_ITEM;																// 카테고리 등, 각 메시지를 세팅한다.
	msg.Protocol = MP_ITEM_INCREASE_INVENTORY_SYN ;
	msg.dwObjectID = HERO->GetID();
	msg.TargetPos = pItem->GetPosition();
	msg.wItemIdx = pItem->GetItemIdx();

	pDlg->SetDisable( TRUE );															// 인벤토리를 고정시킨다.

	if( IsChangeItem( pItem->GetItemIdx() ) )											// 변환 가능 아이템이면,
	{
		const DWORD changeSecond = WAITMILISECOND;										// 변환 시간을 세팅한다.

		pDlg->SetCoolTime( pItem->GetItemIdx(), changeSecond );							// 인벤토리에 쿨타임을 세팅한다.

		{
			cQuickSlotDlg* dialog = NULL ;
			dialog = (cQuickSlotDlg*)WINDOWMGR->GetWindowForID(QI1_QUICKSLOTDLG);		// 퀵 슬롯 다이얼로그를 받는다.
			ASSERT( dialog );

			dialog->SetCoolTime( pItem->GetItemIdx(), changeSecond );					// 퀵 슬롯 다이얼로그에 쿨타임을 적용한다.

			// 080706 LYW --- ItemManager : 확장 퀵 슬롯 추가 처리.
			cQuickSlotDlg* dialog2 = NULL ;
			dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
			ASSERT( dialog2 ) ;

			if(dialog2)
			{
				dialog2->SetCoolTime( pItem->GetItemIdx(), changeSecond ) ;
			}
		}

		{
			CProgressDialog* dialog = (CProgressDialog*)WINDOWMGR->GetWindowForID(PROGRESS_DIALOG);		// 프로그레스 다이얼로그를 받는다.
			ASSERT( dialog );

			dialog->Wait( CProgressDialog::eActionChange );								// 프로그레스 다이얼로그를 기다리도록 세팅한다.
		}
	}
	else																				// 변환 불가능 아이템이면,
	{
		NETWORK->Send( &msg, sizeof( msg ) ) ;											// 메시지를 전송한다.
	}
}





//=========================================================================
//	NAME : Item_Increase_Inventory_Ack
//	DESC : 071210 LYW --- 인벤토리 확장 성공 함수 추가.
//=========================================================================
void CItemManager::Item_Increase_Inventory_Ack( void* pMsg )
{
	if( !pMsg ) return ;																// 메시지의 무결성을 체크한다.

	MSG_DWORD* pmsg = NULL ;															// 메시지를 받는다.
	pmsg = (MSG_DWORD*)pMsg ;

	if( !pmsg ) return ;																// 메시지의 무결성 체크.

	if( pmsg->dwObjectID != HEROID ) return ;											// HERO와 무관한지 체크.

	CInventoryExDialog* pDlg = NULL ;													
	pDlg = GAMEIN->GetInventoryDialog() ;												// 인벤토리를 받고, 무결성 체크를 한다.

	if( !pDlg ) return ;

	switch(pmsg->dwData)																// 메시지 타입이 1이면,
	{
	case 1 : 
		{
			pDlg->ShowTab(2,TRUE) ;														// 인벤토리 한칸 확장 처리를 한다.
			HERO->Set_HeroExtendedInvenCount(1) ;
		}
		break ;
	case 2 :																			// 메시지 타입이 2이면,
		{
			pDlg->ShowTab(2,TRUE) ; 
			pDlg->ShowTab(3,TRUE) ; 
																						// 인벤토리 두칸 확장 처리를 한다.
			HERO->Set_HeroExtendedInvenCount(2) ;
		}
		break ;
	default : break ;
	}

	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1249) ) ;						// 인벤토리 확장을 알린다.

	// 071212 LYW --- ItemManager : 인벤토리 확장 방어코드.
	pDlg->SetDisable(FALSE) ;
}





//=========================================================================
//	NAME : Item_Increase_Inventory_Nack
//	DESC : 071210 LYW --- 인벤토리 확장 실패 함수 추가.
//=========================================================================
void CItemManager::Item_Increase_Inventory_Nack( void* pMsg )
{
	if( !pMsg ) return ;																// 메시지의 무결성 체크.

	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;															// 메시지를 받는다.

	if( !pmsg ) return ;

	switch(pmsg->dwData)
	{
	case 0 :																			// 0은 아이템 사용이 실패 하였을때 보낸다.
		{
			WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1250) ) ;
		}
		break ;

	case 1 :																			// 1은 더이상 늘릴 아이템이 없을 시에 보낸다.
		{
			WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1248) ) ;
		}
		break ;
	}

	// 071212 LYW --- ItemManager : 인벤토리 확장 방어코드.
	CInventoryExDialog* pDlg = NULL ;													// 인벤토리를 받는다.
	pDlg = GAMEIN->GetInventoryDialog() ;

	if( !pDlg ) return ;

	pDlg->SetDisable(FALSE) ;
}


//=========================================================================
//	NAME : UseItem_ChangeName
//	DESC : 이름 변경 아이템 사용을 처리하는 함수. 071226 LYW
//=========================================================================
void CItemManager::UseItem_ChangeName(char* pName)
{
	if( !pName ) return ;																// 이름의 무결성 체크.

	int nNameLength = 0 ;																// 이름의 길이를 받는다.
	nNameLength = strlen(pName) ;

	// 080124 KTH -- 피케이 모드 시에는 사용할수 없다.
	if(HERO->IsPKMode())
	{
		WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1263) ) ;					// 에러 메시지를 출력한다.
		return;
	}

	// 080610 LYW --- ItemManager : 캐릭명 변경이 8글자가 안된다고 하는 버그 수정.
	//if( nNameLength < 4 || nNameLength >= MAX_NAME_LENGTH )								// 이름 길이의 유효성을 체크.
	if( nNameLength < 4 || nNameLength >= MAX_NAME_LENGTH + 1 )								// 이름 길이의 유효성을 체크.
	{
		cEditBox * pNameBox = NULL ;
		pNameBox = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NAME_BOX) ;					// 이름 입력 박스를 받는다.

		if( pNameBox )
		{
			pNameBox->SetEditText("") ;													// 공란으로 세팅한다.
		}

		WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(19) ) ;					// 에러 메시지를 출력한다.
		return ;
	}

	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )						// 잘못 된 이름인지 필터링 체크를 한다.
	{
		cEditBox * pNameBox = NULL ;
		pNameBox = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NAME_BOX) ;					

		if( pNameBox )
		{
			pNameBox->SetEditText("") ;													// 공란으로 세팅한다.
		}

		WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(14) ) ;					// 에러 메시지를 출력한다.
		return ;
	}

	if( !FILTERTABLE->IsUsableName(pName) )												// 사용 가능한 이름인지 체크한다.
	{
		cEditBox * pNameBox = NULL ;
		pNameBox = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NAME_BOX) ;

		if( pNameBox )																	// 이름 입력란을 공란으로 세팅한다.
		{
			pNameBox->SetEditText("") ;
		}

		WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(14) ) ;					// 에러 메시지를 출력한다.
		return ;
	}

	CInventoryExDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetInventoryDialog() ;												// 인벤토리를 받는다.

	if( !pDlg ) return ;																// 인벤토리의 무결성 체크.

	CItem* pItem = NULL ;
	pItem = pDlg->Get_QuickSelectedItem() ;												// 아이템을 받는다.

	if( !pItem ) return ;																// 아이템의 무결성 체크.

	MSG_CHANGE_CHARACTER_NAME_SYN msg ;
	memset(&msg, 0, sizeof(MSG_CHANGE_CHARACTER_NAME_SYN)) ;							// 메시지 구조체를 선언하고 초기화 한다.

	msg.Category = MP_ITEM ;
	msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_SYN ;
	msg.dwObjectID = HEROID ;															// 카테고리, 등의 정보를 세팅한다.

	strcpy(msg.changeName, pName) ;														// 이름을 복사한다.

	msg.TargetPos = pItem->GetPosition() ;												// 아이템 정보를 세팅한다.
	msg.wItemIdx  = pItem->GetItemIdx() ;

	pItem->SetLock(FALSE) ;
	pDlg->Set_QuickSelectedItem(NULL) ;													// 아이템의 잠김을 해제한다.

	NETWORK->Send(&msg, sizeof(msg)) ;													// 메시지를 전송한다.

	cChangeNameDialog* pNameBox = NULL ;
	pNameBox = GAMEIN->GetChangeNameDlg() ;												// 이름 변경창을 받는다.

	if( !pNameBox ) return ;															// 이름 변경차의 무결성 체크.

	pNameBox->SetActive(FALSE) ;														// 이름 변경창을 비활성화 한다.
}


//=========================================================================
//	NAME : Item_Change_Character_Name_Ack
//	DESC : 이름 변경 아이템 사용 성공을 처리하는 함수. 071226 LYW
//=========================================================================
void CItemManager::Item_Change_Character_Name_Ack( void* pMsg )
{
	/*if( !pMsg ) return ;

	MSG_CHANGE_CHARACTER_NAME_ACK* pmsg = NULL ;
	pmsg = (MSG_CHANGE_CHARACTER_NAME_ACK*)pMsg ;

	if( !pmsg ) return ;

	CPlayer* pPlayer = NULL ;
	pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID) ;

	if( !pPlayer ) return ;

	pPlayer->SetObjectName( pmsg->changeName ) ;*/

	//HERO->SetObjectName(pmsg->changeName) ;

	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1254) ) ;				// 이름 변경을 위한 로그아웃을 알린다.
}


//=========================================================================
//	NAME : Item_Change_Character_Name_Nack
//	DESC : 이름 변경 아이템 사용 실패를 처리하는 함수. 071226 LYW
//=========================================================================
void CItemManager::Item_Change_Character_Name_Nack( void* pMsg ) 
{
	if( !pMsg ) return ;																// 메시지 무결성 체크.

	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;															// 메시지를 받는다.

	if( !pmsg ) return ;																// 메시지의 무결성 체크.

	int nMsgNum = 0 ;																	// 메시지 번호를 담을 변수 선언.

	switch( pmsg->dwData )																// 메시지의 타입에 따라, 
	{
	case 0 :nMsgNum = 1255 ;	break ;													// 이름 변경 실패,
	case 2 :nMsgNum = 1259 ;	break ;													// (패밀리) 변경 실패
	case 3 :nMsgNum = 1258 ;	break ;													// (파티) 변경 실패
	case 4 :nMsgNum = 1257 ;	break ;													// (길드) 변경 실패
	case 5 :nMsgNum = 1256 ;	break ;													// (중복) 변경 실패
	default : return ;
	}

	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg( nMsgNum ) ) ;					// 에러 메시지를 띄운다.
}


// 080228 LUJ, 조합 보호 아이템 여부
BOOL CItemManager::IsMixProtectionItem( const ITEMBASE& item ) const
{
	return mMixProtectionItemSet.end() != mMixProtectionItemSet.find( item.wIconIdx );
}


// 080228 LUJ, 강화 보호 아이템 여부
BOOL CItemManager::IsReinforceProtectionItem( const ITEMBASE& item ) const
{
	return mReinforceProtectionItemSet.end() != mReinforceProtectionItemSet.find( item.wIconIdx );
}


// 080228 LUJ, 인챈트 보호 조건을 반환
const CItemManager::EnchantProtection& CItemManager::GetEnchantProtection() const
{
	return mEnchantProtection;
}

void CItemManager::LoadItemEffectList()
{
	CMHFile file;

	if( ! file.Init("Data/Script/Direct/EnchantSetting.bin", "rb" ) )
	{
		return;
	}

	while( ! file.IsEOF() )
	{
		EnchantEffect* pEffect = new EnchantEffect;

		pEffect->ItemIdx = file.GetDword();
		
		if ( mEffectList.GetData( pEffect->ItemIdx ) )
		{
			continue;
		}

		for( int i =0; i < 13; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				pEffect->Effect[ i ][ j ] = file.GetWord();
			}
		}

		mEffectList.Add( pEffect, pEffect->ItemIdx );
	}
}

EnchantEffect* CItemManager::GetItemEffect( DWORD ItemIdx )
{
	return (EnchantEffect*)mEffectList.GetData( ItemIdx );
}

// 080414 LUJ, 외양 변환 스크립트를 읽는다
void CItemManager::LoadBodyChangeScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/ItemBodyChange.bin", "rb" ) )
	{
		return;
	}

	char line[ MAX_PATH * 5 ] = { 0 };

	enum ScriptState
	{
		StateNone,
		StateItem,
		StateRace,
		StatePart,
		StateGender,
	}
	scriptState = StateNone;
	
	int						openCount	= 0;
	BodyChangeScriptList*	scriptList	= 0;
	BodyChangeScript		script		= { 0 };

	typedef std::list< std::string >	TextList;
	TextList							textList;

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = strlen( line );

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = line[ i ];
			const char b = line[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				line[ i ] = 0;
				break;
			}
		}

		textList.push_back( line );

		const char* separator	= " \n\t=,+\"~()";
		const char* token		= strtok( line, separator );
		const char* markBegin	= "{";
		const char* markEnd		= "}";

		if( ! token )
		{
			textList.pop_back();
		}
		// 새로운 클로저가 열리면 헤드를 파싱한다
		else if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			textList.pop_back();

			switch( scriptState )
			{
			case StateNone:
				{
					++openCount;

					scriptState = StateItem;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "item" ) )
						{
							const char* itemIndex = strtok( 0, separator );

							scriptList = &( mBodyChangeScriptListMap[ atoi( itemIndex ) ] );
						}
					}

					break;
				}
			case StateItem:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "elf" ) )
						{
							scriptState		= StateRace;
							script.mRace	= RaceType_Elf;
						}
						else if( ! stricmp( token, "human" ) )
						{
							scriptState		= StateRace;
							script.mRace	= RaceType_Human;
						}
					}

					break;
				}
			case StateRace:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "hair" ) )
						{
							scriptState		= StatePart;							
							script.mPart	= ePartType_Hair;
						}
						else if( ! stricmp( token, "face" ) )
						{
							scriptState		= StatePart;							
							script.mPart	= ePartType_Face;
						}
					}

					break;
				}
			case StatePart:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "female" ) )
						{
							scriptState		= StateGender;							
							script.mGender	= GENDER_FEMALE;
						}
						else if( ! stricmp( token, "male" ) )
						{
							scriptState		= StateGender;							
							script.mGender	= GENDER_MALE;
						}
					}

					break;
				}				
			}

			// 080511 LUJ, 스크립트가 올바로 파싱되도록 수정
			textList.clear();
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			textList.pop_back();

			--openCount;

			switch( scriptState )
			{
			case StateItem:
				{
					ZeroMemory( &script, sizeof( script ) );
					scriptList	= 0;

					scriptState	= StateNone;					
					break;
				}
			case StateRace:
				{
					scriptState	= StateItem;
					break;
				}
			case StatePart:
				{
					scriptState	= StateRace;
					break;
				}
			case StateGender:
				{
					scriptState	= StatePart;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						for(
							const char* key = strtok( line, separator );
							key;
							key = strtok( 0, separator ) )
						{
							const char* value = strtok( 0, separator );

							if( ! value )
							{
								continue;
							}

							if( ! stricmp( "value", key ) )
							{
								script.mValue	= atoi( value );
							}
							else if( ! stricmp( "text", key ) )
							{
								script.mText	= atoi( value );
							}
						}

						if( scriptList )
						{
							scriptList->push_back( script );
						}						
					}
					
					break;
				}
			}

			textList.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}
}

// 080414 LUJ, 아이템에 해당하는 외양 변환 스크립트를 반환한다.
const CItemManager::BodyChangeScriptListMap& CItemManager::GetBodyChangeScriptListMap() const
{
	return mBodyChangeScriptListMap;
}

void CItemManager::LoadGiftEvent()
{
	CMHFile file;
	if(!file.Init("System/Resource/Event_get_list.bin", "rb"))
		return;

	char Token[256];
	char* buf;
	int len;

	while(1)
	{
		if(file.IsEOF())
			break;

		file.GetString( Token );
		if( strcmp(Token,"#EventInfo") == 0 )
		{
			while(1)
			{
				if(file.IsEOF())
					break;

				EVENTINFO* pInfo = new EVENTINFO;

				pInfo->Index = file.GetDword();
				pInfo->Delay = file.GetDword();
				pInfo->Next  = file.GetDword();
				pInfo->Item  = file.GetDword();
				pInfo->Count = file.GetDword();

				file.GetString();

				buf = file.GetStringInQuotation();
				len = strlen( buf );
				pInfo->Msg = new char[ len + 1];
				strcpy( pInfo->Msg, buf );
				pInfo->Msg[ len ] = 0;

				mEventInfoTable.Add( pInfo, pInfo->Index );
			}
		}
	}
}

void CItemManager::Item_Gift_Event_Notify( void* pMsg )
{
	MSG_DWORD2* pmsg = ( MSG_DWORD2* )pMsg;

	DWORD eventidx = pmsg->dwData1;
	DWORD state = pmsg->dwData2;

	EVENTINFO* pInfo = mEventInfoTable.GetData( eventidx );

	if( !pInfo )
		return;

	switch( state )
	{
	case 1:	// 이벤트 시작
		{
			DWORD time = pInfo->Delay * 1000;

			m_GiftEventTime = time;
		}
		break;
	case 2: // 이벤트 완료 아이템 지급
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, pInfo->Msg) ;

			GAMEIN->GetItemPopupDlg()->SetText( pInfo->Msg );
			GAMEIN->GetItemPopupDlg()->SetActive( TRUE );
		}
		break;
	}
}
