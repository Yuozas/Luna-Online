// QuestNpc.cpp: implementation of the CQuestNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestNpc.h"																		// ����Ʈ NPC ����� �ҷ��´�.

#include "QuestScriptLoader.h"																// ����Ʈ ��ũ���� �δ� ����� �ҷ��´�.
#include "QuestNpcData.h"																	// ����Ʈ NPC ������ ����� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpc::CQuestNpc( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )		// ������ �Լ�.
{
	m_dwQuestIdx = dwQuestIdx;																// ����Ʈ �ε����� �����Ѵ�.
	m_dwSubQuestIdx = dwSubQuestIdx;														// ���� ����Ʈ �ε����� �����Ѵ�.
	ReadNpcData( pTokens, dwQuestIdx, dwSubQuestIdx );										// NPC �����͸� �о���δ�.
}

CQuestNpc::~CQuestNpc()																		// �Ҹ��� �Լ�.
{
	Release();																				// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestNpc::Release()																	// ���� �Լ�.
{
	CQuestNpcData* pNpcData = NULL;															// ����Ʈ NPC ������ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	PTRLISTPOS pos = m_NpcDataList.GetHeadPosition();										// NPC ������ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																			// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcData = (CQuestNpcData*)m_NpcDataList.GetNext( pos );							// ��ġ�� �ش��ϴ� npc �����͸� �޴´�.

		if( pNpcData )																		// npc ������ ������ ��ȿ�ϸ�,
		{
			delete pNpcData;																// npc ������ ������ �����Ѵ�.
		}
	}
}

BOOL CQuestNpc::ReadNpcData( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// npc ������ ������ �д� �Լ�.
{
	char* OneToken;																				// ��ū �����͸� �����Ѵ�.

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )									// ��ū ������ ��ȿ�ѵ��� while���� ������.
	{
		if( OneToken[0] == '@' )																// ��ū�� @�� ���ٸ�,
		{
			m_NpcDataList.AddTail( CQuestScriptLoader::LoadQuestNpcData( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );	// npc ������ ����Ʈ�� npc�����ͷ� �ε���
		}																													// �߰��Ѵ�.
	}
	return TRUE;																				// TRUE�� �����Ѵ�.
}