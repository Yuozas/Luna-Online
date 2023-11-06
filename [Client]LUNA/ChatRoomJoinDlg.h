#pragma once
//=================================================================================================
//	FILE		: CChatRoomJoinDlg.h
//	DESC		: Dialog interface to join chatting room.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
//#define _MB_CRJD_																// Use this definition to show messagebox.





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./interface/cDialog.h"												// Include header file of cDialog calss.

class cButton ;																	// Button interface.
class cEditBox ;																// EditBox interface.
class cStatic ;																	// Static interface.





//-------------------------------------------------------------------------------------------------
//		The class CChatRoomJoinDlg.
//-------------------------------------------------------------------------------------------------
class CChatRoomJoinDlg : public cDialog
{
	BYTE			m_byInputMode ;												// ���ȣ/��й�ȣ ��带 ���� ����.

	cEditBox*		m_pEdb_Code ;												// ��й�ȣ �Է� ����Ʈ�ڽ�.

	cButton*		m_pBtn_OK ;													// Ȯ�ι�ư.
	cButton*		m_pBtn_Cancel ;												// ��ҹ�ư.

	cStatic*		m_pStc_RoomIndex ;											// [ ���ȣ �Է� ]�� ��� �� ����ƽ.
	cStatic*		m_pStc_SecretCode ;											// [ ��й�ȣ �Է� ]�� ��� �� ����ƽ.

	cStatic*		m_pStc_IndexDesc ;											// ���ȣ �Է� ������ �� ����ƽ.
	cStatic*		m_pStc_CodeDesc ;											// ��й�ȣ �Է� ������ �� ����ƽ.

	//BYTE			m_byRoomIdx ;												// ä�ù� �ε����� ���� ����.

public:
	CChatRoomJoinDlg(void);														// ������ �Լ�.
	virtual ~CChatRoomJoinDlg(void);											// �Ҹ��� �Լ�.

	void Linking() ;															// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo ) ;					// Ű���� �̺�Ʈ�� ó���ϴ� �Լ�.

	virtual void SetActive(BOOL val) ;											// ���� ���̾�α� Ȱ��/��Ȱ��ȭ �Լ�.

	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;					// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.

	void SetInputMode(BYTE byMode) ;											// �Է� ��带 �����ϴ� �Լ�.
	//void SetRoomIdx(BYTE byIdx) { m_byRoomIdx = byIdx ; }						// 
};
