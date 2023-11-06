#pragma once
//=================================================================================================
//	FILE		: CChatRoomOptionDlg.h
//	DESC		: Dialog interface to show option of chatroom.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
//#define _MB_CROD_																// Use this definition to show messagebox.





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./interface/cDialog.h"												// Include header file of cDialog calss.

class cButton ;
class cComboBox ;
class cPushupButton ;
class cEditBox ;





//-------------------------------------------------------------------------------------------------
//		The class CChatRoomOptionDlg.
//-------------------------------------------------------------------------------------------------
class CChatRoomOptionDlg : public cDialog
{
	cPushupButton*		m_pOpen ;												// ���� ��� ��ư.
	cPushupButton*		m_pClose ;												// ����� ��� ��ư.

	cEditBox*			m_pCodeBox ;											// ��й�ȣ ����Ʈ �ڽ�.

	cComboBox*			m_pGuestCount ;											// �ִ� ������ �� �޺� �ڽ�.
	cComboBox*			m_pRoomType ;											// ä�ù� �з� �޺� �ڽ�.

	cEditBox*			m_pTitleBox ;											// ���� �Է� �޺� �ڽ�.

	cButton*			m_pOKBtn ;												// Ȯ�� ��ư.
	cButton*			m_pCancelBtn ;											// ��� ��ư.

	BYTE				m_bySecretMode ;										// ����/����� ��� ����.
	//WORD				m_wSecretCode ;											// Store secret code.

	BYTE				m_byGeustCount ;										// �ִ� ������ �� Ÿ�� ����.
	BYTE				m_byRoomType ;											// ä�ù� �з� ����.

	//char				m_TitleBuff[128] ;										// Store title of room.

	// ���� �� ���� ���� ����.
	//BYTE				m_byStoredMode ;										// ����/����� ��� ����.
	////WORD				m_wStoredCode ;											// Store secret code.
	//char				m_code[SECRET_CODE_SIZE+1] ;							// ��й�ȣ�� ���� ���ڿ�.

	//BYTE				m_byStoredGuestCount ;									// �ִ� ������ �� Ÿ�� ����.
	//BYTE				m_byStoredRoomType ;									// ä�ù� �з� ����.

	//char				m_StoredTitleBuff[128] ;								// ������ ���� ���ڿ�.

public:
	CChatRoomOptionDlg(void) ;													// ������ �Լ�.
	virtual ~CChatRoomOptionDlg(void) ;											// �Ҹ��� �Լ�.

	void Linking() ;															// ���� ��Ʈ�� ��ũ �Լ�.

	BOOL SetStoredDlg() ;														// ���� �� ������ �ɼ�â�� �����ϴ� �Լ�.
	void SetDlgToDefault() ;													// �⺻ ������ �ɼ�â�� �����ϴ� �Լ�.

	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;					// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.

	void Chk_Open() ;															// �������� �����ϴ� �Լ�.	
	void Chk_Close() ;															// ����� ���� �����ϴ� �Լ�.

	void Cmb_Guest() ;															// �ִ� ������ ���� �����ϴ� �Լ�.
	void Cmb_Roomtype() ;														// ä�ù� �з��� �����ϴ� �Լ�.
	
	void Btn_Ok() ;																// Ȯ�� ��ư�� ������ �� ó���� �ϴ� �Լ�.
	void Btn_Cancel() ;															// ��� ��ư�� ������ �� ó���� �ϴ� �Լ�.

	// �ɼ��� �����ϴ� �Լ�.
	//BOOL SetOption(ST_CR_ROOM_SRV* pInfo) ;										// �ɼ��� �����ϴ� �Լ�.
};
