// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																			// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "Quest.h"																			// 퀘스트 헤더를 불러온다.

#include "..\[CC]Quest\QuestInfo.h"															// 퀘스트 정보 헤더를 불러온다.
#include "..\[CC]Quest\SubQuestInfo.h"														// 서브 퀘스트 정보 헤더를 불러온다.

#include "ObjectManager.h"																	// 오브젝트 매니져 헤더를 불러온다.
#include "StatusIconDlg.h"																	// 상태 아이콘 다이얼로그 헤더를 불러온다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuest::CQuest( CQuestInfo* pQuestInfo )													// 생성자 함수.
{
	m_pQuestInfo = pQuestInfo;																// 퀘스트 정보를 받아 멤버 변수에 세팅한다.

	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );				// 서브 퀘스트 정보를 담는 배열을 메모리 세팅을 한다.

	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );							// 퀘스트 정보에서 0번째 서브 퀘스트 정보를 받아 배열0번에 세팅한다.

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();									// 서브 퀘스트 카운트를 받아 멤버 카운트에 세팅한다.


	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );							// 서브 퀘스트 테이블을 메모리 세팅한다.

	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;												// 서브 퀘스트 플래그, 데이터, 시간을 0으로 세팅한다.
	
	m_bCheckTime = FALSE;																	// 시간체크 여부를 FALSE로 세팅한다.
	m_CheckType = 0;																		// 체크 타입은 0으로 세팅한다.
	m_CheckTime.value = 0;																	// 시간 값을 0으로 세팅한다.
}

CQuest::~CQuest()																			// 소멸자 함수.
{
}

void CQuest::Initialize()																	// 초기화 함수.
{
	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );				// 서브 퀘스트 정보를 담는 배열을 메모리 세팅한다.

	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );							// 퀘스트 정보에서 0번째 서브 퀘스트 정보를 받아 배열0번에 세팅한다.

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();									// 서브 퀘스트 카운트를 받아 멤버 카운트에 세팅한다.

	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );							// 서브 퀘스트 테이블을 메모리 세팅한다.

	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;												// 서브 퀘스트 플래그, 데이터, 타임 값을 0으로 세팅한다.

	m_bCheckTime = FALSE;																	// 시간 체크 여부를 FALSE로 세팅한다.
	m_CheckType = 0;																		// 체크 타입을 0으로 세팅한다.
	m_CheckTime.value = 0;																	// 시간 값을 0으로 세팅한다.
}

void CQuest::SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )	// 메인 퀘스트 데이터를 세팅하는 함수.
{
	m_CurSubQuestInfoArray[0] = NULL;														// 현재, 서브 퀘스트 정보 배열 0을 null 처리 한다.
	m_dwSubQuestFlag = dwSubQuestFlag;														// 서브 퀘스트 플래그를 세팅한다.
	m_dwData = dwData;																		// 데이터를 세팅한다.
	m_dwTime = dwTime;																		// 시간 값을 세팅한다.

	m_CheckType = bCheckType;																// 체크 타입을 세팅한다.
	m_CheckTime.value = dwCheckTime;														// 체크 타임을 세팅한다.

	if( m_CheckType != 0 )																	// 체크 타입이 0과 같지 않으면,
	{
		m_bCheckTime = TRUE;																// 시간 체크 여부를 TRUE로 세팅한다.

		if( GetCurTime() < m_CheckTime.value )												// 현재 시간이 체크 타임보다 작으면,
            STATUSICONDLG->AddQuestIconCount();												// 퀘스트 아이콘 카운트를 추가한다.
	}
}

void CQuest::SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwSubData, DWORD dwSubTime )		// 서브 퀘스트 데이터를 세팅하는 함수.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );	// 서브 퀘스트 인덱스에 해당하는, 서브 퀘스트 배열에 퀘스트 정보를 담는다.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;										// 서브 퀘스트 데이터를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;										// 서브 퀘스트 시간을 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();	// 서브 퀘스트 카운트를 세팅한다.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// 시작하는 서브 퀘스트 인덱스를 세팅한다.
}

void CQuest::StartQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )					// 퀘스트를 시작하는 함수.
{
	m_CurSubQuestInfoArray[0] = NULL;														// 서브 퀘스트 정보 배열0을 NULL 처리 한다.
	m_dwSubQuestFlag = dwSubQuestFlag;														// 서브 퀘스트 플래그를 세팅한다.
	m_dwData = dwData;																		// 데이터를 세팅한다.
	m_dwTime = dwTime;																		// 시간을 세팅한다.
}

void CQuest::EndQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )					// 퀘스트를 종료하는 함수.
{
	if( dwSubQuestFlag == 0 )	// 반복														// 서브 퀘스트 플래그가 0과 같다면,
	{
		Initialize();																		// 초기화 함수를 호촐한다.
	}
	else																					// 그렇지 않으면,
	{
		m_dwSubQuestFlag = dwSubQuestFlag;													// 서브 퀘스트 플래그를 세팅한다.
		m_dwData = dwData;																	// 데이터를 세팅한다.
		m_dwTime = dwTime;																	// 시간 값을 세팅한다.

		m_bCheckTime = FALSE;																// 시간 체크 여부를 FALSE로 세팅한다.
		m_CheckType = 0;																	// 체크 타입을 0으로 세팅한다.
		m_CheckTime.value = 0;																// 시간 값을 0으로 세팅한다.
	}
}

void CQuest::StartSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )	// 서브 퀘스트를 시작하는 함수.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );					// 서브 퀘스트 정보를 세팅한다.
	m_dwSubQuestFlag = dwSubQuestFlag;																		// 서브 퀘스트 플래그를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// 서브 퀘스트 데이터를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// 서브 퀘스트 시간을 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();		// 서브 퀘스트 카운트를 세팅한다.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// 시작하는 서브 퀘스트 인덱스를 세팅한다.
}

void CQuest::EndSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )		// 서브 퀘스트를 종료하는 함수.
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = NULL;															// 서브 퀘스트 정보를 null 처리를 한다.
	m_dwSubQuestFlag = dwSubQuestFlag;																		// 서브 퀘스트 플래그를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// 서브 퀘스트 데이터를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// 서브 퀘스트 시간을 세팅한다.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// 시작하는 서브 퀘스트 인덱스를 세팅한다.
}

void CQuest::UpdateSubQuestData( DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )	// 서브 퀘스트를 업데이트 하는 함수.
{
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = dwMaxCount;													// 서브 퀘스트 최대 카운트를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;														// 서브 퀘스트 데이터를 세팅한다.
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;														// 서브 퀘스트 시간을 세팅한다.

	m_dwCurSubQuestIdx = dwSubQuestIdx ;																	// 시작하는 서브 퀘스트 인덱스를 세팅한다.
}

DWORD CQuest::GetQuestIdx()																					// 퀘스트 인덱스를 반환하는 함수.
{
	return m_pQuestInfo->GetQuestIdx();																		// 퀘스트 정보에 퀘스트 인덱스를 반환한다.
}

BOOL CQuest::IsNpcRelationQuest( DWORD dwNpcIdx )															// npc관련 퀘스트인지 체크하는 함수.
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// 서브 퀘스트 수만큼 for문을 돌린다.
	{
		if( m_CurSubQuestInfoArray[i] )																		// 카운트에 해당하는 서브 퀘스트 정보가 유효하면,
		if( m_CurSubQuestInfoArray[i]->IsNpcRelationQuest( dwNpcIdx ) )										// npc 관련 퀘스트가 맞다면,
			return TRUE;																					// TRUE를 리턴한다.
	}
	return FALSE;																							// FALSE를 리턴한다.
}

DWORD CQuest::GetNpcScriptPage( DWORD dwNpcIdx )															// NPC 스크립트 페이지를 반환하는 함수.
{
	DWORD dwPage = 0;																						// 페이지를 담을 변수를 선언하고 0으로 세팅한다.

	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// 서브 퀘스트 수만큼 for문을 돌린다.
	{
		if( m_CurSubQuestInfoArray[i] )																		// 현재 서브 퀘스트 정보가 유효하다면,
		if( dwPage = m_CurSubQuestInfoArray[i]->GetNpcScriptPage( dwNpcIdx ) )								// 현재 서브 퀘스트 정보의 npc 스크립트 페이지가 유효하다면,
			return dwPage;																					// 페이지를 리턴한다.
	}
	return dwPage;																							// 페이지를 리턴한다.
}

BOOL CQuest::IsSubQuestComplete( DWORD dwSubQuestIdx )														// 서브 퀘스트의 완료 여부를 리턴하는 함수.
{
	DWORD value = 1;																						// 값을 1로 세팅한다.
	return ( m_dwSubQuestFlag & ( value << (31-dwSubQuestIdx) ) ) ? TRUE : FALSE;							// 서브 퀘스트 플래그에 따른 값을 리턴한다.
}

DWORD CQuest::GetNpcMarkType( DWORD dwNpcIdx )																// npc 마크 타입을 반환한다.
{
	DWORD dwNpcMarkType = 0;																				// npc 마크 타입을 0으로 세팅한다.

	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// 서브 퀘스트 카운트 만큼 for문을 돌린다.
	{
		if( m_CurSubQuestInfoArray[i] )																		// 서브 퀘스트 정보가 유효하면,
		if( dwNpcMarkType = m_CurSubQuestInfoArray[i]->GetNpcMarkType( dwNpcIdx ) )							// 서브 퀘스트 정보에서 npc 인덱스로 마크 타입을 받는다.
			return dwNpcMarkType;																			// 마크 타입 정보가 유효하면, npc 타입을 리턴한다.
	}
	return dwNpcMarkType;																					// 마크 타입을 리턴한다.
}

CSubQuestInfo* CQuest::GetSubQuestInfo( DWORD dwSubQuestIdx )												// 서브 퀘스트 정보를 리턴하는 함수.
{
	return m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );													// 들어온 서브 퀘스트 인덱스로 서브 퀘스트 정보를 반환한다.
}

int CQuest::IsQuestState( DWORD dwNpcIdx )																	// 퀘스트 상태를 반환하는 함수.
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )															// 서브 퀘스트 카운트 만큼 for문을 돌린다.
	{
		if( m_CurSubQuestInfoArray[i] )																		// 서브 퀘스트 정보가 유효하면,
		{
			int rt = m_CurSubQuestInfoArray[i]->IsQuestState( dwNpcIdx );									// 현재 서브 퀘스트의 상태 값을 결과로 받는다.
			if( rt == 1 || rt == 2 )																		// 결과가 1이나 2이면, 
				return rt;																					// 결과를 리턴한다.
		}
	}
	return 0;																								// 0을 리턴한다.
}

BOOL CQuest::CanItemUse( DWORD dwQuestIdx )																	// 아이템 사용여부를 리턴한다.
{
	if( m_CurSubQuestInfoArray[0] )																			// 서브 퀘스트 정보 배열 0번이 유효하면,
	{
		return m_CurSubQuestInfoArray[0]->CheckLimitCondition( dwQuestIdx );								// 퀘스트 제한을 체크하여 리턴한다.
	}
	return FALSE;																							// FALSE를 리턴한다.
}

void CQuest::RegistCheckTime( BYTE bCheckType, DWORD dwCheckTime )											// 시간 체크를 등록하는 함수.
{
	m_bCheckTime = TRUE;																					// 시간 체크 여부를 TRUE로 세팅한다.
	m_CheckType = bCheckType;																				// 체크 타입을 세팅한다.
	m_CheckTime.value = dwCheckTime;																		// 타임 값을 세팅한다.
}

void CQuest::UnRegistCheckTime()																			// 시간 체크를 해제 하는 함수.
{
	m_bCheckTime = FALSE;																					// 시간 체크 여부를 FALSE로 세팅한다.
	m_CheckType = 0;																						// 체크 타입을 0으로 세팅한다.
	m_CheckTime.value = 0;																					// 타임 값을 0으로 세팅한다.
}