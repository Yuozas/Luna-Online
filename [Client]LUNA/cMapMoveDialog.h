#pragma once
//=================================================================================================
//	FILE		: cMapMoveDialog.h
//	PURPOSE		: Dialog interface to move map.
//	PROGRAMMER	: Yongs Lee
//	DATE		: November 30, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		필요한 헤더파일을 불러온다.
//=========================================================================
#include "./Interface/cDialog.h"





//=========================================================================
//		필요한 컨트롤 인터페이스를 선언한다.
//=========================================================================
class cButton ;																// 버튼 클래스를 선언한다.
class cListDialog ;															// 리스트 다이얼로그 클래스를 선언한다.
class CItem ;																// 아이템 클래스를 선언한다.

struct stMAPMOVEITEM
{
	WORD	mapnum ;

	char	mapName[128] ;
} ;





//=========================================================================
//		맵 이동 다이얼로그 클래스 정의
//=========================================================================
class cMapMoveDialog : public cDialog
{
	cPtrList	 m_MapMoveList ;											// 맵 이동 리스트.

	cListDialog* m_pMapListDlg ;											// 맵 리스트 다이얼로그.

	cButton*	 m_pMoveBtn ;												// 이동버튼.
	cButton*	 m_pCloseBtn ;												// 닫기버튼.
	cButton*	 m_pXCloseBtn ;												// X닫기 버튼.

	int			 m_nSelectedMapNum ;										// 선택 된 맵 번호.

	CItem*		 m_pItem ;													// 아이템 포인터.

public:
	cMapMoveDialog(void);
	virtual ~cMapMoveDialog(void);

	void Linking() ;														// 하위 컨트롤 링킹 함수.

	virtual DWORD ActionEvent(CMouse * mouseInfo) ;							// 액션 이벤트 함수들 추가.
	virtual void OnActionEvent( LONG id, void* p, DWORD event ) ;			

	void InitializeMapList() ;												// 맵 리스트 초기화 함수.
	void RelaseMapLise() ;													// 맵 리스트 해제 함수.

	void MoveMapSyn() ;														// 이동 싱크 요청 함수.
	void MoveNotice() ;														// 이동 확인 알림 함수.

	void SetItemInfo(CItem* pItem) { m_pItem = pItem ; }					// 선택 한 아이템 인덱스와 포지션을 세팅한다.
	CItem* GetItemInfo() { return m_pItem ; }

	cListDialog* GetMapListDlg() { return m_pMapListDlg ; }

	void SetDisableState(BOOL val) ;
};
