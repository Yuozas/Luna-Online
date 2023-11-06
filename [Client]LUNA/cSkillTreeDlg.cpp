#include "stdafx.h"
#include ".\cskilltreedlg.h"
#include "cJobSkillDlg.h"
#include "./Interface/cPushupButton.h"
#include "ObjectManager.h"
#include "HERO.h"
#include ".\interface\cstatic.h"
#include "WindowIDEnum.h"

// 071022 LYW --- cSkillTreeDlg : Include header file of tutorial manager.
#include "TutorialManager.h"


cSkillTreeDlg::cSkillTreeDlg(void)
{
	m_type = WT_SKILLTREE_DLG;

	// 080203 LYW --- �ൿ���� �������� �ε��� �����⸦ �ʱ�ȭ �Ѵ�.
	m_IdxGenerator.Init(MAX_CONDUCTICON_NUM, IG_CONDUCT_STARTINDEX) ;
}

cSkillTreeDlg::~cSkillTreeDlg(void)
{
	// 080203 LYW --- �ൿ���� �������� �ε��� �����⸦ �����Ѵ�.
	m_IdxGenerator.Release() ;
}

void cSkillTreeDlg::Linking()
{
	mpSkillPoint = ( cStatic* )GetWindowForID( STD_SKILLPOINT );
}

void cSkillTreeDlg::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}
void cSkillTreeDlg::Add( cWindow * window )
{
	if( window->GetType() == WT_PUSHUPBUTTON )
		AddTabBtn( curIdx1++, (cPushupButton * )window );
	else if(window->GetType() == WT_JOBSKILL_DLG )
	{
		AddTabSheet( curIdx2++, window );
	}
	else 
		cDialog::Add(window);
}

void cSkillTreeDlg::SetSkillPoint()
{
	char buf[256];

	sprintf( buf, "%d", HERO->GetHeroTotalInfo()->SkillPoint );
	mpSkillPoint->SetStaticText( buf );
}

void cSkillTreeDlg::SetActive(BOOL val)
{
	if( val )
	{
		SetSkillPoint();
	}

	cTabDialog::SetActive( val );

	// 071022 LYW --- cSkillTreeDlg : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), val) ;
}