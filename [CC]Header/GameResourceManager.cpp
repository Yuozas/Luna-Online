// GameResourceManager.cpp: implementation of the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifndef _MURIMNET_

#include "GameResourceManager.h"
#include "MHFile.h"
#include "../[cc]skill/client/info/ActiveSkillInfo.h"
#ifdef _CLIENT_
#include "./Audio/MHAudioManager.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "CharSelect.h"
#else
#include "../[cc]skill/server/manager/skillmanager.h"
#include "ItemDrop.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGameResourceManager);

BOOL g_bTESTSERVER = FALSE;
extern int g_nServerSetNum;


CGameResourceManager::CGameResourceManager()
{
	m_bNpcRegenLoad = FALSE;
	m_bMonsterListLoad = FALSE;
	m_bNpcListLoad = FALSE;
	m_bMonsterDropItem = FALSE;

	m_TotalNpcGroupCount = 0;
	m_MaxNpcRegenCount = 0;

	m_SMonsterListTable.Initialize(100);

#ifdef _SERVER_RESOURCE_FIELD_
	m_StaticNpcTable.Initialize( 300 );
	// 070325 LYW --- GameResourceManager : Modified WareNpcTable count.
	//m_WareNpcTable.Initialize( 10 );
	m_WareNpcTable.Initialize( 20 );
#else
	m_PreLoadDataTable.Initialize(4);	
	m_PreLoadEffectTable.Initialize(25);	

#endif //_SERVER_RESOURCE_FIELD_

#ifdef _CLIENT_RESOURCE_FIELD_
	m_PreLoadItemlist = NULL;
#endif	
	m_AvatarEquipTable.Initialize(50);
	m_ShopItemDupOptionTable.Initialize(30);	

#ifdef _SERVER_RESOURCE_FIELD_
	m_HideNpcTable.Initialize(3);

//---KES 스킬초기화돈	071207
	m_htSkillMoney.Initialize(100);
	// 071221 KTH -- 기억할수 있는 맵 리스트
	m_PreLoadMemoryMapList.Initialize(100);
//----------------------------
#endif
}

CGameResourceManager::~CGameResourceManager()
{
	SAFE_DELETE_ARRAY(m_NpcChxList);	

	ResetMonsterList();

#ifdef _CLIENT_RESOURCE_FIELD_
	DeletePreLoadData();
#endif

#ifdef _SERVER_RESOURCE_FIELD_
	ReleaseStaticNpc();
#endif
	//
	AVATARITEM* pItem = NULL;
	m_AvatarEquipTable.SetPositionHead();
	while(pItem = m_AvatarEquipTable.GetData())
	{
		if(pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_AvatarEquipTable.RemoveAll();

	SHOPITEMDUP* pDupOption = NULL;
	m_ShopItemDupOptionTable.SetPositionHead();
	while(pDupOption = m_ShopItemDupOptionTable.GetData())
	{
		if(pDupOption)
		{
			delete pDupOption;
			pDupOption = NULL;
		}
	}
	m_ShopItemDupOptionTable.RemoveAll();

#ifdef _SERVER_RESOURCE_FIELD_
	HIDE_NPC_INFO* info = NULL;
	m_HideNpcTable.SetPositionHead();
	while(info = m_HideNpcTable.GetData())
	{
		if(info)
		{
			delete info;
			info = NULL;
		}
	}
	m_HideNpcTable.RemoveAll();

//---KES 스킬초기화돈	071207
	m_htSkillMoney.SetPositionHead();
	while( SKILL_MONEY* pSkillMoney = m_htSkillMoney.GetData() )
	{
		delete pSkillMoney;
	}
	m_htSkillMoney.RemoveAll();
//----------------------------

#endif

	// 070420 LYW --- GameResourceManager : Release m_ModFileList.
	ReleaseModFileList() ;
}

// 070420 LYW --- GameResourceManager : Add function to release m_ModFileList.
void CGameResourceManager::ReleaseModFileList()
{
	
}

BOOL CGameResourceManager::LoadDeveloperInfo()
{	
	CMHFile file;
	
	if(!file.Init(
			"../DeveloperWorking/Developer.ini", 
			"rt", 
			MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		return FALSE;
	}
		
	char value[256];

	while( ! file.IsEOF() )
	{
		file.GetString( value );
		
		if( ! strcmpi(value,"*WINDOWMODE") )
		{
#ifdef _CLIENT_
			m_GameDesc.dispInfo.dispType = file	.GetBool() ? WINDOW_WITH_BLT : FULLSCREEN_WITH_BLT;
#endif
		}
		if( ! strcmpi(value,"*ASSERTMSGBOX") )
		{
			g_bAssertMsgBox = file.GetBool();
		}
		else if( ! strcmpi(value,"*TESTSERVER") )
		{
			g_bTESTSERVER = file.GetBool();
		}
	}

	return TRUE;
}

void CGameResourceManager::ResetMonsterList()
{
	BASE_MONSTER_LIST * pInfo = NULL;
	m_SMonsterListTable.SetPositionHead();
	while(pInfo = m_SMonsterListTable.GetData())
	{
		delete [] pInfo->AttackIndex;
		delete [] pInfo->AttackRate;
		delete [] pInfo->SkillInfo;

		pInfo->AttackIndex = NULL;
		pInfo->AttackRate = NULL;
		pInfo->SkillInfo = NULL;
		delete pInfo;
		pInfo = NULL;
	}
	m_SMonsterListTable.RemoveAll();
}

void CGameResourceManager::LoadUserCommendList()
{
	m_UserCommendList.clear();

	CMHFile file;
	if(!file.Init("./System/Resource/UserCommandList.bin", "rb"))
		return;

	while(1)
	{
		if(file.IsEOF())
			break;

		std::string szUserAction;

		int ActionNumber = file.GetInt();

		szUserAction = file.GetString();

		m_UserCommendList.push_back(szUserAction);
	}
}

void CGameResourceManager::LoadItemChangeRateofAttrib()
{
	m_ItemChangeRateAttribList.clear();

	CMHFile file;
	if(!file.Init("./System/Resource/AttribItemChangeRato.bin", "rb"))
		return;

	while(1)
	{
		if(file.IsEOF())
			break;

		ItemChangeRateofAttrib ChangeRateInfo;

		for(int i = 0; i < 5; ++i)
		{
			ChangeRateInfo.attribinfo[i].changeIndex = file.GetInt();
			ChangeRateInfo.attribinfo[i].changeRate = file.GetInt();
		}

		m_ItemChangeRateAttribList.push_back(ChangeRateInfo);
	}

}

void CGameResourceManager::LoadItemChangeRateofLv()
{
	m_ItemChangeRateofLvList.clear();

	CMHFile file;
	if(!file.Init("./System/Resource/ItemChangeRateofLv.bin", "rb"))
	{
		///::MessageBox(NULL,"ItemChangeRateofLv.bin - 파일이 올바로 로드되지 않았습니다.","경고",MB_OK);
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		ItemChangeRateofLv ChangeRateInfo;

		ChangeRateInfo.Level = file.GetInt();

		for(int i = 0; i < 10; ++i)
		{
			ChangeRateInfo.Rate[i] = file.GetInt();//변화율 저장
		}

		m_ItemChangeRateofLvList.push_back(ChangeRateInfo);
	}



}

int CGameResourceManager::CheckAttribItemIndex(WORD idx)
{
	for(unsigned int i = 0; i < m_ItemChangeRateAttribList.size(); ++i)
	{
		if(m_ItemChangeRateAttribList[i].attribinfo[0].changeIndex == idx)
			return i;
	}

	return -1;
}



void CGameResourceManager::LoadMonsterInfoInMapList()
{
	//파일을 로드하여 정보를 저장한다.

	m_MonsterInfoInMapList.clear();

	CMHFile file;
	if(!file.Init("./System/Resource/MonsterInfoInMap.bin", "rb"))
		return;

	while(1)
	{
		if(file.IsEOF())
			break;

		MonsterInfoInMap MonsterInfo;

		MonsterInfo.MapNumber = file.GetInt();
		char* s = file.GetString();
		MonsterInfo.MinLevel = file.GetInt();
		MonsterInfo.MaxLevel = file.GetInt();

		m_MonsterInfoInMapList.push_back(MonsterInfo);
	}

	//unsigned int size = m_MonsterInfoInMapList.size();

}

unsigned int CGameResourceManager::GetMinMonsterLvInMap(unsigned int MapNum)
{
	unsigned int MinLv = 0;
	for(unsigned int i = 0; i < m_MonsterInfoInMapList.size(); ++i)
	{
		if(m_MonsterInfoInMapList[i].MapNumber == MapNum)
		{
			MinLv = m_MonsterInfoInMapList[i].MinLevel;
			return MinLv;
		}
	}

	return MinLv;
}

void CGameResourceManager::LoadSMonsterList()
{
	CMHFile file;
	if(!file.Init("./System/Resource/SMonsterList.bin", "rb"))
		return;

	//SMONSTER_LIST * pSMonsterList = NULL;
	int num = file.GetInt();
	for( int i = 0 ; i < num ; ++i )
	{
		file.GetInt();
		LoadSMonsterInfo(i, file.GetString());
	}
}
BASE_MONSTER_LIST * CGameResourceManager::GetMonsterListInfo(int idx)
{
	return m_SMonsterListTable.GetData(idx);
}

int CGameResourceManager::GetMonsterIndexForName( char* pStrName )
{
	m_SMonsterListTable.SetPositionHead();
	BASE_MONSTER_LIST* pList;
	while( pList = m_SMonsterListTable.GetData() )
	{
		if( strcmp( pList->Name, pStrName ) == 0 )
			return pList->MonsterKind;
	}

	return 0;
}

void CGameResourceManager::LoadSMonsterInfo( int idx, char * fileName )
{
	SMONSTER_LIST * pList = new SMONSTER_LIST;
	memset(pList, 0, sizeof(SMONSTER_LIST));

	CMHFile fp;
	char buff[256];
	sprintf(buff, "./System/Resource/%s", fileName);
	if(!fp.Init(buff, "rb"))
		return;

	WORD aidx=0;
	WORD anum=0;
	DWORD ridx=0;
	DWORD attackIdx = 0;
	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
		else if(buff[0] == '{' || buff[0] == '}')
		{
			continue;
		}
		CMD_ST(buff)
			CMD_CS("#MONSTERKIND")
			pList->MonsterKind = fp.GetWord();
		CMD_CS("#NAME")
			SafeStrCpy( pList->Name, fp.GetStringInQuotation(), MAX_NAME_LENGTH+1 );
		CMD_CS("#ENGNAME")
			//		SafeStrCpy( pList->EngName, fp.GetStringInQuotation(), MAX_NAME_LENGTH+1 );			
			CMD_CS("#LEVEL")
			pList->Level = fp.GetLevel();
		CMD_CS("#SCALE")
			pList->Scale = fp.GetFloat();
		CMD_CS("#GRAVITY")
			pList->Gravity = fp.GetDword();
		CMD_CS("#WEIGHT")
			pList->Weight = fp.GetFloat();
		CMD_CS("#MONSTERRADIUS")
			pList->MonsterRadius = fp.GetDword();
		CMD_CS("#LIFE")
			pList->Life = fp.GetDword();
		CMD_CS("#EXPPOINT")
			// 080604 LYW --- GameResourceManager : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
			//pList->ExpPoint = fp.GetExpPoint();
			pList->ExpPoint = fp.GetDword() ;
		CMD_CS("#TALL")
			pList->Tall = fp.GetWord();
		CMD_CS("#CHXNAME")
			fp.GetString(pList->ChxName);

		CMD_CS("#ATTACKPHYSICALMIN")
			pList->AttackPhysicalMin = fp.GetWord();
		CMD_CS("#ATTACKPHYSICALMAX")
			pList->AttackPhysicalMax = fp.GetWord();
		CMD_CS("#CRITICAL")
			pList->CriticalPercent = fp.GetWord();
		CMD_CS("#PHYSICALDEFENSE")
			pList->PhysicalDefense = fp.GetWord();
		CMD_CS("#PROPERTYRESIST1")
			//			pList->AttribResist.SetElement_Val(ATTR_FIRE,fp.GetFloat());
			CMD_CS("#PROPERTYRESIST2")
			//			pList->AttribResist.SetElement_Val(ATTR_WATER,fp.GetFloat());
			CMD_CS("#PROPERTYRESIST3")
			//			pList->AttribResist.SetElement_Val(ATTR_TREE,fp.GetFloat());
			CMD_CS("#PROPERTYRESIST4")
			//			pList->AttribResist.SetElement_Val(ATTR_IRON,fp.GetFloat());
			CMD_CS("#PROPERTYRESIST5")
			//			pList->AttribResist.SetElement_Val(ATTR_EARTH,fp.GetFloat());

			CMD_CS("#WALKMOVE")
			pList->WalkMove = fp.GetWord();
		CMD_CS("#RUNMOVE")
			pList->RunMove = fp.GetWord();
		CMD_CS("#RUNAWAYMOVE")
			pList->RunawayMove = fp.GetWord();

		CMD_CS("#STANDTIME")
			pList->StandTime = fp.GetDword();
		CMD_CS("#STANDRATE")
			pList->StandRate = fp.GetByte();
		CMD_CS("#DOMAINRANGE")
			pList->DomainRange = fp.GetDword();
		CMD_CS("#PURSUITFORGIVETIME")
			pList->PursuitForgiveTime = fp.GetDword();
		CMD_CS("#PURSUITFORGIVEDISTANCE")
			pList->PursuitForgiveDistance = fp.GetDword();
		CMD_CS("#FOREATTACK")
			pList->bForeAttack = fp.GetBool();
		CMD_CS("#SEARCHPERIODICTIME")
			pList->SearchPeriodicTime = fp.GetDword();
		CMD_CS("#TARGETSELECT")
			pList->TargetSelect = fp.GetWord();
		CMD_CS("#SEARCHRANGE")
			pList->SearchRange = fp.GetDword();
		CMD_CS("#SEARCHANGLE")
			pList->SearchAngle = fp.GetDword();
		CMD_CS("#TARGETCHANGE")
			pList->TargetChange = fp.GetWord();

		// 수정 되어야 할 부분
		CMD_CS("#MOVEDRAMANUM")
			//			pList->MoveDramaNum = FindEffectNum(fp.GetString());
			CMD_CS("#MOTIONID");
		//	pList->MotionID = fp.GetDword();
		CMD_CS("#DAMAGE1DRAMANUM")
			pList->Damage1DramaNum = FindEffectNum(fp.GetString());
		CMD_CS("#DIE1DRAMANUM")
			pList->Die1DramaNum = FindEffectNum(fp.GetString());
		CMD_CS("#DIE2DRAMANUM")
			pList->Die2DramaNum = FindEffectNum(fp.GetString());


		CMD_CS("#ATTACKNUM")
			pList->AttackNum = fp.GetDword();

		pList->AttackIndex = new DWORD[pList->AttackNum];
		pList->AttackRate = new DWORD[pList->AttackNum];
		pList->SkillInfo = new cActiveSkillInfo*[pList->AttackNum];
		for(DWORD i = 0 ; i < pList->AttackNum ; ++i)
		{
			pList->AttackIndex[i] = 0;
			pList->AttackRate[i] = 0;
			pList->SkillInfo[i] = 0;
		}
		CMD_CS("#ATTACKKIND")
			pList->AttackKind = fp.GetWord();
		CMD_CS("#ATTACK")
			attackIdx =  fp.GetDword();
		ASSERT( attackIdx < pList->AttackNum );
		/*		CMD_CS("#ATTACKINDEX")
		pList->AttackIndex[attackIdx] = fp.GetDword();
		pList->SkillInfo[attackIdx] = SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[attackIdx]);
		ASSERT(pList->SkillInfo[attackIdx]);
		*/		CMD_CS("#ATTACKRATE")
		pList->AttackRate[attackIdx] = fp.GetDword();

		// ATTACK PATTERN INFO
		CMD_CS("#ATTACKPATTERNNUM")
			pList->AttackPatternClass.Init(fp.GetWord());
		CMD_CS("#ATTACKPATTERN")
			aidx = fp.GetWord();
		anum = fp.GetWord();
		CMD_CS("#ATTACKPATTERNINDEX")
			pList->AttackPatternClass.InitPattern(aidx, anum);
		for(int i = 0 ; i < anum ; ++i )
			pList->AttackPatternClass.SetVal(aidx, i, fp.GetWord());

		//RECALL INFOMATION
		CMD_CS("#RECALLNUM")
			pList->RecallInfoClass.Init(fp.GetDword());
		CMD_CS("#RECALL")
			ridx = fp.GetDword();
		CMD_CS("#RECALLTYPE")
			pList->RecallInfoClass.SetRecallType(ridx, fp.GetDword());
		CMD_CS("#RECALLID")
			pList->RecallInfoClass.SetRecallID(ridx, fp.GetDword());
		CMD_CS("#RECALLMONSTERNUM")
			pList->RecallInfoClass.SetRecallMonsterNum(ridx, fp.GetDword());
		CMD_CS("#RECALLRANGE")
			pList->RecallInfoClass.SetRecallRange(ridx, fp.GetDword());

		//HELP
		CMD_CS("#INITHELPTYPE")
			pList->InitHelpType = fp.GetWord();
		CMD_CS("#HEARING")
			pList->bHearing = fp.GetBool();
		CMD_CS("#HEARINGDISTANCE")
			pList->HearingDistance = fp.GetDword();

		CMD_EN
	}

	ASSERT(m_SMonsterListTable.GetData(pList->MonsterKind) == NULL);
	/*
	#ifdef _TESTCLIENT_
	if(m_SMonsterListTable.GetData(pList->MonsterKind))
	m_SMonsterListTable.Remove(pList->MonsterKind);
	#endif */

	m_SMonsterListTable.Add(pList, pList->MonsterKind);
}

BOOL CGameResourceManager::LoadMonsterList()
{
	char filename[64];
	//	char value[64] = {0,};
	CMHFile file;

	sprintf(filename,"./System/Resource/MonsterList.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;

	int n =0;
	MONSTEREX_LIST * pList = NULL;
	while(1)
	{
//		if(n == MAX_MONSTER_NUM)
//			break;

		if(file.IsEOF())
			break;
		pList = new MONSTEREX_LIST;
		memset(pList, 0, sizeof(MONSTEREX_LIST));
		pList->MonsterKind				= file.GetWord();		// 번호
		SafeStrCpy( pList->Name, file.GetString(), MAX_MONSTER_NAME_LENGTH+1 );	// 이름
		file.GetString();//1		SafeStrCpy( pList->EngName, file.GetString(), MAX_NAME_LENGTH+1 );	// 영문명

		SafeStrCpy( pList->ChxName, file.GetString(), 32 );	// CHX 파일

		pList->Damage1DramaNum			= FindEffectNum(file.GetString());// 맞음 1 연출 번호	
		pList->Die1DramaNum				= FindEffectNum(file.GetString());// 죽음 1 연출 번호	
		pList->Die2DramaNum				= FindEffectNum(file.GetString());// 죽음 2 연출 번호	

		pList->SpecialType				= file.GetWord();	// 몬스터 종류
		pList->ObjectKind				= file.GetWord();
		pList->MonsterType				= file.GetWord();	// 유형분류
		pList->MonsterAttribute			= file.GetWord();	// 속성
		pList->MonsterRace				= file.GetWord();	// 대표 종족 코드

		pList->Level					= file.GetLevel();		// 몬스터레벨	
		// 080604 LYW --- GameResourceManager : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
		//pList->ExpPoint					= file.GetExpPoint();	// 경험치
		pList->ExpPoint					= file.GetDword() ;	// 경험치

		pList->Life						= file.GetDword();		// 체력
		file.GetFloat();	// 배수

		pList->Scale					= file.GetFloat();		// 크기	
		pList->Weight					= file.GetFloat();		// 무게
		pList->Gravity					= file.GetDword();		// 비중
		pList->MonsterRadius			= file.GetWord();		// 반지름 	
		pList->Tall						= file.GetWord();		// 키(이름출력 위치)

		pList->Accuracy					= file.GetWord();
		pList->Avoid					= file.GetWord();

		pList->AttackPhysicalMax		= file.GetWord();// 공격 최대물리 공격력	
		pList->AttackPhysicalMin		= ( WORD )( pList->AttackPhysicalMax * 0.8f );// 공격 최소물리 공격력	
		file.GetFloat();

		pList->AttackMagicalMax			= file.GetWord();// 공격 최대물리 공격력	
		pList->AttackMagicalMin			= ( WORD )( pList->AttackMagicalMax * 0.8f );// 공격 최소물리 공격력	
		file.GetFloat();

		pList->CriticalPercent			= file.GetWord();// 크리티컬 퍼센트
		file.GetFloat();
		pList->PhysicalDefense			= file.GetWord();// 물리 방어력	
		file.GetFloat();
		pList->MagicalDefense			= file.GetWord();// 물리 방어력	
		file.GetFloat();

		pList->WalkMove					= file.GetWord();// 몬스터걷기이동력	
		pList->RunMove					= file.GetWord();// 몬스터뛰기이동력	
		pList->RunawayMove				= pList->RunMove;// 몬스터도망이동력


		pList->StandTime				= file.GetDword();
		pList->StandRate				= file.GetByte();
		pList->WalkRate					= file.GetByte();
		pList->RunRate					= 0;
		pList->DomainRange				= file.GetDword();		// 영역 범위 : 반지름
		pList->PursuitForgiveTime		= file.GetDword();
		pList->PursuitForgiveDistance	= file.GetDword();

		pList->bForeAttack				= file.GetBool();					// 선공격
		file.GetBool();
		pList->HelpRequestCondition		= 0;//file.GetWord();
		pList->HelpRequestDistance		= 0;//file.GetWord();
		pList->HelpRequestTargetNum		= 0;//file.GetWord();

		pList->InitHelpType				= file.GetWord();
		pList->bHearing					= file.GetBool();
		pList->HearingDistance			= file.GetDword();

		pList->SearchPeriodicTime		= file.GetDword();				// 탐색 주기
		pList->TargetSelect				= file.GetWord();					// 타겟 선택 : FIND_CLOSE, FIND_FIRST
		pList->SearchRange				= file.GetWord();
		pList->SearchAngle				= file.GetByte();
		pList->TargetChange				= file.GetWord();

		pList->AttackNum				= file.GetWord();
		pList->AttackIndex				= new DWORD[5];
		pList->AttackRate				= new DWORD[5];
		pList->SkillInfo				= new cActiveSkillInfo*[5];

		for( WORD  cnt = 0; cnt < 5; cnt++ )
		{
			pList->AttackIndex[cnt]		= file.GetDword();

			if( pList->AttackIndex[cnt] )
			{
				pList->SkillInfo[cnt]		= ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo(pList->AttackIndex[0]);
			}
			else
			{
				pList->SkillInfo[cnt]		= NULL;
			}

			if( !pList->SkillInfo[cnt] )
			{
				// desc_hseos_ASSERT수정_01
				// S ASSERT수정 추가 added by hseos 2007.05.22
				// ASSERT(0);
				// E ASSERT수정 추가 added by hseos 2007.05.22
			}
		}
		for( cnt = 0; cnt < 5; cnt++ )
		{
			pList->AttackRate[cnt]		= file.GetWord();
		}

		// 070423 LYW --- GameResourceManager : Applay drop item.
		pList->dropMoney		= file.GetDword();
		pList->dwMoneyDropRate	= file.GetDword();

		for( BYTE count = 0 ; count < eDROPITEMKIND_MAX-1 ; ++count )
		{
			file.GetString();
			pList->dropItem[count].wItemIdx = file.GetDword();
			pList->dropItem[count].byCount  = file.GetByte() ;
			pList->dropItem[count].dwDropPercent = file.GetDword() ;
		}

		// 070423 LYW --- GameResourceManager : Delete not use part.
		/*
		pList->MinDropMoney				= 1000;//file.GetDword();// 최소돈 액수	
		pList->MaxDropMoney				= 500;//file.GetDword();// 최대 돈 액수	
		ASSERT(pList->MaxDropMoney >= pList->MinDropMoney);
		pList->DropRate[eMONEY]			= 10000;//file.GetWord();// 돈 확률(%)	
		pList->DropRate[eITEM1]			= 0;//file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM2]			= 0;//file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM3]			= 0;//file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eNOITEM]			= 0;//file.GetWord();// 노템 확률(%)	

#ifndef _CLIENT_
		DWORD temp = 0;
		for(int t=0;t<eDROPITEMKIND_MAX;++t)
		{
			pList->CurDropRate[t] = pList->DropRate[t];
			temp += pList->DropRate[t];
		}

		if( temp != MAX_DROP_ITEM_PERCENT )
			ASSERTMSG( 0, "drop rate error! in MonsterList" );
#endif
		pList->ItemListNum1				= 0;//file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum2				= 0;//file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum3				= 0;//file.GetWord();// 해당아이템리스트표번호	

		pList->MaxDropItemNum			= 0;//file.GetByte();// 최대아이템 갯수

		pList->fRunawayLifeRate			= 0;//file.GetFloat();
		pList->RunawayRate				= 0;//file.GetWord();
		pList->RunawayHelpRate			= 0;//file.GetWord();
		pList->RunawayDistance			= 0;//file.GetWord();	//????
		*/

		ASSERT(!m_SMonsterListTable.GetData(pList->MonsterKind));

		m_SMonsterListTable.Add(pList, pList->MonsterKind);

		++n;
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadNpcList(NPC_LIST* pNpcList)
{
	CMHFile fp;
	fp.Init("./System/Resource/NpcList.bin", "rb");
	int idx = 0;
	if(!fp.IsInited()) return FALSE;
	while(1)
	{
		if(fp.IsEOF())
			break;

		pNpcList[idx].NpcKind = fp.GetWord();
		//fp.GetString(pNpcList[idx].Name);
		SafeStrCpy(pNpcList[idx].Name, fp.GetString(), MAX_NPC_NAME_LENGTH+1 );
		pNpcList[idx].ModelNum = fp.GetWord();
		pNpcList[idx].JobKind = fp.GetWord();
		pNpcList[idx].Scale = fp.GetFloat(); // Npc 크기
		pNpcList[idx].Tall	= fp.GetWord(); // Npc 키
		pNpcList[idx].ShowJob = fp.GetBool();


		idx++;
	}

	fp.Release();

#ifdef _SERVER_RESOURCE_FIELD_
	SetNpcListLoadFlag(TRUE);
#endif

	return TRUE;
}

#ifdef _SERVER_RESOURCE_FIELD_
NPC_LIST* CGameResourceManager::GetNpcList(WORD JobKind)
{
	for(int i=0; i<MAX_NPC_NUM; ++i)
	{
		if(m_NpcList[i].NpcKind == JobKind)
			return &m_NpcList[i];
	}
	return NULL;
}
#endif


//060608 - Add by wonju
// 071214 LUJ, 코드 정리
BOOL CGameResourceManager::LoadMapChangeArea()
{
	CMHFile file;

	if( ! file.Init( "System/Resource/MapMoveList.bin", "rb" ) )
	{
		return FALSE;
	}

	for( int i = 0; ! file.IsEOF(); ++i  )
	{
		MAPCHANGE_INFO& info = m_MapChangeArea[ i ];

		info.Kind			= file.GetWord();
		SafeStrCpy( info.CurMapName, file.GetString(), sizeof( info.CurMapName ) );
		SafeStrCpy( info.ObjectName, file.GetString(), sizeof( info.ObjectName ) );

		info.CurMapNum		= file.GetWord();
		info.MoveMapNum		= file.GetWord();
		info.CurPoint.x		= file.GetFloat();
		info.CurPoint.y		= 0;
		info.CurPoint.z		= file.GetFloat();
		info.MovePoint.x	= file.GetFloat();
		info.MovePoint.y	= 0;
		info.MovePoint.z	= file.GetFloat();
		info.chx_num		= file.GetWord();
	}

	return TRUE;

}


// 071214 LUJ, 코드 정리
BOOL CGameResourceManager::LoadMapChangeArea(MAPCHANGE_INFO* pMapChangeArea)
{
	CMHFile file;

	if( ! file.Init( "System/Resource/MapMoveList.bin", "rb" ) )
	{
		return FALSE;
	}

	for( ; ! file.IsEOF(); ++pMapChangeArea  )
	{
		MAPCHANGE_INFO& info = *pMapChangeArea;

		info.Kind			= file.GetWord();
		SafeStrCpy( info.CurMapName, file.GetString(), sizeof( info.CurMapName ) );
		SafeStrCpy( info.ObjectName, file.GetString(), sizeof( info.ObjectName ) );

		info.CurMapNum		= file.GetWord();
		info.MoveMapNum		= file.GetWord();
		info.CurPoint.x		= file.GetFloat();
		info.CurPoint.y		= 0;
		info.CurPoint.z		= file.GetFloat();
		info.MovePoint.x	= file.GetFloat();
		info.MovePoint.y	= 0;
		info.MovePoint.z	= file.GetFloat();
		info.chx_num		= file.GetWord();
	}

	return TRUE;
}


// 070121 LYW --- Add function to check potal position.
WORD CGameResourceManager::CheckMapChange( VECTOR3* pVector, WORD curMapNum )
{
	int count = 0 ;

	float fDistance  ;
	
	for( count = 0 ; count < MAX_POINT_NUM ; ++count )
	{

		fDistance = CalcDistanceXZ( pVector, &( m_MapChangeArea[count].CurPoint ) ) ;

		// 070206 LYW --- Modified this line.
		//if( fDistance <= POTAL_RAD )
		if( (fDistance <= POTAL_RAD) && (m_MapChangeArea[ count ].CurMapNum == curMapNum ) )
		{
			return m_MapChangeArea[ count ].Kind ;
		}
	}

	return 65535 ;
}

BOOL CGameResourceManager::LoadLoginPoint(LOGINPOINT_INFO* pLogin)
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"System/Resource/LoginPoint.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		pLogin[n].Kind = file.GetWord();
		//file.GetString(pLogin[n].MapName);
		SafeStrCpy( pLogin[n].MapName, file.GetString(), MAX_NAME_LENGTH+1 );
		pLogin[n].MapNum = file.GetWord();
		pLogin[n].MapLoginPointNum = file.GetByte();
		if(pLogin[n].MapLoginPointNum == 0)
		{
			MessageBox(NULL,"LoginPointNum is Zero!!!",0,0);
		}
		if(pLogin[n].MapLoginPointNum > 10)
		{
			MessageBox(NULL,"LoginPointNum is too big!!!",0,0);
		}
		for(int i=0;i<pLogin[n].MapLoginPointNum;++i)
		{
			pLogin[n].CurPoint[i].x = file.GetFloat();
			pLogin[n].CurPoint[i].y = 0;
			pLogin[n].CurPoint[i].z = file.GetFloat();
		}
		pLogin[n].chx_num = file.GetWord();
		++n;
	}

	return TRUE;
}

#ifdef _MAPSERVER_
BOOL CGameResourceManager::CheckCorrectLoginPoint(VECTOR3* pos)
{
	static float maxdist = 1000;
	for( int i = 0; i < MAX_POINT_NUM; i++)
	{
		if(m_LoginPoint[i].MapNum == GetLoadMapNum())
		{
			for(int n=0;n<m_LoginPoint[i].MapLoginPointNum;++n)
			{
				float dist = CalcDistanceXZ(pos,&m_LoginPoint[i].CurPoint[n]);
				if(dist < maxdist)	//임의
					return TRUE;
			}
		}
	}
	for( i = 0; i < MAX_POINT_NUM; i++)
	{
		if(m_MapChangeArea[i].MoveMapNum == GetLoadMapNum())
		{
			float dist = CalcDistanceXZ(pos,&m_MapChangeArea[i].MovePoint);
			if(dist < maxdist)	//임의
				return TRUE;
		}
	}

	return FALSE;
}


void CGameResourceManager::GetRegenPoint(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex)
{
	if(MapchangeIndex)
	{
		MAPCHANGE_INFO* changeinfo = GetMapChangeInfo(MapchangeIndex);
		if(!changeinfo)
		{
			char temp[128];
			sprintf(temp,"MapChangePoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			if(changeinfo->MoveMapNum != GAMERESRCMNGR->GetLoadMapNum())
				MapchangeIndex = 0;
			else
			{
				pos.x = changeinfo->MovePoint.x;
				pos.y = 0;
				pos.z = changeinfo->MovePoint.z;
			}
		}
	}
	if(MapchangeIndex == 0)
	{
		LOGINPOINT_INFO* LoginPoint = GetLoginPointInfoFromMapNum(GAMERESRCMNGR->GetLoadMapNum());//LoginIndex);
		if(!LoginPoint)
		{
			char temp[128];
			sprintf(temp,"LoginPoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			int rl = rand() % LoginPoint->MapLoginPointNum;
			pos.x = LoginPoint->CurPoint[rl].x;
			pos.y = 0;
			pos.z = LoginPoint->CurPoint[rl].z;
		}
	}

	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = pos.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = pos.z + temp;
	RandPos.y = 0;

	ASSERT(!(pos.x == 0 && pos.z == 0));
}

void CGameResourceManager::GetRegenPointFromEventMap( VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex,DWORD dwTeam)
{
	if(MapchangeIndex)
	{
		MAPCHANGE_INFO* changeinfo = GetMapChangeInfo(MapchangeIndex);
		if(!changeinfo)
		{
			char temp[128];
			sprintf(temp,"MapChangePoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			if(changeinfo->MoveMapNum != GAMERESRCMNGR->GetLoadMapNum())
				MapchangeIndex = 0;
			else
			{
				pos.x = changeinfo->MovePoint.x;
				pos.y = 0;
				pos.z = changeinfo->MovePoint.z;
			}
		}
	}
	if(MapchangeIndex == 0)
	{
		LOGINPOINT_INFO* LoginPoint = GetLoginPointInfoFromMapNum(GAMERESRCMNGR->GetLoadMapNum());//LoginIndex);
		if(!LoginPoint)
		{
			char temp[128];
			sprintf(temp,"LoginPoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			int rl = dwTeam - 1;
			pos.x = LoginPoint->CurPoint[rl].x;
			pos.y = 0;
			pos.z = LoginPoint->CurPoint[rl].z;
		}
	}

	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = pos.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = pos.z + temp;
	RandPos.y = 0;

	ASSERT(!(pos.x == 0 && pos.z == 0));
}

#endif

//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
void CGameResourceManager::LoadRevivePoint()
{
	CMHFile file;
	WORD	mapnum = 0;
	float	posx = 0;
	float	posz = 0;

	if(!file.Init("./System/Resource/SafetyLoc.bin", "rb"))
		return;


	while(!file.IsEOF())
	{
		mapnum = file.GetWord();
		posx = file.GetFloat();
		posz = file.GetFloat();

		VECTOR3 vector3;

		vector3.x = posx;
		vector3.y = 0;
		vector3.z = posz;

		m_RevivePointMap[mapnum] = vector3;

/*		if( mapnum == m_LoadingMapNum )
		{
			m_RevivePoint.x = posx;
			m_RevivePoint.y = 0;
			m_RevivePoint.z = posz;

			break;
		}*/
		//m_RevivePointMap
	}

}

void CGameResourceManager::GetMonsterStats(WORD wMonsterKind, monster_stats * stats)
{
	/*	stats->AttributeAttackMin1 = GetMonsterInfo(wMonsterKind)->Attack1Property;
	stats->AttributeAttackMax1 = GetMonsterInfo(wMonsterKind)->Attack2Property;
	stats->AttributeAttackMin2 = GetMonsterInfo(wMonsterKind)->Attack1Property;
	stats->AttributeAttackMax2 = GetMonsterInfo(wMonsterKind)->Attack2Property;*/

	stats->Attack1OccurProportion = (WORD)GetMonsterListInfo(wMonsterKind)->AttackRate[0];				// 공격1 발생 확률
	stats->PhysicalAttackMin1 = GetMonsterListInfo(wMonsterKind)->AttackPhysicalMin;						// 공격1 최소 공격력
	stats->PhysicalAttackMax1 = GetMonsterListInfo(wMonsterKind)->AttackPhysicalMax;
	//	stats->PhysicalAttackMin2 = GetMonsterInfo(wMonsterKind)->Attack2PhysicalMin;						// 공격1 최소 공격력
	//	stats->PhysicalAttackMax2 = GetMonsterInfo(wMonsterKind)->Attack2PhysicalMax;
	// 방어
	stats->PhysicalDefense = GetMonsterListInfo(wMonsterKind)->PhysicalDefense;
	//	stats->AttributeResist = GetMonsterListInfo(wMonsterKind)->AttribResist;
}

BOOL CGameResourceManager::LoadNpcRegen(NPC_REGEN pNpcRegen[MAX_NPC_GROUPNUM][MAX_NPC_REGEN_NUM])
{
	char filename[64];
	char value[64] = {0,};
	CMHFile file;
	sprintf(filename,"./System/Resource/NpcRegen.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;

	int n =0;
	WORD Group = 1;
	WORD count = 1;

	while(1)
	{
		if(file.IsEOF())
			break;
		if(Group > MAX_NPC_GROUPNUM || n > MAX_NPC_REGEN_NUM) break;

		if(count > MONSTERGROUPUNIT)
		{
			count = 1;
			n = 1;
			Group++;
			m_TotalNpcGroupCount++;
		}
		pNpcRegen[Group][n].MapNum = file.GetWord();
		if(pNpcRegen[Group][n].MapNum == 0)
			break;

		pNpcRegen[Group][n].NpcKind = file.GetWord();
		//file.GetString(pNpcRegen[Group][n].Name);
		SafeStrCpy( pNpcRegen[Group][n].Name, file.GetString(), MAX_NAME_LENGTH+1 );

		pNpcRegen[Group][n].NpcIndex = (WORD)file.GetDword();
		pNpcRegen[Group][n].Pos.x = file.GetFloat();
		pNpcRegen[Group][n].Pos.y = 0;
		pNpcRegen[Group][n].Pos.z = file.GetFloat();
		pNpcRegen[Group][n].Angle = file.GetFloat();
		count++;
		n++;
		if(n > m_MaxNpcRegenCount)
			m_MaxNpcRegenCount = n;
	}
	SetNpcRegenLoadFlag(TRUE);
	return TRUE;
}


void CGameResourceManager::SetLoadMapNum(WORD Mapnum)
{
	m_LoadingMapNum = Mapnum;
}

BOOL CGameResourceManager::LoadStaticNpc()
{
	CMHFile file;
	char filename[64];
	sprintf( filename, "./System/Resource/StaticNpc.bin" );	
	if( !file.Init( filename, "rb" ) )
		return FALSE;
	STATIC_NPCINFO Info;
	while( 1 )
	{
		if( file.IsEOF() )
			break;

		Info.MapNum = file.GetWord();
		Info.wNpcJob = file.GetWord();
		//file.GetString( Info.NpcName );
		SafeStrCpy( Info.NpcName, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.wNpcUniqueIdx = file.GetWord();
		Info.vPos.x = file.GetFloat();
		Info.vPos.y = 0.0f;
		Info.vPos.z = file.GetFloat();
		file.GetFloat();

		if( m_LoadingMapNum != Info.MapNum )
			continue;

		STATIC_NPCINFO* pInfo = new STATIC_NPCINFO;
		*pInfo = Info;


		m_StaticNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );

		// 070325 LYW --- GameResourceManager : Modified check npc job.
		//if( pInfo->wNpcJob == 10 )
		if( pInfo->wNpcJob == 8 )
			m_WareNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );
	}

	file.Release();

	sprintf( filename, "./system/Resource/QuestNpcList.bin" );	
	if( !file.Init( filename, "rb" ) )
		return FALSE;

	while( 1 )
	{
		if( file.IsEOF() )
			break;

		Info.MapNum = file.GetWord();
		Info.wNpcJob = file.GetWord();
		//		file.GetString( Info.NpcName );
		SafeStrCpy( Info.NpcName, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.wNpcUniqueIdx = file.GetWord();
		Info.vPos.x = file.GetFloat();
		Info.vPos.y = 0.0f;
		Info.vPos.z = file.GetFloat();
		file.GetFloat();

		if( m_LoadingMapNum != Info.MapNum )
			continue;

		STATIC_NPCINFO* pInfo = new STATIC_NPCINFO;
		*pInfo = Info;
		m_StaticNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );
	}

	file.Release();

	return TRUE;
}


BOOL CGameResourceManager::IsInWareNpcPosition( VECTOR3* pvPos )
{
	float dist = 0.0f;
	STATIC_NPCINFO* pInfo = NULL;
	m_WareNpcTable.SetPositionHead();
	while( pInfo = m_WareNpcTable.GetData() )
	{
		dist = CalcDistanceXZ( pvPos, &pInfo->vPos );
		if( dist <= 5000.0f )	return TRUE;
	}

	return FALSE;
}


void CGameResourceManager::ReleaseStaticNpc()
{
	STATIC_NPCINFO* pData = NULL;
	m_StaticNpcTable.SetPositionHead();
	while( pData = m_StaticNpcTable.GetData() )
		delete pData;
	m_StaticNpcTable.RemoveAll();
}

STATIC_NPCINFO* CGameResourceManager::GetStaticNpcInfo( WORD wNpcUniqueIdx )
{
	return m_StaticNpcTable.GetData( wNpcUniqueIdx );	
}

//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
BOOL CGameResourceManager::LoadExpPoint()
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"System/Resource/characterExpPoint.bin");	

	// 070411 LYW --- GameResourceManager : Check binary file.
	/*
	if(!file.Init(filename, "rb"))
		return FALSE;
		*/
	BOOL bResult = FALSE ;

	bResult = file.Init(filename, "rb") ;

	if( !bResult )
	{
		MessageBox(NULL,"Loading failed!! ./System/Resource/characterExpPoint.bin",0,0);

		return FALSE ;
	}


	int idx =0;
	while(1)
	{
		if(idx > MAX_CHARACTER_LEVEL_NUM-1) break;
		if(file.IsEOF())
			break;
		file.GetWord();												// 레벨
		// 080603 LYW --- GameResourceManager : 경험치 처리 (__int32)에서 (__int64)로 변경 처리. 
		//m_CharacterExpPoint[idx] = file.GetExpPoint();				// 경험치
		m_CharacterExpPoint[idx] = file.GetDwordEx() ;
		idx++;
	}

	file.Release();
	return TRUE;	
}

BOOL CGameResourceManager::LoadFishingExpPoint()
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"System/Resource/FishingExpPoint.bin");	

	BOOL bResult = FALSE ;

	bResult = file.Init(filename, "rb") ;

	if( !bResult )
	{
		MessageBox(NULL,"Loading failed!! ./System/Resource/FishingExpPoint.bin",0,0);

		return FALSE ;
	}


	int idx =0;
	while(1)
	{
		if(idx > MAX_FISHING_LEVEL-1) break;
		if(file.IsEOF())
			break;
		file.GetWord();												// 레벨
		m_FishingExpPoint[idx] = file.GetExpPoint();				// 경험치
		idx++;
	}

	file.Release();
	return TRUE;	
}

BOOL CGameResourceManager::LoadPlayerxMonsterPoint()
{
	CMHFile file;
	int idx = 0;
	if( !file.Init( "System/Resource/PlayerxMonsterPoint.bin", "rb") )
		return FALSE;
	while(1)
	{
		if(idx > MAX_PLAYERLEVEL_NUM - 1) break;

		char value[64] = {0,};

		// MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1  1은 gap이 0일경우때문에
		for(int levelgap = 0 ; levelgap < MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1 ; levelgap++)
		{
			m_PLAYERxMONSTER_POINT[idx][levelgap] = file.GetInt();
		}
		idx++;
	}
	file.Release();

	return TRUE;
}

EXPTYPE CGameResourceManager::GetPLAYERxMONSTER_POINT(LEVELTYPE level,int levelgap)
{
	ASSERT(0 < level && level < 255);	// 255 is temp Max Level of Mon
	ASSERT(-MONSTERLEVELRESTRICT_LOWSTARTNUM <= levelgap && 
		levelgap <= MAX_MONSTERLEVELPOINTRESTRICT_NUM );

	return m_PLAYERxMONSTER_POINT[level-1][levelgap+MONSTERLEVELRESTRICT_LOWSTARTNUM];
}
//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

BOOL CGameResourceManager::LoadAvatarEquipList()
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"System/Resource/AvatarEquip.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		int ItemIdx = file.GetWord();
		if( ItemIdx < 55000 )
			continue;

		AVATARITEM* pAvatar = new AVATARITEM;
		pAvatar->Gender = file.GetByte();					// 성별
		pAvatar->Position = file.GetByte();					// 착용부위
		pAvatar->Item[eAvatar_Hat] = file.GetWord();		// 모자
		pAvatar->Item[eAvatar_Hair] = file.GetWord();		// 머리
		pAvatar->Item[eAvatar_Face] = file.GetWord();		// 얼굴
		pAvatar->Item[eAvatar_Mask] = file.GetWord();		// 가면
		pAvatar->Item[eAvatar_Glasses] = file.GetWord();	// 안경
		pAvatar->Item[eAvatar_Mustache] = file.GetWord();	// 수염
		pAvatar->Item[eAvatar_Dress] = file.GetWord();		// 옷
		pAvatar->Item[eAvatar_Shoulder] = file.GetWord();	// 어깨
		pAvatar->Item[eAvatar_Back] = file.GetWord();		// 등
		pAvatar->Item[eAvatar_Shoes] = file.GetWord();		// 신발
		pAvatar->Item[eAvatar_Effect] = file.GetWord();		// 이펙트
		pAvatar->Item[eAvatar_Hand] = file.GetWord();		// 손

		pAvatar->Item[eAvatar_Weared_Hair] = file.GetWord();		// 입는 아이템 머리
		pAvatar->Item[eAvatar_Weared_Face] = file.GetWord();		// 입는 아이템 얼굴
		pAvatar->Item[eAvatar_Weared_Hat] = file.GetWord();			// 입는 아이템 머리띠		
		pAvatar->Item[eAvatar_Weared_Dress] = file.GetWord();		// 입는 아이템 옷
		pAvatar->Item[eAvatar_Weared_Shoes] = file.GetWord();		// 입는 아이템 신발		

		pAvatar->Item[eAvatar_Weared_Gum] = file.GetWord();			// 착용무기 검
		pAvatar->Item[eAvatar_Weared_Gwun] = file.GetWord();		// 착용무기 권
		pAvatar->Item[eAvatar_Weared_Do] = file.GetWord();			// 착용무기 도
		pAvatar->Item[eAvatar_Weared_Chang] = file.GetWord();		// 착용무기 창
		pAvatar->Item[eAvatar_Weared_Gung] = file.GetWord();		// 착용무기 궁
		pAvatar->Item[eAvatar_Weared_Amgi] = file.GetWord();		// 착용무기 암기

		m_AvatarEquipTable.Add( pAvatar, ItemIdx );
	}	
	return TRUE;
}



BOOL CGameResourceManager::LoadShopItemDupList()
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"System/Resource/ItemdupOption.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		char buf[64] = { 0, };

		DWORD Index = file.GetDword();
		if( Index == 0 )			break;

		SHOPITEMDUP* pDupOption = new SHOPITEMDUP;		
		pDupOption->Index = Index;

		file.GetString( buf );
		if( strcmp( buf, "#CHARM" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Charm;
		else if( strcmp( buf, "#HERB" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Herb;
		else if( strcmp( buf, "#INCANTATION" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Incantation;
		else if( strcmp( buf, "#SUNDRIES" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Sundries;
		else
		{
			ASSERT(0);
			pDupOption->DupType = eShopItemDupType_None;
		}

		pDupOption->Param = file.GetDword();

		// 주석
		file.GetString( buf );

		m_ShopItemDupOptionTable.Add( pDupOption, pDupOption->Index );
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------------------------------//
//		클라이언트만 사용하는 부분
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
void CGameResourceManager::DeletePreLoadData()
{
	//
	sPRELOAD_INFO* pPreLoadInfo = NULL;
	m_PreLoadDataTable.SetPositionHead();
	while( pPreLoadInfo = m_PreLoadDataTable.GetData() )
	{
		delete pPreLoadInfo;		
	}
	m_PreLoadDataTable.RemoveAll();	

	//
	PRELOAD* pEff = NULL;
	PTRLISTSEARCHSTART(m_PreLoadEfflist, PRELOAD*, pEff);
	//		m_PreLoadEfflist.Remove(pEff);
	delete pEff;
	pEff = NULL;
	PTRLISTSEARCHEND
		m_PreLoadEfflist.RemoveAll();

	//
	PRELOAD* pMod = NULL;
	PTRLISTSEARCHSTART(m_PreLoadModlist, PRELOAD*, pMod);
	//		m_PreLoadModlist.Remove(pMod);
	delete pMod;
	pMod = NULL;
	PTRLISTSEARCHEND
		m_PreLoadModlist.RemoveAll();

	if(m_PreLoadItemlist)
		delete[] m_PreLoadItemlist;

	sPRELOAD_EFFECT_INFO* pPreLoadEffectInfo = NULL;
	m_PreLoadEffectTable.SetPositionHead();
	while( pPreLoadEffectInfo = m_PreLoadEffectTable.GetData() )
	{
		PRELOAD* pEffect = NULL;
		PTRLISTSEARCHSTART(pPreLoadEffectInfo->Effect, PRELOAD*, pEffect);
		delete pEffect;
		pEffect = NULL;
		PTRLISTSEARCHEND
			pPreLoadEffectInfo->Effect.RemoveAll();
		delete pPreLoadEffectInfo;		
	}
	m_PreLoadEffectTable.RemoveAll();	

}


#include ".\Engine\EngineObject.h"		// for PreLoadObject()
void CGameResourceManager::PreLoadData()
{
	//	DWORD n;
	// YH2DO 로딩에 대한 확정 필요

	/* 케릭터는 너무 많아서 막아놓음
	//////////////////////////////////////////////////////////////////////////
	// 케릭터 Preload
	DIRECTORYMGR->SetLoadMode(eLM_Character);

	for(int job=0;job<eJOB_Max;++job)
	{
	CEngineObject::PreLoadObject(m_ModFileList[GENDER_MALE][job].BaseObjectFile);
	CEngineObject::PreLoadObject(m_ModFileList[GENDER_FEMALE][job].BaseObjectFile);

	for(n=0;n<m_ModFileList[GENDER_MALE][job].MaxModFile;++n)
	CEngineObject::PreLoadObject(m_ModFileList[GENDER_MALE][job].ModFile[n]);

	for(n=0;n<m_ModFileList[GENDER_FEMALE][job].MaxModFile;++n)
	CEngineObject::PreLoadObject(m_ModFileList[GENDER_FEMALE][job].ModFile[n]);
	}

	for(n=0;n<m_HairModList[GENDER_MALE].MaxModFile;++n)
	CEngineObject::PreLoadObject(m_HairModList[GENDER_MALE].ModFile[n]);

	for(n=0;n<m_HairModList[GENDER_FEMALE].MaxModFile;++n)
	CEngineObject::PreLoadObject(m_HairModList[GENDER_FEMALE].ModFile[n]);
	//////////////////////////////////////////////////////////////////////////
	*/
	DIRECTORYMGR->SetLoadMode(eLM_Character);
//	CEngineObject::PreLoadObject("Man.chx");
//	CEngineObject::PreLoadObject("Woman.chx");

//imsi	//chx 파일에 무엇무엇 들어있나...?  처음 한번만 로딩해 놓아도 되는건가? KES 070805
	CEngineObject::PreLoadObject("H_Man.chx");
	CEngineObject::PreLoadObject("H_Woman.chx");
	CEngineObject::PreLoadObject("E_Man.chx");
	CEngineObject::PreLoadObject("E_Woman.chx");


	/*
	//////////////////////////////////////////////////////////////////////////
	// Npc
	DIRECTORYMGR->SetLoadMode(eLM_Npc);

	for(n=0;n<m_MaxNpcChxList;++n)
	CEngineObject::PreLoadObject(m_NpcChxList[n].FileName);
	//////////////////////////////////////////////////////////////////////////
	*/

	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

BOOL CGameResourceManager::LoadModFileList(MOD_LIST pModList[RaceType_Max][GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/ModList_H_M.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	// BaseObjectFile
	pModList[RaceType_Human][GENDER_MALE].BaseObjectFile = file.GetString();	

	pModList[RaceType_Human][GENDER_MALE].MaxModFile = file.GetDword();
	pModList[RaceType_Human][GENDER_MALE].ModFile = new StaticString[pModList[RaceType_Human][GENDER_MALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[RaceType_Human][GENDER_MALE].MaxModFile) break;
		pModList[RaceType_Human][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx ;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/ModList_H_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	// BaseObjectFile
	pModList[RaceType_Human][GENDER_FEMALE].BaseObjectFile = file.GetString();

	pModList[RaceType_Human][GENDER_FEMALE].MaxModFile = file.GetDword();
	pModList[RaceType_Human][GENDER_FEMALE].ModFile = new StaticString[pModList[RaceType_Human][GENDER_FEMALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[RaceType_Human][GENDER_FEMALE].MaxModFile) break;
		pModList[RaceType_Human][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/ModList_E_M.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	// BaseObjectFile
	pModList[RaceType_Elf][GENDER_MALE].BaseObjectFile = file.GetString();	

	pModList[RaceType_Elf][GENDER_MALE].MaxModFile = file.GetDword();
	pModList[RaceType_Elf][GENDER_MALE].ModFile = new StaticString[pModList[RaceType_Elf][GENDER_MALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[RaceType_Elf][GENDER_MALE].MaxModFile) break;
		pModList[RaceType_Elf][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/ModList_E_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	// BaseObjectFile
	pModList[RaceType_Elf][GENDER_FEMALE].BaseObjectFile = file.GetString();

	pModList[RaceType_Elf][GENDER_FEMALE].MaxModFile = file.GetDword();
	pModList[RaceType_Elf][GENDER_FEMALE].ModFile = new StaticString[pModList[RaceType_Elf][GENDER_FEMALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[RaceType_Elf][GENDER_FEMALE].MaxModFile) break;
		pModList[RaceType_Elf][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();
	return TRUE;
}
BOOL CGameResourceManager::LoadFaceModFileList(MOD_LIST pFaceModList[RaceType_Max][GENDER_MAX]) //pjscode
{
	//얼굴 관련
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/FaceList_H_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;

	pFaceModList[RaceType_Human][GENDER_MALE].MaxModFile = file.GetDword();
	pFaceModList[RaceType_Human][GENDER_MALE].ModFile = new StaticString[pFaceModList[RaceType_Human][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pFaceModList[RaceType_Human][GENDER_MALE].MaxModFile) break;
		pFaceModList[RaceType_Human][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/FaceList_H_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	pFaceModList[RaceType_Human][GENDER_FEMALE].MaxModFile = file.GetDword();
	pFaceModList[RaceType_Human][GENDER_FEMALE].ModFile = new StaticString[pFaceModList[RaceType_Human][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pFaceModList[RaceType_Human][GENDER_FEMALE].MaxModFile) break;
		pFaceModList[RaceType_Human][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/FaceList_E_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;

	pFaceModList[RaceType_Elf][GENDER_MALE].MaxModFile = file.GetDword();
	pFaceModList[RaceType_Elf][GENDER_MALE].ModFile = new StaticString[pFaceModList[RaceType_Elf][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pFaceModList[RaceType_Elf][GENDER_MALE].MaxModFile) break;
		pFaceModList[RaceType_Elf][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/FaceList_E_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	pFaceModList[RaceType_Elf][GENDER_FEMALE].MaxModFile = file.GetDword();
	pFaceModList[RaceType_Elf][GENDER_FEMALE].ModFile = new StaticString[pFaceModList[RaceType_Elf][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pFaceModList[RaceType_Elf][GENDER_FEMALE].MaxModFile) break;
		pFaceModList[RaceType_Elf][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}

BOOL CGameResourceManager::LoadBodyModFileList(MOD_LIST pBodyModList[RaceType_Max][GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/BodyList_H_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;
	pBodyModList[RaceType_Human][GENDER_MALE].MaxModFile = file.GetDword();
	pBodyModList[RaceType_Human][GENDER_MALE].ModFile = new StaticString[pBodyModList[RaceType_Human][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pBodyModList[RaceType_Human][GENDER_MALE].MaxModFile) break;
		pBodyModList[RaceType_Human][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/BodyList_H_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	pBodyModList[RaceType_Human][GENDER_FEMALE].MaxModFile = file.GetDword();
	pBodyModList[RaceType_Human][GENDER_FEMALE].ModFile = new StaticString[pBodyModList[RaceType_Human][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pBodyModList[RaceType_Human][GENDER_FEMALE].MaxModFile) break;
		pBodyModList[RaceType_Human][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/BodyList_E_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;
	pBodyModList[RaceType_Elf][GENDER_MALE].MaxModFile = file.GetDword();
	pBodyModList[RaceType_Elf][GENDER_MALE].ModFile = new StaticString[pBodyModList[RaceType_Elf][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pBodyModList[RaceType_Elf][GENDER_MALE].MaxModFile) break;
		pBodyModList[RaceType_Elf][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/BodyList_E_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;

	pBodyModList[RaceType_Elf][GENDER_FEMALE].MaxModFile = file.GetDword();
	pBodyModList[RaceType_Elf][GENDER_FEMALE].ModFile = new StaticString[pBodyModList[RaceType_Elf][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pBodyModList[RaceType_Elf][GENDER_FEMALE].MaxModFile) break;
		pBodyModList[RaceType_Elf][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}


BOOL CGameResourceManager::LoadHairModFileList(MOD_LIST pHairModList[RaceType_Max][GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/HairList_H_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;
	pHairModList[RaceType_Human][GENDER_MALE].MaxModFile = file.GetDword();
	pHairModList[RaceType_Human][GENDER_MALE].ModFile = new StaticString[pHairModList[RaceType_Human][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pHairModList[RaceType_Human][GENDER_MALE].MaxModFile) break;
		pHairModList[RaceType_Human][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/HairList_H_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
	pHairModList[RaceType_Human][GENDER_FEMALE].MaxModFile = file.GetDword();
	pHairModList[RaceType_Human][GENDER_FEMALE].ModFile = new StaticString[pHairModList[RaceType_Human][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pHairModList[RaceType_Human][GENDER_FEMALE].MaxModFile) break;
		pHairModList[RaceType_Human][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
	sprintf(filename,"System/Resource/HairList_E_M.bin");		
	if(!file.Init(filename,"rb"))
		return FALSE;
	pHairModList[RaceType_Elf][GENDER_MALE].MaxModFile = file.GetDword();
	pHairModList[RaceType_Elf][GENDER_MALE].ModFile = new StaticString[pHairModList[RaceType_Elf][GENDER_MALE].MaxModFile];

	while(1)
	{
		if(idx >= pHairModList[RaceType_Elf][GENDER_MALE].MaxModFile) break;
		pHairModList[RaceType_Elf][GENDER_MALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();


	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
	sprintf(filename,"System/Resource/HairList_E_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
	pHairModList[RaceType_Elf][GENDER_FEMALE].MaxModFile = file.GetDword();
	pHairModList[RaceType_Elf][GENDER_FEMALE].ModFile = new StaticString[pHairModList[RaceType_Elf][GENDER_FEMALE].MaxModFile];


	while(1)
	{
		if(idx >= pHairModList[RaceType_Elf][GENDER_FEMALE].MaxModFile) break;
		pHairModList[RaceType_Elf][GENDER_FEMALE].ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}

BOOL CGameResourceManager::LoadPetModList()
{
	CMHFile file;
	if(!file.Init("System/Resource/Pet_Costume.bin","rb"))
		return FALSE;

	int idx = 0;

	mPetModList.MaxModFile = file.GetDword();
	mPetModList.ModFile = new StaticString[ mPetModList.MaxModFile ];

	while(1)
	{
		if(idx >= mPetModList.MaxModFile) break;
		mPetModList.ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}

BOOL CGameResourceManager::LoadNpcChxList()
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	idx = 0;
	sprintf(filename,"System/Resource/NpcChxList.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
	m_MaxNpcChxList = file.GetDword();
	m_NpcChxList = new CHXLIST[m_MaxNpcChxList];

	while(1)
	{
		if(idx >= m_MaxNpcChxList) break;
		m_NpcChxList[idx].FileName = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}

char* CGameResourceManager::GetNpcChxName(DWORD index)
{
	ASSERT(index > 0 && index < m_MaxNpcChxList);
	return m_NpcChxList[index].FileName;
}

BOOL CGameResourceManager::LoadGameDesc()
{
	char filename[64];
	char value[64] = {0,};
	CMHFile file;
	sprintf(filename,"system/GameDesc.bin");	

	if(!file.Init(filename, "rb"))
		return FALSE;

	while(! file.IsEOF() )
	{
		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*DISPWIDTH") == 0)
		{
			m_GameDesc.dispInfo.dwWidth = file.GetDword();
		}
		else if(strcmp(value,"*DISPHEIGHT") == 0)
		{
			m_GameDesc.dispInfo.dwHeight = file.GetDword();
		}
		else if(strcmp(value,"*BPS") == 0)
		{
			m_GameDesc.dispInfo.dwBPS = file.GetDword();
		}
		else if(strcmp(value,"*WINDOWMODE") == 0)
		{
			m_GameDesc.dispInfo.dispType = file.GetBool() ? WINDOW_WITH_BLT : FULLSCREEN_WITH_BLT;
		}
		else if(strcmp(value,"*SHADOW") == 0)
		{
			m_GameDesc.bShadow = file.GetByte();
		}
		else if(strcmp(value,"*MAXSHADOWLIGHT") == 0)
		{
			m_GameDesc.MaxShadowNum = file.GetDword();
		}
		else if(strcmp(value,"*SHADOWDETAIL") == 0)
		{
			m_GameDesc.MaxShadowTexDetail = file.GetDword();
		}
		else if(strcmp(value,"*DISTRIBUTESERVERIP") == 0)
		{
			file.GetString(m_GameDesc.DistributeServerIP);
		}
		else if(strcmp(value,"*DISTRIBUTESERVERPORT") == 0)
		{
			m_GameDesc.DistributeServerPort = file.GetWord();
		}
		else if(strcmp(value,"*AGENTSERVERPORT") == 0)
		{
			m_GameDesc.AgentServerPort = file.GetWord();
		}
		else if(strcmp(value,"*MOVEPOINT") == 0)
		{
			file.GetString(m_GameDesc.MovePoint);
		}
		else if(strcmp(value,"*FPS") == 0)
		{
			FILE* file = fopen( "system\\launcher.sav", "r" );

			if( file )
			{
				char buffer[ 100 ];																
				const char* separator = " =\n";													
				while( fgets( buffer, sizeof( buffer ), file ) )
					{
						const char* token = strtok( buffer, separator );							

						if( 0 == token || ';' == token[ 0 ] )										
						{
							continue;																
						}
						else if( ! strcmpi( "FPS", token ) )									
						{
							token = strtok( NULL, separator );										// 토큰을 받는다.
							if( ! strcmpi("30", token))
							{
								m_GameDesc.FramePerSec = 30;
							}
							else if( ! strcmpi("60", token))
							{
								m_GameDesc.FramePerSec = 60;
							}
							else
							{
								m_GameDesc.FramePerSec = 30;
							}
						}
						else if( ! strcmpi( "", token ) )
						{
							m_GameDesc.FramePerSec = 30;
						}
					}
			}
			//m_GameDesc.FramePerSec = file.GetDword();
			m_GameDesc.TickPerFrame = 1000/(float)m_GameDesc.FramePerSec;
		}
		else if(strcmp(value,"*CAMERAANGLEX") == 0)
		{
			m_GameDesc.CameraMinAngleX = file.GetFloat();
			m_GameDesc.CameraMaxAngleX = file.GetFloat();
		}
		else if(strcmp(value,"*CAMERADISTANCE") == 0)
		{
			m_GameDesc.CameraMinDistance = file.GetFloat();
			m_GameDesc.CameraMaxDistance = file.GetFloat();
		}
		else if(strcmp(value,"*GRAVITY") == 0)
		{
			gAntiGravity = 1/file.GetFloat();
		}		

		// Volume
		else if(strcmp(value,"*MASTERVOLUME") == 0)
		{
			m_GameDesc.MasterVolume = file.GetFloat();
		}
		else if(strcmp(value,"*SOUNDVOLUME") == 0)
		{
			m_GameDesc.SoundVolume = file.GetFloat();
		}
		else if(strcmp(value,"*BGMVOLUME") == 0)
		{
			m_GameDesc.BGMVolume = file.GetFloat();
		}
		//limit
		else if(strcmp(value,"*LIMITDAY") == 0)
		{
			m_GameDesc.LimitDay = file.GetDword();
		}
		else if(strcmp(value,"*LIMITID") == 0)
		{
			file.GetString(m_GameDesc.LimitID);
		}
		else if(strcmp(value,"*LIMITPWD") == 0)
		{
			file.GetString(m_GameDesc.LimitPWD);
		}
		else if(strcmp(value,"*WINDOWTITLE") == 0)
		{
			file.GetString(m_GameDesc.strWindowTitle);
		}
	}

	// 웅주, 070530 해상도, 창모드는 이 파일에서 읽는다(bin 파일에 쓸 방법이 현재는 없기 때문이다)
	{
		FILE* fp = fopen("system\\launcher.sav","r");

		if( fp )
		{
			TCHAR buffer[MAX_PATH] = {0};
			LPCTSTR separator = " =\r\n";

			while( fgets( buffer, sizeof( buffer ), fp ) )
			{
				const char* token = strtok( buffer, separator );

				if( 0 == token ||	// empty line
					';' == token[ 0 ] )	// comment
				{
					continue;
				}
				else if( ! strcmpi( "resolution",  token ) )
				{
					token = strtok( 0, separator );
					LPCTSTR token2 = strtok(0, separator);

					switch( atoi( token ) )
					{
					case 0:
						{
							m_GameDesc.dispInfo.dwWidth = 800;
							m_GameDesc.dispInfo.dwHeight = 600;
							break;
						}
					case 1:
						{
							m_GameDesc.dispInfo.dwWidth = 1024;
							m_GameDesc.dispInfo.dwHeight = 768;
							break;
						}
					case 2:
						{
							m_GameDesc.dispInfo.dwWidth = 1280;
							m_GameDesc.dispInfo.dwHeight = 1024;
							break;
						}
					default:
						{
							m_GameDesc.dispInfo.dwWidth = _ttoi(token);
							m_GameDesc.dispInfo.dwHeight = _ttoi(token2 ? token2 : "");
							break;
						}
					}
				}
				else if( ! strcmpi( "windowMode", token ) )
				{
					token = strtok( NULL, separator );

					m_GameDesc.dispInfo.dispType = ( ! strcmpi( "true", token ) ? WINDOW_WITH_BLT : FULLSCREEN_WITH_BLT );
				}
			}

			fclose( fp );
		}
	}

	gTickPerFrame = m_GameDesc.TickPerFrame;
	AUDIOMGR->SetMasterVolume(m_GameDesc.MasterVolume);
	AUDIOMGR->SetSoundVolume(m_GameDesc.SoundVolume);
	AUDIOMGR->SetBGMVolume(m_GameDesc.BGMVolume);

	return TRUE;
}

BOOL CGameResourceManager::LoadServerList()
{
	m_nMaxServerList = 0;

	char filename[64];
	char value[64] = {0,};
	int index = 0;
	CMHFile file;

	sprintf( filename, "system/ServerList.bin" );
	if( !file.Init( filename, "rb" ) )
		return FALSE;

	while( 1 )
	{
		if( file.IsEOF() )
			break;

		strcpy( value, strupr(file.GetString()) );

		if( strcmp( value,"*USERSERVER" ) == 0 )
		{
			++index;
			if( index > 0 )
			{
				m_ServerList[index-1].bEnter = file.GetBool();
				file.GetString( m_ServerList[index-1].DistributeIP );
				m_ServerList[index-1].DistributePort = file.GetWord();
				file.GetStringInQuotation( m_ServerList[index-1].ServerName );
				m_ServerList[index-1].ServerNo = file.GetWord();
				++m_nMaxServerList;
			}
		}

		else if( strcmp( value,"*TESTSERVER" ) == 0 && g_bTESTSERVER)
		{
			++index;
			if( index > 0 )
			{
				m_ServerList[index-1].bEnter = file.GetBool();
				file.GetString( m_ServerList[index-1].DistributeIP );
				m_ServerList[index-1].DistributePort = file.GetWord();
				file.GetStringInQuotation( m_ServerList[index-1].ServerName );
				m_ServerList[index-1].ServerNo = file.GetWord();
				++m_nMaxServerList;
			}
		}
		else if( strcmp( value, "*CHANNELNUM" ) == 0 )
		{
			int nSet = file.GetInt();
			int nCount = file.GetInt();
			if( nSet > 0 && nCount > 0 )
				CHARSELECT->SetMaxChannel( nSet, nCount );				
		}
	}

	file.Release();

	return TRUE;
}


char* CGameResourceManager::GetServerSetName()
{
	for(int i=0; i<m_nMaxServerList; ++i)
	{
		if( g_nServerSetNum == m_ServerList[i].ServerNo )
			return m_ServerList[i].ServerName;
	}
	return NULL;
}



#include "ItemManager.h"			// for LoadPreDataTable()
void CGameResourceManager::LoadPreDataTable()
{
	CMHFile file;
	if(!file.Init("./System/Resource/PreLoadData.bin", "rb"))
		return;
	char Token[64];	

	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token, "#EFFECT") == 0)
		{
			PRELOAD* pEff = NULL;
			pEff = new PRELOAD;

			char buf[128];

			file.GetString(buf);
			pEff->FileName = buf;

			m_PreLoadEfflist.AddTail(pEff);
		}
		else if(strcmp(Token, "#MOD") == 0)
		{
			PRELOAD* pEff = NULL;
			pEff = new PRELOAD;

			char buf[128];

			file.GetString(buf);
			pEff->FileName = buf;

			m_PreLoadModlist.AddTail(pEff);
		}
		else if(strcmp(Token, "#MAPNUM") == 0)
		{
			sPRELOAD_INFO* pPreLoadInfo = new sPRELOAD_INFO;
			pPreLoadInfo->MapNum = file.GetInt();

			// 해당맵의 몬스터 정보 로딩
			LoadPreMonsterData(pPreLoadInfo, pPreLoadInfo->MapNum);

			int Count = file.GetInt();
			int* Level = NULL;

			ITEM_INFO* pItemInfo = NULL;

			/*
			for(int i=0; i<Count; i++)
			{
			Level = file.GetInt();
			// 해당 레벨 아이템 로딩
			ITEMMGR->SetPreItemData(pPreLoadInfo, Level);
			}
			*/


			// Item종류가 워낙 많아서.. 차라리 이게 낫지..
			Level = new int[Count];

			for(int i=0; i<Count; i++)
				Level[i] = file.GetInt();

			ITEMMGR->SetPreItemData(pPreLoadInfo, Level, Count);

			delete Level;

			m_PreLoadDataTable.Add(pPreLoadInfo, pPreLoadInfo->MapNum);
		}
		///////////////////////////////////////////////////////////////////////////////
		// 06. 04. PreLoadData 추가기능 - 이영준
		// 아이템 사전 로드기능 추가
		// #ITEM [수량] [아이템 인덱스]...
		else if(strcmp(Token, "#ITEM") == 0)
		{
			// 있으면 지워준다
			if(m_PreLoadItemlist)
				delete[] m_PreLoadItemlist;

			// 수량을 가져온다
			int Count = file.GetInt();

			if(Count > 0)
			{
				// 버퍼 생성
				m_PreLoadItemlist = new WORD[Count + 1];
				// 첫번째 자리에 수량을 넣어준다
				m_PreLoadItemlist[0] = Count;

				WORD ItemIdx = 0;

				for(int i = 1; i < Count + 1; i++)
				{		
					ItemIdx = file.GetWord();
					m_PreLoadItemlist[i] = ItemIdx;
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		// 06. 05. PreLoadData 추가기능 - 이영준
		// 맵별 이펙트 사전 로드기능 추가
		// #MAPEFFECT [수량] [파일이름]...
		else if(strcmp(Token, "#MAPEFFECT") == 0)
		{
			sPRELOAD_EFFECT_INFO* pPreLoadInfo = new sPRELOAD_EFFECT_INFO;

			pPreLoadInfo->MapNum = file.GetInt();
			pPreLoadInfo->Count = file.GetInt();

			for(int i = 0; i < pPreLoadInfo->Count; i++)
			{
				PRELOAD* pEff = NULL;
				pEff = new PRELOAD;

				char buf[128];

				file.GetString(buf);
				pEff->FileName = buf;

				pPreLoadInfo->Effect.AddTail(pEff);
			}

			m_PreLoadEffectTable.Add(pPreLoadInfo, pPreLoadInfo->MapNum);
		}
		///////////////////////////////////////////////////////////////////////////////

	}
}


void CGameResourceManager::LoadPreMonsterData(sPRELOAD_INFO* pPreLoadInfo, MAPTYPE MapNum)
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"System/Resource/Monster_%02d.bin",MapNum);
	if(!file.Init(filename,"rb"))
		return ;

	char buff[256];
	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}
		file.GetString(buff);
		strcpy(buff, _strupr(buff));

		CMD_ST(buff)			
			CMD_CS("#ADD")
			file.GetByte();
		file.GetDword();
		int kind = file.GetDword();
		if(kind)
		{
			// 같은게 있는지 찾고
			for(int i=0; i<pPreLoadInfo->Count[ePreLoad_Monster]; i++)
				if(kind == pPreLoadInfo->Kind[ePreLoad_Monster][i])		break;

			// 없으면 새로운 몬스터 추가
			if(i == pPreLoadInfo->Count[ePreLoad_Monster])
			{
				pPreLoadInfo->Kind[ePreLoad_Monster][i] = kind;
				++pPreLoadInfo->Count[ePreLoad_Monster];

				//
				if(pPreLoadInfo->Count[ePreLoad_Monster] >= MAX_KIND_PERMAP)
					break;
			}
		}
		CMD_EN
	}
}


#ifdef _TESTCLIENT_
BOOL CGameResourceManager::LoadTestClientInfo()
{
	char filename[64];
	char value[64] = {0,};
	sprintf(filename,"../DeveloperWorking/TestClient.ini");
	CMHFile file;
	if(!file.Init(filename, "rt"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*STARTPOSITION") == 0)
		{
			m_TestClientInfo.x = file.GetFloat();
			m_TestClientInfo.z = file.GetFloat();
		}		
		else if(strcmp(value,"*BOSSMONSTERKIND") == 0)
		{
			m_TestClientInfo.BossMonsterKind = file.GetWord();

		}
		else if(strcmp(value,"*BOSSMONSTERNUM") == 0)
		{
			m_TestClientInfo.BossMonsterNum = file.GetWord();
		}
		else if(strcmp(value,"*BOSSMONSTERFILE") == 0)
		{
			file.GetString(m_TestClientInfo.BossInfofile);
		}		
		else if(strcmp(value,"*EFFECT") == 0)
		{
			m_TestClientInfo.Effect = FindEffectNum(file.GetString());
		}
		else if(strcmp(value,"*SKILL") == 0)
		{
			m_TestClientInfo.SkillIdx = file.GetWord();
		}
		else if(strcmp(value,"*WEAPON") == 0)
		{
			m_TestClientInfo.WeaponIdx = file.GetWord();
		}
		else if(strcmp(value,"*SHOES") == 0)
		{
			m_TestClientInfo.ShoesIdx = file.GetWord();		
		}
		else if(strcmp(value,"*HAT") == 0)
		{
			m_TestClientInfo.HatIdx = file.GetWord();		
		}
		else if(strcmp(value,"*DRESS") == 0)
		{
			m_TestClientInfo.DressIdx = file.GetWord();		
		}
		else if(strcmp(value,"*SHIELD") == 0)
		{
			m_TestClientInfo.ShieldIdx = file.GetWord();		
		}
		else if(strcmp(value,"*GENDER") == 0)
		{
			m_TestClientInfo.Gender = file.GetByte();
		}
		else if(strcmp(value,"*RACE") == 0)
		{
			m_TestClientInfo.Race = file.GetByte();
		}
		else if(strcmp(value,"*JOB") == 0)
		{
			m_TestClientInfo.Job = file.GetByte();
		}

		else if(strcmp(value,"*MONSTERNUM") == 0)
		{
			m_TestClientInfo.MonsterNum = file.GetWord();
		}
		else if(strcmp(value,"*MONSTEREFFECT") == 0)
		{
			m_TestClientInfo.MonsterEffect = FindEffectNum(file.GetString());
		}
		else if(strcmp(value,"*MONSTERKIND") == 0)
		{
			m_TestClientInfo.MonsterKind = file.GetWord();
		}		
		else if(strcmp(value,"*MAP") == 0)
		{
			m_TestClientInfo.Map = file.GetByte();
		}
		else if(strcmp(value,"*CHARACTERNUM") == 0)
		{
			m_TestClientInfo.CharacterNum = file.GetWord();
		}
		else if(strcmp(value,"*HAIR") == 0)
		{
			m_TestClientInfo.HairType = file.GetByte();
		}
		else if(strcmp(value,"*FACE") == 0)
		{
			m_TestClientInfo.FaceType = file.GetByte();
		}
		else if(strcmp(value,"*LIGHTEFFECT") == 0)
		{
			m_TestClientInfo.LightEffect = file.GetBool();
		}
		//trustpak
		else if(strcmp(value, "*TEST_CHARACTER_NUM") == 0)
		{
			m_TestClientInfo.dwTestCharacterNo = file.GetDword();
		}
		else if(strcmp(value, "*TEST_CHARACTER_INFO") == 0)
		{
			static int s_iTestCharacterCount = 0;

			if (MAX_TEST_CHARACTER_NO > s_iTestCharacterCount)
			{
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].byGender		= file.GetByte();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fX			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fY			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fZ			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fAngle		= DEGTORAD( file.GetFloat() );

				++s_iTestCharacterCount;
			}			
		}
		//
	}

	file.Release();



	return TRUE;
}
#endif

//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

// 06. 05 HIDE NPC - 이영준
#ifdef _SERVER_RESOURCE_FIELD_
BOOL CGameResourceManager::LoadHideNpcList()
{
	CMHFile file;
	if(!file.Init("System/Resource/HideNpcList.bin","rb"))
		return FALSE;

	WORD mapnum = GetLoadMapNum();
	WORD index;

	while(!file.IsEOF())
	{
		// 현재 맵에 해당되는 NPC만 읽어들인다
		if(file.GetWord() == mapnum)
		{
			index = file.GetWord();

			HIDE_NPC_INFO* info = new HIDE_NPC_INFO;

			memset(info, 0, sizeof(HIDE_NPC_INFO));

			info->UniqueIdx = index;

			m_HideNpcTable.Add(info, index);
		}
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadSkillMoney()
{
	CMHFile file;
	if(!file.Init("./System/Resource/SkillMoney.bin","rb"))
		return FALSE;

	while( !file.IsEOF() )
	{
		DWORD dwSkillIdx	= file.GetDword();
		if( dwSkillIdx == 0 )
		{
			g_Console.LOG( 4, "LoadSkillMoney() Error!!" );
			break;
		}

		DWORD dwSP			= file.GetDword();
		DWORD dwMoney		= file.GetDword();

        SKILL_MONEY* pSkillMoney = new SKILL_MONEY;
		pSkillMoney->SkillIdx	= dwSkillIdx;
		pSkillMoney->SP			= dwSP;
		pSkillMoney->Money		= dwMoney;

		m_htSkillMoney.Add( pSkillMoney, dwSkillIdx );
	}

	return TRUE;
}


DWORD CGameResourceManager::GetSkillMoney( DWORD SkillIdx )
{
	SKILL_MONEY* pSkillMoney = m_htSkillMoney.GetData( SkillIdx );

	if( pSkillMoney )
	{
		return pSkillMoney->Money;
	}
	else
	{
		g_Console.LOG( 4, "GetSkillMoney( SkillIdx : %d ) Error!!", SkillIdx );
		return 0;
	}
}

void CGameResourceManager::LoadPreMemoryMapList()
{
	CMHFile file;
	if(!file.Init("./System/Resource/MemorizableMap.bin", "rb"))
		return;

	while( !file.IsEOF() )
	{
		MAPTYPE	mapType	=	file.GetWord();

		m_PreLoadMemoryMapList.Add(&mapType, mapType);
	}
}

BOOL CGameResourceManager::IsMemorizableMap(MAPTYPE mapType)
{
	if(m_PreLoadMemoryMapList.GetData(mapType) != NULL )
		return TRUE;

	return FALSE;
}

#endif

#endif
