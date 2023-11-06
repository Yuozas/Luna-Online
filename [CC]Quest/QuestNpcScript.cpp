// QuestNpcScript.cpp: implementation of the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "QuestNpcScript.h"

#include "QuestScriptLoader.h"
#include "QuestNpcScriptData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScript::CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// 생성자 함수.
{
	m_dwQuestIdx = dwQuestIdx;														// 퀘스트 인덱스를 세팅한다.
	m_dwSubQuestIdx = dwSubQuestIdx;												// 서브 퀘스트 인덱스를 세팅한다.
	ReadNpcScript( pTokens, dwQuestIdx, dwSubQuestIdx );							// NPC 스크립트를 읽어들인다.
}

CQuestNpcScript::~CQuestNpcScript()													// 소멸자 함수.
{
	Release();																		// 해제 함수를 호출한다.
}

void CQuestNpcScript::Release()														// 해제 함수.
{
	CQuestNpcScriptData* pNpcScriptData = NULL;										// NPC 스크립트 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// NPC 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																	// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// npc 스크립트 데이터 정보를 받는다.

		if( pNpcScriptData )														// npc 스크립트 데이터 정보가 유효하다면,
		{
			delete pNpcScriptData;													// npc 스크립트 데이터 정보를 삭제한다.
		}
	}
}

BOOL CQuestNpcScript::ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// npc 스크립트를 읽어들이는 함수.
{
	char* OneToken;																	// 토큰 포인터를 선언한다.

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )						// 토큰 정보가 유효한동안 while문을 돌린다.
	{
		if( OneToken[0] == '@' )													// 토큰이 @와 같다면,
		{
			// npc 스크립트 리스트에 npc 스크립트 데이터를 로딩해서 추가한다.
			m_NpcScriptList.AddTail( CQuestScriptLoader::LoadQuestNpcScriptData( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
		}
	}
	return TRUE;																	// TRUE를 리턴한다.
}

DWORD CQuestNpcScript::IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )			// npc 스크립트 페이지를 리턴하는 함수.
{
	*pQuestIdx = m_dwQuestIdx;														// 퀘스트 인덱스를 세팅한다.

	*pSubQuestIdx = m_dwSubQuestIdx;												// 서브 퀘스트 인덱스를 세팅한다.

	DWORD dwPage = 0;																// 페이지를 담을 변수를 선언하고 0으로 세팅한다.

	CQuestNpcScriptData* pNpcScriptData = NULL;										// NPC 스크립트 데이터 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// NPC 스크립트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while( pos )																	// 위치정보가 유효한동안 while문을 돌린다.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// 위치 정보에 해당하는 npc 스크립트 정보를 받는다.

		if( pNpcScriptData )														// npc 스크립트 정보가 유효하다면,
		{
			if( dwPage = pNpcScriptData->IsSameNpc( dwNpcIdx ) )					// 페이지를 받는다.
			{
				return dwPage;														// 페이지를 리턴한다.
			}
		}
	}
	return dwPage;																	// 페이지를 리턴한다.
}

DWORD CQuestNpcScript::IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )	// npc 마크를 리턴하는 함수.
{
	*pQuestIdx = m_dwQuestIdx;														// 퀘스트 인덱스를 세팅한다.

	*pSubQuestIdx = m_dwSubQuestIdx;												// 서브 퀘스트를 세팅한다.

	DWORD dwNpcMark = 0;															// npc 마크를 담을 변수를 선언하고 0으로 세팅한다.

	CQuestNpcScriptData* pNpcScriptData = NULL;										// npc 스크립트 데이터를 받을 포인터를 선언하고 null 처리를 한다.

	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();								// npc 스크립트 리스트를 헤드로 세팅하고 위치정보를 받는다.

	while( pos )																	// 위치 정보가 유효한동안 while문을 돌린다.
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );		// npc 스크립트 데이터 정보를 받는다.

		if( pNpcScriptData )														// npc 스크립트 데이터 정보가 유효하다면,
		{
			if( dwNpcMark = pNpcScriptData->IsSameNpcMark( dwNpcIdx ) )				// npc 마크를 받는다.
			{
				return dwNpcMark;													// npc 마크를 리턴한다.
			}
		}
	}
	return dwNpcMark;																// npc 마크를 리턴한다.
}
