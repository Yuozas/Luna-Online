// QuickManager.cpp: implementation of the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickManager.h"
#include "ObjectManager.h"

#include "InventoryExDialog.h"

#include "GameIn.h"
#include "cQuickItem.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ItemManager.h"
#include "cSkillTreeManager.h"
#include "./Interface/cIcon.h"
#include "./Input/UserInput.h"
#include "ChatManager.h"

#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/info/skillinfo.h"
#include "QuickDlg.h"

// 070216 LYW --- QuickManager : Include header file of conduct part.
#include "ConductManager.h"
#include "ConductInfo.h"
#include "../[cc]skill/client/info/activeskillinfo.h"
#include "cSkillBase.h"

//#include "QuickSlotDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuickManager);

CQuickManager::CQuickManager()
{
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_QUICKITEM_STARTINDEX);
}

CQuickManager::~CQuickManager()
{
	// desc_hseos_ASSERT수정_01
	// S ASSERT수정 추가 added by hseos 2007.06.01
	// ..CGameIn::Release 에서 QUICKMGR->Release(); 로 Release 하고 있는데,
	// ..여기서 다시 Release 하고 있었음. 수정
	// Release();
	// E ASSERT수정 추가 added by hseos 2007.06.01
	m_IconIndexCreator.Release();
}

// 080702 LYW --- QuickManager : 확장슬롯 추가 처리.
cQuickItem* CQuickManager::NewQuickItem( WORD tab, WORD pos )
{
	cQuickItem* pQuickItem;
	pQuickItem = new cQuickItem( tab, pos );

	pQuickItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, NULL, m_IconIndexCreator.GenerateIndex());
	WINDOWMGR->AddWindow(pQuickItem);	

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//mQuickItemTable[ tab ][ pos ] = pQuickItem;
	if(tab < MAX_SLOTPAGE)
	{
		mQuickItemTable[ tab ][ pos ] = pQuickItem;
	}
	else
	{
		mExQuickItemTable[ tab-MAX_SLOTPAGE ][ pos ] = pQuickItem;
	}

	return pQuickItem;
}

// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
void CQuickManager::AddQuickItem( WORD tab, WORD pos, SLOT_INFO* pSlot )
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//cQuickItem* pQuickItem = mQuickItemTable[ tab ][ pos ];
	cQuickItem* pQuickItem = NULL ;
	if(tab < MAX_SLOTPAGE)
	{
		pQuickItem = mQuickItemTable[ tab ][ pos ];
	}
	else
	{
		pQuickItem = mExQuickItemTable[ tab-MAX_SLOTPAGE ][ pos ];
	}

	if(!pQuickItem) return ;

	// 070221 LYW --- QuickManager : Call function to ClearLink.
	pQuickItem->ClearLink();

	memcpy( pQuickItem->GetSlotInfo(), pSlot, sizeof( SLOT_INFO ) );

	//GetImage( pQuickItem );

	// 070221 LYW --- QuickManager : Call function to clear data.
	pQuickItem->ClearData() ;

	switch( pSlot->kind )
	{
	case QuickKind_None:
		{
		}
		break;
	case QuickKind_SingleItem:
		{
			CItem* pItem = ITEMMGR->GetItem( pSlot->dbIdx );

			// 071126 LYW --- QuickManager : 봉인 아이템 처리.
			if( !pItem ) return ;

			if( pItem )
			{
				pQuickItem->SetCoolTime( pItem->GetRemainedCoolTime() );
			}

			const ITEMBASE& pItemBase = pItem->GetItemBaseInfo() ;

			if( pItemBase.nSealed == eITEM_TYPE_SEAL )
			{
				pQuickItem->SetSeal(TRUE) ;
			}

			pQuickItem->SetLinkItem( pItem );
		}
		break;
	case QuickKind_MultiItem:
		{
			CItem* item = ITEMMGR->GetItem( pSlot->dbIdx );

			if( item )
			{
				pQuickItem->SetCoolTime( item->GetRemainedCoolTime() );
			}

			WORD dur = ( WORD )( GAMEIN->GetInventoryDialog()->GetTotalItemDurability( pSlot->idx ) );
			pQuickItem->SetSlotData( dur );
		}
		break;
	case QuickKind_Skill:
		{
			const DWORD tabSize		= 8;
			const DWORD itemSize	= 10;
			const BYTE	level		= BYTE( pSlot->data );
			const DWORD	index		= pSlot->idx;

			// 인덱스를 키로 해서 맵에 저장하고
			// 세트 스킬이 갱신될 때 해당 스킬 정보를 갱신 요청할 것이다.

			cSkillBase* pSkillBase = SKILLTREEMGR->GetSkill( index );

			if( pSkillBase )
			{
				// 080303 ggomgrak --- SlotData Update
				pQuickItem->SetSlotData((WORD)pSkillBase->GetLevel());
				pQuickItem->SetLinkItem( ( CBaseItem* )pSkillBase ); 
				break;
			}

			//ASSERT(0);
			// 세트 아이템으로 인해 보유하지 않은 스킬을 단축창에 등록할 수 있다.
			// 이 경우에도 툴팁은 표시되어야 한다.
			cSkillInfo* info = SKILLMGR->GetSkillInfo( index + level - 1 );

			if( info )
			{
				cImage imgToolTip;
				SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );
				pQuickItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

				char line[64];
				wsprintf(line, "[%s]", info->GetName() );

				pQuickItem->AddToolTipLine( line, TTTC_SKILLNAME );

				if( info->GetKind() != SKILLKIND_PASSIVE )
				{
					const ACTIVE_SKILL_INFO* skill = ( ( cActiveSkillInfo* )info )->GetSkillInfo();

					if( ! skill )
					{
						return;
					}
					// 080616 LUJ, 상대적으로 소요되는 마나 양 표시
					else if( skill->mMana.mPercent )
					{
						sprintf( line,
							"%s%0.f%%",
							CHATMGR->GetChatMsg( 768 ),
							skill->mMana.mPercent * 100.0f );
					}
					// 080616 LUJ, 절대적으로 소요되는 마나 양 표시
					else if( skill->mMana.mPlus )
					{
						sprintf( line,
							"%s%0.f",
							CHATMGR->GetChatMsg( 768 ),
							skill->mMana.mPlus );
					}

					pQuickItem->AddToolTipLine( line, TTTC_SKILLLEVEL );

					float cool = ( ( cActiveSkillInfo* )info )->GetSkillInfo()->CoolTime / 1000.f;		//KES 070530 : 1000 --> 1000.f ??	//ncf
					sprintf(line, CHATMGR->GetChatMsg( 776 ), cool);

					pQuickItem->AddToolTipLine( line, TTTC_SKILLLEVEL );
				}

				ITEMMGR->AddToolTip( pQuickItem, info->GetTooltip() );
			}
		}
		break;

		// 070220 LYW --- QuickManager : Add case part for conduct.
	case QuickKind_Conduct :
		{
			// 070220 LYW --- QuickManager : Link item to quick slot.
			//CItem* pItem = (CItem*)CONDUCTMGR->NewConductIcon( (BYTE)pSlot->idx ) ;
			//pQuickItem->SetLinkItem( ( CBaseItem* )pItem ) ;
		}
		break ;
	case QuickKind_PetItem:
		{
		}
		break;
	case QuickKind_PetSkill:
		{
		}
		break;
	}

	GetImage( pQuickItem );
	ITEMMGR->AddToolTip( pQuickItem );
}

// 071126 LYW --- QuickManager : 퀵 아이템 반환 함수.
cQuickItem* CQuickManager::GetQuickItem(WORD tab, WORD pos)
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//if( tab < 0 || tab >= 8 ) return NULL ;
	//if( pos < 0 || pos >= 10 ) return NULL ;

	if( tab < 0 || tab >= MAX_SLOTPAGE ) return NULL ;
	if( pos < 0 || pos >= MAX_SLOTNUM ) return NULL ;

	cQuickItem* pQuickItem = NULL ;

	if(tab < MAX_SLOTPAGE)
	{
		pQuickItem = mQuickItemTable[ tab ][ pos ] ;
	}
	else
	{
		pQuickItem = mExQuickItemTable[ tab-MAX_SLOTPAGE ][ pos ] ;
	}

	return pQuickItem ;
}

void CQuickManager::RemoveQuickItem( WORD tab, WORD pos )
{
	// 080703 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//cQuickItem* pQuickItem = mQuickItemTable[ tab ][ pos ];

	//// 071127 LYW --- QuickManager : 봉인 마크 해제 처리.
	//if( !pQuickItem ) return ;
	//pQuickItem->SetSeal(FALSE) ;

	//pQuickItem->ClearLink();
	//pQuickItem->SetCoolTime( 0 );

	cQuickItem* pQuickItem = NULL ;
	if(tab < MAX_SLOTPAGE)
	{
		pQuickItem = mQuickItemTable[ tab ][ pos ];
	}
	else
	{
		pQuickItem = mExQuickItemTable[ tab-MAX_SLOTPAGE ][ pos ];
	}

	if( !pQuickItem ) return ;

	pQuickItem->SetSeal(FALSE) ;

	pQuickItem->ClearLink();
	pQuickItem->SetCoolTime( 0 );
}

void CQuickManager::ChangeQuickItem( WORD tab1, WORD pos1, WORD tab2, WORD pos2 )
{
	// 080703 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//cQuickItem* pQuickItem1 = mQuickItemTable[ tab1 ][ pos1 ];
	//cQuickItem* pQuickItem2 = mQuickItemTable[ tab2 ][ pos2 ];

	//SLOT_INFO info1;
	//SLOT_INFO info2;

	//memcpy( &info1, pQuickItem1->GetSlotInfo(), sizeof( SLOT_INFO ) );
	//memcpy( &info2, pQuickItem2->GetSlotInfo(), sizeof( SLOT_INFO ) );

	//RemoveQuickItem( tab1, pos1 );
	//RemoveQuickItem( tab2, pos2 );

	//AddQuickItem( tab1, pos1, &info2 );
	//AddQuickItem( tab2, pos2, &info1 );

	cQuickItem* pQuickItem1 = NULL ;
	if(tab1 < MAX_SLOTPAGE)
	{
		pQuickItem1 = mQuickItemTable[ tab1 ][ pos1 ];
	}
	else
	{
		pQuickItem1 = mExQuickItemTable[ tab1-MAX_SLOTPAGE ][ pos1 ];
	}

	if(!pQuickItem1) return ;

	cQuickItem* pQuickItem2 = NULL ;
	if(tab2 < MAX_SLOTPAGE)
	{
		pQuickItem2 = mQuickItemTable[ tab2 ][ pos2 ];
	}
	else
	{
		pQuickItem2 = mExQuickItemTable[ tab2-MAX_SLOTPAGE ][ pos2 ];
	}

	SLOT_INFO info1;
	SLOT_INFO info2;

	memcpy( &info1, pQuickItem1->GetSlotInfo(), sizeof( SLOT_INFO ) );
	memcpy( &info2, pQuickItem2->GetSlotInfo(), sizeof( SLOT_INFO ) );

	RemoveQuickItem( tab1, pos1 );
	RemoveQuickItem( tab2, pos2 );

	AddQuickItem( tab1, pos1, &info2 );
	AddQuickItem( tab2, pos2, &info1 );
}

void CQuickManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_QUICK_INFO:
		{
			Quick_Info( pMsg ) ;
		}
		break;
	case MP_QUICK_ADD_ACK:
		{
			Quick_Add_Ack( pMsg ) ;
		}
		break;
	case MP_QUICK_REMOVE_ACK:
		{
			Quick_Remove_Ack( pMsg ) ;
		}
		break;
	case MP_QUICK_CHANGE_ACK:
		{
			Quick_Change_Ack( pMsg ) ;
		}
		break;
	}
}


void CQuickManager::UseQuickItem( WORD tab, WORD pos )
{
	// 080704 LYW --- QuickManager : 확장 슬롯 추가 처리.
	cQuickItem* pQuickItem = NULL ;
	if(tab < MAX_SLOTPAGE)
	{
		pQuickItem = mQuickItemTable[ tab ][ pos ];
	}
	else
	{
		pQuickItem = mExQuickItemTable[ tab-MAX_SLOTPAGE ][ pos ];
	}

	if(!pQuickItem) return;

	switch( pQuickItem->GetSlotInfo()->kind )
	{
	case QuickKind_None:
		{
			return;
		}
		break;
	case QuickKind_SingleItem:
		{
			CItem* pItem = ITEMMGR->GetItem( pQuickItem->GetSlotInfo()->dbIdx );

			if( pItem )
			{
				GAMEIN->GetInventoryDialog()->UseItem( pItem );
			}
		}
		break;
	case QuickKind_MultiItem:
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForIdx( pQuickItem->GetSlotInfo()->idx );

			if( pItem )
			{
				GAMEIN->GetInventoryDialog()->UseItem( pItem );
			}

		}
		break;
	case QuickKind_Skill:
		{
			cSkillBase* pSkillBase = SKILLTREEMGR->GetSkill( pQuickItem->GetSlotInfo()->idx );

			if( pSkillBase )
			{
				const SKILL_BASE* pBase = pSkillBase->GetSkillBase();
				SKILLMGR->OnSkillCommand( pBase->wSkillIdx - 1 + pBase->Level );
			}
		}
		break;

		// 070220 LYW --- QuickManager : Add case part to use conduct item.
	case QuickKind_Conduct :
		{
			CONDUCTMGR->OnConductCommand( pQuickItem->GetSlotInfo()->idx ) ;
		}
		break ;
	case QuickKind_PetItem:
		{
		}
		break;
	case QuickKind_PetSkill:
		{
			if( !HEROPET )
				return;
			
			CItem* pItem = ITEMMGR->GetItem( pQuickItem->GetSlotInfo()->dbIdx );
			if( !pItem )
				return;

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pQuickItem->GetSlotInfo()->idx );
			if( !pInfo )
				return;

			DWORD target = OBJECTMGR->GetSelectedObjectID();

			DWORD skillindex = HEROPET->GetRealSkillIndex( pInfo->SupplyValue );

			if( target )
			{
				SKILLMGR->OnPetSkillCommand( skillindex );
			}
		}
		break;

	}
}

void CQuickManager::Release()
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//cQuickItem* pQuick;
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		// desc_hseos_ASSERT수정_01
	//		// S ASSERT수정 추가 added by hseos 2007.06.01
	//		// ..Release 하기 전에 NULL 로 설정하고 있었음.
	//		// ..Release 한 후 NULL 로 설정
	//		/*
	//		pQuick = mQuickItemTable[ sheet ][ pos ];
	//		mQuickItemTable[ sheet ][ pos ] = NULL;
	//		ReleaseQuickItem( pQuick );
	//		*/
	//			pQuick = mQuickItemTable[ sheet ][ pos ];
	//		ReleaseQuickItem( pQuick );
	//		mQuickItemTable[ sheet ][ pos ] = NULL;
	//		// E ASSERT수정 추가 added by hseos 2007.06.01
	//	}
	//}

	cQuickItem* pQuick;
	cQuickItem* pQuick2 ;

	for( WORD sheet = 0; sheet < TOTAL_SLOTPAGE; sheet++ )
	{
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			// 첫번째 슬롯 해제 처리.
			if(sheet < MAX_SLOTPAGE)
			{
				pQuick = mQuickItemTable[ sheet ][ pos ];
				ReleaseQuickItem( pQuick );
				mQuickItemTable[ sheet ][ pos ] = NULL;
			}
			else
			{
				pQuick2 = mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ];
				ReleaseQuickItem( pQuick2 );
				mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ] = NULL;
			}
		}
	}
}

void CQuickManager::RefreshQickItem()
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//cQuickItem* pQuick;
	//WORD dur = 0;
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		pQuick = mQuickItemTable[ sheet ][ pos ];

	//		if( pQuick->GetSlotInfo()->kind == QuickKind_MultiItem )
	//		{
	//			pQuick = mQuickItemTable[ sheet ][ pos ];
	//			dur	= ( WORD )( GAMEIN->GetInventoryDialog()->GetTotalItemDurability( pQuick->GetSlotInfo()->idx ) );

	//			//if( dur <= 0 )
	//			//{
	//			//	RemoveQuickItem(sheet, pos) ;

	//			//	cQuickSlotDlg* pSlot = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( sheet ) ;

	//			//	pSlot->DeleteQuickItem(sheet, pos) ;

	//			//	continue ;
	//			//}

	//			if( dur <= 0 )
	//			{
	//				pQuick->SetZeroCount(TRUE) ;
	//			}
	//			else
	//			{
	//				pQuick->SetZeroCount(FALSE) ;
	//			}

	//			pQuick->SetSlotData( dur );
	//		}
	//	}
	//}

	CInventoryExDialog* pInven = NULL ;
	pInven = GAMEIN->GetInventoryDialog() ;

	if(!pInven) return ;

	SLOT_INFO* pSlotInfo ;

	cQuickItem* pQuick;
	WORD dur = 0;

	for( WORD sheet = 0; sheet < TOTAL_SLOTPAGE; sheet++ )
	{
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			if(sheet < MAX_SLOTPAGE)
			{
				pQuick = mQuickItemTable[ sheet ][ pos ];
			}
			else
			{
				pQuick = mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ];
			}

			if(!pQuick) continue ;

			if( pQuick->GetSlotInfo()->kind == QuickKind_MultiItem )
			{
				pSlotInfo = NULL ;
				pSlotInfo = pQuick->GetSlotInfo() ;

				if(!pSlotInfo) continue ;

				dur	= ( WORD )( pInven->GetTotalItemDurability( pSlotInfo->idx ) );

				if( dur <= 0 )
				{
					pQuick->SetZeroCount(TRUE) ;
				}
				else
				{
					pQuick->SetZeroCount(FALSE) ;
				}

				pQuick->SetSlotData( dur );
			}
		}
	}
}

// 071215 LYW --- QuickManager : 퀵슬롯에 등록 된 아이템이 인벤에서 삭제될 때, 퀵 아이템을 삭제하는 함수 추가.
//
// 퀵 슬롯에 등록 된 아이템이 인벤토리에서 삭제될 때, 퀵슬롯에 등록된 퀵 아이템도 삭제해야 한다. 
// 봉인관련 아이템이나, 인챈트, 강화에 의해 기본 아이템정보에서 변경이 된 아이템들의 툴팁이 뿌려지고 있는상황에서, 
// 인벤에서만 아이템이 삭제될 경우, 이전에 진행하고 있던 처리과정의 정보들을 얻지 못해, 에러가 발생하기 때문이다.
//
void CQuickManager::DeleteLinkdedQuickItem(DWORD dwDBIdx)
{
	SLOT_INFO* pSlotInfo ;

	cQuickItem* pQuick ;
	WORD dur = 0 ;

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		pQuick = NULL ;
	//		pQuick = mQuickItemTable[ sheet ][ pos ] ;

	//		if( pQuick->GetSlotInfo()->kind != QuickKind_SingleItem ) continue ; 

	//		pQuick = mQuickItemTable[ sheet ][ pos ] ;
	//		
	//		if( !pQuick ) continue ;

	//		pSlotInfo = NULL ;
	//		pSlotInfo = pQuick->GetSlotInfo() ;

	//		if( !pSlotInfo ) continue ;

	//		if(pSlotInfo->dbIdx != dwDBIdx) continue ;

	//		RemoveQuickItem(sheet, pos) ;
	//	}
	//}

	for( WORD sheet = 0; sheet < TOTAL_SLOTPAGE; sheet++ )
	{
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			pQuick = NULL ;
			if(sheet < MAX_SLOTPAGE)
			{
				pQuick = mQuickItemTable[ sheet ][ pos ] ;
			}
			else
			{
				pQuick = mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ] ;
			}

			if( !pQuick ) continue ;

			if( pQuick->GetSlotInfo()->kind != QuickKind_SingleItem ) continue ; 			

			pSlotInfo = NULL ;
			pSlotInfo = pQuick->GetSlotInfo() ;

			if( !pSlotInfo ) continue ;

			if(pSlotInfo->dbIdx != dwDBIdx) continue ;

			RemoveQuickItem(sheet, pos) ;
		}
	}
}

// 071215 LYW --- QuickManager : 봉인된 아이템이 링크가 걸려있을 경우,
// 봉인이 풀렸을 경우 봉인을 풀어주기 위한 함수 추가.
void CQuickManager::UnsealQuickItem(DWORD dwDBIdx)
{
	SLOT_INFO* pSlotInfo ;

	cQuickItem* pQuick ;
	WORD dur = 0 ;

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		pQuick = NULL ;
	//		pQuick = mQuickItemTable[ sheet ][ pos ] ;

	//		if( pQuick->GetSlotInfo()->kind != QuickKind_SingleItem ) continue ; 

	//		pQuick = mQuickItemTable[ sheet ][ pos ] ;
	//		
	//		if( !pQuick ) continue ;

	//		pSlotInfo = NULL ;
	//		pSlotInfo = pQuick->GetSlotInfo() ;

	//		if( !pSlotInfo ) continue ;

	//		if(pSlotInfo->dbIdx != dwDBIdx) continue ;

	//		pQuick->SetSeal(FALSE) ;
	//	}
	//}


	for( WORD sheet = 0; sheet < TOTAL_SLOTPAGE; sheet++ )
	{
		// 첫번째 슬롯 봉인 해제 처리.
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			pQuick = NULL ;
			if(sheet < MAX_SLOTPAGE)
			{
				pQuick = mQuickItemTable[ sheet ][ pos ] ;
			}
			else
			{
				pQuick = mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ] ;
			}

			if( !pQuick ) continue ;

			if( pQuick->GetSlotInfo()->kind != QuickKind_SingleItem ) continue ; 			

			pSlotInfo = NULL ;
			pSlotInfo = pQuick->GetSlotInfo() ;

			if( !pSlotInfo ) continue ;

			if(pSlotInfo->dbIdx != dwDBIdx) continue ;

			pQuick->SetSeal(FALSE) ;
		}
	}
}

void CQuickManager::ReleaseQuickItem(cQuickItem * quick)
{
	if( quick == NULL )
	{
		ASSERT(0);
		return;
	}
	m_IconIndexCreator.ReleaseIndex(quick->GetID());
	WINDOWMGR->AddListDestroyWindow( quick );
}

void CQuickManager::GetImage( cQuickItem * pQuickItem )
{
	cImage lowImage;
	cImage highImage;

	SLOT_INFO* pInfo = pQuickItem->GetSlotInfo();

	switch( pInfo->kind )
	{
	case QuickKind_None:
		{
			pQuickItem->SetImage2( NULL, NULL );
		}
		break;
	case QuickKind_SingleItem:
	case QuickKind_MultiItem:
	case QuickKind_PetItem:
	case QuickKind_PetSkill:
		{
			SCRIPTMGR->GetImage( ITEMMGR->GetItemInfo( pInfo->idx )->Image2DNum, &lowImage, PFT_ITEMPATH );
			pQuickItem->SetImage2( &lowImage, NULL );
		}
		break;
	case QuickKind_Skill:
		{
			cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pInfo->idx - 1 + pInfo->data );
			if( pSkillInfo == NULL )
			{
				ASSERT( 0 );
				return;
			}

			if( pSkillInfo->GetImage() != -1 )
				SCRIPTMGR->GetImage( pSkillInfo->GetImage(), &highImage, PFT_SKILLPATH );

			pQuickItem->SetImage2( &lowImage, &highImage );
		}
		break;
		// 070216 LYW --- QuickManager : Add case part for conduct to GetImage function.
	case QuickKind_Conduct :
		{
			cConductInfo* pConductInfo = CONDUCTMGR->GetConductInfo( pInfo->idx ) ;

			if( pConductInfo == NULL )
			{
				ASSERT(0) ;
				return ;
			}

			if( pConductInfo->GetHighImage() != -1 )
				SCRIPTMGR->GetImage( pConductInfo->GetHighImage(), &highImage, PFT_CONDUCTPATH ) ;

			pQuickItem->SetImage2( &lowImage, &highImage ) ;
		}
		break ;
	}
}

BOOL CQuickManager::CanEquip(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_QUICKITEM)
	{
		return TRUE;
	}
	else if(pIcon->GetType() == WT_ITEM)
	{
		CItem * pItem = (CItem *)pIcon;

		if( pItem->GetItemInfo()->Category == eItemCategory_Equip )
		{
			return TRUE;
		}

		if( pItem->GetItemInfo()->Category == eItemCategory_Expend )
		{
			return TRUE;
		}

		return FALSE;
		/*		eITEM_KINDBIT bits = pItem->GetItemKind();
		if(bits & eYOUNGYAK_ITEM)
		{
		return TRUE;
		}
		else if(bits & eSKILL_ITEM)
		{
		if( ITEMMGR->CanConvertToSkill(pItem->GetItemIdx()) )
		return TRUE;
		else
		{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
		return FALSE;
		}
		}
		else if(bits & eEQUIP_ITEM)
		{
		if( ITEMMGR->CanEquip(pItem->GetItemIdx()) )
		return TRUE;
		else
		{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
		return FALSE;
		}

		}
		else if(bits & eEXTRA_ITEM)
		{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
		return FALSE;
		}
		else if(bits & eSHOP_ITEM)
		{
		if(bits==eSHOP_ITEM_CHARM || bits==eSHOP_ITEM_HERB ||
		bits==eSHOP_ITEM_INCANTATION || bits==eSHOP_ITEM_SUNDRIES ||
		bits==eSHOP_ITEM_PET)
		return TRUE;
		}
		else if(bits & eQUEST_ITEM_PET)
		{
		return TRUE;
		}
		return FALSE;
		*/
	}
	else if( pIcon->GetType() == WT_SKILL )
	{
		cSkillBase * pSkill = (cSkillBase *)pIcon;

		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pSkill->GetSkillBase()->wSkillIdx );

		if( pSkillInfo->GetKind() == SKILLKIND_PASSIVE || pSkillInfo->GetKind() == SKILLKIND_BUFF )
		{
			return FALSE;
		}

		return TRUE;
	}
	// 070216 LYW --- QuickManager : Add conduct part to CanEquip function.
	else if( pIcon->GetType() == WT_CONDUCT )
	{
		return TRUE ;
	}

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
	return FALSE;
}


BOOL CQuickManager::CheckQPosForItemIdx( DWORD ItemIdx )
{
	cQuickItem* pQuick;

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		pQuick = mQuickItemTable[ sheet ][ pos ];

	//		if( pQuick->GetSlotInfo()->idx == ItemIdx )
	//		{
	//			return FALSE;
	//		}
	//	}
	//}

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	for( WORD sheet = 0; sheet < MAX_SLOTPAGE; sheet++ )
	{
		for( WORD pos = 0; pos < TOTAL_SLOTPAGE; pos++ )
		{
			if(sheet < MAX_SLOTPAGE)
			{
				pQuick = mQuickItemTable[ sheet ][ pos ];
			}
			else
			{
				pQuick = mExQuickItemTable[ sheet-MAX_SLOTPAGE ][ pos ];
			}

			if( pQuick->GetSlotInfo()->idx == ItemIdx )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}


//=================================================================================================
//	NAME		: Quick_Info()
//	PURPOSE		: Process network message of quick manager part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CQuickManager::Quick_Info( void* pMsg )
{
	MSG_QUICK_INFO* pmsg = ( MSG_QUICK_INFO* )pMsg;

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//for( WORD tab = 0; tab < 8; tab++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		AddQuickItem( tab, pos, &( pmsg->tab[ tab ].slot[ pos ] ) );
	//	}
	//}

	for( WORD tab = 0; tab < TOTAL_SLOTPAGE; tab++ )
	{
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			if(tab < MAX_SLOTPAGE)
			{
				AddQuickItem( tab, pos, &( pmsg->tab[ tab ].slot[ pos ] ) );
			}
			else
			{
				AddQuickItem( tab, pos, &( pmsg->tab[ tab ].slot[ pos ] ) );
			}
		}
	}
}


//=================================================================================================
//	NAME		: Quick_Add_Ack()
//	PURPOSE		: Process network message of quick manager part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CQuickManager::Quick_Add_Ack( void* pMsg )
{
	MSG_QUICK_ADD_SYN* pmsg = ( MSG_QUICK_ADD_SYN* )pMsg;

	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//AddQuickItem( pmsg->tabNum, pmsg->pos, &( pmsg->slot ) );
	if(pmsg->tabNum < MAX_SLOTPAGE)
	{
		AddQuickItem( pmsg->tabNum, pmsg->pos, &( pmsg->slot ) );
	}
	else
	{
		AddQuickItem( pmsg->tabNum, pmsg->pos, &( pmsg->slot ) );
	}
}


//=================================================================================================
//	NAME		: Quick_Remove_Ack()
//	PURPOSE		: Process network message of quick manager part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CQuickManager::Quick_Remove_Ack( void* pMsg )
{
	MSG_QUICK_REMOVE_SYN* pmsg = ( MSG_QUICK_REMOVE_SYN* )pMsg;

	RemoveQuickItem( pmsg->tabNum, pmsg->pos );
}





// 071206 LYW --- QuickManager : 퀵슬롯을 모두 비우는 함수 추가.
void CQuickManager::Quick_Remove_AllSkill()
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
	//for( int count = 0 ; count < 8 ; ++count )
	//{
	//	for(int count2 = 0 ; count2 < 10 ; ++count2 )
	//	{
	//		cQuickItem* pQuickItem = NULL ;
	//		pQuickItem = mQuickItemTable[ count ][ count2 ] ;

	//		if( !pQuickItem ) continue ;

	//		const SLOT_INFO* pInfo = pQuickItem->GetSlotInfo() ;

	//		ASSERT( pInfo ) ;

	//		if( QuickKind_Skill == pInfo->kind )
	//		{
	//			pQuickItem->ClearLink() ;
	//			pQuickItem->SetCoolTime( 0 ) ;
	//		}
	//	}
	//}

	for( int count = 0 ; count < MAX_SLOTPAGE ; ++count )
	{
		for(int count2 = 0 ; count2 < MAX_SLOTNUM ; ++count2 )
		{
			// 첫번째 슬롯 삭제 처리.
			cQuickItem* pQuickItem = NULL ;

			if( count < MAX_SLOTPAGE)
			{
				pQuickItem = mQuickItemTable[ count ][ count2 ] ;
			}
			else
			{
				pQuickItem = mExQuickItemTable[ count-MAX_SLOTPAGE ][ count2 ] ;
			}

			if( !pQuickItem ) continue ;

			const SLOT_INFO* pInfo = pQuickItem->GetSlotInfo() ;

			ASSERT( pInfo ) ;

			if( QuickKind_Skill == pInfo->kind )
			{
				pQuickItem->ClearLink() ;
				pQuickItem->SetCoolTime( 0 ) ;
			}
		}
	}
}





//=================================================================================================
//	NAME		: Quick_Change_Ack()
//	PURPOSE		: Process network message of quick manager part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CQuickManager::Quick_Change_Ack( void* pMsg )
{
	MSG_QUICK_CHANGE_SYN* pmsg = ( MSG_QUICK_CHANGE_SYN* )pMsg;

	ChangeQuickItem( pmsg->tabNum1, pmsg->pos1, pmsg->tabNum2, pmsg->pos2 );
}

void CQuickManager::AddToolTip( cQuickItem* pQuickItem )
{
	ITEMMGR->AddToolTip( pQuickItem );
}


void CQuickManager::RefreshSkillItem( DWORD skillIndex )
{
	// 080702 LYW --- 확장 슬롯 추가 처리.
	//const DWORD tabSize		= 8;
	//const DWORD itemSize	= 10;

	const DWORD tabSize		= TOTAL_SLOTPAGE;
	const DWORD itemSize	= MAX_SLOTNUM;

	for( DWORD i = 0; i < tabSize; ++i )
	{
		for( DWORD j = 0; j < itemSize; ++j )
		{
			cQuickItem* item = NULL ;
			if(i < MAX_SLOTPAGE)
			{
				item = mQuickItemTable[ i ][ j ];
			}
			else
			{
				item = mExQuickItemTable[ i-MAX_SLOTPAGE ][ j ];
			}

			if( ! item )
			{
				continue;
			}

			// 080303 ggomgrak --- 아래에서 info라는 이름의 변수가 또 사용되어서 pSlot으로 변경함.
			//const SLOT_INFO* info = item->GetSlotInfo();
			//ASSERT( info );
			const SLOT_INFO* pSlot = item->GetSlotInfo();
			ASSERT( pSlot );

			if( QuickKind_Skill != pSlot->kind )
			{
				continue;
			}

			const DWORD index		= pSlot->idx;
			const BYTE	level		= BYTE( pSlot->data );
			cSkillBase* pSkillBase	= SKILLTREEMGR->GetSkill( index );

			if( pSkillBase )
			{
				// 080303 ggomgrak --- SlotData Update
				item->SetSlotData((WORD)pSkillBase->GetLevel());
				item->SetLinkItem( pSkillBase ); 
			}
			else
			{
				// 세트 아이템으로 인해 보유하지 않은 스킬을 단축창에 등록할 수 있다.
				// 이 경우에도 툴팁은 표시되어야 한다.
				cSkillInfo* info = SKILLMGR->GetSkillInfo( index + level - 1 );

				if( info )
				{
					cImage imgToolTip;
					SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );
					item->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

					char line[64];
					wsprintf(line, "[%s]", info->GetName() );

					item->AddToolTipLine( line, TTTC_SKILLNAME );

					if( info->GetKind() != SKILLKIND_PASSIVE )
					{
						const ACTIVE_SKILL_INFO* skill = ( ( cActiveSkillInfo* )info )->GetSkillInfo();

						if( ! skill )
						{
							return;
						}
						// 080616 LUJ, 상대적으로 소요되는 마나 양 표시
						else if( skill->mMana.mPercent )
						{
							sprintf(
								line,
								"%s%0.0f%%",
								CHATMGR->GetChatMsg( 768 ),
								skill->mMana.mPercent * 100.0f );
						}
						// 080616 LUJ, 절대적으로 소요되는 마나 양 표시
						else if( skill->mMana.mPlus )
						{
							sprintf(
								line,
								"%s%0.0f",
								CHATMGR->GetChatMsg( 768 ),
								skill->mMana.mPlus );
						}

						item->AddToolTipLine( line, TTTC_SKILLLEVEL );

						float cool = ( ( cActiveSkillInfo* )info )->GetSkillInfo()->CoolTime / 1000.f;		//KES 070530 : 1000 --> 1000.f ??	//ncf
						sprintf(line, CHATMGR->GetChatMsg( 776 ), cool);

						item->AddToolTipLine( line, TTTC_SKILLLEVEL );
					}

					ITEMMGR->AddToolTip( item, info->GetTooltip() );
				}
				else
				{
					ASSERT( 0 );
				}
			}

			GetImage( item );
		}
	}
}
