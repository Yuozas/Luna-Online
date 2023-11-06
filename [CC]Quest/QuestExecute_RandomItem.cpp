// QuestExecute_RandomItem.cpp: implementation of the CQuestExecute_RandomItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestExecute_RandomItem.h"													// ����Ʈ ���� ���� ������ ����� �ҷ��´�.

#include "QuestScriptLoader.h"															// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_																		// �� ������ ����Ǿ� �ִٸ�,

#include "QuestGroup.h"																	// ����Ʈ �׷� ����� �ҷ��´�.

#include "ItemSlot.h"

#include "Player.h"

#include "ItemManager.h"

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestExecute_RandomItem::CQuestExecute_RandomItem( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_dwMaxItemCount, pTokens );										// ������ �ִ� ���� �޴´�.
	GetScriptParam( m_dwRandomItemCount, pTokens );										// ���� ������ ī��Ʈ�� �޴´�.
	
	m_pRandomItem = new RANDOMITEM[m_dwMaxItemCount];									// �ִ� �� ��ŭ ���� ������ ������ ���� �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.

	for( DWORD i = 0; i < m_dwMaxItemCount; ++i )										// �ִ� ������ �� ��ŭ for���� ������.
	{
		GetScriptParam( m_pRandomItem[i].wItemIdx, pTokens );							// ������ �ε����� �޴´�.
		GetScriptParam( m_pRandomItem[i].wItemNum, pTokens );							// ������ ������ �޴´�.
		GetScriptParam( m_pRandomItem[i].wPercent, pTokens );							// �ۼ�Ʈ�� �޴´�.
	}
}

CQuestExecute_RandomItem::~CQuestExecute_RandomItem()									// �Ҹ��� �Լ�.
{
	if( m_pRandomItem )																	// ���� ������ ������ ��Ÿ���� ������ ������ ��ȿ�ϸ�,
	{
		delete [] m_pRandomItem;														// ���� ������ ������ �����Ѵ�.
		m_pRandomItem = NULL;															// ������ ��Ÿ���� �����͸� NULL ó���� �Ѵ�.
	}
}

// ���� �Լ�.
BOOL CQuestExecute_RandomItem::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_																		// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )															// ���� ������ Ȯ���Ѵ�.
	{
	case eQuestExecute_RandomTakeItem:													// ���� �������� �޴� �����̸�,
		{
			cPtrList* pRandomItemList = NULL ;											// ���� ������ ����Ʈ ������ ���� �����͸� �����Ѵ�.
			pRandomItemList = pQuestGroup->GetRandomItemList() ;						// ����Ʈ �׷��� ���� ������ ����Ʈ�� �޴´�.

			if( !pRandomItemList ) return FALSE ;

			RANDOMITEM* pItem ;

			PTRLISTPOS pos = NULL ;
			pos = pRandomItemList->GetHeadPosition() ;

			while(pos)
			{
				pItem = NULL ;
				pItem = (RANDOMITEM*)pRandomItemList->GetNext(pos) ;

				if( !pItem ) continue ;
		
				pQuestGroup->TakeItem( pPlayer, pItem->wItemIdx, pItem->wItemNum, MAX_QUEST_PROBABILITY );
			}

			pQuestGroup->ReleaseRandomItemList() ;										// ���� ����Ʈ �׷��� ���� ������ ����Ʈ�� ��� ����.
		}
		break;
	}

#endif	

	return TRUE;																		// TRUE�� �����Ѵ�.
}

int CQuestExecute_RandomItem::CheckCondition( PLAYERTYPE* pPlayer,				
							CQuestGroup* pQuestGroup, CQuest* pQuest )					
{
	int nErrorCode = e_EXC_ERROR_NO_ERROR ;												// ���� �ڵ带 ���� ������ �����ϰ� e_EXE_ERROR_NO_ERROR�� �����Ѵ�.

	if( !pPlayer )																		// �÷��̾� ������ ��ȿ���� ������, 
	{
		nErrorCode = e_EXC_ERROR_NO_PLAYERINFO ;										// �÷��̾� ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;																// ���� �ڵ带 return ó���� �Ѵ�.
	}

	if( !pQuestGroup )																	// ����Ʈ �׷� ������ ��ȿ���� ������,
	{
		nErrorCode = e_EXC_ERROR_NO_QUESTGROUP ;										// ����Ʈ �׷� ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;																// ���� �ڵ带 return ó���� �Ѵ�.
	}

	if( !pQuest )																		// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	{
		nErrorCode = e_EXC_ERROR_NO_QUEST ;												// ����Ʈ ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;																// ���� �ڵ带 return ó���� �Ѵ�.
	}

#ifdef _MAPSERVER_																		// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )															// ���� ������ Ȯ���Ѵ�.
	{
	case eQuestExecute_RandomTakeItem:													// ���� �������� �޴� �����̸�,
		{
			RANDOMITEM* pItem = NULL;													// ���� ������ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

			CItemSlot* pSlot ;															// �κ��丮 ���� ������ ���� �����͸� �����Ѵ�.

			cPtrList* pRandomItemList = NULL ;											// ���� ������ ����Ʈ ������ ���� �����͸� �����Ѵ�.
			pRandomItemList = pQuestGroup->GetRandomItemList() ;						// ����Ʈ �׷��� ���� ������ ����Ʈ�� �޴´�.

			if( !pRandomItemList )
			{
				nErrorCode = e_EXC_ERROR_NO_RANDOMITEMLIST ;							// ����Ʈ �׷��� ���� ������ ����Ʈ�� ���ٴ� ������ �����Ѵ�.
				return nErrorCode ;														// ���� �ڵ带 return ó���� �Ѵ�.
			}

			pQuestGroup->ReleaseRandomItemList() ;										// ���� ����Ʈ �׷��� ���� ������ ����Ʈ�� ��� ����.

			for( DWORD i = 0; i < m_dwRandomItemCount; ++i )							// �ִ� ������ �� ��ŭ for���� ������.
			{
				pItem = NULL ;
				pItem = GetRandomItem();												// ���� ������ ������ �޴´�.

				if( ! pItem ) continue ;												// ������ ��ȿ���� ������ continue ó���� �Ѵ�.

				pSlot = NULL ;															// ������ ���� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
				pSlot = pPlayer->GetSlot(eItemTable_Inventory) ;						// �÷��̾��� �κ��丮 ���� ������ �޴´�.

				if( !pSlot )															// �κ��丮 ������ ��ȿ���� ������,
				{
					nErrorCode = e_EXC_ERROR_NO_INVENTORYINFO ;							// �κ��丮 ������ ���ٴ� �����ڵ带 �����Ѵ�.
					return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.

					pQuestGroup->ReleaseRandomItemList() ;								// ���� ����Ʈ �׷��� ���� ������ ����Ʈ�� ��� ����.
				}

				WORD EmptyCellPos[255];													// �� ���� ��ġ�� ���� ������ �����Ѵ�.
				WORD EmptyCellNum;														// �� ���� ������ ���� ������ �����Ѵ�.

				WORD wResult = 0 ;														// �� ������ üũ ��� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

				wResult = ITEMMGR->CheckExtraSlot(pPlayer, pSlot, pItem->wItemIdx,		// �������� ���� ������ ������� ��� ���� �޴´�.
								pItem->wItemNum, EmptyCellPos, EmptyCellNum) ;

				if( wResult == 0 )														// ��� ���� 0�� ������,
				{
					nErrorCode = e_EXC_ERROR_NO_INVENTORYINFO_RANDOMITEM ;				// ���� �������� ���� �κ��丮�� �����ϴٴ� ó���� �Ѵ�.
					return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.

					pQuestGroup->ReleaseRandomItemList() ;								// ���� ����Ʈ �׷��� ���� ������ ����Ʈ�� ��� ����.
				}

				RANDOMITEM* pRandomItem = new RANDOMITEM ;
				pRandomItem->wItemIdx = pItem->wItemIdx ;
				pRandomItem->wItemNum = pItem->wItemNum ;
				pRandomItemList->AddTail(pRandomItem) ;									// �������� ������ ������ ������ �߰��Ѵ�.
			}
		}
		break;
	}

#endif //_MAPSERVER_

	return nErrorCode ;																	// �⺻ ���� �����Ѵ�.
}


RANDOMITEM* CQuestExecute_RandomItem::GetRandomItem()									// ���� ������ ������ ��ȯ�ϴ� �Լ�.
{
	WORD RandRate = rand()%10001;														// �������� �̴´�.
	WORD FromPercent = 0;																// From �ۼ�Ʈ ������ �����ϰ� 0���� �����Ѵ�.
	WORD ToPercent = 0;																	// To �ۼ�Ʈ ������ �����ϰ� 0���� �����Ѵ�.

	for( DWORD i = 0; i < m_dwMaxItemCount; ++i )										// �ִ� ������ �� ��ŭ for���� ������.
	{
		FromPercent = ToPercent;														// From �ۼ�Ʈ�� To �ۼ�Ʈ�� �����Ѵ�.

		ToPercent += m_pRandomItem[i].wPercent;											// To �ۼ�Ʈ�� ���� �ۼ�Ʈ�� ���Ѵ�.

		if( FromPercent <= RandRate && RandRate < ToPercent )							// From �ۼ�Ʈ�� ������ �����̰�, �������� To�ۼ�Ʈ ���� ������,
		{
			return &m_pRandomItem[i];													// ���� ������ ������ �����Ѵ�.
		}
	}

	return NULL;																		// NULL�� �����Ѵ�.
}
