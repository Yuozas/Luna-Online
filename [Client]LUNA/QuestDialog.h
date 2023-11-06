// QuestDialog.h: interface for the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
#define AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"										// 아이콘 그리드 헤더파일을 불러온다.
#include "..\[CC]Quest\QuestDefines.h"										// 퀘스트 정의 헤더파일을 불러온다.

#include "GameResourceStruct.h"

#define MAX_QUEST_COUNT	20

#define MAX_REQUITAL_ICON_CELL 6

class cButton ;																// 버튼 클래스를 선언한다.
class cIconGridDialog;														// 아이콘 그리드 다이얼로그 클래스를 선언한다.
class cIcon;																// 아이콘 클래스를 선언한다.
class cListDialog;															// 리스트 다이얼로그 클래스를 선언한다.
class cMsgBox;																// 메시지 박스 클래스를 선언한다.
class cPtrList;																// 포인터 리스트 클래스를 선언한다.
class cPushupButton;														// 푸쉬업 버튼 클래스를 선언한다.
class CQuestString;															// 퀘스트 스트링 클래스를 선언한다.
class cStatic;																// 스태틱 클래스를 선언한다.

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

struct QuestInfo															// 퀘스트 정보 구조체.
{
	CQuestString*	pQString;												// 퀘스트 스트링 포인터.
	SUBQUEST		sSubQuest;												// 서브 퀘스트 구조체.

	QuestInfo::QuestInfo()													// 생성자 함수.
	{
		pQString = NULL;													// 퀘스트 스트링 포인터를 NULL처리 한다.
		memset(&sSubQuest, 0, sizeof(sSubQuest));							// 서브 퀘스트 구조체를 메모리 셋한다.
	}
	QuestInfo::~QuestInfo()													// 소멸자 함수.
	{
		pQString = NULL;													// 퀘스트 스트링 포인터를 NULL처리 한다.
	}
};

struct QuestTree															// 퀘스트 트리 구조체.
{
	DWORD		State;														// 상태를 담는 변수.
	DWORD		Count;														// 카운트 변수.
	DWORD		QuestIdx;													// 퀘스트 인덱스.
	DWORD		SubQuestIdx;												// 서브 퀘스트 인덱스.

	int			nMapNum ;													// 퀘스트 맵 번호.

	cPtrList	list;														// 리스트.

	QuestTree()																// 생성자 함수.
	{
		State = eQTree_NotUse;												// 상태를 eQTree_NotUse로 세팅한다.
		Count = 0;															// 카운트를 0으로 세팅한다.
		QuestIdx = 0;														// 퀘스트 인덱스를 0으로 세팅한다.
	}

	~QuestTree()															// 소멸자 함수.
	{
		list.RemoveAll();													// 리스트를 모두 비운다.
	}
};

struct sQuestItem															// 퀘스트 아이템 구조체.
{
	cIcon*				pIcon;												// 퀘스트 아이콘 포인터.

	QUEST_ITEM_INFO*	pQuestItemInfo;										// 퀘스트 아이템 정보 포인터.

	DWORD				Count;												// 카운트.
	WORD				Pos;												// 위치.

	sQuestItem()															// 생성자 함수.
	{
		pIcon = NULL;														// 아이콘 포인터를 NULL 처리 한다.
		pQuestItemInfo = NULL;												// 퀘스트 아이템 정보 포인터를 NULL 처리 한다.
		Count = 0;															// 카운트를 0으로 세팅한다.
		Pos = -1;															// 위치를 -1로 세팅한다.
	}

	~sQuestItem()															// 소멸자 함수.
	{
		pIcon = NULL;														// 아이콘 포인터를 NULL처리 한다.
		pQuestItemInfo = NULL;												// 퀘스트 아이템 정보 포인터를 NULL처리 한다.
	}
};


class CQuestDialog : public cIconDialog										// 아이콘 그리드 다이얼로그를 상속받은 퀘스트 다이얼로그 클래스.
{
	cPushupButton*		m_pPageBtn[MAX_QUEST_PAGEBTN];						// 최대 아이템 페이지 수 만큼 페이지 버튼 포인터를 선언한다.

	cIconGridDialog*	m_pIconGridDlg;										// 아이콘 그리드 다이얼로그 포인터.
	cListDialog*		m_pQuestListDlg;									// 퀘스트 리스트 다이얼로그 포인터.

	cStatic*			m_pQuestCount ;										// 퀘스트 보유수를 출력하는 스태틱.

	cButton*			m_pNoticeBtn ;										// 퀘스트 알림이 버튼.
	cButton*			m_pGiveupBtn ;										// 퀘스트 포기 버튼.

	cListDialog*		m_pQuestDescList ;									// 퀘스트 설명 리스트 다이얼로그 포인터.

	cIconGridDialog*	m_pRequitalGrid ;									// 퀘스트 보상 아이템 다이얼로그 포인터.

	cStatic*			m_pRequitalGold ;									// 골드 보상 스태틱 포인터.
	cStatic*			m_pExperience ;										// 경험치 보상 스태틱 포인터.

	int					m_SelectedQuestID;									// 선택 된 퀘스트 아이디.

	cMsgBox*			m_pMsgBox;											// 메세지 박스가 떠있는지 단지 확인하기 위해서

	int					m_QuestItemCount;									// 퀘스트 아이템 카운트.
	int					m_QuestItemPage;									// 퀘스트 아이템 페이지.
	int					m_TooltipIdx;										// 툴팁 인덱스.
	int					m_RequitalItemTooltipIdx ;							// 보상 아이템 툴팁 인덱스.

	cPtrList			m_QuestItemList;									// 퀘스트 아이템 리스트.
	cPtrList			m_QuestRequitalList ;								// 퀘스트 보상 아이템 리스트.

	CIndexGenerator		m_IndexGenerator;									// 인덱스 생성 클래스.

	DWORD			State;													// 퀘스트 상태를 담는 변수.

	int				m_QTreeCount;											// 퀘스트 트리 카운트.
	int				m_nQuestCount ;											// 퀘스트 카운트.

	cPtrList		m_QuestTree;											// 퀘스트 트리 리스트.

	// 선택된 라인의 색을  바꿔주는건데.. 이녀석들 때문에.. 지저분하다.. 
	// 좋은 방법이 생기면 바꾸자..
	int				m_QListCount;											// 퀘스트 리스트 카운트.
	int				m_ChangedRow;											// 변경 된 줄 번호를 담는 변수.
	int				m_PreSelectedIdx;										// 선택 된 인덱스.
	int				m_ListLineIdx;											// 리스트 라인 인덱스.
	int				m_toPreList;											// 이전 리스트?.

	CQuestString*	m_QuestList[MAX_PROCESSQUEST];							// 퀘스트 스트링 정보를 담는 퀘스트 리스트.
	CQuestString*	m_PreDescQuestString;									// 이전 설명 퀘스트 스트링 포인터?

	DWORD			m_dwQuestList[MAX_PROCESSQUEST] ;

	DWORD m_dwNpcIdx ;															// 퀘스트 정보에서 npc 인덱스를 받는다. 받는다.
	DWORD m_dwScriptPage ;
	DWORD m_dwDialogType ;

public:
	CQuestDialog();															// 생성자 함수.
	virtual ~CQuestDialog();												// 소멸자 함수.

	virtual DWORD ActionEvent(CMouse * mouseInfo);							// 이벤트 처리 함수.

	void	Linking();														// 하위 컨트롤들을 링크하는 함수.

	void	Render();														// 렌더 함수.

	// QuestInfo
	void	ProcessQuestAdd(CQuestString* pQuestString, int nMapNum);		// 퀘스트 추가 관련 프로세스 함수.
	void	ProcessQuestDelete(CQuestString* pQuestString);					// 퀘스트 삭제 관련 프로세스 함수.
	void	CompleteQuestDelete(CQuestString* pQuestString);				// 완료 퀘스트 삭제 처리를 하는 함수.
//	BOOL	CheckMainQuestList(CQuestString* pQuestString);

	CQuestString* QuestTitleString(int index) ;
	void	QuestListAdd(CQuestString* pQuestString, int SubID=0);			// 퀘스트 리스트에 퀘스트 스트링 정보를 추가하는 함수.
	void	QuestListReset();												// 퀘스트 리스트를 다시 세팅하는 함수.

	void	SetSelectedQuestIdx(DWORD index) {	m_SelectedQuestID = index ; }
	DWORD	GetSelectedQuestID()		{	return m_SelectedQuestID;	}	// 선택 된 퀘스트 아이디를 리턴하는 함수.
	DWORD	GetQuestIdx(int index) ;

	void	CloseMsgBox()				{	m_pMsgBox = NULL;	}			// 메시지 박스를 닫는 함수.

	void	GiveupQuestDelete(DWORD QuestIdx);								// 포기 퀘스트를 삭제하는 함수.

	// QuestItem
	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);		// 퀘스트 아이템을 추가하는 함수.
	void	QuestItemDelete(DWORD ItemIdx);									// 퀘스트 아이템을 삭제하는 함수.
	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);			// 퀘스트 아이템을 업데이트 하는 함수.
	void	RefreshRequitalItem() ;
	void	RefreshQuestItem(DWORD Page);									// 퀘스트 아이템 페이지를 새로고침하는 함수.

	// QuestLoad Confirm
	void	CheckQuestDate();												// 퀘스트 날짜를 체크하는 함수.

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
	void	QuestDescView(CQuestString* pQuestString);						// 퀘스트 설명을 보는 함수.

	cListDialog* GetQuestList() { return m_pQuestListDlg ; }
	cListDialog* GetQuestDescList() { return m_pQuestDescList ; }

	void AddQuestTitle(CQuestString* pQuestString, int nMapNum) ;

protected:
	//void	MainQuestAdd(CQuestString* pQuestString, int nMapNum);			// 메인 퀘스트를 추가하는 함수.
	void	SubQuestAdd(CQuestString* pQuestString);						// 서브 퀘스트를 추가하는 함수.
	void	AddTooltipInfo(char* pString, cIcon* pIcon);					// 툴팁 정보를 추가하는 함수.

	void	AddListEmptyLine();												// 리스트에 공백(빈라인)을 추가하는 함수.
	
	void	ShowItemToolTip(DWORD Index);									// 아이템 툴팁을 보여주는 함수.
	void	ShowRequitalItemToolTip(DWORD Index) ;
	void	QuestTreeStateCheck(int RowIdx);								// 퀘스트 트리 상태를 체크하는 함수.




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
//#include "./Interface/cIconDialog.h"										// 아이콘 그리드 헤더파일을 불러온다.
//#include "..\[CC]Quest\QuestDefines.h"										// 퀘스트 정의 헤더파일을 불러온다.
//
//#include "GameResourceStruct.h"
//
//#define MAX_QUEST_COUNT	20
//
//#define MAX_REQUITAL_ICON_CELL 6
//
//class cButton ;																// 버튼 클래스를 선언한다.
//class cIconGridDialog;														// 아이콘 그리드 다이얼로그 클래스를 선언한다.
//class cIcon;																// 아이콘 클래스를 선언한다.
//class cListDialog;															// 리스트 다이얼로그 클래스를 선언한다.
//class cMsgBox;																// 메시지 박스 클래스를 선언한다.
//class cPtrList;																// 포인터 리스트 클래스를 선언한다.
//class cPushupButton;														// 푸쉬업 버튼 클래스를 선언한다.
//class CQuestString;															// 퀘스트 스트링 클래스를 선언한다.
//class cStatic;																// 스태틱 클래스를 선언한다.
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
//struct QuestInfo															// 퀘스트 정보 구조체.
//{
//	CQuestString*	pQString;												// 퀘스트 스트링 포인터.
//	SUBQUEST		sSubQuest;												// 서브 퀘스트 구조체.
//
//	QuestInfo::QuestInfo()													// 생성자 함수.
//	{
//		pQString = NULL;													// 퀘스트 스트링 포인터를 NULL처리 한다.
//		memset(&sSubQuest, 0, sizeof(sSubQuest));							// 서브 퀘스트 구조체를 메모리 셋한다.
//	}
//	QuestInfo::~QuestInfo()													// 소멸자 함수.
//	{
//		pQString = NULL;													// 퀘스트 스트링 포인터를 NULL처리 한다.
//	}
//};
//
//struct QuestTree															// 퀘스트 트리 구조체.
//{
//	DWORD		State;														// 상태를 담는 변수.
//	DWORD		Count;														// 카운트 변수.
//	DWORD		QuestIdx;													// 퀘스트 인덱스.
//	DWORD		SubQuestIdx;												// 서브 퀘스트 인덱스.
//
//	int			nMapNum ;													// 퀘스트 맵 번호.
//
//	cPtrList	list;														// 리스트.
//
//	QuestTree()																// 생성자 함수.
//	{
//		State = eQTree_NotUse;												// 상태를 eQTree_NotUse로 세팅한다.
//		Count = 0;															// 카운트를 0으로 세팅한다.
//		QuestIdx = 0;														// 퀘스트 인덱스를 0으로 세팅한다.
//	}
//
//	~QuestTree()															// 소멸자 함수.
//	{
//		list.RemoveAll();													// 리스트를 모두 비운다.
//	}
//};
//
//struct sQuestItem															// 퀘스트 아이템 구조체.
//{
//	cIcon*				pIcon;												// 퀘스트 아이콘 포인터.
//
//	QUEST_ITEM_INFO*	pQuestItemInfo;										// 퀘스트 아이템 정보 포인터.
//
//	DWORD				Count;												// 카운트.
//	WORD				Pos;												// 위치.
//
//	sQuestItem()															// 생성자 함수.
//	{
//		pIcon = NULL;														// 아이콘 포인터를 NULL 처리 한다.
//		pQuestItemInfo = NULL;												// 퀘스트 아이템 정보 포인터를 NULL 처리 한다.
//		Count = 0;															// 카운트를 0으로 세팅한다.
//		Pos = -1;															// 위치를 -1로 세팅한다.
//	}
//
//	~sQuestItem()															// 소멸자 함수.
//	{
//		pIcon = NULL;														// 아이콘 포인터를 NULL처리 한다.
//		pQuestItemInfo = NULL;												// 퀘스트 아이템 정보 포인터를 NULL처리 한다.
//	}
//};
//
//
//class CQuestDialog : public cIconDialog										// 아이콘 그리드 다이얼로그를 상속받은 퀘스트 다이얼로그 클래스.
//{
//	cPushupButton*		m_pPageBtn[MAX_QUEST_PAGEBTN];						// 최대 아이템 페이지 수 만큼 페이지 버튼 포인터를 선언한다.
//
//	cIconGridDialog*	m_pIconGridDlg;										// 아이콘 그리드 다이얼로그 포인터.
//	cListDialog*		m_pQuestListDlg;									// 퀘스트 리스트 다이얼로그 포인터.
//
//	cStatic*			m_pQuestCount ;										// 퀘스트 보유수를 출력하는 스태틱.
//
//	cButton*			m_pNoticeBtn ;										// 퀘스트 알림이 버튼.
//	cButton*			m_pGiveupBtn ;										// 퀘스트 포기 버튼.
//
//	cListDialog*		m_pQuestDescList ;									// 퀘스트 설명 리스트 다이얼로그 포인터.
//
//	cIconGridDialog*	m_pRequitalGrid ;									// 퀘스트 보상 아이템 다이얼로그 포인터.
//
//	cStatic*			m_pRequitalGold ;									// 골드 보상 스태틱 포인터.
//	cStatic*			m_pExperience ;										// 경험치 보상 스태틱 포인터.
//
//	int					m_SelectedQuestID;									// 선택 된 퀘스트 아이디.
//
//	cMsgBox*			m_pMsgBox;											// 메세지 박스가 떠있는지 단지 확인하기 위해서
//
//	int					m_QuestItemCount;									// 퀘스트 아이템 카운트.
//	int					m_QuestItemPage;									// 퀘스트 아이템 페이지.
//	int					m_TooltipIdx;										// 툴팁 인덱스.
//	int					m_RequitalItemTooltipIdx ;							// 보상 아이템 툴팁 인덱스.
//
//	cPtrList			m_QuestItemList;									// 퀘스트 아이템 리스트.
//	cPtrList			m_QuestRequitalList ;								// 퀘스트 보상 아이템 리스트.
//
//	CIndexGenerator		m_IndexGenerator;									// 인덱스 생성 클래스.
//
//	DWORD			State;													// 퀘스트 상태를 담는 변수.
//
//	int				m_QTreeCount;											// 퀘스트 트리 카운트.
//	int				m_nQuestCount ;											// 퀘스트 카운트.
//
//	cPtrList		m_QuestTree;											// 퀘스트 트리 리스트.
//
//	// 선택된 라인의 색을  바꿔주는건데.. 이녀석들 때문에.. 지저분하다.. 
//	// 좋은 방법이 생기면 바꾸자..
//	int				m_QListCount;											// 퀘스트 리스트 카운트.
//	int				m_ChangedRow;											// 변경 된 줄 번호를 담는 변수.
//	int				m_PreSelectedIdx;										// 선택 된 인덱스.
//	int				m_ListLineIdx;											// 리스트 라인 인덱스.
//	int				m_toPreList;											// 이전 리스트?.
//
//	CQuestString*	m_QuestList[MAX_PROCESSQUEST];							// 퀘스트 스트링 정보를 담는 퀘스트 리스트.
//	CQuestString*	m_PreDescQuestString;									// 이전 설명 퀘스트 스트링 포인터?
//
//public:
//	CQuestDialog();															// 생성자 함수.
//	virtual ~CQuestDialog();												// 소멸자 함수.
//
//	virtual DWORD ActionEvent(CMouse * mouseInfo);							// 이벤트 처리 함수.
//
//	void	Linking();														// 하위 컨트롤들을 링크하는 함수.
//
//	void	Render();														// 렌더 함수.
//
//	// QuestInfo
//	void	ProcessQuestAdd(CQuestString* pQuestString, int nMapNum);		// 퀘스트 추가 관련 프로세스 함수.
//	void	ProcessQuestDelete(CQuestString* pQuestString);					// 퀘스트 삭제 관련 프로세스 함수.
//	void	CompleteQuestDelete(CQuestString* pQuestString);				// 완료 퀘스트 삭제 처리를 하는 함수.
////	BOOL	CheckMainQuestList(CQuestString* pQuestString);
//
//	CQuestString* QuestTitleString(int index) ;
//	void	QuestListAdd(CQuestString* pQuestString, int SubID=0);			// 퀘스트 리스트에 퀘스트 스트링 정보를 추가하는 함수.
//	void	QuestListReset();												// 퀘스트 리스트를 다시 세팅하는 함수.
//
//	void	SetSelectedQuestID( int SelectedIdx ) { m_SelectedQuestID = SelectedIdx ; }
//	DWORD	GetSelectedQuestID()		{	return m_SelectedQuestID;	}	// 선택 된 퀘스트 아이디를 리턴하는 함수.
//
//	void	CloseMsgBox()				{	m_pMsgBox = NULL;	}			// 메시지 박스를 닫는 함수.
//
//	void	GiveupQuestDelete(DWORD QuestIdx);								// 포기 퀘스트를 삭제하는 함수.
//
//	// QuestItem
//	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);		// 퀘스트 아이템을 추가하는 함수.
//	void	QuestItemDelete(DWORD ItemIdx);									// 퀘스트 아이템을 삭제하는 함수.
//	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);			// 퀘스트 아이템을 업데이트 하는 함수.
//	void	RefreshRequitalItem() ;
//	void	RefreshQuestItem(DWORD Page);									// 퀘스트 아이템 페이지를 새로고침하는 함수.
//
//	// QuestLoad Confirm
//	void	CheckQuestDate();												// 퀘스트 날짜를 체크하는 함수.
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
//	void	QuestDescView(CQuestString* pQuestString);						// 퀘스트 설명을 보는 함수.
//
//	cListDialog* GetQuestList() { return m_pQuestListDlg ; }
//	cListDialog* GetQuestDescList() { return m_pQuestDescList ; }
//
//	void AddQuestTitle(CQuestString* pQuestString, int nMapNum) ;
//
//protected:
//	//void	MainQuestAdd(CQuestString* pQuestString, int nMapNum);			// 메인 퀘스트를 추가하는 함수.
//	void	SubQuestAdd(CQuestString* pQuestString);						// 서브 퀘스트를 추가하는 함수.
//	void	AddTooltipInfo(char* pString, cIcon* pIcon);					// 툴팁 정보를 추가하는 함수.
//
//	void	AddListEmptyLine();												// 리스트에 공백(빈라인)을 추가하는 함수.
//	
//	void	ShowItemToolTip(DWORD Index);									// 아이템 툴팁을 보여주는 함수.
//	void	ShowRequitalItemToolTip(DWORD Index) ;
//	void	QuestTreeStateCheck(int RowIdx);								// 퀘스트 트리 상태를 체크하는 함수.
//};
//
//#endif // !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
