// QuestNpcScript.cpp: implementation of the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestNpcScript.h"

#include "QuestScriptLoader.h"
#include "QuestNpcScriptData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScript::CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// ������ �Լ�.
{
	m_dwQuestIdx = dwQuestIdx;														// ����Ʈ �ε����� �����Ѵ�.
	m_dwSubQuestIdx = dwSubQuestIdx;												// ���� ����Ʈ �ε����� �����Ѵ�.
	ReadNpcScript( pTokens, dwQuestIdx, dwSubQuestIdx );							// NPC ��ũ��Ʈ�� �о���δ�.
}

CQuestNpcScript::~CQuestNpcScript()													// �Ҹ��� �Լ�.
{
	Release();																		// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestNpcScript::Release()														// ���� �Լ�.
{
	CQuestNpcScriptData* pNpcScriptData = NULL;										// NPC ��ũ��Ʈ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// NPC ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																	// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// npc ��ũ��Ʈ ������ ������ �޴´�.

		if( pNpcScriptData )														// npc ��ũ��Ʈ ������ ������ ��ȿ�ϴٸ�,
		{
			delete pNpcScriptData;													// npc ��ũ��Ʈ ������ ������ �����Ѵ�.
		}
	}
}

BOOL CQuestNpcScript::ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// npc ��ũ��Ʈ�� �о���̴� �Լ�.
{
	char* OneToken;																	// ��ū �����͸� �����Ѵ�.

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )						// ��ū ������ ��ȿ�ѵ��� while���� ������.
	{
		if( OneToken[0] == '@' )													// ��ū�� @�� ���ٸ�,
		{
			// npc ��ũ��Ʈ ����Ʈ�� npc ��ũ��Ʈ �����͸� �ε��ؼ� �߰��Ѵ�.
			m_NpcScriptList.AddTail( CQuestScriptLoader::LoadQuestNpcScriptData( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
		}
	}
	return TRUE;																	// TRUE�� �����Ѵ�.
}

DWORD CQuestNpcScript::IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )			// npc ��ũ��Ʈ �������� �����ϴ� �Լ�.
{
	*pQuestIdx = m_dwQuestIdx;														// ����Ʈ �ε����� �����Ѵ�.

	*pSubQuestIdx = m_dwSubQuestIdx;												// ���� ����Ʈ �ε����� �����Ѵ�.

	DWORD dwPage = 0;																// �������� ���� ������ �����ϰ� 0���� �����Ѵ�.

	CQuestNpcScriptData* pNpcScriptData = NULL;										// NPC ��ũ��Ʈ ������ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// NPC ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )																	// ��ġ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// ��ġ ������ �ش��ϴ� npc ��ũ��Ʈ ������ �޴´�.

		if( pNpcScriptData )														// npc ��ũ��Ʈ ������ ��ȿ�ϴٸ�,
		{
			if( dwPage = pNpcScriptData->IsSameNpc( dwNpcIdx ) )					// �������� �޴´�.
			{
				return dwPage;														// �������� �����Ѵ�.
			}
		}
	}
	return dwPage;																	// �������� �����Ѵ�.
}

DWORD CQuestNpcScript::IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )	// npc ��ũ�� �����ϴ� �Լ�.
{
	*pQuestIdx = m_dwQuestIdx;														// ����Ʈ �ε����� �����Ѵ�.

	*pSubQuestIdx = m_dwSubQuestIdx;												// ���� ����Ʈ�� �����Ѵ�.

	DWORD dwNpcMark = 0;															// npc ��ũ�� ���� ������ �����ϰ� 0���� �����Ѵ�.

	CQuestNpcScriptData* pNpcScriptData = NULL;										// npc ��ũ��Ʈ �����͸� ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// npc ��ũ��Ʈ ����Ʈ�� ���� �����ϰ� ��ġ������ �޴´�.

	while( pos )																	// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// npc ��ũ��Ʈ ������ ������ �޴´�.

		if( pNpcScriptData )														// npc ��ũ��Ʈ ������ ������ ��ȿ�ϴٸ�,
		{
			if( dwNpcMark = pNpcScriptData->IsSameNpcMark( dwNpcIdx ) )				// npc ��ũ�� �޴´�.
			{
				return dwNpcMark;													// npc ��ũ�� �����Ѵ�.
			}
		}
	}
	return dwNpcMark;																// npc ��ũ�� �����Ѵ�.
}
