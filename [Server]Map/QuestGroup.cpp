// QuestGroup.cpp: implementation of the CQuestGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestGroup.h"

#include "QuestManager.h"
#include "Quest.h"

#include "Player.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "QuestRegenMgr.h"
#include "..\[CC]Quest\QuestExecute_RandomItem.h"													// 퀘스트 실행 랜덤 아이템 헤더를 불러온다.

#include "PartyManager.h"
#include "Party.h"
#include "UserTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CheckQuestProbability(DWORD dwProbability)
{
	if( dwProbability == 0 )
		return FALSE;
	if( dwProbability != MAX_QUEST_PROBABILITY )
	{
		DWORD nFlag = rand()%MAX_QUEST_PROBABILITY;
		if( nFlag >= dwProbability )
			return FALSE;
	}

	return TRUE;
}

CQuestGroup::CQuestGroup()
{
	m_pPlayer = NULL;
	m_QuestTable.Initialize( MAX_QUEST );
	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

CQuestGroup::CQuestGroup( PLAYERTYPE* pPlayer )
{
	m_pPlayer = pPlayer;
	m_QuestTable.Initialize( MAX_QUEST );
	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

CQuestGroup::~CQuestGroup()
{
	Release();
}

void CQuestGroup::Initialize( PLAYERTYPE* pPlayer )
{
	m_pPlayer = pPlayer;
//	m_QuestTable.Initialize( MAX_QUEST );
//	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

void CQuestGroup::Release()
{
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		delete pQuest;
		pQuest = NULL;
	}
	m_QuestTable.RemoveAll();

	QUESTITEM* pQuestItem = NULL;
	m_QuestItemTable.SetPositionHead();
	while( pQuestItem = m_QuestItemTable.GetData() )
	{
		delete pQuestItem;
		pQuestItem = NULL;
	}
	m_QuestItemTable.RemoveAll();

	m_pPlayer = NULL;
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

void CQuestGroup::ReleaseRandomItemList()
{
	RANDOMITEM* pItem = NULL;	

	PTRLISTPOS pos = NULL ;
	pos = m_RandomItemList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (RANDOMITEM*)m_RandomItemList.GetNext(pos) ;

		if( !pItem ) continue ;

		m_RandomItemList.Remove(pItem) ;

		delete pItem ;
	}

	m_RandomItemList.RemoveAll() ;
}

void CQuestGroup::CreateQuest( DWORD dwQuestIdx, CQuestInfo* pQuestInfo )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest == NULL )
	{
		pQuest = new CQuest( m_pPlayer, pQuestInfo );
		m_QuestTable.Add( pQuest, dwQuestIdx );
	}
}

void CQuestGroup::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
		pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );
}

void CQuestGroup::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
		pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );
}

void CQuestGroup::SetQuestItem( DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	QUESTITEM* pQuestItem = NULL;

	// AO´AAo E®AICI°i AO8¸e Ao¿i´U.
	pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if(pQuestItem)
	{
		m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
		delete pQuestItem;
		pQuestItem = NULL;
	}

	pQuestItem = new QUESTITEM;
	pQuestItem->dwQuestIdx = dwQuestIdx;
	pQuestItem->dwItemIdx = dwItemIdx;
	pQuestItem->dwItemNum = dwItemNum;
	
	m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );
}

void CQuestGroup::AddQuestEvent( CQuestEvent* pQuestEvent, DWORD dwQuestIdx )
{
	if( m_dwQuestEventNum < MAX_QUESTEVENT_PLYER )
	{
		m_QuestEvent[m_dwQuestEventNum].dwQuestIdx = dwQuestIdx;
		m_QuestEvent[m_dwQuestEventNum].QuestEvent.SetValue( pQuestEvent );
		++m_dwQuestEventNum;
	}
}

BOOL CQuestGroup::IsQuestComplete( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );

	if( pQuest )
	if( pQuest->IsQuestComplete() )
		return TRUE;

	return FALSE;
}

void CQuestGroup::DeleteQuest( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->EndQuest( dwQuestIdx, 1, FALSE );

		// quest item delete
		QUESTITEM* pQuestItem = NULL;
		m_QuestItemTable.SetPositionHead();
		cPtrList list;
		while( pQuestItem = m_QuestItemTable.GetData() )
		{
			if( pQuestItem->dwQuestIdx == dwQuestIdx )
			{
				list.AddTail( pQuestItem );
				//m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
				//delete pQuestItem;
				//pQuestItem = NULL;
			}
		}

		PTRLISTPOS pos = list.GetHeadPosition();
		while( pos )
		{
			QUESTITEM* p = (QUESTITEM*)list.GetNext(pos);
			m_QuestItemTable.Remove( p->dwItemIdx );
			delete p;
		}
		list.RemoveAll();

		// item delete
		MSG_EXCHANGE_REMOVEITEM Msg;
		Msg.Category = MP_QUEST;
		Msg.Protocol = MP_QUEST_GIVEITEM_ACK;

		WORD wKind, wType;
		ITEM_TOTALINFO ItemInfo;
		ITEMBASE logitem;
		m_pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR );
		

		// 071223 LYW --- QuestExecute_Item : 아이템 반납 처리 수정.
		int nTotalInvenCount = 0 ;
		nTotalInvenCount = (int)(SLOT_INVENTORY_NUM + m_pPlayer->GetInventoryExpansionSize()) ;

		for( int i = 0; i < nTotalInvenCount ;	++i )
		{
			ITEMMGR->GetItemKindType( ItemInfo.Inventory[i].wIconIdx, &wKind, &wType );
			if( wKind == eQUEST_ITEM_EQUIP && wType == dwQuestIdx )
			{			
				logitem.Durability = ItemInfo.Inventory[i].Durability;
				logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
				logitem.Position = ItemInfo.Inventory[i].Position;
				logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
			
				if( EI_TRUE == ITEMMGR->DiscardItem( m_pPlayer, ItemInfo.Inventory[i].Position, ItemInfo.Inventory[i].wIconIdx, 1 ) )
				{
					Msg.wAbsPosition = ItemInfo.Inventory[i].Position;
					m_pPlayer->SendMsg( &Msg, sizeof(Msg) );
					
					// Log
					LogItemMoney(m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
						eLog_ItemDestroyDeleteQuest, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, m_pPlayer->GetPlayerExpPoint());
				}
			}
		}
		for( i = 0; i < SLOT_WEAR_NUM; ++i )
		{
			ITEMMGR->GetItemKindType( ItemInfo.WearedItem[i].wIconIdx, &wKind, &wType );
			if( wKind == eQUEST_ITEM_EQUIP && wType == dwQuestIdx )
			{			
				logitem.Durability = ItemInfo.Inventory[i].Durability;
				logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
				logitem.Position = ItemInfo.Inventory[i].Position;
				logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;

				if( EI_TRUE == ITEMMGR->DiscardItem( m_pPlayer, ItemInfo.WearedItem[i].Position, ItemInfo.WearedItem[i].wIconIdx, 1 ) )
				{
					// Log
					LogItemMoney(m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
						eLog_ItemDestroyDeleteQuest, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, m_pPlayer->GetPlayerExpPoint());

					Msg.wAbsPosition = ItemInfo.WearedItem[i].Position;
					m_pPlayer->SendMsg( &Msg, sizeof(Msg) );
				}
			}
		}
	}
}

void CQuestGroup::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	if( pQuest->EndQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		QUESTMGR->EndQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )	
	if( pQuest->StartSubQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		// questmgr
		QUESTMGR->StartSubQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )	
	if( pQuest->EndSubQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		// questmgr
		if( dwSubQuestIdx == 0 )
			QUESTMGR->StartQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
		else
			QUESTMGR->EndSubQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::ChangeSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwKind )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, dwKind );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwKind );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

//void CQuestGroup::Process()
//{
//	if( m_pPlayer == NULL )			return;
//	if( m_dwQuestEventNum <= 0 )	return;
//
//	CQuest* pQuest = NULL;
//	m_QuestTable.SetPositionHead();
//	while( pQuest = m_QuestTable.GetData() )
//	{
//		for( DWORD i = 0; i < m_dwQuestEventNum; ++i )
//		{
//			if( !pQuest->IsQuestComplete() )
//				pQuest->OnQuestEvent( m_pPlayer, this, &m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
//		}
//	}
//
//	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
//	m_dwQuestEventNum = 0;
//}

//void CQuestGroup::Process()
//{
//	if( m_pPlayer == NULL )			return;									// 플레이어 정보가 유효하지 않으면 return 처리를 한다.
//	if( m_dwQuestEventNum <= 0 )	return;									// 퀘스트 이벤트가 없으면 return 처리를 한다.
//
//	DWORD dwPartyIdx = m_pPlayer->GetPartyIdx() ;							// 플레이어의 파티 인덱스를 받는다.
//	CParty* pParty = PARTYMGR->GetParty(dwPartyIdx);						// 받은 파티 인덱스로 파티 정보를 받는다.
//
//	if( pParty )															// 플레이어가 파티에 속해 있다면,
//	{
//		DWORD dwMemberId ;													// 파티 멤버의 아이디를 담을 변수를 선언한다.
//		CPlayer* pMember ;													// 파티 멤버의 정보를 담을 포인터를 선언한다.
//		CQuestGroup* pMemberQuestGroup ;									// 멤버의 퀘스트 그룹 정보를 받을 포인터를 선언한다.
//
//		float fDistance ;													// 현재 플레이어와 비교 대상 멤버의 거리를 담을 변수를 선언한다.
//
//		VECTOR3 playerPos ;													// 현재 플레이어의 위치를 담을 벡터를 선언한다.
//		VECTOR3 memberPos ;													// 비교 대상 멤버의 위치를 담을 벡터를 선언한다.
//
//		memset(&playerPos, 0, sizeof(VECTOR3)) ;							// 현재 플레이어의 위치를 담을 벡터를 초기화 한다.
//		m_pPlayer->GetPosition(&playerPos) ;								// 현재 플레이어의 위치를 받는다.
//
//		for( int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )			// 최대 파티 멤버 수 만큼 for문을 돌린다.
//		{
//			dwMemberId = pParty->GetMemberID(count) ;						// 카운트에 해당하는 멤버의 아이디를 받는다.
//
//			pMember = NULL ;												// 멤버 정보를 받을 포인터를 null처리를 한다.
//			pMember = (CPlayer*)g_pUserTable->FindUser( dwMemberId );		// 받은 멤버 아이디로 멤버 정보를 받는다.
//
//			if( !pMember ) continue ;										// 멤버 정보가 유효하지 않으면, continue 처리를 한다.
//
//			if( !pParty->IsMemberLogIn(count) ) continue ;					// 멤버가 로그인 상태가 아니면, continue 처리를 한다.
//
//			pMemberQuestGroup = NULL ;										// 멤버의 퀘스트 그룹 정보를 받을 포인터를 null처리를 한다.
//			pMemberQuestGroup = pMember->GetQuestGroup() ;					// 멤버의 퀘스트 그룹 정보를 받는다.
//
//			if( !pMemberQuestGroup ) continue ;								// 멤버의 퀘스트 그룹 정보가 유효하지 않으면 continue 처리를 한다.
//
//			fDistance = 0.0f ;												// 현재 플레이어와 멤버의 거리를 담을 변수를 초기화 한다.
//
//			memset(&memberPos, 0, sizeof(VECTOR3)) ;						// 멤버의 위치를 담을 벡터를 초기화 한다.			
//			pMember->GetPosition(&memberPos) ;								// 멤버의 위치를 받는다.
//
//			fDistance = CalcDistanceXZ( &playerPos, &memberPos ) ;			// 현재 플레이어와 멤버의 거리를 구한다.
//
//			if( fDistance > POINT_VALID_DISTANCE ) continue ;				// 거리가 파티 유효 거리보다 크면, continue 처리를 한다.
//
//			CQuest* pQuest = NULL;											// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
//			CQuest* pMemberQuest ;											// 멤버의 퀘스트 정보를 받을 포인터를 선언한다.
//
//			DWORD dwQuestIdx ;												// 퀘스트 인덱스를 담을 변수를 선언한다.
//
//			m_QuestTable.SetPositionHead();									// 퀘스트 테이블을 헤드 포지션으로 세팅한다.
//			while( pQuest = m_QuestTable.GetData() )						// 퀘스트 테이블에서 받은 퀘스트 정보가 유효할 동안 while문을 돌린다.
//			{
//				dwQuestIdx = 0 ;											// 퀘스트 인덱스를 담을 변수를 0으로 초기화 한다.
//				dwQuestIdx = pQuest->GetQuestIdx() ;						// 현재 퀘스트의 인덱스를 받는다.
//
//				pMemberQuest = NULL ;										// 멤버의 퀘스트를 받을 포인터를 null 처리를 한다.
//				pMemberQuest = pMember->GetQuestGroup()->GetQuest(dwQuestIdx);	// 현재 퀘스트의 인덱스로 멤버의 퀘스트 정보를 받는다.
//
//				if( !pMemberQuest ) continue ;								// 멤버의 퀘스트 정보가 유효하지 않으면 continue 처리를 한다.
//
//				for( DWORD i = 0; i < m_dwQuestEventNum; ++i )				// 퀘스트 이벤트 수 만큼 for문을 돌린다.
//				{
//					if( pMemberQuest->IsQuestComplete() ) continue ;		// 퀘스트를 완료 하였다면, continue 처리를 한다.
//
//					if( m_QuestEvent[i].QuestEvent.m_dwQuestEventKind == eQuestEvent_Count && 
//						pMember->GetID() != m_pPlayer->GetID() ) continue ;
//					
//					pMemberQuest->OnQuestEvent( pMember, pMemberQuestGroup,	// 현재 퀘스트 이벤트로, 멤버의 퀘스트에 퀘스트 이벤트를 발생시킨다.
//					&m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
//				}
//			}
//		}
//	}
//	else																	// 플레이어가 파티에 속해 있지 않으면,
//	{
//		CQuest* pQuest = NULL;												// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
//
//		m_QuestTable.SetPositionHead();										// 퀘스트 테이블을 헤드 포지션으로 세팅한다.
//		while( pQuest = m_QuestTable.GetData() )							// 퀘스트 테이블에서 받은 퀘스트 정보가 유효할 동안 while문을 돌린다.
//		{
//			for( DWORD i = 0; i < m_dwQuestEventNum; ++i )					// 퀘스트 이벤트 수 만큼 for문을 돌린다.
//			{
//				if( pQuest->IsQuestComplete() ) continue ;					// 퀘스트를 완료 하였다면, continue 처리를 한다.
//				
//				pQuest->OnQuestEvent( m_pPlayer, this, &m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
//			}
//		}
//	}
//
//	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
//	m_dwQuestEventNum = 0;
//}

void CQuestGroup::Process()
{
	if( m_pPlayer == NULL )			return;									// 플레이어 정보가 유효하지 않으면 return 처리를 한다.
	if( m_dwQuestEventNum <= 0 )	return;									// 퀘스트 이벤트가 없으면 return 처리를 한다.

	DWORD dwPartyIdx = m_pPlayer->GetPartyIdx() ;							// 플레이어의 파티 인덱스를 받는다.
	CParty* pParty = PARTYMGR->GetParty(dwPartyIdx);						// 받은 파티 인덱스로 파티 정보를 받는다.

	if( pParty )															// 플레이어가 파티에 속해 있다면,
	{
		DWORD dwMemberId ;													// 파티 멤버의 아이디를 담을 변수를 선언한다.
		CPlayer* pMember ;													// 파티 멤버의 정보를 담을 포인터를 선언한다.
		CQuestGroup* pMemberQuestGroup ;									// 멤버의 퀘스트 그룹 정보를 받을 포인터를 선언한다.

		float fDistance ;													// 현재 플레이어와 비교 대상 멤버의 거리를 담을 변수를 선언한다.

		VECTOR3 playerPos ;													// 현재 플레이어의 위치를 담을 벡터를 선언한다.
		VECTOR3 memberPos ;													// 비교 대상 멤버의 위치를 담을 벡터를 선언한다.

		CQuest* pQuest = NULL;												// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
		CQuest* pMemberQuest ;												// 멤버의 퀘스트 정보를 받을 포인터를 선언한다.

		DWORD dwQuestIdx ;													// 퀘스트 인덱스를 담을 변수를 선언한다.

		DWORD dwEventKind ;

		memset(&playerPos, 0, sizeof(VECTOR3)) ;							// 현재 플레이어의 위치를 담을 벡터를 초기화 한다.
		m_pPlayer->GetPosition(&playerPos) ;								// 현재 플레이어의 위치를 받는다.

		for( int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )			// 최대 파티 멤버 수 만큼 for문을 돌린다.
		{
			dwMemberId = pParty->GetMemberID(count) ;						// 카운트에 해당하는 멤버의 아이디를 받는다.

			pMember = NULL ;												// 멤버 정보를 받을 포인터를 null처리를 한다.
			pMember = (CPlayer*)g_pUserTable->FindUser( dwMemberId );		// 받은 멤버 아이디로 멤버 정보를 받는다.

			if( !pMember ) continue ;										// 멤버 정보가 유효하지 않으면, continue 처리를 한다.

			// 071215 LYW --- QuestQroup : 파티 멤버가 죽은 상태에서는, 보상멤버로 추가하지 않는다.
			if( pMember->GetState() == eObjectState_Die ) continue ;

			if( !pParty->IsMemberLogIn(count) ) continue ;					// 멤버가 로그인 상태가 아니면, continue 처리를 한다.

			if( m_pPlayer->GetGridID() != pMember->GetGridID() ) continue ;	// 멤버와 채널이 다르면, continue 처리를 한다.

			pMemberQuestGroup = NULL ;										// 멤버의 퀘스트 그룹 정보를 받을 포인터를 null처리를 한다.
			pMemberQuestGroup = pMember->GetQuestGroup() ;					// 멤버의 퀘스트 그룹 정보를 받는다.

			if( !pMemberQuestGroup ) continue ;								// 멤버의 퀘스트 그룹 정보가 유효하지 않으면 continue 처리를 한다.

			fDistance = 0.0f ;												// 현재 플레이어와 멤버의 거리를 담을 변수를 초기화 한다.

			memset(&memberPos, 0, sizeof(VECTOR3)) ;						// 멤버의 위치를 담을 벡터를 초기화 한다.			
			pMember->GetPosition(&memberPos) ;								// 멤버의 위치를 받는다.

			fDistance = CalcDistanceXZ( &playerPos, &memberPos ) ;			// 현재 플레이어와 멤버의 거리를 구한다.

			if( fDistance > POINT_VALID_DISTANCE ) continue ;				// 거리가 파티 유효 거리보다 크면, continue 처리를 한다.

			m_QuestTable.SetPositionHead();									// 퀘스트 테이블을 헤드 포지션으로 세팅한다.
			while( pQuest = m_QuestTable.GetData() )						// 퀘스트 테이블에서 받은 퀘스트 정보가 유효할 동안 while문을 돌린다.
			{
				dwQuestIdx = 0 ;											// 퀘스트 인덱스를 담을 변수를 0으로 초기화 한다.
				dwQuestIdx = pQuest->GetQuestIdx() ;						// 현재 퀘스트의 인덱스를 받는다.

				pMemberQuest = NULL ;										// 멤버의 퀘스트를 받을 포인터를 null 처리를 한다.
				pMemberQuest = pMember->GetQuestGroup()->GetQuest(dwQuestIdx);	// 현재 퀘스트의 인덱스로 멤버의 퀘스트 정보를 받는다.

				if( !pMemberQuest ) continue ;								// 멤버의 퀘스트 정보가 유효하지 않으면 continue 처리를 한다.

				if( pMemberQuest->IsQuestComplete() ) continue ;			// 퀘스트를 완료 하였다면, continue 처리를 한다.

				dwEventKind = 0 ;
				dwEventKind = m_QuestEvent[0].QuestEvent.m_dwQuestEventKind ;

				if( pMember->GetID() == m_pPlayer->GetID() )
				{
					for( DWORD i = 0; i < m_dwQuestEventNum; ++i )				// 퀘스트 이벤트 수 만큼 for문을 돌린다.
					{	
						pMemberQuest->OnQuestEvent( pMember, pMemberQuestGroup,	// 현재 퀘스트 이벤트로, 멤버의 퀘스트에 퀘스트 이벤트를 발생시킨다.
						&m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
					}
				}
				else
				{
					if( dwEventKind != eQuestEvent_Hunt &&  dwEventKind != eQuestEvent_Count) continue ;

					if( dwEventKind == eQuestEvent_Count )
					{
						//for( DWORD i = 0; i < m_dwQuestEventNum; ++i )				// 퀘스트 이벤트 수 만큼 for문을 돌린다.
						//{
						//	DWORD dwSQCount = pMemberQuest->GetSubQuestCount() ;

						//	for( int j = dwSQCount-1; j > 0; --j )
						//	{
						//		CSubQuestInfo* pSubQuest = pMemberQuest->GetSubQuestData(j) ;
						//		if( !pSubQuest ) continue ;

						//		if( m_CurSubQuestInfoArray[i] )
						//			m_CurSubQuestInfoArray[i]->OnQuestEvent( pPlayer, pQuestGroup, this, pQuestEvent );
						//	}
						//}
					}
					else
					{
						for( DWORD i = 0; i < m_dwQuestEventNum; ++i )				// 퀘스트 이벤트 수 만큼 for문을 돌린다.
						{	
							pMemberQuest->OnQuestEvent( pMember, pMemberQuestGroup,	// 현재 퀘스트 이벤트로, 멤버의 퀘스트에 퀘스트 이벤트를 발생시킨다.
							&m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
						}
					}
				}
			}
		}
	}
	else																	// 플레이어가 파티에 속해 있지 않으면,
	{
		CQuest* pQuest = NULL;												// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.

		m_QuestTable.SetPositionHead();										// 퀘스트 테이블을 헤드 포지션으로 세팅한다.
		while( pQuest = m_QuestTable.GetData() )							// 퀘스트 테이블에서 받은 퀘스트 정보가 유효할 동안 while문을 돌린다.
		{
			if( pQuest->IsQuestComplete() ) continue ;					// 퀘스트를 완료 하였다면, continue 처리를 한다.

			for( DWORD i = 0; i < m_dwQuestEventNum; ++i )					// 퀘스트 이벤트 수 만큼 for문을 돌린다.
			{				
				pQuest->OnQuestEvent( m_pPlayer, this, &m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
			}
		}
	}

	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

void CQuestGroup::AddCount( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind )
{
	DWORD wWeapon = m_pPlayer->GetWearedWeapon();
	if( ITEMMGR->GetWeaponKind( wWeapon ) != dwWeaponKind )
		return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromQWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromLevelGap( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel )
{
	int nMin = m_pPlayer->GetLevel() - m_pPlayer->GetKillMonsterLevel();
	int nMax = m_pPlayer->GetKillMonsterLevel() - m_pPlayer->GetLevel();
	if( nMin > 0 && nMin > (int)dwMinLevel )	return;
	if( nMax > 0 && nMax > (int)dwMaxLevel )	return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromMonLevel( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel )
{
	WORD wMonLevel = m_pPlayer->GetKillMonsterLevel();
	if( wMonLevel < dwMinLevel )	return;
	if( wMonLevel > dwMaxLevel )	return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

BOOL CQuestGroup::GiveQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	if( !pPlayer ) return FALSE ;

	BOOL bResult = FALSE ;

	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );

	if( pQuestItem )
	{
		if(QUESTMGR->GiveQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, 0 ))
		{
			m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
			delete pQuestItem;

			bResult = TRUE ;
		}
		else
		{
			bResult = FALSE ;
		}
	}
	else
	{
		bResult = FALSE ;
	}

	return bResult ;
}

void CQuestGroup::TakeQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability )
{
	if( CheckQuestProbability(dwProbability) == FALSE )
		return;

	DWORD dwPlayerID = pPlayer->GetID() ;

	ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		pQuestItem->dwItemNum += dwItemNum;
        DWORD dwValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );
		if( pQuestItem->dwItemNum != dwValue )
		{
			if( pQuestItem->dwItemNum > dwValue )
				pQuestItem->dwItemNum = dwValue;
		}
//		if( pQuestItem->dwItemNum > 100 )
//			pQuestItem->dwItemNum = 100;
		QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
	else	// ¾ø´U¸e
	{
		pQuestItem = new QUESTITEM;
		pQuestItem->dwQuestIdx = dwQuestIdx;
		pQuestItem->dwItemIdx = dwItemIdx;
		pQuestItem->dwItemNum = dwItemNum;
		m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

		QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
}

void CQuestGroup::TakeQuestItemFromQWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind )
{
	if( CheckQuestProbability(dwProbability) == FALSE )
		return;

	ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		int nValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );

		pQuestItem->dwItemNum = nValue;
		QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
	else	// ¾ø´U¸e
	{
		pQuestItem = new QUESTITEM;
		pQuestItem->dwQuestIdx = dwQuestIdx;
		pQuestItem->dwItemIdx = dwItemIdx;
		pQuestItem->dwItemNum = dwItemNum;
		m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

		QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
}

void CQuestGroup::TakeQuestItemFromWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind )
{
	// 이 기능은 사용하지 않기로 결정 됨.

	//DWORD wWeapon = pPlayer->GetWearedWeapon();
	//if( ITEMMGR->GetWeaponKind( wWeapon ) != dwWeaponKind )
	//	return;

	//if( CheckQuestProbability(dwProbability) == FALSE )
	//	return;

	//ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	//// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	//QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	//if( pQuestItem )
	//{
	//	int nValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );

 //       pQuestItem->dwItemNum = nValue;
	//	QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	//}
	//else	// ¾ø´U¸e
	//{
	//	pQuestItem = new QUESTITEM;
	//	pQuestItem->dwQuestIdx = dwQuestIdx;
	//	pQuestItem->dwItemIdx = dwItemIdx;
	//	pQuestItem->dwItemNum = dwItemNum;
	//	m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

	//	QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	//}
}

//void CQuestGroup::GiveItem( PLAYERTYPE* pPlayer, DWORD wItemIdx, DURTYPE dwItemNum )
//{
//	MSG_EXCHANGE_REMOVEITEM Msg;
//	Msg.Category = MP_QUEST;
//	Msg.Protocol = MP_QUEST_GIVEITEM_ACK;
//
//	int i = 0;
//	ITEM_TOTALINFO ItemInfo;
//	ITEMBASE logitem;
//	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR );
//	for( i = 0; i < SLOT_INVENTORY_NUM;	++i )
//	{
//		if( ItemInfo.Inventory[i].wIconIdx == wItemIdx )
//		{
//			logitem.Durability = ItemInfo.Inventory[i].Durability;
//			logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
//			logitem.Position = ItemInfo.Inventory[i].Position;
//			logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
//			
//			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, ItemInfo.Inventory[i].Position, wItemIdx, dwItemNum ) )
//			{
//
//				// Log
//				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
//					eLog_ItemDestroyGiveQuest, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
//					logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, pPlayer->GetPlayerExpPoint());
//				
//				Msg.wAbsPosition = ItemInfo.Inventory[i].Position;
//				pPlayer->SendMsg( &Msg, sizeof(Msg) );
//			}
//		}
//	}
//	for( i = 0; i < SLOT_WEAR_NUM; ++i )
//	{
//		if( ItemInfo.WearedItem[i].wIconIdx == wItemIdx )
//		{
//			logitem.Durability = ItemInfo.Inventory[i].Durability;
//			logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
//			logitem.Position = ItemInfo.Inventory[i].Position;
//			logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
//			
//			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, ItemInfo.WearedItem[i].Position, wItemIdx, dwItemNum ) )
//			{
//				// Log
//				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
//					eLog_ItemDestroyGiveQuest, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
//					logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, pPlayer->GetPlayerExpPoint());
//
//				Msg.wAbsPosition = ItemInfo.WearedItem[i].Position;
//				pPlayer->SendMsg( &Msg, sizeof(Msg) );
//			}
//		}
//	}
//}

// 070828 LYW --- QuestGroup : 아이템 반납 처리 함수를 수정함.

//BOOL CQuestGroup::CheckGiveItemCount(PLAYERTYPE* pPlayer, DWORD wItemIdx, DURTYPE dwItemNum)
//{
//	if( !pPlayer ) return FALSE ;
//
//	DWORD nGiveItemCount = 0 ;																	// 반납할 아이템 수를 담을 변수를 선언하고 0으로 세팅한다.
//
//	int count ;																					// for문을 돌릴 카운트를 선언한다.
//
//	ITEM_TOTALINFO ItemInfo ;																	// 아이템의 토탈 정보를 담는 구조체를 선언한다.
//
//	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR ) ;			// 플레이어의 토탈 아이템 정보를 받아온다.
//
//	for( count = 0 ; count < SLOT_INVENTORY_NUM ; ++count )										// 인벤토리 슬롯 수 만큼 for문을 돌린다.
//	{
//		if( ItemInfo.Inventory[count].wIconIdx == wItemIdx )									// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
//		{
//			++nGiveItemCount ;																	// 반납할 아이템 수를 증가한다.
//		}
//	}
//
//	for( count = 0 ; count < SLOT_WEAR_NUM ; ++count )											// 장비 창의 슬롯 수 만큼 for문을 돌린다.
//	{
//		if( ItemInfo.WearedItem[count].wIconIdx == wItemIdx )									// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
//		{
//			++nGiveItemCount ;																	// 반납할 아이템 수를 증가한다.
//		}
//	}
//
//	if( nGiveItemCount >= dwItemNum ) return TRUE ;
//	else return FALSE ;
//}

//BOOL CQuestGroup::GiveItem( PLAYERTYPE* pPlayer, DWORD wItemIdx, DURTYPE dwItemNum )			// 퀘스트로 지급 받은 아이템을 반납하는 함수.
//{	
//	BOOL bResult = FALSE ;																		// 반납 결과를 담을 변수를 선언하고 false로 세팅한다.
//
//	if( !pPlayer ) return FALSE ;
//
//	int count ;																					// for문을 돌릴 카운트를 선언한다.
//
//	DWORD nGiveItemCount = 0 ;																	// 반납할 아이템 수를 담을 변수를 선언하고 0으로 세팅한다.
//	ITEM_TOTALINFO ItemInfo ;																	// 아이템의 토탈 정보를 담는 구조체를 선언한다.
//	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR ) ;			// 플레이어의 토탈 아이템 정보를 받아온다.
//
//	MSG_EXCHANGE_REMOVEITEM Msg;																// 아이템 삭제 위치를 세팅하는 메시지 구조체를 선언한다.
//
//	Msg.Category = MP_QUEST;																	// 카테고리를 퀘스트로 세팅한다.
//	Msg.Protocol = MP_QUEST_GIVEITEM_ACK;														// 프로토콜을 아이템 반납 성공으로 세팅한다.
//
//	memset( &ItemInfo, 0, sizeof(ITEM_TOTALINFO) ) ;											// 아이템의 토탈 정보를 담을 구조체를 메모리 셋한다.
//	ITEMBASE logitem ;																			// 로그용 아이템 기본 정보 구조체를 선언한다.
//	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR );			// 플레이어의 토탈 아이템 정보를 받아온다.
//
//	DWORD dwPlayerID	= pPlayer->GetID() ;													// 플레이어 아이디를 받는다.
//	char* pPlayerName	= pPlayer->GetObjectName() ;											// 플레이어 이름을 받는다.
//	MONEYTYPE Money		= pPlayer->GetMoney(eItemTable_Inventory) ;								// 플레이어 머니를 받는다.
//	EXPTYPE	playerExp	= pPlayer->GetPlayerExpPoint() ;										// 플레이어 경험치를 받는다.
//
//	POSTYPE	pos ;																				// 포지션 값을 담을 변수를 선언한다.
//
//	nGiveItemCount = 0 ;																		// 반납할 아이템 수를 담을 변수를 0으로 세팅한다.
//
//	for( count = 0 ; count < SLOT_INVENTORY_NUM ; ++count )										// 인벤토리 슬롯 수 만큼 for문을 돌린다.
//	{
//		if( ItemInfo.Inventory[count].wIconIdx == wItemIdx )									// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
//		{
//			logitem.Durability = ItemInfo.Inventory[count].Durability;							// 내구력을 세팅한다.
//			logitem.dwDBIdx = ItemInfo.Inventory[count].dwDBIdx;								// DB 인덱스를 세팅한다.
//			logitem.Position = ItemInfo.Inventory[count].Position;								// 포지션을 세팅한다.
//			logitem.wIconIdx = ItemInfo.Inventory[count].wIconIdx;								// 아이콘 인덱스를 세팅한다.
//
//			pos = ItemInfo.Inventory[count].Position ;											// 인벤토리의 포지션 값을 받는다.
//			
//			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, pos, wItemIdx, dwItemNum ) )			// 아이템 반납 처리가 성공하면,
//			{
//				LogItemMoney(dwPlayerID, pPlayerName, 0, "", eLog_ItemDestroyGiveQuest,			// 로그를 남긴다.
//					Money, 0, 0, logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 
//					0, logitem.Durability, playerExp);
//				
//				Msg.wAbsPosition = pos ;														// 전송할 메시지 구조체에 포지션을 세팅한다.
//				pPlayer->SendMsg( &Msg, sizeof(Msg) );											// 플레이어에게 메시지를 전송한다.
//
//				++nGiveItemCount ;																// 반납할 아이템 수를 증가시킨다.
//
//				if( nGiveItemCount >= dwItemNum )
//				{
//					bResult = TRUE ;															// 반납 결과를 true로 세팅한다.
//
//					return bResult ;															// 결과 값을 리턴 한다.
//				}
//			}
//		}
//	}
//
//	for( count = 0 ; count < SLOT_WEAR_NUM ; ++count )											// 장비 창의 슬롯 수 만큼 for문을 돌린다.
//	{
//		if( ItemInfo.WearedItem[count].wIconIdx == wItemIdx )									// 인자로 넘어온 아이템 인덱스와 같은 아이템이 있다면,
//		{
//			logitem.Durability = ItemInfo.Inventory[count].Durability;							// 내구력을 세팅한다.
//			logitem.dwDBIdx = ItemInfo.Inventory[count].dwDBIdx;								// DB 인덱스를 세팅한다.
//			logitem.Position = ItemInfo.Inventory[count].Position;								// 포지션을 세팅한다.
//			logitem.wIconIdx = ItemInfo.Inventory[count].wIconIdx;								// 아이콘 인덱스를 세팅한다.
//
//			pos = ItemInfo.WearedItem[count].Position ;											// 장비창의 포지션 값을 받는다.
//			
//			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, pos, wItemIdx, dwItemNum ) )			// 아이템 반납 처리가 성공하면,
//			{
//				LogItemMoney(dwPlayerID, pPlayerName, 0, "", eLog_ItemDestroyGiveQuest,			// 로그를 남긴다.
//					Money, 0, 0, logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 
//					0, logitem.Durability, playerExp);
//
//				Msg.wAbsPosition = ItemInfo.WearedItem[count].Position;							// 전송할 메시지 구조체에 포지션을 세팅한다.
//				pPlayer->SendMsg( &Msg, sizeof(Msg) );											// 플레이어에게 메시지를 전송한다.
//
//				++nGiveItemCount ;																// 반납할 아이템 수를 증가시킨다.
//
//				if( nGiveItemCount >= dwItemNum )
//				{
//					bResult = TRUE ;															// 반납 결과를 true로 세팅한다.
//
//					return bResult ;															// 결과 값을 리턴 한다.													
//				}
//			}
//		}
//	}
//
//	return bResult ;																			// 반납 결과를 리턴한다.
//}





// 071027 LYW --- QuestGroup : Modified function to give item of quest group.
BOOL CQuestGroup::GiveItem( PLAYERTYPE* pPlayer, DWORD wItemIdx, DURTYPE dwItemNum, DWORD dwQuestIdx )			// 아이템을 반납하는 함수.
{	
	if( !pPlayer ) return FALSE ;

	DWORD dwPlayerID	= pPlayer->GetID() ;								// 플레이어 아이디를 받는다.
	char* pPlayerName	= pPlayer->GetObjectName() ;						// 플레이어 이름을 받는다.
	MONEYTYPE Money		= pPlayer->GetMoney(eItemTable_Inventory) ;			// 플레이어 머니를 받는다.
	EXPTYPE	playerExp	= pPlayer->GetPlayerExpPoint() ;					// 플레이어 경험치를 받는다.

	ITEM_TOTALINFO ItemInfo ;												// 아이템의 토탈 정보를 담는 구조체를 선언한다.
	memset( &ItemInfo, 0, sizeof(ITEM_TOTALINFO) ) ;						// 아이템의 토탈 정보를 담을 구조체를 메모리 셋한다.
	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR ) ;			// 플레이어의 토탈 아이템 정보를 받아온다.

	BOOL bStackItem = FALSE ;												// 스택 아이템인지 여부를 담을 변수를 선언하고 false 세팅한다.

	if( ITEMMGR->IsDupItem( wItemIdx ) )									// 인자로 넘어온 아이템이, 스택 아이템이면,
	{
		bStackItem = TRUE ;													// true 세팅한다.
	}

	int nGiveNum = (int)dwItemNum ;											// 아이템 개수를 받는다.

	if( bStackItem )														// 스택 아이템 이라면,
	{
		Give_StackItem_ItemInven(pPlayer, &ItemInfo, wItemIdx, &nGiveNum, dwQuestIdx) ; // 아이템 인벤토리에서 스택 아이템을 반납한다.

		if( nGiveNum <= 0 ) return TRUE ;									// 필요한 아이템 수를 반납했으면 return 처리한다.

		Give_StackItem_WearInven(pPlayer, &ItemInfo, wItemIdx, &nGiveNum, dwQuestIdx) ;	// 장비 인벤토리에서 스택 아이템을 반납한다.
	}
	else																	// 일반 아이템이라면,
	{
		Give_NormalItem_ItemInven(pPlayer, &ItemInfo, wItemIdx, &nGiveNum, dwQuestIdx) ;// 아이템 인벤토리에서 일반 아이템을 반납한다.

		if( nGiveNum <= 0 ) return TRUE ;									// 필요한 아이템 수를 반납했으면 return 처리한다.

		Give_NormalItem_WearInven(pPlayer, &ItemInfo, wItemIdx, &nGiveNum, dwQuestIdx) ;// 장비 인벤토리에서 일반 아이템을 반납한다.
	}

	if( nGiveNum <= 0 ) return TRUE ;										// 필요한 아이템 수를 반납했으면 true return 처리한다.
	else return FALSE ;														// false return 처리한다.
}





// 071027 LYW --- QuestGroup : Add function to give stack item of item inventory.
void CQuestGroup::Give_StackItem_ItemInven(PLAYERTYPE* pPlayer, ITEM_TOTALINFO* pInfo, DWORD wItemIdx, int* pGiveNum, DWORD dwQuestIdx)
{
	ASSERT(pPlayer) ;														// ASSERT 체크.
	ASSERT(pInfo) ;															// ASSERT 체크.

	ITEMBASE* pInven ;														// 인벤토리를 받을 포인터를 선언한다.
	POSTYPE	pos ;															// 포지션 값을 담을 변수를 선언한다.

	// 071223 LYW --- QuestExecute_Item : 아이템 반납 처리 수정.
	int nTotalInvenCount = 0 ;
	nTotalInvenCount = (int)(SLOT_INVENTORY_NUM + pPlayer->GetInventoryExpansionSize()) ;

	for( int count = 0 ; count < nTotalInvenCount ; ++count )				// 인벤토리 슬롯 수 만큼 for문을 돌린다
	{
		if( *pGiveNum <= 0 ) return ;										// 반납할 아이템 수가 0이하면, return 처리한다.

		pInven = NULL ;														// 인벤토리 포인터를 null 처리한다.
		pInven = &pInfo->Inventory[count] ;									// 위치에 해당하는 인벤토리 정보를 받는다.

		ASSERT( pInven ) ;													// ASSERT 체크.

		if( pInven->wIconIdx != wItemIdx ) continue ;						// 인덱스가 일치하지 않으면, continue 처리한다.

		pos = NULL ;														// 위치 값을 받을 포인터를 null.
		pos = pInven->Position ;											// 인벤토리의 포지션 값을 받는다.

		int nResult = err_No_Error ;										// 아이템 반환 결과를 담을 변수를 선언하고, -1로 세팅.
		nResult = ITEMMGR->QG_Discard_StackItem(pPlayer, pos, wItemIdx, pGiveNum) ;	// 결과 값을 받는다.

		if( nResult == err_No_Error )										// 에러가 발생하지 않았으면, 
		{
			Log_GiveItem(pPlayer, pInven) ;									// 로그를 남긴다.
		}
		else																// 에러가 발생했다면,
		{
			MSG_DWORD4 Msg ;												// 메시지 구조체를 선언한다.

			Msg.Category = MP_QUEST ;										// 카테고리를 퀘스트로 세팅한다.
			Msg.Protocol = MP_QUEST_GIVEITEM_ERR ;							// 프로토콜을 아이템 반납 에러로 세팅한다.

			Msg.dwData1 = QG_GSI ;											// 에러 위치를 세팅한다.
			Msg.dwData2 = nResult ;											// 에러 값을 세팅한다.
			Msg.dwData3 = dwQuestIdx ;										// 현재 퀘스트 인덱스를 세팅한다.
			Msg.dwData4 = wItemIdx ;										// 아이템 인덱스를 세팅한다.

			pPlayer->SendMsg( &Msg, sizeof(Msg) ) ;							// 플레이어에게 메시지를 전송한다.

//			*pGiveNum = 0 ;													// 아이템 수를 0으로 세팅한다.

			return ;														// return.
		}
	}
}





// 071027 LYW --- QuestGroup : Add function to give stack item of wear inventory.
void CQuestGroup::Give_StackItem_WearInven(PLAYERTYPE* pPlayer, ITEM_TOTALINFO* pInfo, DWORD wItemIdx, int* pGiveNum, DWORD dwQuestIdx)
{
	ASSERT(pPlayer) ;														// ASSERT 체크.
	ASSERT(pInfo) ;															// ASSERT 체크.

	ITEMBASE* pInven ;														// 인벤토리를 받을 포인터를 선언한다.
	POSTYPE	pos ;															// 포지션 값을 담을 변수를 선언한다.

	//for( int count = 0 ; count < SLOT_INVENTORY_NUM ; ++count )				// 인벤토리 슬롯 수 만큼 for문을 돌린다.
	for( int count = 0 ; count < (SLOT_INVENTORY_NUM + pPlayer->GetInventoryExpansionSize()); ++count )				// 인벤토리 슬롯 수 만큼 for문을 돌린다.
	{
		if( *pGiveNum <= 0 ) return ;										// 반납할 아이템 수가 0이하면, return 처리한다.

		pInven = NULL ;														// 인벤토리 포인터를 null 처리한다.
		pInven = &pInfo->WearedItem[count] ;								// 위치에 해당하는 인벤토리 정보를 받는다.

		ASSERT( pInven ) ;													// ASSERT 체크.

		if( pInven->wIconIdx != wItemIdx ) continue ;						// 인덱스가 일치하지 않으면, continue 처리한다.

		pos = NULL ;														// 위치 값을 받을 포인터를 null.
		pos = pInven->Position ;											// 인벤토리의 포지션 값을 받는다.

		int nResult = err_No_Error ;										// 아이템 반환 결과를 담을 변수를 선언하고, -1로 세팅.
		nResult = ITEMMGR->QG_Discard_StackItem(pPlayer, pos, wItemIdx, pGiveNum) ;	// 결과 값을 받는다.

		if( nResult == err_No_Error )										// 에러가 발생하지 않았으면, 
		{
			Log_GiveItem(pPlayer, pInven) ;									// 로그를 남긴다.
		}
		else																// 에러가 발생했다면,
		{
			MSG_DWORD4 Msg ;												// 메시지 구조체를 선언한다.

			Msg.Category = MP_QUEST ;										// 카테고리를 퀘스트로 세팅한다.
			Msg.Protocol = MP_QUEST_GIVEITEM_ERR ;							// 프로토콜을 아이템 반납 에러로 세팅한다.

			Msg.dwData1 = QG_GSW ;											// 에러 위치를 세팅한다.
			Msg.dwData2 = nResult ;											// 에러 값을 세팅한다.
			Msg.dwData3 = dwQuestIdx ;										// 현재 퀘스트 인덱스를 세팅한다.
			Msg.dwData4 = wItemIdx ;										// 아이템 인덱스를 세팅한다.

			pPlayer->SendMsg( &Msg, sizeof(Msg) ) ;							// 플레이어에게 메시지를 전송한다.

//			*pGiveNum = 0 ;													// 아이템 수를 0으로 세팅한다.

			return ;														// return.
		}
	}
}





// 071027 LYW --- QuestGroup : Add function to give normal item of item inventory.
void CQuestGroup::Give_NormalItem_ItemInven(PLAYERTYPE* pPlayer, ITEM_TOTALINFO* pInfo, DWORD wItemIdx, int* pGiveNum, DWORD dwQuestIdx)
{
	ASSERT(pPlayer) ;														// ASSERT 체크.
	ASSERT(pInfo) ;															// ASSERT 체크.

	ITEMBASE* pInven ;														// 인벤토리를 받을 포인터를 선언한다.
	POSTYPE	pos ;															// 포지션 값을 담을 변수를 선언한다.

	// 071223 LYW --- QuestExecute_Item : 아이템 반납 처리 수정.
	int nTotalInvenCount = 0 ;
	nTotalInvenCount = (int)(SLOT_INVENTORY_NUM + pPlayer->GetInventoryExpansionSize()) ;

	for( int count = 0 ; count < nTotalInvenCount ; ++count )				// 인벤토리 슬롯 수 만큼 for문을 돌린다.
	{
		if( *pGiveNum <= 0 ) return ;										// 반납할 아이템 수가 0이하면, return 처리한다.

		pInven = NULL ;														// 인벤토리 포인터를 null 처리한다.
		pInven = &pInfo->Inventory[count] ;									// 위치에 해당하는 인벤토리 정보를 받는다.

		ASSERT( pInven ) ;													// ASSERT 체크.

		if( pInven->wIconIdx != wItemIdx ) continue ;						// 인덱스가 일치하지 않으면, continue 처리한다.

		pos = NULL ;														// 위치 값을 받을 포인터를 null.
		pos = pInven->Position ;											// 인벤토리의 포지션 값을 받는다.

		int nResult = err_No_Error ;										// 아이템 반환 결과를 담을 변수를 선언하고, -1로 세팅.
		nResult = ITEMMGR->QG_Discard_NormalItem(pPlayer, pos, wItemIdx, pGiveNum) ;	// 결과 값을 받는다.

		if( nResult == err_No_Error )										// 에러가 발생하지 않았으면, 
		{
			Log_GiveItem(pPlayer, pInven) ;									// 로그를 남긴다.
		}
		else																// 에러가 발생했다면,
		{
			MSG_DWORD4 Msg ;												// 메시지 구조체를 선언한다.

			Msg.Category = MP_QUEST ;										// 카테고리를 퀘스트로 세팅한다.
			Msg.Protocol = MP_QUEST_GIVEITEM_ERR ;							// 프로토콜을 아이템 반납 에러로 세팅한다.

			Msg.dwData1 = QG_GNI ;											// 에러 위치를 세팅한다.
			Msg.dwData2 = nResult ;											// 에러 값을 세팅한다.
			Msg.dwData3 = dwQuestIdx ;										// 현재 퀘스트 인덱스를 세팅한다.
			Msg.dwData4 = wItemIdx ;										// 아이템 인덱스를 세팅한다.

			pPlayer->SendMsg( &Msg, sizeof(Msg) ) ;							// 플레이어에게 메시지를 전송한다.

//			*pGiveNum = 0 ;													// 아이템 수를 0으로 세팅한다.

			return ;														// return.
		}
	}
}





// 071027 LYW --- QuestGroup : Add function to give normal item of wear inventory.
void CQuestGroup::Give_NormalItem_WearInven(PLAYERTYPE* pPlayer, ITEM_TOTALINFO* pInfo, DWORD wItemIdx, int* pGiveNum, DWORD dwQuestIdx)
{
	ASSERT(pPlayer) ;														// ASSERT 체크.
	ASSERT(pInfo) ;															// ASSERT 체크.

	ITEMBASE* pInven ;														// 인벤토리를 받을 포인터를 선언한다.
	POSTYPE	pos ;															// 포지션 값을 담을 변수를 선언한다.

	for( int count = 0 ; count < (SLOT_INVENTORY_NUM + pPlayer->GetInventoryExpansionSize()); ++count )				// 인벤토리 슬롯 수 만큼 for문을 돌린다.
	{
		if( *pGiveNum <= 0 ) return ;										// 반납할 아이템 수가 0이하면, return 처리한다.

		pInven = NULL ;														// 인벤토리 포인터를 null 처리한다.
		pInven = &pInfo->WearedItem[count] ;								// 위치에 해당하는 인벤토리 정보를 받는다.

		ASSERT( pInven ) ;													// ASSERT 체크.

		if( pInven->wIconIdx != wItemIdx ) continue ;						// 인덱스가 일치하지 않으면, continue 처리한다.

		pos = NULL ;														// 위치 값을 받을 포인터를 null.
		pos = pInven->Position ;											// 인벤토리의 포지션 값을 받는다.

		int nResult = err_No_Error ;										// 아이템 반환 결과를 담을 변수를 선언하고, -1로 세팅.
		nResult = ITEMMGR->QG_Discard_NormalItem(pPlayer, pos, wItemIdx, pGiveNum) ;	// 결과 값을 받는다.

		if( nResult == err_No_Error )										// 에러가 발생하지 않았으면, 
		{
			Log_GiveItem(pPlayer, pInven) ;									// 로그를 남긴다.
		}
		else																// 에러가 발생했다면,
		{
			MSG_DWORD4 Msg ;												// 메시지 구조체를 선언한다.

			Msg.Category = MP_QUEST ;										// 카테고리를 퀘스트로 세팅한다.
			Msg.Protocol = MP_QUEST_GIVEITEM_ERR ;							// 프로토콜을 아이템 반납 에러로 세팅한다.

			Msg.dwData1 = QG_GNW ;											// 에러 위치를 세팅한다.
			Msg.dwData2 = nResult ;											// 에러 값을 세팅한다.
			Msg.dwData3 = dwQuestIdx ;										// 현재 퀘스트 인덱스를 세팅한다.
			Msg.dwData4 = wItemIdx ;										// 아이템 인덱스를 세팅한다.

			pPlayer->SendMsg( &Msg, sizeof(Msg) ) ;							// 플레이어에게 메시지를 전송한다.

//			*pGiveNum = 0 ;													// 아이템 수를 0으로 세팅한다.

			return ;														// return.
		}
	}
}





// 071027 LYW --- QuestGroup : Add functino to write log.
void CQuestGroup::Log_GiveItem(PLAYERTYPE* pPlayer, ITEMBASE* pInven)
{
	ASSERT(pPlayer) ;														// ASSERT 체크.
	ASSERT(pInven) ;														// ASSERT 체크.

	ITEMBASE logitem ;														// 로그용 아이템 기본 정보 구조체를 선언한다.

	DWORD dwPlayerID	= pPlayer->GetID() ;								// 플레이어 아이디를 받는다.
	char* pPlayerName	= pPlayer->GetObjectName() ;						// 플레이어 이름을 받는다.
	MONEYTYPE Money		= pPlayer->GetMoney(eItemTable_Inventory) ;			// 플레이어 머니를 받는다.
	EXPTYPE	playerExp	= pPlayer->GetPlayerExpPoint() ;					// 플레이어 경험치를 받는다.

	logitem.Durability	= pInven->Durability ;								// 내구력을 세팅한다.
	logitem.dwDBIdx		= pInven->dwDBIdx ;									// DB 인덱스를 세팅한다.
	logitem.Position	= pInven->Position ;								// 포지션을 세팅한다.
	logitem.wIconIdx	= pInven->wIconIdx ;								// 아이콘 인덱스를 세팅한다.

	LogItemMoney(
		dwPlayerID, pPlayerName, 0, "", eLog_ItemDestroyGiveQuest,			
		Money, 0, 0, logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 
		0, logitem.Durability, playerExp) ;									// 로그를 남긴다.
}












void CQuestGroup::GiveMoney( PLAYERTYPE* pPlayer, DWORD dwMoney )
{
	MONEYTYPE money = pPlayer->SetMoney( dwMoney, MONEY_SUBTRACTION );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_GIVEMONEY_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = money;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

BOOL CQuestGroup::TakeItem( PLAYERTYPE* pPlayer, DWORD wItemIdx, DWORD dwItemNum, DWORD dwProbability )
{
	if( CheckQuestProbability(dwProbability) == FALSE )
		return TRUE;

	if( !ITEMMGR->GetItemInfo( wItemIdx ) )	return TRUE;

	if( ITEMMGR->ObtainItemFromQuest( pPlayer, wItemIdx, dwItemNum ) == 1 )	return FALSE;

	return TRUE;
}

void CQuestGroup::TakeMoney( PLAYERTYPE* pPlayer, DWORD dwMoney )
{
	MONEYTYPE money = pPlayer->SetMoney( dwMoney, MONEY_ADDITION );

	// log
	InsertLogMoney( eMoneyLog_GetFromQuest, pPlayer->GetID(), pPlayer->GetMoney(), dwMoney, 0, 0 );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKEMONEY_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = money;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeExp( PLAYERTYPE* pPlayer, DWORD dwExp )
{
	pPlayer->SetPlayerExpPoint( pPlayer->GetPlayerExpPoint() + dwExp );
	//log
	InsertLogExp( eExpLog_GetbyQuest, pPlayer->GetID(), pPlayer->GetLevel(), dwExp, pPlayer->GetPlayerExpPoint(),
		0, 0, /*어빌리티 삭제 - pPlayer->GetPlayerAbilityExpPoint()*/0 );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKEEXP_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = dwExp;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeSExp( PLAYERTYPE* pPlayer, DWORD dwSExp )
{
	/*어빌리티 삭제 - pPlayer->AddAbilityExp( dwSExp );*/
	//log
	InsertLogExp( eExpLog_SExpGetbyQuest, pPlayer->GetID(), pPlayer->GetLevel(), dwSExp, pPlayer->GetPlayerExpPoint(),
		0, 0, /*어빌리티 삭제 - pPlayer->GetPlayerAbilityExpPoint()*/0 );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKESEXP_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = dwSExp;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeMoneyPerCount( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwMoney )
{
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		if( pQuestItem->dwItemNum > 0 )		// Item이 하나라도 있다면...
		{
			MONEYTYPE total = pQuestItem->dwItemNum * dwMoney;

			// quest item 지움
			m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
			QUESTMGR->GiveQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, 0 );
			delete pQuestItem;
			ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Reset );

			// money 계산
			MONEYTYPE money = pPlayer->SetMoney( total, MONEY_ADDITION );
			
			MSG_DWORD Msg;
			Msg.Category = MP_QUEST;
			Msg.Protocol = MP_QUEST_TAKEMONEY_ACK;
			Msg.dwObjectID = pPlayer->GetID();
			Msg.dwData = money;
			pPlayer->SendMsg(&Msg, sizeof(Msg));
		}
		else
		{
		}
	}
	else	// Item이 없으면...
	{
	}
}

void CQuestGroup::RegenMonster( DWORD dwRegenType )
{
	QUESTREGENMGR->MonsterRegen( m_pPlayer, dwRegenType );
}





//=========================================================================
//	NAME : MapChange
//	DESC : The function to move player to specified destination. 080104 LYW
//=========================================================================
void CQuestGroup::MapChange( MAPTYPE destinationMap, float xPos, float zPos )
{
	m_pPlayer->SendPlayerToMap(destinationMap, xPos, zPos) ;
}


//void CQuestGroup::MapChange( WORD wCurMap, WORD wReturnMap )
//{
//	m_pPlayer->SetDieInSpecialMap( TRUE, wReturnMap );
//	m_pPlayer->SendPlayerToMap() ;
//}
// 070414 LYW --- QuestGroup : Active function ChangeStage.
//void CQuestGroup::ChangeStage( BYTE bStage )
// 070416 LYW --- QuestGroup : Modified function ChangeStage.
void CQuestGroup::ChangeStage( DWORD dwGrade, DWORD dwIndex ) 
{
	// 070415 LYW --- Modified function change stage.
	/*
	BYTE stage = m_pPlayer->GetStage();
	if( stage == bStage )	return;

	switch( bStage )
	{
	case eStage_Hwa:
		{
			if( stage != eStage_Normal )	return;
		}
		break;
	case eStage_Hyun:
		{
			if( stage != eStage_Hwa )	return;
		}
		break;
	case eStage_Geuk:
		{
			if( stage != eStage_Normal )	return;
		}
		break;
	case eStage_Tal:
		{
			if( stage != eStage_Geuk )	return;
		}
		break;
	}
	
	m_pPlayer->SetStage( bStage );
	*/

	/*
	if( bStage > 0 && bStage < 7 )
	{
		m_pPlayer->SetStage( bStage ) ;
	}
	*/

	BYTE byStage = (BYTE)dwGrade ;
	BYTE byIndex = (BYTE)dwIndex ;

	if( byIndex > 0 && byIndex < 7 )
	{
		switch( byStage )
		{
		case 2 : if( byIndex > 0 && byIndex < 3 ) m_pPlayer->SetJob( byStage, byIndex ) ; break ;
		case 3 : if( byIndex > 0 && byIndex < 4 ) m_pPlayer->SetJob( byStage, byIndex ) ; break ;
		case 4 : if( byIndex > 0 && byIndex < 5 ) m_pPlayer->SetJob( byStage, byIndex ) ; break ;
		case 5 : if( byIndex > 0 && byIndex < 7 ) m_pPlayer->SetJob( byStage, byIndex ) ; break ;
		case 6 : if( byIndex > 0 && byIndex < 4 ) m_pPlayer->SetJob( byStage, byIndex ) ; break ;
		}
	}
}

void CQuestGroup::RegistCheckTime( DWORD dwQuestIdx, DWORD dwSubQuestIdx, BYTE bType, DWORD dwDay, DWORD dwHour, DWORD dwMin )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return;

	BOOL bStart = TRUE;
	stTIME curTime, regTime;
	curTime.value = GetCurTime();
	if( bType == 1 )		// 고정 시간
	{		
		if( curTime.GetHour() > dwHour )
		{
			regTime.SetTime( 0, 0, dwDay, 0, 0, 0 );
			regTime += curTime;
			regTime.SetTime( regTime.GetYear(), regTime.GetMonth(), regTime.GetDay(), dwHour, dwMin, 0 );
		}
		else if( curTime.GetHour() == dwHour )
		{
			if( curTime.GetMinute() < dwMin )
			{
				regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
			}
			else
			{
				regTime.SetTime( 0, 0, dwDay, 0, 0, 0 );
				regTime += curTime;
				regTime.SetTime( regTime.GetYear(), regTime.GetMonth(), regTime.GetDay(), dwHour, dwMin, 0 );
			}
		}
		else
		{
			regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
		}
	}
	else if( bType == 2 )	// 일정 시간후
	{
		regTime.SetTime( 0, 0, dwDay, dwHour, dwMin, 0 );
		regTime += curTime;
	}
	else if( bType == 3 )	// 일정 시간후 실패
	{
		regTime.SetTime( 0, 0, dwDay, dwHour, dwMin, 0 );
		regTime += curTime;
	}
	else if( bType == 4 )	// 고정 시간에 실패
	{
		regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
	}
	else
	{
		regTime.value = 0;
		bStart = FALSE;
	}

	pQuest->RegistCheckTime( bStart, bType, regTime );	
}


DWORD CQuestGroup::GetSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return -1;

	return pQuest->GetSubQuestData( dwSubQuestIdx );
}
