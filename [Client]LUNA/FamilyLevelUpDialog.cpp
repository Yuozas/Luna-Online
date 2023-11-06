// FamilyLevelUpDialog.cpp: implementation of the CFamilyLevelUpDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FamilyLevelUpDialog.h"
#include "./interface/cStatic.h"
#include "WindowIDEnum.h"
//#include "FamilyManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "ObjectStateManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyLevelUpDialog::CFamilyLevelUpDialog()
{
	m_type = WT_FAMILYLEVELUPDLG;
}

CFamilyLevelUpDialog::~CFamilyLevelUpDialog()
{

}

void CFamilyLevelUpDialog::Linking()
{
	for(int i=0; i<4; ++i)
	{
		m_pLevelupNotComplete[i] = (cStatic*)GetWindowForID(FD_LU1NOTCOMPLETE+i);
		m_pLevelupComplete[i] = (cStatic*)GetWindowForID(FD_LU1COMPLETE+i);
	}
	for(i=0; i<5; ++i)
		m_pLevel[i] = (cStatic*)GetWindowForID(GD_LU1+i);
}

void CFamilyLevelUpDialog::SetLevel(BYTE level)
{
	ASSERT(level>=0);
	ASSERT(level<=5);
	BYTE lvl = level - 1;
	for(int i=0; i<lvl; ++i)
	{
		m_pLevelupNotComplete[i]->SetActive(FALSE);
		m_pLevelupComplete[i]->SetActive(TRUE);
	}
	for(i=lvl; i<4; ++i)
	{
		m_pLevelupNotComplete[i]->SetActive(TRUE);
		m_pLevelupComplete[i]->SetActive(FALSE);
	}
	for(i=0; i<5; ++i)
		m_pLevel[i]->SetFGColor(RGB_HALF(255, 255, 255));
	m_pLevel[level-1]->SetFGColor(RGB_HALF(255,255,0));
}

void CFamilyLevelUpDialog::SetActive(BOOL val)
{
	// 070801 웅주, 기존 패밀리 제거하며 주석처리

	//if(val == TRUE)
	//{
	//	BYTE level = FAMILYMGR->GetFamilyLevel();
	//	SetLevel(level);
	//}
	//else
	//{
	//	if(HERO == 0)
	//		return;
	//	if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
	//			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	//}
	cDialog::SetActive(val);
}
