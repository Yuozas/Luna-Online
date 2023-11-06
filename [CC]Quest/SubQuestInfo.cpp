
// SubQuestInfo.cpp: implementation of the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "SubQuestInfo.h"																	// ���� ����Ʈ ���� �ش��� �ҷ��´�.

#include "QuestLimit.h"																		// ����Ʈ ���� ����� �ҷ��´�.
#include "QuestTrigger.h"																	// ����Ʈ Ʈ���� ����� �ҷ��´�.
#include "QuestNpcScript.h"																	// ����Ʈ npc ��ũ��Ʈ ����� �ҷ��´�.
#include "QuestNpc.h"																		// ����Ʈ npc ����� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubQuestInfo::CSubQuestInfo()																// ������ �Լ�.
{
	m_dwMaxCount = 0;																		// �ִ� ī��Ʈ�� 0���� �����Ѵ�.
}

CSubQuestInfo::~CSubQuestInfo()																// �Ҹ��� �Լ�.
{
	Release();																				// ���� �Լ��� ȣ���Ѵ�.
}

void CSubQuestInfo::Release()																// ���� �Լ�.
{
	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� ������ ������ �����ϰ� null ó���� �Ѵ�.

	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ��ġ ������ �ش��ϴ� ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ���� üũ�Ѵ�.
		{
			delete pLimit;																	// ����Ʈ ���� ������ �����Ѵ�.
		}
	}

	m_QuestLimitList.RemoveAll();															// ����Ʈ ���� ����Ʈ�� ��� ����.

	CQuestTrigger* pTrigger = NULL;															// ũ���� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	pos = m_QuestTriggerList.GetHeadPosition();												// ����Ʈ Ʈ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while ���� ������.
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );						// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.

		if( pTrigger )																		// Ʈ���� ������ ��ȿ���� üũ�Ѵ�.
		{
			delete pTrigger;																// Ʈ���� ������ �����Ѵ�.
		}
	}

	m_QuestTriggerList.RemoveAll();															// Ʈ���� ����Ʈ�� ��� ����.

	CQuestNpcScript* pNpcScript = NULL;														// ����Ʈ npc ��ũ��Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	pos = m_QuestNpcScriptList.GetHeadPosition();											// ����Ʈ npc ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// ��ġ ������ �ش��ϴ� npc ��ũ��Ʈ ������ �޴´�.

		if( pNpcScript )																	// npc ��ũ��Ʈ ������ ��ȿ���� üũ�Ѵ�.
		{
			delete pNpcScript;																// npc ��ũ��Ʈ ������ �����Ѵ�.
		}
	}

	m_QuestNpcScriptList.RemoveAll();														// npc ��ũ��Ʈ ����Ʈ�� ��� ����.
	
	CQuestNpc* pQuestNpc = NULL;															// ����Ʈ npc ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	pos = m_QuestNpcList.GetHeadPosition();													// ����Ʈ npc ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pQuestNpc = (CQuestNpc*)m_QuestNpcList.GetNext( pos );								// ����Ʈ npc ������ �޴´�.

		if( pQuestNpc )																		// ����Ʈ npc ������ ��ȿ���� üũ�Ѵ�.
		{
			delete pQuestNpc;																// ����Ʈ npc ������ �����Ѵ�.
		}
	}

	m_QuestNpcList.RemoveAll();																// ����Ʈ npc ����Ʈ�� ��� ����.
}

void CSubQuestInfo::AddQuestLimit( CQuestLimit* pQuestLimit )								// ����Ʈ ���� ������ �߰��ϴ� �Լ�.
{
	if( !pQuestLimit ) return;																// ����Ʈ ���� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	m_QuestLimitList.AddTail( pQuestLimit );												// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
}

void CSubQuestInfo::AddQuestTrigger( CQuestTrigger* pQuestTrigger )							// ����Ʈ Ʈ���Ÿ� �߰��ϴ� �Լ�.
{
	if( !pQuestTrigger ) return;															// ����Ʈ Ʈ���� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	m_QuestTriggerList.AddTail( pQuestTrigger );											// ����Ʈ Ʈ���� ����Ʈ�� �߰��Ѵ�.
}

void CSubQuestInfo::AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript )					// ����Ʈ npc ��ũ��Ʈ�� �߰��ϴ� �Լ�.
{
	if( !pQuestNpcScript ) return;															// ����Ʈ npc ��ũ��Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	m_QuestNpcScriptList.AddTail(  pQuestNpcScript );										// ����Ʈ npc ��ũ��Ʈ ����Ʈ�� �߰��Ѵ�.
}

void CSubQuestInfo::AddQuestNpc( CQuestNpc* pQuestNpc )										// ����Ʈ npc �� �߰��ϴ� �Լ�.
{
	if( !pQuestNpc ) return;																// ����Ʈ npc ������ ��ȿ ���� ������ ���� ó���� �Ѵ�.

	m_QuestNpcList.AddTail(  pQuestNpc );													// ����Ʈ npc ����Ʈ�� �߰��Ѵ�.
}

void CSubQuestInfo::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent )	// ����Ʈ �̺�Ʈ�� ó���ϴ� �Լ�.
{
	// limit
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ�ϴٸ�,
		{
			if( !pLimit->CheckLimit( pPlayer, pQuestGroup, pQuest ) )						// ����Ʈ ���ѿ� �ɸ����� üũ���Ѵ�.
			{
				return;																		// ���� ó���� �Ѵ�.
			}
		}
	}
	
	// trigger
	pos = m_QuestTriggerList.GetHeadPosition();												// Ʈ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestTrigger* pTrigger = NULL;															// Ʈ���� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ �ִµ��� while���� ������.
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );						// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.

		if( pTrigger )																		// Ʈ���� ������ ��ȿ���� üũ�Ѵ�.
		{
			pTrigger->OnQuestEvent( pPlayer, pQuestGroup, pQuest, pQuestEvent );			// Ʈ������ ����Ʈ �̺�Ʈ�� �����Ѵ�.
		}
	}
}

BOOL CSubQuestInfo::IsNpcRelationQuest( DWORD dwNpcIdx )									// npc ���� ����Ʈ���� üũ�ϴ� �Լ�.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// ����Ʈ �ε����� ���� ����Ʈ �ε����� ���� ������ �����Ѵ�.

	dwQuestIdx = dwSubQuestIdx = 0;															// ����Ʈ �ε����� ���� ����Ʈ �ε����� 0���� �����Ѵ�.

	DWORD rt = 0;																			// ����� ���� ������ �����ϰ� 0���� �����Ѵ�.

	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// npc ��ũ��Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// npc ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȣ�� ���� while���� ������.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// ��ġ�� �ش��ϴ� ��ũ��Ʈ ������ �޴´�.

		if( pNpcScript )																	// ��ũ��Ʈ ������ ��ȿ�ϴٸ�,
		{
			if( rt = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )		// npc�� ���� �̸����� üũ�Ͽ� ����� �޴´�.
			{
				break;																		// ���� �̸��̸� while ���� ���� ���´�.
			}
		}
	}

	if( rt == 0 )																			// ����� 0�� ������,
	{
		return FALSE;																		// FALSE �� �����Ѵ�.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{	
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ��ġ�� �ش��ϴ� ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// ����Ʈ ���ѿ� �ɸ����� üũ�Ѵ�.
			{
				return FALSE;																// FALSE ���� ó���� �Ѵ�.
			}
		}
	}

	return TRUE;																			// TRUE ���� ó���� �Ѵ�.
}

DWORD CSubQuestInfo::GetNpcScriptPage( DWORD dwNpcIdx )										// NPC ��ũ��Ʈ �������� �����ϴ� �Լ�.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// ����Ʈ �ε����� ���� ����Ʈ �ε����� ���� ������ �����Ѵ�.

	dwQuestIdx = dwSubQuestIdx = 0;															// ����Ʈ�� ���� ����Ʈ �ε����� 0���� �����Ѵ�.

	DWORD dwPage = 0;																		// �������� ���� ������ �����ϰ� 0���� �����Ѵ�.
	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// NPC ��ũ��Ʈ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// NPC ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// npc ��ũ��Ʈ ������ �޴´�.

		if( pNpcScript )																	// ��ũ��Ʈ ������ ��ȿ�ϴٸ�,
		{
			if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// ���� npc ���� üũ�Ͽ� �������� �޴´�.
			{
				break;																		// while���� �������´�.
			}
		}
	}

	if( dwPage == 0 )																		// �������� 0�� ���ٸ�,
	{
		return dwPage;																		// �������� �����Ѵ�.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ��ġ�� �ش��ϴ� ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// ����Ʈ ���ѿ� �ɸ����� üũ�Ѵ�.
			{
				return 0;	// limit														// 0�� �����Ѵ�.
			}
		}
	}

	return dwPage;																			// �������� �����Ѵ�.
}

DWORD CSubQuestInfo::GetNpcMarkType( DWORD dwNpcIdx )										// npc ��ũ Ÿ���� ��ȯ�ϴ� �Լ�.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// ����Ʈ �ε����� ���� ����Ʈ �ε����� ���� ������ �����Ѵ�.

	dwQuestIdx = dwSubQuestIdx = 0;															// ����Ʈ �ε����� ��������Ʈ �ε������� 0���� �����Ѵ�.

	DWORD dwNpcMark = 0;																	// npc ��ũ�� ���� ������ �����ϰ� 0���� �����Ѵ�.

	// npcidx is same
	CQuestNpcScript* pNpcScript = NULL;														// npc ��ũ��Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// npc ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// ��ġ�� �ش��ϴ� npc ��ũ��Ʈ ������ �޴´�.

		if( pNpcScript )																	// ��ũ��Ʈ ������ ��ȿ�ϸ�,
		{	
			if( dwNpcMark = pNpcScript->IsSameNpcMark( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// ���� �̸��� npc��� npc ��ũ�� �޴´�.
			{
				break;																		// while���� �������´�.
			}
		}
	}

	if( dwNpcMark == 0 )																	// npc ��ũ�� 0�� ���ٸ�,
	{
		return dwNpcMark;																	// npc ��ũ�� �����Ѵ�.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// ����Ʈ ���ѿ� �ɸ����� üũ�Ѵ�.
			{
				return 0;	// limit														// 0�� �����Ѵ�.
			}
		}
	}

	return dwNpcMark;																		// npc ��ũ�� �����Ѵ�.
}
	
int CSubQuestInfo::IsQuestState( DWORD dwNpcIdx )											// ����Ʈ ���¸� �����ϴ� �Լ�.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// ����Ʈ �ε����� ���� ����Ʈ �ε����� ���� ������ �����Ѵ�.

	dwQuestIdx = dwSubQuestIdx = 0;															// ����Ʈ �ε����� ���� ���� �ε����� 0���� �����Ѵ�.

	DWORD dwPage = 0;																		// �������� ���� ������ �����ϰ� 0���� �����Ѵ�.

	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// npc ��ũ��Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// ����Ʈ npc ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					//  npc ��ũ��Ʈ ������ �޴´�.

		if( pNpcScript )																	// npc ��ũ��Ʈ ������ ��ȿ�ϴٸ�,
		{
			if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// �������� �޴´�.
			{
				break;																		// while���� �������´�.
			}
		}
	}

	if( dwPage == 0 )																		// �������� 0�� ���ٸ�,
	{
		return 0;																			// 0�� �����Ѵ�.
	}
	
	// if check limit
	pos = m_QuestLimitList.GetHeadPosition();												// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ��ġ ������ �ش��ϴ� ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// ����Ʈ ���ѿ� �ɸ����� üũ�Ѵ�.
			{
				return 0;	// limit														// 0�� �����Ѵ�.
			}
		}
	}

	if( dwSubQuestIdx == 0 )	return 1;			// ���డ���� ����						// ���� ����Ʈ �ε����� 0�� ������, ���డ���� ����.
	else						return 2;			// �������� ����						// ���� ����Ʈ �ε����� 0�� ���� ������, �������� ����.

	return 0;		// �ƹ��͵� �ƴϴ�.	
}

BOOL CSubQuestInfo::CheckLimitCondition( DWORD dwQuestIdx )									// ����Ʈ ���� ���¸� Ȯ���ϴ� �Լ�.
{
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimit* pLimit = NULL;																// ����Ʈ ���� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// ����Ʈ ���� ������ �޴´�.

		if( pLimit )																		// ����Ʈ ���������� ��ȿ�ϸ�,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, 0 ) )										// ����Ʈ ������ üũ�Ѵ�.
			{
				return FALSE;	// limit													// FALSE�� �����Ѵ�.
			}
		}
	}
	return TRUE;																			// TRUE�� �����Ѵ�.
}