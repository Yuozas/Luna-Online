// QuestExecute_Quest.cpp: implementation of the CQuestExecute_Quest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestExecute_Quest.h"															// ����Ʈ ���� ����Ʈ ����� �ҷ��´�.

#include "QuestScriptLoader.h"															// ����Ʈ ��Ʈ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_																		// �� ������ ����Ǿ� ���� ������,

#include "QuestGroup.h"																	// ����Ʈ �׷� ����� �ҷ��´�.
#include "QuestManager.h"																// ����Ʈ �Ŵ��� ����� �ҷ��´�.
#include "QuestInfo.h"																	// ����Ʈ ���� ����� �ҷ��´�.

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestExecute_Quest::CQuestExecute_Quest( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwExeQuestIdx = m_dwExeSubQuestIdx = 0;											// ���� ����Ʈ �ε���, ���� ���� ����Ʈ �ε����� 0���� �����Ѵ�.

	if( m_dwExecuteKind == eQuestExecute_StartSub )										// ���� ������ ���� ����Ʈ ����� ���ٸ�,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );									// ���� ���� ����Ʈ �ε����� �޴´�.
	}
	else if( m_dwExecuteKind == eQuestExecute_EndQuest )								// ���� ������ ����Ʈ ������,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
	}
	else if( m_dwExecuteKind == eQuestExecute_EndOtherSub )								// ���� ������ �ٸ� ���� ����Ʈ ������,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );									// ���� ���� ����Ʈ �ε����� �޴´�.
	}
	else if( m_dwExecuteKind == eQuestExecute_RegenMonster )							// ���� ������ ���� ������ ���ٸ�,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
	}
	// 080104 LYW --- QuestExecute_Quest : �� �̵� ����� ó���� ������.
	else if( m_dwExecuteKind == eQuestExecute_MapChange )
	{
		GetScriptParam( m_DestinationMapNum, pTokens ) ;								// ���������� �� ��ȣ�� �޴´�.
		GetScriptParam( m_fDestination_XPos, pTokens ) ;								// ���������� X��ǥ�� �޴´�.
		GetScriptParam( m_fDestination_ZPos, pTokens ) ;								// ���������� Z��ǥ�� �޴´�.
	}
	else if( m_dwExecuteKind == eQuestExecute_ChangeStage )								// ���� ������ �������� ü���� ���,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
		GetScriptParam( m_dwExeSubQuestIdx, pTokens ) ;									// ���� ���� ����Ʈ �ε����� �޴´�.
	}
	else if( m_dwExecuteKind == eQuestExecute_ChangeSubAttr )							// ���� ������ ���� ����Ʈ Attr �����̶��,
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );										// ���� ����Ʈ �ε����� �޴´�.
	}
}

CQuestExecute_Quest::~CQuestExecute_Quest()												// �Ҹ��� �Լ�.
{
}

// ���� �Լ�.
BOOL CQuestExecute_Quest::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_																		// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )															// ���� ������ Ȯ���Ѵ�.
	{
	case eQuestExecute_EndQuest:														// ����Ʈ�� �����ϴ� �����̶��,
		{			
			pQuestGroup->EndQuest( m_dwQuestIdx, m_dwExeQuestIdx );						// ����Ʈ ���� �Լ��� ȣ���Ѵ�.
		}
		break;		

	case eQuestExecute_StartSub:														// ���� ����Ʈ�� �����ϴ� �����̶��,
		{
			pQuestGroup->StartSubQuest( m_dwExeQuestIdx, m_dwExeSubQuestIdx );			// ���� ����Ʈ�� �����Ѵ�.
		}
		break;

	case eQuestExecute_EndSub:															// ���� ����Ʈ�� �����ϴ� �����̶��,
		{
			pQuestGroup->EndSubQuest( m_dwQuestIdx, m_dwSubQuestIdx );					// ���� ����Ʈ�� �����ϴ� �Լ��� ȣ���Ѵ�.
		}
		break;

	case eQuestExecute_EndOtherSub:														// �ٸ� ���� ����Ʈ�� �����ϴ� �����̶��,
		{
			pQuestGroup->EndSubQuest( m_dwExeQuestIdx, m_dwExeSubQuestIdx );			// �ٸ� ���� ����Ʈ�� �����ϴ� �Լ��� ȣ���Ѵ�.
		}
		break;

	case eQuestExecute_RegenMonster:													// ���͸� ���� �ϴ� ��������,
		{
			pQuestGroup->RegenMonster( m_dwExeQuestIdx );								// ���͸� ���� �ϴ� �Լ��� ȣ���Ѵ�.
		}
		break;

	case eQuestExecute_MapChange:														// �� ü������ �ϴ� �����̸�,
		{
			// 080104 LYW --- QuestExecute_Quest : �� ������ �ϴ� ����� ������.
			pQuestGroup->MapChange( m_DestinationMapNum, m_fDestination_XPos, m_fDestination_ZPos ) ;
			//pQuestGroup->MapChange( (WORD)m_dwExeQuestIdx, (WORD)m_dwExeSubQuestIdx );	// �� ü������ �ϴ� �Լ��� ȣ���Ѵ�.
		}
		break;

	case eQuestExecute_ChangeStage:														// ���������� �ٲٴ� �����̶��,
		{
			pQuestGroup->ChangeStage( m_dwExeQuestIdx, m_dwExeSubQuestIdx );			// ���������� �ٲٴ� �Լ��� ȣ���Ѵ�.
		}
		break;
	}
#endif	// _MAPSERVER_
	return TRUE;
}

int CQuestExecute_Quest::CheckCondition( PLAYERTYPE* pPlayer,							// ����Ʈ �̺�Ʈ�� �����ϱ� ���� ������ �����ϴ��� üũ�ϴ� �Լ�.
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

	//switch( m_dwExecuteKind )															// ���� ������ Ȯ���Ѵ�.
	//{
	//case eQuestExecute_EndQuest:														// ����Ʈ�� �����ϴ� �����̶��,
	//	{			
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;		

	//case eQuestExecute_StartSub:														// ���� ����Ʈ�� �����ϴ� �����̶��,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;

	//case eQuestExecute_EndSub:															// ���� ����Ʈ�� �����ϴ� �����̶��,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;

	//case eQuestExecute_EndOtherSub:														// �ٸ� ���� ����Ʈ�� �����ϴ� �����̶��,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;

	//case eQuestExecute_RegenMonster:													// ���͸� ���� �ϴ� ��������,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;

	//case eQuestExecute_MapChange:														// �� ü������ �ϴ� �����̸�,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;

	//case eQuestExecute_ChangeStage:														// ���������� �ٲٴ� �����̶��,
	//	{
	//		return nErrorCode ;															// ���� �ڵ带 return ó���� �Ѵ�.
	//	}
	//	break;
	//}

#endif //_MAPSERVER_

	return nErrorCode ;																	// �⺻ ���� �����Ѵ�.
}
