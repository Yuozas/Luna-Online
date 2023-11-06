// QuestInfo.cpp: implementation of the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "QuestInfo.h"																// 퀘스트 정보 헤더파일을 불러온다.

#include "SubQuestInfo.h"															// 서브 퀘스트 정보 헤더 파일을 불러온다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestInfo::CQuestInfo( DWORD dwQuestIdx )											// 생성자 함수.
{
	m_dwQuestIdx = dwQuestIdx;														// 퀘스트 인덱스를 세팅한다.

	m_dwSubQuestCount = 0;															// 서브 퀘스트 카운트를 0으로 세팅한다.
	m_dwEndParam = 0;																// 종료 파라메터를 0으로 세팅한다.
	m_SubQuestTable.Initialize( MAX_SUBQUEST );										// 서브 퀘스트 정보 테이블을 최대 서브 퀘스트 개수로 초기화 한다.

	m_bRepeat = FALSE;
}

CQuestInfo::~CQuestInfo()															// 소멸자 함수.
{
	Release();																		// 해제 함수를 호출한다.
}

void CQuestInfo::Release()															// 해제 함수.
{
	CSubQuestInfo* pSubQuestInfo = NULL;											// 서브 퀘스트 정보를 받을 포인터 변수를 선언하고, null 처리를 한다.

	m_SubQuestTable.SetPositionHead();												// 서브 퀘스트 테이블을 헤드로 세팅한다.

	while( pSubQuestInfo = m_SubQuestTable.GetData() )								// 서브 퀘스트 정보가 있는 동안 while 문을 돌린다.
	{
		delete pSubQuestInfo;														// 서브 퀘스트 정보를 삭제한다.
	}

	m_SubQuestTable.RemoveAll();													// 서브 퀘스트 정보 테이블을 모두 비운다.
}

void CQuestInfo::AddSubQuestInfo( DWORD dwSubQuestIdx, CSubQuestInfo* pSubQuest )	// 서브 퀘스트 정보를 추가하는 함수.
{
	if( dwSubQuestIdx >= MAX_SUBQUEST ) return;										// 서브 퀘스트 인덱스가, 최대 서브 퀘스트 인덱스를 넘어가면, 리턴 처리를 한다.

	m_SubQuestTable.Add( pSubQuest, dwSubQuestIdx );								// 서브 퀘스트 정보 테이블에 서브 퀘스트를 추가한다.
	
	++m_dwSubQuestCount;															// 서브 퀘스트 카운터를 증가한다.
}
