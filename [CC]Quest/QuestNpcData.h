// QuestNpcData.h: interface for the CQuestNpcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_)
#define AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;									// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.

class CQuestNpcData										// ����Ʈ NPC ������ Ŭ����.
{
protected:
	DWORD		m_dwNpcIdx;								// NPC �ε����� ���� ����.

public:
	CQuestNpcData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestNpcData();																		// �Ҹ��� �Լ�.

	// getfn
	DWORD	GetNpcIdx()			{ return m_dwNpcIdx; }												// NPC �ε����� ��ȯ�ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_)
