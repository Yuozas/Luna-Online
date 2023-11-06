#pragma once
//=================================================================================================
//	FILE		: CChatRoomMainDlg.h
//	DESC		: Dialog interface to create chatting room.
//	PROGRAMMER	: Yongs Lee
//	DATE		: APRIL 3, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
//#define _MB_CRCD_																						// If you want to see error message box, then use this definition.





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./Interface/cDialog.h"																		// Include header file of cDialog calss.

class cButton ;
class cComboBox ;
class cEditBox ;
class cPushupButton ;





//-------------------------------------------------------------------------------------------------
//		The class CChatRoomCreateDlg.
//-------------------------------------------------------------------------------------------------
class CChatRoomCreateDlg : public cDialog
{
	cPushupButton*		m_pOpen ;					// ����/����� ��� ��ư.
	cPushupButton*		m_pClose ;

	cEditBox*			m_pCodeBox ;				// ��й�ȣ �Է� ����Ʈ�ڽ�.

	cComboBox*			m_pGuestCount ;				// �ִ� ������ �� �޺��ڽ�.
	cComboBox*			m_pRoomType ;				// ä�ù� �з� �޺��ڽ�.

	cEditBox*			m_pTitleBox ;				// ä�ù� ���� �Է� ����Ʈ�ڽ�.

	cButton*			m_pOKBtn ;					// Ȯ�� ��ư.
	cButton*			m_pCancelBtn ;				// ��� ��ư.

	BYTE				m_bySecretMode ;			// ����/����� ��� ����.

	BYTE				m_byGeustCount ;			// �ִ� ������ Ÿ���� ���� ����.
	BYTE				m_byRoomType ;				// ä�ù� �з��� ���� ����.

public:
	CChatRoomCreateDlg(void) ;						// ������ �Լ�.
	virtual ~CChatRoomCreateDlg(void) ;				// �Ҹ��� �Լ�.

	void Linking() ;								// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	virtual void SetActive(BOOL val) ;				// Ȱ��/��Ȱ��ȭ �Լ�.

	void SetDefaultDlg() ;							// ä�ù� ���� ���̾�α׸� �⺻ ���·� �����ϴ� �Լ�.

	// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;

	void Chk_Open() ;								// �������� �����ϴ� �Լ�.
	void Chk_Close() ;								// ����� ���� �����ϴ� �Լ�.

	void Cmb_Guest() ;								// �ִ� ������ ���� �����ϴ� �Լ�.
	void Cmb_Roomtype() ;							// ä�ù� �з��� �����ϴ� �Լ�.

	void Btn_Ok() ;									// Ȯ�� ��ư �̺�Ʈ�� ó���ϴ� �Լ�.
	void Btn_Cancel() ;								// ��� ��ư �̺��͸� ó���ϴ� �Լ�.

public :
	cEditBox* GetCodeBox()	{ return m_pCodeBox ; }	// ��й�ȣ �Է� ����Ʈ �ڽ� ��ȯ �Լ�.
};
