#include "stdafx.h"
#include "CharacterDialog.h"
#include "ObjectManager.h"
#include "Interface/CWindowManager.h"
#include "Interface/cStatic.h"
#include "Interface/cButton.h"
#include "WindowIDEnum.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "GuildMarkDialog.h"
#include "CheatMsgParser.h"
#include "CharacterCalcManager.h"

#include "GameIn.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "ChatManager.h"

// 070111 LYW --- Include header file to need.
#include "cResourceManager.h"

// 071022 LYW --- CharacterDialog : Include header file of tutorial manager.
#include "TutorialManager.h"


CCharacterDialog::CCharacterDialog()
{
	// 070113 LYW --- Add variable for count.
	int count = 0 ;

	m_type = WT_CHARINFODIALOG;
	m_nocoriPoint = 0;
	m_MinusPoint = -1;

	// 070111 LYW --- Add button control for call class tree dialog.
	m_pClassTreeBtn = NULL ;

	m_bShowTreeDlg		= FALSE ;
	m_bCheckedTreeInfo  = FALSE ;

	// 070112 LYW --- Add variable to store up character image number.
	m_byCharImgNum = 0x01 ;

	// 070113 LYW --- Add static controls for background image.
	// 070118 LYW --- Delete this part.
	/*
	for( count = 0 ; count < MAX_BGSTATIC ; ++count )
	{
		m_pBGStatic[ count ] = NULL ;
	}
	*/

	// 070118 LYW --- Delete this part.
	/*
	for( count = 0 ; count < MAX_CIRCLE_IMAGE ; ++count )
	{
		m_pCircleImage[ count ] = NULL ;
	}
	*/

	for( count = 0 ; count < MAX_CHAR_IMAGE ; ++count )
	{
		m_pCharImage[ count ] = NULL ;
	}

	// 070118 LYW --- Delete this part.
	/*
	for( count = 0 ; count < MAX_RESISTANCE_IMAGE ; ++count )
	{
		m_pResistanceImage[ count ] = NULL ;
	}
	*/

	// 070118 LYW --- Delete this part.
	/*
	for( count = 0 ; count < MAX_LOCKED_TEXT ; ++count )
	{
		m_pLockedText[ count ] = NULL ;
	}
	*/

	for( count = 0 ; count < MAX_CHANGE_TEXT ; ++count )
	{
		m_pChangeText[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_STATUS_BTN ; ++count )
	{
		m_pStatusBtn[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_SUB_ITEMS ; ++count )
	{
		m_pSubItem[ count ] = NULL ;
	}

	for( count = 0 ; count < MAX_CLASS_STATIC ; ++count )
	{
		m_pClassStatic[ count ] = NULL ;
	}

	m_pCloseBtn			= NULL ;
	m_pClassTreeBtn		= NULL ;

	m_pTreeBgImg		= NULL ;
	m_pTreeMark			= NULL ;
	m_pTreeTitle		= NULL ;

	// 070416 LYW --- CharacterDialog : Add static to print racial.
	m_pRacial = NULL ;

	// 070503 LYW --- CharacterDialog : Add static to setting family name.
	m_pFamilyName = NULL ;
}

CCharacterDialog::~CCharacterDialog()
{

}

void CCharacterDialog::Linking()
{
	// 070111 LYW --- Add variable for count.
	int count = 0 ;

	for( count = 0 ; count < MAX_CHAR_IMAGE ; ++count )
	{
		m_pCharImage[ count ] = ( cStatic* )GetWindowForID( CI_CHAR_IMAGE0 + count ) ;
	}

	for( count = 0 ; count < MAX_CHANGE_TEXT ; ++count )
	{
		m_pChangeText[ count ] = ( cStatic* )GetWindowForID( CI_CHANGED_TEXT0 + count ) ;
		m_pChangeText[ count ]->SetActive( TRUE ) ;
	}

	for( count = 0 ; count < MAX_STATUS_BTN ; ++count )
	{
		m_pStatusBtn[ count ] = ( cButton* )GetWindowForID( CI_STATUS_BTN_0 + count ) ;
		// 070122 LYW --- Setting to deactive.
		m_pStatusBtn[ count ]->SetActive( FALSE ) ;
	}

	for( count = 0 ; count < MAX_SUB_ITEMS ; ++count )
	{
		m_pSubItem[ count ] = ( cStatic* )GetWindowForID( CI_SUB_ITEMS1 + count ) ;
	}

	m_pCloseBtn			= ( cButton* )GetWindowForID( CI_CLOSEBTN ) ;
	m_pClassTreeBtn		= ( cButton* )GetWindowForID( CI_CLASSTREEBTN ) ;

	m_pTreeBgImg		= ( cStatic* )GetWindowForID( CI_TREEBGIMG ) ;
	m_pTreeMark			= ( cStatic* )GetWindowForID( CI_TREEMARK ) ;
	m_pTreeTitle		= ( cStatic* )GetWindowForID( CI_TREETITLE ) ;

	for( count = 0 ; count < MAX_CLASS_STATIC ; ++count )
	{
		m_pClassStatic[ count ] = ( cStatic* )GetWindowForID( CI_CLASS1_STATIC + count ) ;
	}

	// 070118 LYW --- Delete this part.
	/*
	m_pLockedText[ 0]->SetStaticText( "캐릭터 정보" ) ;
	m_pLockedText[ 1]->SetStaticText( "LV." ) ;
	m_pLockedText[ 2]->SetStaticText( "PK" ) ;
	m_pLockedText[ 3]->SetStaticText( "이름" ) ;
	m_pLockedText[ 4]->SetStaticText( "길드" ) ;
	m_pLockedText[ 5]->SetStaticText( "호칭" ) ;
	m_pLockedText[ 6]->SetStaticText( "클레스" ) ;
	m_pLockedText[ 7]->SetStaticText( "STATUS" ) ;
	m_pLockedText[ 8]->SetStaticText( "STR" ) ;
	m_pLockedText[ 9]->SetStaticText( "DEX" ) ;
	m_pLockedText[10]->SetStaticText( "VIT" ) ;
	m_pLockedText[11]->SetStaticText( "INT" ) ;
	m_pLockedText[12]->SetStaticText( "WIS" ) ;
	m_pLockedText[13]->SetStaticText( "POINT" ) ;
	m_pLockedText[14]->SetStaticText( "세부 능력치" ) ;
	m_pLockedText[15]->SetStaticText( "물리 공격력" ) ;
	m_pLockedText[16]->SetStaticText( "사정거리(활)" ) ;
	m_pLockedText[17]->SetStaticText( "크 리 티 컬" ) ;
	m_pLockedText[18]->SetStaticText( "회       피" ) ;
	m_pLockedText[19]->SetStaticText( "명  중   율" ) ;
	m_pLockedText[20]->SetStaticText( "물리 방어력" ) ;
	m_pLockedText[21]->SetStaticText( "마법 공격력" ) ;
	m_pLockedText[22]->SetStaticText( "마법 명중율" ) ;
	m_pLockedText[23]->SetStaticText( "마 법 확 율" ) ;
	m_pLockedText[24]->SetStaticText( "마법 방어력" ) ;
	m_pLockedText[25]->SetStaticText( "SP" ) ;
	m_pLockedText[26]->SetStaticText( "속성 저항" ) ;
	m_pLockedText[27]->SetStaticText( "화" ) ;
	m_pLockedText[28]->SetStaticText( "수" ) ;
	m_pLockedText[29]->SetStaticText( "토" ) ;
	m_pLockedText[30]->SetStaticText( "풍" ) ;
	m_pLockedText[31]->SetStaticText( "암흑" ) ;
	m_pLockedText[32]->SetStaticText( "신성" ) ;	
	*/

//	m_pGuildMarkDlg = (CGuildMarkDialog*)GetWindowForID(CI_MUNPAMARK);
	// 070113 LYW --- Delete this part.
	/*
	m_ppStatic.munpa = (cStatic *)GetWindowForID(CI_CHARMUNPA);
	m_ppStatic.jikwe = (cStatic *)GetWindowForID(CI_CHARJIKWE);

	m_ppStatic.fame = (cStatic *)GetWindowForID(CI_CHARFAME);	// 명성
	m_ppStatic.spname = (cStatic *)GetWindowForID(CI_SPCHARNAME);	// 별호
	m_ppStatic.badfame = (cStatic *)GetWindowForID(CI_CHARBADFAME);
	m_ppStatic.name = (cStatic *)GetWindowForID(CI_CHARNAME);
//	m_ppStatic.stage = (cStatic *)GetWindowForID(CI_CHARSTAGE);		// 경지
//	m_ppStatic.job = (cStatic *)GetWindowForID(CI_CHARJOB);		// 직업
//	m_ppStatic.ideology = (cStatic *)GetWindowForID(CI_CHARIDEOLOGY);	// 가치관

	m_ppStatic.Str = (cStatic *)GetWindowForID(CI_CHARSTR);
	m_ppStatic.Wis = (cStatic *)GetWindowForID(CI_CHARWIS);
	m_ppStatic.Dex = (cStatic *)GetWindowForID(CI_CHARDEX);
	m_ppStatic.Vit = (cStatic *)GetWindowForID(CI_CHARVIT);
	*/

	// 070111 LYW --- Add static for intelligence part.
	// 070113 LYW --- Delete this part.
	/*
	m_ppStatic.Int = (cStatic *)GetWindowForID(CI_CHARVIT) ;

	m_ppStatic.level = (cStatic *)GetWindowForID(CI_CHARLEVEL);
	m_ppStatic.expPercent = (cStatic *)GetWindowForID(CI_CHAREXPPERCENT);
	m_ppStatic.point = (cStatic *)GetWindowForID(CI_CHARPOINT);		// 잔여포인트
	
	m_ppStatic.meleeattack = (cStatic *)GetWindowForID(CI_CHARATTACK);	// 공격력
	m_ppStatic.rangeattack = (cStatic *)GetWindowForID(CI_LONGATTACK);	// 공격력
	m_ppStatic.defense = (cStatic *)GetWindowForID(CI_CHARDEFENSE);
//	m_ppStatic.kyeongkong = (cStatic *)GetWindowForID(CI_CHARKYEONGKONG);
//	m_ppStatic.move = (cStatic *)GetWindowForID(CI_CHARMOVE);
	m_ppStatic.life = (cStatic *)GetWindowForID(CI_CHARLIFE);
	m_ppStatic.Mana = (cStatic *)GetWindowForID(CI_CHARMANA);
	
	m_ppStatic.critical = (cStatic *)GetWindowForID(CI_CRITICAL);	
	m_ppStatic.attackdistance = (cStatic *)GetWindowForID(CI_DISTANCE);	
	*/

//	m_ppStatic.dok = (cStatic *)GetWindowForID(CI_CHARDOK);
	
	// 070111 LYW --- Modified linking button controls.
	/*
	m_pPointBtn[STR_POINT] = (cButton *)GetWindowForID(CI_STR_POINT);
	m_pPointBtn[WIS_POINT] = (cButton *)GetWindowForID(CI_WIS_POINT);
	m_pPointBtn[DEX_POINT] = (cButton *)GetWindowForID(CI_DEX_POINT);
	m_pPointBtn[VIT_POINT] = (cButton *)GetWindowForID(CI_VIT_POINT);
	*/
	// 070113 LYW --- Delete this part.
	/*
	for( count = 0 ; count < MAX_BTN_POINT ; ++count )
	{
		m_pPointBtn[ count ] = ( cButton* )GetWindowForID( CI_STR_POINT + count ) ;
	}
	*/

	
	// RaMa
	// 070111 LYW --- Delete these control.
	/*
	m_pPointMinusBtn[STR_POINT] = (cButton *)GetWindowForID(CI_STR_POINT2);
	m_pPointMinusBtn[WIS_POINT] = (cButton *)GetWindowForID(CI_WIS_POINT2);
	m_pPointMinusBtn[DEX_POINT] = (cButton *)GetWindowForID(CI_DEX_POINT2);
	m_pPointMinusBtn[VIT_POINT] = (cButton *)GetWindowForID(CI_VIT_POINT2);
	*/

	// 070113 LYW --- Delete this part.
	/*
	for(int n=0;n<ATTR_MAX;++n)
	{
		m_AttrDefComponent.pStatic.SetElement_Val(ATTR_FIRE+n,(cStatic *)GetWindowForID(CI_CHARHWA+n));
		m_AttrDefComponent.pGuage.SetElement_Val(ATTR_FIRE+n,(cGuagen *)GetWindowForID(CI_DEFENCE_HWA+n));
	}

	// 070111 LYW --- Add button control for call class tree dialog.
	m_pClassTreeBtn = ( cButton* )GetWindowForID( CI_CLASSTREEBTN ) ;

	// 070111 LYW --- Add new controls for class tree.
	m_pTreeBgImg = (cStatic*)GetWindowForID( CI_TREEBGIMG ) ;
	m_pTreeMark  = (cStatic*)GetWindowForID( CI_TREEMARK ) ;
	m_pTreeTitle = (cStatic*)GetWindowForID( CI_TREETITLE ) ;

	for( count = 0 ; count < MAX_SUBITEM ; ++count )
	{
		m_pSubItem[ count ] = ( cStatic* )GetWindowForID( CI_SUB_ITEMS1 + count ) ;
	}

	for( count = 0 ; count < MAX_CLASS_STATIC ; ++count )
	{
		m_pClassStatic[ count ] = ( cStatic* )GetWindowForID( CI_CLASS1_STATIC + count ) ;
	}

	for( count = 0 ; count < MAX_CHARIMAGE ; ++count )
	{
		m_pCharImage[ count ] = ( cStatic* )GetWindowForID( CI_CHAR_IMAGE1 + count ) ;
	}
	*/

	// 070416 LYW --- CharacterDialog : linking static to print racial.
	m_pRacial = (cStatic*)GetWindowForID(CI_RACIAL) ;

	// 070503 LYW --- CharacterDialog : Add family name.
	m_pFamilyName = (cStatic*)GetWindowForID(CI_FAMILYNAME) ;
}
void CCharacterDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	//m_type = WT_CHARINFODIALOG;
}

void CCharacterDialog::UpdateData()
{
	char tempStr[128] = {0, } ;

	BASEOBJECT_INFO ObjInfo;
	CHARACTER_TOTALINFO ChaInfo;
	HERO_TOTALINFO HeroInfo;
	OBJECTMGR->GetHero()->GetBaseObjectInfo(&ObjInfo);
	OBJECTMGR->GetHero()->GetCharacterTotalInfo(&ChaInfo);
	OBJECTMGR->GetHero()->GetHeroTotalInfo(&HeroInfo);

	
	m_pChangeText[1]->SetStaticValue(ChaInfo.BadFame);
	m_pChangeText[2]->SetStaticText(ObjInfo.ObjectName);

	// 070416 LYW --- CharacterDialog : Setting Class name.
	WORD idx = 1;
	if( ChaInfo.JobGrade > 1 )
	{
		idx = ChaInfo.Job[ChaInfo.JobGrade - 1];
	}
	WORD job = ( ChaInfo.Job[0] * 1000 ) + ( ( ChaInfo.Race + 1 ) * 100 ) + ( ChaInfo.JobGrade * 10 ) + idx;

	m_pChangeText[5]->SetStaticText( RESRCMGR->GetMsg(RESRCMGR->GetClassNameNum(job)) ) ; 

	// 070416 LYW --- CharacterDialog : Setting racial.
	if( !ChaInfo.Race )
	{
		m_pRacial->SetStaticText(RESRCMGR->GetMsg(247)) ;
	}
	else
	{
		m_pRacial->SetStaticText(RESRCMGR->GetMsg(248)) ;
	}

	for(int n=0;n<ATTR_MAX;++n)
	{
		WORD value = (WORD)(HERO->GetAttDefense(ATTR_FIRE+n)*100.5);
		value += HERO->GetShopItemStats()->RegistAttr;
		
		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가
		// 속성방어력
		float val = 1 + HERO->GetSkillStatsOption()->AttDef;

		if( val < 0 )
			val = 0.0f;

		value = (WORD)(value * val + 0.5);
		//////////////////////////////////////////////////////////////////////////
		
		char tempStr[128] = {0, } ;
		sprintf( tempStr, "%d%%", value ) ;
		m_pChangeText[23 + n]->SetStaticText( tempStr ) ;
	}
	m_pChangeText[28]->SetStaticText( "0%" ) ;

	// 080214 LYW --- CharacterDialog : 패밀리 정보 새로고침 추가.
	RefreshFamilyInfo() ;
}

void CCharacterDialog::SetExpPointPercent(float perc)
{
	static char buf[66];
	sprintf(buf, "[%.2f%%]", perc);

#ifdef _CHEATENABLE_	
	if(CHEATMGR->IsCheatEnable())
	{
		char buf1[66];
		wsprintf(buf1, " : %d/%d", HERO->GetExpPoint(), HERO->GetMaxExpPoint());
		strcat(buf, buf1);
	}
#endif

	m_ppStatic.expPercent->SetStaticText(buf);
}
void CCharacterDialog::SetLevel(WORD level)
{
	// 070113 LYW --- Modified this line.
	//m_ppStatic.level->SetStaticValue(level);
	m_pChangeText[0]->SetStaticValue(level);
}
void CCharacterDialog::SetLife(DWORD life)
{
	// 070113 LYW --- Delete this part for a moment.
	/*
	//GuageOverCheck
	if(life > HERO->GetMaxLife())
		life = HERO->GetMaxLife();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", life, HERO->GetMaxLife());
	m_ppStatic.life->SetStaticText(buf1);

#else
	m_ppStatic.life->SetStaticValue(life);
#endif
	*/
	
}

void CCharacterDialog::SetMana(DWORD mana)
{
	// 070113 LYW --- Delete this part for a moment.
	/*
	//GuageOverCheck
	if(mana > HERO->GetMaxMana())
		mana = HERO->GetMaxMana();

#ifdef _DEBUG
	char buf1[66];
	wsprintf(buf1, "%d/%d", mana, HERO->GetMaxMana());
	m_ppStatic.Mana->SetStaticText(buf1);

#else
	m_ppStatic.Mana->SetStaticValue(mana);
#endif
	*/
}

// 070412 LYW ---  주요능력치 처리 ( 클라이언트 )
void CCharacterDialog::SetStrength()
{
	char buf[256] = {0,};
	DWORD resVal = HERO->GetStrength();		//KES 070530 : WORD --> DWORD
	DWORD baseVal = HERO->GetHeroTotalInfo()->Str;	//KES 070530 : WORD --> DWORD
	DWORD gapVal = 0;

	// 070412 LYW --- CharacterDialog : Apply extend color to strength
	if( m_pChangeText[6]->IsHasExtendColor() )
	{
		if( baseVal > resVal )
		{
			m_pChangeText[6]->SetFGColor( m_pChangeText[6]->GetExtendColor(1) ) ;

			gapVal = baseVal - resVal;

			sprintf( buf, "%d - %d", baseVal, gapVal );
		}
		else if( baseVal < resVal )
		{
			m_pChangeText[6]->SetFGColor( m_pChangeText[6]->GetExtendColor(2) ) ;

			gapVal = resVal - baseVal;
			
			sprintf( buf, "%d+%d", baseVal, gapVal );
		}
		else
		{
			m_pChangeText[6]->SetFGColor( m_pChangeText[6]->GetExtendColor(0) ) ;

			sprintf( buf, "%d", baseVal );
		}
	}

	m_pChangeText[6]->SetStaticText( buf );
}
void CCharacterDialog::SetDexterity()
{
	char buf[256] = {0,};
	DWORD resVal = HERO->GetDexterity();		//KES 070530 : WORD --> DWORD
	DWORD baseVal = HERO->GetHeroTotalInfo()->Dex;	//KES 070530 : WORD --> DWORD
	DWORD gapVal = 0;

	// 070412 LYW --- CharacterDialog : Apply extend color to strength
	if( m_pChangeText[7]->IsHasExtendColor() )
	{
		if( baseVal > resVal )
		{
			m_pChangeText[7]->SetFGColor( m_pChangeText[6]->GetExtendColor(1) ) ;

			gapVal = baseVal - resVal;

			sprintf( buf, "%d - %d", baseVal, gapVal );
		}
		else if( baseVal < resVal )
		{
			m_pChangeText[7]->SetFGColor( m_pChangeText[6]->GetExtendColor(2) ) ;

			gapVal = resVal - baseVal;
			
			sprintf( buf, "%d+%d", baseVal, gapVal );
		}
		else
		{
			m_pChangeText[7]->SetFGColor( m_pChangeText[6]->GetExtendColor(0) ) ;

			sprintf( buf, "%d", baseVal );
		}
	}

	m_pChangeText[7]->SetStaticText( buf );
}
void CCharacterDialog::SetVitality()
{
	char buf[256] = {0,};
	DWORD resVal = HERO->GetVitality();		//KES 070530 : WORD --> DWORD
	DWORD baseVal = HERO->GetHeroTotalInfo()->Vit;	//KES 070530 : WORD --> DWORD
	DWORD gapVal = 0;

	// 070412 LYW --- CharacterDialog : Apply extend color to strength
	if( m_pChangeText[8]->IsHasExtendColor() )
	{
		if( baseVal > resVal )
		{
			m_pChangeText[8]->SetFGColor( m_pChangeText[6]->GetExtendColor(1) ) ;

			gapVal = baseVal - resVal;

			sprintf( buf, "%d - %d", baseVal, gapVal );
		}
		else if( baseVal < resVal )
		{
			m_pChangeText[8]->SetFGColor( m_pChangeText[6]->GetExtendColor(2) ) ;

			gapVal = resVal - baseVal;
			
			sprintf( buf, "%d+%d", baseVal, gapVal );
		}
		else
		{
			m_pChangeText[8]->SetFGColor( m_pChangeText[6]->GetExtendColor(0) ) ;

			sprintf( buf, "%d", baseVal );
		}
	}

	m_pChangeText[8]->SetStaticText( buf );
}
void CCharacterDialog::SetWisdom()
{
	char buf[256] = {0,};
	DWORD resVal = HERO->GetWisdom();		//KES 070530 : WORD --> DWORD
	DWORD baseVal = HERO->GetHeroTotalInfo()->Wis;	//KES 070530 : WORD --> DWORD
	DWORD gapVal = 0;

	// 070412 LYW --- CharacterDialog : Apply extend color to strength
	if( m_pChangeText[9]->IsHasExtendColor() )
	{
		if( baseVal > resVal )
		{
			m_pChangeText[9]->SetFGColor( m_pChangeText[6]->GetExtendColor(1) ) ;

			gapVal = baseVal - resVal;

			sprintf( buf, "%d - %d", baseVal, gapVal );
		}
		else if( baseVal < resVal )
		{
			m_pChangeText[9]->SetFGColor( m_pChangeText[6]->GetExtendColor(2) ) ;

			gapVal = resVal - baseVal;
			
			sprintf( buf, "%d+%d", baseVal, gapVal );
		}
		else
		{
			m_pChangeText[9]->SetFGColor( m_pChangeText[6]->GetExtendColor(0) ) ;

			sprintf( buf, "%d", baseVal );
		}
	}

	m_pChangeText[9]->SetStaticText( buf );
}

void CCharacterDialog::SetIntelligence()
{
	char buf[256] = {0,};
	DWORD resVal = HERO->GetIntelligence();		//KES 070530 : WORD --> DWORD
	DWORD baseVal = HERO->GetHeroTotalInfo()->Int;	//KES 070530 : WORD --> DWORD
	DWORD gapVal = 0;

	// 070412 LYW --- CharacterDialog : Apply extend color to strength
	if( m_pChangeText[10]->IsHasExtendColor() )
	{
		if( baseVal > resVal )
		{
			m_pChangeText[10]->SetFGColor( m_pChangeText[6]->GetExtendColor(1) ) ;

			gapVal = baseVal - resVal;

			sprintf( buf, "%d - %d", baseVal, gapVal );
		}
		else if( baseVal < resVal )
		{
			m_pChangeText[10]->SetFGColor( m_pChangeText[6]->GetExtendColor(2) ) ;

			gapVal = resVal - baseVal;
			
			sprintf( buf, "%d+%d", baseVal, gapVal );
		}
		else
		{
			m_pChangeText[10]->SetFGColor( m_pChangeText[6]->GetExtendColor(0) ) ;

			sprintf( buf, "%d", baseVal );
		}
	}

	m_pChangeText[10]->SetStaticText( buf );
}
void CCharacterDialog::SetFame(FAMETYPE fame)
{
	m_ppStatic.fame->SetStaticValue(fame); 
}
void CCharacterDialog::SetBadFame(FAMETYPE badfame)
{
	// 070113 LYW --- Modified this line.
	//m_ppStatic.badfame->SetStaticValue(badfame); 
	m_pChangeText[1]->SetStaticValue(badfame); 
}

void CCharacterDialog::SetAttackRate()
{
	// 070410 LYW --- CharacterDialog : Modified function SetAttackRate.
	/*
	char buf1[66];
	DWORD minv,maxv;
	minv = HERO->GetPhyAttackPowerMin();
	maxv = HERO->GetPhyAttackPowerMax();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 일반 공격력 향상
	float val = 1 + HERO->GetSkillStatsOption()->BaseAtk;

	if( val < 0 )
		val = 0.0f;

	minv = (DWORD)((minv * val) + 0.5);
	maxv = (DWORD)((maxv * val) + 0.5);
	//////////////////////////////////////////////////////////////////////////
	// 070113 LYW --- Modified this line.
	//wsprintf(buf1, "%d ~ %d", minv, maxv);
	wsprintf(buf1, "%d", (minv+maxv)/2 ) ;

	if( HERO->GetWeaponEquipType() != WP_GUNG &&
		HERO->GetWeaponEquipType() != WP_AMGI )
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.rangeattack->SetStaticText("-");
		//m_ppStatic.meleeattack->SetStaticText(buf1);
		m_pChangeText[12]->SetStaticText(buf1);
		m_pChangeText[13]->SetStaticText("-");
	}
	else
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.meleeattack->SetStaticText("-");
		//m_ppStatic.rangeattack->SetStaticText(buf1);
		m_pChangeText[12]->SetStaticText("-");
		m_pChangeText[13]->SetStaticText(buf1);
	}

	// 070113 LYW --- Delete this line.
	//if( HERO->GetAvatarOption()->Attack )
	//{
	//	m_ppStatic.rangeattack->SetFGColor(TTTC_SHOPITEMOPTION);
	//	m_ppStatic.meleeattack->SetFGColor(TTTC_SHOPITEMOPTION);
	//}
	//else
	//{
	//	m_ppStatic.rangeattack->SetFGColor(RGB_HALF(221, 217, 158));
	//	m_ppStatic.meleeattack->SetFGColor(RGB_HALF(221, 217, 158));
	//}
	*/

	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetAttackRate() ) ;
	m_pChangeText[12]->SetStaticText(buf1) ;
}
void CCharacterDialog::SetDefenseRate()
{
	// 070410 LYW --- CharacterDialog : Modified function SetDefenseRate.
	/*.
	WORD value = (WORD)HERO->GetPhyDefense();
	value += (value*HERO->GetShopItemStats()->RegistPhys)/100;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 물리방어력
	float val = 1 + HERO->GetSkillStatsOption()->PhyDef;

	if( val < 0 )
		val = 0.0f;

	value = (WORD)(value * val + 0.5);
	//////////////////////////////////////////////////////////////////////////

	// 070113 LYW --- Modified this line.
	//m_ppStatic.defense->SetStaticValue(value);
	m_pChangeText[17]->SetStaticValue(value);

	// 070113 LYW --- Delete this line.
	//if(HERO->GetShopItemStats()->RegistPhys)
	//	m_ppStatic.defense->SetFGColor(TTTC_SHOPITEMOPTION);
	//else
	//	m_ppStatic.defense->SetFGColor(RGB_HALF(221, 217, 158));
	*/

	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetDefenseRate() ) ;
	m_pChangeText[17]->SetStaticText(buf1) ;
}
void CCharacterDialog::SetCritical()
{
	// 070410 LYW --- CharacterDialog : Modified function SetCritical.
	/*
	DWORD resVal1 = HERO->GetCritical();
	DWORD resVal2 = HERO->GetDecisive();
	resVal1 += HERO->GetShopItemStats()->Critical;
	resVal1 += HERO->GetAvatarOption()->Critical;
	resVal2 += HERO->GetShopItemStats()->Decisive;
	resVal2 += HERO->GetAvatarOption()->Decisive;

	char buf1[66];
	// 070114 LYW --- Modified this part.
	//wsprintf(buf1, "%d/%d", resVal1, resVal2);
	//m_ppStatic.critical->SetStaticText(buf1);
	
	//if(HERO->GetShopItemStats()->Critical || HERO->GetAvatarOption()->Critical)		
	//	m_ppStatic.critical->SetFGColor(TTTC_SHOPITEMOPTION);
	//else
	//	m_ppStatic.critical->SetFGColor(RGB_HALF(221, 217, 158));
	// 070117 LYW --- Modified this part.
	//wsprintf( buf1, "%d", resVal2/resVal2 ) ;
	//m_pChangeText[14]->SetStaticText(buf1);
	//if( resVal2 != 0 )
	//{
	//	wsprintf( buf1, "%d", resVal2/resVal2 ) ;
	//	m_pChangeText[14]->SetStaticText(buf1);
	//}
	//else
	//{
	//	m_pChangeText[14]->SetStaticText("0");
	//}
	*/
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetCritical() ) ;
	m_pChangeText[14]->SetStaticText(buf1) ;
}

// 070411 LYW --- CharacterDialog : Add function to print values of physical part.
void CCharacterDialog::PrintAttackRate() 
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetAttackRate() ) ;
	m_pChangeText[12]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintDefenseRate()
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetDefenseRate() ) ;
	m_pChangeText[17]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintMagAttackRate() 
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetMagAttackRate() ) ;
	m_pChangeText[18]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintMagDefenseRate() 
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetMagDefenseRate() ) ;
	m_pChangeText[21]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintAccuracyRate()
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetAccuracyRate() ) ;
	m_pChangeText[16]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintEvasionRate() 
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->GetEvasionRate() ) ;
	m_pChangeText[15]->SetStaticText(buf1) ;
}

void CCharacterDialog::PrintCriticalRate() 
{
	char buf1[66] = {0, } ;
	wsprintf( buf1, "%d", HERO->DoGetCriticalRate() ) ;
	m_pChangeText[14]->SetStaticText(buf1) ;
}


void CCharacterDialog::SetAttackRange()
{
	// 070114 LYW --- Modified this line.
	//m_ppStatic.attackdistance->SetStaticText("-");
	m_pChangeText[13]->SetStaticText( "-" ) ;
}
void CCharacterDialog::SetPointLeveling(BOOL val, WORD point)
{
	m_nocoriPoint = point;	

	// Hard Cording
	if( m_MinusPoint != -1 )
		m_nocoriPoint += (30 - HERO->GetShopItemStats()->StatePoint);

	// 070113 LYW --- Modified this line.
	//m_ppStatic.point->SetStaticValue( m_nocoriPoint );
	m_pChangeText[11]->SetStaticValue( m_nocoriPoint );
	// 070113 LYW --- Delete this line.
	/*
	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
		m_pPointBtn[i]->SetActive(val);
		*/
	
	HERO->SetHeroLevelupPoint( point );

	// 070124 LYW --- Setting status button.
	if( m_nocoriPoint > 0 )
	{
		for(int i = 0 ; i < MAX_BTN_POINT ; i++)
			m_pStatusBtn[i]->SetActive(TRUE);
	}
	else
	{
		for(int i = 0 ; i < MAX_BTN_POINT ; i++)
			m_pStatusBtn[i]->SetActive(FALSE);
	}
}
void CCharacterDialog::SetPointLevelingHide()
{
	WORD lvpoint = 0;
	if( m_MinusPoint != -1 )
		lvpoint = (30 - HERO->GetShopItemStats()->StatePoint);

	// 070113 LYW --- Modified this line.
	//m_ppStatic.point->SetStaticValue( lvpoint );
	m_pChangeText[11]->SetStaticValue( lvpoint );
	// 070113 LYW --- Delete this line.
	/*
	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
		m_pPointBtn[i]->SetActive(FALSE);
		*/
	
	HERO->SetHeroLevelupPoint( 0 );
}

void CCharacterDialog::SetMinusPoint(BOOL val, WORD point)
{
	// 070111 LYW --- Delete this code.
	/*
	for(int i = 0 ; i < MAX_BTN_POINT ; i++)
		m_pPointMinusBtn[i]->SetActive(val);
		*/

	// Hard Cording	
	if( val )
	{
		WORD LevelupPoint = (WORD)HERO->GetHeroLevelUpPoint();
		LevelupPoint += (30 - HERO->GetShopItemStats()->StatePoint);
		// 070113 LYW --- Modified this line.
		//m_ppStatic.point->SetStaticValue( LevelupPoint );
		m_pChangeText[11]->SetStaticValue( LevelupPoint );
	}

	// 
	if( val )
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.point->SetFGColor(TTTC_SHOPITEMOPTION);
		m_pChangeText[11]->SetFGColor(TTTC_SHOPITEMOPTION);
		m_MinusPoint = 30 - HERO->GetShopItemStats()->StatePoint;
	}
	else
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.point->SetStaticValue( HERO->GetHeroLevelUpPoint() );
		//m_ppStatic.point->SetFGColor(RGB_HALF(221, 217, 158));
		m_pChangeText[11]->SetStaticValue( HERO->GetHeroLevelUpPoint() );
		m_pChangeText[11]->SetFGColor(RGB_HALF(221, 217, 158));
		m_MinusPoint = -1;
	}	
}
void CCharacterDialog::OnAddPoint(BYTE whatsPoint)
{
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_POINTADD_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = whatsPoint;

	
	if(--m_nocoriPoint <= 0)
	{
		SetPointLeveling(FALSE);
		//return;
	}
	// 070114 LYW --- Modified this line.
	//m_ppStatic.point->SetStaticValue(m_nocoriPoint);
	m_pChangeText[11]->SetStaticValue(m_nocoriPoint);
	
	NETWORK->Send(&msg,sizeof(MSG_WORD));
}
void CCharacterDialog::OnMinusPoint(BYTE whatsPoint)
{
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_POINTMINUS_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = whatsPoint;

	NETWORK->Send(&msg,sizeof(MSG_WORD));
}


void CCharacterDialog::RefreshGuildInfo()
{
	BYTE rank = HERO->GetGuildMemberRank();
	// 070113 LYW --- Modified this line.
	//m_ppStatic.munpa->SetStaticText(HERO->GetGuildName());
	if( strlen(HERO->GetGuildName()) <= 0 ) 
	{
		m_pChangeText[3]->SetStaticText("") ;
	}
	else
	{
		m_pChangeText[3]->SetStaticText(HERO->GetGuildName());
	}
	// 070417 LYW --- CharacterDialog : Deactive nickName.
	/*
	if(rank == GUILD_NOTMEMBER)
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.jikwe->SetStaticText("");
		m_pChangeText[4]->SetStaticText("");
	}
	else
	{
		// 070113 LYW --- Modified this line.
		//m_ppStatic.jikwe->SetStaticText( GUILDMGR->GetRankName(HERO->GetGuildMemberRank()) );
		m_pChangeText[4]->SetStaticText( GUILDMGR->GetRankName(HERO->GetGuildMemberRank()) );
	}
	// 070114 LYW --- Delete this line.
	//m_ppStatic.fame->SetStaticValue(HERO->GetFame());
	*/
}

// 070503 LYW --- CharacterDialog : Add function to setting family name
void CCharacterDialog::RefreshFamilyInfo()
{
	/*if( strlen(HERO->GetFamilyName()) < 4 )
	{
		m_pFamilyName->SetStaticText("") ;
	}
	else
	{
		m_pFamilyName->SetStaticText(HERO->GetFamilyName()) ;
	}*/

	CSHFamily* pFamily = HERO->GetFamily() ;

	if( pFamily )																	// 패밀리 정보가 유효한지 체크한다.
	{
		m_pFamilyName->SetStaticText(pFamily->Get()->szName) ;						// 패밀리명을 세팅한다.
	}
	else
	{
		m_pFamilyName->SetStaticText("") ;
	}
}

void CCharacterDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	// 070112 LYW --- Call function to check image of character.
	CheckHeroImage() ;

	// 071022 LYW --- CharacterDialog : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), val) ;

	// 070124 LYW --- Delete this part.
	/*
	// 070122 LYW --- Setting button.
	if( m_nocoriPoint > 0 )
	{
		for(int i = 0 ; i < MAX_BTN_POINT ; i++)
			m_pStatusBtn[i]->SetActive(TRUE);
	}
	else
	{
		for(int i = 0 ; i < MAX_BTN_POINT ; i++)
			m_pStatusBtn[i]->SetActive(FALSE);
	}
	*/
}

void CCharacterDialog::RefreshInfo()
{
	// 070411 LYW --- CharacterDialog : Modified function RefreshInfo.
	/*
	SetStrength();
	SetDexterity();
	SetVitality();
	SetWisdom();
	SetCritical();
	SetAttackRate();

	STATSMGR->CalcCharStats(HERO);
	SetDefenseRate();
	*/

	SetStrength();
	SetDexterity();
	SetVitality();
	SetIntelligence() ;
	SetWisdom();

	PrintAttackRate() ;
	PrintDefenseRate() ;
	PrintMagAttackRate() ;
	PrintMagDefenseRate() ;
	PrintAccuracyRate() ;
	PrintEvasionRate() ;
	PrintCriticalRate() ;
}


// 070111 LYW --- Add function to process events.
//=================================================================================================
// NAME			: ActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
DWORD CCharacterDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL ;

	we |= cDialog::ActionEvent( mouseInfo ) ;

	return we ;
}


//=================================================================================================
// NAME			: OnActionEvent
// PURPOSE		: Processing envet function.
// ATTENTION	:
//=================================================================================================
void CCharacterDialog::OnActionEvent( LONG lId, void* p, DWORD we )
{
	if( lId == CI_CLASSTREEBTN )
	{
		if( !m_bCheckedTreeInfo )
		{
			m_bCheckedTreeInfo = TRUE ;
			// 070416 LYW --- CharacterDialog : Delete call check tree.
			//CheckTreeInfo() ;
		}

		if( m_bShowTreeDlg )
		{
			m_bShowTreeDlg = FALSE ;
			ShowTreeDlg() ;
		}
		else
		{
			m_bShowTreeDlg = TRUE ;
			ShowTreeDlg() ;
		}
	}
}


// 070111 LYW --- Add function to check and show tree information.
//=================================================================================================
// NAME			: CheckTreeInfo
// PURPOSE		: Check information of class tree.
// ATTENTION	:
//=================================================================================================
void CCharacterDialog::CheckTreeInfo()
{
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( HERO->GetID() );
    
	if( !pPlayer )
		return;

	BYTE jobGrade	= pPlayer->GetCharacterTotalInfo()->JobGrade ;
	BYTE job		= pPlayer->GetCharacterTotalInfo()->Job[0] ;

	switch( job )
	{
	case 1 :
		{
			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 41) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;

			// 070118 LYW --- Delete this part.
			/*
			m_pClassStatic[0]->SetStaticText( "휴먼 파이터" ) ;
			m_pClassStatic[1]->SetStaticText( "워리어" ) ;
			m_pClassStatic[2]->SetStaticText( "다크 나이트" ) ;
			m_pClassStatic[3]->SetStaticText( "버서커" ) ;
			m_pClassStatic[4]->SetStaticText( "소드 마스터" ) ;
			m_pClassStatic[5]->SetStaticText( "그랜드 소드" ) ;
			m_pClassStatic[6]->SetStaticText( "마스터" ) ;
			m_pClassStatic[7]->SetStaticText( "데스 나이트" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/
		}
		break ;

	case 2 :
		{
			// 070118 LYW --- Modified this part.
			/*
			m_pClassStatic[0]->SetStaticText( "휴먼 로그" ) ;
			m_pClassStatic[1]->SetStaticText( "시프" ) ;
			m_pClassStatic[2]->SetStaticText( "호크" ) ;
			m_pClassStatic[3]->SetStaticText( "헌터" ) ;
			m_pClassStatic[4]->SetStaticText( "팬텀" ) ;
			m_pClassStatic[5]->SetStaticText( "대거 마스터" ) ;
			m_pClassStatic[6]->SetStaticText( "" ) ;
			m_pClassStatic[7]->SetStaticText( "어쎄신" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/

			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 42) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
		}
		break ;

	case 3 :
		{
			// 070118 LYW --- Modified this part.
			/*
			m_pClassStatic[0]->SetStaticText( "휴먼 메이지" ) ;
			m_pClassStatic[1]->SetStaticText( "카발리스트" ) ;
			m_pClassStatic[2]->SetStaticText( "소서러" ) ;
			m_pClassStatic[3]->SetStaticText( "매지션" ) ;
			m_pClassStatic[4]->SetStaticText( "위저드" ) ;
			m_pClassStatic[5]->SetStaticText( "엘리멘탈" ) ;
			m_pClassStatic[6]->SetStaticText( "메이지" ) ;
			m_pClassStatic[7]->SetStaticText( "네츄럴 메이지" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/

			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 43) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
		}
		break ;

	case 4 :
		{
			// 070118 LYW --- Modified this part.
			/*
			m_pClassStatic[0]->SetStaticText( "엘프 파이터" ) ;
			m_pClassStatic[1]->SetStaticText( "하이 나이트" ) ;
			m_pClassStatic[2]->SetStaticText( "팔라딘" ) ;
			m_pClassStatic[3]->SetStaticText( "가드" ) ;
			m_pClassStatic[4]->SetStaticText( "스킨 마스터" ) ;
			m_pClassStatic[5]->SetStaticText( "그랜드 스킨" ) ;
			m_pClassStatic[6]->SetStaticText( "마스터" ) ;
			m_pClassStatic[7]->SetStaticText( "엔젤 나이트" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/
			
			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 44) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
		}
		break ;

	case 5 :
		{
			// 070118 LYW --- Modified this part.
			/*
			m_pClassStatic[0]->SetStaticText( "엘프 로그" ) ;
			m_pClassStatic[1]->SetStaticText( "아처" ) ;
			m_pClassStatic[2]->SetStaticText( "스카우트" ) ;
			m_pClassStatic[3]->SetStaticText( "써그" ) ;
			m_pClassStatic[4]->SetStaticText( "슈터" ) ;
			m_pClassStatic[5]->SetStaticText( "보우 마스터" ) ;
			m_pClassStatic[6]->SetStaticText( "" ) ;
			m_pClassStatic[7]->SetStaticText( "레인져" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/

			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 45) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
		}
		break ;

	case 6 :
		{
			// 070118 LYW --- Modified this part.
			/*
			m_pClassStatic[0]->SetStaticText( "엘프 메이지" ) ;
			m_pClassStatic[1]->SetStaticText( "어콜라이트" ) ;
			m_pClassStatic[2]->SetStaticText( "클레릭" ) ;
			m_pClassStatic[3]->SetStaticText( "몽크" ) ;
			m_pClassStatic[4]->SetStaticText( "프리스트" ) ;
			m_pClassStatic[5]->SetStaticText( "세인트" ) ;
			m_pClassStatic[6]->SetStaticText( "" ) ;
			m_pClassStatic[7]->SetStaticText( "디바인" ) ;
			m_pClassStatic[8]->SetStaticText( "" ) ;
			*/

			// 070122 LYW --- Modified this line.
			//m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[0]->SetStaticText( RESRCMGR->GetMsg( 46) ) ;
			m_pClassStatic[1]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[2]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[3]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[4]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[5]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[6]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[7]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
			m_pClassStatic[8]->SetStaticText( RESRCMGR->GetMsg( 982) ) ;
		}
		break ;
	}

	for(int i=0; i<9; ++i)
	{
		m_pClassStatic[i]->SetFGColor( RGB(255, 255, 255) ) ;
	}
	switch( jobGrade )
	{
	case 1 :
		{
			m_pClassStatic[0]->SetFGColor( RGB(0, 255, 255) ) ;
			m_pChangeText[5]->SetStaticText( m_pClassStatic[0]->GetStaticText() ) ;
		}
		break ;

	case 2 :
		{
			m_pClassStatic[1]->SetFGColor( RGB(0, 255, 255) ) ;
			m_pChangeText[5]->SetStaticText( m_pClassStatic[1]->GetStaticText() ) ;
		}
		break ;

	case 3 :
		{
			BYTE thirdJob = pPlayer->GetCharacterTotalInfo()->Job[2] ;

			if( (thirdJob%2) != 0 )
			{
				m_pClassStatic[2]->SetFGColor( RGB(0, 255, 255) ) ;
				m_pChangeText[5]->SetStaticText( m_pClassStatic[2]->GetStaticText() ) ;
			}
			else
			{
				m_pClassStatic[3]->SetFGColor( RGB(0, 255, 255) ) ;
				m_pChangeText[5]->SetStaticText( m_pClassStatic[3]->GetStaticText() ) ;
			}
		}
		break ;

	case 4 :
		{
			m_pClassStatic[4]->SetFGColor( RGB(0, 255, 255) ) ;
			m_pChangeText[5]->SetStaticText( m_pClassStatic[4]->GetStaticText() ) ;
		}
		break ;

	case 5 :
		{
			BYTE thirdJob = pPlayer->GetCharacterTotalInfo()->Job[4] ;

			char tempClassName[128] = {0, } ;

			if( (thirdJob%2) != 0 )
			{
				m_pClassStatic[5]->SetFGColor( RGB(0, 255, 255) ) ;
				m_pClassStatic[6]->SetFGColor( RGB(0, 255, 255) ) ;

				sprintf( tempClassName, "%s%s", m_pClassStatic[5]->GetStaticText(), m_pClassStatic[5]->GetStaticText() ) ;
				m_pChangeText[5]->SetStaticText( tempClassName ) ;
			}
			else
			{
				m_pClassStatic[7]->SetFGColor( RGB(0, 255, 255) ) ;
				m_pClassStatic[8]->SetFGColor( RGB(0, 255, 255) ) ;

				sprintf( tempClassName, "%s%s", m_pClassStatic[7]->GetStaticText(), m_pClassStatic[8]->GetStaticText() ) ;
				m_pChangeText[5]->SetStaticText( tempClassName ) ;
			}
		}
		break ;
	}
}


//=================================================================================================
// NAME			: ShowTreeDlg
// PURPOSE		: Setting whether controls for tree dialog to show or not.
// ATTENTION	:
//=================================================================================================
void CCharacterDialog::ShowTreeDlg()
{
	int count = 0 ;

	if( m_bShowTreeDlg )
	{
		m_pTreeBgImg->SetActive( TRUE ) ;
		m_pTreeMark->SetActive( TRUE ) ;
		m_pTreeTitle->SetActive( TRUE ) ;

		for( count = 0 ; count < MAX_SUBITEM ; ++count )
		{
			m_pSubItem[ count ]->SetActive( TRUE ) ;
		}

		for( count = 0 ; count < MAX_CLASS_STATIC ; ++count )
		{
			m_pClassStatic[ count ]->SetActive( TRUE ) ;
		}
	}
	else
	{
		m_pTreeBgImg->SetActive( FALSE ) ;
		m_pTreeMark->SetActive( FALSE ) ;
		m_pTreeTitle->SetActive( FALSE ) ;

		for( count = 0 ; count < MAX_SUBITEM ; ++count )
		{
			m_pSubItem[ count ]->SetActive( FALSE ) ;
		}

		for( count = 0 ; count < MAX_CLASS_STATIC ; ++count )
		{
			m_pClassStatic[ count ]->SetActive( FALSE ) ;
		}
	}
}


//=================================================================================================
// NAME			: ShowTreeDlg
// PURPOSE		: Add function to check character image.
// ATTENTION	: // 070112 LYW --- Add function to check character image.
//=================================================================================================
void CCharacterDialog::CheckHeroImage()
{
	int count = 0 ;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( HERO->GetID() );

	m_byCharImgNum	= pPlayer->GetCharacterTotalInfo()->Race ;

	for( count = 0 ; count < MAX_CHARIMAGE ; ++count )
	{
		m_pCharImage[ count ]->SetActive( FALSE ) ;
	}

	switch( m_byCharImgNum )
	{
		// 070122 LYW --- Modified this part.

	case 0x00 :
		{
			if( pPlayer->GetCharacterTotalInfo()->Gender == 0 )
			{
				m_pCharImage[2]->SetActive( TRUE ) ;
			}
			else
			{
				m_pCharImage[3]->SetActive( TRUE ) ;
			}
		}
		break ;

	case 0x01 :
		{
			if( pPlayer->GetCharacterTotalInfo()->Gender == 0 )
			{
				m_pCharImage[0]->SetActive( TRUE ) ;
			}
			else
			{
				m_pCharImage[1]->SetActive( TRUE ) ;
			}
		}
		break ;
		/*
	case 0x01 :
		{
			m_pCharImage[0]->SetActive( TRUE ) ;
		}
		break ;

	case 0x02 :
		{
			m_pCharImage[1]->SetActive( TRUE ) ;
		}
		break ;

	case 0x03 :
		{
			m_pCharImage[2]->SetActive( TRUE ) ;
		}
		break ;

	case 0x04 :
		{
			m_pCharImage[3]->SetActive( TRUE ) ;
		}
		break ;
		*/
	}
}
