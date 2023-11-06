// QuestNpcScriptData.h: interface for the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
#define AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;										// 스트링 토크나이져 클래스를 선언한다.

class CQuestNpcScriptData									// 퀘스트 npc 스크립트 데이터 클래스.
{
protected:
	DWORD		m_dwNpcIdx;									// npc 인덱스를 담는 변수.
	DWORD		m_dwScriptPage;								// 스크립트 페이지를 담는 변수.
	DWORD		m_dwNpcMarkType;							// npc 마크 타입을 담는 변수.

	DWORD		m_dwDialogType ;							// 다이얼로그 출력 타입.
	
public:
	CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );			// 생성자 함수.
	virtual ~CQuestNpcScriptData();																				// 소멸자 함수.

	DWORD GetNpcIdx() { return m_dwNpcIdx ; }
	DWORD GetScriptPage() { return m_dwScriptPage ; }
	DWORD GetNpcMarkType() { return m_dwNpcMarkType ; }
	DWORD GetDialogType() { return m_dwDialogType ; }

	inline DWORD	IsSameNpc( DWORD dwNpcIdx )		{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwScriptPage : 0; }	// 스크립트 페이지를 리턴하는 함수.
	inline DWORD	IsSameNpcMark( DWORD dwNpcIdx )	{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwNpcMarkType : 0; }	// npc 마크를 리턴하는 함수.
	inline DWORD	GetNpcScriptPage()				{ return m_dwScriptPage; }									// npc 스크립트 페이지를 리턴하는 함수.
};

#endif // !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
