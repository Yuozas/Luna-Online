#include "stdafx.h"

#include <map>

#include "MHFile.h"
using namespace std;

BOOL g_bAssertMsgBox = FALSE;
char _g_AssertErrorMessage[255];

#ifndef _MURIMNET_


#include "CommonGameFunc.h"
#ifdef _CLIENT_
#include "../ItemManager.h"
#include "../mhMap.h"
#include "../ChatManager.h"
#endif

#ifdef __MAPSERVER__
#ifndef _MURIMNET_
#include "itemManager.h"
#endif
#include "GameResourceManager.h"

#endif

// 무공인덱스를 넘기면 무공의 무기유형을 리턴
// enum WP_KIND{GUM=0,GWUN,DO,CHANG,GUNG,AMGI,WP_MAX, WP_ERR};
/*
#define ATTACK_SKILL_INTERVAL		200
#define DEFENSE_SKILL_INTERVAL		100
#define ASSIST_SKILL_INTERVAL		100
*/
DWORD DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
DWORD DayOfMonth_Yundal[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char Days[7][16] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
static char WeatherState[][16] = { "CLEAR", "SNOW" };

#ifdef _CLIENT_

void AddComma( char* pBuf )
{
	if( *pBuf == 0 ) return;

	char buf[64];
//	strcpy( buf, pBuf );	//buf = 원본
	SafeStrCpy( buf, pBuf, 64 );
	char* p = buf;

	*(pBuf++) = *(p++);

	int n = strlen( pBuf ) % 3;

	while( *p )
	{
		if( n != 0 )
		{
			*pBuf = *p;
			++p;
		}
		else
		{
			*pBuf = ',';
		}

		++pBuf;
		n = ( n == 0 ) ? 3 : n - 1;
	}
	
	*pBuf = 0;
}

char* AddComma( DWORD dwMoney )
{
	static char buf[32];

	//unsigned long long llMoney;
	//wsprintf( buf, "%I64d", llMoney);
	//if( llMoney > MAX_INVENTORY_MONEY )
	//	dwMoney = MAX_INVENTORY_MONEY;
	//else
	//	dwMoney = (DWORD)llMoney;

	wsprintf( buf, "%u", dwMoney );
	AddComma( buf );
	return buf;
}

char* RemoveComma( char* str )
{
	static char buf[32];
	char* p = buf;

	while( *str )
	{
		if( *str != ',' )
		{
			*p = *str;
			++p;
		}

		++str;
	}

	*p = 0;

	return buf;
}


char* GetMapName(MAPTYPE mapnum)
{
	ASSERT(mapnum != 0);

	static char mapname[256];
#define MAPNAMEDEF(num,Name)	case num:	strcpy(mapname, Name );	break;

	// RaMa - 44번은 이미 있는데 수련장에서 사용.. 여기에서 처리한후 리턴해버리자.
	if( mapnum == 44 )
	{
		strcpy( mapname, CHATMGR->GetChatMsg(922) );
		return mapname;
	}

	switch(mapnum)
	{
	MAPNAMEDEF(2,	CHATMGR->GetChatMsg(1067));
	MAPNAMEDEF(13,	CHATMGR->GetChatMsg(142));
	MAPNAMEDEF(14,	CHATMGR->GetChatMsg(143));
	MAPNAMEDEF(15,	CHATMGR->GetChatMsg(141));
	MAPNAMEDEF(16,	CHATMGR->GetChatMsg(1226));
	MAPNAMEDEF(17,	CHATMGR->GetChatMsg(1227));
	MAPNAMEDEF(19,	CHATMGR->GetChatMsg(137));
	MAPNAMEDEF(20,	CHATMGR->GetChatMsg(138));
	MAPNAMEDEF(21,	CHATMGR->GetChatMsg(139));
	MAPNAMEDEF(22,	CHATMGR->GetChatMsg(145));
	MAPNAMEDEF(23,	CHATMGR->GetChatMsg(144));
	MAPNAMEDEF(25,	CHATMGR->GetChatMsg(146));
	MAPNAMEDEF(26,	CHATMGR->GetChatMsg(147));
	MAPNAMEDEF(27,	CHATMGR->GetChatMsg(140));
	MAPNAMEDEF(28,	CHATMGR->GetChatMsg(150));
	MAPNAMEDEF(29,	CHATMGR->GetChatMsg(1260));
	MAPNAMEDEF(31,	CHATMGR->GetChatMsg(148));
	MAPNAMEDEF(32,	CHATMGR->GetChatMsg(1036));
	MAPNAMEDEF(33,	CHATMGR->GetChatMsg(1032));
	MAPNAMEDEF(34,	CHATMGR->GetChatMsg(1033));
	MAPNAMEDEF(41,	CHATMGR->GetChatMsg(1251));
	MAPNAMEDEF(42,	CHATMGR->GetChatMsg(1328));
	MAPNAMEDEF(51,	CHATMGR->GetChatMsg(1426));
	MAPNAMEDEF(52,	CHATMGR->GetChatMsg(1569));
	MAPNAMEDEF(53,	CHATMGR->GetChatMsg(1572));
	// 080714 LUJ, "기사단의 묘지"맵 추가
	MAPNAMEDEF(54,	CHATMGR->GetChatMsg(1596));
	MAPNAMEDEF(55,	CHATMGR->GetChatMsg(1570));
	MAPNAMEDEF(91,	CHATMGR->GetChatMsg(1661));
	MAPNAMEDEF(92,	CHATMGR->GetChatMsg(1662));
	MAPNAMEDEF(93,	CHATMGR->GetChatMsg(1663));
	default: mapname[0] = 0; break;
	}

	return mapname;
}

WORD GetMapNumForName( char* pStrName )
{
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1067) ) == 0 )	return Alker_Farm;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(142) ) == 0 )		return Moon_Blind_Swamp;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(143) ) == 0 )		return Red_Orc_Outpost;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(141) ) == 0 )		return Moon_Blind_Forest;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1226) ) == 0 )	return Haunted_Mine_1F;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1227) ) == 0 )	return Haunted_Mine_2F;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(137) ) == 0 )		return The_Gate_Of_Alker;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(138) ) == 0 )		return Alker_Harbor;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(139) ) == 0 )		return Ruins_Of_Draconian;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(145) ) == 0 )		return Zakandia;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(144) ) == 0 )		return Tarintus;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(146) ) == 0 )		return Mont_Blanc_Port;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(147) ) == 0 )		return Dryed_Gazell_Fall;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(140) ) == 0 )		return Zakandia_Outpost;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(150) ) == 0 )		return The_Dark_Portal;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1260) ) == 0 )	return Distorted_crevice ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(148) ) == 0 )		return The_Way_To_The_Howling_Ravine ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1036) ) == 0 )	return Howling_Ravine ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1032) ) == 0 )	return Howling_Cave_1F ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1033) ) == 0 )	return Howling_Cave_2F ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1251) ) == 0 )	return Ghost_Tree_Swamp ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1260) ) == 0 )	return Distorted_crevice ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1328) ) == 0 )	return Lair_of_Kierra ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1426) ) == 0 )	return The_Valley_Of_Fairy ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1569) ) == 0 )	return The_Town_of_Nera_Castle ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1572) ) == 0 )	return The_Great_Garden ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1570) ) == 0 )	return A_Harbor_of_Nera ;
	// 080714 LUJ, "기사단의 묘지"맵 추가
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1596) ) == 0 )	return TheKnightsGrave ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1661) ) == 0 )	return DateDungeon_Cemetery ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1662) ) == 0 )	return DateDungeon_BrokenHouse ;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1663) ) == 0 )	return DateDungeon_DarkCave ;
	return 0;
}

#else

char* GetMapName(MAPTYPE mapnum)
{
	static char mapname[256];
#define MAPNAMEDEF(num,Name)	case num:	strcpy(mapname, #Name );	break;
	switch(mapnum)
	{
	MAPNAMEDEF(Alker_Farm,						Alker_Farm) ;
	MAPNAMEDEF(Moon_Blind_Swamp,				Moon_Blind_Swamp) ;	
	MAPNAMEDEF(Red_Orc_Outpost,					Red_Orc_Outpost) ;	
	MAPNAMEDEF(Moon_Blind_Forest,				Moon_Blind_Forest) ;	
	MAPNAMEDEF(Haunted_Mine_1F,					Haunted_Mine_1F) ;
	MAPNAMEDEF(Haunted_Mine_2F,					Haunted_Mine_2F) ;
	MAPNAMEDEF(The_Gate_Of_Alker,				The_Gate_Of_Alker) ;	
	MAPNAMEDEF(Alker_Harbor,					Alker_Harbor) ;
	MAPNAMEDEF(Ruins_Of_Draconian,				Ruins_Of_Draconian) ;
	MAPNAMEDEF(Zakandia,						Zakandia) ;
	MAPNAMEDEF(Tarintus,						Tarintus) ;	
	MAPNAMEDEF(Mont_Blanc_Port,					Mont_Blanc_Port) ;
	MAPNAMEDEF(Dryed_Gazell_Fall,				Dryed_Gazell_Fall) ;	
	MAPNAMEDEF(Zakandia_Outpost,				Zakandia_Outpost) ;	
	MAPNAMEDEF(The_Dark_Portal,					The_Dark_Portal) ;	
	MAPNAMEDEF(Distorted_crevice,				Distorted_crevice) ;
	MAPNAMEDEF(The_Way_To_The_Howling_Ravine,	The_Way_To_The_Howling_Ravine) ;
	MAPNAMEDEF(Howling_Ravine,					Howling_Ravine) ;
	MAPNAMEDEF(Howling_Cave_1F,					Howling_Cave_1F) ;
	MAPNAMEDEF(Howling_Cave_2F,					Howling_Cave_2F) ;
	MAPNAMEDEF(Ghost_Tree_Swamp,				Ghost_Tree_Swamp) ;
	MAPNAMEDEF(Lair_of_Kierra,					Lair_of_Kierra) ;
	MAPNAMEDEF(The_Valley_Of_Fairy,				The_Valley_Of_Fairy) ;
	MAPNAMEDEF(The_Town_of_Nera_Castle,			The_Town_of_Nera_Castle) ;
	MAPNAMEDEF(The_Great_Garden,				The_Great_Garden) ;
	MAPNAMEDEF(A_Harbor_of_Nera,				A_Harbor_of_Nera) ;
	// 080714 LUJ, "기사단의 묘지"맵 추가
	MAPNAMEDEF(TheKnightsGrave,					TheKnightsGrave) ;
	MAPNAMEDEF(DateDungeon_Cemetery,			DateDungeon_Cemetery) ;
	MAPNAMEDEF(DateDungeon_BrokenHouse,			DateDungeon_BrokenHouse) ;
	MAPNAMEDEF(DateDungeon_DarkCave,			DateDungeon_DarkCave) ;

	default:0,	mapname[0] = 0;
	}

	return mapname;
}
#endif

void SetProtocol(MSGBASE* pMsg,BYTE bCategory,BYTE bProtocol)
{
	pMsg->Category = bCategory;
	pMsg->Protocol = bProtocol;	
}


WP_KIND GetWeaponType(DWORD skillIdx)
{
//	if(MIN_SKILL_INDEX > skillIdx || skillIdx >= MAX_SKILL_INDEX) return WP_ERR;

	if(MIN_ATTACK_SKILL_INDEX <= skillIdx && skillIdx < MAX_ATTACK_SKILL_INDEX)
	{
		
		return (WP_KIND)((skillIdx - MIN_ATTACK_SKILL_INDEX)/ATTACK_SKILL_INTERVAL);
	}
	else if(MIN_DEFENSE_SKILL_INDEX <= skillIdx && skillIdx < MAX_DEFENSE_SKILL_INDEX)
	{
		return (WP_KIND)((skillIdx - MIN_DEFENSE_SKILL_INDEX)/DEFENSE_SKILL_INTERVAL);
	}
	else if(MIN_ASSIST_SKILL_INDEX <= skillIdx && skillIdx < MAX_ASSIST_SKILL_INDEX)
	{
		return (WP_KIND)((skillIdx - MIN_ASSIST_SKILL_INDEX)/ASSIST_SKILL_INTERVAL);
	}

	return WP_ERR;
}


eITEMTABLE GetTableIdxPosition(WORD abs_pos)
{
	//if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	// 071210 KTH -- 확장인벤토리까지 검사
	if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_EXTENDED_INVENTORY2_END)
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_STORAGE_START <= abs_pos && abs_pos < TP_STORAGE_END)
	{
		return eItemTable_Storage;
	}	
	/*else if(TP_STREETSTALL_START <= abs_pos && abs_pos < TP_STREETSTALL_END)
	{
		return eItemTable_StreetStall;
	}*/
	else if(TP_QUICK_START <= abs_pos && abs_pos < TP_QUICK_END)
	{
		return eItemTable_Quick;
	}
	else if(TP_SKILL1_START <= abs_pos && abs_pos < TP_SKILL2_END)
	{
		return eItemTable_Skill;
	}
	else if(TP_JINBUB_START <= abs_pos && abs_pos < TP_JINBUB_END)
	{
		return eItemTable_Skill;
	}
/*	else if(TP_MUNPAWAREHOUSE_START <= abs_pos && abs_pos < TP_MUNPAWAREHOUSE_END)
	{
		return eItemTable_MunpaWarehouse;
	}*/
	else if(TP_ABILITY_START <= abs_pos && abs_pos < TP_ABILITY_END)
	{
		return eItemTable_Ability;
	}
	else if(TP_SHOPINVEN_START <= abs_pos && abs_pos < TP_SHOPINVEN_END)
	{
		return eItemTable_ShopInven;
	}

	return eItemTable_Max;
}
/*
eITEM_KINDBIT GetItemKind(DWORD iconIdx)
{
#ifdef _CLIENT_
//	ASSERT( ITEMMGR->GetItemInfo(iconIdx) );	//KES 040503
	if( ITEMMGR->GetItemInfo(iconIdx) )
		return (eITEM_KINDBIT)ITEMMGR->GetItemInfo(iconIdx)->ItemKind;
#endif
#ifdef __MAPSERVER__
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(iconIdx);
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;
	else
	{
		return eKIND_ITEM_MAX;
	}
#endif

	return (eITEM_KINDBIT)0;
}
*/
eQUICKICON_KIND GetIconKind(DWORD wIdx)
{
	if(wIdx >= MIN_ABILITY_INDEX)
		return eABILITY_ICON;	
	if(wIdx >= MIN_YOUNGYAKITEM_INDEX)
		return eITEM_ICON;
	else
		return eSKILL_ICON;
}
/*
eSKILLITEM_KIND GetItemKindDetail(DWORD iconIdx)
{
	eITEM_KINDBIT kind = GetItemKind(iconIdx);
	if( (kind & 1024) == FALSE)
		return eERROR_BOOK;

	if(kind == 1038)
	{
		return eJINBUBBOOK;
	}
	if(kind == 1037)
	{
		return eSIMBUBBOOK;
	}
	return eSKILLBOOK;
}
*/
POSTYPE ConvAbsPos2SkillPos(POSTYPE abspos)
{
	if( abspos >= TP_SKILL1_START && abspos < TP_SKILL2_END )
		return abspos - TP_SKILL1_START;
	else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
		return abspos - TP_JINBUB_START + SLOT_SKILL_NUM*2;
	else
	{
		ASSERT(0);
		return 0;
	}
}


DWORD GetMainTargetID(MAINTARGET* pMainTarget)
{
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_OBJECTID)
		return pMainTarget->dwMainTargetID;
	else
		return 0;
}

#ifdef _MAPSERVER_
#ifndef _MURIMNET_
#include "UserTable.h"
#include "CharMove.h"
#endif
#endif
#ifdef _CLIENT_
#include "ObjectManager.h"
#endif

BOOL GetMainTargetPos(MAINTARGET* pMainTarget,VECTOR3* pRtPos, CObject ** ppObject)
{	
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
	{
		pMainTarget->cpTargetPos.Decompress(pRtPos);
		if(ppObject)
			*ppObject = NULL;
		return TRUE;
	}
	else
	{
		
#ifdef _MAPSERVER_
		CObject* pTarget;
		pTarget = g_pUserTable->FindUser(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		*pRtPos = *CCharMove::GetPosition(pTarget);
#endif
#ifdef _CLIENT_
		CObject* pTarget;
		pTarget = OBJECTMGR->GetObject(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		pTarget->GetPosition(pRtPos);
#endif
		return TRUE;
	}
}

void SetVector3(VECTOR3* vec,float x,float y,float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

#ifdef _CLIENT_
void VRand(VECTOR3* pResult,VECTOR3* pOriginPos,VECTOR3* pRandom)
{
	int temp;
	temp = (int)pRandom->x;
	pResult->x = temp ? pOriginPos->x + (rand() % (2*temp)) - temp : pOriginPos->x;
	temp = (int)pRandom->y;
	pResult->y = temp ? pOriginPos->y + (rand() % (2*temp)) - temp : pOriginPos->y;
	temp = (int)pRandom->z;
	pResult->z = temp ? pOriginPos->z + (rand() % (2*temp)) - temp : pOriginPos->z;
}

void TransToRelatedCoordinate(VECTOR3* pResult,VECTOR3* pOriginPos,float fAngleRadY)
{
	if(pResult == pOriginPos)
	{
		VECTOR3 tempOri = *pOriginPos;
		pOriginPos = &tempOri;
	}
	static BOOL bb = FALSE;
	if(bb == FALSE)
	{
		MATRIX4 my;
		SetRotationYMatrix(&my,-fAngleRadY);
		
		TransformVector3_VPTR2(pResult,pOriginPos,&my,1);
	}
	else
	{
		RotateVectorAxisY(pResult,pOriginPos,fAngleRadY);
	}
}

void RotateVectorAxisX(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadX)
{
	if(fAngleRadX  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		MATRIX4 mx;
		SetRotationXMatrix(&mx,-fAngleRadX);
		
		TransformVector3_VPTR2(pResult,pOriginVector,&mx,1);
	}
}

void RotateVectorAxisZ(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadZ)
{
	if(fAngleRadZ  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadZ) - pOriginVector->y*sinf(fAngleRadZ);
		pResult->y = pOriginVector->x*sinf(fAngleRadZ) + pOriginVector->y*cosf(fAngleRadZ);
		pResult->z = pOriginVector->z;
	}
}

void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, float x, float y, float z )
{
	RotateVectorAxisX( pResult, pOriginVector, x );
	RotateVectorAxisY( pResult, pResult, y );
	RotateVectorAxisZ( pResult, pResult, z );
}

void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, VECTOR3 vRot )
{
	RotateVectorAxisX( pResult, pOriginVector, vRot.x );
	RotateVectorAxisY( pResult, pResult, vRot.y );
	RotateVectorAxisZ( pResult, pResult, vRot.z );
}

#endif


void RotateVectorAxisY(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadY)
{
	if(fAngleRadY == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadY) - pOriginVector->z*sinf(fAngleRadY);
		pResult->y = pOriginVector->y;
		pResult->z = pOriginVector->x*sinf(fAngleRadY) + pOriginVector->z*cosf(fAngleRadY);
	}
}

void BeelinePoint(VECTOR3 * origin, VECTOR3 * dir, float dist, VECTOR3 * result)
{
/*
	float m = dir->x / dir->z;
	result->x = origin->x + dist*cosf(m);
	result->z = origin->z + dist*sinf(m);
*/
//KES
	float rrr = sqrt( dir->x*dir->x + dir->z*dir->z );
	result->x = origin->x + dir->x * dist / rrr;
	result->z = origin->z + dir->z * dist / rrr;
}
void AdjacentPointForOriginPoint(VECTOR3 * origin, VECTOR3 * target, float dist, VECTOR3 * result)
{
	float rrr = CalcDistanceXZ(origin,target);

	if(rrr < dist)
	{
		result->x = target->x;
		result->z = target->z;
		return;
	}

//KES
	result->x = origin->x + ( target->x - origin->x ) * dist / rrr;
	result->z = origin->z + ( target->z - origin->z ) * dist / rrr;

/*
	float m = (origin->x - target->x)/(origin->z - target->z);
	// taiyo 2004/5/18 : origin->x + , origin->z+
	result->x = origin->x + dist*cosf(m);
	result->z = origin->z + dist*sinf(m);
*/
}
float CalcDistanceXZ(VECTOR3* v1,VECTOR3* v2)
{
//	float x = v1->x - v2->x;
//	float z = v1->z - v2->z;
//	return sqrtf(x*x + z*z);
	return sqrtf( (v1->x - v2->x)*(v1->x - v2->x) + (v1->z - v2->z)*(v1->z - v2->z) );
}

BOOL IsSameDay(char* strDay,WORD Year,WORD Month,WORD Day)
{
	//ex) 2002.02.08
	char temp[64];
	sprintf(temp,"%d.%02d.%02d",Year,Month,Day);
	return strcmp(strDay,temp) == 0;
}



#endif	//_MURIMNET_


#ifdef _CLIENT_
extern DWORD gUserID;
void SendAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// 메세지 박스
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	char temp[256];
	if(pMsg)
		sprintf(temp,"%s\t%d\t%s",pStrFileName,Line,pMsg);
	else
		sprintf(temp,"%s\t%d",pStrFileName,Line);
		
	MSG_ASSERT msg;
	msg.SetMsg(gUserID,gHeroID,temp);
	NETWORK->Send(&msg,sizeof(msg));
}

#endif

void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// 메세지 박스
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	char temp[256];
	if(pMsg)
		sprintf(temp,"[%s] %s line:%d, msg:%s, ",g_szHeroIDName ,pStrFileName,Line,pMsg);
	else
		sprintf(temp,"[%s] %s line:%d, ",g_szHeroIDName ,pStrFileName,Line);
	
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
#ifdef _MAPSERVER_
	sprintf( szFile, "./Log/Assert_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
#else
	sprintf( szFile, "./Log/Assert_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );
#endif

	//파일의 용량검사.

	static BOOL bWrite = FALSE;

	if( bWrite == FALSE )
	{


		

		DWORD dwFileSize;
		DWORD dwSizeLow;
		DWORD dwSizeHigh;
		HANDLE hFile;
		OFSTRUCT data;

		hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);

		if((HFILE)hFile == 0xffffffff)
		{

			::CloseHandle(hFile);

			//파일이 존재하지 않는다면

			//while(1)
			{
				FILE* fp = fopen(szFile,"a+");
				if(fp)
				{
					fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());

					fclose(fp);
				}

				//hFile = hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);
				//dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
				//dwFileSize = dwSizeHigh << 16 | dwSizeLow;
				//::CloseHandle(hFile);

			//	if(dwFileSize > 1000000) break;
			}


			return ;
		}

		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		dwFileSize = dwSizeHigh << 16 | dwSizeLow;
		::CloseHandle(hFile);


		if(dwFileSize <= 1000000) //1M 보다 작다면 기록
		{
			FILE* fp = fopen(szFile,"a+");
			if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());

				fclose(fp);
			}
		}
		else
		{
			bWrite = TRUE;
			
		}

		

	}

}

void CriticalAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
	if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
		__asm int 3;
	return;
}


void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize )
{
	strncpy( pDest, pSrc, nDestBufSize -1 );
	pDest[nDestBufSize -1] = 0;
}


void SafeStrCpyEmptyChange( char* pDest, const char* pSrc, int nDestBufSize )
{
	// Hard.. Hard.. Temp.. 난중에 유연하게 바꾸도록 하자.. ㅋㅋㅋ
	// char b, char a   b->a 로 인자로 받아서 바꾸도록..
	char tmp[256] = { 0, };
	strncpy(tmp, pSrc, nDestBufSize-1);

	for(int i=0; i<256; i++)
	{
		if(tmp[i] == '_')		pDest[i] = ' ';
		else					pDest[i] = tmp[i];		
	}

	pDest[nDestBufSize-1] = 0;
}

char* GetCurTimeToString()
{
	static char temp[256];
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(temp,"%d월/%d일 %d시%d분%d.%d초",		// 월/일 시:분:초
		ti.wMonth,ti.wDay,
		ti.wHour,ti.wMinute,ti.wSecond,ti.wMilliseconds);
	return temp;
}

DWORD GetCurTimeValue()
{
	char temp2[256];

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(temp2,"%d%02d%02d",ti.wYear,ti.wMonth,ti.wDay);
	DWORD v2 = atoi(temp2);

	return v2;
}

DWORD GetCurTime()
{
	// 시간계산
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	stTIME time;
	time.SetTime( systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond );

	return time.value;
}

DWORD GetCurTimeToSecond()
{
	stTIME curtime;
	curtime.value = GetCurTime();

	DWORD timetosecond = 0;
	timetosecond += curtime.GetYear() * YEARTOSECOND;
	timetosecond += curtime.GetMonth() * MONTHTOSECOND;
	timetosecond += curtime.GetDay() * DAYTOSECOND;
	timetosecond += curtime.GetHour() * HOURTOSECOND;
	timetosecond += curtime.GetMinute() * MINUTETOSECOND;
	timetosecond += curtime.GetSecond();

	return timetosecond;
}

void StringtimeToSTTIME(stTIME* Time, char* strTime)
{
 	char year[3]		= { 0, };
 	char month[3]		= { 0, };
 	char day[3]			= { 0, };
 	char hour[3]		= { 0, };
 	char minute[3]		= { 0, };

	strncpy(year, &strTime[2], 2);
 	strncpy(month, &strTime[5], 2);
 	strncpy(day, &strTime[8], 2);
 	strncpy(hour, &strTime[11], 2);
 	strncpy(minute, &strTime[14],2);
 	Time->SetTime( atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), 0 );
}

BOOL CheckValidPosition(VECTOR3& pos)
{
	if(pos.x < 0 || pos.z < 0)
		return FALSE;
	if(pos.x >= 51200 || pos.z >= 51200)
		return FALSE;
	return TRUE;
}


float roughGetLength( float fDistX,float fDistY )
{
	if( fDistX < 0.0f ) fDistX = -fDistX;
	if( fDistY < 0.0f ) fDistY = -fDistY;
	
	int min = (int)(( fDistX > fDistY ) ? fDistY : fDistX);

	return ( fDistX + fDistY - ( min >> 1 ) - ( min >> 2 ) + ( min >> 4 ) );
}

//////////////////////////////////////////////////////////////////////////
// Effect번호를 파일이름으로 찾기 위한 부분
map<string,DWORD> g_fileTable;

void LoadEffectFileTable(char* pListFileName)
{
#ifdef _CLIENT_
	CMHFile file;
	file.Init(pListFileName,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	DWORD MaxEffectDesc = file.GetDword();
	
	DWORD effnum;
	char efffile[256];
	for(DWORD n=0;n<MaxEffectDesc;++n)
	{
		//ASSERT(!pFile->IsEOF());

		effnum = file.GetDword();
		file.GetString(efffile);
		strupr(efffile);
		if(strcmp(efffile,"NULL") == 0)
			continue;
		
		g_fileTable.insert(map<string,DWORD>::value_type(efffile,effnum));
	}	

	file.Release();
#endif
}

int FindEffectNum(char* pFileName)
{
#ifdef _CLIENT_
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;

	static char tempbuf[64];
	strcpy(tempbuf,pFileName);
	map<string,DWORD>::iterator iter;
	iter = g_fileTable.find(strupr(tempbuf));
	if(iter == g_fileTable.end())
	{
//		ASSERTMSG(0,pFileName);
		return -1;
	}
	else
		return iter->second;
#else
	// 06. 04. 스킬 버그 수정 - 이영준
    // 맵서버에서는 이펙트가 있는지 없는지만 가지고 있는다
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;
	else
		return 1;
#endif
}

void ERRORBSGBOX(char* str,...)
{
	char msg2[255];
	va_list argList;

	va_start(argList, str);
	vsprintf(msg2,str,argList);
	va_end(argList);

	MessageBox(NULL,msg2,NULL,NULL);
	
}

// Weather System
const char* GetDay(WORD nDay)
{
	return Days[nDay];
}

WORD GetDay(const char* strDay)
{
	for(WORD i = 0; i < 7; i++)
	{
		if(strcmp(strDay, Days[i]) == 0)
			return i;
	}

	return -1;
}

const char* GetWeather(WORD nWeatherState)
{
	return WeatherState[nWeatherState];
}

WORD GetWeather(const char* strWeather)
{
	for(WORD i = 0; i < eWS_Max; i++)
	{
		if(strcmp(strWeather, WeatherState[i]) == 0)
			return i;
	}

	return -1;
}

BOOL IsEmptyLine(char* pszText)
{
	int nLen = strlen(pszText);

	// 문자열 중에 스페이스나 탭 외의 문자가 섞여 있으면 FALSE
	for(int i=0; i<nLen; i++)
	{
		if (pszText[i] == ' ' ||
			pszText[i] == '	')
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

// 0710111524 와 같은 int 형식의 날짜를 사용
int	 GetPastTime(DWORD nDate)
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	DWORD nYear			= nDate/100000000;
	DWORD nMonth		= (nDate - nYear*100000000)/1000000;
	DWORD nDay			= (nDate - nYear*100000000 - nMonth*1000000)/10000;
	DWORD nHour			= (nDate - nYear*100000000 - nMonth*1000000 - nDay*10000)/100;
	DWORD nMin			= (nDate - nYear*100000000 - nMonth*1000000 - nDay*10000 - nHour*100);
	DWORD tblMonth[]	= {NULL, 1,	 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 1,	 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12};
	DWORD tblDay[]		= {NULL, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// 4년마다 2월은 29일이당..
	if (t.wYear%4 == 0)
	{
		tblDay[2]  = 29;
		tblDay[14] = 29;
	}

	// 일단 년도 차이에 따른 일 계산
	int nPastTime = (t.wYear-2000 - nYear) * 365;

	// 현재 월이 과거 월보다 크다면 과거 월 -> 현재 월 순서로 날짜 합산
	if (t.wMonth >= nMonth)
	{
		for(int i=nMonth; i<25; i++)
		{
			if (t.wMonth == tblMonth[i])
			{
				break;
			}

			// 월 차이에 따른 일 계산
			nPastTime += tblDay[i];
		}
	}
	// 현재 월이 과거 월보다 작다면 현재 월 <- 과거 월 순서로 날짜 차감
	else
	{
		for(int i=nMonth; i>=0; i--)
		{
			if (t.wMonth == tblMonth[i])
			{
				break;
			}

			// 월 차이에 따른 일 계산
			nPastTime -= tblDay[i];
		}
	}

	// 일 차이, 시간에 따른 총 지난 시간 계산
	nPastTime += (t.wDay - nDay);
	nPastTime = nPastTime*24 + ((t.wHour - nHour) - (59 - (t.wMinute - nMin))/60);

	return nPastTime;
}

// '2007-12-11 17:46' 와 같은 char 형식의 날짜를 사용
int	 GetPastTime(char* pszDate)
{
	// 잘못된 버퍼를 사용했을 경우 처리
	if (strlen(pszDate) < 16) return 0;

	char szTmp[16]="";

	SYSTEMTIME t;
	GetLocalTime(&t);

	DWORD nYear			= atoi(strncpy(szTmp, pszDate, 4));
	ZeroMemory(szTmp, sizeof(szTmp));
	DWORD nMonth		= atoi(strncpy(szTmp, &pszDate[5], 2));
	ZeroMemory(szTmp, sizeof(szTmp));
	DWORD nDay			= atoi(strncpy(szTmp, &pszDate[8], 2));
	ZeroMemory(szTmp, sizeof(szTmp));
	DWORD nHour			= atoi(strncpy(szTmp, &pszDate[11], 2));
	ZeroMemory(szTmp, sizeof(szTmp));
	DWORD nMin			= atoi(strncpy(szTmp, &pszDate[14], 2));
	ZeroMemory(szTmp, sizeof(szTmp));
	DWORD tblMonth[]	= {NULL, 1,	 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 1,	 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12};
	DWORD tblDay[]		= {NULL, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// 4년마다 2월은 29일이당..
	if (t.wYear%4 == 0)
	{
		tblDay[2]  = 29;
		tblDay[14] = 29;
	}

	// 일단 년도 차이에 따른 일 계산
	int nPastTime = (t.wYear - nYear) * 365;
	// 현재 월이 과거 월보다 크다면 과거 월 -> 현재 월 순서로 날짜 합산
	if (t.wMonth >= nMonth)
	{
		for(int i=nMonth; i<25; i++)
		{
			if (t.wMonth == tblMonth[i])
			{
				break;
			}

			// 월 차이에 따른 일 계산
			nPastTime += tblDay[i];
		}
	}
	// 현재 월이 과거 월보다 작다면 현재 월 <- 과거 월 순서로 날짜 차감
	else
	{
		for(int i=nMonth; i>=0; i--)
		{
			if (t.wMonth == tblMonth[i])
			{
				break;
			}

			// 월 차이에 따른 일 계산
			nPastTime -= tblDay[i];
		}
	}

	// 일 차이, 시간에 따른 총 지난 시간 계산
	nPastTime += (t.wDay - nDay);
	nPastTime = nPastTime*24 + ((t.wHour - nHour) - (59 - (t.wMinute - nMin))/60);

	return nPastTime;
}

// 090109 LUJ, 스크립트 검증 값을 설정한다
void SetScriptCheckValue( ScriptCheckValue& checkValue )
{
#ifdef _UNICODE
	typedef	std::wstring Text;
#else
	typedef	std::string	Text;
#endif
	typedef std::list< Text >	NameList;
	NameList					nameList;

	// 090106 LUJ, 폴더에 불필요한 bin 파일이 있을 경우 접속 불능 사유가 되므로,
	//			불편해도 필수 파일을 수동으로 목록에 넣자
	// 090119 LUJ, 파일 이름에 붙은 확장자 제거/파일 목록 수정. 확장자는 빼고
	//			파일명을 입력해야한다. 또한, 확인이 쉽도록 정렬시켜 입력하자.
	{
		nameList.push_back( _T( "BodyList_E_M" ) );
		nameList.push_back( _T( "BodyList_E_W" ) );
		nameList.push_back( _T( "BodyList_H_M" ) );
		nameList.push_back( _T( "BodyList_H_W" ) );
		nameList.push_back( _T( "BossMonsterfileList" ) );
		nameList.push_back( _T( "BRList" ) );
		nameList.push_back( _T( "CharMake_List" ) );
		nameList.push_back( _T( "DateMatching" ) );
		nameList.push_back( _T( "Dealitem" ) );
		nameList.push_back( _T( "Event_get_list" ) );
		nameList.push_back( _T( "Family" ) );
		nameList.push_back( _T( "Farm" ) );
		nameList.push_back( _T( "FarmRenderObj" ) );
		nameList.push_back( _T( "FieldBossList" ) );
		nameList.push_back( _T( "FilterWord" ) );
		nameList.push_back( _T( "FishingExpPoint" ) );
		nameList.push_back( _T( "FishingMissionCode" ) );
		nameList.push_back( _T( "FishingRateDefine" ) );
		nameList.push_back( _T( "guild_setting" ) );
		nameList.push_back( _T( "ItemApplyOption" ) );
		nameList.push_back( _T( "ItemBodyChange" ) );
		nameList.push_back( _T( "itemCoolTime" ) );
		nameList.push_back( _T( "itemDissolve" ) );
		nameList.push_back( _T( "ItemEnchant" ) );
		nameList.push_back( _T( "ItemList" ) );
		nameList.push_back( _T( "ItemMix" ) );
		nameList.push_back( _T( "itemReinforce" ) );
		nameList.push_back( _T( "LoginPoint" ) );
		nameList.push_back( _T( "MapMoveList" ) );
		nameList.push_back( _T( "MapMoveScroll" ) );
		nameList.push_back( _T( "mix_setting" ) );
		nameList.push_back( _T( "MonsterList" ) );
		nameList.push_back( _T( "MonsterMeterReward" ) );
		nameList.push_back( _T( "MonSummonItem" ) );
		nameList.push_back( _T( "NpcBuffList" ) );
		nameList.push_back( _T( "NpcList" ) );
		nameList.push_back( _T( "NpcRecallInfo" ) );
		nameList.push_back( _T( "Pet_Costume" ) );
		nameList.push_back( _T( "PetHPMPInfo" ) );
		nameList.push_back( _T( "Petlist" ) );
		nameList.push_back( _T( "PetStatusInfo" ) );
		nameList.push_back( _T( "PetSystemInfo" ) );
		nameList.push_back( _T( "QuestItemList" ) );
		nameList.push_back( _T( "QuestScript" ) );
		nameList.push_back( _T( "ResidentRegist" ) );
		nameList.push_back( _T( "SafetyLoc" ) );
		nameList.push_back( _T( "SetItem" ) );
		nameList.push_back( _T( "SkillList" ) );
		nameList.push_back( _T( "SkillMoney" ) );
		nameList.push_back( _T( "Skill_Buff_List" ) );
		nameList.push_back( _T( "skill_get_list" ) );
		nameList.push_back( _T( "StorageListInfo" ) );
	}

	LONGLONG	base		= 1;
	const Text	path		= _T( "system\\resource\\" );
	const Text	extenstion	= _T( ".bin" );

	// 090106 LUJ, 코드 값을 얻어낸다
	for(	NameList::const_iterator nameIterator = nameList.begin();
			nameList.end() != nameIterator;
			++nameIterator, base *= 10 )
	{
		const Text& name( *nameIterator );
		const Text	fileName( path + name + extenstion );

		CMHFile mhFile;
		mhFile.Init( const_cast< TCHAR* >( fileName.c_str() ), _T( "rb" ) );

		checkValue.mValue = ( checkValue.mValue + LONGLONG( mhFile.GetCrc1() ) * base + LONGLONG( mhFile.GetCrc2() ) * base * 10 + mhFile.GetDataSize());//beyond
	}
}

BOOL IsCharInString( char* pStr, char* pNoChar )
{
	while( *pStr )
	{
		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;			
		}
		else
		{
			char* p = pNoChar;
			while( *p )
			{
				if( *pStr == *p ) return TRUE;
				++p;
			}
		}
		++pStr;
	}

	return FALSE;
}
