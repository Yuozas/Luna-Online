// MHCamera.cpp: implementation of the CMHCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHCamera.h"
#include ".\Engine\EngineCamera.h"
#include "Object.h"
#include "MHCameraDefine.h"
#include "MoveManager.h"
#include "ObjectManager.h"

#include "GameResourceManager.h"

#include "mhMap.h"
#include "./Input/UserInput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float DEFAULT_ANGLEX_FOLLOW_VIEW = 15.f;
float DEFAULT_ANGLEY_FOLLOW_VIEW = 180.f;
float DEFAULT_DISTANCE_FOLLOW_VIEW = 700.f;

float CHARHEIGHT	= 140.f;
float SITHEIGHT		= 90.f;
float DIEHEIGHT		= 80.f;



SHAKEVECTOR MHDesc_Earthquake[] =
{
	{	0.f,	10.f,	0.f,	50	},
	{	0.f,	-10.f,	0.f,	50	},
	{	0.f,	20.f,	0.f,	50	},
	{	0.f,	-20.f,	0.f,	50	},
	{	0.f,	15.f,	0.f,	50	},
	{	0.f,	-15.f,	0.f,	50	},
	{	0.f,	10.f,	0.f,	50	},
	{	0.f,	-10.f,	0.f,	50	},
	{	0.f,	5.f,	0.f,	50	},
	{	0.f,	-5.f,	0.f,	50	},
	{	0.f,	2.f,	0.f,	50	},
	{	0.f,	-2.f,	0.f,	50	},
	{	0.f,	1.f,	0.f,	50	},
	{	0.f,	-1.f,	0.f,	50	},
};

SHAKEVECTOR MHDesc_Vibration[] =
{
	{	0.f,	5.f,	0.f,	50	},
	{	0.f,	-5.f,	0.f,	50	},
	{	0.f,	5.f,	0.f,	50	},
	{	0.f,	-5.f,	0.f,	50	},
	{	0.f,	2.f,	0.f,	50	},
	{	0.f,	-2.f,	0.f,	50	},
	{	0.f,	1.f,	0.f,	50	},
	{	0.f,	-1.f,	0.f,	50	},
};

GLOBALTON(CMHCamera)
CMHCamera::CMHCamera()
{
	for(int n=0;n<MAX_CAMERA_DESC;++n)
	{
		m_CameraDesc[n].m_AngleX.SetAngleDeg(0);
		m_CameraDesc[n].m_AngleY.SetAngleDeg(0);
		m_CameraDesc[n].m_pPivotObject = NULL;
		m_CameraDesc[n].m_fDistance = 0;
		m_CameraDesc[n].m_bCameraAngleMovingX = 0;
		m_CameraDesc[n].m_bCameraAngleMovingY = 0;
		m_CameraDesc[n].m_bCameraDistanceMoving = 0;
		//KES
		m_CameraDesc[n].m_bHeightChanging = FALSE;
	}
	m_CurCamera = 0;
	
	m_bIsVibrating = FALSE;
	m_bIsShaking = FALSE;


	m_ppCameraShakeDesc = new MHCAMERASHAKE*[eCSD_Max];
	m_ppCameraShakeDesc[eCSD_Earthquake] = new MHCAMERASHAKE(MHDesc_Earthquake,sizeof(MHDesc_Earthquake));
	m_ppCameraShakeDesc[eCSD_Vibration] = new MHCAMERASHAKE(MHDesc_Vibration,sizeof(MHDesc_Vibration));

	m_fDistanceRate		= 100.0f;		//(%)
	m_fMaxSightDistance	= 10000.0f;		//(cm)
	m_fFov				= gPHI/4.0f;		//rad


	m_CurPosition = eCP_CHARSEL;
	m_GotoPosition = -1;
	m_MoveSpeed = 300;
	m_bMoved = FALSE;
	m_pCameraFilter = NULL;

	m_LastMoveNoticeTime = 0;
}

CMHCamera::~CMHCamera()
{
	for(int n=1;n<eCSD_Max;++n)
	{
		delete m_ppCameraShakeDesc[n];
	}
	delete [] m_ppCameraShakeDesc;

	//---KES Map GXObject Alpha 071020	
	m_AlphaHandleList.RemoveAll();
	//----------------------------
}

void CMHCamera::Init(CObject* pObject,float angleX,float angleY,float fDistance)
{	
	VECTOR3 pos;
	if(pObject)
		pObject->GetPosition(&pos);
	else
		pos.x = pos.y = pos.z = 0;
	
	InitCamera(0,angleX,angleY,fDistance,pObject);
	SetCurCamera(0);

	SetCameraMode(eCM_Free);
}
void CMHCamera::InitCamera(int CameraNum,float angleX,float angleY,float fDistance,CObject* pObject)
{
	m_CameraDesc[CameraNum].m_AngleX.SetAngleDeg(angleX);
	m_CameraDesc[CameraNum].m_AngleY.SetAngleDeg(angleY);
	m_CameraDesc[CameraNum].m_fDistance = fDistance;
	m_CameraDesc[CameraNum].m_CharHeight = CHARHEIGHT;
	m_CameraDesc[CameraNum].m_TargetCharHeight = CHARHEIGHT;
	
	m_CameraDesc[CameraNum].m_bCameraAngleMovingX = FALSE;
	m_CameraDesc[CameraNum].m_bCameraAngleMovingY = FALSE;
	m_CameraDesc[CameraNum].m_bCameraDistanceMoving = FALSE;

	SetCameraPivotObject(CameraNum,pObject);

	if(m_CurCamera == CameraNum)
	{
		CalcCameraPosAngle();
		SetToEngineCamera();
	}

	float fov = MAP->GetMapDesc()->Fov;
	float radian = fov*gPHI/360;
	m_DownVector.x = 0;	
	m_DownVector.y = -CAMERA_NEAR * (tanf(radian) + 1);
	m_DownVector.z = 0;
}
void CMHCamera::InitCamera(int CameraNum,float angleX,float angleY,float fDistance,VECTOR3* pPivotPos)
{
	m_CameraDesc[CameraNum].m_pPivotObject = NULL;
	m_CameraDesc[CameraNum].m_AngleX.SetAngleDeg(angleX);
	m_CameraDesc[CameraNum].m_AngleY.SetAngleDeg(angleY);
	m_CameraDesc[CameraNum].m_fDistance = fDistance;
	m_CameraDesc[CameraNum].m_CharHeight = CHARHEIGHT;
	m_CameraDesc[CameraNum].m_TargetCharHeight = CHARHEIGHT;

	m_CameraDesc[CameraNum].m_PivotPos = *pPivotPos;

	m_CameraDesc[CameraNum].m_bPivotObject = FALSE;

	if(m_CurCamera == CameraNum)
	{
		CalcCameraPosAngle();
		SetToEngineCamera();
	}
}


void CMHCamera::SetToEngineCamera()
{
	VECTOR3 AnglePYR;
	AnglePYR.x = -m_CameraDesc[m_CurCamera].m_AngleX.ToRad();
	AnglePYR.y = -m_CameraDesc[m_CurCamera].m_AngleY.ToRad();
	AnglePYR.z = 0;

	VECTOR3 CameraAngle = AnglePYR + m_VibrateAngle;
	VECTOR3 CameraPos = m_CameraDesc[m_CurCamera].m_CameraPos;	
	if(m_bIsVibrating)
		CameraPos = CameraPos + m_VibratePos;
	if(m_bIsShaking)
		CameraPos = CameraPos + m_ShakingPos;

	m_EngineCamera.SetCameraPos(&CameraPos);
	m_EngineCamera.SetCameraAngle(&CameraAngle);
}

//---KES Map GXObject Alpha 071020	
void CMHCamera::GXOAlphaProcess()
{
	static DWORD dwLastCheckTime = 0;
	DWORD ElapsedTime = gCurTime - dwLastCheckTime;
	DWORD interval = ElapsedTime/5;

	if( ElapsedTime >= 50 )	//---�ʹ� ���� ȣ�� �Ǵ� ���� ����
	{
		PICK_GXOBJECT_DESC gxoDesc[5];
		POINT pt = { GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2, GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2 };	//---���� �ػ��� �߽���
		DWORD dwPickNum = g_pExecutive->GXOGetMultipleObjectWithScreenCoord( gxoDesc, 5, &pt, 0, /*PICK_TYPE_PER_FACE*/PICK_TYPE_PER_COLLISION_MESH ); //---�ӵ� �켱

		VECTOR3 CameraPos = m_CameraDesc[m_CurCamera].m_CameraPos;
		VECTOR3 HeroHead;
		if( m_CameraDesc[m_CurCamera].m_bPivotObject )
			m_CameraDesc[m_CurCamera].m_pPivotObject->GetPosition(&HeroHead);
		else
			HeroHead = m_CameraDesc[m_CurCamera].m_PivotPos;
				
		HeroHead.y += m_CameraDesc[m_CurCamera].m_CharHeight;

		float distCamera = sqrt( ( CameraPos.x - HeroHead.x ) * ( CameraPos.x - HeroHead.x ) +
								 ( CameraPos.y - HeroHead.y ) * ( CameraPos.y - HeroHead.y ) +
								 ( CameraPos.z - HeroHead.z ) * ( CameraPos.z - HeroHead.z ) );

		for( DWORD i = 0 ; i < dwPickNum ; ++i )
		{
			if( gxoDesc[i].fDist + 300.f < distCamera )	//--- �ӵ����(PICK_TYPE_PER_COLLISION_MESH)�� �Ͽ��� ������, ��ġ �ʴ� ����� ���� �� �ִ�. 300.f�� ���۸� ����.
			{
				if(gxoDesc[i].gxo)
				{
					CEngineObject* pEngineObject = (CEngineObject*)g_pExecutive->GetData(gxoDesc[i].gxo);
					if( pEngineObject == NULL )	//map object�� ��쿣 EngineObject�� ����
					{
						int setAlpha = g_pExecutive->GetAlphaFlag(gxoDesc[i].gxo) - (interval*2);
						if( setAlpha < 100 )
						{
							setAlpha = 100;	//--- �ּ� ���ļ�ġ = 0
						}

						g_pExecutive->SetAlphaFlag(gxoDesc[i].gxo,setAlpha);

						if( m_AlphaHandleList.Find(gxoDesc[i].gxo) == NULL )	//---����Ʈ�� ���ٸ� ����Ʈ�� �߰�.
						{
							m_AlphaHandleList.AddTail(gxoDesc[i].gxo);
						}
					}
				}
			}
		}

		//---����Ʈ�� ��� ������Ʈ�� ���ĸ� �����־� ���� ���·� ���ư��� ���ش�.
		//---������ interval*2 �� ���ĸ� �ι�� ���ִ� ������, ���⼭ interval��ŭ �����ֱ� �����̴�.
		PTRLISTPOS pos = m_AlphaHandleList.GetHeadPosition();
		while(pos)
		{
			PTRLISTPOS oldPos = pos;
			GXOBJECT_HANDLE gxHandle = (GXOBJECT_HANDLE)m_AlphaHandleList.GetNext(pos);

			if( gxHandle == NULL )
			{
				m_AlphaHandleList.RemoveAt( oldPos );
				continue;
			}

			DWORD curAlpha = g_pExecutive->GetAlphaFlag(gxHandle);
			DWORD setAlpha = curAlpha + interval;
			if( setAlpha >= 255 )
			{
				setAlpha = 255;
				m_AlphaHandleList.RemoveAt( oldPos );
			}
			g_pExecutive->SetAlphaFlag( gxHandle, setAlpha );			
		}

		dwLastCheckTime = gCurTime;
	}
}
//-----------------------------

void CMHCamera::Process()
{
	ProcessCameraMoving();				// ī�޶� �̵�ó��
	ProcessCameraMode();				// ī�޶� ��忡 ���� ó��
	RestrictCameraAngleDistance();		// ���� ���� ó��
	CalcCameraPosAngle();				// ī�޶� ��ġ ���� ���
	CalcVibration();					// ���� ��� ó��(������ �и��°�)
	CalcShaking();						// ���� ��� ó��(�����̳� ���︲��)
	SetToEngineCamera();				// ���� ������ ����

	// �̵��̸� ��������
	if(m_GotoPosition != -1)
		Interpolation();

	//---KES Map GXObject Alpha 071020		
	GXOAlphaProcess();
	//----------------------------

/* test
	else
	{
		if(m_CurPosition == eCP_CHARSEL)
		{
			m_CurPosition = eCP_CHARSEL;
			CAMERA->MoveStart(eCP_CHARMAKE);
		}
		else
		{
			m_CurPosition = eCP_CHARMAKE;
			CAMERA->MoveStart(eCP_CHARSEL);
		}
	}
//*/
}
void CMHCamera::Interpolation()
{
	float anX, anY;
	VECTOR3 Pos;
	float mtime = (gCurTime - m_MoveStartTime)* 0.001f;
	if(mtime > 1.f)		mtime = 1.f;
	float di = (float)sin(gPHI*mtime*0.5f);

	anX = m_MovePos[m_CurPosition].m_AngleX + m_MovePos[2].m_AngleX*di;
	anY = m_MovePos[m_CurPosition].m_AngleY + m_MovePos[2].m_AngleY*di;
	
// ��ü �̳༮�� ��ü�� ���ôٳ�.. �Ѥ� �̰� �̻���..
/*	anX.SetAngleDeg(m_MovePos[m_CurPosition].m_AngleX + m_MovePos[2].m_AngleX*di);
	anY.SetAngleDeg(m_MovePos[m_CurPosition].m_AngleY + m_MovePos[2].m_AngleY*di);	
	m_EngineCamera.SetCameraAngle(anX.ToRad(), anY.ToRad());*/


/*	VECTOR3 Angle;
	Angle.x = m_MovePos[m_CurPosition].m_AngleX + m_MovePos[2].m_AngleX*di;
	Angle.y = m_MovePos[m_CurPosition].m_AngleY + m_MovePos[2].m_AngleY*di;
	Angle.z = 0;
	m_EngineCamera.SetCameraAngle(&Angle);*/


// �̰� ���� Ȯ���� �ʿ�
//	if(m_CurPosition == eCP_CHARMAKE)		di = sin(gPHI*mtime*0.5f);
//	else									di = cos(gPHI*mtime*0.5f);

	Pos.x = m_MovePos[m_CurPosition].m_Pivot.x + m_MovePos[2].m_Pivot.x*di;
	Pos.y = m_MovePos[m_CurPosition].m_Pivot.y + m_MovePos[2].m_Pivot.y*di;
	Pos.z = m_MovePos[m_CurPosition].m_Pivot.z + m_MovePos[2].m_Pivot.z*di;
//	m_EngineCamera.SetCameraPos(&Pos);

	// �̰ɷ� ��ü.. �Ѥ�a
	InitCamera(m_CurCamera, anX, anY, 2000, &Pos);

	if(mtime == 1.f)
	{
		m_CurPosition = m_GotoPosition;
		m_GotoPosition = -1;
		m_bMoved = TRUE;
	}
}

void CMHCamera::ResetInterPolation()
{
	m_bMoved = FALSE;
	m_CurPosition = eCP_CHARSEL;
	m_GotoPosition = -1;
}

void CMHCamera::SetMovePos(int Index, MHCAMERAPOS* pCameraPos)
{
	memcpy(&m_MovePos[Index], pCameraPos, sizeof(MHCAMERAPOS));
}

void CMHCamera::MoveStart(int GotoIndex)
{
	m_GotoPosition = GotoIndex;
	m_MoveStartTime = (float)gCurTime;

	float dx = m_MovePos[m_CurPosition].m_Pivot.x - m_MovePos[m_GotoPosition].m_Pivot.x;
	float dy = m_MovePos[m_CurPosition].m_Pivot.y - m_MovePos[m_GotoPosition].m_Pivot.y;
	float dz = m_MovePos[m_CurPosition].m_Pivot.z - m_MovePos[m_GotoPosition].m_Pivot.z;	
	float Dist = sqrtf(dx*dx + dy*dy + dz*dz);

	m_MoveTime = Dist / m_MoveSpeed;

	m_MovePos[2].m_AngleX = m_MovePos[m_GotoPosition].m_AngleX - m_MovePos[m_CurPosition].m_AngleX;
	m_MovePos[2].m_AngleY = m_MovePos[m_GotoPosition].m_AngleY - m_MovePos[m_CurPosition].m_AngleY;
	m_MovePos[2].m_Dist = m_MovePos[m_GotoPosition].m_Dist - m_MovePos[m_CurPosition].m_Dist;
	m_MovePos[2].m_Pivot.x = m_MovePos[m_GotoPosition].m_Pivot.x - m_MovePos[m_CurPosition].m_Pivot.x;
	m_MovePos[2].m_Pivot.y = m_MovePos[m_GotoPosition].m_Pivot.y - m_MovePos[m_CurPosition].m_Pivot.y;
	m_MovePos[2].m_Pivot.z = m_MovePos[m_GotoPosition].m_Pivot.z - m_MovePos[m_CurPosition].m_Pivot.z;
}


void CMHCamera::SetCameraFilter(CEngineObject* pEngineObj,float minAngle,float maxAngle)
{
	m_pCameraFilter = pEngineObj;
	m_MinAngle = minAngle;
	m_MaxAngle = maxAngle;
}

void CMHCamera::CalcCameraPosAngle()
{
	MHCAMERADESC* pCurCamera = &m_CameraDesc[m_CurCamera];

	static float x_rad = -1;
	static float y_rad = -1;
	static float lastdist = -1;
	static VECTOR3 HeroPos;
	float curx = pCurCamera->m_AngleX.ToRad();
	float cury = -pCurCamera->m_AngleY.ToRad();
	static float fHeight = 0;
	
	VECTOR3 pos;
	if(pCurCamera->m_bPivotObject)
		pCurCamera->m_pPivotObject->GetPosition(&pos);
	else
		pos = pCurCamera->m_PivotPos;


	if( x_rad == curx &&
		y_rad == cury &&
		HeroPos.x == pos.x &&
		HeroPos.y == pos.y &&
		HeroPos.z == pos.z &&
		lastdist == pCurCamera->m_fDistance &&
		!pCurCamera->m_bHeightChanging &&
		fHeight == m_fAddHeight && m_CurCameraMode != eCM_EyeView)
	{
		return;
	}
	

	x_rad = curx;
	y_rad = cury;
	HeroPos = pos;
	lastdist = pCurCamera->m_fDistance;

	fHeight = m_fAddHeight;

	if(m_pCameraFilter)
	{
		float tempx = x_rad > gPHI ? x_rad - 2*gPHI : x_rad;
		float alpha;
		if(tempx > m_MaxAngle)
			alpha = 0;
		else if(tempx < m_MinAngle)
			alpha = 1;
		else
			alpha = (m_MaxAngle - tempx) / (m_MaxAngle - m_MinAngle);
		m_pCameraFilter->SetAlpha(alpha);
	}
	
	MATRIX4 mx,my,mtotal;
	SetRotationXMatrix(&mx,x_rad);
	SetRotationYMatrix(&my,y_rad);
	MatrixMultiply2(&mtotal,&mx,&my);

	VECTOR3 dir;
	dir.x = 0;
	dir.y = 0;
	dir.z = 1;
	TransformVector3_VPTR2(&pCurCamera->m_CameraDir,&dir,&mtotal,1);
	TransformVector3_VPTR2(&pCurCamera->m_CameraDownVector,&m_DownVector,&mtotal,1);

	if( m_CurCameraMode == eCM_EyeView )
	{
//		pCurCamera->m_CameraPos = HeroPos;
//		pCurCamera->m_CameraPos.y += pCurCamera->m_CharHeight + m_fAddHeight;

		CalcFreeCamera();
	}
	else
	{
		pCurCamera->m_CameraPos = HeroPos - pCurCamera->m_CameraDir * pCurCamera->m_fDistance;
		pCurCamera->m_CameraPos.y += pCurCamera->m_CharHeight;
	}

	pCurCamera->m_CameraDownPositon = pCurCamera->m_CameraPos + pCurCamera->m_CameraDownVector;

	///////////////////////////////////////////////////////////////////////////
	// Check Collison by Tile
	/*
	if(MAP->CollisonCheck(&HeroPos,&pCurCamera->m_CameraPos,&temp) == TRUE)
	{
		float dx,dz;
		dx = pCurCamera->m_CameraPos.x - HeroPos.x;
		dz = pCurCamera->m_CameraPos.z - HeroPos.z;
		float distOri = sqrtf(dx*dx + dz*dz);
		dx = temp.x - HeroPos.x;
		dz = temp.z - HeroPos.z;
		float distCha = sqrtf(dx*dx + dz*dz);
		float fRate = distCha/distOri;

		float fHeight = pCurCamera->m_CameraPos.y - HeroPos.y;
		pCurCamera->m_CameraPos.x = temp.x;
		pCurCamera->m_CameraPos.y = fHeight * fRate;
		pCurCamera->m_CameraPos.z = temp.z;
	}
	*/

	// ���࿡ �ɸ��Ϳ��� ī�޶�� ���� ���̸� ����� ���� �浹�Ѵٸ�
	// �� �浹 ��ǥ�� m_CameraPos�� �����Ѵ�.
	float OriDist, ColDist;
	VECTOR3* pColPos;
	VECTOR3 HeroHead = HeroPos;

	HeroHead.y += pCurCamera->m_CharHeight;
	pColPos = m_EngineCamera.IsCollisonBetweenCamAndHero(&pCurCamera->m_CameraDownPositon,&HeroHead, &OriDist, &ColDist );
	if(pColPos && m_CurCameraMode != eCM_LOGIN)
	{
		if( ColDist < 260 )
			ColDist = 260;	//�ʹ� ���� �Ǵ°� ����:

		VECTOR3 HeroToCamDir = pCurCamera->m_CameraPos - HeroHead;
		HeroToCamDir = HeroToCamDir * ColDist / OriDist;
		pCurCamera->m_CameraPos = HeroHead + HeroToCamDir;
	}

}


void CMHCamera::CalcFreeCamera()
{
	MHCAMERADESC* pCurCamera = &m_CameraDesc[m_CurCamera];

	// ��
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_W ) )
	{
		pCurCamera->m_CameraPos = pCurCamera->m_CameraPos + pCurCamera->m_CameraDir*40.f;
	}
	// ��
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_S ) )
	{
		pCurCamera->m_CameraPos = pCurCamera->m_CameraPos - pCurCamera->m_CameraDir*40.f;
	}
	// ��
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Q ) )
	{
		pCurCamera->m_CameraPos.y += 15.f;
	}
	// ��
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_E ) )
	{
		pCurCamera->m_CameraPos.y -= 15.f;
	}
	// ��
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_A ) )
	{
		VECTOR3 vOut, tv1, tv2;
		tv1 = pCurCamera->m_CameraDir;
		tv2 = pCurCamera->m_CameraDir;
		tv1.y += 100.f;
		Normalize(&tv1, &tv1);		
		CrossProduct(&vOut, &tv1, &tv2);
		pCurCamera->m_CameraPos = pCurCamera->m_CameraPos - vOut*40.f;
	}
	// ��
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_D ) )
	{
		VECTOR3 vOut, tv1, tv2;
		tv1 = pCurCamera->m_CameraDir;
		tv2 = pCurCamera->m_CameraDir;
		tv1.y += 100.f;
		Normalize(&tv1, &tv1);
		CrossProduct(&vOut, &tv1, &tv2);
		pCurCamera->m_CameraPos = pCurCamera->m_CameraPos + vOut*40.f;
	}

	// �� ���� �ʵ���..
	VECTOR3 pos;
	pCurCamera->m_pPivotObject->GetPosition( &pos );
	if(pCurCamera->m_CameraPos.y < pos.y+140.f)
		pCurCamera->m_CameraPos.y = pos.y+140.f;
	
	BASEMOVE_INFO moveinfo;
	moveinfo.bMoving = FALSE;				
	moveinfo.KyungGongIdx = 0;
	moveinfo.MoveMode = eMoveMode_Run;
	moveinfo.CurPosition.x = pCurCamera->m_CameraPos.x;
	moveinfo.CurPosition.z = pCurCamera->m_CameraPos.z;	
	MOVEMGR->InitMove(HERO, &moveinfo);

	// ������ ĳ������ �̵��� �˷� �׸��� ������ �����Ѵ�.	
	if( (gCurTime-m_LastMoveNoticeTime) >= 300 )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos( &pCurCamera->m_CameraPos );
		msg.SetTargetPos( &pCurCamera->m_CameraPos );		
		NETWORK->Send(&msg,msg.GetSize());
		
		m_LastMoveNoticeTime = gCurTime;
	}	
}


#define MOUSEROTATE_RATE 0.3f

void CMHCamera::MouseRotate(int x, int y)
{
	m_CameraDesc[m_CurCamera].m_AngleX.SetAngleDeg(m_CameraDesc[m_CurCamera].m_AngleX.ToDeg() - y * MOUSEROTATE_RATE);
	m_CameraDesc[m_CurCamera].m_AngleY.SetAngleDeg(m_CameraDesc[m_CurCamera].m_AngleY.ToDeg() + x * MOUSEROTATE_RATE);
}

void CMHCamera::SetCurCamera(int Camera)
{
	ASSERT(Camera >= 0 && Camera < MAX_CAMERA_DESC);
	m_CurCamera = Camera;
}

void CMHCamera::SetCameraPivotObject(int Camera,CObject* pObject)
{
	if(pObject)
		m_CameraDesc[Camera].m_bPivotObject = TRUE;
	else
		m_CameraDesc[Camera].m_bPivotObject = FALSE;
	m_CameraDesc[Camera].m_pPivotObject = pObject;
}
void CMHCamera::SetCameraPivotPos(int Camera,VECTOR3* pPos)
{
	m_CameraDesc[Camera].m_PivotPos = *pPos;
}


void CMHCamera::Rotate(int Camera,float RotateAngle,DWORD Duration)
{
	if(m_CameraDesc[Camera].m_bCameraAngleMovingY)	// ȸ�����̾��ٸ�
	{
		m_CameraDesc[Camera].m_AngleY.Process(1);
	}

	m_CameraDesc[Camera].m_AngleY.SetRotateDeg(RotateAngle);
	m_CameraDesc[Camera].m_bCameraAngleMovingY = TRUE;
	m_CameraDesc[Camera].m_CameraAngleMoveStartY = gCurTime;
	m_CameraDesc[Camera].m_CameraAngleMoveDurationY = Duration;	
}

void CMHCamera::SetTargetAngleX(int Camera,float fAngleX,DWORD Duration)
{
	if(m_CameraDesc[Camera].m_bCameraAngleMovingX)	// ȸ�����̾��ٸ�
	{
		m_CameraDesc[Camera].m_AngleX.Process(1);
	}
	
	m_CameraDesc[Camera].m_AngleX.SetTargetDeg(fAngleX);
	m_CameraDesc[Camera].m_bCameraAngleMovingX = TRUE;
	m_CameraDesc[Camera].m_CameraAngleMoveStartX = gCurTime;
	m_CameraDesc[Camera].m_CameraAngleMoveDurationX = Duration;
}

void CMHCamera::SetTargetAngleY(int Camera,float fAngleY,DWORD Duration)
{
	if(m_CameraDesc[Camera].m_bCameraAngleMovingY)	// ȸ�����̾��ٸ�
	{
		m_CameraDesc[Camera].m_AngleY.Process(1);
	}
	
	m_CameraDesc[Camera].m_AngleY.SetTargetDeg(fAngleY);
	m_CameraDesc[Camera].m_bCameraAngleMovingY = TRUE;
	m_CameraDesc[Camera].m_CameraAngleMoveStartY = gCurTime;
	m_CameraDesc[Camera].m_CameraAngleMoveDurationY = Duration;
}

void CMHCamera::SetDistance(int Camera,float fDistance,DWORD Duration)
{
	float fChange = fDistance - m_CameraDesc[Camera].m_fDistance;
	ZoomDistance(Camera,fChange,Duration);
}

void CMHCamera::ZoomDistance(int Camera,float fDistanceChange,DWORD Duration)
{
	m_CameraDesc[Camera].m_fDistanceStart = m_CameraDesc[Camera].m_fDistance;
	m_CameraDesc[Camera].m_fDistanceChange = fDistanceChange;
	m_CameraDesc[Camera].m_bCameraDistanceMoving = TRUE;
	m_CameraDesc[Camera].m_CameraDistanceMoveStart = gCurTime;
	m_CameraDesc[Camera].m_CameraDistanceMoveDuration = Duration;
}

// ���� ī�޶󿡼� �ٸ� ī�޶�� �ε巴�� �̵��Ѵ�.
void CMHCamera::ChangeCameraSmooth(int Camera,DWORD Duration)
{
	MHCAMERADESC tempdesc;
	memcpy(&tempdesc,&m_CameraDesc[Camera],sizeof(MHCAMERADESC));
	memcpy(&m_CameraDesc[Camera],&m_CameraDesc[m_CurCamera],sizeof(MHCAMERADESC));
	StopAllMoving(Camera);
	SetTargetAngleX(Camera,tempdesc.m_AngleX.ToDeg(),Duration);
	SetTargetAngleY(Camera,tempdesc.m_AngleY.ToDeg(),Duration);
	
}
	
// ���� ī�޶��� �������� �״�� 0�� ī�޶� �����Ѵ�.
void CMHCamera::ChangeToDefaultCamera()
{
	memcpy(&m_CameraDesc[0],&m_CameraDesc[m_CurCamera],sizeof(MHCAMERADESC));
	StopAllMoving(0);
	SetCurCamera(0);
}


// ȸ���̳� �ܵ��� �����
void CMHCamera::StopAllMoving(int Camera)
{
	m_CameraDesc[Camera].m_bCameraAngleMovingX = FALSE;
	m_CameraDesc[Camera].m_bCameraAngleMovingY = FALSE;
	m_CameraDesc[Camera].m_bCameraDistanceMoving = FALSE;
}



//////////////////////////////////////////////////////////////////////////
// ��������� ���� �Լ���
void CMHCamera::SetPushedVibration(VECTOR3* PushedVector,DWORD Duration)
{
	SetVector3(&m_VibratePos,0,0,0);
	SetVector3(&m_VibrateAngle,0,0,0);
	m_PushedVector = *PushedVector;
	m_VibrateStartTime = gCurTime;
	m_VibrateDuration = Duration;
	m_bIsVibrating = TRUE;
}

void CMHCamera::CalcVibration()
{
	if(m_bIsVibrating == FALSE)
		return;

	DWORD elapsedTime = gCurTime - m_VibrateStartTime;
	float fElapsedRate = elapsedTime / (float)m_VibrateDuration;
	
	if(fElapsedRate >= 1)
	{
		fElapsedRate = 1;
		m_bIsVibrating = 0;
	}

	m_VibratePos = m_PushedVector * (1 - fElapsedRate);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ����ŷ ����� ���� �Լ���
void CMHCamera::SetShaking(int ShakeNum, float amp)
{
	ASSERT(ShakeNum >= 0);
	ASSERT(ShakeNum < eCSD_Max);

	m_bIsShaking = TRUE;
	m_nCurShakingNum = ShakeNum;
	m_CurShakingPos = 0;
	m_LastShakingPosChangedTime = gCurTime;
	m_Amplitude = amp;
}

void CMHCamera::CalcShaking()
{
	if(m_bIsShaking == FALSE)
		return;

	DWORD elapsedTime = gCurTime - m_LastShakingPosChangedTime;
	MHCAMERASHAKE* pCurShake = m_ppCameraShakeDesc[m_nCurShakingNum];
	SHAKEVECTOR* pCurVector = &pCurShake->pShakeVector[m_CurShakingPos];
	if(elapsedTime >= pCurVector->Duration)
	{
		m_LastShakingPosChangedTime = gCurTime;
		if(++m_CurShakingPos >= pCurShake->MaxShakeVector)
		{
			m_bIsShaking = FALSE;
			return;
		}

		pCurVector = &pCurShake->pShakeVector[m_CurShakingPos];
	}

	m_ShakingPos = pCurVector->ShakeVector * m_Amplitude;
}
//////////////////////////////////////////////////////////////////////////

void CMHCamera::SetCameraMode(int CameraMode)
{
	m_CurCameraMode = CameraMode;
	switch(CameraMode)
	{
	case eCM_Free:
		{
			//SetDistance( m_CurCamera, 1000, 0 );
			m_CameraDesc[m_CurCamera].m_fDistance += 1;
			m_fAddHeight = 0;
		}
		break;
	case eCM_QuarterView:
		{
			SetTargetAngleX(0,30,500);
			SetTargetAngleY(0,0,500);
			SetDistance(0,1000,500);			
		}
		break;
	case eCM_FollowView:
		{
//			float angle;
//			if(m_CameraDesc[0].m_bPivotObject)
//				angle = m_CameraDesc[0].m_pPivotObject->GetAngle();
//			else
//				angle = 0;

//			SetTargetAngleX(0,DEFAULT_ANGLEX_FOLLOW_VIEW,500);
//			SetTargetAngleY(0,DEFAULT_ANGLEY_FOLLOW_VIEW+RADTODEG(angle),500);
//			SetDistance(0,DEFAULT_DISTANCE_FOLLOW_VIEW,500);
		}
		break;
	case eCM_EyeView:
		{
			m_CameraDesc[m_CurCamera].m_fDistance -= 1;
			m_fAddHeight = 0;
		}
		break;
	}
}


void CMHCamera::ToggleCameraViewMode()
{
	if( m_CurCameraMode == eCM_Free )
		SetCameraMode( eCM_FollowView );
	else
		SetCameraMode( eCM_Free );
}


void CMHCamera::ProcessCameraMode()
{
	switch(m_CurCameraMode)
	{
	case eCM_Free:
		{

		}
		break;
	case eCM_QuarterView:
		{		
		}
		break;
	case eCM_FollowView:
		{
			if(m_CameraDesc[0].m_bPivotObject == FALSE)
				break;
			
//			if(MOVEMGR->IsMoving(m_CameraDesc[0].m_pPivotObject) == FALSE)
//			{
//				break;
//			}
			if( m_CameraDesc[0].m_bCameraAngleMovingX || 
				m_CameraDesc[0].m_bCameraAngleMovingY || 
				m_CameraDesc[0].m_bCameraDistanceMoving)
				break;	

			if( !g_UserInput.GetMouse()->RButtonPressed()
				&& !g_UserInput.GetKeyboard()->GetKeyPressed( KEY_LEFT )
				&& !g_UserInput.GetKeyboard()->GetKeyPressed( KEY_RIGHT ) )
				FollowCamera();
		}
		break;
	case eCM_EyeView:
		{
		}
		break;
	}
}

void CMHCamera::ProcessCameraMoving()
{
	MHCAMERADESC* pDesc;
	for(int n=0;n<MAX_CAMERA_DESC;++n)
	{
		pDesc = &m_CameraDesc[n];

		if(pDesc->m_bCameraAngleMovingX)
		{
			float fRate;
			DWORD MoveTime = gCurTime - pDesc->m_CameraAngleMoveStartX;
			if(pDesc->m_CameraAngleMoveDurationX == 0)
				fRate = 1.0f;
			else
				fRate = MoveTime / (float)pDesc->m_CameraAngleMoveDurationX;
			if(fRate >= 1.0f)
			{
				fRate = 1.0f;
				pDesc->m_bCameraAngleMovingX = FALSE;
			}
			pDesc->m_AngleX.Process(fRate);
		}
		if(pDesc->m_bCameraAngleMovingY)
		{
			float fRate;
			DWORD MoveTime = gCurTime - pDesc->m_CameraAngleMoveStartY;
			if(pDesc->m_CameraAngleMoveDurationY == 0)
				fRate = 1.0f;
			else
				fRate = MoveTime / (float)pDesc->m_CameraAngleMoveDurationY;
			if(fRate >= 1.0f)
			{
				fRate = 1.0f;
				pDesc->m_bCameraAngleMovingY = FALSE;
			}
			pDesc->m_AngleY.Process(fRate);
		}
		if(pDesc->m_bCameraDistanceMoving)
		{
			float fRate;
			DWORD MoveTime = gCurTime - pDesc->m_CameraDistanceMoveStart;
			if(pDesc->m_CameraDistanceMoveDuration == 0)
				fRate = 1.0f;
			else
				fRate = MoveTime / (float)pDesc->m_CameraDistanceMoveDuration;
			if(fRate >= 1.0f)
			{
				fRate = 1.0f;
				pDesc->m_bCameraDistanceMoving = FALSE;
			}
			pDesc->m_fDistance = pDesc->m_fDistanceStart + pDesc->m_fDistanceChange*fRate;
		}
		if(pDesc->m_bHeightChanging)
		{
			float fRate;
			DWORD MoveTime = gCurTime - pDesc->m_dwHeightStartTime;
			if(pDesc->m_dwHeightChangeDuration == 0)
				fRate = 1.0f;
			else
				fRate = MoveTime / (float)pDesc->m_dwHeightChangeDuration;
			if(fRate >= 1.0f)
			{
				fRate = 1.0f;
				pDesc->m_bHeightChanging = FALSE;
			}
			pDesc->m_CharHeight = pDesc->m_StartCharHeight + (pDesc->m_TargetCharHeight - pDesc->m_StartCharHeight)*fRate;
		}
	}
}

void CMHCamera::RestrictCameraAngleDistance()
{
	if(m_CurCameraMode == eCM_LOGIN)
		return;

	MHCAMERADESC* pDesc = &m_CameraDesc[m_CurCamera];
	
#ifndef _TESTCLIENT_
	//////////////////////////////////////////////////////////////////////////
	// AngleX
	float angleX = pDesc->m_AngleX.ToDeg();
	if(angleX > 180)
		angleX -= 360;

	if(angleX < GAMERESRCMNGR->m_GameDesc.CameraMinAngleX)
	{
		pDesc->m_AngleX.SetAngleDeg(GAMERESRCMNGR->m_GameDesc.CameraMinAngleX);
	}
	else if(angleX > GAMERESRCMNGR->m_GameDesc.CameraMaxAngleX)
	{
		pDesc->m_AngleX.SetAngleDeg(GAMERESRCMNGR->m_GameDesc.CameraMaxAngleX);
	}


	//////////////////////////////////////////////////////////////////////////
	// AngleY

	//////////////////////////////////////////////////////////////////////////
	// Distance
	if(pDesc->m_fDistance < GAMERESRCMNGR->m_GameDesc.CameraMinDistance)
	{
		pDesc->m_fDistance = GAMERESRCMNGR->m_GameDesc.CameraMinDistance;
	}
	if(pDesc->m_fDistance > GAMERESRCMNGR->m_GameDesc.CameraMaxDistance)
	{
		pDesc->m_fDistance = GAMERESRCMNGR->m_GameDesc.CameraMaxDistance;
	}
#endif
}

void CMHCamera::FollowCamera()
{
	float fRate = gTickTime * 0.005f;
	if(fRate > 1)
		fRate = 1;
	
	MHCAMERADESC* pDesc = &m_CameraDesc[0];
	float angle = pDesc->m_pPivotObject->GetAngle();
	
//	pDesc->m_AngleX.SetTargetDeg(DEFAULT_ANGLEX_FOLLOW_VIEW);
//	pDesc->m_AngleX.Process(fRate);

	pDesc->m_AngleY.SetTargetDeg(DEFAULT_ANGLEY_FOLLOW_VIEW+RADTODEG(angle));
	pDesc->m_AngleY.Process(fRate);

//	float target = DEFAULT_DISTANCE_FOLLOW_VIEW;
//	pDesc->m_fDistance = target * fRate + pDesc->m_fDistance * (1-fRate);
}


void CMHCamera::SetSightDistanceRate( float fDistanceRate )
{
	if( m_fDistanceRate == fDistanceRate )	return;

	m_fDistanceRate = fDistanceRate;
	m_EngineCamera.ResetCamera( m_fMaxSightDistance * (float)(100-(100-fDistanceRate)*1.5) / 100.0f, m_fFov );	
}

void CMHCamera::SetCharState( int eCharState, DWORD dwDuration )
{
	MHCAMERADESC* pCurCamera = &m_CameraDesc[m_CurCamera];

	pCurCamera->m_bHeightChanging	= TRUE;
	pCurCamera->m_dwHeightStartTime = gCurTime;
	pCurCamera->m_dwHeightChangeDuration = dwDuration;
	pCurCamera->m_StartCharHeight = pCurCamera->m_CharHeight;
	switch( eCharState )
	{
	case eCS_Sit:
		pCurCamera->m_TargetCharHeight = SITHEIGHT;
		break;
	case eCS_Die:
		pCurCamera->m_TargetCharHeight = DIEHEIGHT;
		break;
	case eCS_Normal:
		pCurCamera->m_TargetCharHeight = CHARHEIGHT;		
		break;
	default:
		pCurCamera->m_TargetCharHeight = CHARHEIGHT;
		break;
	}
}

void CMHCamera::Fly( float fFly )
{
	m_fAddHeight += fFly/2;
	if( m_fAddHeight > 5000 )
		m_fAddHeight = 5000;
	else if( m_fAddHeight < 0 )
	{
		m_fAddHeight = 0;
	}
}
