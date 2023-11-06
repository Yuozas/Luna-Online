#include "stdafx.h"
#include "PCRoomManager.h"
#include "WindowIDEnum.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "DealDialog.h"
#include "MiniMapDlg.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[CC]Skill/Client/Info/BuffSkillInfo.h"
#include "FriendDialog.h"

CPCRoomManager::CPCRoomManager()
{
}

CPCRoomManager::~CPCRoomManager()
{
//	ClearMemberInfo();
}

BOOL CPCRoomManager::CanBuyItem( DWORD dwItemIdx, DWORD dwBuyCnt )
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo( dwItemIdx );
	if( !pItemInfo || pItemInfo->wPointType != eItemPointType_PCRoomPoint )
		return FALSE;

	// ����Ʈ ����
	if( m_dwHeroPoint < dwBuyCnt )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2011 ) );
		return FALSE;
	}

	return TRUE;
}

void CPCRoomManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
		// PC�濡 �����Ѱ��
	case MP_PCROOM_CONNECT_INFO:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
			cWindow* pMallBtn = pMiniMapDlg->GetWindowForID( MNM_PCROOM_MALL_BTN );
			if( !pMallBtn )
				break;

			pMallBtn->SetActive( TRUE );

			// PC�� ������ ���� ��ư ����
			BOOL bCanProvide = (pmsg->bData ? TRUE : FALSE);
			cWindow* pProvideItemBtn = pMiniMapDlg->GetWindowForID( MNM_PCROOM_PROVIDE_ITEM_BTN );
			if( !pProvideItemBtn )
				break;

			pProvideItemBtn->SetActive( bCanProvide );
		}
		break;

	case MP_PCROOM_PROVIDE_ITEM_ACK:
		{
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;
			DWORD dwItemIdx[5] = {0,};
			dwItemIdx[0] = pmsg->dwData1;
			dwItemIdx[1] = pmsg->dwData2;
			dwItemIdx[2] = pmsg->dwData3;
			dwItemIdx[3] = pmsg->dwData4;
			dwItemIdx[4] = pmsg->dwData5;

			for( int i=0 ; i<sizeof(dwItemIdx)/sizeof(*dwItemIdx) ; ++i )
			{
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( dwItemIdx[i] );
				if( pItemInfo )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2012 ), pItemInfo->ItemName );
			}

			// PC�� ������ ���� ��ư ��Ȱ��ȭ
			CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
			cWindow* pProvideItemBtn = pMiniMapDlg->GetWindowForID( MNM_PCROOM_PROVIDE_ITEM_BTN );
			if( pProvideItemBtn )
				pProvideItemBtn->SetActive( FALSE );
		}
		break;

		// PC�� Mall Open
	case MP_PCROOM_OPEN_MALL_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if( pmsg->dwData1 != HEROID )
				break;

			if( HERO->GetState() == eObjectState_Die)
				break;

			WORD wDealerIdx = WORD(pmsg->dwData2);
			DWORD dwPCRoomPoint = pmsg->dwData3;

			GAMEIN->GetDealDialog()->ShowDealer( wDealerIdx );
			GAMEIN->GetDealDialog()->ShowDealDialog( TRUE );
			GAMEIN->GetDealDialog()->SetActivePointInfo( TRUE, dwPCRoomPoint );

			SetHeroPoint( dwPCRoomPoint );
		}
		break;

	// 100106 ONS PC���� �ƴϸ� �޼��� ���.
	case MP_PCROOM_OPEN_MALL_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2009 ) );
		}
		break;

	case MP_PCROOM_CHANGE_POINT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if( pmsg->dwData1 != HEROID )
				break;

			DWORD dwPCRoomPoint = pmsg->dwData2;
			GAMEIN->GetDealDialog()->SetActivePointInfo( GAMEIN->GetDealDialog()->IsActivePointInfo(), dwPCRoomPoint );

			SetHeroPoint( dwPCRoomPoint );
		}
		break;

	// 100105 ONS PC������ο� �޼��� ���
	//case MP_PCROOM_PROVIDE_DAILYBUFF:
	//	{
	//		MSG_PCROOM_BUFF_INFO* pmsg = (MSG_PCROOM_BUFF_INFO*)pMsg;

	//		for( int i=0 ; i<sizeof(pmsg->dwBuffIndex)/sizeof(*pmsg->dwBuffIndex) ; ++i )
	//		{
	//			cBuffSkillInfo* pBuffSkillInfo = NULL ;												
	//			pBuffSkillInfo =  (cBuffSkillInfo*)SKILLMGR->GetSkillInfo( pmsg->dwBuffIndex[i] ) ;
	//			if( pBuffSkillInfo )
	//			{
	//				// 100219 ONS PC��������� ������� ä��â �޼����� �ߺ���µ��� �ʵ��� ����.
	//				BOOL bIsRepeat = FALSE;
	//				for( int n = 0; n < i; n++ )
	//				{
	//					const cBuffSkillInfo* pTempSkillInfo = (cBuffSkillInfo*)SKILLMGR->GetSkillInfo( pmsg->dwBuffIndex[n] );												
	//					if( !strcmp( pTempSkillInfo->GetName(), pBuffSkillInfo->GetName() ) )
	//					{
	//						bIsRepeat = TRUE;
	//						break;
	//					}
	//				}
	//				if( bIsRepeat ) continue;
	//				
	//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2017 ), pBuffSkillInfo->GetName() );
	//			}
	//		}	
	//	}
	//	break;
	//// 100312 ONS PC�� ��Ƽ������ �и��ؼ� ó��.
	//case MP_PCROOM_PROVIDE_PARTYBUFF:
	//	{
	//		MSG_PCROOM_BUFF_INFO* pmsg = (MSG_PCROOM_BUFF_INFO*)pMsg;
	//		DWORD dwPartyStage = pmsg->wStage;
	//		BYTE  bIsChanged = pmsg->bIsChanged;
	//		
	//		// PC�� ��Ƽ������ �����Ǿ��� ���.
	//		if( TRUE == bIsChanged && 0 == dwPartyStage )
	//		{
	//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2045 ) );
	//			break;
	//		}

	//		for( int i=0 ; i<sizeof(pmsg->dwBuffIndex)/sizeof(*pmsg->dwBuffIndex) ; ++i )
	//		{
	//			cBuffSkillInfo* pBuffSkillInfo = NULL ;												
	//			pBuffSkillInfo =  (cBuffSkillInfo*)SKILLMGR->GetSkillInfo( pmsg->dwBuffIndex[i] ) ;
	//			if( pBuffSkillInfo )
	//			{
	//				BOOL bIsRepeat = FALSE;
	//				for( int n = 0; n < i; n++ )
	//				{
	//					const cBuffSkillInfo* pTempSkillInfo = (cBuffSkillInfo*)SKILLMGR->GetSkillInfo( pmsg->dwBuffIndex[n] );												
	//					if( !strcmp( pTempSkillInfo->GetName(), pBuffSkillInfo->GetName() ) )
	//					{
	//						bIsRepeat = TRUE;
	//						break;
	//					}
	//				}
	//				if( bIsRepeat ) continue;
	//				
	//				if( !bIsChanged )
	//				{
	//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2018 ), pBuffSkillInfo->GetName(), dwPartyStage + 1 );
	//				}
	//				else
	//				{
	//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 2038 ), pBuffSkillInfo->GetName(), dwPartyStage + 1 );
	//				}
	//			}
	//		}
	//	}
	//	break;

	// 100118 ONS ģ������ ���̾�α׿� PC���������� ����߰�
	/*case MP_PCROOM_INFO_ACK:
		{
			MSG_PCROOM_MEMBER_LIST* pmsg = (MSG_PCROOM_MEMBER_LIST*)pMsg;
			SetMemberInfo( pmsg->PCRoomMemList, pmsg->count );
			
			GAMEIN->GetFriendDialog()->SetPCRoomInfo( TRUE, pmsg->PartyStage, pmsg->StageMemberMin );
			GAMEIN->GetFriendDialog()->SetPCRoomMemberList(1);
		}
		break;
	}*/
}

// 100118 ONS PC�� ���Ӹ�� ������ �����Ѵ�.
//void CPCRoomManager::ClearMemberInfo()
//{
//	PTRLISTPOS pos = m_PCRoomMemberList.GetHeadPosition();
//	while( pos )
//	{
//		PCROOM_MEMBER* pMember = (PCROOM_MEMBER*)m_PCRoomMemberList.GetNext( pos );
//		SAFE_DELETE(pMember);
//	}
//	m_PCRoomMemberList.RemoveAll();
//}
//
//// 100118 ONS PC�� ���Ӹ�� ������ �����Ѵ�.
//void CPCRoomManager::SetMemberInfo(PCROOM_MEMBER* pInfo,BYTE count)
//{
//	if( !pInfo )
//		return;
//
//	ClearMemberInfo();
//
//	int num = 0;
//	for(int i=0; i<count; ++i)
//	{
//		if(pInfo[i].CharIndex == 0)
//			break;
//		PCROOM_MEMBER* pMemberInfo = new PCROOM_MEMBER;
//		pMemberInfo->CharIndex = pInfo[i].CharIndex;
//		pMemberInfo->CharClass = pInfo[i].CharClass;
//		SafeStrCpy(pMemberInfo->Name, pInfo[i].Name, MAX_NAME_LENGTH+1);
//		m_PCRoomMemberList.AddTail(pMemberInfo);
//		
//		if( (i % MAX_PCROOM_MEMBER_LIST) == 0 )
//		{
//			num = i/MAX_PCROOM_MEMBER_LIST;
//			m_pListPos[num] = m_PCRoomMemberList.GetTailPosition();
//		}			
//	}
//}
//
//// 100118 ONS PC�� ���Ӹ�� ������ �����´�.
//void CPCRoomManager::GetMemberInfoList(PCROOM_MEMBER_LIST* rtInfo, int num)
//{
//	if((num-1) < 0 || (num-1) >= MAX_PCROOM_MEMBER_NUM/MAX_PCROOM_MEMBER_LIST)
//	{
//		return;
//	}
//
//	PCROOM_MEMBER* pMember = NULL;	
//	PTRLISTPOS pos = m_pListPos[num-1];
//	for(int i=0; i<MAX_PCROOM_MEMBER_LIST; ++i)
//	{
//		if(pos)
//		{ 
//			pMember = (PCROOM_MEMBER*)m_PCRoomMemberList.GetAt(pos);
//			if(pMember)
//			{
//				rtInfo->Member[i].CharIndex = pMember->CharIndex;
//				rtInfo->Member[i].CharClass = pMember->CharClass;
//				SafeStrCpy(rtInfo->Member[i].Name, pMember->Name, MAX_NAME_LENGTH+1);
//			
//				m_PCRoomMemberList.GetNext(pos);
//			}
//		}
//		else
//			break;
//	}
//	rtInfo->totalnum = (m_PCRoomMemberList.GetCount()-1)/MAX_PCROOM_MEMBER_LIST + 1;
}