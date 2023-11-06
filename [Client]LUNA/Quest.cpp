// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "Quest.h"																			// ����Ʈ ����� �ҷ��´�.

#include "..\[CC]Quest\QuestInfo.h"															// ����Ʈ ���� ����� �ҷ��´�.
#include "..\[CC]Quest\SubQuestInfo.h"														// ���� ����Ʈ ���� ����� �ҷ��´�.

#include "ObjectManager.h"																	// ������Ʈ �Ŵ��� ����� �ҷ��´�.
#include "StatusIconDlg.h"																	// ���� ������ ���̾�α� ����� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuest::CQuest( CQuestInfo* pQuestInfo )													// ������ �Լ�.
{
	m_pQuestInfo = pQuestInfo;																// ����Ʈ ������ �޾� ��� ������ �����Ѵ�.

	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );				// ���� ����Ʈ ������ ��� �迭�� �޸� ������ �Ѵ�.

	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );							// ����Ʈ �������� 0��° ���� ����Ʈ ������ �޾� �迭0���� �����Ѵ�.

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();									// ���� ����Ʈ ī��Ʈ�� �޾� ��� ī��Ʈ�� �����Ѵ�.


	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );							// ���� ����Ʈ ���̺��� �޸� �����Ѵ�.

	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;												// ���� ����Ʈ �÷���, ������, �ð��� 0���� �����Ѵ�.
	
	m_bCheckTime = FALSE;																	// �ð�üũ ���θ� FALSE�� �����Ѵ�.
	m_CheckType = 0;																		// üũ Ÿ���� 0���� �����Ѵ�.
	m_CheckTime.value = 0;																	// �ð� ���� 0���� �����Ѵ�.
}

CQuest::~CQuest()																			// �Ҹ��� �Լ�.
{
}

void CQuest::Initialize()																	// �ʱ�ȭ �Լ�.
{
	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );				// ���� ����Ʈ ������ ��� �迭�� �޸� �����Ѵ�.

	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );							// ����Ʈ �������� 0��° ���� ����Ʈ ������ �޾� �迭0���� �����Ѵ�.

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();									// ���� ����Ʈ ī��Ʈ�� �޾� ��� ī��Ʈ�� �����Ѵ�.

	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );							// ���� ����Ʈ ���̺��� �޸� �����Ѵ�.

	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;												// ���� ����Ʈ �÷���, ������, Ÿ�� ���� 0���� �����Ѵ�.

	m_bCheckTime = FALSE;																	// �ð� üũ ���θ� FALSE�� �����Ѵ�.
	m_CheckType = 0;																		// üũ Ÿ���� 0���� �����Ѵ�.
	m_CheckTime.value = 0;																	// �ð� ���� 0���� �����Ѵ�.
}

void CQuest::SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )	// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
{
	m_CurSubQuestInfoArray[0] = NULL;														// ����, ���� ����Ʈ ���� �迭 0�� null ó�� �Ѵ�.
	m_dwSubQuestFlag = dwSubQuestFlag;														// ���� ����Ʈ �÷��׸� �����Ѵ�.
	m_dwData = dwData;																		// �����͸� �����Ѵ�.
	m_dwTime = dwTime;																		// �ð� ���� �����Ѵ�.

	m_CheckType = bCheckType;																// üũ Ÿ���� �����Ѵ�.
	m_CheckTime.value = dwCheckTime;														// üũ Ÿ���� �����Ѵ�.

	if( m_CheckType != 0 )																	// üũ Ÿ���� 0�� ���� ������,
	{
		m_bCheckTime = TRUE;																// �ð� üũ ���θ� TRUE�� �����Ѵ�.

		if( GetCurTime() < m_CheckTime.value )												// ���� �ð��� üũ Ÿ�Ӻ��� ������,
            STATUSICONDLG->AddQuestIconCount();												// ����Ʈ ������ ī��Ʈ�� �߰��Ѵ�.
	}
}

void CQuest::SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwSubData, DWORD dwSubTime )		// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );	// ���� ����Ʈ �ε����� �ش��ϴ�, ���� ����Ʈ �迭�� ����Ʈ ������ ��´�.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;										// ���� ����Ʈ �����͸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;										// ���� ����Ʈ �ð��� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();	// ���� ����Ʈ ī��Ʈ�� �����Ѵ�.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// �����ϴ� ���� ����Ʈ �ε����� �����Ѵ�.
}

void CQuest::StartQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )					// ����Ʈ�� �����ϴ� �Լ�.
{
	m_CurSubQuestInfoArray[0] = NULL;														// ���� ����Ʈ ���� �迭0�� NULL ó�� �Ѵ�.
	m_dwSubQuestFlag = dwSubQuestFlag;														// ���� ����Ʈ �÷��׸� �����Ѵ�.
	m_dwData = dwData;																		// �����͸� �����Ѵ�.
	m_dwTime = dwTime;																		// �ð��� �����Ѵ�.
}

void CQuest::EndQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )					// ����Ʈ�� �����ϴ� �Լ�.
{
	if( dwSubQuestFlag == 0 )	// �ݺ�														// ���� ����Ʈ �÷��װ� 0�� ���ٸ�,
	{
		Initialize();																		// �ʱ�ȭ �Լ��� ȣ���Ѵ�.
	}
	else																					// �׷��� ������,
	{
		m_dwSubQuestFlag = dwSubQuestFlag;													// ���� ����Ʈ �÷��׸� �����Ѵ�.
		m_dwData = dwData;																	// �����͸� �����Ѵ�.
		m_dwTime = dwTime;																	// �ð� ���� �����Ѵ�.

		m_bCheckTime = FALSE;																// �ð� üũ ���θ� FALSE�� �����Ѵ�.
		m_CheckType = 0;																	// üũ Ÿ���� 0���� �����Ѵ�.
		m_CheckTime.value = 0;																// �ð� ���� 0���� �����Ѵ�.
	}
}

void CQuest::StartSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )	// ���� ����Ʈ�� �����ϴ� �Լ�.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );					// ���� ����Ʈ ������ �����Ѵ�.
	m_dwSubQuestFlag = dwSubQuestFlag;																		// ���� ����Ʈ �÷��׸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// ���� ����Ʈ �����͸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// ���� ����Ʈ �ð��� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();		// ���� ����Ʈ ī��Ʈ�� �����Ѵ�.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// �����ϴ� ���� ����Ʈ �ε����� �����Ѵ�.
}

void CQuest::EndSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )		// ���� ����Ʈ�� �����ϴ� �Լ�.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = NULL;															// ���� ����Ʈ ������ null ó���� �Ѵ�.
	m_dwSubQuestFlag = dwSubQuestFlag;																		// ���� ����Ʈ �÷��׸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// ���� ����Ʈ �����͸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// ���� ����Ʈ �ð��� �����Ѵ�.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// �����ϴ� ���� ����Ʈ �ε����� �����Ѵ�.
}

void CQuest::UpdateSubQuestData( DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )	// ���� ����Ʈ�� ������Ʈ �ϴ� �Լ�.
{
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = dwMaxCount;													// ���� ����Ʈ �ִ� ī��Ʈ�� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// ���� ����Ʈ �����͸� �����Ѵ�.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// ���� ����Ʈ �ð��� �����Ѵ�.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// �����ϴ� ���� ����Ʈ �ε����� �����Ѵ�.
}

DWORD CQuest::GetQuestIdx()																					// ����Ʈ �ε����� ��ȯ�ϴ� �Լ�.
{
	return m_pQuestInfo->GetQuestIdx();																		// ����Ʈ ������ ����Ʈ �ε����� ��ȯ�Ѵ�.
}

BOOL CQuest::IsNpcRelationQuest( DWORD dwNpcIdx )															// npc���� ����Ʈ���� üũ�ϴ� �Լ�.
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// ���� ����Ʈ ����ŭ for���� ������.
	{
		if( m_CurSubQuestInfoArray[i] )																		// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ ��ȿ�ϸ�,
		if( m_CurSubQuestInfoArray[i]->IsNpcRelationQuest( dwNpcIdx ) )										// npc ���� ����Ʈ�� �´ٸ�,
			return TRUE;																					// TRUE�� �����Ѵ�.
	}
	return FALSE;																							// FALSE�� �����Ѵ�.
}

DWORD CQuest::GetNpcScriptPage( DWORD dwNpcIdx )															// NPC ��ũ��Ʈ �������� ��ȯ�ϴ� �Լ�.
{
	DWORD dwPage = 0;																						// �������� ���� ������ �����ϰ� 0���� �����Ѵ�.

	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// ���� ����Ʈ ����ŭ for���� ������.
	{
		if( m_CurSubQuestInfoArray[i] )																		// ���� ���� ����Ʈ ������ ��ȿ�ϴٸ�,
		if( dwPage = m_CurSubQuestInfoArray[i]->GetNpcScriptPage( dwNpcIdx ) )								// ���� ���� ����Ʈ ������ npc ��ũ��Ʈ �������� ��ȿ�ϴٸ�,
			return dwPage;																					// �������� �����Ѵ�.
	}
	return dwPage;																							// �������� �����Ѵ�.
}

BOOL CQuest::IsSubQuestComplete( DWORD dwSubQuestIdx )														// ���� ����Ʈ�� �Ϸ� ���θ� �����ϴ� �Լ�.
{
	DWORD value = 1;																						// ���� 1�� �����Ѵ�.
	return ( m_dwSubQuestFlag & ( value << (31-dwSubQuestIdx) ) ) ? TRUE : FALSE;							// ���� ����Ʈ �÷��׿� ���� ���� �����Ѵ�.
}

DWORD CQuest::GetNpcMarkType( DWORD dwNpcIdx )																// npc ��ũ Ÿ���� ��ȯ�Ѵ�.
{
	DWORD dwNpcMarkType = 0;																				// npc ��ũ Ÿ���� 0���� �����Ѵ�.

	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
	{
		if( m_CurSubQuestInfoArray[i] )																		// ���� ����Ʈ ������ ��ȿ�ϸ�,
		if( dwNpcMarkType = m_CurSubQuestInfoArray[i]->GetNpcMarkType( dwNpcIdx ) )							// ���� ����Ʈ �������� npc �ε����� ��ũ Ÿ���� �޴´�.
			return dwNpcMarkType;																			// ��ũ Ÿ�� ������ ��ȿ�ϸ�, npc Ÿ���� �����Ѵ�.
	}
	return dwNpcMarkType;																					// ��ũ Ÿ���� �����Ѵ�.
}

CSubQuestInfo* CQuest::GetSubQuestInfo( DWORD dwSubQuestIdx )												// ���� ����Ʈ ������ �����ϴ� �Լ�.
{
	return m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );													// ���� ���� ����Ʈ �ε����� ���� ����Ʈ ������ ��ȯ�Ѵ�.
}

int CQuest::IsQuestState( DWORD dwNpcIdx )																	// ����Ʈ ���¸� ��ȯ�ϴ� �Լ�.
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
	{
		if( m_CurSubQuestInfoArray[i] )																		// ���� ����Ʈ ������ ��ȿ�ϸ�,
		{
			int rt = m_CurSubQuestInfoArray[i]->IsQuestState( dwNpcIdx );									// ���� ���� ����Ʈ�� ���� ���� ����� �޴´�.
			if( rt == 1 || rt == 2 )																		// ����� 1�̳� 2�̸�, 
				return rt;																					// ����� �����Ѵ�.
		}
	}
	return 0;																								// 0�� �����Ѵ�.
}

BOOL CQuest::CanItemUse( DWORD dwQuestIdx )																	// ������ ��뿩�θ� �����Ѵ�.
{
	if( m_CurSubQuestInfoArray[0] )																			// ���� ����Ʈ ���� �迭 0���� ��ȿ�ϸ�,
	{
		return m_CurSubQuestInfoArray[0]->CheckLimitCondition( dwQuestIdx );								// ����Ʈ ������ üũ�Ͽ� �����Ѵ�.
	}
	return FALSE;																							// FALSE�� �����Ѵ�.
}

void CQuest::RegistCheckTime( BYTE bCheckType, DWORD dwCheckTime )											// �ð� üũ�� ����ϴ� �Լ�.
{
	m_bCheckTime = TRUE;																					// �ð� üũ ���θ� TRUE�� �����Ѵ�.
	m_CheckType = bCheckType;																				// üũ Ÿ���� �����Ѵ�.
	m_CheckTime.value = dwCheckTime;																		// Ÿ�� ���� �����Ѵ�.
}

void CQuest::UnRegistCheckTime()																			// �ð� üũ�� ���� �ϴ� �Լ�.
{
	m_bCheckTime = FALSE;																					// �ð� üũ ���θ� FALSE�� �����Ѵ�.
	m_CheckType = 0;																						// üũ Ÿ���� 0���� �����Ѵ�.
	m_CheckTime.value = 0;																					// Ÿ�� ���� 0���� �����Ѵ�.
}