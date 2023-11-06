
//#include	"../../4DyuchiGXExecutive/GXObject.h"
#include	"../myself97common.h"
#include	"../CheckClock.h"
#include	"SphereCollisionTest.h"
#include	"../../4DyuchiGXGFunc/global.h"
#include	"../../4DyuchiGXExecutive/GXEventTriggerObject.h"

// �浹 ó���� ������.
struct _Collision
{
	// static object�� �ﰢ�� �ε��� ����.
#define		cMaxTriCount			(400)
#define		cMaxSObjCount			(100)
	CGXStaticMapObject*				pSObj[cMaxSObjCount];
	DWORD							dwSObj;
	DWORD							pTempTriIndex[cMaxTriCount];
	VECTOR3							pTri[cMaxTriCount*3];
	BYTE							pbTriChecked[cMaxTriCount];		// üũ ����.
	DWORD							dwTriCount;

	// dynamic object�� �ε��� ����.
#define		cMaxObjCount			(600)
	CoGXObject*						pDFound[cMaxObjCount];
	BYTE							pbObjCheck[cMaxObjCount];
	DWORD							dwDObj;

	// �̺�Ʈ�ڽ�.
#define		cMaxEventBoxCount		(50)
	CoGXEventTrigger*				pEventBox[cMaxEventBoxCount];
	DWORD							dwEventBoxCount;
} gColl;

//struct		DYNAMIC_COLLISION_IN
struct		COLLISION_IN
{
	VECTOR3		From;
	VECTOR3		To;
	float		fRadius;
};

struct		COLLISION_OUT
{
	VECTOR3		MeetPivot;		// ���������� �Ǻ�.
	float		fMeetTime;		// ���������� from-to�� ����.

	VECTOR3		WhereMeet;		// ���� ����.
	PLANE		MeetPlane;		// ���� ������ ���.
};

//	������ �浹ó�� �ϴ� �Լ�.
BOOL	TestMovingSphereMeetSphere( COLLISION_OUT* pOut,  COLLISION_IN* pIn, BOUNDING_SPHERE* pTarget);

BOOL	TestMovingSphereMeetTri( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetTriEdge( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetLine( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pL1, VECTOR3* pL2);
BOOL	TestMovingSphereMeetPLANE( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetVertex( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pV);


//	Ǫ��~.
//	pOutCandidate�� �и� ���� ���ο� to �ĺ�, pIn�� ��Ȳ �Է¿�.
void	PushSphere(VECTOR3* pOutCandidate, COLLISION_OUT* pIn, VECTOR3* pTo, float fRadius);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	SphereCollisionTest(CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex)
{
	pCollTestResult->LastVelocity.x		=	0.0f;
	pCollTestResult->LastVelocity.y		=	0.0f;
	pCollTestResult->LastVelocity.z		=	0.0f;

	pCollTestResult->bLand				=	FALSE;

	pCollTestResult->Candidate		=	*pTo;

	DWORD		dwClock;
	BOOL		bResult;

	MClipper*			pClipper	=	pExecutive->GetGXMap()->GetClipper();
	GXSCHEDULE_PROC_MSG_COLLISION_ARG	CallBack;			// �浹�� �Ͼ�� �� pThis�� ������ �ݹ� ����.

	COLLISION_IN	in;
	COLLISION_OUT	out;

	GXSchedulePROC		pProc	=	pThis->GetProcedure();			// pThis�� ���ν���.
	
	VECTOR3		ObjPivot;		// ������Ʈ�� ����. �浹ó�� �ϴµ��� ��� ���Ѵ�.
	VECTOR3		ObjTo;			// ������Ʈ�� ������. ���� �浹ó�� �ϴµ��� ��� ���Ѵ�.
	VECTOR3		ObjVelocity;	// ������Ʈ�� �ӵ�����. 
	float		fObjVelocity;	// ������Ʈ�� �ӵ�. ��Į��.

	BOUNDING_VOLUME*	pbv	=	pThis->GetBoundingVolume();
	in.fRadius		=	pbv->fRadius;


	// �Ǻ� ó��.
	pThis->GetPosition(&ObjPivot);
	VECTOR3_ADDEQU_VECTOR3(&ObjPivot, &(pbv->vPivot));
	ObjTo		=	*pTo;
	VECTOR3_ADDEQU_VECTOR3(&ObjTo, &(pbv->vPivot));
	VECTOR3_SUB_VECTOR3( &ObjVelocity, &ObjTo, &ObjPivot);
	fObjVelocity	=	VECTOR3Length( &ObjVelocity);

	// ��ó ������Ʈ ã�´�.
	BOUNDING_SPHERE		bs;
	bs.v3Point		=	ObjTo;
	bs.fRs			=	fObjVelocity + pbv->fRadius + 10.0f;		// 10.0f�� ����... -_-;

	// ���� Ʈ�� Ÿ��,
	dwClock	=	GetLowClock();
	pClipper->FindNearObject(bs);

	dwClock		=	GetLowClock() - dwClock;

	// ���̳��� ������Ʈ�� ����� ����.
	CoGXObject**		ppFoundDObjTmp;
	gColl.dwDObj	=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundDObjTmp, GX_MAP_OBJECT_TYPE_OBJECT);
	// ���ۺ��� �� ���� ���� ��� üũ.
	if( gColl.dwDObj > cMaxObjCount)
		_asm int 3;
	// ������ ���۷� ī���Ѵ�.
	if( gColl.dwDObj)
		memcpy( gColl.pDFound, ppFoundDObjTmp, sizeof(CGXMapObject*)*gColl.dwDObj);

	// ������ƽ �� ������Ʈ.
	if( (pbv->dwCollisionTargetFlag & COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT) == 0)
//	if( !(pbv->dwCollisionTargetFlag & COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT))
		goto lbPassStaticTree;
	CGXStaticMapObject**	ppFoundSObj;
	gColl.dwSObj	=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundSObj, GX_MAP_OBJECT_TYPE_STRUCT);
	// ���ۺ��� �� ���� ���� ���.
	if( gColl.dwSObj > cMaxSObjCount)
		_asm int 3;
	// ���� ī��.
	if( gColl.dwSObj)
		memcpy( gColl.pSObj, ppFoundSObj, sizeof(CGXStaticMapObject*)*gColl.dwSObj);

	// ������ƽ ������Ʈ�� ���鼭 �ﰢ������ ã�Ƴ���.

	gColl.dwTriCount	=	0;					// �ﰢ�� ����.

	DWORD	dwSObjIndex;
	for( dwSObjIndex = 0; dwSObjIndex < gColl.dwSObj; dwSObjIndex++)
	{
		// ������ƽ ������Ʈ�� Ʈ���� ��,
		MStaticObjectTree*	pTree	=	gColl.pSObj[dwSObjIndex]->GetTree();
		// ���� ������Ʈ�� �ִ� ��ó�� �ﰢ���� ���ۿ� ī��.
		DWORD	dwTempTriCount	=	pTree->FindTriWithSphere( gColl.pTempTriIndex, cMaxTriCount, &bs, FALSE);	// �� �ɼ��� �ٲ㺼 �ʿ䰡 �ִ�.
		DWORD	dwTempIndex;
		for( dwTempIndex = 0; dwTempIndex < dwTempTriCount; dwTempIndex++)
		{
			memcpy( &(gColl.pTri[gColl.dwTriCount*3]), &(pTree->GetTriBuffer()[gColl.pTempTriIndex[dwTempIndex]*3]), sizeof(VECTOR3)*3);
			gColl.dwTriCount++;
		}
	}
lbPassStaticTree:

	// ������ƽ ������Ʈ���� �ﰢ���� �� �̾Ƴ´�.
	
	gColl.dwEventBoxCount =	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundDObjTmp, GX_MAP_OBJECT_TYPE_TRIGGER);
	if( gColl.dwEventBoxCount > cMaxEventBoxCount)
		_asm int 3;
	if( gColl.dwEventBoxCount)
		memcpy( gColl.pEventBox, ppFoundDObjTmp, sizeof(CGXMapObject*)*gColl.dwEventBoxCount);


	DWORD				dwLastMeetObjType	=	0xffffffff;
	DWORD				dwLastMeetObjNum	=	0xffffffff;
	CGXMapObject*		pLastMeetObj		=	0;
	COLLISION_OUT		lastMeetOut;
	memset( &lastMeetOut, 0, sizeof(COLLISION_OUT));
	BOOL				bFound;

	// üũ ���� Ŭ����.
	memset( gColl.pbObjCheck, 0, sizeof(BYTE)*gColl.dwDObj);
	memset( gColl.pbTriChecked, 0, sizeof(BYTE)*cMaxTriCount);
	

	DWORD		dwTempLoopCount	=	0;
	while(1)	// ������ ���鼭,
	{
		dwTempLoopCount++;
		bFound						=	FALSE;
		lastMeetOut.fMeetTime		=	1.0f;
		// �� ���̳��� ������Ʈ.
		if( pbv->dwCollisionTargetFlag&COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT )
		{
			for( DWORD i = 0; i < gColl.dwDObj; i++)
			{
				if( gColl.pDFound[i] == pThis)		// �ڱ� �ڽ��̸� �н�.
					continue;
				
				BOUNDING_VOLUME* pTargetBV	=	gColl.pDFound[i]->GetBoundingVolume();		// Ÿ��.
				VECTOR3		TargetPivot;
				gColl.pDFound[i]->GetPosition( &TargetPivot);
				TargetPivot	=	TargetPivot + pTargetBV->vPivot;
				

				// ������Ʈ Ÿ���� üũ.
				if( pTargetBV->dwType == BOUNDING_VOLUME_TYPE_NONE)
				{
					continue;				
				}

				// �ٿ�� ���Ǿ��� �����Ѵ�.
				BOUNDING_CAPSULE		bc;
				bc.v3From		=	ObjPivot;
				bc.v3To			=	ObjTo;
				bc.fRadius		=	pbv->fRadius;

				// �浹 �������� �ִ°�.? �˻��ؼ� �н�.
				bs.fRs			=	pTargetBV->fRadius;
				bs.v3Point		=	TargetPivot;
				int	iResult		=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, bs);

				if( iResult < 0)
					continue;

				// ������ �浹�� �ϴ°� �˻��Ѵ�.
				in.fRadius	=	pbv->fRadius;
				in.From		=	ObjPivot;
				in.To		=	ObjTo;
//				in.Target	=	bs;
				bResult		=	TestMovingSphereMeetSphere( &out, &in, &bs);
				if( bResult )
				{
					// ������ t üũ. ���� ����� ���ΰ�.?
					if( out.fMeetTime < lastMeetOut.fMeetTime)
					{
						pLastMeetObj	=	gColl.pDFound[i];
						lastMeetOut		=	out;
						bFound			=	TRUE;
						dwLastMeetObjNum	=	i;
						dwLastMeetObjType	=	COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT;
					}
				}
				
			}
		} // if
		

		// �� ������ƽ ������Ʈ�� �ﰢ��.
		if( pbv->dwCollisionTargetFlag&COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT)
		{
			// �ﰢ�� ����Ʈ�� ���ư��鼭,
			DWORD	dwTriIndex;
			for( dwTriIndex = 0; dwTriIndex < gColl.dwTriCount; dwTriIndex++)
			{
				// �浹 �׽�Ʈ.
				in.fRadius	=	pbv->fRadius;
				in.From		=	ObjPivot;
				in.To		=	ObjTo;
				BOOL bMeetTri	=	TestMovingSphereMeetTri( &out, &in, &(gColl.pTri[dwTriIndex*3]));
				if( bMeetTri)
				{
					if( out.fMeetTime < lastMeetOut.fMeetTime) // ���� �ֱ��� t�� ��, �� ������ �ٲ۴�.
					{
						if( out.fMeetTime < -0.01f)			// float�� �ణ�� ���������� ������ �д�.
						{
							VECTOR3		Gab;
							VECTOR3_SUB_VECTOR3( &Gab, &(in.From), &(out.MeetPivot));
							float		fGab	=	VECTOR3Length( &Gab);
							if( fGab > 0.1f)
							{
								_asm int 3;		// �׷��� �װ� Ȳ���� ������ �������� �� ����ϴ�.
							}
						}
						lastMeetOut	=	out;
						bFound		=	TRUE;
						dwLastMeetObjType			=	COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT;
//						dwLastMeetComponentType		=	dwType;
//						gColl.pbTriChecked[dwTriIndex]		=	TRUE;
						dwLastMeetObjNum	=	dwTriIndex;
					}
				}
				_asm nop;
			}
		}

		if( bFound )		// �ϳ��� ������ �ִ�.
		{
			// �̹� ������ ���ΰ�.?
			if( dwLastMeetObjType	==	COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT)
			{
				if( gColl.pbObjCheck[dwLastMeetObjNum] == TRUE)
				{
					_asm nop;
					// �浹 ������ �� ������� �� ���� ����.
//					*pCandidate		=	lastMeetOut.MeetPivot;
					pCollTestResult->Candidate	=	lastMeetOut.MeetPivot;
					goto	lbReturn;
				}
				// �װ� �ƴ϶�� �浹 ó��.
				gColl.pbObjCheck[dwLastMeetObjNum]	=	TRUE;				// üũ�ϰ�,
			}



			// ���� ������Ʈ Ÿ���� �ǹ��� ���, ������ �ﰢ���� �� ������.?
			if(dwLastMeetObjType == COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT)
			{
				if( gColl.pbTriChecked[dwLastMeetObjNum] == TRUE)
				{
					_asm nop;
//					*pCandidate		=	lastMeetOut.MeetPivot;
//					goto	lbReturn;
				}
				_asm nop;
				gColl.pbTriChecked[dwLastMeetObjNum]		=	TRUE;
			}

			// ���ν��� ��. ��� �ൿ���� �����Ѵ�.
			CallBack.dwCollisionType	=	GX_MAP_OBJECT_TYPE_OBJECT;
			CallBack.gxHandle			=	gColl.pDFound[dwLastMeetObjNum];
			CallBack.vMeetPos			=	lastMeetOut.MeetPivot;
			CallBack.vWhereMeet			=	lastMeetOut.WhereMeet;
//			CallBack.vToCandidate		=	ObjTo;
			CallBack.MeetPlane.v3Up		=	lastMeetOut.MeetPivot - lastMeetOut.WhereMeet;
			Normalize( &CallBack.MeetPlane.v3Up, &CallBack.MeetPlane.v3Up);
			CallBack.MeetPlane.D		=	-1.0f * ( CallBack.MeetPlane.v3Up * lastMeetOut.WhereMeet );
			DWORD dwProcedureReturn	=	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE;		// �⺻��.
			if( pProc)
				dwProcedureReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_COLLISION, DWORD( &CallBack), 0, pThis->GetData());
			switch( dwProcedureReturn)		// ���ν��� ���ϰ��� ���� �ൿ ���.
			{
				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_STOP:
//					*pCandidate		=	lastMeetOut.MeetPivot;
					pCollTestResult->Candidate		=	lastMeetOut.MeetPivot;
					goto	lbReturn;
				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE:
					// ã���� ������� Ǫ��~
					VECTOR3		NewTo;
					PushSphere( &NewTo, &lastMeetOut, &ObjTo, pbv->fRadius);
					// ���ο� From�� To.
					ObjPivot	=	lastMeetOut.MeetPivot;
					ObjTo		=	NewTo;

					break;
//				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_REFLECT:
//					break;
				default:
					_asm int 3;
			}

		}
		else	// ������ �ϳ��� ���ٸ� ���� �׸�~
		{
//			*pCandidate		=	ObjTo;
			pCollTestResult->Candidate		=	ObjTo;
			goto	lbReturn;
		}

		// �浹 ���� �����ΰ�.?
		ObjVelocity		=	ObjTo	-	ObjPivot;
		fObjVelocity	=	VECTOR3Length( &ObjVelocity);
		if( fObjVelocity < 1.0f)
		{
//			*pCandidate	=	ObjPivot;
			pCollTestResult->Candidate		=	ObjPivot;
			goto	lbReturn;
		}

		// �ӵ� ���͸� �ణ ���δ�.
		Normalize( &ObjVelocity, &ObjVelocity);
		fObjVelocity	*=	0.99f;
		ObjVelocity	=	ObjVelocity	*	fObjVelocity;
		ObjTo	=	ObjPivot	+	ObjVelocity;

		if( dwTempLoopCount > 100)
		{
			_asm int 3;				// �ʹ� ������ ���� ���Ҵ�.
		}
	} //while

lbReturn:
	// ���������� ���� ��ġ�� �̺�Ʈ�ڽ��� �����°� �˻��Ѵ�.
	BOUNDING_SPHERE	ThisSphere;
//	ThisSphere.v3Point	=	*pCandidate;
	ThisSphere.v3Point		=	pCollTestResult->Candidate;
	ThisSphere.fRs		=	pbv->fRadius;
	DWORD	dwEventIndex;
	for( dwEventIndex = 0; dwEventIndex < gColl.dwEventBoxCount; dwEventIndex++)
	{
		BOUNDING_BOX*	pBox	=	&(gColl.pEventBox[dwEventIndex]->GetCollisionMeshObjectDesc()->boundingBox);
		BOOL	bMeetBox	=	IsCollisionSphereAndBox( &ThisSphere,  pBox);

		if( bMeetBox)		// ������ ���,
		{
			DWORD	dwTriggerProcReturn	=	0;
			GXSchedulePROC		pTriggerProc	=	gColl.pEventBox[dwEventIndex]->GetProc();
			if( pTriggerProc)
				pTriggerProc( pExecutive, (GXOBJECT_HANDLE)gColl.pEventBox[dwEventIndex], GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, (DWORD)pThis, 0, gColl.pEventBox[dwEventIndex]->GetData());


//			DWORD	dwEventTriggerReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());

			if( pProc)
			{
				GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG		TriggerArg;
				TriggerArg.dwEventTriggerID		=	gColl.pEventBox[dwEventIndex]->GetID();
//				dwTriggerProcReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, (DWORD)gColl.pEventBox[dwEventIndex], 0, pThis->GetData());
				dwTriggerProcReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());
			}
		}
	}


	VECTOR3_SUBEQU_VECTOR3( &(pCollTestResult->Candidate), &(pbv->vPivot));

	if( dwTempLoopCount > 5)
	{
		_asm nop;
	}

	return	;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	����		:	X = F + (T-F)*t;
	������		:	r1;
	
	Ÿ������	:	P;
	Ÿ�ٹ�����	:	r2;
	
	|P-X| = r1+r2;
	�� �����ϸ�,
	t^2*(T-F)(T-F) + 2*t*(F(T-F)-P(T-F)) + PP - 2*PF + FF - (r1+r2)*(r1+r2) = 0;
	    ----------       ---------------   --------------------------------
			= a                 = b                        = c
*/
BOOL TestMovingSphereMeetSphere( COLLISION_OUT* pOut,  COLLISION_IN* pIn, BOUNDING_SPHERE* pTarget)
{
	VECTOR3		vTemp;

//	float	r	=	pIn->fRadius	+	pIn->Target.fRs;
	float	r	=	pIn->fRadius	+	pTarget->fRs;

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &pIn->To, &pIn->From);

	float	a	=	DotProduct( &Velocity, &Velocity);
	float	b	=	DotProduct( &(pIn->From), &Velocity)	-	DotProduct(&(pTarget->v3Point), &Velocity);
//	float	c	=	DotProduct( &(pTarget->v3Point),&(pIn->Target.v3Point)) - 2 * DotProduct( &(pIn->Target.v3Point), &(pIn->From)) + DotProduct( &(pIn->From), &(pIn->From)) - pIn->fRadius * pIn->fRadius;
	float	c	=	DotProduct( &(pTarget->v3Point),&(pTarget->v3Point)) - 2 * DotProduct( &(pTarget->v3Point), &(pIn->From)) + DotProduct( &(pIn->From), &(pIn->From)) - r*r;

	float	fDet	=	b*b-a*c;		// �Ǻ���.
	if( fDet < -0.00001f)
	{
		// ������ �ʴ´�.
		return	FALSE;
	}
	else	if( fDet > 0.00001f)	// �������� ������. �ƹ����� ��κ� ���� �ɸ���.
	{
		// ���� ����.	t = (-b - root(b*b-a*c))/a;
		//				t = (-b + root(b*b-a*c))/a; �� �̹� ������ ���¿��� �����ٴ� ���̹Ƿ� �ǹ̰� ����. 
		pOut->fMeetTime		=	( -1.0f*b - (float)sqrt(b*b - a*c) ) / a;
	}
	else		// -0.0001f <= fDet <= 0.0001f �������� ������.
	{
		// ���� ����.	t = (-b) / a;
		pOut->fMeetTime		=	(-1.0f*b) / a;
	}


	// ���� t�� �˻��� �� �浹�� ��ȿ������ �˾Ƴ���.
	if( pOut->fMeetTime > 1.0f)
	{
		// �ʹ� �ָ��� ������. 
		return	FALSE;
	}
	
	if( pOut->fMeetTime < 0.0f)	//	'-'�� ������ ���.
	{
		// �ε� �Ҽ����� ����������, ���� ������ '-'�� ���� �̹� � ��ü�� ������� ���� �����Ƿ� üũ�ؼ�,
		// ������ �ִٸ� time���� '0'���� ���߰� '0'�� �������� ������ ������ ��Ƴ���.

		//	�������������~



		VECTOR3		From2Target;			// Ÿ���� �Ǻ� - ���Ǿ�From;
		VECTOR3_SUB_VECTOR3( &From2Target, &(pTarget->v3Point), &(pIn->From));
		Normalize( &From2Target, &From2Target);

		VECTOR3		VelocityNormal;
		Normalize( &VelocityNormal, &Velocity);			// ���Ǿ��� �ӵ�.
	
		fDet	=	DotProduct( &From2Target, &VelocityNormal);		// ������ ����� ���Ǿ��� �ӵ� ����, ���� �׽�Ʈ.
		if( fDet <= 0.0f)		// ���� ������ ������ ��� �����ϰų� �־����� ������ ���.
		{
			return	FALSE;
		}

		// �ణ ��ġ�°ɷ� �Ǻ�.
		if( pOut->fMeetTime < 0.01f)
		{
			_asm nop;			// ����ü ��������� �� ���ΰ�.?
		}

	}

	//	�����ٰ� �Ǻ��Ǵ� ��� pOut�� ä��� ����. ���� pOut->MeetPlane�� �Լ� �ܺο��� ä���.

//	pOut->MeetPivot	=	pIn->From	+	(pIn->To - pIn->From)*pOut->fMeetTime
	VECTOR3_SUB_VECTOR3( &vTemp, &(pIn->To), &(pIn->From));
	VECTOR3_MULEQU_FLOAT( &vTemp, pOut->fMeetTime);
	VECTOR3_ADD_VECTOR3( &(pOut->MeetPivot), &(pIn->From), &vTemp);
//	pOut->WhereMeet		=	pOut->MeetPivot + (pIn->Target.v3Point - pOut->MeetPivot) * (pIn->fRadius) / (pIn->fRadius + pIn->Target.fRs);
	VECTOR3		Candidate2Target;
	VECTOR3_SUB_VECTOR3( &Candidate2Target, &(pTarget->v3Point), &(pOut->MeetPivot));
	VECTOR3_MUL_FLOAT( &vTemp, &Candidate2Target, (pIn->fRadius) / (pIn->fRadius + pTarget->fRs));
	VECTOR3_ADD_VECTOR3( &(pOut->WhereMeet), &(pOut->MeetPivot), &vTemp);
	// X*N+d = 0;
//	Normalize( &(pOut->MeetPlane.v3Up), &Candidate2Target);
//	VECTOR3_MULEQU_FLOAT( &(pOut->MeetPlane.v3Up), -1.0f);
//	pOut->MeetPlane.D	=	-1 * DotProduct( &(pOut->MeetPlane.v3Up), &(pOut->WhereMeet));

	return	TRUE;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Ǫ��~.
//	pOutCandidate�� �и� ���� ���ο� to �ĺ�, pIn�� ��Ȳ �Է¿�.
void	PushSphere(VECTOR3* pOutCandidate, COLLISION_OUT* pIn, VECTOR3* pTo, float fRadius)
{
	// �������� ���� ������ ���Ǿ� ������ ������ �Ÿ��� ����Ѵ�. 
	float	a	=	pIn->MeetPivot	*	pIn->MeetPlane.v3Up	+	pIn->MeetPlane.D;

	if( a < fRadius)	// ���� ��鿡 ���Ǿ �̹� �İ� ���ֳ�.?
	{
		//	�̷� ����� ���ͼ� �ȵȴ�.
		//	�浹 �׽�Ʈ �ϴ� �Լ����� ó������ t�� '-'�� �������� �ȵȴ�.
//		_asm int 3;
//		a		=	fRadius;			// ��� a�� ������?
	}

	//	��ǥ �������� ������ ���� ������ �Ÿ�, ���� '+', '0', '-' �� ���� �� �ִ�.
	float	b	=	*pTo	*	pIn->MeetPlane.v3Up	+	pIn->MeetPlane.D;

	*pOutCandidate	=	*pTo	+	pIn->MeetPlane.v3Up * (a - b);	// �о ����Ʈ�� ���� �ϰ�,

	// ����.
	return	;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTri( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	BOOL	bResult;
	BOOL	bFound	=	FALSE;		// �ﰢ���� ������ �����°�.?
	COLLISION_OUT		out;

	DWORD	dwLastMeetComponent	=	0xffffffff;

	// ���� ������ ��.?
	bResult	=	TestMovingSphereMeetPLANE( &out, pIn, pTri);		// ���ϰ� �� out.MeetPlane�� ��ȿ.
	if( bResult == TRUE)
	{
		dwLastMeetComponent	=	0;
		bFound	=	TRUE;
		*pOut	=	out;
		CheckVectorArray( &(out.MeetPivot), 1);
	}


	// ������ ������ ��.?
	bResult	=	TestMovingSphereMeetTriEdge( &out, pIn, pTri);	// ���ϰ� �� out.MeetPlane�� ��ȿ.
	if( bResult == TRUE)
	{
		if( (bFound == TRUE && pOut->fMeetTime > out.fMeetTime) || bFound == FALSE)
		{
			dwLastMeetComponent	=	1;
			bFound	=	TRUE;
			*pOut	=	out;
			CheckVectorArray( &(out.MeetPivot), 1);
		}
	}


	// �𼭸��� ������ ��?
	int	i;
	for( i = 0; i < 3; i++)
	{
		bResult	=	TestMovingSphereMeetVertex( &out, pIn, &(pTri[i]));		// ���ϰ� �� out.MeetPlane�� ��ȿ.
		if( bResult == TRUE)
		{
			if( (bFound == TRUE && pOut->fMeetTime > out.fMeetTime) || bFound == FALSE)
			{
				dwLastMeetComponent	=	2;
				bFound	=	TRUE;
				*pOut	=	out;
				CheckVectorArray( &(out.MeetPivot), 1);
			}
		}
	}



lbReturn:

	// ������ �ִ°�.?
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	// ������ ���� ����Ѵ�.

	CheckVectorArray( &(pOut->MeetPivot), 1);


	pOut->MeetPlane.v3Up	=	pOut->WhereMeet	-	pOut->MeetPivot;
	Normalize( &pOut->MeetPlane.v3Up, &pOut->MeetPlane.v3Up);
	pOut->MeetPlane.D		=	-1.0f * DotProduct( &pOut->MeetPlane.v3Up, &pOut->WhereMeet);


//	if( pOut->MeetPlane.v3Up.x < 600000.0f)
//		_asm int 3;

	return	TRUE;			// ��.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTriEdge( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	// �ﰢ���� �� ������ ���ƺ���.
	float	t	=	1.0f;

	BOOL	bResult;
	BOOL	bFound	=	FALSE;
	COLLISION_OUT	Out;

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[0], &pTri[1]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[1], &pTri[2]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[2], &pTri[0]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	if( bFound == FALSE)
	{
		return	FALSE;
	}

			CheckVectorArray( &(pOut->MeetPivot), 1);


	CheckVectorArray( &(pOut->MeetPivot), 0);
	CheckVectorArray( &(pOut->MeetPlane.v3Up), 0);


	return	TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t�� '-'�� ��� �ٽ� ������ �ִ��� üũ�ϰ� ��ġ��. �� ����ۿ� ����. -_-;
	2003.03.18	t�� '-'�� ��� �������� ��ġ��.
	�����̴� ������ ���ΰ� �����°�.?

	������		M = A + B*s;
	���� ������ P = C + D*t;

	�̶�
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	�ΰ��� ���´�.

	��1�� M, P�� �����Ͽ� s = ~~  ������ ������ ��,
	���� M = A + B*s�� �����Ͽ� M�� t�� ���� ������ ����.

	M�� P�� ��2�� �����ؼ�, (E+F*t)*(E+F*t) = r^2 ����,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	�Ǻ��� det = b^2 - 4*a*c;

	���� �ΰ��϶� t1 = (-b - root(det)) / 2a �ϳ��� ��� (t2�� �ǹ̰� ����.)

    �̶� 0 < s < 1 �� �ƴϸ� ���� �ǹ̰� ����.
*/

//�и��� �̳��� ������ �ִ�.
BOOL	TestMovingSphereMeetLine( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pL1, VECTOR3* pL2)
{
	VECTOR3		A	=	*pL1;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pL2, pL1);

//	VECTOR3_SUBEQU_VECTOR3( &B, &A);
	VECTOR3		M;									// Line���� ����.
	float		s;									// �׶��� ����.

	VECTOR3		C	=	pIn->From;					// Sphere From.
	VECTOR3		D	=	pIn->To;					// Sphere Velocity.
	VECTOR3_SUBEQU_VECTOR3( &D, &C);
	VECTOR3		P;									// Sphere���� ����.
	float		t;									// �׶��� ����.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);

	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pIn->fRadius * pIn->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// �Ǻ���.

	// �Ǻ��� < 0
	if( det < -0.00001f)
	{
		// ������ ����.? �����.
		return	FALSE;
	}

	// �Ǻ��� = 0
	if( det >= -0.00001f && det <= 0.00001f)
	{
		t	=	b / (-2.0f * a);
	}

	// �Ǻ��� > 0
	if( det > 0.00001f)
	{
		t	=	(-b - (float)sqrt(det)) / (2.0f*a);
	}

	// t�� �ʹ� �� ���. 
	if( t > 1.0f)
	{
		return	FALSE;
	}
	if( t < -1.0f)
	{
		return	FALSE;
	}

	// ������ Sphere�� ������ ���.?
	P	=	C + D*t;

	// ������ Line�� ��ġ�� ���.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line�� ��ġ�� �����,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// ������ ������ �κ�.

	// t�� '-'�� ��� ���Ǿ��� ���� ��ġ���� �̹� ������ �ִ°��� üũ�Ѵ�.
	if( t < 0.0f)
	{
		float	fDist	=	VECTOR3Length( &(A-M)  );
		if( fDist > pIn->fRadius)
			return	FALSE;		
		_asm nop;
	}

	pOut->fMeetTime	=	t;
	pOut->MeetPivot	=	P;
	pOut->WhereMeet	=	M;

	return	TRUE;
}

/*
	2003.03.18 ���͸��� ���� �Ǿ��ִ��� ���ƴ�. �� �ڵ尡 ��������.?
	2002.??.?? ���� �ۼ�. 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetPLANE( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	BOOL		bResult;
	float		fTemp;

	float		fPlaneDotVelocity;

	float		t;			// ���Ǿ �鿡 �������� ����.
	VECTOR3		X;			// ���Ǿ �鿡 ��� ��ġ.
	VECTOR3		MeetPos;	// ���Ǿ�� ���� �´�� ��.

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &(pIn->To), &(pIn->From));

	PLANE	p;
//	MakePLANE( &p, &(pTri[0]) , &(pTri[1]), &(pTri[2]));
	CalcPlaneEquation( &p, pTri);


	// �����ϰų�, ���Ǿ��� ���� ����� ���� �븻�� ���� �����̸� ������ �� ��ģ��.
	fPlaneDotVelocity	=	DotProduct( &(p.v3Up), &Velocity);
	if( fPlaneDotVelocity >= 0.0f)
	{
		// �ȸ����� ����.
		return	FALSE;
	}

	// ���Ǿ��� ����	: X = F + (T-F)*t
	// ��鿡�� ���Ǿ� ���������� �Ÿ�.		: X*N + d = r  (r�� ���Ǿ� ������.)
	// t = -1*(N*F+d-r)/(N*(T-F))
	t	=	DotProduct( &(p.v3Up), &(pIn->From));
	t	=	pIn->fRadius - (t + p.D);
	t	=	t / fPlaneDotVelocity;//DotProduct( &(p.v3Up), &Velocity);
	// X = F + (T-F)*t
	VECTOR3_MUL_FLOAT( &X, &Velocity, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &(pIn->From));


	if( t > 1.0f)
	{
		return	FALSE;
	}

	if( t < -1.0f)	//	�̷� ����,
	{
		//	�׳� ���� �ص� �ɱ�.?
		//	����� ��Ȯ�ϴٸ� �翬�ϰ���.?
		return	FALSE;

		//	'-1'�̸� �������ǰ�.? �� ����.?
	}

	// ���⼭ 0�� ����� t���� �׳� 0���� ó�� �ϴ°� ���� ������.?
	

	// ���Ǿ�� ����� ������ ���� �´��� ����.?
	// X���� ���븻�� �ݴ�������� ��������ŭ �� ��.
	// MeetPos	=	X + N*(-1*fRadius)
	MeetPos	=	p.v3Up;
	VECTOR3_MULEQU_FLOAT( &MeetPos, pIn->fRadius * -1.0f);
	VECTOR3_ADDEQU_VECTOR3( &MeetPos, &X);

	// �� ������ ���� �ﰢ�� �ȿ� �ִ°�.?
	VECTOR2		v2Meet;
	bResult	=	IsTriMeetVertex(pTri, &MeetPos);
	if( bResult	== FALSE)
		return	FALSE;

	// ���� float�� ������ ���� �̹� ������ �ִ� ������ �Ǻ��� ��츦 ������ ó���Ѵ�.
	if( t < 0.0f)
	{
		// ���� �̹� ������ �ִ� ����� ���Ǿ �����̱� ���� ��ġ�� �����Ѵ�.
		fTemp	=	DotProduct( &(pIn->From), &(p.v3Up)) + p.D;
		if( fTemp <= 0.0f )	// ������ ����ٸ�,
			return	FALSE;

		pOut->fMeetTime	=	0.0f;
		pOut->MeetPivot	=	pIn->From;
		pOut->WhereMeet	=	pIn->From	+	(p.v3Up * -1.0f) * pIn->fRadius;

		CheckVectorArray( &(pOut->MeetPivot), 1);


		return	TRUE;

	}

	// �̱��� ���� �ﰢ���� �̷�� ���� ������.
	// pOut �����ϰ� ����.
	pOut->fMeetTime	=	t;
	pOut->MeetPivot	=	X;
	pOut->WhereMeet	=	MeetPos;
	// pOut->MeetPlane�� ���߿� �ϰ������� ���.

//	VECTOR3	vTemp	=	pIn->From	+	(p.v3Up * -1.0f) * pIn->fRadius;
		CheckVectorArray( &(pOut->MeetPivot), 1);

	// ����.
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	�����̴� ���Ǿ ���� �����°�.?
	
	���� P, 
	������ ������ X = A + B*t;	�� ����.
	PX�� ���̴� r.

*/
BOOL TestMovingSphereMeetVertex( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pP)
{
	VECTOR3		A	=	pIn->From;								// ���� ����.
	VECTOR3		B;	
	VECTOR3_SUB_VECTOR3( &B, &(pIn->To), &(pIn->From));			// ���� �ӵ�����.
	VECTOR3		X;												// ���찡 ���� �������� ��ġ.
	float		t;												// ������ ����.
	// X = A + B*t

	VECTOR3		P	=	*pP;		// Ÿ�� ��.

	VECTOR3		PA;
	VECTOR3_SUB_VECTOR3( &PA, &A, &P);

	float		a	=	DotProduct( &B, &B);
	float		b	=	2.0f * DotProduct( &B, &PA);
	float		c	=	DotProduct( &PA, &PA)	-	pIn->fRadius * pIn->fRadius;

	float		det	=	b*b		-	4*a*c;

	if( det <= -0.00001f)
	{
		return	FALSE;
	}
	
	t	=	(-b - (float)sqrt(det)) / (2*a);
	if( t > 1.0f)
	{
		return	FALSE;
	}

	//	X	=	A	+	B*t;
	VECTOR3_MUL_FLOAT( &X, &B, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &A);

	// t�� '-'�̴��� �̹� ������ ������ TRUE ����.
	VECTOR3		vTemp;
	VECTOR3_SUB_VECTOR3( &vTemp, &(pIn->From), &P);
	float	fDist	=	VECTOR3Length( &vTemp);

	if( fDist >= pIn->fRadius)
	{
		return	FALSE;
	}

	// pOut ����.
	pOut->fMeetTime	=	t;			// �������� ����.
	pOut->MeetPivot	=	X;			// �������� ���� �Ǻ�.
	pOut->WhereMeet	=	P;			// ��� ������.?

	return	TRUE;
}

