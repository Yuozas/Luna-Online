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
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "./Interface/cDialog.h"





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class cButton ;																// ��ư Ŭ������ �����Ѵ�.
class cListDialog ;															// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
class CItem ;																// ������ Ŭ������ �����Ѵ�.

struct stMAPMOVEITEM
{
	WORD	mapnum ;

	char	mapName[128] ;
} ;





//=========================================================================
//		�� �̵� ���̾�α� Ŭ���� ����
//=========================================================================
class cMapMoveDialog : public cDialog
{
	cPtrList	 m_MapMoveList ;											// �� �̵� ����Ʈ.

	cListDialog* m_pMapListDlg ;											// �� ����Ʈ ���̾�α�.

	cButton*	 m_pMoveBtn ;												// �̵���ư.
	cButton*	 m_pCloseBtn ;												// �ݱ��ư.
	cButton*	 m_pXCloseBtn ;												// X�ݱ� ��ư.

	int			 m_nSelectedMapNum ;										// ���� �� �� ��ȣ.

	CItem*		 m_pItem ;													// ������ ������.

public:
	cMapMoveDialog(void);
	virtual ~cMapMoveDialog(void);

	void Linking() ;														// ���� ��Ʈ�� ��ŷ �Լ�.

	virtual DWORD ActionEvent(CMouse * mouseInfo) ;							// �׼� �̺�Ʈ �Լ��� �߰�.
	virtual void OnActionEvent( LONG id, void* p, DWORD event ) ;			

	void InitializeMapList() ;												// �� ����Ʈ �ʱ�ȭ �Լ�.
	void RelaseMapLise() ;													// �� ����Ʈ ���� �Լ�.

	void MoveMapSyn() ;														// �̵� ��ũ ��û �Լ�.
	void MoveNotice() ;														// �̵� Ȯ�� �˸� �Լ�.

	void SetItemInfo(CItem* pItem) { m_pItem = pItem ; }					// ���� �� ������ �ε����� �������� �����Ѵ�.
	CItem* GetItemInfo() { return m_pItem ; }

	cListDialog* GetMapListDlg() { return m_pMapListDlg ; }

	void SetDisableState(BOOL val) ;
};
