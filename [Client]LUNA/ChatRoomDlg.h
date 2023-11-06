#pragma once
//=================================================================================================
//	FILE		: CChatRoomDlg.h
//	DESC		: Dialog interface chatting room.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
//#define _MB_CRD_																// Use this definition to show messagebox.





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./interface/cDialog.h"												// Include header file of cDialog calss.

class cButton ;
class cEditBox ;
class cListDialog ;
class cStatic ;
//class cTextArea ;





//-------------------------------------------------------------------------------------------------
//		The class CChatRoomDlg.
//-------------------------------------------------------------------------------------------------
class CChatRoomDlg : public cDialog
{
    cStatic*			m_pRoomTitle ;						// ä�ù� ���� ��¿� ����ƽ.

	cListDialog*		m_pChatList ;						// ä�� ���� ��¿� ����Ʈ ���̾�α�.

	cEditBox*			m_pInputBox ;						// ä�� ���� �Է¿� ����Ʈ �ڽ�.
	//cTextArea*			m_pInputBox ;

	cButton*			m_pBtn_GuestList ;					// ��ȭ��� ��ư.
	cButton*			m_pBtn_Option ;						// ä�ÿɼ� ��ư.

	cButton*			m_pBtn_Close ;						// ä�ù� �ݱ� ��ư.

public:
	CChatRoomDlg(void) ;									// ������ �Լ�.
	virtual ~CChatRoomDlg(void) ;							// �Ҹ��� �Լ�.

	void Linking() ;										// ���� ��Ʈ�� ��ũ �Լ�.

	// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;
	//virtual void SetActive(BOOL val) ;						// ä�ù� Ȱ��/��Ȱ��ȭ �Լ�.

	void AddMsg(char* pMsg, BYTE byType) ;					// ä�ù� ���� �޽����� �߰��ϴ� �Լ�.
	void AddCommonMsg(BYTE byType, char* pMsg) ;			// �޽��� Ÿ�԰� �Բ� �޽����� �߰��ϴ� �Լ�.

	void SetRoomTitle(ST_CR_ROOM_CLT* pInfo) ;				// ä�ù� ���� �����Լ�.
	void SetDlgToDefault() ;								// ä�ù��� �⺻ ���·� �����ϴ� �Լ�.

	cEditBox* GetInputBox() { return m_pInputBox ; }		// ä�ù� ���� �Է¿� ����Ʈ �ڽ� ��ȯ �Լ�.
	cListDialog* GetListDlg() { return m_pChatList ; }		// ä�ù� ���� ��¿� ����Ʈ ���̾�α� ��ȯ �Լ�.

	DWORD GetMsgColor(BYTE byColorType) ;					// �޽��� Ÿ�Կ� ���� ������ ��ȯ�ϴ� �Լ�.

	void CloseChatRoom() ;									// ä�ù��� �ݴ� ó���� �ϴ� �Լ�.




	//virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	//virtual void Render() ;
	//virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo ) ;
	//void AddMsg(char* pMsg, DWORD dwColor) ;
	//void SetRoomTitle(char* pMsg) ;
	//cTextArea* GetInputBox() { return m_pInputBox ; }
};
