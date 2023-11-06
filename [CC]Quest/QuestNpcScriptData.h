// QuestNpcScriptData.h: interface for the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
#define AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;										// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.

class CQuestNpcScriptData									// ����Ʈ npc ��ũ��Ʈ ������ Ŭ����.
{
protected:
	DWORD		m_dwNpcIdx;									// npc �ε����� ��� ����.
	DWORD		m_dwScriptPage;								// ��ũ��Ʈ �������� ��� ����.
	DWORD		m_dwNpcMarkType;							// npc ��ũ Ÿ���� ��� ����.

	DWORD		m_dwDialogType ;							// ���̾�α� ��� Ÿ��.
	
public:
	CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );			// ������ �Լ�.
	virtual ~CQuestNpcScriptData();																				// �Ҹ��� �Լ�.

	DWORD GetNpcIdx() { return m_dwNpcIdx ; }
	DWORD GetScriptPage() { return m_dwScriptPage ; }
	DWORD GetNpcMarkType() { return m_dwNpcMarkType ; }
	DWORD GetDialogType() { return m_dwDialogType ; }

	inline DWORD	IsSameNpc( DWORD dwNpcIdx )		{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwScriptPage : 0; }	// ��ũ��Ʈ �������� �����ϴ� �Լ�.
	inline DWORD	IsSameNpcMark( DWORD dwNpcIdx )	{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwNpcMarkType : 0; }	// npc ��ũ�� �����ϴ� �Լ�.
	inline DWORD	GetNpcScriptPage()				{ return m_dwScriptPage; }									// npc ��ũ��Ʈ �������� �����ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
