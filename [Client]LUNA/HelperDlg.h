#pragma once
//=================================================================================================
//	FILE		: HelperDlg.h
//	PURPOSE		: Window interface to render Help list.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		GM�� ���� ���� �� �� ���� �޽����� ����ϱ� ���� ���� �޽��� ����
//=========================================================================
#ifdef _GMTOOL_

// CATION PART.
#define LK_CAPTION	"cHelperDlg::Linking"									// ��ũ �Լ� ĸ��.
#define AE_CAPTION	"cHelperDlg::ActionEvent"								// �׼� �̺�Ʈ �Լ� ĸ��.
#define OAE_CAPTION	"cHelperDlg::OnActionEvent"								// �� �׼� �̺�Ʈ �Լ� ĸ��.
#define GLT_CAPTION	"cHelperDlg::Get_Linked_Tutorial"						// ��ũ �� Ʃ�丮�� ��ȯ �Լ� ĸ��.

// ERROR MESSAGE
#define HD_EMSG1	"Failed to receive helper link list."					// ���� ��ũ �ޱ� ����.
#define HD_EMSG2	"Invalid Selected row number. %d"						// ���� ��  �� ��ȣ ����.
#define HD_EMSG3	"Failed to receive selected item pointer. %d"			// ���� �� ������ ������ �ޱ� ����.
#define HD_EMSG4	"Failed to receive default item pointer. %d"			// �⺻ ������ ������ �ޱ� ����.
#define HD_EMSG5	"Failed to receive link item position. %d"				// ��ũ ������ ������ �ޱ� ����.
#define HD_EMSG6	"Failed to receive link item pointer. %d"				// ��ũ ������ ������ �ޱ� ����.
#define HD_EMSG7	"Failed to receive linked tutorial pointer. %d"			// ��ũ �� Ʃ�丮�� ������ �ޱ� ����.
#define HD_EMSG8	"Failed to receive tutorial dialog's pointer."			// Ʃ�丮�� ���̾�α� ������ �ޱ� ����.

#endif //_GMTOOL_





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "./Interface/cDialog.h"											// ���̾�α� �������̽��� �����Ѵ�.

#include "TutorialManager.h"												// Ʃ�丮�� �Ŵ��� ����� �����Ѵ�.





//=========================================================================
//		�ʼ� ���� ��Ʈ.
//=========================================================================
#define DEFULT_LINKCOLOR	RGBA_MAKE(255, 255, 255, 255)					// �⺻ ��ũ ������ ���� �Ѵ�.
#define SELECT_LINKCOLOR	RGBA_MAKE(255, 255, 0, 255)						// ���� �� ������ ���� �Ѵ�.





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class cListDialog ;															// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
class cCheckBox ;															// üũ �ڽ� Ŭ������ �����Ѵ�.
class cButton ;																// ��ư Ŭ������ �����Ѵ�.





//=========================================================================
//		���� ��ũ ���̾�α� Ŭ���� ����
//=========================================================================
class cHelperDlg : public cDialog											// ���� ��Ʈ�� ������ ���̾�α� Ŭ������ �����Ѵ�.
{
	cListDialog*			m_pHelpList ;									// ���� ����Ʈ ������.

	cCheckBox*				m_pShowTutorial ;								// Ʃ�丮���� ���̱� ���θ� üũ�ϴ� üũ�ڽ� ������.

	cButton*				m_pStartBtn ;									// ���� ��ư ������.
	cButton*				m_pCloseBtn ;									// �ݱ� ��ư ������.

	cPtrList*				m_pLinkList ;									// ���� ��ũ ����Ʈ ������.

	int						m_nSelectedLinkNum ;							// ���� �� ��ũ ��ȣ�� ��� ��� ����.

public:
	cHelperDlg(void);														// ������ �Լ�.
	virtual ~cHelperDlg(void);												// �Ҹ��� �Լ�.

	void Linking() ;														// ���̾�α� ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	virtual void OnActionEvent(LONG lId, void* p, DWORD we) ;				// ���� �� ��Ʈ�� �̺�Ʈ�� ó�� �� �Լ�.
	virtual DWORD ActionEvent(CMouse* mouseInfo) ;							// ���콺 �̺�Ʈ ó���� �ϴ� �Լ�.

	virtual void Render() ;													// ���� �Լ�.

	cTutorial* Get_Linked_Tutorial() ;										// ��ũ �� Ʃ�丮�� ������ ��ȯ�ϴ� �Լ�.
};
