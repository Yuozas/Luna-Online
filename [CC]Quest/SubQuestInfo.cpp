
// SubQuestInfo.cpp: implementation of the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "SubQuestInfo.h"																	// 서브 퀘스트 정보 해더를 불러온다.

#include "QuestLimit.h"																		// 퀘스트 제한 헤더를 불러온다.
#include "QuestTrigger.h"																	// 퀘스트 트리거 헤더를 불러온다.
#include "QuestNpcScript.h"																	// 퀘스트 npc 스크립트 헤더를 불러온다.
#include "QuestNpc.h"																		// 퀘스트 npc 헤더를 불러온다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubQuestInfo::CSubQuestInfo()																// 생성자 함수.
{
	m_dwMaxCount = 0;																		// 최대 카운트를 0으로 세팅한다.
}

CSubQuestInfo::~CSubQuestInfo()																// 소멸자 함수.
{
	Release();																				// 해제 함수를 호출한다.
}

void CSubQuestInfo::Release()																// 해제 함수.
{
	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터 변수를 선언하고 null 처리를 한다.

	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// 퀘스트 제한 리스트를 헤드로 세팅하고 위치를 받는다.

	while( pos )																			// 위치 정보가 유효한 동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 위치 정보에 해당하는 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효한지 체크한다.
		{
			delete pLimit;																	// 퀘스트 제한 정보를 삭제한다.
		}
	}

	m_QuestLimitList.RemoveAll();															// 퀘스트 제한 리스트를 모두 비운다.

	CQuestTrigger* pTrigger = NULL;															// 크리거 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	pos = m_QuestTriggerList.GetHeadPosition();												// 퀘스트 트리가 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한 동안 while 문을 돌린다.
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );						// 위치에 해당하는 트리거 정보를 받는다.

		if( pTrigger )																		// 트리거 정보가 유효한지 체크한다.
		{
			delete pTrigger;																// 트리거 정보를 삭제한다.
		}
	}

	m_QuestTriggerList.RemoveAll();															// 트리거 리스트를 모두 비운다.

	CQuestNpcScript* pNpcScript = NULL;														// 퀘스트 npc 스크립트 정보를 받을 포인터를 선언하고 null 처리를 한다.

	pos = m_QuestNpcScriptList.GetHeadPosition();											// 퀘스트 npc 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한 동안 while문을 돌린다.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// 위치 정보에 해당하는 npc 스크립트 정보를 받는다.

		if( pNpcScript )																	// npc 스크립트 정보가 유효한지 체크한다.
		{
			delete pNpcScript;																// npc 스크립트 정보를 삭제한다.
		}
	}

	m_QuestNpcScriptList.RemoveAll();														// npc 스크립트 리스트를 모두 비운다.
	
	CQuestNpc* pQuestNpc = NULL;															// 퀘스트 npc 정보를 받을 포인터를 선언하고 null 처리를 한다.

	pos = m_QuestNpcList.GetHeadPosition();													// 퀘스트 npc 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pQuestNpc = (CQuestNpc*)m_QuestNpcList.GetNext( pos );								// 퀘스트 npc 정보를 받는다.

		if( pQuestNpc )																		// 퀘스트 npc 정보가 유효한지 체크한다.
		{
			delete pQuestNpc;																// 퀘스트 npc 정보를 삭제한다.
		}
	}

	m_QuestNpcList.RemoveAll();																// 퀘스트 npc 리스트를 모두 비운다.
}

void CSubQuestInfo::AddQuestLimit( CQuestLimit* pQuestLimit )								// 퀘스트 제한 정보를 추가하는 함수.
{
	if( !pQuestLimit ) return;																// 퀘스트 제한 정보가 유효하지 않으면 리턴 처리를 한다.

	m_QuestLimitList.AddTail( pQuestLimit );												// 퀘스트 제한 리스트에 추가한다.
}

void CSubQuestInfo::AddQuestTrigger( CQuestTrigger* pQuestTrigger )							// 퀘스트 트리거를 추가하는 함수.
{
	if( !pQuestTrigger ) return;															// 퀘스트 트리거 정보가 유효하지 않으면 리턴 처리를 한다.

	m_QuestTriggerList.AddTail( pQuestTrigger );											// 퀘스트 트리거 리스트에 추가한다.
}

void CSubQuestInfo::AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript )					// 퀘스트 npc 스크립트를 추가하는 함수.
{
	if( !pQuestNpcScript ) return;															// 퀘스트 npc 스크립트 정보가 유효하지 않으면 리턴 처리를 한다.

	m_QuestNpcScriptList.AddTail(  pQuestNpcScript );										// 퀘스트 npc 스크립트 리스트에 추가한다.
}

void CSubQuestInfo::AddQuestNpc( CQuestNpc* pQuestNpc )										// 퀘스트 npc 를 추가하는 함수.
{
	if( !pQuestNpc ) return;																// 퀘스트 npc 정보가 유효 하지 않으면 리턴 처리를 한다.

	m_QuestNpcList.AddTail(  pQuestNpc );													// 퀘스트 npc 리스트에 추가한다.
}

void CSubQuestInfo::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent )	// 퀘스트 이벤트를 처리하는 함수.
{
	// limit
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	while( pos )																			// 위치 정보가 유효한 동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효하다면,
		{
			if( !pLimit->CheckLimit( pPlayer, pQuestGroup, pQuest ) )						// 퀘스트 제한에 걸리는지 체크해한다.
			{
				return;																		// 리턴 처리를 한다.
			}
		}
	}
	
	// trigger
	pos = m_QuestTriggerList.GetHeadPosition();												// 트리거 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestTrigger* pTrigger = NULL;															// 트리거 정보를 받을 포인터를 선언하고 null 처리를 한다.

	while( pos )																			// 위치 정보가 있는동안 while문을 돌린다.
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );						// 위치에 해당하는 트리거 정보를 받는다.

		if( pTrigger )																		// 트리거 정보가 유효한지 체크한다.
		{
			pTrigger->OnQuestEvent( pPlayer, pQuestGroup, pQuest, pQuestEvent );			// 트리거의 퀘스트 이벤트를 실행한다.
		}
	}
}

BOOL CSubQuestInfo::IsNpcRelationQuest( DWORD dwNpcIdx )									// npc 관련 퀘스트인지 체크하는 함수.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// 퀘스트 인덱스와 서브 퀘스트 인덱스를 담을 변수를 선언한다.

	dwQuestIdx = dwSubQuestIdx = 0;															// 퀘스트 인덱스와 서브 퀘스트 인덱스를 0으로 세팅한다.

	DWORD rt = 0;																			// 결과를 담을 변수를 선언하고 0으로 세팅한다.

	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// npc 스크립트 정볼를 받을 포인터를 선언하고 null 처리를 한다.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// npc 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유호한 동안 while문을 돌린다.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// 위치에 해당하는 스크립트 정보를 받는다.

		if( pNpcScript )																	// 스크립트 정보가 유효하다면,
		{
			if( rt = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )		// npc와 같은 이름인지 체크하여 결과를 받는다.
			{
				break;																		// 같은 이름이면 while 문을 빠져 나온다.
			}
		}
	}

	if( rt == 0 )																			// 결과가 0과 같으면,
	{
		return FALSE;																		// FALSE 를 리턴한다.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	while( pos )																			// 위치 정보가 유효한 동안 while문을 돌린다.
	{	
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 위치에 해당하는 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효하면,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// 퀘스트 제한에 걸리는지 체크한다.
			{
				return FALSE;																// FALSE 리턴 처리를 한다.
			}
		}
	}

	return TRUE;																			// TRUE 리턴 처리를 한다.
}

DWORD CSubQuestInfo::GetNpcScriptPage( DWORD dwNpcIdx )										// NPC 스크립트 페이지를 리턴하는 함수.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// 퀘스트 인덱스와 서브 퀘스트 인덱스를 담을 변수를 선언한다.

	dwQuestIdx = dwSubQuestIdx = 0;															// 퀘스트와 서브 퀘스트 인덱스를 0으로 세팅한다.

	DWORD dwPage = 0;																		// 페이지를 받을 변수를 선언하고 0으로 세팅한다.
	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// NPC 스크립트 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// NPC 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// npc 스크립트 정보를 받는다.

		if( pNpcScript )																	// 스크립트 정보가 유효하다면,
		{
			if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// 같은 npc 인지 체크하여 페이지를 받는다.
			{
				break;																		// while문을 빠져나온다.
			}
		}
	}

	if( dwPage == 0 )																		// 페이지가 0과 같다면,
	{
		return dwPage;																		// 페이지를 리턴한다.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 null 처리를 한다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 위치에 해당하는 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효하면,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// 퀘스트 제한에 걸리는지 체크한다.
			{
				return 0;	// limit														// 0을 리턴한다.
			}
		}
	}

	return dwPage;																			// 페이지를 리턴한다.
}

DWORD CSubQuestInfo::GetNpcMarkType( DWORD dwNpcIdx )										// npc 마크 타입을 반환하는 함수.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// 퀘스트 인덱스와 서브 퀘스트 인덱스를 담을 변수를 선언한다.

	dwQuestIdx = dwSubQuestIdx = 0;															// 퀘스트 인덱스와 서브퀘스트 인덱스르르 0으로 세팅한다.

	DWORD dwNpcMark = 0;																	// npc 마크를 받을 변수를 선언하고 0으로 세팅한다.

	// npcidx is same
	CQuestNpcScript* pNpcScript = NULL;														// npc 스크립트 정보를 받을 포인터를 선언하고 null 처리를 한다.

	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// npc 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					// 위치에 해당하는 npc 스크립트 정보를 받는다.

		if( pNpcScript )																	// 스크립트 정보가 유효하면,
		{	
			if( dwNpcMark = pNpcScript->IsSameNpcMark( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// 같은 이름의 npc라면 npc 마크를 받는다.
			{
				break;																		// while문을 빠져나온다.
			}
		}
	}

	if( dwNpcMark == 0 )																	// npc 마크가 0과 같다면,
	{
		return dwNpcMark;																	// npc 마크를 리턴한다.
	}

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();												// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효하면,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// 퀘스트 제한에 걸리는지 체크한다.
			{
				return 0;	// limit														// 0을 리턴한다.
			}
		}
	}

	return dwNpcMark;																		// npc 마크를 리턴한다.
}
	
int CSubQuestInfo::IsQuestState( DWORD dwNpcIdx )											// 퀘스트 상태를 리턴하는 함수.
{
	DWORD dwQuestIdx, dwSubQuestIdx;														// 퀘스트 인덱스와 서브 퀘스트 인덱스를 담을 변수를 선언한다.

	dwQuestIdx = dwSubQuestIdx = 0;															// 퀘스트 인덱스와 서브 퀘슽 인덱스를 0으로 세팅한다.

	DWORD dwPage = 0;																		// 페이지를 담을 변수를 선언하고 0으로 세팅한다.

	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;														// npc 스크립트 정보를 받을 포인터를 선언하고 null 처리를 한다.
	
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();								// 퀘스트 npc 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );					//  npc 스크립트 정보를 받는다.

		if( pNpcScript )																	// npc 스크립트 정보가 유효하다면,
		{
			if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )	// 페이지를 받는다.
			{
				break;																		// while문을 빠져나온다.
			}
		}
	}

	if( dwPage == 0 )																		// 페이지가 0과 같다면,
	{
		return 0;																			// 0을 리턴한다.
	}
	
	// if check limit
	pos = m_QuestLimitList.GetHeadPosition();												// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 null 처리를 한다.

	while( pos )																			// 위치 정보가 유효한 동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 위치 정보에 해당하는 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한 정보가 유효하면,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )							// 퀘스트 제한에 걸리는지 체크한다.
			{
				return 0;	// limit														// 0을 리턴한다.
			}
		}
	}

	if( dwSubQuestIdx == 0 )	return 1;			// 수행가능한 협행						// 서브 퀘스트 인덱스가 0과 같으면, 수행가능한 협행.
	else						return 2;			// 수행중인 협행						// 서브 퀘스트 인덱스가 0과 같지 않으면, 수행중인 협행.

	return 0;		// 아무것두 아니다.	
}

BOOL CSubQuestInfo::CheckLimitCondition( DWORD dwQuestIdx )									// 퀘스트 제한 상태를 확인하는 함수.
{
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();									// 퀘스트 제한 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	CQuestLimit* pLimit = NULL;																// 퀘스트 제한 정보를 받을 포인터를 선언하고 null 처리를 한다.

	while( pos )																			// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );								// 퀘스트 제한 정보를 받는다.

		if( pLimit )																		// 퀘스트 제한정보가 유효하면,
		{
			if( !pLimit->CheckLimit( dwQuestIdx, 0 ) )										// 퀘스트 제한을 체크한다.
			{
				return FALSE;	// limit													// FALSE를 리턴한다.
			}
		}
	}
	return TRUE;																			// TRUE를 리턴한다.
}