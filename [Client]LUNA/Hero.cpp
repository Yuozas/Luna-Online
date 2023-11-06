#include "stdafx.h"
#include "Hero.h"
#include "GameIn.h"
#include "MHCamera.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
//#include "MinimapDialog.h"
#include "ItemManager.h"
#include "CharacterCalcManager.h"
#include "KyungGongManager.h"
#include "PartyManager.h"
#include "cSkillTreeManager.h"
#include "ObjectGuagen.h"
#include "./Audio/MHAudioManager.h"
#include "StreetStallManager.h"
#include "CheatMsgParser.h"
#include "interface/cWindowManager.h"
//#include "OptionManager.h"
#include "WantedManager.h"
#include "ChatManager.h"


#include "WindowIdEnum.h"

#include "ReviveDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "DealDialog.h"
#include "CharStateDialog.h"
#include "ProgressDialog.h"

#include "mhMap.h"
#include "GameEventManager.h"

#include "BattleSystem_Client.h"
#include "NpcScriptDialog.h"
#include "CommonCalcFunc.h"
#include "AppearanceManager.h"

#include "SkillPointRedist.h"
#include "SkillPointNotify.h"

#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "MoveDialog.h"

#include "PeaceWarModeManager.h"
#include "GuageDialog.h"
#include "MussangManager.h"
#include "WeatherManager.h"
#include "InventoryExDialog.h"
#include "StorageDialog.h"

#include "QuickDlg.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/status/status.h"

#include "../[cc]skill/client/info/skillinfo.h"
#include "../[cc]skill/client/info/activeskillinfo.h"
#include "../[cc]skill/client/info/buffskillinfo.h"


#include "NpcImageDlg.h"
#include "cskilltrainingdlg.h"
#include "cSkillBase.h"
#include "cSkillTreeDlg.h"

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
#include "../hseos/Common/SHMath.h"
// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.12
#include "../hseos/Family/SHFamily.h"
// E �йи� �߰� added by hseos 2007.07.12

#ifdef _DEBUG
#include "interface\cFont.h"
#include "ChatManager.h"
#endif

#include "ObjectBalloon.h"
#include "../hseos/Farm/SHFarmManager.h"

#include "PKManager.h"
#include "StreetStallTitleTip.h"

VECTOR3 gHeroPos;


CHero::CHero()
{
	//m_NextAction.Clear();
	m_CurComboNum = 0;
	m_maxExpPoint = 0;
	m_StorageNum = 0;	
	m_bUngiFlag = FALSE;
	mStatusTable.Initialize( 10 );
	mBuffSkillList.Initialize( 10 );

	m_matchedQuantity = 0;

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.12
	m_pcsFamily = new CSHFamily;
	// E �йи� �߰� added by hseos 2007.07.12
	
	// 071210 LYW --- Hero : HERO�� Ȯ�� �κ��丮 ���� ��� ���� �߰�.
	m_byExtendedInvenCount = 0 ;

//	ZeroMemory(m_szStallSearch, sizeof(m_szStallSearch));
}

CHero::~CHero()
{
	ITEMMGR->RemoveCoolTime( GetID() );

	Release();
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.12
	SAFE_DELETE(m_pcsFamily)
	// E �йи� �߰� added by hseos 2007.07.12
}

void CHero::InitHero(HERO_TOTALINFO * pHeroTotalInfo)
{	
	m_bSkillCooling = 0;
	m_MovingAction.Clear();
	m_NextAction.Clear();
	m_SkillStartAction.Clear();
	m_bIsAutoAttackMode = TRUE;
	m_bIsAutoAttacking = FALSE;
	m_bIsKyungGongMode = FALSE;

	memcpy(&m_HeroTotalInfo, pHeroTotalInfo, sizeof(HERO_TOTALINFO));

	m_maxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
	MUSSANGMGR->SetMussangReady(FALSE);
	MUSSANGMGR->SetMussangMaxPoint(pHeroTotalInfo->MaxMussangPoint);
	//	m_HeroTotalInfo.KyungGongGrade = OPTIONMGR->GetKyungGongIdx();	//***
	//	m_bIsKyungGongMode = OPTIONMGR->GetKyungGongMode();				//***

	//�������̽��� ����
	//ApplyInterface(); RegisterHero�� �ű�
	ZeroMemory( &mItemStat, sizeof( mItemStat ) );

	// 080313 LUJ, ��Ʈ ������ �ɷ� �ʱ�ȭ
	ZeroMemory( &mSetItemStat, sizeof( mSetItemStat ) );

	m_dwLastSocietyAction = gCurTime;

	GAMEIN->GetQuickDlg()->SetActiveSheet( 0, 0 );

	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
	ZeroMemory(&m_stMonstermeterInfo, sizeof(m_stMonstermeterInfo));
	m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
	// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

	mpCurrentSkill = NULL;


//---KES ��� ���� ���� ���� 071128
	m_bNoDeffenceFromArmor = FALSE;
//----------------------------------

//---KES ���󰡱�
	m_dwFollowPlayerIdx = 0;
//---------------
}


#define SKILL_PERSUIT_TICK	1000

void CHero::Process()
{
	static DWORD	dwSkillPersuitTime	= 0;

	CPlayer::Process();

	CAMERA->Process();	//? Gamin�� ���μ����� �ִ�. KES 040406 //��.. ���⼭ ���ϴ� ī�޶� ��鸰��.
	//	WEATHERMGR->Process();
	if(m_MovingAction.HasAction())
	{
		if(m_MovingAction.CheckTargetDistance(&GetCurPosition()) == TRUE)
		{
			MOVEMGR->HeroMoveStop();

			m_MovingAction.ExcuteAction(this);
			m_MovingAction.Clear();
		}
		else
		{
			if( m_MovingAction.GetActionKind() == eActionKind_Skill )
			{
				if( gCurTime - dwSkillPersuitTime > SKILL_PERSUIT_TICK )
				{
					m_MovingAction.ExcuteAction(this);
					dwSkillPersuitTime = gCurTime;
				}
			}
			else if( MOVEMGR->IsMoving(this) == FALSE )
			{
				m_MovingAction.Clear();
			}
		}
	}
	else
	{
		if(m_bIsAutoAttacking)
		{
			if(m_NextAction.HasAction() == FALSE)
			{
				if(GetCurComboNum() < 3 || GetCurComboNum() == SKILL_COMBO_NUM)	// �ڵ������� �޺� 2������	12/3�� ȸ�� ��� 3���� 2�� �ٲ�
				{
					if(SKILLMGR->OnSkillCommand(this,&m_AutoAttackTarget,FALSE) == FALSE)
						DisableAutoAttack();
				}
			}
			else
			{
				m_NextAction.ExcuteAction(this);
			}
		}
	}

	// ���
	if(m_bIsKyungGongMode)
	{
		if( MOVEMGR->IsMoving(this) &&
			m_MoveInfo.KyungGongIdx == 0 &&
			GetKyungGongGrade() &&
			GetMoveSpeed() != 0 &&
			GetState() != eObjectState_Ungijosik)			
		{
			if(m_MoveInfo.m_bEffectMoving == FALSE)
			{
				// RaMa - 05.09.05 - ��������̰� ����.
				if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
					MOVEMGR->ToggleHeroKyungGongMode();
				else
				{
#ifdef TAIWAN_LOCAL
					MOVEMGR->ToggleHeroKyungGongMode();
#else
					CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(GetKyungGongGrade());
					DWORD ElapsedTime = gCurTime - m_ObjectState.State_Start_Time;
					if(ElapsedTime > pKGInfo->GetChangeTime())
					{
						MOVEMGR->ToggleHeroKyungGongMode();
					}
#endif		// TAWAN_LOCAL
				}
			}
		}
	}

	// ��ų ��Ÿ��
	if(m_bSkillCooling)
	{
		if(gCurTime - m_SkillCoolTimeStart > m_SkillCoolTimeDuration)
			m_bSkillCooling = FALSE;
	}

	VECTOR3 pos;
	float angle;
	GetPosition(&pos);
	angle = GetAngle();

	// �׸��ڿ� ����Ʈ �̵��� ����	
	MAP->SetShadowPivotPos(&pos);

	// 3D ������ Listener ������ ����
	AUDIOMGR->SetListenerPosition(&pos,angle);

	// �׾����� ������ �ð��� ������ â ����.
	//	if(GetState() == eObjectState_Die)
	//	{	
	//		if( gCurTime - GetStateStartTime() > PLAYERREVIVE_TIME ) //���� �ð��� ����
	//		{
	//if( ) �񹫷� �׾���. �ƴϴ�.
	//Ȯ�ο��! confirm						//process�� ���ȴٸ� NONE�� ���� �ִ�.
	//			if( BATTLESYSTEM->GetBattle(this)->GetBattleKind() == eBATTLE_KIND_NONE )
	//			{
	//				WANTEDMGR->SetActiveDialog();
	//			}
	//		}
	//	}

	if(GetState() == eObjectState_SkillUsing)
	{
		if(gCurTime - GetStateStartTime() > 10000)	//���ڸ� ���̱⿣ �����κ��� �ɸ���.
		{
			OBJECTSTATEMGR->EndObjectState(this,eObjectState_SkillUsing);
			//////////////////////////////////////////////////////////////////////////
			// 06. 04. ��ų ���� ���� - �̿���
			// ��ų ��� ���¸� ������ �����Ҷ� �޼����� ���޾�
			// �������� ���� �ӽý�ų��ü�� �����ش�.
			//			cSkillObject* pSObj = SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID);
			//			if(pSObj)
			//				SKILLMGR->DeleteTempSkill();
			//////////////////////////////////////////////////////////////////////////
		}
	}

	gHeroPos = pos;

	// Play Time ���	
	m_HeroTotalInfo.Playtime  += gTickTime/1000;

	//SW050810 ��ȭ��� �ڵ� ��ȯ
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->DoCheckForPeaceMode(this);

	if( mSkillCancelDelay.IsStart() )
	{
		if( !mSkillCancelDelay.Check() )
		{
			mpCurrentSkill = NULL;
		}
	}

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
	ProcMonstermeterPlayTime();
	// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

	//---KES PK 071202
	if( GetState() == eObjectState_Die )	//���� ���¿��� �ð��� �Ȱ���.
	{
		PKMGR->SetPKStartTimeReset();
	}
	//----------------

	// 080229 NYJ --- ���ӿɼ�(�׸���-�ڽ�) üũ�� ���� �ɼ��� ������ ������� ���� ���׸� ���� �Ʒ��ڵ带 ����.
	OBJECTMGR->ApplyShadowOption(OBJECTMGR->GetHero());
}

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
void CHero::ProcMonstermeterPlayTime()
{
	if (gCurTime - m_stMonstermeterInfo.nPlayTimeTick > SHMath_MINUTE(1))
	{
		m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
		m_stMonstermeterInfo.nPlayTime++;
		m_stMonstermeterInfo.nPlayTimeTotal++;
	}
}
// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

void CHero::GetHeroTotalInfo(HERO_TOTALINFO * info)
{
	ASSERT(info);
	memcpy(info, &m_HeroTotalInfo, sizeof(HERO_TOTALINFO));
}


void CHero::ApplyInterface()
{
#ifndef _TESTCLIENT_
	SetMoney(m_HeroTotalInfo.Money);
	SetLevel(m_CharacterInfo.Level);
	SetMaxLife(m_CharacterInfo.MaxLife);
	SetLife(m_CharacterInfo.Life, 0);
	SetMaxMana(m_HeroTotalInfo.MaxMana);
	SetMana(m_HeroTotalInfo.Mana, 0);
	SetMaxExpPoint(m_maxExpPoint);

	SetExpPoint(m_HeroTotalInfo.ExpPoint);

	SetStrength(m_HeroTotalInfo.Str);
	SetDexterity(m_HeroTotalInfo.Dex);
	SetVitality(m_HeroTotalInfo.Vit);
	SetWisdom(m_HeroTotalInfo.Wis);
	SetIntelligence(m_HeroTotalInfo.Int);

	SetPartyIdx(m_HeroTotalInfo.PartyID);
	SetGuildMemberRank(m_CharacterInfo.PositionInMunpa);
	SetGuildIdx(m_CharacterInfo.MunpaID);
	SetGuildName(m_CharacterInfo.GuildName);

	SetGuageName(m_BaseObjectInfo.ObjectName);
	SetGuild();
	// 070503 LYW --- Hero : Setting family name.
	SetFamilyName(m_CharacterInfo.FamilyName) ;
	SetFamily() ;
	SetBadFame(m_CharacterInfo.BadFame);

	//SetStage(m_CharacterInfo.Stage);

	// RaMa
	RefreshLevelupPoint();

	// 070117 LYW --- Add function to setting character face.
	GAMEIN->GetGuageDlg()->MakeHeroFace() ;
#endif
}

void CHero::SetGuageName(char * szName)
{
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAME);
	if( ps )	ps->SetStaticText( szName );
}

void CHero::SetMaxLife(DWORD maxlife)
{
	CPlayer::SetMaxLife(maxlife);

	DWORD newMaxLife = GetMaxLife();
	if(newMaxLife == 0)
	{
		newMaxLife = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)m_CharacterInfo.Life/(float)newMaxLife, 0 );

	// �������̽� ������ ����
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);

	// �������̽� : ��ġ ǥ��
	char szValue[50];
	// 070122 LYW --- Modified this line. - _parsingKeywordString �Լ������� ��ĭ�� ������ ���� �߻�!!
	//sprintf( szValue, "%d / %d", GetLife(), newMaxLife );
	sprintf( szValue, "%d/%d", GetLife(), newMaxLife );

	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );	
}

void CHero::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{
	CHATMGR->AddMsg( CTC_GETMONEY, CHATMGR->GetChatMsg(778), pHealer->GetObjectName(), HealVal );
	EFFECTMGR->AddHealNumber(HealVal,pHealer,this,HealKind);
	ChangeLife(HealVal);
}
void CHero::ChangeLife(int changeval)
{
	// 070419 LYW --- Hero : modified function ChangeLife.
	// 080616 LUJ, �ִ� HP�� ���� �ʰ� ǥ�õǵ��� �Ѵ�
	DWORD newlife = min( GetMaxLife(), GetLife() + changeval );

	SetLife(newlife);
	/*	
	if( m_bUngiFlag == FALSE )
	if( newlife )
	if( (GetMaxLife()/newlife) > 2 )
	{
	m_bUngiFlag = TRUE;
	}
	*/	

	//SetLife(changeval);
}

void CHero::SetLife(DWORD life, BYTE type)
{
	CPlayer::SetLife(life);

	if(GetState() == eObjectState_Die)
		life = 0;

//---KES divide 0 ����
	DWORD MaxLife = GetMaxLife();
	if(MaxLife == 0)
	{
		MaxLife = 1;
	}
//-----------

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)life/(float)MaxLife, 
		(type == 0 ? 0 : (2000/MaxLife)*life) );

	GAMEIN->GetCharacterDialog()->SetLife(life);

/*
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)life/(float)GetMaxLife(), 
		(type == 0 ? 0 : (2000/GetMaxLife())*life) );

	GAMEIN->GetCharacterDialog()->SetLife(life);
*/



	// 070419 LYW --- Hero : Modified function SetLife.
	/*
	// �������̽� ������ ����
	DWORD MaxLife = GetMaxLife();
	if(MaxLife == 0)
	{
	MaxLife = 1;
	}


	if(GetState() == eObjectState_Die)
	life = 0;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
	(float)life/(float)MaxLife, 
	(type == 0 ? 0 : (2000/MaxLife)*life) );
	GAMEIN->GetCharacterDialog()->SetLife(life);
	*/
	// �������̽� : ��ġ ǥ��
	char szValue[50];
	// 070122 LYW --- Modified this line. - _parsingKeywordString �Լ������� ��ĭ�� ������ ���� �߻�!!
	//sprintf( szValue, "%d / %d", life, GetMaxLife() );
	sprintf( szValue, "%d/%d", life, GetMaxLife() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetMoney(MONEYTYPE Money)
{
	m_HeroTotalInfo.Money = Money;
	if(GAMEIN->GetInventoryDialog())
		GAMEIN->GetInventoryDialog()->SetMoney(Money);

	GAMEIN->GetSkillTrainingDlg()->SetGold();
}

void CHero::SetKyungGongGrade(WORD grade)
{	
	m_HeroTotalInfo.KyungGongGrade = grade;

	//	if(grade != 0)
	//		OPTIONMGR->SetKyungGongIdx(HERO->GetKyungGongGrade());//***
	GAMEIN->GetCharacterDialog()->UpdateData();
}

void CHero::SetMaxExpPoint(EXPTYPE dwPoint)
{
	m_maxExpPoint = dwPoint;

	float value = (float)GetExpPoint()/(float)m_maxExpPoint;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( value, 0 );
	// 070113 LYW --- Delete this line.
	//GAMEIN->GetCharacterDialog()->SetExpPointPercent( value*100 );

	// �������̽� : ��ġ ǥ��
	char szValue[50];
	// 070124 LYW --- Modified this line.
	//sprintf( szValue, "%d / %d", GetExpPoint(), dwPoint );
	sprintf( szValue, "%d/%d", GetExpPoint(), dwPoint );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetExpPoint(EXPTYPE dwPoint, BYTE type)
{
	m_HeroTotalInfo.ExpPoint = dwPoint;

	// �������̽� ������ ����
	float value = (float)dwPoint/(float)m_maxExpPoint;
	// 080530 LYW --- Hero : Modified a data type DWORD to EXPTYPE.
	DWORDEX valueTime = (type == 0)? 0 : (2000/m_maxExpPoint)*dwPoint;
	//DWORD valueTime = 0 ;

	// 080604 LYW --- Hero : ����ġ ������ ó�� ����.
	// ����ġ �������� time�� ���� �ʴ°� �´´ٰ� �Ǵ�.
	// type�� 0���� �����Ѵ�.
	type = 1 ;

	if(GetLevel() == MAX_CHARACTER_LEVEL_NUM || type == 0 )
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( 1, 0 );
	else
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( value, (DWORD)valueTime );

	// �������̽� : ĳ���� â
	CObjectGuagen* ps = (CObjectGuagen *)GAMEIN->GetCharacterDialog()->GetWindowForID(CI_EXPPOINTGUAGE);
	if( ps )
	{
		if(type == 0)
			ps->SetValue( value, 0 );
		else
			ps->SetValue( value, (DWORD)valueTime );
	}	

	// 070113 LYW --- Delete this line.
	//GAMEIN->GetCharacterDialog()->SetExpPointPercent(value*100);
	// �������̽� : ��ġ ǥ��

	// 080603 LYW --- Hero : ����ġ ó�� (__int32)���� (__int64)�� ���� ó��.
	//char szValue[50];
	//if(GetLevel() >= 99)
	//	sprintf( szValue, "[ %s ]", CHATMGR->GetChatMsg(179) );	//"[ �� ]"
	//else
	//	sprintf( szValue, "%.2f%%", value*100 );

//	#ifdef _CHEATENABLE_	
//	if(CHEATMGR->IsCheatEnable())
//	{
//		char buf1[66];
//		wsprintf(buf1, " %d/%d", dwPoint, GetMaxExpPoint());
//		strcat(szValue, buf1);
//	}
//	#endif

	char szValue[50];
	sprintf( szValue, "%.2f%%", value*100 );

#ifdef _CHEATENABLE_	
	if(CHEATMGR->IsCheatEnable())
	{
		char buf1[66];
		EXPTYPE exp = GetMaxExpPoint() ;
 		sprintf(buf1, " %0.f/%0.f", (float)dwPoint, (float)exp);
		strcat(szValue, buf1);
	}
#endif

	cStatic* ps2 = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps2 )	ps2->SetStaticText( szValue );
}

void CHero::SetLevel(LEVELTYPE level)
{
	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetLevel(level);

	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELEVEL);
	// 070117 LYW --- Modified this line.
	//if( ps )	ps->SetStaticValue( level );
	char tempStr[ 128 ] = {0, } ;
	// 070124 LYW --- Modified this line.
	//sprintf( tempStr, "Lv %d", level ) ;
	// 070417 LYW --- Hero : Modified Lv. to not use.
	//sprintf( tempStr, "Lv.%d", level ) ;
	sprintf( tempStr, "%d", level ) ;
	if( ps ) ps->SetStaticText( tempStr ) ;

	// �������� ����Ʈǥ��
	if(level > CPlayer::GetLevel())
	{
		EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);
	}

	CPlayer::SetLevel(level);

	//�� ��ȯ
	ITEMMGR->RefreshAllItem();
	SKILLTREEMGR->RefreshSkill();
	//	if( GAMEIN->GetQuickDlg() )
	//		GAMEIN->GetQuickDlg()->RefreshIcon();
}

void CHero::SetSkillPoint(DWORD point)
{
	m_HeroTotalInfo.SkillPoint += point;
	GAMEIN->GetSkillTrainingDlg()->SetSkillPoint();
	GAMEIN->GetSkillTreeDlg()->SetSkillPoint();
}

void CHero::SetMaxMana(DWORD val)
{
	m_HeroTotalInfo.MaxMana = val;

	// �������̽� ������ ����
	DWORD neMaxMana = GetMaxMana();
	if(neMaxMana == 0)
	{
		neMaxMana = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANA))->SetValue(
		(float)m_HeroTotalInfo.Mana/(float)neMaxMana, 0 );

	GAMEIN->GetCharacterDialog()->SetMana(m_HeroTotalInfo.Mana);

	// �������̽� : ��ġ ǥ��
	char szValue[50];
	// 070122 LYW --- Modified this line. - _parsingKeywordString �Լ������� ��ĭ�� ������ ���� �߻�!!
	//sprintf( szValue, "%d / %d", GetMana(), neMaxMana );
	sprintf( szValue, "%d/%d", GetMana(), neMaxMana );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANATEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetMana(DWORD val, BYTE type)
{
	CPlayer::SetMana(val);

	if(m_HeroTotalInfo.MaxMana != 0)
	{
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANA))->SetValue(
			(float)val/(float)GetMaxMana(), 
			( type == 0 ? 0 : (1000/GetMaxMana())*val) );
	}
	// 070419 LYW --- Hero : Modified function SetMana.
	/*
	if(val > GetMaxMana()) 
	{
		//		ASSERT(0);
		val = GetMana();
	}

	// �������̽� â ǥ�� ��ġ ����
	DWORD dMaxMana = GetMaxMana();
	//	if( dMaxMana == 0 )
	//		dMaxMana = 1;

	if(m_HeroTotalInfo.MaxMana != 0)
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANA))->SetValue(
		(float)val/(float)dMaxMana, 
		( type == 0 ? 0 : (1000/dMaxMana)*val) );

	GAMEIN->GetCharacterDialog()->SetMana(val);
	*/

		// �������̽� : ��ġ ǥ��
		char szValue[50];
	// 070122 LYW --- Modified this line. - _parsingKeywordString �Լ������� ��ĭ�� ������ ���� �߻�!!
	//sprintf( szValue, "%d / %d", val, dMaxMana );
	sprintf( szValue, "%d/%d", val, GetMaxMana() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANATEXT);
	if( ps )	ps->SetStaticText( szValue );

	// ��ġ����
	m_HeroTotalInfo.Mana = val;
}
void CHero::SetStrength(DWORD val)
{
	m_HeroTotalInfo.Str = val;

	//SW060906 ���۾����� ����
	CHARCALCMGR->CalcCharStats(this);
	//CHARCALCMGR->CalcCharStats(this);

	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetStrength();
	// 070411 LYW --- Hero : Modified function SetStrength.
	//GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->PrintAttackRate();

	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetDexterity(DWORD val)
{
	m_HeroTotalInfo.Dex = val;

	CHARCALCMGR->CalcCharStats(this);
	//CHARCALCMGR->CalcCharStats(this);


	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetDexterity();
	// 070411 LYW --- Hero : Modified function SetDexterity.
	//GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->PrintAccuracyRate();
	GAMEIN->GetCharacterDialog()->PrintEvasionRate();
	GAMEIN->GetCharacterDialog()->PrintCriticalRate();

	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetVitality(DWORD val)
{
	m_HeroTotalInfo.Vit = val;

	CHARCALCMGR->CalcCharStats(this);
	//CHARCALCMGR->CalcCharStats(this);

	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetVitality();
	// 070411 LYW --- Hero : Modified function SetVitality.
	//GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->PrintDefenseRate();

	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetWisdom(DWORD val)
{
	m_HeroTotalInfo.Wis = val;

	CHARCALCMGR->CalcCharStats(this);
	//CHARCALCMGR->CalcCharStats(this);

	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetWisdom();
	GAMEIN->GetCharacterDialog()->UpdateData();
	// 070411 LYW --- Hero : Modified function SetWisdom.
	//GAMEIN->GetCharacterDialog()->SetMana(GetMana());
	GAMEIN->GetCharacterDialog()->PrintMagDefenseRate() ;
	GAMEIN->GetCharacterDialog()->UpdateData();
}

// 070411 LYW --- Hero : Add function to setting intelligence to hero.
void CHero::SetIntelligence( DWORD val )
{
	m_HeroTotalInfo.Int = val;

	CHARCALCMGR->CalcCharStats(this);

	// �������̽� â ǥ�� ��ġ ����
	GAMEIN->GetCharacterDialog()->SetIntelligence();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->PrintMagAttackRate() ;
	GAMEIN->GetCharacterDialog()->UpdateData();
}

void CHero::SetGuild()
{
	GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
}

void CHero::SetFamily()
{
	GAMEIN->GetCharacterDialog()->RefreshFamilyInfo();
}

void CHero::SetFame(FAMETYPE val)
{
	m_HeroTotalInfo.Fame = val;
	GAMEIN->GetCharacterDialog()->SetFame(GetFame());
}

void CHero::SetBadFame( FAMETYPE val )
{
	m_CharacterInfo.BadFame = val;
	GAMEIN->GetCharacterDialog()->SetBadFame( val );	
}

BOOL CHero::StartSocietyAct( int nStartMotion, int nIngMotion, int nEndMotion, BOOL bHideWeapon )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Immortal )
	{
		if( GetState() == eObjectState_Society )
		{
			if( m_ObjectState.State_End_Motion != -1 )	//�ɱ� ������ ���
			{
				if( m_ObjectState.bEndState == TRUE )
					return FALSE;
				if( gCurTime - GetStateStartTime() < GetEngineObject()->GetAnimationTime(m_ObjectState.State_Start_Motion ) )
					return FALSE;

				OBJECTSTATEMGR->EndObjectState( this, GetState(), GetEngineObject()->GetAnimationTime( m_ObjectState.State_End_Motion ) );
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	m_ObjectState.State_Start_Motion	= (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion		= (short int)nIngMotion;
	m_ObjectState.State_End_Motion		= (short int)nEndMotion;
	m_ObjectState.MotionSpeedRate = 1.0f;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;


	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );

		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}


	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );

	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;

	if( m_ObjectState.State_Ing_Motion == -1 )
		OBJECTSTATEMGR->EndObjectState( this, eObjectState_Society, GetEngineObject()->GetAnimationTime( m_ObjectState.State_Start_Motion ) );

	return TRUE;
}

BOOL CHero::EndSocietyAct()
{
	if( m_ObjectState.State_Ing_Motion != -1 )
	{
		return FALSE;
	}
	return TRUE;
}

void CHero::StartSocietyActForQuest( int nStartMotion, int nIngMotion /* = -1 */, int nEndMotion /* = -1 */, BOOL bHideWeapon /* = FALSE  */ )
{
	m_ObjectState.State_Start_Motion = (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion = (short int)nIngMotion;
	m_ObjectState.State_End_Motion = (short int)nEndMotion;
	m_ObjectState.MotionSpeedRate = 1.0f;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;

	if( m_ObjectState.State_Start_Motion != -1 )
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );
		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}

	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );
}

#ifdef _GMTOOL_
const char* GetStateName( BYTE state )
{
	static char StateName[eObjectState_MAX][64] = {
	"eObjectState_None",						
	"eObjectState_Enter",						
	"eObjectState_Move",						
	"eObjectState_Ungijosik",					
	"eObjectState_Rest",						
	"eObjectState_Deal",						
	"eObjectState_Exchange",					
	"eObjectState_StreetStall_Owner",			
	"eObjectState_StreetStall_Guest",			
	"eObjectState_PrivateWarehouse",			
	"eObjectState_Munpa",						
	"eObjectState_SkillStart",				
	"eObjectState_SkillSyn",					
	"eObjectState_SkillBinding",				
	"eObjectState_SkillUsing",				
	"eObjectState_SkillDelay",				
	"eObjectState_TiedUp_CanMove",			
	"eObjectState_TiedUp_CanSkill",
	"eObjectState_TiedUp",
	"eObjectState_Die",						
	"eObjectState_BattleReady",
	"eObjectState_Exit",						
	"eObjectState_Immortal",					
	"eObjectState_Society",					
	"eObjectState_ItemUse",
	"eObjectState_TournamentReady",
	"eObjectState_TournamentProcess",
	"eObjectState_TournamentEnd",
	"eObjectState_TournamentDead",
	"eObjectState_Engrave",
	"eObjectState_RestDamage",
	"eObjectState_Connect"
	};

	return StateName[state];
}
#endif

void CHero::OnStartObjectState(BYTE State)
{
#ifdef _GMTOOL_1
	FILE* fp;

	fp = fopen( "HeroState.txt", "a+" );

	fprintf( fp, "StartState\t%d\t%s\n", State, GetStateName( State ) );

	fclose(fp);
#endif

	CPlayer::OnStartObjectState(State);
}
void CHero::OnEndObjectState(BYTE State)
{
#ifdef _GMTOOL_1
	FILE* fp;

	fp = fopen( "HeroState.txt", "a+" );

	fprintf( fp, "EndState\t%d\t%s\n", State, GetStateName( State ) );

	fclose(fp);
#endif

	switch(State)
	{
	case eObjectState_Enter:
		{
		}
		break;
	case eObjectState_Deal:
		{
			HERO->HeroStateNotify(eObjectState_Deal);
			GAMEIN->GetNpcImageDlg()->SetDisable( FALSE );
			//GAMEIN->GetNpcImageDlg()->SetActive( FALSE );
		}
		break;

	case eObjectState_SkillUsing:
	case eObjectState_SkillBinding:
		SKILLMGR->OnComboTurningPoint(this);
		break;

	case eObjectState_SkillSyn:
		{
#ifndef _TESTCLIENT_
//			if(SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID) != NULL)	// �������� ������ �ȿ°��
//				OBJECTSTATEMGR->StartObjectState(this,eObjectState_SkillUsing);
//			else				
				SKILLMGR->OnComboTurningPoint(this);
#else
			SKILLMGR->OnComboTurningPoint(this);
#endif
		}
		break;

	case eObjectState_Move:
		{
		}
		break;
	case eObjectState_SkillStart:
		{
			if(m_SkillStartAction.GetActionKind() != eActionKind_Skill_RealExecute)
				break;

			m_SkillStartAction.ExcuteAction(this);
			m_SkillStartAction.Clear();
		}
		break;
	}
	CPlayer::OnEndObjectState(State);
}

//////////////////////////////////////////////////////////////////////////
// �ൿ �Լ���..
void CHero::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	//	ASSERT(GetLife() == 0);

	WINDOWMGR->CloseAllMsgBox();

	GAMEIN->GetDealDialog()->SetDisable(FALSE);
	GAMEIN->GetDealDialog()->SetActive(FALSE);	// ����â �ݱ�

	GAMEIN->GetNpcScriptDialog()->SetDisable(FALSE);	//NPCâ�� �ݱ�
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);

	// ���α׷��� â �ݱ�
	{
		CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );

		if( dialog->IsActive() )
		{
			dialog->Cancel();
		}
	}

	//��ȭ ���� ��޾�â �ݱ� -- ���ô��� �� �� ������..
	/*
	if( GAMEIN->GetUpgradeDialog()->IsActive() &&
	!GAMEIN->GetUpgradeDialog()->IsDisable() )
	{
	GAMEIN->GetUpgradeDialog()->Release();
	GAMEIN->GetUpgradeDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetMixDialog()->IsActive() &&
	!GAMEIN->GetMixDialog()->IsDisable() )
	{
	GAMEIN->GetMixDialog()->Close();
	GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetReinforceDialog()->IsActive() &&
	!GAMEIN->GetReinforceDialog()->IsDisable() )
	{
	GAMEIN->GetReinforceDialog()->Release();
	GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetDissolveDialog()->IsActive() &&
	!GAMEIN->GetDissolveDialog()->IsDisable() )
	{
	GAMEIN->GetDissolveDialog()->SetActive( FALSE );
	}
	//
	if( GAMEIN->GetSkPointDlg()->IsActive() )
	{
	GAMEIN->GetSkPointDlg()->SetActive( FALSE );
	}
	*/
	SetMovingAction(NULL);
	DisableAutoAttack();					// ������� ����
	SetLife(0);

	// LBS 03.10.04 �׾����� �������� �ݴ´�.	//���⼭? ������ �ʼ������� ������ִ� ���� �����ٵ�?
	//	STREETSTALLMGR->CloseStreetStall();

	if( pAttacker )
		if(pAttacker->GetObjectKind() & eObjectKind_Monster )
		{
			//		GAMEEVENTMGR->AddEvent(eGameEvent1_HeroDie);
			if( GetLevel() < 5 )
				GAMEEVENTMGR->AddEvent(eGameEvent_Die);
			else
				GAMEEVENTMGR->AddEvent(eGameEvent_DieAfter5);
		}

		//	if(GAMEIN->GetCharStateDialog())
		//		GAMEIN->GetCharStateDialog()->SetUngiMode( FALSE );

		WORD weapon = GetWeaponEquipType();

		DWORD time = 0;
		if( bFatalDamage )
		{
			//time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Fly);

			time = HERO->GetEngineObject()->GetAnimationTime( CHARACTER_MOTION[ eCharacterMotion_Died2 ][ weapon ] );
		}
		else
		{
			//time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Normal);
			time = HERO->GetEngineObject()->GetAnimationTime( CHARACTER_MOTION[ eCharacterMotion_Died1 ][ weapon ] );
		}
		CAMERA->SetCharState( eCS_Die, time );

		CPlayer::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
		GAMEIN->GetCharacterDialog()->SetAttackRate();
		GAMEIN->GetCharacterDialog()->SetDefenseRate();
		GAMEIN->GetCharacterDialog()->UpdateData();		

		//	SKILLMGR->RemoveTarget(this, SKILLRESULTKIND_POSITIVE|SKILLRESULTKIND_NEGATIVE);
}

void CHero::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive)
{
	//////////////////////////////////////////////////////////////////////////
	// ī�޶� �ڷ� �и�
	VECTOR3 push;
	MATRIX4 my;
	SetVector3(&push,0,0,30);
	SetRotationYMatrix(&my,-m_RotateInfo.Angle.ToRad());
	TransformVector3_VPTR2(&push,&push,&my,1);
	CAMERA->SetPushedVibration(&push,200);
	//////////////////////////////////////////////////////////////////////////

	//---KES PK
	if( pAttacker )
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		PKMGR->SetPKStartTimeReset();
	}
	//---------

	CPlayer::Damage(pAttacker,DamageKind,Damage,bCritical,bDecisive);
}

void CHero::OnHitTarget(CObject* pMainTarget)
{
	//////////////////////////////////////////////////////////////////////////
	// ���ΰ��� ������ �Դ� ����Ÿ�ٰ� ������ ���ݾ� ����Ʈ��	
	VECTOR3 targetpos = pMainTarget->GetCurPosition();
	VECTOR3 Curpos = HERO->GetCurPosition();

	float WeaponMinDistance = 100;
	switch(GetWeaponEquipType())
	{
	case eWeaponType_Sword	:	WeaponMinDistance = 150;	break;
	case eWeaponType_Mace	:	WeaponMinDistance = 150;	break;
	case eWeaponType_Axe	:	WeaponMinDistance = 150;	break;
	case eWeaponType_Dagger	:	WeaponMinDistance = 100;	break;
	case eWeaponType_Staff	:	WeaponMinDistance = 150;	break;
	case eWeaponType_Bow	:	WeaponMinDistance = 150;	break;	
	case eWeaponType_Gun	:	WeaponMinDistance = 100;	break;	
		// desc_hseos_ASSERT����_01
		// S ASSERT���� �߰� added by hseos 2007.06.02
		// ..�Ǽ����� ������ ���� ������ �ƴϹǷ� �߰�
	case eWeaponType_None	:								break;
		// E ASSERT���� �߰� added by hseos 2007.06.02
	default: ASSERT(0);
	}

	if(CalcDistanceXZ(&targetpos,&Curpos) < WeaponMinDistance)
	{
		VECTOR3 pos;	pos.x = 0;	pos.y = 0;	pos.z = 10.f;
		float angle = HERO->GetAngle();
		TransToRelatedCoordinate(&pos,&pos,angle);
		pos = pos + Curpos;
		MOVEMGR->HeroEffectMove(HERO,&pos,0,0);
	}
}


void CHero::ClearTarget(CObject* pObject)
{
	if(m_AutoAttackTarget.GetTargetID() == pObject->GetID())
		DisableAutoAttack();
	if(m_NextAction.GetTargetID() == pObject->GetID())
	{
		m_NextAction.Clear();
	}
}


void CHero::Revive(VECTOR3* pRevivePos)
{
	DisableAutoAttack();

	CAMERA->SetCharState( eCS_Normal, 0 );	//�Ͼ�� ������ ����.

	CPlayer::Revive(pRevivePos);
}

BOOL CHero::GetKyungGongMode()
{
	return m_bIsKyungGongMode;
}

void CHero::SetKyungGongMode(BOOL bMode)
{
	m_bIsKyungGongMode = bMode;

	if( m_bIsKyungGongMode == FALSE )
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}
	else
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}

	//KES CHARSTATEDLG 031016
	if(GAMEIN->GetCharStateDialog())
		GAMEIN->GetCharStateDialog()->SetKyungGongMode( m_bIsKyungGongMode );
	//

	//	OPTIONMGR->SetKyungGongMode(m_bIsKyungGongMode);//***
}


int CHero::GetSkillLevel(DWORD SkillIdx)
{
	if(SkillIdx < 100)		// �޺�
		return 1;

	cSkillBase* pSkill = SKILLTREEMGR->GetSkill(SkillIdx);
	if(pSkill == NULL)
		return 0;

	return pSkill->GetLevel();
}
void CHero::AddStatus(cStatus* pStatus)
{
	mStatusTable.Add( pStatus, pStatus->GetKind() );
	/*
	CObject::AddStatus(pStatus);

	MOVEMGR->RefreshHeroMove();

	SetMaxLife(m_CharacterInfo.MaxLife);
	SetMaxMana(m_HeroTotalInfo.MaxMana);

	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	*/
}

void CHero::RemoveStatus(cStatus* pStatus)
{
	mStatusTable.Remove( pStatus->GetKind() );
	/*
	CObject::RemoveStatus(pStatus);

	MOVEMGR->RefreshHeroMove();

	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMANA))->SetMaxValue(GetMaxMana());
	GAMEIN->GetCharacterDialog()->SetMana(m_HeroTotalInfo.Mana);
	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetMaxValue(GetMaxLife());
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
	*/
}

cStatus* CHero::GetStatus( WORD kind )
{
	return mStatusTable.GetData( kind );
}

// 070412 LYW --- ���ݷ� ��ġ ó�� ( Ŭ���̾�Ʈ )
// 070411 LYW --- Hero : Add functions to setting physical part.
DWORD CHero::DoGetAttackRate() 
{
	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	DWORD wWeaponAttack		= DWORD( mItemStat.mPhysicAttack.mPlus * ( 1.0f + mItemStat.mPhysicAttack.mPercent ) );

	DWORD wEnchantAttack	= 0;
	DWORD wLevel			= GetLevel() ;
	float wPassiveSkill		= GetPassiveStatus()->PhysicAttack ;
	float fAttackA			= (float)( wWeaponAttack + wEnchantAttack + wLevel + wPassiveSkill + 3) ;
	
	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wStrengthAll		= ( mCharStat.mStrength.mPlus + mItemStat.mStrength.mPlus ) * ( 1.0f + mItemStat.mStrength.mPercent );
	float wStrengthAll		= ( mCharStat.mStrength.mPlus + mItemStat.mStrength.mPlus + GetBuffStatus()->Str) * ( 1.0f + mItemStat.mStrength.mPercent );

	// ���� ���� �� ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mStrength;

		wStrengthAll =	
			max( 0, ( wStrengthAll + value.mPlus ) * ( 1.0f + value.mPercent ) )  +
			GetPassiveStatus()->Str + 
			GetBuffStatus()->Str;
	}

	wStrengthAll *= 0.2f;

	float wMasteryLevel		= 0 ;

	switch( GetWeaponAniType() )
	{ 
	case eWeaponAnimationType_OneHand:	wMasteryLevel = GetPassiveStatus()->OneHanded;	break;
	case eWeaponAnimationType_TwoHand:	wMasteryLevel = GetPassiveStatus()->TwoHanded;	break;
	case eWeaponAnimationType_TwoBlade:	wMasteryLevel = GetPassiveStatus()->TwoBlade;	break;
	}

	switch( GetWeaponEquipType() )
	{
	case eWeaponType_Sword:		wMasteryLevel += GetPassiveStatus()->Sword;	break;
	case eWeaponType_Mace:		wMasteryLevel += GetPassiveStatus()->Mace;	break;
	case eWeaponType_Axe:		wMasteryLevel += GetPassiveStatus()->Axe;	break;
	case eWeaponType_Dagger:	wMasteryLevel = GetPassiveStatus()->Dagger;	break;
	case eWeaponType_Staff:		wMasteryLevel = GetPassiveStatus()->Staff;	break;
	case eWeaponType_Bow:		wMasteryLevel = GetPassiveStatus()->Bow;	break;
	case eWeaponType_Gun:		wMasteryLevel = GetPassiveStatus()->Gun;	break;
	}

	float wAttackBuffVal	= GetBuffStatus()->PhysicAttack ;

	//float fAttackB			= 1000 + (fValA + fValB + fValC) ;
	float fAttackB			= 1000 + (((wStrengthAll -10) * 20) + (wMasteryLevel * 25) + (wAttackBuffVal * 10)) ;

	DWORD dwTotalAttack		= (DWORD)GetFloatRound( fAttackA * fAttackB / 400 );

	// ��Ʈ �ɷ� �߰�
	{
		const PlayerStat::Value& setValue = GetSetItemStats().mPhysicAttack;

		dwTotalAttack = ( max( 0, ( dwTotalAttack + setValue.mPlus ) * ( 1.0f + setValue.mPercent ) ) );
	}

	return dwTotalAttack;
}

DWORD CHero::DoGetDefenseRate() 
{
	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	DWORD wDefenseAll		= DWORD( mItemStat.mPhysicDefense.mPlus * ( 1.0f + mItemStat.mPhysicDefense.mPercent ) );
	
	DWORD wEnchantAll		= 0 ;
	DWORD wLevel			= GetLevel() ;
	float wPassiveSkill		= GetPassiveStatus()->PhysicDefense; ;

	DWORD Armor = GetWearedItemIdx( eWearedItem_Dress );
	DWORD Boots = GetWearedItemIdx( eWearedItem_Shoes );
	DWORD Glove = GetWearedItemIdx( eWearedItem_Glove );

	ITEM_INFO* pArmor = ITEMMGR->GetItemInfo( Armor );
	ITEM_INFO* pBoots = ITEMMGR->GetItemInfo( Boots );
	ITEM_INFO* pGlove = ITEMMGR->GetItemInfo( Glove );

	//---KES ��� ���� ���� ���� 071128
	m_bNoDeffenceFromArmor = FALSE;
	//-----------------------------------

	if( m_CharacterInfo.Job[0] == 2 )	//�α�
	{
		if( ( pArmor && pArmor->ArmorType == eArmorType_Metal ) ||
			( pBoots && pBoots->ArmorType == eArmorType_Metal ) ||
			( pGlove && pGlove->ArmorType == eArmorType_Metal ) )
		{
			if( GetPassiveStatus()->HeavyArmor < 0.1f )
			{
				wDefenseAll = 0;
				//---KES ��� ���� ���� ���� 071128
				m_bNoDeffenceFromArmor = TRUE;
				//-----------------------------------
			}
		}
	}
	else if( m_CharacterInfo.Job[0] == 3 )	//������
	{
		if( ( pArmor && pArmor->ArmorType == eArmorType_Metal ) ||
			( pBoots && pBoots->ArmorType == eArmorType_Metal ) ||
			( pGlove && pGlove->ArmorType == eArmorType_Metal ) )
		{
			if( GetPassiveStatus()->HeavyArmor < 0.1f )
			{
				wDefenseAll = 0;
				//---KES ��� ���� ���� ���� 071128
				m_bNoDeffenceFromArmor = TRUE;
				//-----------------------------------
			}
		}
		else if( ( pArmor && pArmor->ArmorType == eArmorType_Leather ) ||
				 ( pBoots && pBoots->ArmorType == eArmorType_Leather ) ||
				 ( pGlove && pGlove->ArmorType == eArmorType_Leather ) )
		{
			if( GetPassiveStatus()->LightArmor < 0.1f )
			{
				wDefenseAll = 0;
				//---KES ��� ���� ���� ���� 071128
				m_bNoDeffenceFromArmor = TRUE;
				//-----------------------------------
			}
		}
	}	

	float fDefense			= (float)wDefenseAll / 2 ;
	float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

	//DWORD wVitalityAll		= m_charStats.Vit + m_itemStats.Vit ;	// + m_skillStats.Vit
	//float wVitalityAll		= (float)(m_charStats.Vit + m_charStats.Vit);
	// 071011 LYW --- Hero : Modified vitiality all.

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wVitalityAll		= ( mCharStat.mVitality.mPlus + mItemStat.mVitality.mPlus ) * ( 1.0f + mItemStat.mVitality.mPercent );
	float wVitalityAll		= ( mCharStat.mVitality.mPlus + mItemStat.mVitality.mPlus + GetBuffStatus()->Vit) * ( 1.0f + mItemStat.mVitality.mPercent );
	
	// ���� ���� �� ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mVitality;

		wVitalityAll =	
			max( 0, ( wVitalityAll + value.mPlus ) * ( 1.0f + value.mPercent ) )  +
			GetPassiveStatus()->Vit+
			GetBuffStatus()->Vit;
	}

	float wMasteryLevel		= 0 ;
	wVitalityAll *= 0.2f;

	if( pArmor && pBoots && pGlove )
	{
		if( ( pArmor->ArmorType == pBoots->ArmorType ) && ( pArmor->ArmorType == pGlove->ArmorType ) )
		{
			switch( pArmor->ArmorType )
			{
			case eArmorType_Robe:		wMasteryLevel = GetPassiveStatus()->Robe;		break;
			case eArmorType_Leather:	wMasteryLevel = GetPassiveStatus()->LightArmor;	break;
			case eArmorType_Metal:		wMasteryLevel = GetPassiveStatus()->HeavyArmor;	break;
			}
		}
	}

	float wDefenseBuffVal	= GetBuffStatus()->PhysicDefense ;

	float fValA				= (((float)wVitalityAll - 20) * 10) ;
	float fValB				= (float)(wMasteryLevel * 30) ;
	float fValC				= (float)(wDefenseBuffVal * 10) ;

	float fDefenseB			= 500 + (fValA + fValB + fValC) ;

	float fResult			= (fDefenseA * fDefenseB) / 200 ;
	// 071012 LYW --- Hero : Modified.
	//float dwTotalDefense	= GetFloatRound( fResult ) ;
	float dwTotalDefense = 0.0f ;
	if( fResult <= 0 )
	{
		dwTotalDefense = 0 ;
	}
	else
	{
		dwTotalDefense	= GetFloatRound( fResult ) ;
	}

	// ��Ʈ �ɷ� �߰�
	{
		const PlayerStat::Value& setValue = GetSetItemStats().mPhysicDefense;

		dwTotalDefense = max( 0, ( dwTotalDefense + setValue.mPlus ) * ( 1.0f + setValue.mPercent ) );
	}

	return (DWORD)dwTotalDefense;
}

DWORD CHero::DoGetMagAttackRate() 
{
	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	DWORD wMagicAttack		= DWORD( mItemStat.mMagicAttack.mPlus * ( 1.0f + mItemStat.mMagicAttack.mPercent ) );
	
	DWORD wEnchantAttack	= 0 ;
	DWORD wLevel			= GetLevel() ;
	float wPassiveSkill		= GetPassiveStatus()->MagicAttack;

	float fAttackA			= (float)(wMagicAttack + wEnchantAttack + wLevel + wPassiveSkill) ;

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wIntelligenceAll	= ( mCharStat.mIntelligence.mPlus + mItemStat.mIntelligence.mPlus ) * ( 1.0f + mItemStat.mIntelligence.mPercent );
	float wIntelligenceAll	= ( mCharStat.mIntelligence.mPlus + mItemStat.mIntelligence.mPlus +GetBuffStatus()->Int) * ( 1.0f + mItemStat.mIntelligence.mPercent );

	// ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mIntelligence;

		wIntelligenceAll =	
			DWORD( max( 0, ( wIntelligenceAll + value.mPlus ) * ( 1.0f + value.mPercent ) ) )  +
			GetPassiveStatus()->Int+
			GetBuffStatus()->Int;
	}
	wIntelligenceAll *= 0.2f;

	float wAttackBuffVal	= GetBuffStatus()->MagicAttack;

	float fValA				= ((float)wIntelligenceAll -10) * 35 ;
	float fValB				= (float)(wAttackBuffVal * 10) ;

	float fAttackB			= 1000 + (fValA + fValB) ;

	float fResult			= (fAttackA * fAttackB) / 250 ;

	float dwTotalAttack		= GetFloatRound( fResult ) ;

	// ��Ʈ �ɷ� �߰�
	{
		const PlayerStat::Value& setValue = GetSetItemStats().mMagicAttack;

		dwTotalAttack = max( 0, ( dwTotalAttack + setValue.mPlus ) * ( 1.0f + setValue.mPercent ) );
	}

	return DWORD( dwTotalAttack );
}

DWORD CHero::DoGetMagDefenseRate() 
{
	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	DWORD wMagDefenseAll		= DWORD( mItemStat.mMagicDefense.mPlus * ( 1.0f + mItemStat.mMagicDefense.mPercent ) );
	
	DWORD wEnchantAll		= 0 ;
	DWORD wLevel			= GetLevel() ;
	float wPassiveSkill		= GetPassiveStatus()->MagicDefense;

	float fDefense			= (float)wMagDefenseAll / 5 ;
	float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wWisdomAll	= ( mCharStat.mWisdom.mPlus + mItemStat.mWisdom.mPlus ) * ( 1.0f + mItemStat.mWisdom.mPercent );
	float wWisdomAll	= ( mCharStat.mWisdom.mPlus + mItemStat.mWisdom.mPlus + GetBuffStatus()->Wis) * ( 1.0f + mItemStat.mWisdom.mPercent );

	// ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mWisdom;

		wWisdomAll =	
			DWORD( max( 0, ( wWisdomAll + value.mPlus ) * ( 1.0f + value.mPercent ) ) )  +
			GetPassiveStatus()->Wis+
			GetBuffStatus()->Wis;
	}
	wWisdomAll *= 0.2f;

	float wDefenseBuffVal	= GetBuffStatus()->MagicDefense;

	float fValA				= ((float)wWisdomAll - 20) * 15 ;
	float fValB				= (float)wDefenseBuffVal * 10 ;

	float fDefenseB			= 500 + (fValA + fValB) ;

	float fResult			= (fDefenseA * fDefenseB) / 150 ;

	float dwTotalDefense	= GetFloatRound( fResult ) ;

	// ��Ʈ �ɷ� �߰�
	{
		const PlayerStat::Value& setValue = GetSetItemStats().mMagicDefense;

		dwTotalDefense = max( 0, ( dwTotalDefense + setValue.mPlus ) * ( 1.0f + setValue.mPercent ) );
	}

	return DWORD( dwTotalDefense );
}

DWORD CHero::DoGetAccuracyRate() 
{
	BYTE byJobOrder			= m_CharacterInfo.Job[0] ;
	DWORD wLevel			= GetLevel() ;

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus ) * ( 1.0f + mItemStat.mDexterity.mPercent );
	//float wStrengthAll	= ( mCharStat.mStrength.mPlus + mItemStat.mStrength.mPlus ) * ( 1.0f + mItemStat.mStrength.mPercent );
	float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus + GetBuffStatus()->Dex) * ( 1.0f + mItemStat.mDexterity.mPercent );
	float wStrengthAll	= ( mCharStat.mStrength.mPlus + mItemStat.mStrength.mPlus + GetBuffStatus()->Str) * ( 1.0f + mItemStat.mStrength.mPercent );
	
	// ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mDexterity;

		wDexterityAll =	
			DWORD( max( 0, ( wDexterityAll + value.mPlus ) * ( 1.0f + value.mPercent ) ) )  +
			GetPassiveStatus()->Dex+
			GetBuffStatus()->Dex;
	}
	wDexterityAll *= 0.2f;
	wStrengthAll *= 0.2f;

	DWORD wClassOrderVal	= 0 ;
	float wPassiveSkill		= GetPassiveStatus()->Accuracy;
	float wBuffSkillVal		= GetBuffStatus()->Accuracy;

	switch( byJobOrder )
	{
	case 1 : wClassOrderVal = 20 ; break ;
	case 2 : wClassOrderVal = 25 ; break ;
	case 3 : wClassOrderVal = 15 ; break ;
	}

	float fValA				= ((float)wDexterityAll / 2) ;
	float fValC				= ((float)wStrengthAll / 4) ;
	float fValB				= (float)(fValA + wLevel + wClassOrderVal + fValC);

	float wAccuracy			= GetFloatRound(fValB) ;

	float dwTotalAccuracy	= wAccuracy + wPassiveSkill + wBuffSkillVal;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = GetSetItemStats().mAccuracy;

		dwTotalAccuracy = max( 0, ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = mItemStat.mAccuracy;

		dwTotalAccuracy = ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	return DWORD( dwTotalAccuracy );
}

DWORD CHero::DoGetEvasionRate() 
{
	BYTE byJobOrder			= m_CharacterInfo.Job[0] ;
	DWORD wLevel			= GetLevel() ;

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus ) * ( 1.0f + mItemStat.mDexterity.mPercent );
	float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus + GetBuffStatus()->Dex) * ( 1.0f + mItemStat.mDexterity.mPercent );

	// ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mDexterity;

		wDexterityAll =	
			DWORD( max( 0, ( wDexterityAll + value.mPlus ) * ( 1.0f + value.mPercent ) ) )  +
			GetPassiveStatus()->Dex+
			GetBuffStatus()->Dex;
	}
	wDexterityAll *= 0.2f;

	DWORD wClassOrderVal	= 0 ;
	float wPassiveSkill		= GetPassiveStatus()->Avoid ;
	float wBuffSkillVal		= GetBuffStatus()->Avoid ;

	switch( byJobOrder )
	{
	case 1 : wClassOrderVal = 20 ; break ;
	case 2 : wClassOrderVal = 25 ; break ;
	case 3 : wClassOrderVal = 15 ; break ;
	}

	float fValA				= ((float)wDexterityAll / 2) ;
	float fValB				= (float)(fValA + wLevel + wClassOrderVal);

	float wEvasion			= GetFloatRound(fValB) ;

	float dwTotalEvasion	= wEvasion + wPassiveSkill + wBuffSkillVal;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = GetSetItemStats().mEvade;

		dwTotalEvasion = max( 0, ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = mItemStat.mEvade;

		dwTotalEvasion = ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	return DWORD( dwTotalEvasion );
}

DWORD CHero::DoGetCriticalRate() 
{
	DWORD wLevel			= GetLevel() ;

	// 080313 LUJ, �ۼ�Ʈ �ɷ�ġ �߰�
	// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
	// 080701 LYW, ������ �þ�� ���� ��ġ�� �߰��Ѵ�.
	//float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus ) * ( 1.0f + mItemStat.mDexterity.mPercent );
	float wDexterityAll	= ( mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus + GetBuffStatus()->Dex) * ( 1.0f + mItemStat.mDexterity.mPercent );

	// ��Ʈ �ɷ� ������ ��ġ�� ����
	{
		const PlayerStat::Value& value = GetSetItemStats().mDexterity;

		wDexterityAll =	
			DWORD( max( 0, ( wDexterityAll + value.mPlus ) * ( 1.0f + value.mPercent ) ) )  +
			GetPassiveStatus()->Dex+
			GetBuffStatus()->Dex;
	}
	wDexterityAll *= 0.2f;

	float wPassiveSkill		= GetPassiveStatus()->CriticalRate ;
	float wBuffSkillVal		= GetBuffStatus()->CriticalRate ;

	float fValA				= (((float)wDexterityAll - 15) * 7) ;
	float fValB				= (float)(125 + wLevel + fValA + wPassiveSkill + wBuffSkillVal);

	float dwCritical		= GetFloatRound(fValB) ;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = GetSetItemStats().mCriticalRate;

		dwCritical = max( 0, ( dwCritical + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = mItemStat.mCriticalRate;

		dwCritical = ( dwCritical + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	return DWORD( dwCritical );
}


/* �����ؾ��� ���°��� �������� �˸���. */
void CHero::HeroStateNotify(BYTE EndState)
{
	MSG_BYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_STATE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.bData = EndState;

	NETWORK->Send(&msg, sizeof(msg));
}

BYTE CHero::GetManaPercent()
{
	return (BYTE)((GetMana()*100) / GetMaxMana());
}

DWORD CHero::DoGetMaxMana()
{
	return m_HeroTotalInfo.MaxMana;
}

void CHero::ClearGuildInfo()
{
	CPlayer::ClearGuildInfo();
}

char* CHero::GetGuildEntryDate()
{
	return m_HeroTotalInfo.MunpaCanEntryDate;
}

void CHero::SetGuildEntryDate(char* day)
{
	SafeStrCpy(m_HeroTotalInfo.MunpaCanEntryDate, day, 11);
}

char* CHero::GetFamilyEntryDate()
{
	return m_HeroTotalInfo.FamilyCanEntryDate;
}

void CHero::SetFamilyEntryDate(char* day)
{
	SafeStrCpy(m_HeroTotalInfo.FamilyCanEntryDate, day, 11);
}

void CHero::CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param )
{
	/*	if( wIdx == 0 )
	return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wIdx );
	if(!pItemInfo)
	{
	char buf[256]= { 0, };
	sprintf(buf, "S ItemInfo�� ����. PlayerIdx[%d] Idx[%d]", GetID(), wIdx);
	ASSERTMSG(0, buf);
	return;
	}

	int calc = -1;
	if(bAdd)	calc=1;

	//////////////////////////////////////////////////////
	// 06. 07 ���� ����(�ϰ�) - �̿���
	if( pItemInfo->ManaRecover )
	{
	m_ShopItemOption.Decisive += (pItemInfo->ManaRecover*calc);
	if(m_ShopItemOption.Decisive <= 0)
	m_ShopItemOption.Decisive=0;
	}
	//////////////////////////////////////////////////////

	if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
	{
	// ����ġ   str
	if( pItemInfo->Str>0 )
	{
	m_ShopItemOption.AddExp += (pItemInfo->Str*calc);
	if(m_ShopItemOption.AddExp <= 0)
	m_ShopItemOption.AddExp=0;

	}
	// ������ ���Ȯ��
	if( pItemInfo->Dex>0 )
	{
	m_ShopItemOption.AddItemDrop += (pItemInfo->Dex*calc);
	if(m_ShopItemOption.AddItemDrop <= 0)
	m_ShopItemOption.AddItemDrop=0;
	}
	// �׾����� ����ġ�� �� �г�Ƽ ���°�		
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
	// ����Ȯ��
	/*if( pItemInfo->LimitJob )
	{
	m_ShopItemOption.ItemMixSuccess = (char)pItemInfo->LimitJob;
	if(m_ShopItemOption.ItemMixSuccess < 0)
	m_ShopItemOption.ItemMixSuccess=0;
	}		
	else */
		/*		if( pItemInfo->ItemIdx == eIncantation_StatePoint )
		{
		if( bAdd )
		{
		m_ShopItemOption.StatePoint = pItemInfo->Str;
		HERO->RefreshLevelupPoint();
		}
		else
		{
		m_ShopItemOption.StatePoint = 0;
		}
		}
		else if( pItemInfo->Life )
		{
		m_ShopItemOption.SkillPoint += pItemInfo->Life;
		/*�����Ƽ ���� - GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();*/
			/*			GAMEIN->GetSkPointNotifyDlg()->SetActive( TRUE );
			}
			else if( pItemInfo->Vit )
			{
			if( bAdd )
			m_ShopItemOption.ProtectCount = (char)pItemInfo->Vit;
			else
			m_ShopItemOption.ProtectCount = (char)Param;
			}
			else if( ( pItemInfo->ItemIdx == eIncantation_Tracking ||
			pItemInfo->ItemIdx == eIncantation_Tracking7 ||
			pItemInfo->ItemIdx == eIncantation_Tracking7_NoTrade ) && bAdd )
			{
			GAMEIN->GetChaseinputDlg()->SetActive( TRUE );
			GAMEIN->GetChaseinputDlg()->SetItemIdx( pItemInfo->ItemIdx );
			}
			else if( pItemInfo->ItemIdx == eIncantation_ChangeName && bAdd )
			{
			/*			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );*/
			/*		}
			else if( pItemInfo->LimitJob )			// ������������ �ֹ���
			{
			if( bAdd )
			m_ShopItemOption.EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
			m_ShopItemOption.EquipLevelFree -= (BYTE)pItemInfo->LimitJob;
			}
			CHARCALCMGR->CalcItemStats( HERO );
			GAMEIN->GetCharacterDialog()->RefreshInfo();
			ITEMMGR->RefreshAllItem();
			}
			else if( pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend || pItemInfo->ItemIdx == eIncantation_MemoryMove2 )
			{
			GAMEIN->GetMoveDialog()->SetExtend( bAdd );
			}
			}
			else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
			{
			if( pItemInfo->Str>0 )
			{
			m_ShopItemOption.Str += (pItemInfo->Str*calc);
			if(m_ShopItemOption.Str <= 0)
			m_ShopItemOption.Str=0;

			CHARCALCMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetStrength();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
			}
			if( pItemInfo->Dex>0 )
			{
			m_ShopItemOption.Dex += (pItemInfo->Dex*calc);
			if(m_ShopItemOption.Dex <= 0)
			m_ShopItemOption.Dex=0;

			CHARCALCMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetDexterity();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
			}
			if( pItemInfo->Vit>0 )
			{		
			m_ShopItemOption.Vit += (pItemInfo->Vit*calc);
			if(m_ShopItemOption.Vit <= 0)
			m_ShopItemOption.Vit=0;

			CHARCALCMGR->CalcItemStats(this);

			GAMEIN->GetCharacterDialog()->SetVitality();
			GAMEIN->GetCharacterDialog()->SetDefenseRate();
			}
			if( pItemInfo->Wis>0 )
			{
			m_ShopItemOption.Wis += (pItemInfo->Wis*calc);
			if(m_ShopItemOption.Wis <= 0)
			m_ShopItemOption.Wis=0;

			CHARCALCMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetWisdom();
			}
			// ����������   Life
			if( pItemInfo->Life>0 )
			{
			m_ShopItemOption.NeagongDamage += (char)(pItemInfo->Life*calc);
			if(m_ShopItemOption.NeagongDamage <= 0)
			m_ShopItemOption.NeagongDamage=0;
			}
			/*
			// �ܰ������   Shield
			if( pItemInfo->Shield>0 )
			{
			m_ShopItemOption.WoigongDamage += (char)(pItemInfo->Shield*calc);
			if(m_ShopItemOption.WoigongDamage <= 0)
			m_ShopItemOption.WoigongDamage=0;
			}
			*/
			// ����Ư��ġ 1��  Plus_SkillIdx
			/*		if( pItemInfo->Mana>0 )
			{
			m_ShopItemOption.AddLevel += (pItemInfo->Mana*calc);
			if( m_ShopItemOption.AddLevel < 0 )
			m_ShopItemOption.AddLevel = 0;
			}
			// �⺻������  Plus_Value
			if( pItemInfo->LimitJob>0 )
			{
			m_ShopItemOption.ComboDamage += (pItemInfo->LimitJob*calc);
			if(m_ShopItemOption.ComboDamage <= 0)
			m_ShopItemOption.ComboDamage=0;
			}
			// ũ��Ƽ�� ������   CriticalPercent
			if( pItemInfo->LimitGender>0 )
			{
			m_ShopItemOption.Critical += (pItemInfo->LimitGender*calc);
			if(m_ShopItemOption.Critical <= 0)
			m_ShopItemOption.Critical=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
			}
			// ũ��Ƽ�ý� ����Ȯ��    RangeAttackMin
			if( pItemInfo->LimitLevel>0 )
			{
			m_ShopItemOption.StunByCri += (pItemInfo->LimitLevel*calc);
			if(m_ShopItemOption.StunByCri <= 0)
			m_ShopItemOption.StunByCri=0;
			}
			// ��������
			if( pItemInfo->LimitStr>0 )
			{
			m_ShopItemOption.RegistPhys += (pItemInfo->LimitStr*calc);
			if(m_ShopItemOption.RegistPhys < 0)
			m_ShopItemOption.RegistPhys = 0;

			GAMEIN->GetCharacterDialog()->SetDefenseRate();
			}
			// �Ӽ�����
			if( pItemInfo->LimitDex>0 )
			{
			m_ShopItemOption.RegistAttr += (pItemInfo->LimitDex*calc);
			if(m_ShopItemOption.RegistAttr < 0)
			m_ShopItemOption.RegistAttr = 0;

			GAMEIN->GetCharacterDialog()->UpdateData();
			}
			// ���¼Ҹ𰨼�
			if( pItemInfo->LimitVit>0 )
			{
			m_ShopItemOption.ManaSpend += (pItemInfo->LimitVit*calc);
			if(m_ShopItemOption.ManaSpend < 0 )
			m_ShopItemOption.ManaSpend = 0;
			}
			// �ִ����� ����ġ
			if( pItemInfo->Plus_SkillIdx )
			{
			m_ShopItemOption.Life += (pItemInfo->Plus_SkillIdx*calc);
			if( m_ShopItemOption.Life <= 0 )
			m_ShopItemOption.Life = 0;	
			}
			// �ִ볻�� ����ġ
			if( pItemInfo->AllPlus_Kind )
			{
			m_ShopItemOption.Mana += (pItemInfo->AllPlus_Kind*calc);
			if( m_ShopItemOption.Mana <= 0 )
			m_ShopItemOption.Mana = 0;
			}
			// ���������
			if( pItemInfo->RangeAttackMin )
			{
			m_ShopItemOption.bKyungGong += (pItemInfo->RangeAttackMin*calc);
			}
			// ����ӵ� ���
			if( pItemInfo->RangeAttackMax )
			{
			m_ShopItemOption.KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
			}
			// ��ȭ ���� ��ġ
			if( pItemInfo->CriticalPercent )
			{
			m_ShopItemOption.ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			CHARCALCMGR->CalcItemStats(this);
			GAMEIN->GetCharacterDialog()->RefreshInfo();
			}
			// �����۵��Ȯ�� 
			if( pItemInfo->PhyDef )
			{
			m_ShopItemOption.AddItemDrop += (pItemInfo->PhyDef*calc);
			}
			// ũ��Ƽ�� ������   CriticalPercent
			if( pItemInfo->ManaRecover>0 )
			{
			m_ShopItemOption.Decisive += (pItemInfo->ManaRecover*calc);
			if(m_ShopItemOption.Decisive <= 0)
			m_ShopItemOption.Decisive=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
			}


			/*		// ����ȸ���� 1.5��
			if( pItemInfo->ManaRecoverRate>0 )
			{
			m_ShopItemOption.RecoverRate += (pItemInfo->ManaRecoverRate*calc);
			if( m_ShopItemOption.RecoverRate<=0 )
			m_ShopItemOption.RecoverRate = 0;

			}*/
			/*	}
			else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
			{
			// �ִ����� ����ġ
			if( pItemInfo->Life > 0 )
			{
			m_ShopItemOption.Life += (WORD)(pItemInfo->Life*calc);
			if( m_ShopItemOption.Life <= 0 )
			m_ShopItemOption.Life = 0;		
			}
			// �ִ볻�� ����ġ
			if( pItemInfo->Mana > 0 )
			{
			m_ShopItemOption.Mana += (pItemInfo->Mana*calc);
			if( m_ShopItemOption.Mana <= 0 )
			m_ShopItemOption.Mana = 0;
			}

			// ���� ����
			// CHARCALCMGR->CalcCharStats( pPlayer );
			}
			else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
			{
			//		APPEARANCEMGR->SetAvatarItem( HERO, pItemInfo->ItemIdx, bAdd );
			}
			else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
			{
			/*
			if( pItemInfo->ItemIdx == eSundries_Shout && bAdd )
			GAMEIN->GetShoutDlg()->SetActive( TRUE );
			*/


// ��������
//		if( pItemInfo->Vit )
//			m_ShopItemOption.bStreetStall += (pItemInfo->Vit*calc);
//	}	
		}


void CHero::ActiveOptionInfoToInterface()
{
	if( m_ShopItemOption.Str )
		GAMEIN->GetCharacterDialog()->SetStrength();
	if( m_ShopItemOption.Dex )
		GAMEIN->GetCharacterDialog()->SetDexterity();
	if( m_ShopItemOption.Vit )
		GAMEIN->GetCharacterDialog()->SetVitality();
	if( m_ShopItemOption.Wis )
		GAMEIN->GetCharacterDialog()->SetWisdom();
}


void CHero::CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos )
{
	AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarItem)	return;

	for(int i=0; i<eAvatar_Weared_Hair; i++)
	{
		// �ƹ�Ÿ �������� ������ �ϴ°���̰ų�
		// ���� �ڸ��� �ƹ�Ÿ �������� �ִ� ��쿣.
		if( (!pAvatarItem->Item[i] && m_ShopItemOption.Avatar[i]) ||
			(m_ShopItemOption.Avatar[i] && i==pAvatarItem->Position) )
		{
			for(int n=0; n<SLOT_SHOPINVEN_NUM/2; n++)
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( n+TP_SHOPINVEN_START );
				
				if( pItem && 
					pItem->GetItemIdx()==m_ShopItemOption.Avatar[i] && pItem->GetUseParam() )
				{
					pItem->SetUseParam( 0 );
					break;
				}				
			}
		}
	}
}


void CHero::RefreshLevelupPoint()
{
	if( m_ShopItemOption.StatePoint==0 )		return;

	// Hard Coding
	WORD point = m_HeroTotalInfo.LevelUpPoint + (30 - m_ShopItemOption.StatePoint);	
	GAMEIN->GetCharacterDialog()->SetMinusPoint( TRUE, point );
}

void CHero::SetGuildName(char* Name)	
{ 
	SafeStrCpy(m_CharacterInfo.GuildName, Name, MAX_GUILD_NAME+1);	
}

char* CHero::GetGuildName()			
{ 
	return m_CharacterInfo.GuildName;
}

void CHero::SetFamilyName(char* Name)	
{ 
	SafeStrCpy(m_CharacterInfo.FamilyName, Name, MAX_FAMILY_NAME+1);	
}

char* CHero::GetFamilyName()			
{ 
	return m_CharacterInfo.FamilyName;
}

BOOL CHero::CanSocietyAction( DWORD curTime )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Society &&
		GetState() != eObjectState_Immortal )
		return FALSE;

	DWORD dwElapsedTime = gCurTime - m_dwLastSocietyAction;
	if( dwElapsedTime > 2000 ) //2��
	{
		m_dwLastSocietyAction = gCurTime;
		return TRUE;
	}

	return FALSE;
}
void CHero::ProcessSpecialSkill()
{
}

DWORD CHero::GetAccuracy()
{
	return mAccuracy;
}

DWORD CHero::GetAvoid()
{
	return mAvoid;
}

DWORD CHero::GetBlock()
{
	return mBlock;
}

DWORD CHero::GetPhysicAttackMax()
{
	return mPhysicAttackMax;
}

DWORD CHero::GetPhysicAttackMin()
{
	return mPhysicAttackMin;
}

DWORD CHero::GetMagicAttack()
{
	return mMagicAttack;
}

DWORD CHero::GetPhysicDefense()
{
	return mPhysicDefense;
}

DWORD CHero::GetMagicDefense()
{
	return mMagicDefense;
}

DWORD CHero::GetCriticalRate()
{
	return mCriticalRate;
}

DWORD CHero::GetCriticalDamage()
{
	return mCriticalDamage;
}

DWORD CHero::GetLifeRecover()
{
	return mLifeRecover;
}

DWORD CHero::GetManaRecover()
{
	return mManaRecover;
}

DWORD	CHero::GetMaxLifeWithItem()
{
	return mMaxLife;
}

DWORD	CHero::GetMaxManaWithItem()
{
	return mMaxMana;
}


void CHero::AddMatch()
{
	++m_matchedQuantity;

	SetHeart( TRUE );
}


void CHero::RemoveMatch()
{
	if( 1 > --m_matchedQuantity )
	{
		SetHeart( FALSE );

		m_matchedQuantity = 0;
	}
}


void CHero::CheckAllHeart()
{
	m_matchedQuantity = 0;
	
	// ��ü ����
	{
		CYHHashTable<CObject>& objectTable = OBJECTMGR->GetObjectTable();
		YHTPOSITION position = objectTable.GetPositionHead();

		for(	CObject* object;
				object = objectTable.GetDataPos( &position );
				)
		{
			if( eObjectKind_Player != object->GetObjectKind() )
			{
				continue;
			}

			CPlayer* player = ( CPlayer* )object;

			if( player->CheckHeart( this ) )
			{
				player->SetHeart( TRUE );

				AddMatch();
			}
			else
			{
				player->SetHeart( FALSE );

				RemoveMatch();
			}
		}
	}
}

DWORD CHero::GetVitality() const 
{ 
	// 080408 LUJ, �Ǽ� ǥ�� ���� ���� ���� ���ڿ� ó�� �� ��ȯ
	char buffer[ MAX_PATH ] = { 0 };
	sprintf( buffer, "%0.1f", mCharStat.mVitality.mPlus + mItemStat.mVitality.mPlus );

	float Result = float( atof( buffer ) );

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = mSetItemStat.mVitality;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Vit + mBuffStatus.Vit );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Vit + mRateBuffStatus.Vit + ( value.mPercent * 100.f ) ) / 100.f + mItemStat.mVitality.mPercent;
	Result += ( mCharStat.mVitality.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CHero::GetWisdom() const
{ 
	// 080408 LUJ, �Ǽ� ǥ�� ���� ���� ���� ���ڿ� ó�� �� ��ȯ
	char buffer[ MAX_PATH ] = { 0 };
	sprintf( buffer, "%0.1f", mCharStat.mWisdom.mPlus + mItemStat.mWisdom.mPlus );

	float Result = float( atof( buffer ) );

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = mSetItemStat.mWisdom;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Wis + mBuffStatus.Wis );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Wis + mRateBuffStatus.Wis + ( value.mPercent * 100.f ) ) / 100.f + mItemStat.mWisdom.mPercent;
	Result += ( mCharStat.mWisdom.mPlus * rate );
	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CHero::GetStrength() const
{
	// 080408 LUJ, �Ǽ� ǥ�� ���� ���� ���� ���ڿ� ó�� �� ��ȯ
	char buffer[ MAX_PATH ] = { 0 };
	sprintf( buffer, "%0.1f", mCharStat.mStrength.mPlus + mItemStat.mStrength.mPlus );

	float Result = float( atof( buffer ) );

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = mSetItemStat.mStrength;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Str + mBuffStatus.Str );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Str + mRateBuffStatus.Str + ( value.mPercent * 100.f ) ) / 100.f + mItemStat.mStrength.mPercent;
	Result += ( mCharStat.mStrength.mPlus * rate );
	
	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CHero::GetDexterity() const 
{ 
	// 080408 LUJ, �Ǽ� ǥ�� ���� ���� ���� ���ڿ� ó�� �� ��ȯ
	char buffer[ MAX_PATH ] = { 0 };
	sprintf( buffer, "%0.1f", mCharStat.mDexterity.mPlus + mItemStat.mDexterity.mPlus );

	float Result = float( atof( buffer ) );

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = mSetItemStat.mDexterity;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Dex + mBuffStatus.Dex );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Dex + mRateBuffStatus.Dex + ( value.mPercent * 100.f ) ) / 100.f + mItemStat.mDexterity.mPercent;
	Result += ( mCharStat.mDexterity.mPlus * rate );
	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CHero::GetIntelligence() const
{
	// 080408 LUJ, �Ǽ� ǥ�� ���� ���� ���� ���ڿ� ó�� �� ��ȯ
	char buffer[ MAX_PATH ] = { 0 };
	sprintf( buffer, "%0.1f", mCharStat.mIntelligence.mPlus + mItemStat.mIntelligence.mPlus );

	float Result = float( atof( buffer ) );

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = mSetItemStat.mIntelligence;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Int + mBuffStatus.Int );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Int + mRateBuffStatus.Int + ( value.mPercent * 100.f ) ) / 100.f + mItemStat.mIntelligence.mPercent;
	Result += ( mCharStat.mIntelligence.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

float CHero::DoGetMoveSpeed()
{
	float Speed = CPlayer::DoGetMoveSpeed();
	
	if( GetAbnormalStatus()->IsMoveStop )
	{
		Speed = 0;
	}

	return Speed;
}

void CHero::StartSkillProgressBar( DWORD time )
{
	GetObjectBalloon()->GetProgressBar()->CLI_Start(CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_TYPE, CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_WIDTH, time);
}
// 071210 LYW --- Hero : HERO�� Ȯ�� �κ��丮 ���� ���� �� ��ȯ�ϴ� �Լ� �߰�.
void CHero::Set_HeroExtendedInvenCount(BYTE count)
{
	// ����� Ȯ�� �κ� ���� �� ���� ���ѵǾ� �ְ�, ���� ������ ���� �� ���� ���� ���� �ʰ� ����Ѵ�.
	if( count > 2 ) return ;	

	m_byExtendedInvenCount = count ;
}

void CHero::PassiveSkillCheckForWeareItem()
{
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );

	SKILL_BASE* pSkillBase = NULL;

	SKILLTREEMGR->SetPositionHead();

	BOOL WeaponCheck;
	BOOL ArmorCheck;

	while( pSkillBase = SKILLTREEMGR->GetSkillBase() )
	{
		DWORD skillidx = pSkillBase->wSkillIdx - 1 + pSkillBase->Level;

		cActiveSkillInfo* pSkill = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillidx );

		if( pSkill->GetKind() != SKILLKIND_PASSIVE )
			continue;

		for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; cnt++ )			
		{
			DWORD index = pSkill->GetSkillInfo()->Buff[ cnt ];		

			if( index == 0 )
			{
				continue;
			}

			WeaponCheck = FALSE;

			cBuffSkillInfo* pSkillInfo = ( cBuffSkillInfo* )SKILLMGR->GetSkillInfo( index );

			switch( pSkillInfo->GetSkillInfo()->EquipItem[0] )
			{
			case eBuffSkillWeapon_None:
				{
					WeaponCheck = TRUE;
				}
				break;
			case eBuffSkillWeapon_Sword:
				{
					if( GetWeaponEquipType() == eWeaponType_Sword )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Shield:
				{
					if( GetWearedItemIdx( eWearedItem_Shield ) )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Mace:
				{
					if( GetWeaponEquipType() == eWeaponType_Mace )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Axe:
				{
					if( GetWeaponEquipType() == eWeaponType_Axe )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Staff:
				{
					if( GetWeaponEquipType() == eWeaponType_Staff )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Bow:
				{
					if( GetWeaponEquipType() == eWeaponType_Bow )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Gun:
				{
					if( GetWeaponEquipType() == eWeaponType_Gun )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Dagger:
				{
					if( GetWeaponEquipType() == eWeaponType_Dagger )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Spear:
				{
					WeaponCheck = TRUE;
				}
				break;
			case eBuffSkillWeapon_TwoHanded:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_TwoHand )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_OneHanded:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_OneHand )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_TwoBlade:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_TwoBlade )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			}

			WORD armortype = 0;
			ArmorCheck = FALSE;

			DWORD Armor = GetWearedItemIdx( eWearedItem_Dress );
			DWORD Boots = GetWearedItemIdx( eWearedItem_Shoes );
			DWORD Glove = GetWearedItemIdx( eWearedItem_Glove );

			ITEM_INFO* pArmor = ITEMMGR->GetItemInfo( Armor );
			ITEM_INFO* pBoots = ITEMMGR->GetItemInfo( Boots );
			ITEM_INFO* pGlove = ITEMMGR->GetItemInfo( Glove );
			
			if( pArmor && pBoots && pGlove )
			{
				if( ( pArmor->ArmorType == pBoots->ArmorType ) && ( pArmor->ArmorType == pGlove->ArmorType ) )
				{
					armortype = pArmor->ArmorType;
				}
			}

			switch( pSkillInfo->GetSkillInfo()->EquipItem[1] )
			{
			case eBuffSkillArmor_None:
				{
					ArmorCheck = TRUE;
				}
				break;
			default:
				{
					if( pSkillInfo->GetSkillInfo()->EquipItem[1] == armortype )
					{
						ArmorCheck = TRUE;
					}
				}
			}

			if( WeaponCheck && ArmorCheck )
			{
				pSkillInfo->AddPassiveStatus();
			}
		}	
	}

	GAMEIN->GetCharacterDialog()->RefreshInfo();
}
/*
void CHero::SetStallSearch(char* szSearch)
{
	strcpy(m_szStallSearch, szSearch);

	CYHHashTable<CObject>& objectTable = OBJECTMGR->GetObjectTable();
	YHTPOSITION position = objectTable.GetPositionHead();

	for(	CObject* object;
				object = objectTable.GetDataPos( &position );
				)
	{
		if( eObjectKind_Player != object->GetObjectKind() )
		{
			continue;
		}

		CPlayer* player = ( CPlayer* )object;
		CObjectBalloon* pBallon = player->GetObjectBalloon();

		if(pBallon)
		{
			CStreetStallTitleTip* pSellStallTitle = pBallon->GetSSTitleTip();
			CStreetStallTitleTip* pBuyStallTitle = pBallon->GetSBSTitleTip();
			if(pSellStallTitle && pSellStallTitle->IsShow())
			{
				player->SearchStreetStallTitle(m_szStallSearch);
			}
			else if(pBuyStallTitle && pBuyStallTitle->IsShow())
			{
				player->SearchStreetBuyStallTitle(m_szStallSearch);
			}
		}
	}
}
*/
