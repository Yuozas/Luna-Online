/**************����Դ���Ի����� ������������ʾ����ҵ ��ѧϰ*******************/


/*************վ�� ���ڰ� ��л����û��Կ�����������Ϸ��Դ�����Ĵ���֧��*****************/

/**************����Դ���Ի����� ������������ʾ����ҵ ��ѧϰ*******************/




/*************վ�� ���ڰ� ��л����û��Կ�����������Ϸ��Դ�����Ĵ���֧��*****************/
#include "stdafx.h"
#include ".\abilityuse_pet.h"
#include "AbilityManager.h"


#ifdef _MHCLIENT_
#include "PetManager.h"

#endif



CAbilityUse_Pet::CAbilityUse_Pet(void)
{
}

CAbilityUse_Pet::~CAbilityUse_Pet(void)
{
}

void CAbilityUse_Pet::Use(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_


	ySWITCH(pAbilityInfo->GetInfo()->Ability_effect_Param1)

		yCASE(eAUKPET_State)
			//!!!�ֻ���â ����

			PETMGR->OpenPetStateDlg();
		yCASE(eAUKPET_Inven)
			//!!!���κ�â ����


			PETMGR->OpenPetInvenDlg();
		yCASE(eAUKPET_Skill)
			//!!!�ֽ�ų���

			PETMGR->CheckRestNSkillUse();
		yCASE(eAUKPET_Rest)
			//!!!���޽Ļ��¼���

			PETMGR->SendPetRestMsg(TRUE);
		yCASE(eAUKPET_Seal)
			//!!!�ֺ���
			PETMGR->SendPetSealMsg();
			yDEFAULT
		yENDSWITCH
#endif


}







