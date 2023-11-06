/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater_Interface.cpp: implementation of the CAbilityUpdater_Interface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUpdater_Interface.h"
#include "SuryunDialog.h"

#include "GameIn.h"
#include "AbilityGroup.h"
#include "AbilityManager.h"

#include "QuickManager.h"
#include "SkillPointRedist.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUpdater_Interface::CAbilityUpdater_Interface()
{



}

CAbilityUpdater_Interface::~CAbilityUpdater_Interface()
{


}

void CAbilityUpdater_Interface::Update(DWORD& Flag,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	if(Flag == ABILITYUPDATE_ABILITYQUICK_CHANGED)
		return;


	BYTE Level;

	int State;
	CAbility* pAbility;
	CAbilityInfo* pInfo;
	CSuryunDialog* pDlg = GAMEIN->GetSuryunDialog();
	pDlg->SetAbilityExp(pGroup->GetAbilityExp());
	CSkillPointRedist* pSkDlg = GAMEIN->GetSkPointDlg();


	pSkDlg->SetAbilityExp(pGroup->GetAbilityExp());

	if(Flag != ABILITYUPDATE_ABILITYEXP_CHANGED)
	{

		// to each Ability
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))
		{
			State = eAIS_Usable;
			Level = pAbility->GetLevel();
			pDlg->UpdateAbility(State,Level,pAbility->GetAbilityInfo());
		}
		
		// each AbilityInfo
		pInfoTable->SetPositionHead();

		while(pInfo = pInfoTable->GetData())
		{
			State = ABILITYMGR->GetState(pGroup,pInfo);
			Level = 0;
			// 06. 01. 绢呼府萍芒 牢磐其捞胶 荐沥 - 捞康霖
			// 扁粮 救焊捞绰 炼扒鳖瘤 焊捞绰 炼扒俊 甸绢艾促
			if(State == eAIS_OnlyVisible || State == eAIS_NotAvailable)
				pDlg->UpdateAbility(State,Level,pInfo);
		}
	}
	
	if(Flag == ABILITYUPDATE_ABILITYINITED)
	{
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))


		{

			if(pAbility->GetQuickPosition() == 0)
				continue;

			// 06. 01 捞康霖 - 窜绵芒 函版
			// 檬扁 汲沥 窃荐 AddQuickItemReal -> SetQuickItemReal

			// add quick item
			QUICKMGR->SetQuickItemReal(pAbility->GetQuickPosition(),
				pAbility->GetAbilityInfo()->GetAbsPosition(), pAbility->GetIndex() + MIN_ABILITY_INDEX );
/*
			CAbilityIcon* pAbilityIcon = GAMEIN->GetSuryunDialog()->GetAbilityIconAbs(pAbility->GetAbilityInfo()->GetAbsPosition());
			ASSERT(pAbilityIcon);
			if(!pAbilityIcon) continue;


			CQuickItem * pNewQItem = QUICKMGR->NewQuickItem((CBaseItem*)pAbilityIcon);


			pNewQItem->SetPosition(QuickPos);
			GAMEIN->GetQuickDialog()->AddQuickItem(QuickPos, pNewQItem);	*/
		}		
	}

	// 06. 01. 绢呼府萍芒 牢磐其捞胶 荐沥 - 捞康霖
	// 饭骇诀矫 砒屏阑 府悸茄促
	if(Flag == ABILITYUPDATE_CHARACTERLEVEL_CHANGED)
	{
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))
		{
			pDlg->UpdateAbility(pAbility->GetAbilityInfo());
		}
				
		pInfoTable->SetPositionHead();
		while(pInfo = pInfoTable->GetData())
		{
			pDlg->UpdateAbility(pInfo);
		}


	}
}





