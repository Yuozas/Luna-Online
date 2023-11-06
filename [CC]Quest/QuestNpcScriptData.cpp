// QuestNpcScriptData.cpp: implementation of the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
#include "QuestNpcScriptData.h"												// 퀘스트 npc 스크립트 데이터 헤더를 불러온다.

#include "QuestScriptLoader.h"												// 퀘슽 스크립트 로더 헤더를 불러온다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScriptData::CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )		// 생성자 함수.
{
	GetScriptParam( m_dwNpcIdx, pTokens );						// 스크립트로 부터 npc 인덱스를 받는다.
	GetScriptParam( m_dwScriptPage, pTokens );					// 스크립트로 부터 스크립트 페이지를 받는다.
	GetScriptParam( m_dwNpcMarkType, pTokens );					// 스크립트로 부터 npc 마크 타입을 받는다.

	GetScriptParam( m_dwDialogType, pTokens ) ;					// 다이얼로그 내용 출력 여부를 결정한다.
}

CQuestNpcScriptData::~CQuestNpcScriptData()						// 소멸자 함수.
{
}

