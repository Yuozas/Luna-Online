// ObjectStateManager.cpp: implementation of the CObjectStateManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ObjectActionManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CObjectStateManager);
CObjectStateManager::CObjectStateManager()
{

}

CObjectStateManager::~CObjectStateManager()
{

}

void CObjectStateManager::InitObjectState(CObject* pObject) 
{
	const BYTE State = pObject->m_BaseObjectInfo.ObjectState;

	if( eObjectState_Die != State )
	{
		pObject->OnStartObjectState( State );
		pObject->SetState( State );
		return;
	}

	switch( pObject->GetObjectKind() )
	{
	case eObjectKind_Monster:
		{
			//----KES ETC 071021
			//---enum문을 수정했다.
			pObject->GetEngineObject()->ChangeMotion( eMonsterMotion_Died, FALSE );
			//------------
			pObject->GetEngineObject()->DisablePick();
			break;
		}
	case eObjectKind_Player:
		{
			CPlayer* player	= ( CPlayer* )	pObject;

			// 시체 포즈
			{
				const WORD	weapon	=	player->GetWeaponAniType();
				const WORD	motion	=	CHARACTER_MOTION[ eCharacterMotion_Died1 ][ weapon ];

				player->ChangeMotion( motion, FALSE );
				player->ChangeBaseMotion( motion );
			}
			
			// 눈 감기
			{
				const CHARACTER_TOTALINFO* info = player->GetCharacterTotalInfo();

				player->SetFlag( TRUE );
				player->SetFaceShape( info->FaceType * 2 + 1 + 72 );
			}
			
			break;
		}
/*	case eObjectKind_BossMonster:
		{
			pObject->GetEngineObject()->ChangeMotion( eBossMonsterMotion_Died, TRUE );
			pObject->GetEngineObject()->DisablePick();
			break;
		}*/
	}
}

BOOL CObjectStateManager::StartObjectState(CObject* pObject,BYTE State) 
{
	if( !pObject )			return FALSE;

	switch(pObject->GetState())
	{
	case eObjectState_SkillSyn:
		{
			return FALSE;
		}
		break;

	case eObjectState_Die:
		{
			// Á×°Å³ª °Ô?Ó?» ³¡³»´Â°Ç ¾ðÁ¦³ª °¡´ÉÇÏ´Ù.
			if( State != eObjectState_Exit )
			{
				char buf[256];
				sprintf(buf, "StartObjectState(NowState : %d, ChangeState : %d)", pObject->GetState(), State);
//				ASSERTMSG(0, buf);
			}
		}
		return FALSE;

	case eObjectState_Move:
		{
			EndObjectState(pObject,pObject->GetState());
		}
		break;

	case eObjectState_Ungijosik:
	case eObjectState_Exchange:
	case eObjectState_StreetStall_Owner:
	case eObjectState_StreetStall_Guest:
	case eObjectState_Deal:
	case eObjectState_TiedUp:
	case eObjectState_Fishing:
	case eObjectState_FishingResult:
		{ 
			// Á×°Å³ª °Ô?Ó?» ³¡³»´Â°Ç ¾ðÁ¦³ª °¡´ÉÇÏ´Ù.
			if( State != eObjectState_Die &&
				State != eObjectState_Exit &&
				pObject->GetID() == HERO->GetID())
			{
				char buf[256];
				sprintf(buf, "StartObjectState(NowState : %d, ChangeState : %d)", pObject->GetState(), State);
//				ASSERTMSG(0, buf);
				return FALSE;
			}
		}
		break;
	case eObjectState_Society:
		{
			//¼­±â?Ï¶§¸¸ µû·Î Ã³¸®ÇÒ °Í?Ì ÇÊ¿äÇÏ´Ù.
			if( pObject->EndSocietyAct() == FALSE )
				return FALSE;
			else
				EndObjectState( pObject, pObject->GetState() );
		}
		break;
	default:
		break;
	}

	//?Ó½Ã
	if(pObject->GetState() == eObjectState_Ungijosik)
		pObject->OnEndObjectState(eObjectState_Ungijosik);

//	if( pObject->GetState() == eObjectState_Die )
//		return;

	pObject->OnStartObjectState(State);
	pObject->SetState(State);

	return TRUE;
}


void CObjectStateManager::EndObjectState(CObject* pObject,BYTE State,DWORD EndStateCount)
{
	if( !pObject )			return;

	if(pObject->GetState() != State) 
	{
		if( pObject->GetState() == eObjectState_Die )		//KESÃß°¡
		{
			return;	//Á×¾ú?»¶§ none?¸·Î ¹Ù²Ù´Â°Í?º ¿ë³³ÇÏÁö ¸øÇÑ´Ù!
		}

		if((pObject->GetState() == eObjectState_None) && (State == eObjectState_Ungijosik))
		{
		}
		else
		{
			char str[256];
			sprintf(str,"EndState Assert Cur:%d EndState:%d",pObject->GetState(),State);
			
//			ASSERTMSG(0,str);
		}
	}

	if(EndStateCount == 0)	// Áö±Ý Áï½Ã ³¡³½´Ù
	{
        pObject->SetState(eObjectState_None);
		pObject->OnEndObjectState(State);		//setstate()ÇÏ±â?ü¿¡ È£ÃâÇØ¾ßÇÑ´Ù.
	}
	else
	{
		pObject->m_ObjectState.State_End_Time = gCurTime + EndStateCount;
		pObject->m_ObjectState.bEndState = TRUE;
	}
}

void CObjectStateManager::StateProcess(CObject* pObject)
{
	if(pObject->m_ObjectState.bEndState)
	{
		if(pObject->m_ObjectState.State_End_Time < gCurTime)
		{
			EndObjectState(pObject,pObject->m_BaseObjectInfo.ObjectState);
		}
		else
		{
			// ³¡³»±â µ¿?Û?¸·Î ?üÈ¯
			DWORD RemainTime = pObject->m_ObjectState.State_End_Time - gCurTime;
			if(RemainTime <= pObject->m_ObjectState.State_End_MotionTime)
			{
				pObject->m_ObjectState.State_End_MotionTime = 0;
				if(pObject->m_ObjectState.State_End_Motion != -1)
					pObject->ChangeMotion(pObject->m_ObjectState.State_End_Motion,FALSE);
			}
		}
	}
}

BYTE CObjectStateManager::GetObjectState(CObject* pObject)
{
	ASSERT(pObject);
	return pObject->GetState();
}

BOOL CObjectStateManager::IsEndStateSetted(CObject* pObject)
{
	return pObject->m_ObjectState.bEndState;
}
/*
BOOL CObjectStateManager::CheckObjectState(CPlayer* pPlayer, BYTE State)
{
	BYTE CurState = pPlayer->GetState();

	switch(State)
	{
	case eObjectState_Ungijosik:
	case eObjectState_Exchange:
	case eObjectState_StreetStall_Owner:
		{
			if( CurState != eObjectState_None ) 
				return FALSE;
		}
		break;
	case eObjectState_SkillStart:
	case eObjectState_SkillSyn:
	case eObjectState_SkillBinding:
	case eObjectState_SkillUsing:
	case eObjectState_SkillDelay:
	case eObjectState_Deal:
	case eObjectState_StreetStall_Guest:
	case eObjectState_Tactic:
		{
			if( CurState != eObjectState_None &&
				CurState != eObjectState_Move ) 
				return FALSE;
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
*/

#define NOT_AVAILABLESTATE(a)		{	if(pObject->GetState() == (a))	return FALSE;	}
#define AVAILABLESTATE(a)			{	if(pObject->GetState() == (a))	return TRUE;	}

BOOL CObjectStateManager::CheckAvailableState(CObject* pObject,eCHECKAVAILABLESTATE cas)
{
	if( pObject->GetState() == eObjectState_Die )
	{
		//Á×?º »óÅÂ¿¡¼­´Â ?åÂø?» º¯°æÇÒ ¼ö ¾ø½?´Ï´Ù.
		return FALSE;
	}

	ySWITCH(cas)
		yCASE(eCAS_EQUIPITEM)
			NOT_AVAILABLESTATE(eObjectState_SkillStart)
			NOT_AVAILABLESTATE(eObjectState_SkillSyn)
			NOT_AVAILABLESTATE(eObjectState_SkillBinding)
			NOT_AVAILABLESTATE(eObjectState_SkillUsing)
			NOT_AVAILABLESTATE(eObjectState_SkillDelay)
			return TRUE;
	yENDSWITCH

	return TRUE;
}
