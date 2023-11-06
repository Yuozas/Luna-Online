// QuestExecute_Item.cpp: implementation of the CQuestExecute_Item class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "QuestExecute_Item.h"													// 퀘스트 실행 아이템 헤더를 불러온다.

#include "QuestScriptLoader.h"													// 퀘스트 스크립트 로더 헤더를 불러온다.

#ifdef _MAPSERVER_																// 맵 서버가 선언되어 있다면,

#include "Player.h"																// 플레이어 헤더를 불러온다.

#include "QuestGroup.h"															// 퀘스트 그룹 헤더를 불러온다.

#include "ItemManager.h"														// 아이템 매니져 헤더를 불러온다.

#include "Quest.h"																// 퀘스트 헤더를 불러온다.

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자 함수.
CQuestExecute_Item::CQuestExecute_Item( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwItemKind = m_dwItemNum = m_dwItemProbability = m_dwWeaponKind = 0;		// 아이템 종류, 아이템 숫자, 아이템 속석, 무기 타입 등을 0으로 세팅한다.

	switch( dwExecuteKind )														// 실행 종류를 확인한다.
	{
	case eQuestExecute_GiveItem:												// 아이템을 주는 실행 이거나,
	case eQuestExecute_GiveQuestItem:											// 퀘스트 아이템을 주는 실행이라면,
		{
			GetScriptParam( m_dwItemKind, pTokens );							// 아이템 종류를 받는다.
			GetScriptParam( m_dwItemNum, pTokens );								// 아이템 수를 받는다.
		}
		break;

	case eQuestExecute_GiveMoney:												// 머니를 주는 실행이거나,
	case eQuestExecute_TakeMoney:												// 머니를 받는 실행이거나,
	case eQuestExecute_TakeExp:													// 경험치를 받는 실행이거나,
	case eQuestExecute_TakeSExp:												// SP를 받는 실행이라면,
		{
			GetScriptParam( m_dwItemNum, pTokens );								// 아이템 수를 받는다.
		}
		break;

	case eQuestExecute_TakeQuestItem:											// 퀘스트 아이템을 받는 실행이거나,
	case eQuestExecute_TakeItem:												// 아이템을 받는 실행이라면,
		{
			GetScriptParam( m_dwItemKind, pTokens );							// 아이템 종류를 받는다.
			GetScriptParam( m_dwItemNum, pTokens );								// 아이템 수를 받는다.
			GetScriptParam( m_dwItemProbability, pTokens );						// 아이템 속성을 받는다.
		}
		break;

	case eQuestExecute_TakeQuestItemFQW:										// 퀘스트 특정 무기를 받는 실행이거나,
	case eQuestExecute_TakeQuestItemFW:											// 퀘스트 무기를 받는 실행이면,
		{
			GetScriptParam( m_dwItemKind, pTokens );							// 아이템 종류를 받는다.
			GetScriptParam( m_dwItemNum, pTokens );								// 아이템 수를 받는다.
			GetScriptParam( m_dwItemProbability, pTokens );						// 아이템 속성을 받는다.
			GetScriptParam( m_dwWeaponKind, pTokens );							// 무기 종류를 받는다.
		}
		break;
		
	case eQuestExecute_TakeMoneyPerCount:										// 카운트당 머니를 받는 실행이면,
		{
			GetScriptParam( m_dwItemKind, pTokens );							// 아이템 종류를 받는다.
			GetScriptParam( m_dwItemNum, pTokens );								// 아이템 수를 받는다.
		}
		break;
	}
}

// 소멸자 함수.
CQuestExecute_Item::~CQuestExecute_Item()										
{
}

// 실행 함수.
BOOL CQuestExecute_Item::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_																// 맵 서버가 선언되어 있다면,

	switch( m_dwExecuteKind )													// 실행 종류를 확인한다.
	{
	case eQuestExecute_TakeQuestItem:											// 퀘스트 아이템을 받는 실행이면,
		{
			pQuestGroup->TakeQuestItem( pPlayer, m_dwQuestIdx, m_dwSubQuestIdx, // 퀘스트 그룹의 퀘스트 아이템을 받는 함수를 호출한다.
				m_dwItemKind, m_dwItemNum, m_dwItemProbability );
		}
		break;

	case eQuestExecute_GiveQuestItem:											// 퀘스트 아이템을 주는 실행이면,
		{
			//pQuestGroup->GiveQuestItem( pPlayer, m_dwQuestIdx, m_dwItemKind,	// 퀘스트 그룹의 퀘스트 아이템을 주는 함수를 호출한다.
			//	m_dwItemNum );

			if( !pQuestGroup->GiveQuestItem( pPlayer, m_dwQuestIdx, m_dwItemKind, m_dwItemNum )) return FALSE ;
		}
		break;

	case eQuestExecute_GiveItem:												// 아이템을 주는 실행이면,
		{
			//if( !pQuestGroup->GiveItem( pPlayer, m_dwItemKind, m_dwItemNum ) )	// 아이템 반납이 실패하면,
			//{
			//	return FALSE ;													// false를 리턴한다.
			//}

			if( !pQuestGroup->GiveItem( pPlayer, m_dwItemKind, m_dwItemNum, pQuest->GetQuestIdx()) )
			{
				return FALSE ;
			}
		}
		break;

	case eQuestExecute_GiveMoney:												// 머니를 주는 실행이면,
		{
			pQuestGroup->GiveMoney( pPlayer, m_dwItemNum );						// 퀘스트 그룹의 머니를 주는 함수를 호출한다.
		}
		break;

	case eQuestExecute_TakeItem:												// 아이템을 받는 실행이면,
		{
			return pQuestGroup->TakeItem( pPlayer, m_dwItemKind,			// 퀘스트 그룹의 아이템을 받는 함수를 호출하여 결과를 리턴한다.
				m_dwItemNum, m_dwItemProbability );
		}
		break;

	case eQuestExecute_TakeMoney:												// 머니를 받는 실행이면,
		{
			pQuestGroup->TakeMoney( pPlayer, m_dwItemNum );						// 퀘스트 그룹의 머니를 받는 함수를 호출한다.
		}
		break;

	case eQuestExecute_TakeExp:													// 경험치를 받는 실행이면,
		{
			pQuestGroup->TakeExp( pPlayer, m_dwItemNum );						// 퀘스트 그룹의 경험치를 받는 함수를 호출한다.
		}
		break;

	case eQuestExecute_TakeSExp:												// SP를 받는 실행이면,
		{
			pQuestGroup->TakeSExp( pPlayer, m_dwItemNum );						// 퀘스트 그룹의 SP를 받는 함수를 호출한다.
		}
		break;

	case eQuestExecute_TakeQuestItemFQW:										// 퀘스트 특정 무기를 받는 실행이면,
		{
			pQuestGroup->TakeQuestItemFromQWeapon( pPlayer, m_dwQuestIdx,		// 특정 무기로 부터 퀘스트 아이템을 받는 함수를 호출한다.
				m_dwSubQuestIdx, m_dwItemKind, m_dwItemNum, m_dwItemProbability, 
				m_dwWeaponKind );
		}
		break;

	case eQuestExecute_TakeQuestItemFW:											// 무기로 부터 퀘스트 아이템을 받는 실행이면,
		{
			pQuestGroup->TakeQuestItemFromWeapon( pPlayer, m_dwQuestIdx,		// 무기로 부터 퀘스트 아이템을 받는 함수를 호출한다.
				m_dwSubQuestIdx, m_dwItemKind, m_dwItemNum, m_dwItemProbability, 
				m_dwWeaponKind );
		}
		break;

	case eQuestExecute_TakeMoneyPerCount:										// 카운트당 머니를 받는 실행이면,
		{
			pQuestGroup->TakeMoneyPerCount( pPlayer, m_dwQuestIdx, m_dwSubQuestIdx, m_dwItemKind, m_dwItemNum );	// 카운트 당 머니를 받는 함수를 호출한다.
		}
		break;
	}
#endif
	return TRUE;
}





int CQuestExecute_Item::CheckCondition( PLAYERTYPE* pPlayer, 
							CQuestGroup* pQuestGroup, CQuest* pQuest )			// 퀘스트 이벤트를 실행하기 위한 조건을 만족하는지 체크하는 함수.
{
	int nErrorCode = e_EXC_ERROR_NO_ERROR ;										// 에러 코드를 담을 변수를 선언하고 e_EXE_ERROR_NO_ERROR로 세팅한다.

	if( !pPlayer )																// 플레이어 정보가 유효하지 않으면, 
	{
		nErrorCode = e_EXC_ERROR_NO_PLAYERINFO ;								// 플레이어 정보가 없다는 에러 코드를 세팅한다.
		return nErrorCode ;														// 에러 코드를 return 처리를 한다.
	}

	if( !pQuestGroup )															// 퀘스트 그룹 정보가 유효하지 않으면,
	{
		nErrorCode = e_EXC_ERROR_NO_QUESTGROUP ;								// 퀘스트 그룹 정보가 없다는 에러 코드를 세팅한다.
		return nErrorCode ;														// 에러 코드를 return 처리를 한다.
	}

	if( !pQuest )																// 퀘스트 정보가 유효하지 않다면,
	{
		nErrorCode = e_EXC_ERROR_NO_QUEST ;										// 퀘스트 정보가 없다는 에러 코드를 세팅한다.
		return nErrorCode ;														// 에러 코드를 return 처리를 한다.
	}

#ifdef _MAPSERVER_																// 맵 서버가 선언되어 있다면,

	switch(m_dwExecuteKind)														// 실행 종류를 확인한다.
	{
	case eQuestExecute_TakeQuestItem :											// 퀘스트 아이템을 얻는 실행문이면,
		{
			return nErrorCode ;													// 에러 코드를 return 처리를 한다.

			// 현재 플레이어의 퀘스트 아이템 슬롯의 정보를 받아오는 부분이 
			// 없으므로, 우선은 에러 없음으로 처리를 하자.

			//CItemSlot* pSlot = NULL ;											// 아이템 슬롯 정보를 받을 포인터를 선언하고 null 처리를 한다.
			//pSlot = pPlayer->GetSlot(eItemTable_Inventory) ;					// 플레이어의 인벤토리 스롯 정보를 받는다.

			//if( !pSlot )														// 인벤토리 정보가 유효하지 않으면,
			//{
			//	nErrorCode = e_EXC_ERROR_NO_INVENTORYINFO ;						// 인벤토리 정보가 없다는 에러코드를 세팅한다.
			//	return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			//}

			//WORD EmptyCellPos[255];											// 빈 슬롯 위치를 받을 변수를 선언한다.
			//WORD EmptyCellNum;												// 빈 슬롯 개수를 받을 변수를 선언한다.

			//WORD wResult = 0 ;												// 빈 슬롯의 체크 결과 값을 받을 변수를 선언하고 0으로 세팅한다.

			//wResult = ITEMMGR->GetCanBuyNumInSpace(pPlayer, pSlot,			// 아이템을 받을 공간이 충분한지 결과 값을 받는다.
			//	m_dwItemKind, m_dwItemNum, EmptyCellPos, EmptyCellNum) ;

			//if( wResult == 0 )												// 결과 값이 0과 같으면,
			//{
			//	nErrorCode = e_EXC_ERROR_NO_EXTRASLOT ;							// 에러 코드를 인벤토리 여분이 없다는 에러 코드를 세팅한다.
			//	return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			//}
		}
		break ;

	case eQuestExecute_GiveQuestItem:											// 퀘스트 아이템을 반납 실행이면,
		{
			QUESTITEM* pQuestItem = NULL ;										// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null 처리를 한다.
			pQuestItem = pQuestGroup->GetQuestItem(m_dwItemKind) ;				// 퀘스트 아이템 인덱스로 퀘스트 아이템 정보를 받는다.

			if( !pQuestItem )													// 퀘스트 아이템 정보가 유효하지 않다면,
			{
				nErrorCode = e_EXC_ERROR_NO_QUESTITEM ;							// 퀘스트 아이템이 없다는 에러 코드를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}

			if( pQuestItem->dwItemNum < m_dwItemNum )							// 아이템의 개수가, 목표 수량보다 작으면,
			{
				nErrorCode = e_EXC_ERROR_NOT_ENOUGH_QUESTITEM ;					// 퀘스트 아이템이 부족하다는 에러 코드를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}
		}
		break;

	case eQuestExecute_GiveItem:												// 아이템을 반납하는 실행이면,
		{
			int count = 0 ;														// for문을 위한 카운트를 선언하고 0으로 세팅한다.

			DWORD nGiveItemCount = 0 ;											// 반납 가능한 아이템 수를 담을 변수를 선언하고 0으로 세팅한다.

			ITEM_TOTALINFO ItemInfo ;											// 아이템의 토탈 정보를 담는 구조체를 선언한다.

			pPlayer->GetItemtotalInfo( &ItemInfo,								// 플레이어의 토탈 아이템 정보를 받아온다.
				GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR ) ;			

			// 071223 LYW --- QuestExecute_Item : 아이템 반납 처리 수정.
			int nTotalInvenCount = 0 ;
			nTotalInvenCount = (int)(SLOT_INVENTORY_NUM + pPlayer->GetInventoryExpansionSize()) ;

			BOOL bStackItem = FALSE ;
			bStackItem = ITEMMGR->IsDupItem( m_dwItemKind ) ;

			for( count = 0 ; count < nTotalInvenCount ; ++count )				
			{
				if( ItemInfo.Inventory[count].wIconIdx == m_dwItemKind )		// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
				{
					// 스택 아이템과 일반아이템을 구분해서 처리 한다.
					if( bStackItem )											// 스택아이템이면, 
					{
						nGiveItemCount += ItemInfo.Inventory[count].Durability ;// 반납 가능한 아이템 수를 추가한다.
					}
					else														// 일반아이템이면, 
					{
						++nGiveItemCount ;										// 반납 아이템 수를 증가한다.
					}
					
				}
			}

			for( count = 0 ; count < SLOT_WEAR_NUM ; ++count )					// 장비 창의 슬롯 수 만큼 for문을 돌린다.
			{
				if( ItemInfo.WearedItem[count].wIconIdx == m_dwItemKind )		// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
				{
					// 스택 아이템과 일반아이템을 구분해서 처리 한다.
					if( bStackItem )											// 스택아이템이면, 
					{
						nGiveItemCount += ItemInfo.Inventory[count].Durability ;// 반납 가능한 아이템 수를 추가한다.
					}
					else														// 일반아이템이면, 
					{
						++nGiveItemCount ;										// 반납 아이템 수를 증가한다.
					}
				}
			}

			if( nGiveItemCount == 0 )											// 반납 가능한 아이템 수가 0개면,
			{
				nErrorCode = e_EXC_ERROR_NO_GIVEITEM ;							// 반납할 아이템이 없다는 에러를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}

			if( nGiveItemCount < m_dwItemNum )
			{
				nErrorCode = e_EXC_ERROR_NOT_ENOUGH_GIVEITEM ;					// 반납할 아이템이 부족하다는 에러를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}
		}
		break;

	case eQuestExecute_GiveMoney:												// 머니를 반납하는 실행이면,
		{
			MONEYTYPE curMoney = pPlayer->GetMoney() ;							// 플레이어의 현재 머니를 받는다.

			if( curMoney < m_dwItemNum )										// 현재 플레이어의 머니가 반납 할 머니보다 부족하면,
			{
				nErrorCode = e_EXC_ERROR_NOT_ENOUGH_GIVEMONEY ;					// 머니가 부족하다는 에러를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}
		}
		break;

	case eQuestExecute_TakeItem :
		{
			CItemSlot* pSlot = NULL ;											// 아이템 슬롯 정보를 받을 포인터를 선언하고 null 처리를 한다.
			pSlot = pPlayer->GetSlot(eItemTable_Inventory) ;					// 플레이어의 인벤토리 스롯 정보를 받는다.

			if( !pSlot )														// 인벤토리 정보가 유효하지 않으면,
			{
				nErrorCode = e_EXC_ERROR_NO_INVENTORYINFO ;						// 인벤토리 정보가 없다는 에러코드를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}

			WORD EmptyCellPos[255];												// 빈 슬롯 위치를 받을 변수를 선언한다.
			WORD EmptyCellNum;													// 빈 슬롯 개수를 받을 변수를 선언한다.

			WORD wResult = 0 ;													// 빈 슬롯의 체크 결과 값을 받을 변수를 선언하고 0으로 세팅한다.

			wResult = ITEMMGR->CheckExtraSlot(pPlayer, pSlot, m_dwItemKind,		// 아이템을 받을 공간이 충분한지 결과 값을 받는다.
								m_dwItemNum, EmptyCellPos, EmptyCellNum) ;

			if( wResult == 0 )													// 결과 값이 0과 같으면,
			{
				nErrorCode = e_EXC_ERROR_NO_EXTRASLOT ;							// 에러 코드를 인벤토리 여분이 없다는 에러 코드를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}
		}
		break ;

	case eQuestExecute_TakeQuestItemFQW:										// 특정 무기를 체크해서 퀘스트 아이템을 받는 실행이면,
		{
			if( pPlayer->GetWearedWeapon() != m_dwWeaponKind )					// 플레이어가 착용하고 있는 무기와 비교대상 무기가 같지 않으면,
			{
				nErrorCode = e_EXC_ERROR_NOT_SAME_WEAPONKIND ;					// 무기 종류가 달라서 퀘스트 아이템을 받을 수 없다는 에러를 세팅한다.
				return nErrorCode ;												// 에러 코드를 return 처리를 한다.
			}
		}
		break;

	case eQuestExecute_TakeQuestItemFW:											// 무기 계열을 체크해서 퀘스트 아이템을 받는 실행이면,
		{
			return nErrorCode ;													// 에러 코드를 return 처리를 한다.

			// 이 처리는 사용하지 않기로 결정.
		}
		break;

	case eQuestExecute_TakeMoney:												// 머니를 지급받는 실행이면,
	case eQuestExecute_TakeMoneyPerCount:										// 카운트당 머니를 받는 실행이면,
		{
			MONEYTYPE curMoney = pPlayer->GetMoney() ;							// 플레이어의 현재 머니를 받는다.

			if( curMoney >= 4200000000 )
			{
				nErrorCode = e_EXC_ERROR_CANT_RECEIVE_MONEY ;					// 플레이어는 더이상 머니를 받을 수 없다는 에러 코드를 세팅한다.
				return nErrorCode ;													// 에러 코드를 return 처리를 한다.
			}
		}


	// 이하 실행 구문은 특별한 에러 발생의 소지가 보이지 않기 때문에, 기본 return 처리를 한다.
	// 추후 에러 발생시 에러 처리를 한다.
	case eQuestExecute_TakeSExp:												// 수련 포인트를 받는 실행이면,
	case eQuestExecute_TakeExp:													// 경험치를 받는 실행이면,
		{
			return nErrorCode ;													// 에러 코드를 return 처리를 한다.
		}
		break;
	}

#endif //_MAPSERVER_

	return nErrorCode ;															// 기본 값을 리턴한다.
}










