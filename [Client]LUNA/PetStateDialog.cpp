#include "stdafx.h"
#include "WindowIDEnum.h"
#include ".\petstatedialog.h"
#include "gamein.h"
#include "cStatic.h"
#include "ObjectManager.h"
#include "petmanager.h"
#include "./Interface/cComboBox.h"
#include "cResourceManager.h"
#include "chatmanager.h"

CPetStateDialog::CPetStateDialog(void)
{
	m_type = WT_PET_STATE_DLG;
}

CPetStateDialog::~CPetStateDialog(void)
{
}

void CPetStateDialog::Linking()
{
	mName			= ( cStatic* )GetWindowForID(PET_STATUS_NAME);
	mLevel			= ( cStatic* )GetWindowForID(PET_STATUS_LEVEL);
	mType			= ( cStatic* )GetWindowForID(PET_STATUS_TYPE);
	mFriendly		= ( cStatic* )GetWindowForID(PET_STATUS_FRIENDLY);
	mGrade			= ( cStatic* )GetWindowForID(PET_STATUS_GRADE);

	mStr			= ( cStatic* )GetWindowForID(PET_STATUS_STR);
	mDex			= ( cStatic* )GetWindowForID(PET_STATUS_DEX);
	mVit			= ( cStatic* )GetWindowForID(PET_STATUS_VIT);
	mInt			= ( cStatic* )GetWindowForID(PET_STATUS_INT);
	mWis			= ( cStatic* )GetWindowForID(PET_STATUS_WIS);

	mPhysicAttack	= ( cStatic* )GetWindowForID(PET_STATUS_PHY_ATT);
	mMagicAttack	= ( cStatic* )GetWindowForID(PET_STATUS_MAG_ATT);
	mPhysicDefense	= ( cStatic* )GetWindowForID(PET_STATUS_PHY_DEF);
	mMagicDefense	= ( cStatic* )GetWindowForID(PET_STATUS_MAG_DEF);
	mAccuracy		= ( cStatic* )GetWindowForID(PET_STATUS_ACCURACY);
	mAvoid			= ( cStatic* )GetWindowForID(PET_STATUS_AVOID);

	mAI				= ( cComboBox* )GetWindowForID(PET_STATUS_AI_COMBOBOX);
}

void CPetStateDialog::OnActionEvent(LONG id, void* p, DWORD event)
{
	if( id == PET_STATUS_AI_COMBOBOX && event & WE_COMBOBOXSELECT)
	{
		BYTE ai = mAI->GetCurSelectedIdx();

		HEROPET->mAI = ai;

		mAI->SelectComboText( HEROPET->mAI );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1579+ai) );
	}
}

void CPetStateDialog::SetActive( BOOL val )
{
	if( val )	SetInfo();

	cDialog::SetActive( val );
}

void CPetStateDialog::SetInfo()
{
	float penalty = PETMGR->GetFriendlyPenalty( HEROPET->mFriendly );

	mAI->SetCurSelectedIdx( HEROPET->mAI );
	mAI->SelectComboText( HEROPET->mAI );

	mName->SetStaticText(HEROPET->GetObjectName());			
	mLevel->SetStaticValue(HEROPET->mLevel);			
	if( HEROPET->mType == ePetType_Basic )
	mType->SetStaticText(RESRCMGR->GetMsg(1005));	
	else
	mType->SetStaticText(RESRCMGR->GetMsg(HEROPET->mType+996));			
	mFriendly->SetStaticValue(HEROPET->mFriendly);		
	mGrade->SetStaticText(RESRCMGR->GetMsg(HEROPET->mGrade+1000));			

	mStr->SetStaticValue(HEROPET->mPetStat.mStrength.mPlus);			
	mDex->SetStaticValue(HEROPET->mPetStat.mDexterity.mPlus);			
	mVit->SetStaticValue(HEROPET->mPetStat.mVitality.mPlus);			
	mInt->SetStaticValue(HEROPET->mPetStat.mIntelligence.mPlus);			
	mWis->SetStaticValue(HEROPET->mPetStat.mWisdom.mPlus);			

	mPhysicAttack->SetStaticValue(HEROPET->mPetStat.mPhysicAttack.mPlus * penalty);	
	mMagicAttack->SetStaticValue(HEROPET->mPetStat.mMagicAttack.mPlus * penalty);	
	mPhysicDefense->SetStaticValue(HEROPET->mPetStat.mPhysicDefense.mPlus * penalty);
	mMagicDefense->SetStaticValue(HEROPET->mPetStat.mMagicDefense.mPlus * penalty);	
	mAccuracy->SetStaticValue(HEROPET->mPetStat.mAccuracy.mPlus);		
	mAvoid->SetStaticValue(HEROPET->mPetStat.mEvade.mPlus);			
}
