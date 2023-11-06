// QuestLimit.cpp: implementation of the CQuestLimit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestLimit.h"																		// ����Ʈ ���� ��������� �ҷ��´�.

#include "QuestScriptLoader.h"																// ��ũ��Ʈ �δ� ����� �ҷ��´�.
#include "QuestLimitKind.h"																	// ����Ʈ ���� ���� ����� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestLimit::CQuestLimit( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// ������ �Լ�.
{
	ReadLimitKind( pTokens,  dwQuestIdx, dwSubQuestIdx );									// ����Ʈ ���� ������ �о���δ�.
}

CQuestLimit::~CQuestLimit()																	// �Ҹ��� �Լ�.
{
	Release();																				// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestLimit::Release()																	// ���� �Լ�.
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();								// ����Ʈ ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimitKind* pQuestLimitKind = NULL;												// ����Ʈ ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																			// ��ġ ������ ��ȿ�� ���� while�� ������.
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );			// ����Ʈ ����Ʈ ���� ������ �޴´�.

		if( pQuestLimitKind )																// ����Ʈ ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			delete pQuestLimitKind;															// ����Ʈ ����Ʈ ���� ������ �����Ѵ�.
		}
	}
	m_QuestLimitKindList.RemoveAll();														// ����Ʈ ����Ʈ ���� ������ ��� ����Ʈ�� ��� ����.
}

BOOL CQuestLimit::ReadLimitKind( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// ����Ʈ ������ �о���̴� �Լ�.
{
	char* OneToken;																					// ��ū �����͸� �����Ѵ�.

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )										// ��ū �����Ͱ� NULL�� �ƴѰ�� while���� ������.
	{
		if( OneToken[0] == '&' )																	// ��ū�� &�� ������,
		{
			// ����Ʈ ����Ʈ ���� ������ ��� ����Ʈ�� �߰��Ѵ�.
			m_QuestLimitKindList.AddTail( CQuestScriptLoader::LoadQuestLimitKind( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
		}
	}
	return TRUE;																					// TRUE�� �����Ѵ�.
}

BOOL CQuestLimit::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )		// ����Ʈ ����Ʈ�� üũ�Ѵ�.(����)
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();										// ����Ʈ ����Ʈ ���� ������ ��� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimitKind* pQuestLimitKind = NULL;														// ����Ʈ ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	while( pos )																					// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );					// ����Ʈ ����Ʈ ���� ������ �޴´�.

		if( pQuestLimitKind )																		// ����Ʈ ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pQuestLimitKind->CheckLimit( pPlayer, pQuestGroup, pQuest ) )						// ����Ʈ ����Ʈ�� �ɸ����� üũ�Ѵ�.
			{
				return FALSE;																		// FALSE�� �����Ѵ�.
			}
		}
	}
	return TRUE;																					// TRUE�� �����Ѵ�.
}

BOOL CQuestLimit::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )								// ����Ʈ ����Ʈ�� üũ�Ѵ�. (Ŭ���̾�Ʈ)
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();										// ����Ʈ ����Ʈ ���� ������ ��� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	CQuestLimitKind* pQuestLimitKind = NULL;														// ����Ʈ ����Ʈ ������ ���� �����͸� �����Ѵ�.

	while( pos )																					// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );					// ����Ʈ ����Ʈ ���� ������ �޴´�.

		if( pQuestLimitKind )																		// ����Ʈ ����Ʈ ���� ������ ��ȿ�ϸ�,
		{
			if( !pQuestLimitKind->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// ����Ʈ ����Ʈ�� �ɸ����� üũ�Ѵ�.
			{
				return FALSE;																		// FALSE�� �����Ѵ�.
			}
		}
	}
	return TRUE;																					// TRUE�� �����Ѵ�.
}
