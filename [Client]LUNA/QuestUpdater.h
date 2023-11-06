// QuestUpdater.h: interface for the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
#define AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"										// ����Ʈ ���� ���� �ش������� �ҷ��´�.


class CPlayer;																// �÷��̾� Ŭ������ �����Ѵ�.
class CQuestString;															// ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.

class CQuestUpdater															// ����Ʈ �������� Ŭ����.
{
public:
	CQuestUpdater();														// ������ �Լ�.
	virtual ~CQuestUpdater();												// �Ҹ��� �Լ�.

	void	JournalItemAdd(JOURNALINFO* pItem);								// ���� �������� �߰��ϴ� �Լ�.

	void	CompleteQuestDelete(CQuestString* pQuestString);				// �Ϸ� ����Ʈ�� �����ϴ� �Լ�.
	void	ProcessQuestAdd(CQuestString* pQuestString);					// ����Ʈ �߰� ���μ���.
	void	ProcessQuestDelete(CQuestString* pQuestString);					// ����Ʈ ���� ���μ���.

	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo , DWORD Count);	// ����Ʈ ������ �߰� �Լ�.
	void	QuestItemDelete(DWORD ItemIdx);									// ����Ʈ ������ ���� �Լ�.
	void	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count);		// ����Ʈ ������ ������Ʈ �Լ�.
	DWORD	GetQuestItemCount(DWORD ItemIdx);								// ����Ʈ ������ �� ��ȯ �Լ�.

	//
	DWORD	GetSelectedQuestID();											// ���� �� ����Ʈ ���̵� ��ȯ �Լ�.
	void	CloseMsgBox();													// �ݱ� �޽��� �ڽ�.
	
	void	DeleteQuest(DWORD QuestIdx);									// ����Ʈ ���� �Լ�.
	
	void	UpdateSubQuestData();											// ���� ����Ʈ ������Ʈ �Լ�.
};

#endif // !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
