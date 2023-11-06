// CharMove.cpp: implementation of the CCharMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharMove.h"

#include "PackedData.h"
#include "Player.h"
#include "TileManager.h"
#include "GridSystem.h"
#include "MapDBMsgParser.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharMove::CCharMove()
{

}

CCharMove::~CCharMove()
{

}

void CCharMove::InitMove(CObject* pObject,VECTOR3* pPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	pMoveInfo->CurPosition = *pPos;
	pMoveInfo->bMoving = FALSE;
	pMoveInfo->m_bMoveCalculateTime = FALSE;
	pMoveInfo->MoveMode = eMoveMode_Run;
	pMoveInfo->KyungGongIdx = 0;
	pMoveInfo->AddedMoveSpeed = 0.0f;
	/*�����Ƽ ���� - �̿��� pMoveInfo->AbilityKyungGongLevel = 0;*/
	pMoveInfo->Move_EstimateMoveTime = 0;
	pMoveInfo->InitTargetPosition();
	
	g_pServerSystem->m_pGridSystem->AddObject(pObject,pPos);
	g_pServerSystem->GetMap()->GetTileManager()->AddObject(pObject);
}

void CCharMove::ReleaseMove(CObject* pObject)
{
	g_pServerSystem->m_pGridSystem->RemoveObject(pObject);
	g_pServerSystem->GetMap()->GetTileManager()->RemoveObject(pObject);
}

VECTOR3* CCharMove::GetPosition(CObject* pObject)
{
	return CalcPositionEx(pObject,gCurTime);
}

void CCharMove::SetPosition(CObject* pObject,VECTOR3* pPos)
{
	//ASSERTVALID_POSITION(*pPos);
	if(CheckValidPosition(*pPos) == FALSE)
	{
		return;
	}

	if(pObject->m_MoveInfo.bMoving)
		pObject->m_MoveInfo.bMoving = FALSE;

	pObject->m_MoveInfo.CurPosition = *pPos;
	pObject->m_bNeededToCalcGrid = TRUE;

	g_pServerSystem->GetMap()->GetTileManager()->Preoccupy(pObject,pPos);
}

//BOOL CCharMove::CheckVaildStop(CPlayer* pPlayer,DWORD CurTime,VECTOR3* EndPos)
BOOL CCharMove::CheckVaildStop(CObject* pObject,DWORD CurTime,VECTOR3* EndPos)
{	
	CalcPositionEx(pObject,CurTime);
	if(CalcDistanceXZ(&pObject->m_MoveInfo.CurPosition,EndPos) > MOVE_LIMIT_ERRORDISTANCE)
	{
		CorrectPlayerPosToServer(pObject);
		return FALSE;
	}
	return TRUE;
}

// ���� ��ġ�� ����ؼ� EndPos�� ���̰� ���� ���� ��ġ-_-;�� ���Ѵ�.
// �̵��� �������� ǥ���Ѵ�.
void CCharMove::EndMove(CObject* pObject,DWORD CurTime,VECTOR3* EndPos)
{	
	//ASSERTVALID_POSITION(*EndPos);
	if(CheckValidPosition(*EndPos) == FALSE)
	{
		pObject->m_MoveInfo.bMoving = FALSE;
		pObject->m_bNeededToCalcGrid = TRUE;
		pObject->m_MoveInfo.InitTargetPosition();
		return;
	}
	
	pObject->m_MoveInfo.CurPosition = *EndPos;
	pObject->m_MoveInfo.bMoving = FALSE;
	pObject->m_MoveInfo.InitTargetPosition();
	pObject->m_bNeededToCalcGrid = TRUE;

	g_pServerSystem->GetMap()->GetTileManager()->Preoccupy(pObject,EndPos);

	if(pObject->m_MoveInfo.KyungGongIdx)
		SetKyungGong(pObject,0);
}

void CCharMove::StartMoveEx(CObject* pObject,DWORD CurTime,VECTOR3* StartPos,VECTOR3* pTargetPos)
{	
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());

	CalcPositionEx(pObject,CurTime);	//---KES �� ���ܰ��� �̵��� ����ġ���� �ǵ��ε�.

	//KES HACK CHECK
	if( pObject->GetObjectKind() == eObjectKind_Player )
	if( CalcDistanceXZ(&pMoveInfo->CurPosition,StartPos) > 10*100 ) //5m ���� �ɷȳ� �׷� 10m
	{
		((CPlayer*)pObject)->SpeedHackCheck();		
	}
	
	pMoveInfo->Move_StartTime = CurTime;

	//YH2DO
	float Speed = pObject->GetMoveSpeed();
	if(Speed == 0)
	{
		EndMove(pObject,CurTime,StartPos);
		return;
	}

	pMoveInfo->CurPosition.x = StartPos->x;
	pMoveInfo->CurPosition.z = StartPos->z;
	pMoveInfo->Move_StartPosition.x = StartPos->x;
	pMoveInfo->Move_StartPosition.z = StartPos->z;

	float Distance = CalcDistanceXZ(pTargetPos,&pMoveInfo->Move_StartPosition);
	pMoveInfo->Move_Direction.x = pTargetPos->x - pMoveInfo->Move_StartPosition.x;
	pMoveInfo->Move_Direction.z = pTargetPos->z - pMoveInfo->Move_StartPosition.z;

	if(Distance)
	{
		float tt = (Speed / Distance);
		pMoveInfo->Move_Direction.x = pMoveInfo->Move_Direction.x * tt;
		pMoveInfo->Move_Direction.z = pMoveInfo->Move_Direction.z * tt;
	}
	else
	{
		pMoveInfo->Move_Direction.x = 0;
		pMoveInfo->Move_Direction.z = 0;
	}

	pMoveInfo->Move_EstimateMoveTime = Distance / Speed;

	if(pMoveInfo->Move_EstimateMoveTime < 0)
	{
		ASSERT(0);
	}
	
	//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
	//////////////////////////////////////////////////////////////////////////
	// YH2DO ���⿡ �浹�˻簰���� �ִ´�.
	//////////////////////////////////////////////////////////////////////////	

	pObject->m_bNeededToCalcGrid = TRUE;

	pMoveInfo->m_bMoveCalculateTime = g_pServerSystem->m_pGridSystem->GetGridChangeTime(
		pObject,StartPos,pTargetPos,Speed,pMoveInfo->m_dwMoveCalculateTime);
	
	g_pServerSystem->GetMap()->GetTileManager()->Preoccupy(pObject,pTargetPos);

	pMoveInfo->bMoving = TRUE;
	ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());
}

VECTOR3* CCharMove::CalcPositionEx(CObject* pObject,DWORD CurTime)
{
	//���� object�� ���������ε���  pMoveInfo->bMovingd= TRUE�� ��찡 �ִ�.
	MOVE_INFO * pMoveInfo = &pObject->m_MoveInfo;
	//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
	if(pMoveInfo->bMoving)
	{
		ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());
		if(CurTime == pMoveInfo->Move_LastCalcTime)	//---KES �ð��� �帣�� �ʾҴٸ�
		{
			if(pMoveInfo->CurPosition.x < 0)			pMoveInfo->CurPosition.x = 0;
			else if(pMoveInfo->CurPosition.x > 51200)	pMoveInfo->CurPosition.x = 51200;

			if(pMoveInfo->CurPosition.z < 0)			pMoveInfo->CurPosition.z = 0;
			else if(pMoveInfo->CurPosition.z > 51200)	pMoveInfo->CurPosition.z = 51200;
			
			return &pMoveInfo->CurPosition;
		}
		
		pMoveInfo->Move_LastCalcTime = CurTime;

		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f); //---KES �������ӿ� �ѹ��� ���Ǿ�� �Ѵ�. =_= ��¥ �����ֱ���.

		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)	//�ð��� ������.
		{
			//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
			// ���� Ÿ������ ����
			if(pMoveInfo->GetCurTargetPosIdx() < pMoveInfo->GetMaxTargetPosIdx()- 1)
			{				
				//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				pMoveInfo->SetCurTargetPosIdx(pMoveInfo->GetCurTargetPosIdx()+1);


				//ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());
				StartMoveEx(pObject, CurTime, &pMoveInfo->CurPosition, pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx()));
				//---KES �̰��� �̻��ϴ�.. �ð��� ���� �귯 2ĭ �̻��� �ǳʾ� �� �ʿ䰡 �ִٸ�?
			}
			else
			{
				//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
				ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				pMoveInfo->bMoving = FALSE;
							
				if(pMoveInfo->KyungGongIdx)
					SetKyungGong(pObject,0);
			}
			//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
		}
		else
		{
			//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
			pMoveInfo->CurPosition.x = pMoveInfo->Move_StartPosition.x + pMoveInfo->Move_Direction.x * fMoveTime;
			pMoveInfo->CurPosition.z = pMoveInfo->Move_StartPosition.z + pMoveInfo->Move_Direction.z * fMoveTime;
			//ASSERTVALID_POSITION(pMoveInfo->CurPosition);
		}
	}
	
	if(pMoveInfo->CurPosition.x < 0)	pMoveInfo->CurPosition.x = 0;
	else if(pMoveInfo->CurPosition.x > 51100)	pMoveInfo->CurPosition.x = 51100;

	if(pMoveInfo->CurPosition.z < 0)	pMoveInfo->CurPosition.z = 0;	
	else if(pMoveInfo->CurPosition.z > 51100)	pMoveInfo->CurPosition.z = 51100;

	return &pMoveInfo->CurPosition;
}

void CCharMove::MoveProcess(CObject* pObject)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pMoveInfo->bMoving == FALSE)
		goto MoveProcessEnd;

	if(pMoveInfo->m_bMoveCalculateTime)
	{
		if(pMoveInfo->m_dwMoveCalculateTime <= gTickTime)
		{
			ASSERT(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx());
			//StartMoveEx(pObject,gCurTime,&pMoveInfo->CurPosition,&pMoveInfo->TargetPositions[pMoveInfo->CurTargetPositionIdx]);
			StartMoveEx(pObject,gCurTime,&pMoveInfo->CurPosition, pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx()));
		}
		else
		{
			pMoveInfo->m_dwMoveCalculateTime -= gTickTime;
		}
			
	}

MoveProcessEnd:
	if(pObject->m_bNeededToCalcGrid == TRUE)
	{		
		if(pMoveInfo->CurPosition.x < 0)	pMoveInfo->CurPosition.x = 0;
		if(pMoveInfo->CurPosition.z < 0)	pMoveInfo->CurPosition.z = 0;
		if(pMoveInfo->CurPosition.x > 51100)	pMoveInfo->CurPosition.x = 51100;
		if(pMoveInfo->CurPosition.z > 51100)	pMoveInfo->CurPosition.z = 51100;
		
		g_pServerSystem->m_pGridSystem->ChangeGrid(pObject,&pMoveInfo->CurPosition);
		pObject->m_bNeededToCalcGrid = FALSE;
	}
}

BOOL CCharMove::IsMoving(CObject* pObject)
{
	if(pObject->m_MoveInfo.bMoving)
		CalcPositionEx(pObject,gCurTime);

	return pObject->m_MoveInfo.bMoving;
}

void CCharMove::SetWalkMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Walk;
}
void CCharMove::SetRunMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Run;
}
BOOL CCharMove::SetKyungGong(CObject* pObject,WORD KGIdx)
{
	pObject->m_MoveInfo.KyungGongIdx = KGIdx;
	/*�����Ƽ ���� - �̿��� if( pObject->GetObjectKind() == eObjectKind_Player )
		pObject->m_MoveInfo.AbilityKyungGongLevel = ((CPlayer*)pObject)->GetAbilityStats()->KyunggongLevel;
	else
		pObject->m_MoveInfo.AbilityKyungGongLevel = 0;
*/
	pObject->m_KyungGongMana_LastCheckTime = 0;	// ���ۺ��� ���̰� �ϱ� ���ؼ�

	return TRUE;
}

BOOL CCharMove::IsValidKyungGong(CObject* pObject,WORD KGIdx)
{
	if(KGIdx == 0)		// ��� ������ �׻� OK
		return TRUE;

	// YH2DO ����� ��� ����? -_-a
	if(pObject->GetObjectKind() != eObjectKind_Player)
		return FALSE;

	if(((CPlayer*)pObject)->GetKyungGongGrade() == KGIdx)
		return TRUE;
	else
		return FALSE;
}

void CCharMove::CorrectPlayerPosToServer(CObject* pObject)
{
	VECTOR3* pCurPos = CCharMove::GetPosition(pObject);
	EndMove(pObject,gCurTime,pCurPos);

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_CORRECTION;
	msg.dwMoverID = pObject->GetID();
	msg.cpos.Compress(pCurPos);

	PACKEDDATA_OBJ->QuickSend(pObject,&msg,sizeof(msg));

}

DWORD CCharMove::GetMoveEstimateTime(CObject* pObject)
{
	DWORD dwEstimateTime = 0;
	float Speed = pObject->GetMoveSpeed();
	CalcPositionEx(pObject, gCurTime);
	VECTOR3 * pVPos = pObject->m_MoveInfo.GetTargetPosition(0);
	if( 0 == pObject->m_MoveInfo.GetMaxTargetPosIdx()) return 0;
	ASSERT(pObject->m_MoveInfo.GetCurTargetPosIdx() < pObject->m_MoveInfo.GetMaxTargetPosIdx());
	for(int i = pObject->m_MoveInfo.GetCurTargetPosIdx() ; i < pObject->m_MoveInfo.GetMaxTargetPosIdx()-1; ++i)
	{
		float Distance = CalcDistanceXZ(&pVPos[i],&pVPos[i+1]);
		dwEstimateTime += (DWORD)(Distance/Speed*1000);
	}

	dwEstimateTime += (DWORD)(pObject->m_MoveInfo.Move_EstimateMoveTime*1000);

	return dwEstimateTime;
}

void CCharMove::ForceStopKyungGong(CPlayer* pPlayer)
{
	MSGBASE msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_FORCESTOPKYUNGGONG;
	pPlayer->SendMsg(&msg,sizeof(msg));
}

void CCharMove::Warp(CObject* pObject, VECTOR3* vPos )
{
	SetPosition(pObject, vPos);

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_WARP;
	msg.dwMoverID = pObject->GetID();
	msg.cpos.Compress(vPos);

	PACKEDDATA_OBJ->QuickSend(pObject,&msg,sizeof(msg));
}
