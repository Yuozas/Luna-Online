// DamageNumber.cpp: implementation of the CDamageNumber class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamageNumber.h"
#include "..\Engine\Engine.h"
#include "GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cImageSelf* CDamageNumber::m_pNumberImage[eDNK_Max];
//cImageSelf* CDamageNumber::m_pMissImage;
cImageSelf* CDamageNumber::m_pCriticalImage;
VECTOR2		CDamageNumber::m_CriticalImgSize;
cImageSelf* CDamageNumber::m_pDodgeImage;

void CDamageNumber::NumberData::Draw(float fAlpha,VECTOR2* pos)
{
	VECTOR2 scale;
	scale.x = 1;
	scale.y = 1;
	if(fAlpha == 2)
	{
		scale.x = 1.5f;
		scale.y = 1.5f;
	}
	DWORD Color = COLORtoDWORD(1,1,1,fAlpha);

	VECTOR2 dispos;
	dispos.x = spos.x + pos->x;
	dispos.y = spos.y + pos->y;
	
	pImage->RenderSprite(
		&scale,NULL,0,&dispos,Color);
}
void CDamageNumber::NumberData::SetImage(cImageSelf* p,VECTOR2* pos)
{
	spos = *pos;
	pImage = p;
}


CDamageNumber::CDamageNumber()
{
	m_bDraw = FALSE;
}

CDamageNumber::~CDamageNumber()
{
	delete[] m_pNumberImage[eDNK_Yellow];
	delete[] m_pNumberImage[eDNK_Green];
	delete[] m_pNumberImage[eDNK_Red];
	delete m_pCriticalImage;
	delete m_pDodgeImage;
}


BOOL CDamageNumber::LoadImage()
{
	char szfile[64];

	DeleteImage();

	m_pNumberImage[eDNK_Yellow] = new cImageSelf[10];
	for(int n=0;n<10;++n)
	{
		sprintf(szfile,"./Data/Interface/2DImage/image/y%d.tif",n);
		m_pNumberImage[eDNK_Yellow][n].LoadSprite(szfile);
	}
	
	m_pNumberImage[eDNK_Green] = new cImageSelf[10];
	for(n=0;n<10;++n)
	{
		sprintf(szfile,"./Data/Interface/2DImage/image/g%d.tif",n);
		m_pNumberImage[eDNK_Green][n].LoadSprite(szfile);
	}
	
	m_pNumberImage[eDNK_Red] = new cImageSelf[10];
	for(n=0;n<10;++n)
	{
		sprintf(szfile,"./Data/Interface/2DImage/image/r%d.tif",n);
		m_pNumberImage[eDNK_Red][n].LoadSprite(szfile);
	}

//	sprintf(szfile,"./Data/Interface/2DImage/Damage/miss.tif",n);
//	m_pMissImage = new cImageSelf;
//	m_pMissImage->LoadSprite(szfile);

	sprintf(szfile,"./Data/Interface/2DImage/image/critical.tif");
	m_pCriticalImage = new cImageSelf;
	m_pCriticalImage->LoadSprite(szfile);
	m_pCriticalImage->GetImageOriginalSize(&m_CriticalImgSize);

	sprintf(szfile,"./Data/Interface/2DImage/image/dodge.tif");
	m_pDodgeImage = new cImageSelf;
	m_pDodgeImage->LoadSprite(szfile);

	return TRUE;
}

void CDamageNumber::DeleteImage()
{
	SAFE_DELETE_ARRAY(m_pNumberImage[eDNK_Yellow]);
	SAFE_DELETE_ARRAY(m_pNumberImage[eDNK_Green]);
	SAFE_DELETE_ARRAY(m_pNumberImage[eDNK_Red]);
//	SAFE_DELETE(m_pMissImage);
	SAFE_DELETE(m_pCriticalImage);
	SAFE_DELETE(m_pDodgeImage);
}

void CDamageNumber::SetDodge(VECTOR3* pPos,VECTOR3* pVelocity)
{
	m_bDodge = TRUE;
	m_bCritical = FALSE;
	m_bDecisive = FALSE;
		
	m_CreatedTime = gCurTime;
	m_fAlpha = 1;

	m_Position = *pPos;
	m_Direction = *pVelocity;

	float charwidth = 24;
	float charheight = 12;

	m_Damage = 0;
	m_Jarisu = 0;

	m_bDraw = TRUE;
}
void CDamageNumber::SetDamage(DWORD Damage,VECTOR3* pPos,VECTOR3* pVelocity,BYTE nDamageNumberKind,BOOL bCritical,BOOL bDecisive)
{
	// desc_hseos_ASSERT����_01
	// S ASSERT���� �߰� added by hseos 2007.06.01
	// ..������ ��ġ�� ������������ Ŭ ��� �Ʒ��� ���ڸ� ��� �κп��� ���ѷ������� ������ �߻��� �� �ִ�.
	// ..�������� 9999999 �� �Ѿ�� ȭ����� ǥ�ô� ������ 9999999 �� �ؼ� Ŭ���̾�Ʈ ������ ���� �ʰ� �Ѵ�.
 	if (Damage > 9999999)
 	{
		Damage = 9999999;
	}
	// E ASSERT���� �߰� added by hseos 2007.06.01

	m_bDodge = FALSE;
	m_bCritical = bCritical;
	m_bDecisive = bDecisive;

	m_CreatedTime = gCurTime;
	m_fAlpha = 1;

	m_Position = *pPos;
	
	if(m_bCritical)
	{		
//		VECTOR3 temp;
		m_PositionCritical = m_Position;
	}

	m_Direction = *pVelocity;

	float charwidth = 24;
	float charheight = 12;

	m_Damage = Damage;

	// ���ڸ� ������ ���Ѵ�
	m_Jarisu = 0;
	DWORD tens=1;
	while(tens<=m_Damage)
	{
		tens*=10;
		++m_Jarisu;
	}
	if(m_Damage == 0)
	{
		/*
		VECTOR2 ScreenPos;
		float MissWidth = 50;
		float MissHeight= 15;
		ScreenPos.x = -MissWidth*m_Jarisu*0.5f;
		ScreenPos.y = -MissHeight*0.5f;

		m_Jarisu = 1;
		m_Numbers[0].SetImage(m_pMissImage,&ScreenPos);
		m_bDraw = TRUE;
		*/
		return;
	}

	ASSERT(m_Jarisu <= MAX_DAMAGE_JARISU);


	// ���ڸ��� ���� ���ؼ� image�� ��ǥ�� �����Ѵ�.
	tens /= 10;
	DWORD eachnum;
	VECTOR2 ScreenPos;
	ScreenPos.x = -charwidth*m_Jarisu*0.5f;
	ScreenPos.y = -charheight*0.5f;

	for(DWORD n=0;n<m_Jarisu;++n)
	{
		eachnum = Damage / tens;
		ASSERT(eachnum<10);
		m_Numbers[n].SetImage(&m_pNumberImage[nDamageNumberKind][eachnum],&ScreenPos);

		Damage = Damage % tens;
		tens /= 10;
		ScreenPos.x += charwidth;
	}

	m_bDraw = TRUE;
}

BOOL CDamageNumber::Render()
{
	if(m_bDraw == FALSE)
		return FALSE;
	if(m_fAlpha == 0)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// ������
	float fVelocity = 1.8f;
	float TotalTime = 700.f;
	float HighestTime = 150.f;
	float AlphaStartTime = 150.f;
	float HighestHeight = 0.0f;
	float LeanAngle = 0.001f;
	float Elapsedtime = (float)(gCurTime - m_CreatedTime);
	float RealElapsedtime = Elapsedtime;

	if(Elapsedtime > TotalTime)
	{
		m_bDraw = FALSE;
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ����
	if(Elapsedtime > AlphaStartTime+ 200.f)
	{
		float ttt = Elapsedtime - AlphaStartTime;
		m_fAlpha = 1 - ttt / (TotalTime - AlphaStartTime);
	}
	else
	{
		m_fAlpha = 1;
	}

	if(Elapsedtime > AlphaStartTime )
	{
		float Des = Elapsedtime - AlphaStartTime;
		Elapsedtime = AlphaStartTime + Des*0.1f;
	}
	
	VECTOR3 pos;
	float cury = (float)(-fVelocity*LeanAngle*(Elapsedtime - HighestTime)*(Elapsedtime - HighestTime) + 10.f*Elapsedtime*0.02);
	pos = m_Position;
	pos.y += cury;
	pos.x = m_Position.x + Elapsedtime * m_Direction.x * fVelocity;
	pos.z = m_Position.z + Elapsedtime * m_Direction.z * fVelocity;
	
	VECTOR3 Temp;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(),0,&pos,&Temp);
	if(Temp.x < 0 || Temp.x > 1 || Temp.y < 0 || Temp.y > 1)
	{
		return TRUE;
	}

	VECTOR2 ScreenPos;
	ScreenPos.x = GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*Temp.x;
	ScreenPos.y = GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*Temp.y;

	if(m_bDodge)
	{
		DWORD Color = COLORtoDWORD(1,1,1,m_fAlpha);
		m_pDodgeImage->RenderSprite(NULL,NULL,0,&ScreenPos,Color);
	}
	for(DWORD n=0;n<m_Jarisu;++n)
	{
		m_Numbers[n].Draw(m_fAlpha,&ScreenPos);
	}
	

	//////////////////////////////////////////////////////////////////////////
	// Critical
	// 06. 07. ���� ����(�ϰ�) - �̿���
	if(m_bCritical || m_bDecisive)
	{
		static float CriticalOverHeight = 45 + 20;
		GetScreenXYFromXYZ(g_pExecutive->GetGeometry(),0,&m_Position,&Temp);
		ScreenPos.x = GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*Temp.x;
		ScreenPos.y = GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*Temp.y;
		ScreenPos.y -= CriticalOverHeight;
		static float CRISCALETIME = 100;
		static float CRIALPHTTIME = 1000;
		static float SCALE = 0.5;
		DWORD Color;
		if(RealElapsedtime < CRIALPHTTIME)
			Color = COLORtoDWORD(1,1,1,(CRIALPHTTIME-RealElapsedtime)/CRIALPHTTIME);
		else
			Color = 0;
		VECTOR2 scale;
		if(RealElapsedtime < CRISCALETIME)
			scale.x = scale.y = 1 + SCALE * (CRISCALETIME-RealElapsedtime)/CRISCALETIME;
		else
			scale.x = scale.y = 1;
		ScreenPos.x = ScreenPos.x - (m_CriticalImgSize.x*0.5f*scale.x);
		ScreenPos.y = ScreenPos.y - (m_CriticalImgSize.y*0.5f*scale.y);
		m_pCriticalImage->RenderSprite(&scale,NULL,0,&ScreenPos,Color);
	}

	return TRUE;
}