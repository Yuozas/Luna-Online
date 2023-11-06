#include "stdafx.h"
#include "GameIn.h"
#include "GameResourceManager.h"
#include "WindowIDEnum.h"
#include "cStatic.h"
#include "cScriptManager.h"
#include "cResourceManager.h"
#include "cRITEMEx.h"
#include "..\effect\DamageNumber.h"
#include "../hseos/Date/SHChallengeZoneClearNo1Dlg.h"

CSHChallengeZoneClearNo1Dlg::CSHChallengeZoneClearNo1Dlg()
{
	m_csChallengeZoneLimitTime1.Init( 24, 0 );
	m_csChallengeZoneLimitTime2.Init( 24, 0 );
	for(int i=0; i<10; i++)
	{
		m_csChallengeZoneLimitTime1.InitDamageNumImage(CDamageNumber::GetImage(eDNK_Red, i), i);
		m_csChallengeZoneLimitTime2.InitDamageNumImage(CDamageNumber::GetImage(eDNK_Red, i), i);
	}

	m_csChallengeZoneLimitTime1.SetFillZero( TRUE );
	m_csChallengeZoneLimitTime1.SetScale( 1.0f, 1.0f );
	m_csChallengeZoneLimitTime1.SetLimitCipher( 2 );

	m_csChallengeZoneLimitTime2.SetFillZero( TRUE );
	m_csChallengeZoneLimitTime2.SetScale( 1.0f, 1.0f );
	m_csChallengeZoneLimitTime2.SetLimitCipher( 2 );

	m_nChallengeZoneStartTime = 0;
	m_nChallengeZoneStartTimeTick = 0;
}

CSHChallengeZoneClearNo1Dlg::~CSHChallengeZoneClearNo1Dlg()
{
}

void CSHChallengeZoneClearNo1Dlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );
}

void CSHChallengeZoneClearNo1Dlg::Linking()
{
	m_csSection = (cStatic*)GetWindowForID(DMD_CHALLENGEZONE_CLEARNO1_SECTION);
	m_csName1	= (cStatic*)GetWindowForID(DMD_CHALLENGEZONE_CLEARNO1_NAME1);
	m_csName2	= (cStatic*)GetWindowForID(DMD_CHALLENGEZONE_CLEARNO1_NAME2);
	m_csTime	= (cStatic*)GetWindowForID(DMD_CHALLENGEZONE_CLEARNO1_TIME);

	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	VECTOR2 stPos;
	stPos.x = (float)dispInfo.dwWidth - 80;
	stPos.y = 8.0f;

	m_csChallengeZoneLimitTime1.SetPosition((int)stPos.x, (int)stPos.y);
	m_csChallengeZoneLimitTime2.SetPosition((int)stPos.x + 60, (int)stPos.y);

	m_csChallengeZoneLimitTime1.SetNumber(0);
	m_csChallengeZoneLimitTime2.SetNumber(0);
} 


void CSHChallengeZoneClearNo1Dlg::Render()
{
 	if( !IsActive() ) return;

 	cDialog::Render();

	// 챌린지 존 시작 후 시간이 흐를 떄
  	if (m_nChallengeZoneStartTimeTick)
	{
		// 제한 시간
		int nPastTime = (m_nChallengeZoneStartTime-(gCurTime - m_nChallengeZoneStartTimeTick)) / 1000;

		m_csChallengeZoneLimitTime1.SetNumber(nPastTime/60);
		m_csChallengeZoneLimitTime1.RenderWithDamageNumImage();
		m_csChallengeZoneLimitTime2.SetNumber(nPastTime%60);
		m_csChallengeZoneLimitTime2.RenderWithDamageNumImage();

		if (nPastTime <= 0)
		{
			m_nChallengeZoneStartTimeTick = 0;
			m_nChallengeZoneStartTime = 0;
		}
	}
	// 챌린지 존에 입장하고 아직 시작 전 카운트 다운 중일 떄
	else if (m_nChallengeZoneStartTime)
	{
		// 제한 시간
		int nPastTime = (m_nChallengeZoneStartTime) / 1000;

		m_csChallengeZoneLimitTime1.SetNumber(nPastTime/60);
		m_csChallengeZoneLimitTime1.RenderWithDamageNumImage();
		m_csChallengeZoneLimitTime2.SetNumber(nPastTime%60);
		m_csChallengeZoneLimitTime2.RenderWithDamageNumImage();
	}
	else
	{
		m_csChallengeZoneLimitTime1.RenderWithDamageNumImage();
		m_csChallengeZoneLimitTime2.RenderWithDamageNumImage();
	}
}
