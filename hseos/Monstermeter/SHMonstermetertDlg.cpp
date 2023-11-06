#include "stdafx.h"
#include "SHMonstermeterDlg.h"
#include "GameIn.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "cStatic.h"
#include "..\effect\DamageNumber.h"
#include "ObjectBalloon.h"

#include "GMNotifyManager.h"
#include "./interface/cFont.h"

CSHMonstermeterDlg::CSHMonstermeterDlg()
{
	m_GiftEventTimeNumber.Init( 12, 0 );
	for(int i=0; i<10; i++)
	{
		m_GiftEventTimeNumber.InitDamageNumImage(CDamageNumber::GetImage(eDNK_Yellow, i), i);
	}
	m_GiftEventTimeNumber.SetFillZero( TRUE );
	m_GiftEventTimeNumber.SetScale( 0.5f, 0.5f );
	m_GiftEventTimeNumber.SetLimitCipher( 2 );

	m_ImageNumber.Init( 12, 0 );
	for(int i=0; i<10; i++)
	{
		m_ImageNumber.InitDamageNumImage(CDamageNumber::GetImage(eDNK_Yellow, i), i);
	}
	m_ImageNumber.SetFillZero( FALSE );
	m_ImageNumber.SetScale( 0.5f, 0.5f );
	m_ImageNumber.SetLimitCipher( 6 );

	m_PlayTimeText = NULL;
	m_KillMonNumText = NULL;

	m_GiftEventTime = 0;
}

CSHMonstermeterDlg::~CSHMonstermeterDlg()
{
}
void CSHMonstermeterDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );
}

void CSHMonstermeterDlg::Linking()
{
 	m_PlayTimeText = (cStatic*)GetWindowForID(MONSTERMETER_DLG_PLAYTIME);
	m_KillMonNumText = (cStatic*)GetWindowForID(MONSTERMETER_DLG_KILLMONNUM);
	//m_GiftTimeText1 = (cStatic*)GetWindowForID(MONSTERMETER_DLG_GIFTTIME1);
	//m_GiftTimeText2 = (cStatic*)GetWindowForID(MONSTERMETER_DLG_GIFTTIME2);
	//m_GiftTimeText3 = (cStatic*)GetWindowForID(MONSTERMETER_DLG_GIFTTIME3);
}

void CSHMonstermeterDlg::Render()
{
 	if( !IsActive() ) return;

	cDialog::Render();

	// ���� ų ����
	m_ImageNumber.SetPosition((int)m_PlayTimeText->GetAbsX(), (int)m_PlayTimeText->GetAbsY());
	m_ImageNumber.SetNumber(HERO->GetMonstermeterInfo()->nPlayTimeTotal);
	m_ImageNumber.RenderWithDamageNumImage();

	m_ImageNumber.SetPosition((int)m_KillMonNumText->GetAbsX(), (int)m_KillMonNumText->GetAbsY());
 	m_ImageNumber.SetNumber(HERO->GetMonstermeterInfo()->nKillMonsterNumTotal);
	m_ImageNumber.RenderWithDamageNumImage();

	DWORD hour = m_GiftEventTime / ( 60 * 60 * 1000 );
	DWORD minute = ( m_GiftEventTime / ( 60 * 1000 ) ) % 60;
	DWORD second = ( m_GiftEventTime /  1000 ) % 60;
/*
	m_GiftEventTimeNumber.SetPosition((int)m_GiftTimeText3->GetAbsX(), (int)m_GiftTimeText3->GetAbsY());
	m_GiftEventTimeNumber.SetNumber(hour);
	m_GiftEventTimeNumber.RenderWithDamageNumImage();

	m_GiftEventTimeNumber.SetPosition((int)m_GiftTimeText2->GetAbsX(), (int)m_GiftTimeText2->GetAbsY());
	m_GiftEventTimeNumber.SetNumber(minute);
	m_GiftEventTimeNumber.RenderWithDamageNumImage();

	m_GiftEventTimeNumber.SetPosition((int)m_GiftTimeText1->GetAbsX(), (int)m_GiftTimeText1->GetAbsY());
	m_GiftEventTimeNumber.SetNumber(second);
	m_GiftEventTimeNumber.RenderWithDamageNumImage();
*/
	// ���� ������ Ÿ��
 	if (m_csFarmRePlantTimeBar.CLI_IsStarted())
	{
		VECTOR2 stPos;
		stPos.x = GetAbsX() - 36*2 - 2 - 2;
		stPos.y = GetAbsY() + 1;

		m_csFarmRePlantTimeBar.CLI_Render(&stPos);
	}
	if (m_csFarmReManureTimeBar.CLI_IsStarted())
	{
		VECTOR2 stPos;
		stPos.x = GetAbsX() - 36 - 2;
		stPos.y = GetAbsY() + 1;

		m_csFarmReManureTimeBar.CLI_Render(&stPos);
	}

    if( NOTIFYMGR->IsEventNotifyUse() )
	{
		if( NOTIFYMGR->GetEventNotifyContext() == NULL || NOTIFYMGR->GetEventNotifyTitle() == NULL ) return;

		RECT rect={ GetAbsX(), GetAbsY() + GetHeight() + 5 , 1, 1 };
		int nStrLen = strlen( NOTIFYMGR->GetEventNotifyTitle() );
		CFONT_OBJ->RenderFontShadow( 0, 1, NOTIFYMGR->GetEventNotifyTitle(), nStrLen, &rect, RGBA_MAKE(255,255,255,255) );
		rect.top += 15;
		
		nStrLen = strlen( NOTIFYMGR->GetEventNotifyContext() );
		int nSecLineLen = 0;
		if( nStrLen > 23 )
		{
			char* p = NOTIFYMGR->GetEventNotifyContext() + 23;	//���� ��
			if( p != CharNext( CharPrev( NOTIFYMGR->GetEventNotifyContext(), p ) ) ) //�������� ���� 2����Ʈ���� �߰��̶��
			{
				nSecLineLen = nStrLen - 22;
				nStrLen = 22;
			}
			else
			{
				nSecLineLen = nStrLen - 23;
				nStrLen = 23;
			}
		}

		CFONT_OBJ->RenderFontShadow( 0, 1, NOTIFYMGR->GetEventNotifyContext(), nStrLen, &rect, RGBA_MAKE(255,255,255,255) );
		if( nSecLineLen )
		{
			rect.top += 15;
			CFONT_OBJ->RenderFontShadow( 0, 1, NOTIFYMGR->GetEventNotifyContext() + nStrLen, nSecLineLen, &rect, RGBA_MAKE(255,255,255,255) );
		}
	}
}
