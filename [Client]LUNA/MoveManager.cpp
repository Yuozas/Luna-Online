// MoveManager.cpp: implementation of the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveManager.h"
#include "Object.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameResourceManager.h"
#include "KyungGongManager.h"
#include "GameIn.h"

#include ".\Engine\EngineObject.h"
#include "MHMap.h"

#include "MovePoint.h"

#include "CAction.h"
#include "ActionTarget.h"


#include "CharStateDialog.h"
#include "PathManager.h"
#include "TileManager.h"
#include "PartyIconManager.h"
#include "MacroManager.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "../[cc]skill/client/status/status.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.08.26	2007.09.13
#include "../hseos/Farm/SHFarmBuyDlg.h"
#include "../hseos/Farm/SHFarmManager.h"
#include "NpcScriptDialog.h"
// E ����ý��� �߰� added by hseos 2007.08.26	2007.09.13

#include "PetManager.h"
// 080509 KTH -- 
#include "../hseos/Farm/FarmAnimalDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMoveManager)
CMoveManager::CMoveManager()
{

}

CMoveManager::~CMoveManager()
{
	//PTRLISTPOS pos = NULL ;

	//DWORD* pObjectID = NULL ;

	//pos = m_AutoTargetList.GetHeadPosition() ;						// ����Ʈ�� ��� ����.

	//while(pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos))
	//{
	//	m_AutoTargetList.Remove(pObjectID) ;

	//	delete pObjectID ;

	//	pObjectID = NULL ;
	//}

	//m_AutoTargetList.RemoveAll() ;

	DWORD* pObjectID ;

	PTRLISTPOS pos = NULL ;
	pos = m_AutoTargetList.GetHeadPosition() ;

	while(pos)
	{
		pObjectID = NULL ;
		pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos) ;

		if( pObjectID)
		{
			m_AutoTargetList.Remove(pObjectID) ;
			delete pObjectID ;
		}

	}

	m_AutoTargetList.RemoveAll() ;
}

void CMoveManager::InitMove(CObject* pObject,BASEMOVE_INFO* pMoveInfo,float AngleDeg)
{
	pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
	pObject->m_RotateInfo.bRotating = FALSE;

	if(pMoveInfo)
	{
		memcpy(&pObject->m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
		if(pMoveInfo->bMoving)
		{
			//StartMove(pObject,gCurTime,&pMoveInfo->TargetPosition);
			StartMoveEx(pObject,NULL, gCurTime);
		}
	}
	else
	{
		memset(&pObject->m_MoveInfo,0,sizeof(BASEMOVE_INFO));
	}
	pObject->m_MoveInfo.m_bLastMoving = FALSE;

	pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

void CMoveManager::SetPosition(CObject* pObject,VECTOR3* pPos)
{
	//if(pObject->GetObjectKind() == eObjectKind_Pet)
		//printf("MOVEMGR_SetPosition: %d	%f %f \n", pObject->GetID(), pPos->x, pPos->z);
	// Move_bMoving�� TRUE �� �ƴҶ����� ����.. ASSERT(Move_bMoving == FALSE); �ɾ����
	//YH2DO
	//ASSERT(pObject->m_MoveInfo.bMoving == FALSE);
	pObject->m_MoveInfo.CurPosition = *pPos;
	pObject->SetPosition(pPos);
}

void CMoveManager::EndYMove(CObject* pObject)
{
	pObject->m_bIsYMoving = FALSE;
	pObject->GetEngineObject()->ApplyHeightField(TRUE);
}

void CMoveManager::EndMove(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Move)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move);

	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	pObject->m_MoveInfo.bMoving = FALSE;
	pObject->m_MoveInfo.InitTargetPosition();

	// 071008 ����
	// ������� ��� ������ �뺸���༭ ���¸� �����ϵ��� �ؾ��Ѵ�. �׷��� ������ �ٸ� �׸���� �̵��� ���, �ٸ� �÷��̾
	// ������� ���¸� ���������� ���� ���Ѵ�(������ �ִµ� �ٰ� �ִ� ���� ������ ����)
	
	//--> KES �������� ó���� ���� ������. �ƴϸ� ���¸� ������ �����ϵ��� �ϴ� �ٸ� ��Ŷ�� �ִ°� ���. 
	//    �ֳ��ϸ� HeroMoveStop�� ȣ���ϸ�, �׸����� ��� �������� ��ε�ĳ������ �ϰԵȴ�. ��Ʈ��ũ ���ϰ� ����.

	if( pObject == HERO )
	{
		if( pObject->m_MoveInfo.KyungGongIdx )
		{
			ToggleHeroKyungGongMode();	//<-- �̾ȿ� HeroMoveStop()�� ����ִ�.
		}
		else
		{
			HeroMoveStop();
		}
	}
}

// �κ� Ÿ�� ������Ʈ 
int CMoveManager::TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos)
{
	//VECTOR3 * TargetPos = &pMoveInfo->TargetPositions[pMoveInfo->CurTargetPositionIdx];
	// Ÿ�� �Ÿ��� ��
	float dx = TargetPos->x - pMoveInfo->CurPosition.x;
	float dz = TargetPos->z - pMoveInfo->CurPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return 1;
	}

	float Speed = pObject->GetMoveSpeed();
	float ttt = Speed / Distance;

	pMoveInfo->Move_StartTime			= gCurTime;
	pMoveInfo->Move_StartPosition		= pMoveInfo->CurPosition;
	pMoveInfo->Move_Direction.x			= dx*ttt;
	pMoveInfo->Move_Direction.y			= 0;
	pMoveInfo->Move_Direction.z			= dz*ttt;
	pMoveInfo->Move_EstimateMoveTime	= Speed==0 ? 999999999 : Distance / Speed;

//	if(pObject->GetObjectKind() == eObjectKind_Pet)
//		printf("[PET]%5.2d\t%5.2d\n", gCurTime,pMoveInfo->Move_EstimateMoveTime);
//	if(pObject == HERO)
//		printf("[HERO]%5.2d\t%5.2d\n", gCurTime,pMoveInfo->Move_EstimateMoveTime);

	return 0;
}
void CMoveManager::StartMoveEx(CObject* pObject, VECTOR3* pStartPos ,DWORD CurTime)
{
	//YH ���� Ƣ�°Ͷ����� �ּ�ó����
	//CalcPositionEx(pObject, CurTime);

	if( !	pObject->GetMoveSpeed() &&
			IsMoving(pObject) )
	{
		EndMove(pObject);
		return;
	}

	ASSERT( pObject->m_MoveInfo.GetCurTargetPosIdx() <= pObject->m_MoveInfo.GetMaxTargetPosIdx() );	//0000
	StartMoveEx(pObject, CurTime, pObject->m_MoveInfo.GetTargetPosition(pObject->m_MoveInfo.GetCurTargetPosIdx()));
}


void CMoveManager::StartMoveEx(CObject* pObject, DWORD CurTime, VECTOR3 * pTargetPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	if(pObject->GetState() == eObjectState_Die)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}

	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	pMoveInfo->bMoving = TRUE;
	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	
	if(TargetUpdate(pObject, pMoveInfo, pTargetPos) != 0)
	{
		return;
	}
	
	float TargetAngle = RADTODEG(VECTORTORAD(pMoveInfo->Move_Direction));
	pObject->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pObject->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pObject->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pObject->m_RotateInfo.Rotate_StartTime = CurTime;
	pObject->m_RotateInfo.bRotating = TRUE;

	// 071122 ����. ��ų ������ ������ ���� ä�� �̵��Ǵ� ��찡 �ִ�. �̶� �̵� ��������
	//	�ٷ� ������� ������, �̲�������/�߸��� �������� �ٱ� ���� ��Ÿ����.StartMoveEx()��
	//	�����Ѵٴ� ���� �̹� �ش� ������Ʈ�� �̵��� ���εǾ��ٴ� ���̹Ƿ� ����� �������ش�.
	if( pObject->GetState() == eObjectState_SkillSyn )
	{
		pObject->SetState( eObjectState_Move );
	}
	else if( pObject->GetState() == eObjectState_Move )
	{
		pObject->SetMotionInState(eObjectState_Move);
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));
	}
	else
	{
		if(OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Move) == TRUE)
			OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));		
	}

	
	/*
	char temp[256] = {0,};
	if( pObject->GetObjectKind() == eObjectKind_Player)
	{
		sprintf(temp, "Player MoveRemain Time: %d\n", pMoveInfo->Move_EstimateMoveTime*1000);
	}
	if( pObject->GetObjectKind() == eObjectKind_Pet)
	{
		sprintf(temp, "Pet MoveRemain Time: %d\n", pMoveInfo->Move_EstimateMoveTime*1000);
	}
	printf(temp);*/
/*
	if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->GetPet() )
	{		
		MOVE_INFO* pminfo = &((CPlayer*)pObject)->GetPet()->m_MoveInfo;
		pminfo->SetTargetPosition( pObject->m_MoveInfo.GetTargetPositionArray() );
		
		VECTOR3 vPos = ((CPlayer*)pObject)->GetPet()->GetCurPosition();
		VECTOR3 tgPos;
		
		pminfo->SetCurTargetPosIdx( pObject->m_MoveInfo.GetCurTargetPosIdx() );

		if( pObject->m_MoveInfo.GetCurTargetPosIdx() == 0 )
		{			
			pminfo->SetMaxTargetPosIdx( pObject->m_MoveInfo.GetMaxTargetPosIdx() );
			
			for(int r=0; r<MAX_CHARTARGETPOSBUF_SIZE; r++)
			{
				if( pObject->m_MoveInfo.GetTargetPosition(r)->x || 
					pObject->m_MoveInfo.GetTargetPosition(r)->y || 
					pObject->m_MoveInfo.GetTargetPosition(r)->z )
				{
					VECTOR3 tp;
					VECTOR3 tPos = *pObject->m_MoveInfo.GetTargetPosition(r);
					tp = tPos;
					tp = tp - vPos;
					Normalize( &tp, &tp );
					tp = tp * 150;
					
					
					VECTOR3 tt = tPos - tp;
					
					printf("[%d]player - x:%6.2f y:%6.2f z:%6.2f\n", r, tPos.x, tPos.y, tPos.z);
					printf("[%d]pet    - x:%6.2f y:%6.2f z:%6.2f\n", r, tt.x, tt.y, tt.z);
					
					pminfo->SetTargetPosition( r, tt );
					
					vPos = tPos;
					tgPos = tt;
				}
				else
					break;
			}
			printf("\n");
		}

		StartMoveEx(((CPlayer*)pObject)->GetPet(), CurTime, &tgPos);
	}
*/
}

#define MOVESKIPCOUNT2	1
BOOL CMoveManager::CalcPositionEx(CObject* pObject,DWORD CurTime)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	
	if(pMoveInfo->bMoving)
	{
		if(IsGameSlow() == TRUE && pObject != HERO)
		{
			if(pObject->m_bMoveSkipCount < MOVESKIPCOUNT2)
			{
				++pObject->m_bMoveSkipCount;
				return FALSE;
			}
			else
				pObject->m_bMoveSkipCount = 0;
		}

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);

		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			// ���� Ÿ������ ����

			//---KES FindPatch Fix 071020
			//if( pMoveInfo->GetMaxTargetPosIdx() > 0 && pMoveInfo->GetCurTargetPosIdx() < pMoveInfo->GetMaxTargetPosIdx()- 1)
			if(pMoveInfo->GetCurTargetPosIdx() < pMoveInfo->GetMaxTargetPosIdx()- 1)	//������ ��찡 ���� �� ���� �� ������...
			//------------------------
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				pMoveInfo->SetCurTargetPosIdx(pMoveInfo->GetCurTargetPosIdx()+1);
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				StartMoveEx(pObject, CurTime, pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx()));
			}
			else
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				bVal = TRUE;
			}
		}
		else
		{
			pMoveInfo->CurPosition.x = pMoveInfo->Move_StartPosition.x + pMoveInfo->Move_Direction.x * fMoveTime;
			pMoveInfo->CurPosition.z = pMoveInfo->Move_StartPosition.z + pMoveInfo->Move_Direction.z * fMoveTime;
		}

		//---KES FindPatch Fix 071020
		//---(0,0) force block
		//pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
		if( pObject->m_MoveInfo.CurPosition.x != 0 && pObject->m_MoveInfo.CurPosition.z != 0 )
		{
			pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
		}
		else
		{
			ASSERT(0);
		}
		//----------------------------------
        
		return bVal;
	}


	return FALSE;
}
/*

// �̵����� ���·� ǥ���ϰ� ���������� �������� ����س��´�.
// �̵����۽ð��� ����س��´�.
// ������ġ�� ����غ��� StartPos �� ���̰� ���� ���� ��ġ-_-;�� ���Ѵ�.
void CMoveManager::StartMove(CObject* pObject,DWORD CurTime,VECTOR3* TargetPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;	
	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}


	pMoveInfo->bMoving = TRUE;

	float Speed = pObject->GetMoveSpeed();
	
	pMoveInfo->Move_StartTime = CurTime;

	// �������� Ÿ�� ó��
	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->TargetPosition.x = TargetPos->x;
	pMoveInfo->TargetPosition.z = TargetPos->z;
	pMoveInfo->Move_StartPosition.y = 0;
	pMoveInfo->TargetPosition.y = 0;

	// Ÿ�� �Ÿ��� ��
	float dx = pMoveInfo->TargetPosition.x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->TargetPosition.z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}
	float ttt = Speed / Distance;

	// ���� Ÿ���� ���� ���� ( �迭Ȥ�� �Ҷ����� ����? )
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	// ���� �ð� ����
	pMoveInfo->Move_EstimateMoveTime = Distance / Speed;
	
	pObject->m_MoveInfo.CurPosition.y = 0;
	float TargetAngle = RADTODEG(VECTORTORAD(pMoveInfo->Move_Direction));
	pObject->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pObject->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pObject->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pObject->m_RotateInfo.Rotate_StartTime = CurTime;
	pObject->m_RotateInfo.bRotating = TRUE;

	if(pObject->GetState() != eObjectState_Move)
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Move);
	OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));
}

// ���� ��ġ�� ����ؼ� Move_Position�� �����ϰ� �Ѱ��ش�.
// ����ؼ� ������� �ʰ� �ʿ��Ҷ��� ����Ѵ�.
// Move_bMoving�� FALSE�̸� �ٷ� �����Ѵ�.
// �ð� ������ ���� OverFlow�� ����ؾ� �Ѵ�.
#define MOVESKIPCOUNT	1
BOOL CMoveManager::CalcPosition(CObject* pObject,DWORD CurTime)
{
	if(pObject->m_MoveInfo.bMoving)
	{
		if(IsGameSlow() == TRUE)
		{
			if(pObject->m_bMoveSkipCount < MOVESKIPCOUNT)
			{
				++pObject->m_bMoveSkipCount;
				return FALSE;
			}
			else
				pObject->m_bMoveSkipCount = 0;
		}

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pObject->m_MoveInfo.Move_StartTime)*0.001f);
		if(pObject->m_MoveInfo.Move_EstimateMoveTime < fMoveTime)
		{
			pObject->m_MoveInfo.CurPosition.x = pObject->m_MoveInfo.TargetPosition.x;
			pObject->m_MoveInfo.CurPosition.z = pObject->m_MoveInfo.TargetPosition.z;
			bVal = TRUE;
		}
		else
		{
			pObject->m_MoveInfo.CurPosition.x = pObject->m_MoveInfo.Move_StartPosition.x + 
				pObject->m_MoveInfo.Move_Direction.x * fMoveTime;
			pObject->m_MoveInfo.CurPosition.z = pObject->m_MoveInfo.Move_StartPosition.z + 
				pObject->m_MoveInfo.Move_Direction.z * fMoveTime;
		}
		
		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
		return bVal;
	}

	return FALSE;
}
*/
BOOL CMoveManager::CalcYPosition(CObject* pObject,DWORD CurTime)
{
	if(pObject->m_bIsYMoving)
	{
		MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);
		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			pMoveInfo->CurPosition.y = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y;
			bVal = TRUE;
		}
		else
		{
			pMoveInfo->CurPosition.y = pMoveInfo->Move_StartPosition.y + pMoveInfo->Move_Direction.y * fMoveTime;
		}
		
		pObject->SetPosition(&pMoveInfo->CurPosition);

		return bVal;
	}

	return FALSE;
}

void CMoveManager::CalcAngle(CObject* pObject,DWORD CurTime)
{	
	if(pObject->m_RotateInfo.bRotating)
	{
		float fRotateTime = ((CurTime - pObject->m_RotateInfo.Rotate_StartTime)*0.001f);
		if(fRotateTime >= pObject->m_RotateInfo.EstimatedRotateTime)
		{
			pObject->m_RotateInfo.Angle.Process(1);
			pObject->m_RotateInfo.bRotating = FALSE;
		}
		else
		{
			pObject->m_RotateInfo.Angle.Process(fRotateTime / pObject->m_RotateInfo.EstimatedRotateTime);
		}
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
}

//void CMoveManager::MoveProcess(CObject* pObject)
//{
//	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
//	if(pObject->m_bIsYMoving)
//	{
//		if(CalcYPosition(pObject,gCurTime) == TRUE)
//			EndYMove(pObject);
//	}
//	else
//	{
//		if(CalcPositionEx(pObject,gCurTime) == TRUE)
//			EndMove(pObject);
//	}
//	CalcAngle(pObject,gCurTime);
//	pMoveInfo->m_bLastMoving = pMoveInfo->bMoving;
//
//}

void CMoveManager::MoveProcess(CObject* pObject)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		if(CalcYPosition(pObject,gCurTime) == TRUE)
			EndYMove(pObject);
	}
	else
	{
		if(CalcPositionEx(pObject,gCurTime) == TRUE)
			EndMove(pObject);
	}
	CalcAngle(pObject,gCurTime);
	pMoveInfo->m_bLastMoving = pMoveInfo->bMoving;

	UpdateAutoTarget(pObject) ;																		// Ÿ�ٸ���Ʈ�� ������Ʈ �Ѵ�.
}

void CMoveManager::UpdateAutoTarget(CObject* pObject)
{
	if( pObject )																					// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
	{
		if( pObject->GetObjectKind() == eObjectKind_Monster )										// ������Ʈ Ÿ���� üũ�Ѵ�.
		{
			float fDistance = 0.0f ;																// HERO���� �Ÿ��� ���� ������ �����Ѵ�.

			VECTOR3 vHeroPos;																		// HERO�� ��ġ�� ���� ���͸� �����Ѵ�.
			HERO->GetPosition(&vHeroPos) ;															// HERO�� ��ġ�� �޴´�.
			fDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;				// HERO�� ������ �Ÿ��� ���Ѵ�.

			SortAutoTargetList(pObject->GetID(), fDistance) ;										// ������Ʈ ������ �����Ͽ� �ڵ� Ÿ�� ����Ʈ�� �����Ѵ�.
		}
		else																						// ������Ʈ �� ���� Ÿ���� �ƴ϶��,
		{
			// �׻� ����Ʈ�� üũ�ؾ��ϱ� ������(�ڵ�Ÿ�� ������ ���������) ������ �Ѵ�.
			SortAutoTargetList(0, 0.0f) ;															
		}
	}
}

void CMoveManager::SortAutoTargetList(DWORD dwObjectID, float fDistance)
{
	//ZeroMemory(sTempList, sizeof(DISTANCE_ID)*6) ;													// ������Ʈ�� ���̵�� �Ÿ��� ���� ����ü�� �ʱ�ȭ �Ѵ�.

	PTRLISTPOS pos	 = NULL ;																		// ����Ʈ���� ��ġ������ ���� �����͸� �����Ѵ�.
	DWORD* pObjectID = NULL ;																		// ������Ʈ ���̵� ���� �����͸� �����Ѵ�.
	CObject* pObject = NULL ;																		// ������Ʈ ������ ���� �����͸� �����Ѵ�.

	BOOL bSameID = FALSE ;																			// ����Ʈ�� ���� ���̵� �����ϴ��� ���θ� ��� ���� ����.

	pos = m_AutoTargetList.GetHeadPosition() ;

	while(pos)
	{
		pObjectID = NULL ;
		pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos) ;

		if( pObjectID )
		{
			pObject = OBJECTMGR->GetObject(*pObjectID) ;

			if( pObject )
			{
				if( pObject->GetState() == eObjectState_Die )
				{
					m_AutoTargetList.Remove(pObjectID) ;
					delete pObjectID ;
				}
			}
		}
	}

	if( m_AutoTargetList.GetCount() > 5 )
	{
		int nDeleteCount = 0 ;
		pos = m_AutoTargetList.GetHeadPosition() ;

		while(pos)
		{
			++nDeleteCount ;

			pObjectID = NULL ;
			pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos) ;

			if( nDeleteCount > 5 )
			{
				if( pObjectID )
				{
					m_AutoTargetList.Remove(pObjectID) ;
					delete pObjectID ;
				}
			}
		}
	}

	if( dwObjectID != 0 )																			// ���̵� �˻��Ѵ�.
	{
		//pos = m_AutoTargetList.GetHeadPosition() ;

		//while(pos)
		//{
		//	pObjectID = NULL ;
		//	pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos) ;

		//	if( pObjectID )
		//	{
		//		if( *pObjectID == dwObjectID )
		//		{
		//			bSameID = TRUE ;
		//			break ;
		//		}
		//	}
		//}

		//if( !bSameID )																				// ���� ���̵� ������,
		//{
		//	if( fDistance > AUTOTARGET_RANGE )														// HERO�� ������ �Ÿ��� �ڵ�Ÿ�� ���� ���̸�,
		//	{
		//		return ;
		//	}

		//	int nListCount = m_AutoTargetList.GetCount() ;										// �ڵ� Ÿ���� ������ ������ ���Ѵ�.

		//	if( nListCount > 4 )																// ����Ʈ ī��Ʈ�� 5���� ������,
		//	{
		//		pos = m_AutoTargetList.GetHeadPosition() ;

		//		while(pos)
		//		{
		//			pObjectID = NULL ;
		//			pObjectID = (DWORD*)m_AutoTargetList.GetNext(pos) ;

		//			if( pObjectID )
		//			{
		//				m_AutoTargetList.Remove(pObjectID) ;
		//				delete pObjectID ;

		//				DWORD* pNewObjectID = new DWORD ;													// ���̵� ���� �޸𸮸� �Ҵ��Ѵ�.
		//				*pNewObjectID = dwObjectID ;														// ���̵� �����Ѵ�.
		//				m_AutoTargetList.AddTail(pNewObjectID) ;											// �ڵ�Ÿ�� ����Ʈ�� ���̵� �߰��Ѵ�.
		//			}
		//		}
		//	}
		//	else
		//	{
		//		DWORD* pNewObjectID = new DWORD ;													// ���̵� ���� �޸𸮸� �Ҵ��Ѵ�.
		//		*pNewObjectID = dwObjectID ;														// ���̵� �����Ѵ�.
		//		m_AutoTargetList.AddTail(pNewObjectID) ;											// �ڵ�Ÿ�� ����Ʈ�� ���̵� �߰��Ѵ�.
		//	}
		//}

 
		for( int count = 0 ; count < 5 ;++count )													// ����Ʈ�� ������ ����ŭ ������ ������.
		{
			pos = m_AutoTargetList.FindIndex(count) ;												// ī��Ʈ�� ���� ��ġ ������ �޴´�.

			if( pos )																				// ��ġ ������ ��ȿ���� üũ�Ѵ�.
			{
				pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;									// ��ü�� �ش��ϴ� ������Ʈ ���̵� �޴´�.

				if( pObjectID )																		// ���̵� ������ ��ȿ���� üũ�Ѵ�.
				{
					DWORD dwCompareID = *pObjectID ;												// ���̵� �޴´�.

					if( dwCompareID == dwObjectID )													// ����Ʈ�� ������Ʈ�� ���ڷ� �Ѿ�� ������Ʈ ���̵� ���ٸ�,
					{
						bSameID = TRUE ;															// ����Ʈ���� ���� ���̵� �ִٰ� �����Ѵ�.
					}
				}
			}
		}

		if( !bSameID )																				// ���� ���̵� ������,
		{
			if( fDistance <= AUTOTARGET_RANGE )														// HERO�� ������ �Ÿ��� �ڵ�Ÿ�� ���� ���̸�,
			{
				int nListCount = m_AutoTargetList.GetCount() ;										// �ڵ� Ÿ���� ������ ������ ���Ѵ�.

				if( nListCount > 5 )																// ����Ʈ ī��Ʈ�� 5���� ������,
				{
					for( int count = nListCount ; count < nListCount-5 ; --count )					// ī��Ʈ�� ����Ʈ ī��Ʈ�� �����ϰ�, ����Ʈ ī��Ʈ -5 �Ѹ�ŭ for���� ������.
					{
                        PTRLISTPOS pos = m_AutoTargetList.FindIndex(count-1) ;						// ���� ī��Ʈ-1�� ��ġ�� �޴´�.
						if( pos )																	// ��ġ ������ ��ȿ���� üũ�Ѵ�.
						{
							m_AutoTargetList.RemoveAt(pos) ;										// ��ġ�� �؈��ϴ� ������ �����.
						}
					}

					PTRLISTPOS firstPos = m_AutoTargetList.FindIndex(0) ;							// ����Ʈ�� ù ��ġ�� �޴´�.

					if( firstPos )																	// ù ��ġ�� ��ȿ�ϸ�,
					{
						m_AutoTargetList.RemoveAt(firstPos) ;										// ù ��ġ�� ������ �����Ѵ�.
					}
				}

				DWORD* pNewObjectID = new DWORD ;													// ���̵� ���� �޸𸮸� �Ҵ��Ѵ�.
				*pNewObjectID = dwObjectID ;														// ���̵� �����Ѵ�.
				m_AutoTargetList.AddTail(pNewObjectID) ;											// �ڵ�Ÿ�� ����Ʈ�� ���̵� �߰��Ѵ�.
			}
		}
	}
}

void CMoveManager::SwapTargetList(DISTANCE_ID* p1, DISTANCE_ID* p2)
{
	DISTANCE_ID pTemp ;																				// �ӽ÷� ���̵�� �Ÿ��� ���� ����ü�� �����Ѵ�.

	memcpy(&pTemp, p2, sizeof(DISTANCE_ID)) ;														// �ӽ� ����ü�� �ι�° ������ �����Ѵ�.
	memcpy(p2, p1, sizeof(DISTANCE_ID)) ;															// �ι�° ����ü�� ù��° ������ �����Ѵ�.
	memcpy(p1, &pTemp, sizeof(DISTANCE_ID)) ;														// ù���� ����ü�� �ӽ� ������ �����Ѵ�.
}

//void CMoveManager::UpdateAutoTarget(CObject* pObject)
//{
//	if( pObject )
//	{
//		if( pObject->GetObjectKind() == eObjectKind_Monster )					// ������Ʈ Ÿ���� ���Ͷ��,
//		{
//			float fDistance = 0.0f ;											// HERO�� ������ �Ÿ��� ���� ������ �����Ѵ�.
//
//			float fTargetDistance = 1000.0f ;
//
//			VECTOR3 vHeroPos;
//			HERO->GetPosition(&vHeroPos) ;
//			fDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;// HERO�� ������ �Ÿ��� ���Ѵ�.
//
//			if( fDistance <= fTargetDistance )									// �Ÿ��� �ڵ� Ÿ�� ���� �����,
//			{
//				int nMonsterCount = m_AutoTargetList.GetCount() ;				// ����Ʈ�� ������ �޴´�.
//
//				if( nMonsterCount >= 5 )										// ������ ���� �ټ��� �̻��̸�,
//				{
//					SortAutoTargetList(pObject->GetID(), fDistance) ;			// ����Ʈ�� ���� �� ������Ʈ �Ѵ�.
//				}
//				else															// ������ ���� �ټ��� �̸��̸�,
//				{
//					if( m_AutoTargetList.GetCount() == 0 )
//					{
//						DWORD* pNewID = new DWORD ;
//						*pNewID = pObject->GetID() ;
//						m_AutoTargetList.AddTail(pNewID) ;							// ����Ʈ�� ������Ʈ ���̵� �߰��Ѵ�.
//					}
//					else
//					{
//						PTRLISTPOS pos = NULL ;
//
//						DWORD* pObjectID = NULL ;
//						DWORD dwCurID = pObject->GetID() ;
//
//						pos = m_AutoTargetList.GetHeadPosition() ;
//
//						BOOL bSameID = FALSE ;
//
//						while(pos)
//						{
//							pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//							if( pObjectID )
//							{
//								if( *pObjectID == dwCurID )
//								{
//									bSameID = TRUE ;
//								}
//							}
//
//							pObjectID = NULL ;
//
//							m_AutoTargetList.GetNext(pos) ;
//						}
//
//						if( !bSameID )
//						{
//							DWORD* pNewID = new DWORD ;
//							*pNewID = dwCurID ;
//							m_AutoTargetList.AddTail(pNewID) ;							// ����Ʈ�� ������Ʈ ���̵� �߰��Ѵ�.
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void CMoveManager::SortAutoTargetList(DWORD dwObjectID, float fDistance)
//{
//	int nTempListCount = 0 ;
//
//	ZeroMemory(sTempList, sizeof(DISTANCE_ID)*6) ;
//
//	float fOldDistance = 0.0f ;
//
//	float fTargetDistance = 1000.0f ;
//
//	DWORD* pObjectID = NULL ;
//
//	CObject* pObject = NULL ;
//
//	PTRLISTPOS pos = NULL ;
//
//	// �ڵ� Ÿ�� ����Ʈ ������Ʈ.
//	pos = m_AutoTargetList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//		if( pObjectID )
//		{
//			pObject = (CObject*)OBJECTMGR->GetObject(*pObjectID) ;
//
//			if( pObject )
//			{
//				VECTOR3 vHeroPos;
//				HERO->GetPosition(&vHeroPos) ;
//
//				fOldDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;
//
//				if( fOldDistance <= fTargetDistance )
//				{
//					sTempList[nTempListCount].dwID = pObject->GetID() ;
//					sTempList[nTempListCount].fDistance = fOldDistance ;
//
//					++nTempListCount ;
//				}
//			}
//		}
//
//		pObjectID = NULL ;
//
//		m_AutoTargetList.GetNext(pos) ;
//	}
//
//	pos = m_AutoTargetList.GetHeadPosition() ;
//
//	DWORD* pDeleteID = NULL ;
//	while(pos)
//	{
//		pDeleteID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//		if( pDeleteID )
//		{
//			delete pDeleteID ;
//		}
//
//		pDeleteID = NULL ;
//
//		m_AutoTargetList.GetNext(pos) ;
//	}
//
//	m_AutoTargetList.RemoveAll() ;
//
//	// ���� ���� ���� ������ �����Ѵ�.
//	if( nTempListCount < 5 )
//	{
//		sTempList[nTempListCount].dwID = dwObjectID ;
//		sTempList[nTempListCount].fDistance = fDistance ;
//	}
//	else
//	{
//		sTempList[5].dwID = dwObjectID ;
//		sTempList[5].fDistance = fDistance ;
//	}
//
//	// ���� ���� ���Ϳ� �Ÿ� �񱳸� �ؼ� �����Ѵ�.
//	int counter = 0 ;
//	DWORD dwCompareID = 0 ;
//	float fCompareDistance = 0.0f ;
//
//	for( int count = 1 ; count < nTempListCount+1 ; ++count )
//	{
//		dwCompareID = sTempList[count].dwID ;
//		fCompareDistance = sTempList[count].fDistance ;
//
//		for( int count2 = count-1 ; (count2 > 0) && (sTempList[count2].fDistance > fCompareDistance) ; --count2, ++counter )
//		{
//			sTempList[count2+1].dwID = sTempList[count2].dwID ;
//			sTempList[count2+1].fDistance = sTempList[count2].fDistance ;
//		}
//
//		sTempList[count2+1].dwID = dwCompareID ;
//		sTempList[count2+1].fDistance = fCompareDistance ;
//	}
//
//	// ����Ʈ�� �ٽ� ������ �����Ѵ�.
//	pos = m_AutoTargetList.GetHeadPosition() ;
//	for( int count3 = 0 ; count3 < 5 ; ++count3 )
//	{
//		if( sTempList[count3].dwID != 0 )
//		{
//			DWORD* pNewID = new DWORD ;
//			*pNewID = sTempList[count3].dwID ;
//			m_AutoTargetList.AddTail(pNewID) ;
//		}
//	}
//}
//
//void CMoveManager::RefreshTargetList()
//{
//	/*DWORD* pObjectID = NULL ;
//
//	CObject* pObject = NULL ;
//
//	PTRLISTPOS pos = NULL ;
//
//	float fOldDistance = 0.0f ;
//
//	float fTargetDistance = 1000.0f ;*/
//
//	// �ڵ� Ÿ�� ����Ʈ ������Ʈ.
//	/*pos = m_AutoTargetList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//		if( pObjectID )
//		{
//			pObject = (CObject*)OBJECTMGR->GetObject(*pObjectID) ;
//
//			if( pObject )
//			{
//				VECTOR3 vHeroPos;
//				HERO->GetPosition(&vHeroPos) ;
//
//				fOldDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;
//
//				if( fOldDistance > fTargetDistance )
//				{
//					m_AutoTargetList.Remove(pObjectID) ;
//
//					delete pObjectID ;
//				}
//			}
//		}
//
//		pObjectID = NULL ;
//
//		pos = NULL ;
//
//		m_AutoTargetList.GetNext(pos) ;
//	}*/
//
//	/*int nListCount = m_AutoTargetList.GetCount() ;
//
//	for( int count = 0 ; count < nListCount ; ++count )
//	{
//		pos = m_AutoTargetList.FindIndex(count) ;
//
//		if( pos )
//		{
//			pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//			if( pObjectID )
//			{
//				pObject = OBJECTMGR->GetObject(*pObjectID) ;
//
//				if( pObject )
//				{
//					VECTOR3 vHeroPos;
//					HERO->GetPosition(&vHeroPos) ;
//
//					fOldDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;
//
//					if( fOldDistance > fTargetDistance )
//					{
//						m_AutoTargetList.Remove(pObjectID) ;
//
//						delete pObjectID ;
//					}
//				}
//			}
//		}
//	}*/
//
//	int nTempListCount = 0 ;
//
//	ZeroMemory(sTempList, sizeof(DISTANCE_ID)*6) ;
//
//	float fOldDistance = 0.0f ;
//
//	float fTargetDistance = 1000.0f ;
//
//	DWORD* pObjectID = NULL ;
//
//	CObject* pObject = NULL ;
//
//	PTRLISTPOS pos = NULL ;
//
//	// �ڵ� Ÿ�� ����Ʈ ������Ʈ.
//	/*pos = m_AutoTargetList.GetHeadPosition() ;
//
//	while(pos)*/
//	int nListCount = m_AutoTargetList.GetCount() ;
//
//	for( int count = 0 ; count < nListCount ; ++count )
//	{
//		pos = m_AutoTargetList.FindIndex(count) ;
//
//		if( pos )
//		{
//			pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;
//
//			if( pObjectID )
//			{
//				pObject = (CObject*)OBJECTMGR->GetObject(*pObjectID) ;
//
//				if( pObject )
//				{
//					if( pObject->GetState() == eObjectState_Die )
//					{
//						m_AutoTargetList.Remove(pObjectID) ;
//
//						delete pObjectID ;
//
//						if( *pObjectID == OBJECTMGR->GetSelectedObjectID() )
//						{
//							OBJECTMGR->SetSelectedObject(NULL) ;
//						}
//
//						continue ;
//					}
//
//					VECTOR3 vHeroPos;
//					HERO->GetPosition(&vHeroPos) ;
//
//					fOldDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;
//
//					if( fOldDistance <= fTargetDistance )
//					{
//						sTempList[nTempListCount].dwID = pObject->GetID() ;
//						sTempList[nTempListCount].fDistance = fOldDistance ;
//
//						++nTempListCount ;
//					}
//					else
//					{
//						m_AutoTargetList.Remove(pObjectID) ;
//
//						delete pObjectID ;
//
//						if( *pObjectID == OBJECTMGR->GetSelectedObjectID() )
//						{
//							OBJECTMGR->SetSelectedObject(NULL) ;
//						}
//					}
//				}
//			}
//
//			pObjectID = NULL ;
//		}
//	}
//
//	
//	m_AutoTargetList.RemoveAll() ;
//
//	// ���� ���� ���� ������ �����Ѵ�.
//	/*if( nTempListCount < 5 )
//	{
//		sTempList[nTempListCount].dwID = dwObjectID ;
//		sTempList[nTempListCount].fDistance = fDistance ;
//	}
//	else
//	{
//		sTempList[5].dwID = dwObjectID ;
//		sTempList[5].fDistance = fDistance ;
//	}*/
//
//	// ���� ���� ���Ϳ� �Ÿ� �񱳸� �ؼ� �����Ѵ�.
//	int counter = 0 ;
//	DWORD dwCompareID = 0 ;
//	float fCompareDistance = 0.0f ;
//
//	//for( int count = 1 ; count < nTempListCount ; ++count )
//	//{
//	//	dwCompareID = sTempList[count].dwID ;
//	//	fCompareDistance = sTempList[count].fDistance ;
//
//	//	for( int count2 = count-1 ; (count2 > 0) && (sTempList[count2].fDistance > fCompareDistance) ; --count2, ++counter )
//	//	{
//	//		sTempList[count2+1].dwID = sTempList[count2].dwID ;
//	//		sTempList[count2+1].fDistance = sTempList[count2].fDistance ;
//	//	}
//
//	//	sTempList[count2+1].dwID = dwCompareID ;
//	//	sTempList[count2+1].fDistance = fCompareDistance ;
//	//}
//
//
//
//	// ����Ʈ�� �ٽ� ������ �����Ѵ�.
//	pos = m_AutoTargetList.GetHeadPosition() ;
//	for( int count3 = 0 ; count3 < 5 ; ++count3 )
//	{
//		if( sTempList[count3].dwID != 0 )
//		{
//			DWORD* pNewID = new DWORD ;
//			*pNewID = sTempList[count3].dwID ;
//			m_AutoTargetList.AddTail(pNewID) ;
//		}
//	}
//}

void CMoveManager::SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay)
{
	CHero* pHero = OBJECTMGR->GetHero();

/*	if( pHero->GetStatus( STATUSKIND_STUN ) || pHero->GetStatus( STATUSKIND_TIEUP ) )
	{
		return;
	}
*/
	if( pHero->GetAbnormalStatus()->IsStun ||
		pHero->GetAbnormalStatus()->IsFreezing ||
		pHero->GetAbnormalStatus()->IsStone ||
		pHero->GetAbnormalStatus()->IsSlip ||
		pHero->GetAbnormalStatus()->IsParalysis )
	{
		return;
	}
/*
	if( pHero->GetState() == eObjectState_SkillStart ||
		pHero->GetState() == eObjectState_SkillSyn ||	
		pHero->GetState() == eObjectState_SkillBinding ||
		pHero->GetState() == eObjectState_SkillUsing ||
		pHero->GetState() == eObjectState_SkillDelay )
	{
		return;
	}
*/
	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanMove &&
		pHero->GetState() != eObjectState_Immortal  )
	{
		if( pHero->GetState() == eObjectState_Society )
		{
			if( pHero->EndSocietyAct() == FALSE )
			{
				CAction act;
				act.InitMoveAction(pPos);
				pHero->SetNextAction(&act);
				pHero->DisableAutoAttack();
				return;
			}
		}
		else
		{
			CAction act;
			act.InitMoveAction(pPos);
			pHero->SetNextAction(&act);
			pHero->DisableAutoAttack();
			return;
		}
	}
	if( pHero->GetState() == eObjectState_Move &&
		pHero->GetEngineObject()->GetCurMotion() == pHero->m_ObjectState.State_End_Motion)
		return;

	
	if(IsMoving(pHero))
		CalcPositionEx(pHero,gCurTime);
	
	//////////////////////////////////////////////////////////////////////////
	// 50cm Ÿ���� �߾ӿ� ���� �Ѵ�.
	/*
	pPos->x = int(pPos->x / TileWidth) * TileWidth + TileWidth * 0.5f;
	pPos->z = int(pPos->z / TileWidth) * TileWidth + TileWidth * 0.5f;
	*/
	pPos->x = (int(pPos->x / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	pPos->y = 0;
	pPos->z = (int(pPos->z / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	//////////////////////////////////////////////////////////////////////////
	
	
	pHero->m_MoveInfo.InitTargetPosition();
	switch(PATHMGR->GetPath(&pHero->m_MoveInfo.CurPosition,pPos,pHero->m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, pHero->m_MoveInfo.GetMaxTargetPosIdxRef(), pHero, FALSE, 0))
	{
	case PATH_BLOCKFAILED:
	case PATH_FAILED:
		//__asm int 3;
		return;
	}
	WORD count = pHero->m_MoveInfo.GetMaxTargetPosIdx();

	// ��ã�� �׽�Ʈ ��
	/*
	EFFECTMGR->RemoveAllEffect();
	for(int k = 0 ; k < count ; ++k)
	{
		EFFECTMGR->StartEffectProcess(1, HERO, 0,0,0,EFFECT_FLAG_ABSOLUTEPOS, &pHero->m_MoveInfo.TargetPositions[k]);
	}*/


	if(bMousePointDisplay)
		MOVEPOINT->SetPoint(pPos);

	// jsd 
	if( count == 0 )
		return;
	else if( count == 1 )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos( &pHero->m_MoveInfo.CurPosition );
		msg.SetTargetPos( pHero->m_MoveInfo.GetTargetPosition(0) );

		NETWORK->Send(&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_TARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos(&pHero->m_MoveInfo.CurPosition);
		//msg.tpos.Compress(&TargetPos);
		ASSERT( count <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
		for(int i=0;i<count;++i)
		{
			msg.AddTargetPos(pHero->m_MoveInfo.GetTargetPosition(i));
		}

		NETWORK->Send(&msg,msg.GetSize());
	}

	//StartMove(pHero,gCurTime,&TargetPos);
	StartMoveEx(pHero,NULL,gCurTime);

	// 071204 LUJ �������̽� ��� ���¿��� ó���� �۾� ó��
	MACROMGR->CheckToggleState( pHero );
}

void CMoveManager::SetHeroPetTarget()
{
	CHero* pHero = OBJECTMGR->GetHero();
	CHeroPet* pHeroPet = OBJECTMGR->GetHeroPet();
	VECTOR3 pos;
	
	if( IsMoving( pHero ) )
		CalcPositionEx( pHero, gCurTime );
	if( IsMoving( pHeroPet ) )
		CalcPositionEx( pHeroPet, gCurTime );

	int ran=0;
	float dx=0,dz=0;
	ran = rand();
	dx = float( ran % ( PETMGR->GetAreaDistance() ) ) * ( ran % 2 ? 1 : -1 );
	ran = rand();
	dz = float( ran % ( PETMGR->GetAreaDistance() ) ) * ( ran % 2 ? 1 : -1 );
	pos.x = pHero->m_MoveInfo.CurPosition.x + dx / 2;
	pos.z = pHero->m_MoveInfo.CurPosition.z + dz / 2;

	if( pos.x < 0 )				pos.x = 0;
	else if( pos.x > 51199 )	pos.x = 51199;

	if( pos.z < 0 )				pos.z = 0;
	else if( pos.z > 51199 )	pos.z = 51199;

	pos.x = ((DWORD)(pos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	pos.z = ((DWORD)(pos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);


	MOVE_ONETARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_MOVE;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pHeroPet->GetID();
	msg.SetStartPos( &pHeroPet->m_MoveInfo.CurPosition );
	msg.SetTargetPos( &pos );

	NETWORK->Send( &msg,msg.GetSize() );
}

void CMoveManager::SetHeroPetTarget(VECTOR3* pPos)
{
	CHeroPet* pHeroPet = OBJECTMGR->GetHeroPet();

	if( pHeroPet->GetAbnormalStatus()->IsStun ||
		pHeroPet->GetAbnormalStatus()->IsFreezing ||
		pHeroPet->GetAbnormalStatus()->IsStone ||
		pHeroPet->GetAbnormalStatus()->IsSlip ||
		pHeroPet->GetAbnormalStatus()->IsParalysis )
	{
		return;
	}

	if( pHeroPet->GetState() != eObjectState_None &&
		pHeroPet->GetState() != eObjectState_Move &&
		pHeroPet->GetState() != eObjectState_TiedUp_CanMove &&
		pHeroPet->GetState() != eObjectState_Immortal  )
	{
		CAction act;
		act.InitMoveAction(pPos);
		pHeroPet->SetNextAction(&act);
		pHeroPet->DisableAutoAttack();
		return;
		
	}
	
	if(IsMoving(pHeroPet))
		CalcPositionEx(pHeroPet,gCurTime);
	
	//////////////////////////////////////////////////////////////////////////
	// 50cm Ÿ���� �߾ӿ� ���� �Ѵ�.
	pPos->x = (int(pPos->x / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	pPos->y = 0;
	pPos->z = (int(pPos->z / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	//////////////////////////////////////////////////////////////////////////

	MOVE_ONETARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_ONETARGET;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pHeroPet->GetID();
	msg.SetStartPos( &pHeroPet->m_MoveInfo.CurPosition );
	msg.SetTargetPos( pPos );

	NETWORK->Send(&msg,msg.GetSize());

	StartMoveEx(pHeroPet,gCurTime,pPos);
}

/////////////////////////////////////////////////////////////////////////////////////
/// 06. 08. 2�� ���� - �̿���
/// ���� SetLookatPos �Լ� ������ ���ڿ� �߰� ȸ������ �߰��ߴ�.
void CMoveManager::SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg)
{
	pObject->m_MoveInfo.CurPosition.y = 0;
	VECTOR3 PositionGap = *pPos - pObject->m_MoveInfo.CurPosition;
	float Distance = VECTOR3Length(&PositionGap);
	if(Distance == 0)
		return;
	VECTOR3 TDirection = PositionGap / Distance;
	/// �߰� ȸ������ �����ش�
	float angle = RADTODEG(VECTORTORAD(TDirection)) + AddDeg;
	SetAngle(pObject,angle,TurnDuration);
}
/////////////////////////////////////////////////////////////////////////////////////

void CMoveManager::SetAngle(CObject* pObject,float AngleDeg,float TurnDuration)
{
	if(TurnDuration == 0)
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.bRotating = FALSE;
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
	else
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.EstimatedRotateTime = TurnDuration;
		pObject->m_RotateInfo.Rotate_StartTime = gCurTime;
		pObject->m_RotateInfo.bRotating = TRUE;
	}
}

void CMoveManager::SetHeroActionMove(CActionTarget* pTarget,CAction* pAction)
{
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return;
	SetHeroTarget(pTargetPos,FALSE);

	CHero* pHero = OBJECTMGR->GetHero();
	pHero->SetMovingAction(pAction);
	
	pHero->SetCurComboNum(0);

//---KES ���󰡱�
	pHero->SetFollowPlayer( 0 );
//---------------
}

void CMoveManager::SetHeroPetActionMove(CActionTarget* pTarget,CAction* pAction)
{
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return;
	SetHeroPetTarget(pTargetPos);

	CHeroPet* pHeroPet = OBJECTMGR->GetHeroPet();
	pHeroPet->SetMovingAction(pAction);
}

void CMoveManager::MoveStop(CObject* pObject,VECTOR3* pStopPos)
{
	if(pObject->m_MoveInfo.bMoving)
		EndMove(pObject);
	if(pStopPos)
	{
		pObject->m_MoveInfo.CurPosition = *pStopPos;
		pObject->SetPosition(pStopPos);
	}
	else
	{
		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
	}
}

void CMoveManager::HeroMoveStop()
{

	CHero* pHero = OBJECTMGR->GetHero();
	//if(pHero->m_MoveInfo.bMoving == FALSE)
	//	return;
	
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.26	2007.09.13
	if (pHero->m_MoveInfo.bMoving)
	{
  		if (g_csFarmManager.CLI_GetTargetObj())
 		{
			// ..���� �ָ��� Ŭ���ؼ� �ָ� ������ ���� ���� ���¶�� ���� ���� â ����
			if (g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_SALEBOARD_KIND)
			{
				GAMEIN->GetFarmBuyDlg()->SetActive(TRUE);
			}
			// ..���縦 Ŭ���ؼ� �ٰ��� ���� ���¶�� ���� ��ȭ â ����
			// ..����� NPC�� �ƴϰ� ���� ������Ʈ�� ��ϵǾ� �ֱ� ������ �ٸ� NPC�ʹ� �ٸ��� ���⼭ ���� ��ȭ â ����
			else if (g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_STEWARD_KIND)
			{
				VECTOR3 pos;																		
				HERO->GetPosition( &pos );															

				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(),&pos,0,gCurTime);									
 				g_csFarmManager.CLI_GetTargetObj()->ChangeMotion(5, FALSE);

				GAMEIN->GetNpcScriptDialog()->OpenDialog( 999, g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind );
			}
			// 080411 KTH ..���� ������ Ŭ�� ���� ���
			else if( g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind >= CSHFarmManager::RENDER_ANIMAL_KIND &&
					 g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind < CSHFarmManager::RENDER_ANIMAL_KIND + CSHFarmManager::RENDER_ANIMAL_KIND_NUM )
			{
				VECTOR3 pos;
				HERO->GetPosition( &pos );

				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(), &pos, 0, gCurTime);
				g_csFarmManager.CLI_GetTargetObj()->ChangeMotion(5, FALSE);

				GAMEIN->GetAnimalDialog()->SetActive(TRUE);
				GAMEIN->GetAnimalDialog()->SetAnimal(g_csFarmManager.CLI_GetAnimalInfo((CSHFarmZone::FARM_ZONE)g_csFarmManager.CLI_GetTargetObjID()->nFarmZone, g_csFarmManager.CLI_GetTargetObjID()->nFarmID, g_csFarmManager.CLI_GetTargetObjID()->nAnimalCageID, g_csFarmManager.CLI_GetTargetObjID()->nAnimalID));
			}
			/*else if (g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND)
			{
				// ���� ������ NPC�� ����Ͽ� ���⼭ ��ȭ ���ڸ� ������.
				VECTOR3 pos;																		
				HERO->GetPosition( &pos );															

				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(),&pos,0,gCurTime);									
 				g_csFarmManager.CLI_GetTargetObj()->ChangeMotion(5, FALSE);

				GAMEIN->GetNpcScriptDialog()->OpenDialog( 998, g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind );
			}*/
		}
	}
	// E ����ý��� �߰� added by hseos 2007.08.26	2007.09.13

	MoveStop(pHero,&pHero->m_MoveInfo.CurPosition);

	// ���� ����
	if(pHero->GetState() == eObjectState_Die)
		return;

	if( !OBJECTSTATEMGR->StartObjectState(pHero, eObjectState_None) )
		return;	

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.cpos.Compress(&pHero->m_MoveInfo.CurPosition);

	NETWORK->Send(&msg,sizeof(MOVE_POS));
}

void CMoveManager::HeroPetMoveStop()
{
	CHeroPet* pHeroPet = OBJECTMGR->GetHeroPet();
	CalcPositionEx( pHeroPet,gCurTime );
	
	MoveStop( pHeroPet,&pHeroPet->m_MoveInfo.CurPosition );
	// ���� ����
	if(pHeroPet->GetState() == eObjectState_Die)
		return;

	if( !OBJECTSTATEMGR->StartObjectState( pHeroPet, eObjectState_None ) )
		return;	

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pHeroPet->GetID();
	msg.cpos.Compress( &pHeroPet->m_MoveInfo.CurPosition );

	NETWORK->Send( &msg,sizeof( MOVE_POS ) );
}

float CMoveManager::CalcDistance(CObject* pObject1,CObject* pObject2)
{
	float tx = pObject1->m_MoveInfo.CurPosition.x - pObject2->m_MoveInfo.CurPosition.x;
	float tz = pObject1->m_MoveInfo.CurPosition.z - pObject2->m_MoveInfo.CurPosition.z;

	return sqrtf(tx*tx+tz*tz);
}


void CMoveManager::SetWalkMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Walk;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}
void CMoveManager::SetRunMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Run;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

BOOL CMoveManager::SetKyungGong(CObject* pObject,WORD KGIdx)
{
	if(IsValidKyungGong(pObject,KGIdx) == FALSE)
		return FALSE;

	pObject->m_MoveInfo.KyungGongIdx = KGIdx;

	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}


	return TRUE;
}
BOOL CMoveManager::SetMonsterMoveType(CObject* pObject,WORD KGIdx)
{
	if(!pObject) return FALSE;
	if((pObject->GetObjectKind() & eObjectKind_Monster) == FALSE)
	{
		ASSERT(0);
		return FALSE;
	}
	
	pObject->m_MoveInfo.KyungGongIdx = KGIdx;
	return TRUE;
}

BOOL CMoveManager::IsValidKyungGong(CObject* pObject,WORD KGIdx)
{
	return TRUE;
}

float CMoveManager::GetKyungGongSpeed(WORD KyungGongIdx)
{
	ASSERT(KyungGongIdx);
	CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(KyungGongIdx);
	ASSERT(pKGInfo);
	if(pKGInfo == NULL)
		return 0;

	return pKGInfo->GetSpeed();
}

void CMoveManager::EffectYMove(CObject* pObject,float StartY,float EndY,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	pMoveInfo->bMoving = TRUE;
	pObject->m_bIsYMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;

	VECTOR3 CurPos;
	pObject->GetPosition(&CurPos);

	pMoveInfo->Move_StartPosition.x = CurPos.x;
	pMoveInfo->Move_StartPosition.z = CurPos.z;
	pMoveInfo->Move_StartPosition.y = CurPos.y + StartY;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = CurPos.x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = CurPos.z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = CurPos.y + EndY;
	//taiyoe
	/*
	pMoveInfo->TargetPosition.x = CurPos.x;
	pMoveInfo->TargetPosition.z = CurPos.z;
	pMoveInfo->TargetPosition.y = CurPos.y + EndY;
	*/

	float Distance = EndY - StartY;
	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;
	Speed = Distance / fTime;
	pMoveInfo->Move_Direction.x = 0;
	pMoveInfo->Move_Direction.y = Speed;
	pMoveInfo->Move_Direction.z = 0;
	pMoveInfo->Move_EstimateMoveTime = fTime;
	
	pObject->GetEngineObject()->ApplyHeightField(FALSE);
}

void CMoveManager::EffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	VECTOR3 repos;
	if( MAP->CollisonCheck(&pMoveInfo->CurPosition,TargetPos,&repos,pObject) == TRUE )
		*TargetPos = repos;



	pMoveInfo->bMoving = TRUE;
	pMoveInfo->m_bEffectMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;

	pMoveInfo->SetCurTargetPosIdx(0);
	pMoveInfo->SetMaxTargetPosIdx(1);

	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->Move_StartPosition.y = 0;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = TargetPos->x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = TargetPos->z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = 0;
/*
	pMoveInfo->TargetPosition.x = TargetPos->x;
	pMoveInfo->TargetPosition.z = TargetPos->z;
	pMoveInfo->TargetPosition.y = 0;
*/
	

	float dx = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return;
	}
	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;
	Speed = Distance / fTime;
	float ttt = Speed / Distance;
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	pMoveInfo->Move_EstimateMoveTime = fTime;
}

void CMoveManager::HeroEffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration,CObject* pOperator)
{
	if( pObject->GetObjectKind() == eObjectKind_SkillObject ||
		pObject->GetObjectKind() == eObjectKind_Item )
		return;

	if(pObject != HERO && pOperator != HERO)
	{
		EffectMove(pObject,TargetPos,Duration);
		return;
	}

	if(pObject == HERO)
	{
		HeroMoveStop();
	}
	
	pObject->m_MoveInfo.SetCurTargetPosIdx(0);
	pObject->m_MoveInfo.SetMaxTargetPosIdx(1);
	EffectMove(pObject,TargetPos,Duration);

	if(pObject->IsDied() == FALSE)
	{
		MOVE_POS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_EFFECTMOVE;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pObject->GetID();
		msg.cpos.Compress(TargetPos);
		NETWORK->Send(&msg,sizeof(msg));	

		// 071204 LUJ �������̽� ��� ���¿��� ó���� �۾� ó��
		MACROMGR->CheckToggleState( pObject );
	}
}

void CMoveManager::EffectMoveInterpolation(CObject* pObject,VECTOR3* TargetPos)
{
	if(pObject->IsDied())
		return;

	
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	DWORD dwTime = (DWORD)(pMoveInfo->Move_EstimateMoveTime * 1000);
	DWORD ElapsedTime = gCurTime - pMoveInfo->Move_StartTime;
	if(ElapsedTime >= dwTime)
	{
		if(pObject->m_MoveInfo.bMoving == FALSE)
			SetPosition(pObject,TargetPos);
	}
	else
	{
		DWORD RemainTime = dwTime - ElapsedTime;
		EffectMove(pObject,TargetPos,RemainTime);
	}
	
}

void CMoveManager::ToggleHeroMoveMode()
{
	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
		HeroMoveStop();
	}

	MSGBASE msg;
	msg.Category = MP_MOVE;
	msg.dwObjectID = HEROID;
	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
	{
		msg.Protocol = MP_MOVE_RUNMODE;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( FALSE );
//
	}
	else
	{
		msg.Protocol = MP_MOVE_WALKMODE;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( TRUE );
//
	}
	NETWORK->Send(&msg,sizeof(msg));

	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
		SetRunMode(HERO);
	else
		SetWalkMode(HERO);

	if(bTarget)
	{
		SetHeroTarget(&TargetPos,FALSE);
	}
}

void CMoveManager::ToggleHeroKyungGongMode()
{
	WORD kgidx = HERO->m_MoveInfo.KyungGongIdx;
	MSG_DWORD2 msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_KYUNGGONG_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = HEROID;


	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	}
	
	HeroMoveStop();		//�̰� ���ϴ°���.

	if(kgidx == 0)
	{
		msg.dwData2 = HERO->GetKyungGongGrade();
		SetKyungGong(HERO,HERO->GetKyungGongGrade());
	}
	else
	{
		msg.dwData2 = 0;
		SetKyungGong(HERO,0);
	}
	NETWORK->Send(&msg,sizeof(msg));


	if(bTarget)
	{
		SetHeroTarget(&TargetPos,FALSE);
	}
}


BYTE CMoveManager::GetMoveMode(CObject* pObject)
{
	return pObject->m_MoveInfo.MoveMode;
}

BOOL CMoveManager::IsMoving(CObject* pObject)
{
	return pObject->m_MoveInfo.bMoving;
}


void CMoveManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_MOVE_TARGET:							Move_Target( pMsg ) ;							break;
	case MP_MOVE_ONETARGET:							Move_OneTarget( pMsg ) ;						break;
	case MP_MOVE_STOP:								Move_Stop( pMsg ) ;								break;
	case MP_MOVE_WALKMODE:							Move_WalkMode( pMsg ) ;							break;	
	case MP_MOVE_RUNMODE:							Move_RunMode( pMsg ) ;							break;
	case MP_MOVE_MONSTERMOVE_NOTIFY:				Move_MonsterMove_Notify( pMsg ) ;				break;
	case MP_MOVE_KYUNGGONG_ACK:						Move_Kyunggong_Ack( pMsg ) ;					break;
	case MP_MOVE_KYUNGGONG_NACK:					Move_Kyunggong_Nack( pMsg ) ;					break;
	case MP_MOVE_EFFECTMOVE:
		{
			/*
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
			{
				return;
			}
			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			EffectMoveInterpolation(pObject,&pos);
			*/
			//ASSERT(0);
		}
		break;
	case MP_MOVE_CORRECTION:						Move_Correction( pMsg ) ;						break;
	case MP_MOVE_FORCESTOPKYUNGGONG:				HERO->SetKyungGongMode(FALSE);					break;		
	case MP_MOVE_WARP:								Move_Warp( pMsg ) ;								break;
	case MP_MOVE_PET_MOVE:							Move_OneTarget( pMsg ) ;						break;
	case MP_MOVE_PET_STOP:							Move_Stop( pMsg ) ;								break;
	}
}

void CMoveManager::RefreshHeroMove()
{
	if(IsMoving(HERO) == FALSE)
		return;

	VECTOR3 TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	SetHeroTarget(&TargetPos,FALSE);
}


void CMoveManager::Move_Target( void* pMsg )
{
	MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
	if(pObject)
	{
		CalcPositionEx(pObject,gCurTime);
		pmsg->GetTargetInfo(&pObject->m_MoveInfo);
		pObject->m_MoveInfo.AddedMoveSpeed = pmsg->AddedMoveSpeed;
		VECTOR3 spos;
		pmsg->spos.Decompress(&spos);
		StartMoveEx(pObject, &spos, gCurTime);
	}
	else
	{
#ifdef _GMTOOL_
		//�������� �޼����� ������ Ȯ���غ���.
		++GMTOOLMGR->m_nNullMove;
#endif				
	}

	// 071204 LUJ �������̽� ��� ���¿��� ó���� �۾� ó��
	MACROMGR->CheckToggleState( pObject );
}


void CMoveManager::Move_OneTarget( void* pMsg )
{
	MOVE_ONETARGETPOS_FROMSERVER* pmsg = (MOVE_ONETARGETPOS_FROMSERVER*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
	if(pObject)
	{
		pObject->m_MoveInfo.AddedMoveSpeed = pmsg->AddedMoveSpeed;
		CalcPositionEx(pObject,gCurTime);
		pmsg->GetTargetInfo(&pObject->m_MoveInfo);
		VECTOR3 spos;
		pmsg->GetStartPos(&spos);
		StartMoveEx(pObject, &spos, gCurTime);
	}
	else
	{
#ifdef _GMTOOL_
		//�������� �޼����� ������ Ȯ���غ���.
	++GMTOOLMGR->m_nNullMove;
#endif
	}
}


void CMoveManager::Move_Stop( void* pMsg )
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
	if(pObject)
	{
		VECTOR3 pos;
		pmsg->cpos.Decompress(&pos);
		CMoveManager::MoveStop(pObject,&pos);
	}
}


void CMoveManager::Move_WalkMode( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if( pPlayer )
		SetWalkMode(pPlayer);

	PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Walk);
}


void CMoveManager::Move_RunMode( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if( pPlayer )
		SetRunMode(pPlayer);

	PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Run);
}


void CMoveManager::Move_MonsterMove_Notify( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CMonster* pMob = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	BOOL rt = SetMonsterMoveType(pMob, pmsg->wData);
}


void CMoveManager::Move_Kyunggong_Ack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	PICONMGR->SetKyungGong(pmsg->dwData1, pmsg->dwData2);

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
	if(pPlayer == NULL)
		return;

	BOOL rt = SetKyungGong(pPlayer,(WORD)pmsg->dwData2);
	ASSERT(rt);			
}


void CMoveManager::Move_Kyunggong_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CHero* pHero = OBJECTMGR->GetHero();
	ASSERT(0);
}


void CMoveManager::Move_Correction( void* pMsg )
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
	if(pObject == NULL)
	{
		return;
	}

	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);

	if(IsMoving(pObject))
		MoveStop(pObject,&pos);
	else
		SetPosition(pObject,&pos);
}


void CMoveManager::Move_Warp( void* pMsg )
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
	if(pObject == NULL)
		return;

	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);
	MoveStop( pObject, &pos );
}


