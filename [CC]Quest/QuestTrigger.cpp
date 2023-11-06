// QuestTrigger.cpp: implementation of the CQuestTrigger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"														// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestTrigger.h"												// ����Ʈ Ʈ���� ����� �ҷ��´�.

#include "QuestScriptLoader.h"											// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.
#include "QuestCondition.h"												// ����Ʈ ���� ����� �ҷ��´�.

#include "QuestExecute.h"												// ����Ʈ ���� ����� �ҷ��´�.
#include "QuestEvent.h"													// ����Ʈ �̺�Ʈ ����� �ҷ��´�.

#ifdef _MAPSERVER_														// �� ������ ����Ǿ� �ִٸ�,

#include "Player.h"														// �÷��̾� ����� �����Ѵ�.

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ������ �Լ�.
CQuestTrigger::CQuestTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	ReadTrigger( pTokens, dwQuestIdx, dwSubQuestIdx );					// Ʈ���Ÿ� �о���δ�.
}

CQuestTrigger::~CQuestTrigger()											// �Ҹ��� �Լ�.
{
	Release();															// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestTrigger::Release()											// ���� �Լ�.
{
	if( m_pQuestCondition )												// ����Ʈ ���� ������ ��ȿ�ϸ�,
	{
		delete m_pQuestCondition;										// ����Ʈ ���� ������ �����Ѵ�.
		m_pQuestCondition = NULL;										// ����Ʈ ���� ������ ��Ÿ���� �����͸� null ó���� �Ѵ�.
	}

	CQuestExecute* pQuestExecute = NULL;								// ����Ʈ ���� �����͸� NULL ó���� �Ѵ�.

	PTRLISTPOS pos = m_QuestExeList.GetHeadPosition();					// ����Ʈ ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pos )														// ��ġ ������ ��ȿ�ѵ��� while���� ������.
	{
		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );	// ����Ʈ ���� ������ �޴´�.

		if( pQuestExecute )												// ������ ��ȿ�ϴٸ�,
		{
			delete pQuestExecute;										// ����Ʈ ���� ������ �����Ѵ�.
		}
	}
	m_QuestExeList.RemoveAll();											// ����Ʈ ���� ����Ʈ�� ��� ����.
}

#include "QuestExecute_Quest.h"											// ����Ʈ ���� ����Ʈ ����� �ҷ��´�.

// Ʈ���Ÿ� �о���̴� �Լ�.
BOOL CQuestTrigger::ReadTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	char* OneToken;														// ��ū �����͸� �����Ѵ�.

	m_dwEndParam = 0;													// ���� �Ķ���͸� 0���� �����Ѵ�.

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )			// ��ū ������ ��ȿ�� ���� while���� ������.
	{
		if( OneToken[0] == '@' )										// ��ū�� @�� ���ٸ�,
		{
			m_pQuestCondition = CQuestScriptLoader::LoadQuestCondition(	// ����Ʈ ���� ������ �ε��Ͽ�, 
				OneToken, pTokens, dwQuestIdx, dwSubQuestIdx );			// ������ ������ �޴´�.
		}
		else if( OneToken[0] == '*' )									// ��ū�� *�� ���ٸ�,
		{
			// �ݺ�����Ʈ�� Ȯ���� ���ؼ� �߰� RaMa - 04.10.26
			CQuestExecute* pQuestExecute = NULL ;						// ����Ʈ ���� ������ ���� ������ ������ �����Ѵ�.

			pQuestExecute = CQuestScriptLoader::LoadQuestExecute(		// ����Ʈ ���� ������ �ε��Ͽ�,
				OneToken, pTokens, dwQuestIdx, dwSubQuestIdx );			// ����Ʈ ���� ������ ���� ������ ������ �޴´�.

			// ����Ʈ ������ ����Ʈ ����� ������,
			if(pQuestExecute && pQuestExecute->GetQuestExecuteKind() == eQuestExecute_EndQuest)
			{
				// ����Ʈ ���� ������ �޴´�.
				CQuestExecute_Quest* pQExe_Quest = (CQuestExecute_Quest*)pQuestExecute;

				if( pQExe_Quest )										// ����Ʈ ���� ������ ��ȿ�ϸ�,
				{
					m_dwEndParam = pQExe_Quest->GetQuestExeIdx();		// ����Ʈ ���� �ε����� �޴´�.
				}
			}
			m_QuestExeList.AddTail( pQuestExecute );					// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
		}
	}
	return TRUE;														// TRUE�� �����Ѵ�.
}

//����Ʈ �̺�Ʈ�� ó���ϴ� �Լ�.
BOOL CQuestTrigger::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup,	// ����Ʈ �̺�Ʈ�� ó���ϴ� �Լ�.
								  CQuest* pQuest, CQuestEvent* pQuestEvent )
{	
	if( !m_pQuestCondition->CheckCondition( pQuestEvent ) )							// �̺�Ʈ ������ ���� ����Ʈ ���� ������ üũ�Ѵ�.
	{
		return FALSE;																// FALSE�� �����Ѵ�.
	}

	int nErrorCode = -1 ;															// ���� �ڵ带 ���� ������ �����ϰ� -1�� �����Ѵ�.

	CQuestExecute* pQuestExecute ;													// ����Ʈ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = m_QuestExeList.GetHeadPosition();								// ����Ʈ ���� ����Ʈ�� ���� �����ϰ�, ��ġ ������ �޴´�.

	while( pos )																	// ����Ʈ ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pQuestExecute = NULL ;														// ����Ʈ ���� ������ ���� �����͸� null ó���� �Ѵ�.
		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );				// ����Ʈ ���� ������ �޴´�.

		if( !pQuestExecute ) continue ;												// ����Ʈ ���� ������ ��ȿ���� ������, continue ó���� �Ѵ�.

		nErrorCode = pQuestExecute->CheckCondition( pPlayer, pQuestGroup, pQuest ) ;// ����Ʈ�� ���� �� ������ ���߰� �ִ��� üũ�Ͽ� ����� �޴´�.

		if( nErrorCode != -1 ) break ;												// ���� �ڵ尡 -1�� �ƴϸ� while���� Ż���Ѵ�.
	}

#ifdef _MAPSERVER_																	// �� ������ ����Ǿ� �ִٸ�,

	if( nErrorCode != -1 )															// ���� ����� FALSE�� ���ٸ�,
	{
		MSG_INT Msg ;																// �޽��� ����ü�� �����Ѵ�.
		Msg.Category = MP_QUEST;													// ī�װ��� ����Ʈ�� �����Ѵ�.
		Msg.Protocol = MP_QUEST_EXECUTE_ERROR;										// ���������� ����Ʈ ���� ������ �����Ѵ�.
		Msg.dwObjectID = pPlayer->GetID();											// �÷��̾� ���̵� �����Ѵ�.

		Msg.nData = nErrorCode ;													// ����Ʈ �̺�Ʈ ó�� ���� �ڵ带 �����Ѵ�.

		pPlayer->SendMsg( &Msg, sizeof(Msg) );										// �÷��̾�� ���� �޽����� �����Ѵ�.

		return FALSE ;																// false return ó���� �Ѵ�.
	}

#endif

	pos = m_QuestExeList.GetHeadPosition();											// ����Ʈ ���� ����Ʈ�� ���� �����ϰ�, ��ġ ������ �޴´�.

	while( pos )																	// ����Ʈ ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pQuestExecute = NULL ;														// ����Ʈ ���� ������ ���� �����͸� null ó���� �Ѵ�.
		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );				// ����Ʈ ���� ������ �޴´�.

		if( !pQuestExecute ) continue ;												// ����Ʈ ���� ������ ��ȿ���� ������, continue ó���� �Ѵ�.

		if( pQuestExecute->Execute( pPlayer, pQuestGroup, pQuest ) == FALSE )		// ����Ʈ �̺�Ʈ�� �����Ѵ�.
			return FALSE;
	}

	return TRUE;
}

//// ����Ʈ �̺�Ʈ�� ó���ϴ� �Լ�.
//BOOL CQuestTrigger::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent )
//{
//	// condition
//	if( !m_pQuestCondition->CheckCondition( pQuestEvent ) )				// �̺�Ʈ ������ ���� ����Ʈ ���� ������ üũ�Ѵ�.
//	{
//		return FALSE;													// FALSE�� �����Ѵ�.
//	}
//
//	// execute
//	BOOL bError = TRUE;													// ���� ���θ� ��� ������ TRUE�� �����Ѵ�.
//
//	PTRLISTPOS pos = m_QuestExeList.GetHeadPosition();					// ����Ʈ ���� ����Ʈ�� ���� �����ϰ�, ��ġ ������ �޴´�.
//
//	CQuestExecute* pQuestExecute = NULL;								// ����Ʈ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
//
//	while( pos )														// ����Ʈ ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );	// ����Ʈ ���� ������ �޴´�.
//
//		if( pQuestExecute )												// ����Ʈ ���� ������ ��ȿ�ϸ�,
//		{
//			bError = pQuestExecute->Execute( pPlayer, pQuestGroup, pQuest );	// ����Ʈ�� �����Ͽ� ����� �޴´�.
//
//			if( bError == FALSE )	
//			{
//				break;								// ����� FALSE�� ������, while���� Ż���Ѵ�.
//			}
//		}
//	}
//
//#ifdef _MAPSERVER_														// �� ������ ����Ǿ� �ִٸ�,
//
//	if( bError == FALSE )												// ���� ����� FALSE�� ���ٸ�,
//	{
//		// network send
//		MSGBASE Msg;													// �޽��� ����ü�� �����Ѵ�.
//		Msg.Category = MP_QUEST;										// ī�װ��� ����Ʈ�� �����Ѵ�.
//		Msg.Protocol = MP_QUEST_EXECUTE_ERROR;							// ���������� ����Ʈ ���� ������ �����Ѵ�.
//		Msg.dwObjectID = pPlayer->GetID();								// �÷��̾� ���̵� �����Ѵ�.
//		pPlayer->SendMsg( &Msg, sizeof(Msg) );							// �÷��̾�� ���� �޽����� �����Ѵ�.
//	}
//
//#endif
//
//	return TRUE;
//}
