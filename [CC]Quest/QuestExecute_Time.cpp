// QuestExecute_Time.cpp: implementation of the QuestExecute_Time class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestExecute_Time.h"																// ����Ʈ ���� �ð� ����� �ҷ��´�.

#include "QuestScriptLoader.h"																// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_																			// �� ������ ����Ǿ� �ִٸ�,

#include "QuestGroup.h"																		// ����Ʈ �׷� ����� �ҷ��´�.

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������.
CQuestExecute_Time::CQuestExecute_Time( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_bType, pTokens );														// Ÿ���� �޴´�.
	GetScriptParam( m_dwDay, pTokens );														// ��¥�� �޴´�.
	GetScriptParam( m_dwHour, pTokens );													// �ð��� �޴´�.
	GetScriptParam( m_dwMin, pTokens );														// ���� �޴´�.
}

CQuestExecute_Time::~CQuestExecute_Time()													// �Ҹ��� �Լ�.
{
}

// ���� �Լ�.
BOOL CQuestExecute_Time::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_																			// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )																// ���� ������ Ȯ���Ѵ�.
	{
	case eQuestExecute_RegistTime:															// �ð� ��� �����̶��,
		{
			// �ð� üũ�� ����ϴ� �Լ��� ȣ���Ѵ�.
			pQuestGroup->RegistCheckTime( m_dwQuestIdx, m_dwSubQuestIdx, m_bType, m_dwDay, m_dwHour, m_dwMin );
		}
		break;
	}

#endif	

	return TRUE;																			// TRUE�� �����Ѵ�.
}

int CQuestExecute_Time::CheckCondition( PLAYERTYPE* pPlayer, 
							CQuestGroup* pQuestGroup, CQuest* pQuest )			// ����Ʈ �̺�Ʈ�� �����ϱ� ���� ������ �����ϴ��� üũ�ϴ� �Լ�.
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

#ifdef _MAPSERVER_																// �� ������ ����Ǿ� �ִٸ�,

#endif //_MAPSERVER_

	return nErrorCode ;															// �⺻ ���� �����Ѵ�.
}
