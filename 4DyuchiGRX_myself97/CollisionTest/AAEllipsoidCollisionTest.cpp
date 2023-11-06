

#include	"../../4DyuchiGXExecutive/GXObject.h"
#include	"../myself97common.h"
#include	"../CheckClock.h"
#include	"../../4DyuchiGXGFunc/global.h"
#include	"../../4DyuchiGXExecutive/GXEventTriggerObject.h"

#include	"CollisionTestMovingEllipsoidMeetTriangle.h"
 
//typedef DWORD (__stdcall *CollisionTestCallBackProcedure)(COLLISION_RESULT*	pResult);
//// ������Ʈ������ �浹ó���� ���� �ݹ�.
DWORD	__stdcall CollisionCallBack(COLLISION_RESULT* pResult);


BOOL AAEllipsoidMeetAAEllipsoid( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, AAELLIPSOID* pTarget);

BOOL AAEllipsoidMeetTriangle( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, VECTOR3* pTri );

BOOL TestMovingSphereMeetPLANE2( float* pTOut, BOUNDING_SPHERE* pSphere, VECTOR3* pVelocity, VECTOR3* pTri);

// Ÿ��ü���� �־��� �� ���� ������ �ִ� ����� �����. return���� false�� ��� Ÿ��ü���� �ʹ� �ָ� ������ ���� ���.
BOOL FindPlaneOnEllipsoidWithVertex( PLANE* pOut, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex);


/*
	Ÿ��ü�������� �ﰢ���� �Բ� ���Ǿ� �������� �����.
	��, ��, �� �׽�Ʈ�� ��ģ ��,
	Ÿ��ü �������� �ﰢ���� Ÿ��ü�� �ǵ�����, ������ t������ ������� ���� ���� ���� �Ѵ�.

	Ÿ��ü�� ���Ǿ�� ���� ��� �� ����, ������ �׶��� t���� ��ȿ�ϰ�,
	������� Ÿ��ü �������� ���� ����ؾ� ���� �� �ϴ�.
	
*/
BOOL AAEllipsoidMeetTriangle( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, VECTOR3* pTri )
{
	MOVING_ELLIPSOID	MovingEllipsoid;
	MovingEllipsoid.From		=	pMoving->P;
	MovingEllipsoid.fHeight		=	pMoving->h;
	MovingEllipsoid.fWidth		=	pMoving->w;
	MovingEllipsoid.Velocity	=	*pVelocity;

	COLLISION_RESULT	CollisionInfo;
	BOOL	bResult	=	CollisionTestMovingEllipsoidMeetTriangle( &CollisionInfo, &MovingEllipsoid, pTri);


	// ������ �ִ°�.?
	if( bResult == FALSE)
	{
		return	FALSE;
	}

	*pOut	=	CollisionInfo;

	return	TRUE;			// ��.
}

void AAEllipsoidCollisionTest( CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex)
{

	// �� ������Ʈ�� ��ġ.
	VECTOR3			GXObjectPosition;
	pThis->GetPosition( &GXObjectPosition);

	// pTo�� ������.? �������� ������ ����.
	VECTOR3			ObjVelocity;
	VECTOR3_SUB_VECTOR3( &ObjVelocity, pTo, &GXObjectPosition);
	float			fObjVelocity	=	VECTOR3Length( &ObjVelocity);
	if( fObjVelocity == 0.0f)
	{

		pCollTestResult->LastVelocity.x			=	0.0f;
		pCollTestResult->LastVelocity.y			=	0.0f;
		pCollTestResult->LastVelocity.z			=	0.0f;

		pCollTestResult->Candidate		=	*pTo;
		pCollTestResult->bLand			=	FALSE;
		return	;
	}


	// ���ν���.
	GXSchedulePROC	pProc	=	pThis->GetProcedure();


	// �� ������Ʈ�� �浹 ���� �� Ÿ��ü �����.
	BOUNDING_VOLUME*	pObjBV;
	pObjBV		=	pThis->GetBoundingVolume();
	AAELLIPSOID	ObjEllip;
	ObjEllip.h		=	pObjBV->fHeight;
	ObjEllip.w		=	pObjBV->fRadius;
	ObjEllip.P		=	pObjBV->vPivot		+	GXObjectPosition;

	// pTo�� �Ǻ� ����.
	VECTOR3			To	=	*pTo;
	VECTOR3_ADDEQU_VECTOR3( &To, &pObjBV->vPivot);

	// STM�� �浹�޽��� �浹 �õ�.
	I3DStaticModel*			pSTM		=		pExecutive->GetGXMap()->GetBaseModel();
	VECTOR3			CollResult;
	VECTOR3			LastVelocity;
	BOOL			bLanding;

	pSTM->CollisionTestAAELLIPSOID( &CollResult, &LastVelocity, &bLanding, CollisionCallBack, &ObjEllip, &To, dwBufferIndex);
	
	pCollTestResult->LastVelocity	=	LastVelocity;
	float	fLastVelocity	=	VECTOR3Length( &LastVelocity);
	if( fLastVelocity == 0.0f)
		_asm nop;
	/*
	 *	���⼭�� CollisionCallBack���� ....
	 */

	// �浹ó�� ������.
	
	// �浹 �߱���~
	// �Ǻ� �����ϰ�,
//	Candidate		=	CollResult		-	pObjBV->vPivot;	// �߰� �Ǻ��� ����.
	
	// Ʈ�� Ÿ�� �̺�ƮƮ���� �ܾ�´�.
	BOUNDING_SPHERE		TriggerBS;
	TriggerBS.fRs		=	1.0f;
	TriggerBS.v3Point	=	CollResult;
	MClipper*	pClipper	=	pExecutive->GetGXMap()->GetClipper();
	pClipper->FindNearObject( TriggerBS);
	CoGXEventTrigger**			ppTrigger;		// �ð��� ���� �� ���۸� ���� ���� ī��.
	DWORD	dwTrigger		=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppTrigger, GX_MAP_OBJECT_TYPE_TRIGGER);
	if( dwTrigger)
	{
		// ������ �׽�Ʈ. �� �� �ȿ� ������ ���°��� �Ǵ��Ѵ�.
		// Ʈ���Źڽ��� �ٱ����� ���ϰ� �����Ƿ� -������ üũ.
		DWORD	i;
		for( i = 0; i < dwTrigger; i++)
		{
			DWORD	j;
			PLANE*		pPlane	=	ppTrigger[i]->GetPlanes();
			for( j = 0; j < 6; j++)
			{
				float	fIn	=	DotProduct(&pPlane[j].v3Up, &CollResult)	+	pPlane[j].D;
				if( fIn >	0.0f)
					goto	lbBreakTrigger;
			}
			
			// �� �������� ������.? ��ü�� ���ν��� �ѹ� ȣ�� ���ش�.
			if( pProc)
			{
				GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG		TriggerArg;
				TriggerArg.dwEventTriggerID		=	ppTrigger[i]->GetID();
				DWORD	dwEventTriggerReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());
			}

lbBreakTrigger:
				_asm nop;

		}
	}


	// ��ġ�� �ٽ� �����ְ� ����.
//	*pCandidate	=	Candidate	-	pObjBV->vPivot;
	pCollTestResult->Candidate		=	CollResult	-	pObjBV->vPivot;
	pCollTestResult->bLand			=	bLanding;

	
}

DWORD	__stdcall CollisionCallBack(COLLISION_RESULT* pResult)
{
	_asm nop;
	return	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE;		// �ϴ�. ������ GXOBJECT�� ���ν��� ȣ���ؼ� �����.
}
