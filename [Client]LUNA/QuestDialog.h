// QuestDialog.h: interface for the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
#define AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"										// ������ �׸��� ��������� �ҷ��´�.
#include "..\[CC]Quest\QuestDefines.h"										// ����Ʈ ���� ��������� �ҷ��´�.

#include "GameResourceStruct.h"

#define MAX_QUEST_COUNT	20

#define MAX_REQUITAL_ICON_CELL 6

class cButton ;																// ��ư Ŭ������ �����Ѵ�.
class cIconGridDialog;														// ������ �׸��� ���̾�α� Ŭ������ �����Ѵ�.
class cIcon;																// ������ Ŭ������ �����Ѵ�.
class cListDialog;															// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
class cMsgBox;																// �޽��� �ڽ� Ŭ������ �����Ѵ�.
class cPtrList;																// ������ ����Ʈ Ŭ������ �����Ѵ�.
class cPushupButton;														// Ǫ���� ��ư Ŭ������ �����Ѵ�.
class CQuestString;															// ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.
class cStatic;																// ����ƽ Ŭ������ �����Ѵ�.

struct RequitalItem
{
	ITEM_INFO itemInfo ;
	int		nItemCount ;
	WORD	wPos ;

	cIcon*				pIcon;

	RequitalItem::RequitalItem()
	{
		memset(&itemInfo, 0, sizeof(RequitalItem)) ;
		nItemCount = 0 ;
		wPos = -1 ;

		pIcon = NULL ;
	}

	RequitalItem::~RequitalItem()
	{
		pIcon = NULL ;
	}
} ;

struct QuestInfo															// ����Ʈ ���� ����ü.
{
	CQuestString*	pQString;												// ����Ʈ ��Ʈ�� ������.
	SUBQUEST		sSubQuest;												// ���� ����Ʈ ����ü.

	QuestInfo::QuestInfo()													// ������ �Լ�.
	{
		pQString = NULL;													// ����Ʈ ��Ʈ�� �����͸� NULLó�� �Ѵ�.
		memset(&sSubQuest, 0, sizeof(sSubQuest));							// ���� ����Ʈ ����ü�� �޸� ���Ѵ�.
	}
	QuestInfo::~QuestInfo()													// �Ҹ��� �Լ�.
	{
		pQString = NULL;													// ����Ʈ ��Ʈ�� �����͸� NULLó�� �Ѵ�.
	}
};

struct QuestTree															// ����Ʈ Ʈ�� ����ü.
{
	DWORD		State;														// ���¸� ��� ����.
	DWORD		Count;														// ī��Ʈ ����.
	DWORD		QuestIdx;													// ����Ʈ �ε���.
	DWORD		SubQuestIdx;												// ���� ����Ʈ �ε���.

	int			nMapNum ;													// ����Ʈ �� ��ȣ.

	cPtrList	list;														// ����Ʈ.

	QuestTree()																// ������ �Լ�.
	{
		State = eQTree_NotUse;												// ���¸� eQTree_NotUse�� �����Ѵ�.
		Count = 0;															// ī��Ʈ�� 0���� �����Ѵ�.
		QuestIdx = 0;														// ����Ʈ �ε����� 0���� �����Ѵ�.
	}

	~QuestTree()															// �Ҹ��� �Լ�.
	{
		list.RemoveAll();													// ����Ʈ�� ��� ����.
	}
};

struct sQuestItem															// ����Ʈ ������ ����ü.
{
	cIcon*				pIcon;												// ����Ʈ ������ ������.

	QUEST_ITEM_INFO*	pQuestItemInfo;										// ����Ʈ ������ ���� ������.

	DWORD				Count;												// ī��Ʈ.
	WORD				Pos;												// ��ġ.

	sQuestItem()															// ������ �Լ�.
	{
		pIcon = NULL;														// ������ �����͸� NULL ó�� �Ѵ�.
		pQuestItemInfo = NULL;												// ����Ʈ ������ ���� �����͸� NULL ó�� �Ѵ�.
		Count = 0;															// ī��Ʈ�� 0���� �����Ѵ�.
		Pos = -1;															// ��ġ�� -1�� �����Ѵ�.
	}

	~sQuestItem()															// �Ҹ��� �Լ�.
	{
		pIcon = NULL;														// ������ �����͸� NULLó�� �Ѵ�.
		pQuestItemInfo = NULL;												// ����Ʈ ������ ���� �����͸� NULLó�� �Ѵ�.
	}
};


class CQuestDialog : public cIconDialog										// ������ �׸��� ���̾�α׸� ��ӹ��� ����Ʈ ���̾�α� Ŭ����.
{
	cPushupButton*		m_pPageBtn[MAX_QUEST_PAGEBTN];						// �ִ� ������ ������ �� ��ŭ ������ ��ư �����͸� �����Ѵ�.

	cIconGridDialog*	m_pIconGridDlg;										// ������ �׸��� ���̾�α� ������.
	cListDialog*		m_pQuestListDlg;									// ����Ʈ ����Ʈ ���̾�α� ������.

	cStatic*			m_pQuestCount ;										// ����Ʈ �������� ����ϴ� ����ƽ.

	cButton*			m_pNoticeBtn ;										// ����Ʈ �˸��� ��ư.
	cButton*			m_pGiveupBtn ;										// ����Ʈ ���� ��ư.

	cListDialog*		m_pQuestDescList ;									// ����Ʈ ���� ����Ʈ ���̾�α� ������.

	cIconGridDialog*	m_pRequitalGrid ;									// ����Ʈ ���� ������ ���̾�α� ������.

	cStatic*			m_pRequitalGold ;									// ��� ���� ����ƽ ������.
	cStatic*			m_pExperience ;										// ����ġ ���� ����ƽ ������.

	int					m_SelectedQuestID;									// ���� �� ����Ʈ ���̵�.

	cMsgBox*			m_pMsgBox;											// �޼��� �ڽ��� ���ִ��� ���� Ȯ���ϱ� ���ؼ�

	int					m_QuestItemCount;									// ����Ʈ ������ ī��Ʈ.
	int					m_QuestItemPage;									// ����Ʈ ������ ������.
	int					m_TooltipIdx;										// ���� �ε���.
	int					m_RequitalItemTooltipIdx ;							// ���� ������ ���� �ε���.

	cPtrList			m_QuestItemList;									// ����Ʈ ������ ����Ʈ.
	cPtrList			m_QuestRequitalList ;								// ����Ʈ ���� ������ ����Ʈ.

	CIndexGenerator		m_IndexGenerator;									// �ε��� ���� Ŭ����.

	DWORD			State;													// ����Ʈ ���¸� ��� ����.

	int				m_QTreeCount;											// ����Ʈ Ʈ�� ī��Ʈ.
	int				m_nQuestCount ;											// ����Ʈ ī��Ʈ.

	cPtrList		m_QuestTree;											// ����Ʈ Ʈ�� ����Ʈ.

	// ���õ� ������ ����  �ٲ��ִ°ǵ�.. �̳༮�� ������.. �������ϴ�.. 
	// ���� ����� ����� �ٲ���..
	int				m_QListCount;											// ����Ʈ ����Ʈ ī��Ʈ.
	int				m_ChangedRow;											// ���� �� �� ��ȣ�� ��� ����.
	int				m_PreSelectedIdx;										// ���� �� �ε���.
	int				m_ListLineIdx;											// ����Ʈ ���� �ε���.
	int				m_toPreList;											// ���� ����Ʈ?.

	CQuestString*	m_QuestList[MAX_PROCESSQUEST];							// ����Ʈ ��Ʈ�� ������ ��� ����Ʈ ����Ʈ.
	CQuestString*	m_PreDescQuestString;									// ���� ���� ����Ʈ ��Ʈ�� ������?

	DWORD			m_dwQuestList[MAX_PROCESSQUEST] ;

	DWORD m_dwNpcIdx ;															// ����Ʈ �������� npc �ε����� �޴´�. �޴´�.
	DWORD m_dwScriptPage ;
	DWORD m_dwDialogType ;

public:
	CQuestDialog();															// ������ �Լ�.
	virtual ~CQuestDialog();												// �Ҹ��� �Լ�.

	virtual DWORD ActionEvent(CMouse * mouseInfo);							// �̺�Ʈ ó�� �Լ�.

	void	Linking();														// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	void	Render();														// ���� �Լ�.

	// QuestInfo
	void	ProcessQuestAdd(CQuestString* pQuestString, int nMapNum);		// ����Ʈ �߰� ���� ���μ��� �Լ�.
	void	ProcessQuestDelete(CQuestString* pQuestString);					// ����Ʈ ���� ���� ���μ��� �Լ�.
	void	CompleteQuestDelete(CQuestString* pQuestString);				// �Ϸ� ����Ʈ ���� ó���� �ϴ� �Լ�.
//	BOOL	CheckMainQuestList(CQuestString* pQuestString);

	CQuestString* QuestTitleString(int index) ;
	void	QuestListAdd(CQuestString* pQuestString, int SubID=0);			// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �߰��ϴ� �Լ�.
	void	QuestListReset();												// ����Ʈ ����Ʈ�� �ٽ� �����ϴ� �Լ�.

	void	SetSelectedQuestIdx(DWORD index) {	m_SelectedQuestID = index ; }
	DWORD	GetSelectedQuestID()		{	return m_SelectedQuestID;	}	// ���� �� ����Ʈ ���̵� �����ϴ� �Լ�.
	DWORD	GetQuestIdx(int index) ;

	void	CloseMsgBox()				{	m_pMsgBox = NULL;	}			// �޽��� �ڽ��� �ݴ� �Լ�.

	void	GiveupQuestDelete(DWORD QuestIdx);								// ���� ����Ʈ�� �����ϴ� �Լ�.

	// QuestItem
	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);		// ����Ʈ �������� �߰��ϴ� �Լ�.
	void	QuestItemDelete(DWORD ItemIdx);									// ����Ʈ �������� �����ϴ� �Լ�.
	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);			// ����Ʈ �������� ������Ʈ �ϴ� �Լ�.
	void	RefreshRequitalItem() ;
	void	RefreshQuestItem(DWORD Page);									// ����Ʈ ������ �������� ���ΰ�ħ�ϴ� �Լ�.

	// QuestLoad Confirm
	void	CheckQuestDate();												// ����Ʈ ��¥�� üũ�ϴ� �Լ�.

	//void QuestAbandon() ;

	cPtrList* GetTreeList() { return &m_QuestTree ; }

	void	SetTreeCount(int nTreeCount) { m_QTreeCount = nTreeCount ; }
	int		GetTreeCount()				 { return m_QTreeCount ; }

	void	UpdateQuestList(DWORD dwQuestIdx, DWORD dwSubQuestIdx) ;
	void	UpdateQuestDescView(DWORD dwQuestIdx, DWORD dwSubQuestIdx) ;

	void	SetQuestCount() ;
	int		GetQuestCount() { return m_nQuestCount ; }

	void	SetGold(DWORD dwGold) ;
	void	SetExp(DWORD dwExp) ;

	//void	AddQuestTree(QuestTree* pData) ;
	void AddQuestTree(int nMapNum) ;
	void DeleteTree(QuestTree* pTree) ;
	void	AddRequitalItem(ITEM_INFO* pQuestItemInfo, DWORD Count) ;

	void	SetQuestPrevInfo(CQuestString* pQuestString) ;
	void	RequestInfo(CQuestString* pQuestString) ;
	void	RequitalInfo(CQuestString* pQuestString) ;
	void	QuestDescView(CQuestString* pQuestString);						// ����Ʈ ������ ���� �Լ�.

	cListDialog* GetQuestList() { return m_pQuestListDlg ; }
	cListDialog* GetQuestDescList() { return m_pQuestDescList ; }

	void AddQuestTitle(CQuestString* pQuestString, int nMapNum) ;

protected:
	//void	MainQuestAdd(CQuestString* pQuestString, int nMapNum);			// ���� ����Ʈ�� �߰��ϴ� �Լ�.
	void	SubQuestAdd(CQuestString* pQuestString);						// ���� ����Ʈ�� �߰��ϴ� �Լ�.
	void	AddTooltipInfo(char* pString, cIcon* pIcon);					// ���� ������ �߰��ϴ� �Լ�.

	void	AddListEmptyLine();												// ����Ʈ�� ����(�����)�� �߰��ϴ� �Լ�.
	
	void	ShowItemToolTip(DWORD Index);									// ������ ������ �����ִ� �Լ�.
	void	ShowRequitalItemToolTip(DWORD Index) ;
	void	QuestTreeStateCheck(int RowIdx);								// ����Ʈ Ʈ�� ���¸� üũ�ϴ� �Լ�.




//======================================================================================//
public:

	void StartQuest(DWORD dwQuestIdx) ;
	void EndQuest(DWORD dwQuestIdx) ;








	void AddMapTree(int nMapNum, DWORD dwQuestIdx) ;
	void DeleteMapTree(int nMapNum) ;



	void RefreshQuestList() ;
















//======================================================================================//































};

#endif // !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)










//// QuestDialog.h: interface for the CQuestDialog class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
//#define AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include "./Interface/cIconDialog.h"										// ������ �׸��� ��������� �ҷ��´�.
//#include "..\[CC]Quest\QuestDefines.h"										// ����Ʈ ���� ��������� �ҷ��´�.
//
//#include "GameResourceStruct.h"
//
//#define MAX_QUEST_COUNT	20
//
//#define MAX_REQUITAL_ICON_CELL 6
//
//class cButton ;																// ��ư Ŭ������ �����Ѵ�.
//class cIconGridDialog;														// ������ �׸��� ���̾�α� Ŭ������ �����Ѵ�.
//class cIcon;																// ������ Ŭ������ �����Ѵ�.
//class cListDialog;															// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
//class cMsgBox;																// �޽��� �ڽ� Ŭ������ �����Ѵ�.
//class cPtrList;																// ������ ����Ʈ Ŭ������ �����Ѵ�.
//class cPushupButton;														// Ǫ���� ��ư Ŭ������ �����Ѵ�.
//class CQuestString;															// ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.
//class cStatic;																// ����ƽ Ŭ������ �����Ѵ�.
//
//struct RequitalItem
//{
//	ITEM_INFO itemInfo ;
//	int		nItemCount ;
//	WORD	wPos ;
//
//	cIcon*				pIcon;
//
//	RequitalItem::RequitalItem()
//	{
//		memset(&itemInfo, 0, sizeof(RequitalItem)) ;
//		nItemCount = 0 ;
//		wPos = -1 ;
//
//		pIcon = NULL ;
//	}
//
//	RequitalItem::~RequitalItem()
//	{
//		pIcon = NULL ;
//	}
//} ;
//
//struct QuestInfo															// ����Ʈ ���� ����ü.
//{
//	CQuestString*	pQString;												// ����Ʈ ��Ʈ�� ������.
//	SUBQUEST		sSubQuest;												// ���� ����Ʈ ����ü.
//
//	QuestInfo::QuestInfo()													// ������ �Լ�.
//	{
//		pQString = NULL;													// ����Ʈ ��Ʈ�� �����͸� NULLó�� �Ѵ�.
//		memset(&sSubQuest, 0, sizeof(sSubQuest));							// ���� ����Ʈ ����ü�� �޸� ���Ѵ�.
//	}
//	QuestInfo::~QuestInfo()													// �Ҹ��� �Լ�.
//	{
//		pQString = NULL;													// ����Ʈ ��Ʈ�� �����͸� NULLó�� �Ѵ�.
//	}
//};
//
//struct QuestTree															// ����Ʈ Ʈ�� ����ü.
//{
//	DWORD		State;														// ���¸� ��� ����.
//	DWORD		Count;														// ī��Ʈ ����.
//	DWORD		QuestIdx;													// ����Ʈ �ε���.
//	DWORD		SubQuestIdx;												// ���� ����Ʈ �ε���.
//
//	int			nMapNum ;													// ����Ʈ �� ��ȣ.
//
//	cPtrList	list;														// ����Ʈ.
//
//	QuestTree()																// ������ �Լ�.
//	{
//		State = eQTree_NotUse;												// ���¸� eQTree_NotUse�� �����Ѵ�.
//		Count = 0;															// ī��Ʈ�� 0���� �����Ѵ�.
//		QuestIdx = 0;														// ����Ʈ �ε����� 0���� �����Ѵ�.
//	}
//
//	~QuestTree()															// �Ҹ��� �Լ�.
//	{
//		list.RemoveAll();													// ����Ʈ�� ��� ����.
//	}
//};
//
//struct sQuestItem															// ����Ʈ ������ ����ü.
//{
//	cIcon*				pIcon;												// ����Ʈ ������ ������.
//
//	QUEST_ITEM_INFO*	pQuestItemInfo;										// ����Ʈ ������ ���� ������.
//
//	DWORD				Count;												// ī��Ʈ.
//	WORD				Pos;												// ��ġ.
//
//	sQuestItem()															// ������ �Լ�.
//	{
//		pIcon = NULL;														// ������ �����͸� NULL ó�� �Ѵ�.
//		pQuestItemInfo = NULL;												// ����Ʈ ������ ���� �����͸� NULL ó�� �Ѵ�.
//		Count = 0;															// ī��Ʈ�� 0���� �����Ѵ�.
//		Pos = -1;															// ��ġ�� -1�� �����Ѵ�.
//	}
//
//	~sQuestItem()															// �Ҹ��� �Լ�.
//	{
//		pIcon = NULL;														// ������ �����͸� NULLó�� �Ѵ�.
//		pQuestItemInfo = NULL;												// ����Ʈ ������ ���� �����͸� NULLó�� �Ѵ�.
//	}
//};
//
//
//class CQuestDialog : public cIconDialog										// ������ �׸��� ���̾�α׸� ��ӹ��� ����Ʈ ���̾�α� Ŭ����.
//{
//	cPushupButton*		m_pPageBtn[MAX_QUEST_PAGEBTN];						// �ִ� ������ ������ �� ��ŭ ������ ��ư �����͸� �����Ѵ�.
//
//	cIconGridDialog*	m_pIconGridDlg;										// ������ �׸��� ���̾�α� ������.
//	cListDialog*		m_pQuestListDlg;									// ����Ʈ ����Ʈ ���̾�α� ������.
//
//	cStatic*			m_pQuestCount ;										// ����Ʈ �������� ����ϴ� ����ƽ.
//
//	cButton*			m_pNoticeBtn ;										// ����Ʈ �˸��� ��ư.
//	cButton*			m_pGiveupBtn ;										// ����Ʈ ���� ��ư.
//
//	cListDialog*		m_pQuestDescList ;									// ����Ʈ ���� ����Ʈ ���̾�α� ������.
//
//	cIconGridDialog*	m_pRequitalGrid ;									// ����Ʈ ���� ������ ���̾�α� ������.
//
//	cStatic*			m_pRequitalGold ;									// ��� ���� ����ƽ ������.
//	cStatic*			m_pExperience ;										// ����ġ ���� ����ƽ ������.
//
//	int					m_SelectedQuestID;									// ���� �� ����Ʈ ���̵�.
//
//	cMsgBox*			m_pMsgBox;											// �޼��� �ڽ��� ���ִ��� ���� Ȯ���ϱ� ���ؼ�
//
//	int					m_QuestItemCount;									// ����Ʈ ������ ī��Ʈ.
//	int					m_QuestItemPage;									// ����Ʈ ������ ������.
//	int					m_TooltipIdx;										// ���� �ε���.
//	int					m_RequitalItemTooltipIdx ;							// ���� ������ ���� �ε���.
//
//	cPtrList			m_QuestItemList;									// ����Ʈ ������ ����Ʈ.
//	cPtrList			m_QuestRequitalList ;								// ����Ʈ ���� ������ ����Ʈ.
//
//	CIndexGenerator		m_IndexGenerator;									// �ε��� ���� Ŭ����.
//
//	DWORD			State;													// ����Ʈ ���¸� ��� ����.
//
//	int				m_QTreeCount;											// ����Ʈ Ʈ�� ī��Ʈ.
//	int				m_nQuestCount ;											// ����Ʈ ī��Ʈ.
//
//	cPtrList		m_QuestTree;											// ����Ʈ Ʈ�� ����Ʈ.
//
//	// ���õ� ������ ����  �ٲ��ִ°ǵ�.. �̳༮�� ������.. �������ϴ�.. 
//	// ���� ����� ����� �ٲ���..
//	int				m_QListCount;											// ����Ʈ ����Ʈ ī��Ʈ.
//	int				m_ChangedRow;											// ���� �� �� ��ȣ�� ��� ����.
//	int				m_PreSelectedIdx;										// ���� �� �ε���.
//	int				m_ListLineIdx;											// ����Ʈ ���� �ε���.
//	int				m_toPreList;											// ���� ����Ʈ?.
//
//	CQuestString*	m_QuestList[MAX_PROCESSQUEST];							// ����Ʈ ��Ʈ�� ������ ��� ����Ʈ ����Ʈ.
//	CQuestString*	m_PreDescQuestString;									// ���� ���� ����Ʈ ��Ʈ�� ������?
//
//public:
//	CQuestDialog();															// ������ �Լ�.
//	virtual ~CQuestDialog();												// �Ҹ��� �Լ�.
//
//	virtual DWORD ActionEvent(CMouse * mouseInfo);							// �̺�Ʈ ó�� �Լ�.
//
//	void	Linking();														// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
//
//	void	Render();														// ���� �Լ�.
//
//	// QuestInfo
//	void	ProcessQuestAdd(CQuestString* pQuestString, int nMapNum);		// ����Ʈ �߰� ���� ���μ��� �Լ�.
//	void	ProcessQuestDelete(CQuestString* pQuestString);					// ����Ʈ ���� ���� ���μ��� �Լ�.
//	void	CompleteQuestDelete(CQuestString* pQuestString);				// �Ϸ� ����Ʈ ���� ó���� �ϴ� �Լ�.
////	BOOL	CheckMainQuestList(CQuestString* pQuestString);
//
//	CQuestString* QuestTitleString(int index) ;
//	void	QuestListAdd(CQuestString* pQuestString, int SubID=0);			// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �߰��ϴ� �Լ�.
//	void	QuestListReset();												// ����Ʈ ����Ʈ�� �ٽ� �����ϴ� �Լ�.
//
//	void	SetSelectedQuestID( int SelectedIdx ) { m_SelectedQuestID = SelectedIdx ; }
//	DWORD	GetSelectedQuestID()		{	return m_SelectedQuestID;	}	// ���� �� ����Ʈ ���̵� �����ϴ� �Լ�.
//
//	void	CloseMsgBox()				{	m_pMsgBox = NULL;	}			// �޽��� �ڽ��� �ݴ� �Լ�.
//
//	void	GiveupQuestDelete(DWORD QuestIdx);								// ���� ����Ʈ�� �����ϴ� �Լ�.
//
//	// QuestItem
//	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);		// ����Ʈ �������� �߰��ϴ� �Լ�.
//	void	QuestItemDelete(DWORD ItemIdx);									// ����Ʈ �������� �����ϴ� �Լ�.
//	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);			// ����Ʈ �������� ������Ʈ �ϴ� �Լ�.
//	void	RefreshRequitalItem() ;
//	void	RefreshQuestItem(DWORD Page);									// ����Ʈ ������ �������� ���ΰ�ħ�ϴ� �Լ�.
//
//	// QuestLoad Confirm
//	void	CheckQuestDate();												// ����Ʈ ��¥�� üũ�ϴ� �Լ�.
//
//	//void QuestAbandon() ;
//
//	cPtrList* GetTreeList() { return &m_QuestTree ; }
//
//	void	SetTreeCount(int nTreeCount) { m_QTreeCount = nTreeCount ; }
//	int		GetTreeCount()				 { return m_QTreeCount ; }
//
//	void	UpdateQuestList(DWORD dwQuestIdx, DWORD dwSubQuestIdx) ;
//	void	UpdateQuestDescView(DWORD dwQuestIdx, DWORD dwSubQuestIdx) ;
//
//	void	SetQuestCount(int nCount) ;
//	int		GetQuestCount() { return m_nQuestCount ; }
//
//	void	SetGold(DWORD dwGold) ;
//	void	SetExp(DWORD dwExp) ;
//
//	//void	AddQuestTree(QuestTree* pData) ;
//	void AddQuestTree(int nMapNum) ;
//	void DeleteTree(QuestTree* pTree) ;
//	void	AddRequitalItem(ITEM_INFO* pQuestItemInfo, DWORD Count) ;
//
//	void	QuestDescView(CQuestString* pQuestString);						// ����Ʈ ������ ���� �Լ�.
//
//	cListDialog* GetQuestList() { return m_pQuestListDlg ; }
//	cListDialog* GetQuestDescList() { return m_pQuestDescList ; }
//
//	void AddQuestTitle(CQuestString* pQuestString, int nMapNum) ;
//
//protected:
//	//void	MainQuestAdd(CQuestString* pQuestString, int nMapNum);			// ���� ����Ʈ�� �߰��ϴ� �Լ�.
//	void	SubQuestAdd(CQuestString* pQuestString);						// ���� ����Ʈ�� �߰��ϴ� �Լ�.
//	void	AddTooltipInfo(char* pString, cIcon* pIcon);					// ���� ������ �߰��ϴ� �Լ�.
//
//	void	AddListEmptyLine();												// ����Ʈ�� ����(�����)�� �߰��ϴ� �Լ�.
//	
//	void	ShowItemToolTip(DWORD Index);									// ������ ������ �����ִ� �Լ�.
//	void	ShowRequitalItemToolTip(DWORD Index) ;
//	void	QuestTreeStateCheck(int RowIdx);								// ����Ʈ Ʈ�� ���¸� üũ�ϴ� �Լ�.
//};
//
//#endif // !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
