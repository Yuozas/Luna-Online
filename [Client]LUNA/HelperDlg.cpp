//=================================================================================================
//	FILE		: HelperDlg.cpp
//	PURPOSE		: Implemention part of helper dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		INCLUDE HEADER FILES
//=========================================================================
#include "stdafx.h"															// ǥ�� ��������� �����Ѵ�.
#include ".\helperdlg.h"													// ���� ���̾�α� ����� �����Ѵ�.

#include "../WindowIDEnum.h"												// ������ ���̵� ���� ����� �����Ѵ�.
#include "GameIn.h"															// gamein ����� �����Ѵ�.

#include "./interface/cListDialog.h"										// ����Ʈ ���̾�α� ����� �����Ѵ�.
#include "../Input/Mouse.h"													// ���콺 Ŭ���� ����� �����Ѵ�.
#include "./Interface/cCheckBox.h"											// üũ�ڽ� Ŭ���� ����� �����Ѵ�.

#include "TutorialDlg.h"													// Ʃ�丮�� ���̾�α� ����� �����Ѵ�.

#include "OptionManager.h"													// �ɼ� �Ŵ����� �����Ѵ�.





//=========================================================================
//		���� ���̾�α��� ������ �Լ�.
//=========================================================================
cHelperDlg::cHelperDlg(void)
{
	m_pHelpList		= NULL ;												// ���� ����Ʈ �����͸� nulló���� �Ѵ�.

	m_pShowTutorial = NULL ;												// Ʃ�丮���� ���̱� ���θ� üũ�ϴ� üũ�ڽ� �����͸� nulló���� �Ѵ�.

	m_pStartBtn		= NULL ;												// ���� ��ư �����͸� nulló���� �Ѵ�.
	m_pCloseBtn		= NULL ;												// �ݱ� ��ư �����͸� nulló���� �Ѵ�.

	m_pLinkList		= NULL ;												// ���� ��ũ ����Ʈ �����͸� null ó���� �Ѵ�.

	m_nSelectedLinkNum = 0 ;												// ���� �� ��ũ ��ȣ�� 0�� �����Ѵ�.
}





//=========================================================================
//		���� ���̾�α��� �Ҹ��� �Լ�.
//=========================================================================
cHelperDlg::~cHelperDlg(void)												// �Ҹ��� �Լ�.
{
}





//=========================================================================
//		���� ���̾�α��� ���� ��Ʈ�� ��ũ �Լ�.
//=========================================================================
void cHelperDlg::Linking()													// ���̾�α� ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
{
	m_pHelpList		= (cListDialog*)GetWindowForID(HD_LINKLIST) ;			// ���� ��ũ ����Ʈ�� ��ũ�Ѵ�.

	m_pShowTutorial	= (cCheckBox*)GetWindowForID(HD_SHOWTUTORIAL) ;			// Ʃ�丮�� ���̱� ���� üũ�ڽ��� ��ũ�Ѵ�.

	m_pStartBtn = (cButton*)GetWindowForID(HD_START) ;						// ���� ��ư�� ��ũ�Ѵ�.
	m_pCloseBtn = (cButton*)GetWindowForID(HD_END) ;						// �ݱ� ��ư�� ��ũ�Ѵ�.

	m_pLinkList = TUTORIALMGR->GetHelperList() ;							// Ʃ�丮�� �޴������� ���� ��ũ����Ʈ�� �޴´�.

	if( !m_pLinkList )														// ��ũ ����Ʈ�� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, HD_EMSG1, LK_CAPTION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	HELPLINK* pLink ;														// ���� ��ũ �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_pLinkList->GetHeadPosition() ;									// ��ũ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pLink = NULL ;														// ��ũ �����͸� null ó���� �Ѵ�.
		pLink = (HELPLINK*)m_pLinkList->GetNext(pos) ;						// ��ũ ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pLink ) continue ;												// ��ũ ������ ������ continue ó���� �Ѵ�.

		if( pLink->nIndex == 0 )											// ��ũ �ε����� 0�� ������,
		{
			m_pHelpList->AddItem(pLink->Title, SELECT_LINKCOLOR, pLink->nIndex) ;	// ���� �� �������� �������� �߰��Ѵ�.
		}
		else																// ��ũ �ε����� 0�� �ƴϸ�,
		{
			m_pHelpList->AddItem(pLink->Title, DEFULT_LINKCOLOR, pLink->nIndex) ;	// �⺻ �������� �������� �߰��Ѵ�.
		}
	}

	if( OPTIONMGR->GetGameOption()->bShowTutorial )
	{
		m_pShowTutorial->SetChecked(TRUE) ;
	}
	else
	{
		m_pShowTutorial->SetChecked(FALSE) ;
	}
}





//=========================================================================
//		���� �� ��Ʈ�� �̺�Ʈ�� ó�� �� �Լ�.
//=========================================================================
void cHelperDlg::OnActionEvent(LONG lId, void* p, DWORD we) 				// ���� �� ��Ʈ�� �̺�Ʈ�� ó�� �� �Լ�.
{
	switch(lId)
	{
	case HD_START :															// ���� ��ư�� �������ٸ�,
		{
			if(m_nSelectedLinkNum < 0)										// ���� �� �� ��ȣ�� 0���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, HD_EMSG2, m_nSelectedLinkNum) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, OAE_CAPTION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return ;													// return ó���Ѵ�.
			}

			cTutorial* pTutorial = NULL ;									// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pTutorial = Get_Linked_Tutorial() ;								// ��ũ �� Ʃ�丮���� �޴´�.

			if( !pTutorial )												// Ʃ�丮�� ������ ��ȿ���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, HD_EMSG7, m_nSelectedLinkNum) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, OAE_CAPTION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return ;													// return ó���Ѵ�.
			}

			TUTORIALMGR->Start_HelpTutorial(pTutorial) ;					// Ʃ�丮���� �����Ѵ�.
		}
		break ;

	case HD_END :															// ���� ��ư�� �������ٸ�,
		{
			cTutorialDlg* pDlg = NULL ;										// Ʃ�丮�� ���̾�α� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pDlg = GAMEIN->GetTutorialDlg() ;								// Ʃ�丮�� ���̾�α� ������ �޴´�.

			if( !pDlg )														// Ʃ�丮�� ���̾�α� ������ ��ȿ���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				MessageBox(NULL, HD_EMSG8, OAE_CAPTION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return ;													// return ó���Ѵ�.
			}

			pDlg->SetActive(FALSE) ;										// Ʃ�丮�� ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
		}
		break ;
	}
}





//=========================================================================
//		���콺 �̺�Ʈ ó���� �ϴ� �Լ�.
//=========================================================================
DWORD cHelperDlg::ActionEvent(CMouse* mouseInfo) 							// ���콺 �̺�Ʈ ó���� �ϴ� �Լ�.
{
	DWORD we = WE_NULL ;													// ������ �̺�Ʈ�� ���� ������ ó���ϰ� null �̺�Ʈ�� �����Ѵ�.

	we = cDialog::ActionEvent(mouseInfo) ;									// �⺻ ���̾�α� �̺�Ʈ�� �޴´�.

	if( !IsActive() ) return we ;											// Ȱ��ȭ ���°� �ƴϸ�, we�� return ó���Ѵ�.

	int nMouseX = mouseInfo->GetMouseX() ;									// �̺�Ʈ�� �߻��ϴ� ���콺 X��ǥ�� �޴´�.
	int nMouseY = mouseInfo->GetMouseY() ;									// �̺�Ʈ�� �߻��ϴ� ���콺 Y��ǥ�� �޴´�.

	if( !m_pHelpList->PtInWindow(nMouseX, nMouseY) ) return we ;			// ����Ʈ ���� �̺�Ʈ�� �ƴϸ�, we�� return ó���� �Ѵ�.

	if( we & WE_LBTNCLICK )													// ���콺 LBUTTON�� Ŭ���ߴٸ�,
	{
		m_nSelectedLinkNum = m_pHelpList->GetCurSelectedRowIdx() ;			// ���� ���� �� �� ��ȣ�� �޴´�.

		if(m_nSelectedLinkNum < 0)											// ���� �� �� ��ȣ�� 0���� ������,
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HD_EMSG2, m_nSelectedLinkNum) ;				// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, AE_CAPTION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return we ;														// we�� return ó���Ѵ�.
		}

		ITEM* pItem = NULL ;												// ������ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		pItem = m_pHelpList->GetItem(m_nSelectedLinkNum) ;					// ���� �� ���� �������� �޴´�.

		if( !pItem )														// ������ ������ ��ȿ���� ������,
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HD_EMSG3, m_nSelectedLinkNum) ;				// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, AE_CAPTION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return we ;														// we�� return ó���Ѵ�.
		}

		ITEM* pDefaultItem ;												// ���� ���� ����, �ٸ� ������ ������ ���� �����͸� �����Ѵ�.

		int listCount = m_pHelpList->GetItemCount() ;						// ���� ���� ����Ʈ�� ������ ������ �޴´�.

		for( int count = 0 ; count < listCount ; ++count )					// ������ �� ��ŭ for���� ������.
		{
			pDefaultItem = NULL ;											// �⺻ ������ �����͸� null ó���� �Ѵ�.
			pDefaultItem = m_pHelpList->GetItem(count) ;					// ī��Ʈ�� �ش��ϴ� ������ ������ �޴´�.

			if( !pDefaultItem )												// ������ ������ ��ȿ���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, HD_EMSG4, m_nSelectedLinkNum) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, AE_CAPTION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				continue ;													// continue ó���� �Ѵ�.
			}

			pDefaultItem->rgb = DEFULT_LINKCOLOR ;							// ������ ������ �⺻ �������� �����Ѵ�.
		}

		pItem->rgb = SELECT_LINKCOLOR ;										// ���� �� �������� ������ ���� �� �������� �����Ѵ�.
	}
	else if( we & WE_LBTNDBLCLICK )											// ���� Ŭ���� �ߴٸ�,
	{
		cTutorial* pTutorial = NULL ;										// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		pTutorial = Get_Linked_Tutorial() ;									// ��ũ �� Ʃ�丮���� �޴´�.

		if( !pTutorial )													// Ʃ�丮�� ������ ��ȿ���� ������,
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HD_EMSG7, m_nSelectedLinkNum) ;				// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, AE_CAPTION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return we ;														// we�� return ó���Ѵ�.
		}

		TUTORIALMGR->Start_HelpTutorial(pTutorial) ;						// Ʃ�丮���� �����Ѵ�.
	}

	return we ;																// we�� return ó���Ѵ�.
}





//=========================================================================
//		���� ���̾�α� ���� �Լ�.
//=========================================================================
void cHelperDlg::Render()													// ���� �Լ�.
{
	if( !IsActive() ) return ;												// Ȱ��ȭ ���°� �ƴϸ�, return ó���� �Ѵ�.

	cDialog::RenderWindow();												// �����츦 �����Ѵ�.
	cDialog::RenderComponent();												// ���� ��ü���� �����Ѵ�.
}





//=========================================================================
//		��ũ �� Ʃ�丮�� ������ ��ȯ�ϴ� �Լ�.
//=========================================================================
cTutorial* cHelperDlg::Get_Linked_Tutorial()			 					// ��ũ �� Ʃ�丮�� ������ ��ȯ�ϴ� �Լ�.
{
	ITEM* pItem = NULL ;													// ������ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pItem = m_pHelpList->GetItem(m_nSelectedLinkNum) ;						// ���� �� ���� �������� �޴´�.

	if( !pItem )															// ������ ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, HD_EMSG3, m_nSelectedLinkNum) ;					// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, GLT_CAPTION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���Ѵ�.
	}

	PTRLISTPOS pos = NULL ;													// ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_pLinkList->FindIndex(pItem->line) ;								// �������� �ε����� �ش��ϴ� ��ũ ��ġ�� �޴´�.

	if( !pos )																// ��ġ ������ �޴µ� ���� �ߴٸ�,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, HD_EMSG5, pItem->line) ;							// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, GLT_CAPTION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���Ѵ�.
	}				

	HELPLINK* pLinkItem = NULL ;											// ���� ��ũ �����͸� �����ϰ� null ó���� �Ѵ�.
	pLinkItem = (HELPLINK*)m_pLinkList->GetAt(pos) ;						// ��ġ�� �ش��ϴ� ��ũ �������� �޴´�.

	if( !pLinkItem )														// ��ũ ������ ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, HD_EMSG6, pItem->line) ;							// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, GLT_CAPTION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���Ѵ�.
	}

	cTutorial* pTutorial = NULL ;											// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pTutorial = TUTORIALMGR->Get_Tutorial(pLinkItem->nTutorial_Idx) ;		// ��ũ �� Ʃ�丮�� ������ �޴´�.

	return pTutorial ;														// Ʃ�丮�� ������ return ó���Ѵ�.
}