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

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.26	2007.09.13
#include "../hseos/Farm/SHFarmBuyDlg.h"
#include "../hseos/Farm/SHFarmManager.h"
#include "NpcScriptDialog.h"
// E 농장시스템 추가 added by hseos 2007.08.26	2007.09.13

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

	//pos = m_AutoTargetList.GetHeadPosition() ;						// 리스트를 모두 비운다.

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
	// Move_bMoving이 TRUE 이 아닐때에만 가능.. ASSERT(Move_bMoving == FALSE); 걸어두자
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

	// 071008 웅주
	// 히어로인 경우 서버에 통보해줘서 상태를 갱신하도록 해야한다. 그렇지 않으면 다른 그리드로 이동할 경우, 다른 플레이어가
	// 히어로의 상태를 정상적으로 보지 못한다(가만히 있는데 뛰고 있는 상태 등으로 나옴)
	
	//--> KES 서버에서 처리할 수는 없을까. 아니면 상태만 서버에 세팅하도록 하는 다른 패킷을 주는게 어떨까. 
	//    왜냐하면 HeroMoveStop을 호출하면, 그리드의 모든 유저에게 브로드캐스팅을 하게된다. 네트워크 부하가 예상.

	if( pObject == HERO )
	{
		if( pObject->m_MoveInfo.KyungGongIdx )
		{
			ToggleHeroKyungGongMode();	//<-- 이안에 HeroMoveStop()이 들어있다.
		}
		else
		{
			HeroMoveStop();
		}
	}
}

// 부분 타겟 업데이트 
int CMoveManager::TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos)
{
	//VECTOR3 * TargetPos = &pMoveInfo->TargetPositions[pMoveInfo->CurTargetPositionIdx];
	// 타겟 거리의 합
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
	//YH 몬스터 튀는것때문에 주석처리함
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

	// 071122 웅주. 스킬 동작이 끝나지 않은 채로 이동되는 경우가 있다. 이때 이동 동작으로
	//	바로 잡아주지 않으면, 미끄러지기/잘못된 방향으로 뛰기 등이 나타난다.StartMoveEx()를
	//	실행한다는 것이 이미 해당 오브젝트에 이동이 승인되었다는 뜻이므로 모션을 변경해준다.
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
			// 다음 타겟으로 변경

			//---KES FindPatch Fix 071020
			//if( pMoveInfo->GetMaxTargetPosIdx() > 0 && pMoveInfo->GetCurTargetPosIdx() < pMoveInfo->GetMaxTargetPosIdx()- 1)
			if(pMoveInfo->GetCurTargetPosIdx() < pMoveInfo->GetMaxTargetPosIdx()- 1)	//뒤집힐 경우가 있을 수 있을 것 같은데...
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

// 이동중인 상태로 표시하고 시작지점과 끝지점을 기억해놓는다.
// 이동시작시간도 기억해놓는다.
// 현재위치를 계산해봐서 StartPos 랑 차이가 많이 나면 조치-_-;를 취한다.
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

	// 여러개의 타겟 처리
	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->TargetPosition.x = TargetPos->x;
	pMoveInfo->TargetPosition.z = TargetPos->z;
	pMoveInfo->Move_StartPosition.y = 0;
	pMoveInfo->TargetPosition.y = 0;

	// 타겟 거리의 합
	float dx = pMoveInfo->TargetPosition.x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->TargetPosition.z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}
	float ttt = Speed / Distance;

	// 여러 타겟의 방향 설정 ( 배열혹은 할때마다 셋팅? )
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	// 예상 시간 설정
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

// 현재 위치를 계산해서 Move_Position에 셋팅하고 넘겨준다.
// 계속해서 계산하지 않고 필요할때만 계산한다.
// Move_bMoving이 FALSE이면 바루 리턴한다.
// 시간 변수에 대한 OverFlow를 고려해야 한다.
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

	UpdateAutoTarget(pObject) ;																		// 타겟리스트를 업데이트 한다.
}

void CMoveManager::UpdateAutoTarget(CObject* pObject)
{
	if( pObject )																					// 오브젝트 정보가 유효한지 체크한다.
	{
		if( pObject->GetObjectKind() == eObjectKind_Monster )										// 오브젝트 타입을 체크한다.
		{
			float fDistance = 0.0f ;																// HERO와의 거리를 담을 변수를 선언한다.

			VECTOR3 vHeroPos;																		// HERO의 위치를 담을 벡터를 선언한다.
			HERO->GetPosition(&vHeroPos) ;															// HERO의 위치를 받는다.
			fDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;				// HERO와 몬스터의 거리를 구한다.

			SortAutoTargetList(pObject->GetID(), fDistance) ;										// 오브젝트 정보를 포함하여 자동 타겟 리스트를 정렬한다.
		}
		else																						// 오브젝트 가 몬스터 타입이 아니라면,
		{
			// 항상 리스트를 체크해야하기 때문에(자동타겟 영역을 벗어나는이유) 정렬을 한다.
			SortAutoTargetList(0, 0.0f) ;															
		}
	}
}

void CMoveManager::SortAutoTargetList(DWORD dwObjectID, float fDistance)
{
	//ZeroMemory(sTempList, sizeof(DISTANCE_ID)*6) ;													// 오브젝트의 아이디와 거리를 담을 구조체를 초기화 한다.

	PTRLISTPOS pos	 = NULL ;																		// 리스트상의 위치정보를 받을 포인터를 선언한다.
	DWORD* pObjectID = NULL ;																		// 오브젝트 아이디를 받을 포인터를 선언한다.
	CObject* pObject = NULL ;																		// 오브젝트 정보를 받을 포인터를 선언한다.

	BOOL bSameID = FALSE ;																			// 리스트상에 같은 아이디가 존재하는지 여부를 담는 변수 선언.

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

	if( dwObjectID != 0 )																			// 아이디를 검색한다.
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

		//if( !bSameID )																				// 같은 아이디가 없으면,
		//{
		//	if( fDistance > AUTOTARGET_RANGE )														// HERO와 몬스터의 거리가 자동타겟 영역 안이면,
		//	{
		//		return ;
		//	}

		//	int nListCount = m_AutoTargetList.GetCount() ;										// 자동 타겟의 아이템 개수를 구한다.

		//	if( nListCount > 4 )																// 리스트 카운트가 5개가 넘으면,
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

		//				DWORD* pNewObjectID = new DWORD ;													// 아이디를 위한 메모리를 할당한다.
		//				*pNewObjectID = dwObjectID ;														// 아이디를 저장한다.
		//				m_AutoTargetList.AddTail(pNewObjectID) ;											// 자동타겟 리스트에 아이디를 추가한다.
		//			}
		//		}
		//	}
		//	else
		//	{
		//		DWORD* pNewObjectID = new DWORD ;													// 아이디를 위한 메모리를 할당한다.
		//		*pNewObjectID = dwObjectID ;														// 아이디를 저장한다.
		//		m_AutoTargetList.AddTail(pNewObjectID) ;											// 자동타겟 리스트에 아이디를 추가한다.
		//	}
		//}

 
		for( int count = 0 ; count < 5 ;++count )													// 리스트의 아이템 수만큼 포문을 돌린다.
		{
			pos = m_AutoTargetList.FindIndex(count) ;												// 카운트에 따른 위치 정보를 받는다.

			if( pos )																				// 위치 정보가 유효한지 체크한다.
			{
				pObjectID = (DWORD*)m_AutoTargetList.GetAt(pos) ;									// 위체에 해당하는 오브젝트 아이디를 받는다.

				if( pObjectID )																		// 아이디 정보가 유효한지 체크한다.
				{
					DWORD dwCompareID = *pObjectID ;												// 아이디를 받는다.

					if( dwCompareID == dwObjectID )													// 리스트의 오브젝트와 인자로 넘어온 오브젝트 아이디가 같다면,
					{
						bSameID = TRUE ;															// 리스트내에 같은 아이디가 있다고 세팅한다.
					}
				}
			}
		}

		if( !bSameID )																				// 같은 아이디가 없으면,
		{
			if( fDistance <= AUTOTARGET_RANGE )														// HERO와 몬스터의 거리가 자동타겟 영역 안이면,
			{
				int nListCount = m_AutoTargetList.GetCount() ;										// 자동 타겟의 아이템 개수를 구한다.

				if( nListCount > 5 )																// 리스트 카운트가 5개가 넘으면,
				{
					for( int count = nListCount ; count < nListCount-5 ; --count )					// 카운트를 리스트 카운트로 세팅하고, 리스트 카운트 -5 한만큼 for문을 돌린다.
					{
                        PTRLISTPOS pos = m_AutoTargetList.FindIndex(count-1) ;						// 현재 카운트-1한 위치를 받는다.
						if( pos )																	// 위치 정보가 유효한지 체크한다.
						{
							m_AutoTargetList.RemoveAt(pos) ;										// 위치에 해댱하는 정보를 지운다.
						}
					}

					PTRLISTPOS firstPos = m_AutoTargetList.FindIndex(0) ;							// 리스트의 첫 위치를 받는다.

					if( firstPos )																	// 첫 위치가 유효하면,
					{
						m_AutoTargetList.RemoveAt(firstPos) ;										// 첫 위치의 정보를 삭제한다.
					}
				}

				DWORD* pNewObjectID = new DWORD ;													// 아이디를 위한 메모리를 할당한다.
				*pNewObjectID = dwObjectID ;														// 아이디를 저장한다.
				m_AutoTargetList.AddTail(pNewObjectID) ;											// 자동타겟 리스트에 아이디를 추가한다.
			}
		}
	}
}

void CMoveManager::SwapTargetList(DISTANCE_ID* p1, DISTANCE_ID* p2)
{
	DISTANCE_ID pTemp ;																				// 임시로 아이디와 거리를 담을 구조체를 선언한다.

	memcpy(&pTemp, p2, sizeof(DISTANCE_ID)) ;														// 임시 구조체에 두번째 정보를 복사한다.
	memcpy(p2, p1, sizeof(DISTANCE_ID)) ;															// 두번째 구조체에 첫번째 정보를 복사한다.
	memcpy(p1, &pTemp, sizeof(DISTANCE_ID)) ;														// 첫번재 구조체에 임시 정보를 복사한다.
}

//void CMoveManager::UpdateAutoTarget(CObject* pObject)
//{
//	if( pObject )
//	{
//		if( pObject->GetObjectKind() == eObjectKind_Monster )					// 오브젝트 타입이 몬스터라면,
//		{
//			float fDistance = 0.0f ;											// HERO와 몬스터의 거리를 담을 변수를 선언한다.
//
//			float fTargetDistance = 1000.0f ;
//
//			VECTOR3 vHeroPos;
//			HERO->GetPosition(&vHeroPos) ;
//			fDistance = CalcDistanceXZ(&vHeroPos, &pObject->m_MoveInfo.CurPosition) ;// HERO와 몬스터의 거리를 구한다.
//
//			if( fDistance <= fTargetDistance )									// 거리가 자동 타겟 영역 내라면,
//			{
//				int nMonsterCount = m_AutoTargetList.GetCount() ;				// 리스트의 개수를 받는다.
//
//				if( nMonsterCount >= 5 )										// 아이템 수가 다섯개 이상이면,
//				{
//					SortAutoTargetList(pObject->GetID(), fDistance) ;			// 리스트를 정렬 및 업데이트 한다.
//				}
//				else															// 아이템 수가 다섯개 미만이면,
//				{
//					if( m_AutoTargetList.GetCount() == 0 )
//					{
//						DWORD* pNewID = new DWORD ;
//						*pNewID = pObject->GetID() ;
//						m_AutoTargetList.AddTail(pNewID) ;							// 리스트에 오브젝트 아이디를 추가한다.
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
//							m_AutoTargetList.AddTail(pNewID) ;							// 리스트에 오브젝트 아이디를 추가한다.
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
//	// 자동 타겟 리스트 업데이트.
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
//	// 새로 들어온 몬스터 정보를 세팅한다.
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
//	// 새로 들어온 몬스터와 거리 비교를 해서 정렬한다.
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
//	// 리스트에 다시 정보를 세팅한다.
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
//	// 자동 타겟 리스트 업데이트.
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
//	// 자동 타겟 리스트 업데이트.
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
//	// 새로 들어온 몬스터 정보를 세팅한다.
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
//	// 새로 들어온 몬스터와 거리 비교를 해서 정렬한다.
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
//	// 리스트에 다시 정보를 세팅한다.
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
	// 50cm 타일의 중앙에 서게 한다.
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

	// 길찾기 테스트 용
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

	// 071204 LUJ 인터페이스 토글 상태에서 처리할 작업 처리
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
	// 50cm 타일의 중앙에 서게 한다.
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
/// 06. 08. 2차 보스 - 이영준
/// 기존 SetLookatPos 함수 마지막 인자에 추가 회전각을 추가했다.
void CMoveManager::SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg)
{
	pObject->m_MoveInfo.CurPosition.y = 0;
	VECTOR3 PositionGap = *pPos - pObject->m_MoveInfo.CurPosition;
	float Distance = VECTOR3Length(&PositionGap);
	if(Distance == 0)
		return;
	VECTOR3 TDirection = PositionGap / Distance;
	/// 추가 회전각을 더해준다
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

//---KES 따라가기
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
	
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.26	2007.09.13
	if (pHero->m_MoveInfo.bMoving)
	{
  		if (g_csFarmManager.CLI_GetTargetObj())
 		{
			// ..농장 팻말을 클릭해서 팻말 가까이 가서 멈춘 상태라면 농장 구입 창 열기
			if (g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_SALEBOARD_KIND)
			{
				GAMEIN->GetFarmBuyDlg()->SetActive(TRUE);
			}
			// ..집사를 클릭해서 다가가 멈춘 상태라면 집사 대화 창 열기
			// ..집사는 NPC가 아니고 농장 오브젝트로 등록되어 있기 때문에 다른 NPC와는 다르게 여기서 직접 대화 창 오픈
			else if (g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_STEWARD_KIND)
			{
				VECTOR3 pos;																		
				HERO->GetPosition( &pos );															

				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(),&pos,0,gCurTime);									
 				g_csFarmManager.CLI_GetTargetObj()->ChangeMotion(5, FALSE);

				GAMEIN->GetNpcScriptDialog()->OpenDialog( 999, g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind );
			}
			// 080411 KTH ..가축 슬롯을 클릭 했을 경우
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
				// 가축 슬롯은 NPC로 취급하여 여기서 대화 상자를 실행함.
				VECTOR3 pos;																		
				HERO->GetPosition( &pos );															

				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(),&pos,0,gCurTime);									
 				g_csFarmManager.CLI_GetTargetObj()->ChangeMotion(5, FALSE);

				GAMEIN->GetNpcScriptDialog()->OpenDialog( 998, g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind );
			}*/
		}
	}
	// E 농장시스템 추가 added by hseos 2007.08.26	2007.09.13

	MoveStop(pHero,&pHero->m_MoveInfo.CurPosition);

	// 상태 변경
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
	// 상태 변경
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

		// 071204 LUJ 인터페이스 토글 상태에서 처리할 작업 처리
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
	
	HeroMoveStop();		//이거 왜하는거지.

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
		//쓸데없는 메세지를 보내나 확인해보자.
		++GMTOOLMGR->m_nNullMove;
#endif				
	}

	// 071204 LUJ 인터페이스 토글 상태에서 처리할 작업 처리
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
		//쓸데없는 메세지를 보내나 확인해보자.
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


