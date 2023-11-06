//=================================================================================================
//	FILE		: TutorialDlg.cpp
//	PURPOSE		: Implemention part of tutorial button dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 23, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		INCLUDE HEADER FILES
//=========================================================================
#include "stdafx.h"															// ǥ�� ��������� �����Ѵ�.
#include ".\tutorialbtndlg.h"												// Ʃ�丮�� �˸� ��ư ���̾�α� ����� �����Ѵ�.

#include "../WindowIDEnum.h"												// ������ ���̵� ���� ����� �����Ѵ�.

#include "./Interface/cButton.h"											// ��ư ����� �����Ѵ�.

#include "TutorialDlg.h"													// Ʃ�丮�� ���̾�α� ����� �����Ѵ�.

#include "GameIn.h"															// gamein ����� �����Ѵ�.

#include "./Audio/MHAudioManager.h"											// ����� �Ŵ����� �ҷ� �´�.
#include "TutorialManager.h"												// Ʃ�丮�� �Ŵ����� �ҷ��´�.
#include "ObjectManager.h"													// ������Ʈ �Ŵ����� �����Ѵ�.
#include "OptionManager.h"													// �ɼ� �Ŵ����� �����Ѵ�.





//=========================================================================
//		Ʃ�丮�� �˸� ��ư ���̾�α� ������ �Լ�.
//=========================================================================
cTutorialBtnDlg::cTutorialBtnDlg(void)										// ������ �Լ�.
{
	m_pButton = NULL ;														// ��ư �����͸� null ó���� �Ѵ�.

	m_nAlpha = 255 ;														// ��ư ���� ���� 255�� �����Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� �˸� ��ư ���̾�α� �Ҹ��� �Լ�.
//=========================================================================
cTutorialBtnDlg::~cTutorialBtnDlg(void)										// �Ҹ��� �Լ�.
{
}





//=========================================================================
//		���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
//=========================================================================
void cTutorialBtnDlg::Linking() 											// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
{
	m_pButton = (cButton*)GetWindowForID(TTR_NOTICEBTN) ;					// ���� ��ư�� ��ũ�Ѵ�.

	//if( OPTIONMGR->GetGameOption()->bShowTutorial )
	//{
	//	m_pButton->SetActive(TRUE) ;										// ��Ȱ��ȭ �Ѵ�.
	//}
	//else
	//{
	//	m_pButton->SetActive(FALSE) ;										// ��Ȱ��ȭ �Ѵ�.
	//}
}




//=========================================================================
//		�̺�Ʈ ó���� ���� �Լ�.
//=========================================================================
void cTutorialBtnDlg::OnActionEvent(LONG lId, void* p, DWORD we ) 			// �̺�Ʈ ó���� ���� �Լ�.
{
	if( lId != TTR_NOTICEBTN ) return ;										// ���� ��ư�� �ƴϸ� return ó���� �Ѵ�.

	cTutorialDlg* pDlg = GAMEIN->GetTutorialDlg() ;							// Ʃ�丮�� ���̾�α׸� ��´�.

	if( !pDlg ) return ;													// Ʃ�丮�� ���̾�αװ� ��ȿ���� ������, 

	pDlg->Start_Tutorial() ;												// Ʃ�丮���� �����Ѵ�.

	SetActive(FALSE) ;														// ���� ��ư ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
	m_pButton->SetActive(FALSE) ;											// ��ư�� ��Ȱ��ȭ �Ѵ�.
}





//=========================================================================
//		���� �Լ�.
//=========================================================================
void cTutorialBtnDlg::Render() 												// ���� �Լ�.
{
	if( !IsActive() ) return ;												// Ȱ��ȭ ���°� �ƴϸ�, return ó���� �Ѵ�.

	DWORD dwCurTick = GetTickCount() ;

	DWORD dwResultTime = dwCurTick - m_dwStartTime ;

	if( dwResultTime >= 1000 )
	{
		m_dwStartTime = GetTickCount() ;
	}

	m_nAlpha = (int)(2.5f * (dwResultTime/10)) ;

	m_pButton->SetAlpha((BYTE)m_nAlpha) ;									// ��ư�� ���� ���� �����Ѵ�.

	cDialog::RenderWindow();												// �����츦 �����Ѵ�.
	cDialog::RenderComponent();												// ���� ��ü���� �����Ѵ�.

	//m_pButton->Render() ;													// ��ư�� �����Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ���� ��ư�� Ȱ��ȭ ��Ű�� �Լ�.
//=========================================================================
void cTutorialBtnDlg::Active_NoticeBtn() 									// Ʃ�丮�� ���� ��ư�� Ȱ��ȭ ��Ű�� �Լ�.
{
	SetActive(TRUE) ;														// ���̾�α׸� Ȱ��ȭ ��Ų��.

	m_nAlpha = 255 ;														// ��ư ���� ���� 255�� �����Ѵ�.

	m_dwStartTime = GetTickCount() ;										// ���� �ð��� �޴´�.

	m_pButton->SetActive(TRUE) ;											// ��ư�� Ȱ��ȭ �Ѵ�.

	AUDIOMGR->Play(TUTORIALMGR->Get_ShowBtnSndIdx(), HERO);					// ��ư �⿬ ���带 �÷��� �Ѵ�.
}