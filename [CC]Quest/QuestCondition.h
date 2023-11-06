// QuestCondition.h: interface for the CQuestCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_)
#define AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQuestEvent;																		// ����Ʈ �̺�Ʈ Ŭ������ �����Ѵ�.

class CQuestCondition																	// ����Ʈ ���� Ŭ����.
{
protected:
	DWORD			m_dwQuestIdx;														// ����Ʈ �ε����� ���� ����.
	DWORD			m_dwSubQuestIdx;													// ���� ����Ʈ �ε����� ���� ����.
	CQuestEvent*	m_pQuestEvent;														// ����Ʈ �̺�Ʈ ������ ����.

public:
	CQuestCondition( CQuestEvent* pQuestEvent, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestCondition();															// �Ҹ��� �Լ�.

	CQuestEvent*	GetQuestEvent()		{ return m_pQuestEvent; }						// ����Ʈ �̺�Ʈ�� ��ȯ�ϴ� �Լ�.

	BOOL	CheckCondition( CQuestEvent* pQuestEvent );									// ���¸� üũ�ϴ� �Լ�.
	BOOL	IsSameQuest( DWORD dwQuestIdx )	{ return (m_dwQuestIdx == dwQuestIdx) ? TRUE : FALSE; }		// ���� ����Ʈ���� üũ�ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_)
