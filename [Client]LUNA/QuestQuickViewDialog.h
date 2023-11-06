#pragma once
#include ".\interface\cdialog.h"					// ���̾�α� Ŭ���� ����� �ҷ��´�.

#define MAX_QUESTTREECOUNT	5													// ��ϰ����� ����Ʈ ���� 5���� ���� �Ѵ�.

enum ENUM_SUB_QUEST_TYPE
{
	e_SQT_NORMAL = 0,
	e_SQT_HUNT,
	e_SQT_COLLECTION,
	e_SQT_COMPLETED,
} ;

struct sQUEST_INFO ;															// ����Ʈ ������ ������ ����ü�� �����Ѵ�.

struct QuestTree ;																// ����Ʈ ������ ��� Ʈ�� ����ü�� �����Ѵ�.

class cListDialog ;																// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
class CQuestString ;															// ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.

class CQuestQuickViewDialog : public cDialog									// ���̾�α� Ŭ������ ��ӹ��� ����Ʈ �˸��� ���̾�α� Ŭ����.
{
	cPtrList		m_QuestTreeList ;											// ����Ʈ Ʈ���� ���� ����Ʈ.

	cListDialog*	m_pViewList ;												// ����Ʈ ������ ���� �� ����Ʈ ���̾�α�.

public:
	CQuestQuickViewDialog(void);												// ������ �Լ�.
	virtual ~CQuestQuickViewDialog(void);										// �Ҹ��� �Լ�.

	void Release() ;															// ���� �Լ�.

	void Linking() ;															// ���� ��Ʈ�ѷ����� ��ũ�ϴ� �Լ�.


	cPtrList* GetViewList() { return &m_QuestTreeList ; }						// ����Ʈ Ʈ���� ��� �ִ� ����Ʈ�� ��ȯ�ϴ� �Լ�.


	BOOL CheckSameQuest(DWORD dwQuestIdx) ;										// ����Ʈ ����Ʈ ���� ���� ����Ʈ�� �ִ��� ���θ� ��ȯ�ϴ� �Լ�.

	void RegistQuest(DWORD dwQuestIdx) ;										// �˸��� â�� ����Ʈ�� ��� ��Ű�� �Լ�.
	void DeleteQuest(DWORD dwQuestIdx) ;										// �˸��� â�� �ִ� ����Ʈ�� ���� �ϴ� �Լ�.

	void ResetQuickViewDialog()	;												// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.

	void AddMainString(DWORD dwQuestIdx, DWORD dwSubQuestIdx) ;					// ����Ʈ ������ �߰��ϴ� �Լ�.
	void AddSubString(DWORD dwQuestIdx, DWORD dwSubQuestIdx, int nType) ;		// ���� ����Ʈ ������ �߰��ϴ� �Լ�.

	void GetTitleString( char* pBuf, CQuestString* pQuestString ) ;				// ����Ʈ ��Ʈ���� ���� ��Ʈ���� ��� �Լ�.
};



//#pragma once
//#include "d:\luna\alpha_dev\[client]luna\interface\cdialog.h"
//
//#define MAX_QUESTTREECOUNT	5
//
//struct sQUEST_INFO ;
//
//class cListDialog ;
////class cStatic ;
//
//class CQuestQuickViewDialog : public cDialog
//{
//	BYTE			m_byAlpha ;
//
//	cPtrList		m_QuestTreeList ;
//
//	//cStatic*		m_pTitle ;
//	cListDialog*	m_pViewList ;
//
//public:
//	CQuestQuickViewDialog(void);
//	virtual ~CQuestQuickViewDialog(void);
//
//	void Release() ;
//
//	void Linking() ;
//
//	virtual DWORD ActionEvent(CMouse * mouseInfo);								// �̺�Ʈ ó�� �Լ�.
//	
//	BOOL CheckSameQuest(DWORD dwQuestIdx) ;										// ����Ʈ ����Ʈ ���� ���� ����Ʈ�� �ִ��� ���θ� ��ȯ�ϴ� �Լ�.
//
//	void RegistQuest(DWORD dwQuestIdx) ;										// �˸��� â�� ����Ʈ�� ��� ��Ű�� �Լ�.
//	void DeleteQuest(DWORD dwQuestIdx) ;										// �˸��� â�� �ִ� ����Ʈ�� ���� �ϴ� �Լ�.
//	void EndQuest(SEND_QUEST_IDX* msg) ;										// �˸��� â�� �ִ� ����Ʈ�� �����ϴ� �Լ�.
//
//	void StartSubQuest(SEND_QUEST_IDX* pmsg) ;									// �˸��� â�� ��� �� ���� ����Ʈ�� �����ϴ� �Լ�.
//	void EndSubQuest(SEND_QUEST_IDX* msg) ;										// �˸��� â�� ��� �� ���� ����Ʈ�� �����ϴ� �Լ�.
//	void UpdateQuest(SEND_SUBQUEST_UPDATE* msg) ;								// �˸��� â�� ��� �� ����Ʈ�� ������Ʈ �ϴ� �Լ�.
//
//	void ResetLoadedQuickViewDialog() ;
//	void ResetQuickViewDialog()	;												// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
//
//	void CheckTreeState(int nSelectedLine) ;									// Ʈ���� ���� �������� ���� �������� üũ�ؼ� ����Ʈ�� �����ϴ� �Լ�.
//
//	//void MainDataLoad() ;														// �� �信 ��� �Ǿ� �ִ� ����Ʈ �����͸� �ε��ϴ� �Լ�.
//
//	cPtrList* GetViewList() { return &m_QuestTreeList ; }
//
//	void LoadQuickViewInfoFromFile(sQUEST_INFO* pInfo) ;
//};
