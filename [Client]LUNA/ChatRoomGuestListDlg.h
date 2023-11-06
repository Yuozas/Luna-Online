#pragma once
//=================================================================================================
//	FILE		: CChatRoomGuestListDlg.h
//	DESC		: Dialog interface to show guest list.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
//#define _MB_CRGLD_																// Use this definition to show messagebox.
//#define MAX_ROW_GUESTLIST		10





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./interface/cDialog.h"												// Include header file of cDialog calss.

class cButton ;
//class cImage ;
//class cListCtrl ;
class cListDialog ;




class CChatRoomGuestListDlg : public cDialog
{
	cListDialog*		m_pGuestList ;								// �����ڸ� �����ִ� ����Ʈ ���̾�α�.

	cPtrList*			m_plGuest ;									// �����ڸ� ����ִ� ����Ʈ�� ���� ������.

	cButton*			m_pBtn_ChangeOwner ;						// �������� ��ư.
	cButton*			m_pBtn_FreezeGuest ;						// ���� ��ư.
	cButton*			m_pBtn_KickGuest ;							// �������� ��ư.
	cButton*			m_pBtn_AddFriend ;							// ģ���߰� ��ư.

	ST_CR_USER			m_SelectedUser ;							// ���� �� ���� ������ ���� ����ü.

	//DWORD				m_dwFreezedID[MAX_USER_PER_ROOM] ;
	//cImage			m_MarkImg[MAX_ROW_GUESTLIST] ;
	//ST_CR_USER		m_MyUserInfo ;
public:
	CChatRoomGuestListDlg(void) ;									// ������ �Լ�.
	virtual ~CChatRoomGuestListDlg(void) ;							// �Ҹ��� �Լ�.

	void Linking() ;												// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;				// ���콺 �̺�Ʈ�� ó���ϴ� �Լ�.
	// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;

	void InitGuestList(ST_CR_USER* pUser, BYTE byCount) ;			// ������ ����Ʈ�� �����ϴ� �Լ�.

	BOOL AddGuest(ST_CR_USER* pUser) ;								// ������ �߰� ó���� �ϴ� �Լ�.
	void DeleteGuest(DWORD dwPlayerID) ;							// ������ ���� ó���� �ϴ� �Լ�.

	void SetClassMark() ;											// ������ ������ ����Ʈ ���̾�α׿� �����ϴ� �Լ�.

	void SetDlgToDefault() ;										// ������ ����Ʈ�� �⺻ ���·� �����ϴ� �Լ�.


	void ChangeOwner() ;											// �������� ��ư�� ������ �� ó���� �ϴ� �Լ�.
	void FreezeGuest() ;											// ���� ��ư�� ������ �� ó���� �ϴ� �Լ�.
	void KickGuest() ;												// �������� ��ư�� ������ �� ó���� �ϴ� �Լ�.
	void AddFriend() ;												// ģ���߰� ��ư�� ������ �� ó���� �ϴ� �Լ�.

	//void ChangeOwner(DWORD dwBeforOwner, DWORD dwCurOwner) ;
	//virtual void Render() ;
	//BOOL IsFreezed(DWORD dwPlayerID) ;
	//void FreezeUser(DWORD dwPlayerID) ;
	//void UnFreezeUser(DWORD dwPlayerID) ;
	//void ClearFreeze(DWORD dwPlayerID) ;
};
