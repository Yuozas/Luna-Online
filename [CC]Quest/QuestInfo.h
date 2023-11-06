// QuestInfo.h: interface for the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_)
#define AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"																	// ����Ʈ ���� ���� ��������� �ҷ��´�.

class CSubQuestInfo;																		// ���� ����Ʈ ���� Ŭ������ �����Ѵ�.

class CQuestInfo																			// ����Ʈ ���� Ŭ����.
{
protected:
	DWORD						m_dwQuestIdx;												// ����Ʈ �ε���.
	DWORD						m_dwSubQuestCount;											// ���� ����Ʈ ī��Ʈ.
	DWORD						m_dwEndParam;												// ���� �Ķ����.
	BOOL						m_bRepeat;

	int							m_nMapNum ;													// ����Ʈ�� ������ �� ��ȣ�� ���� ����.

	DWORD						m_dwNpcIdx ;												// ����Ʈ�� ������ npc �ε����� ���� ����.

	CYHHashTable<CSubQuestInfo>	m_SubQuestTable;											// ���� ����Ʈ ������ ��� �ִ� ����Ʈ ���̺�.

public:
	CQuestInfo( DWORD dwQuestIdx );															// ������ �Լ�.
	virtual ~CQuestInfo();																	// �Ҹ��� �Լ�.

	void	Release();																		// ���� �Լ�.

	// script
	void	AddSubQuestInfo( DWORD dwSubQuestIdx, CSubQuestInfo* pSubQuest );				// ���� ����Ʈ ������ �߰��ϴ� �Լ�.
	void	SetEndParam( DWORD Data )						{ m_dwEndParam = Data;	}		// ���� �Ķ���͸� �����ϴ� �Լ�.

	// getfn
	DWORD			GetQuestIdx()							{ return m_dwQuestIdx; }								// ����Ʈ �ε����� ��ȯ�ϴ� �Լ�.
	DWORD			GetSubQuestCount()						{ return m_dwSubQuestCount; }							// ���� ����Ʈ ������ ��ȯ�ϴ� �Լ�.
	CSubQuestInfo*	GetSubQuestInfo( DWORD dwSubQuestIdx )	{ return m_SubQuestTable.GetData( dwSubQuestIdx ); }	// ���� ����Ʈ ������ ��ȯ�ϴ� �Լ�.
	DWORD			GetEndParam()							{ return m_dwEndParam;	}								// ���� �Ķ���͸� ��ȯ�ϴ� �Լ�.

	void SetMapNum(int nMapNum)		{ m_nMapNum = nMapNum ; }								// ����Ʈ�� ������ �� ��ȣ�� �����ϴ� �Լ�.
	int	 GetMapNum()				{ return m_nMapNum ; }									// ����Ʈ�� ������ �� ��ȣ�� ��ȯ�ϴ� �Լ�.

	void  SetNpcIdx(DWORD dwNpcIdx)	{ m_dwNpcIdx = dwNpcIdx ; }								// ����Ʈ�� ������ npc �ε����� �����ϴ� �Լ�.
	DWORD GetNpcIdx()				{ return m_dwNpcIdx ; }									// ����Ʈ�� ������ npc �ε����� ��ȯ�ϴ� �Լ�.

	void  SetRepeat()				{ m_bRepeat = TRUE; }
	BOOL  IsRepeat()				{ return m_bRepeat; }
};

#endif // !defined(AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_)
