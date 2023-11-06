// GameResourceManager.h: interface for the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameResourceStruct.h"
//#include "MapChange.h"
class CMHFile;
#include "ptrlist.h"
#include <vector>


#define GAMERESRCMNGR	USINGTON(CGameResourceManager)

// 070121 LYW --- Define radius of potal.
#define POTAL_RAD	308

///////////////////////////////////////////////////////////////////////////////
// 06. 05. PreLoadData 추가기능 - 이영준
// 이펙트 PreLoad
struct sPRELOAD_EFFECT_INFO{
	int MapNum;
	int Count;

	cPtrList Effect;
};
///////////////////////////////////////////////////////////////////////////////



class CGameResourceManager  
{
	BOOL m_bMonsterRegenLoad;
	BOOL m_bNpcRegenLoad;
	BOOL m_bMonsterListLoad;
	BOOL m_bNpcListLoad;
	BOOL m_bMonsterDropItem;
	
	DWORD m_MaxNpcChxList;
	CHXLIST* m_NpcChxList;

	//060608 - fixed by wonju-------------------------------------------------------------------------
private:
	//맵에 분포하는 몬스터의 레벨
	struct MonsterInfoInMap
	{
		unsigned int MapNumber;
		unsigned int MinLevel;
		unsigned int MaxLevel;
	};

	struct ItemChangeRateofLv
	{
		unsigned int Level;    //레벨
		unsigned int Rate[10]; //변화율 -> 속성수치 +0 ~ +9 : 각각의 변화율
	};

	struct ItemChangeRateofAttrib
	{
		
		struct _attribInfo
		{
			WORD changeIndex;
			unsigned int changeRate;
		};
		
		_attribInfo attribinfo[5]; //0 base 1 life 2 mana 3 power 4 defend
	};

	std::vector<MonsterInfoInMap> m_MonsterInfoInMapList; //by wonju
	std::vector<ItemChangeRateofLv> m_ItemChangeRateofLvList; //Add 060705 by wonju
	std::vector<ItemChangeRateofAttrib> m_ItemChangeRateAttribList; //Add 060707 by wonju
	std::vector<std::string> m_UserCommendList; //Add 060727 by wonju

public:

	// 070420 LYW --- GameResourceManager : Add function to release m_ModFileList.
	void ReleaseModFileList() ;

	void LoadUserCommendList(); //Add 060727 by wonju

	void LoadItemChangeRateofLv();    //Add 060705 by wonju
	void LoadItemChangeRateofAttrib();//Add 060707 by wonju
	
	void LoadMonsterInfoInMapList(); //by wonju

	std::vector<std::string>& GetUserCommendList() { return m_UserCommendList; }
	std::vector<MonsterInfoInMap>& GetMonsterInfoInMapList() { return m_MonsterInfoInMapList; }
	int CheckAttribItemIndex(WORD idx);
	std::vector<ItemChangeRateofAttrib>& GetItemChangeRateAttribList() { return m_ItemChangeRateAttribList; }
	std::vector<ItemChangeRateofLv>& GetItemChangeRateOfLvList() { return m_ItemChangeRateofLvList; }
	unsigned int GetMinMonsterLvInMap(unsigned int MapNum);
	
	
	//------------------------------------------------------------------------------------------------
public:
	
	WORD m_TotalNpcGroupCount;
	WORD m_MaxNpcRegenCount;
	CGameResourceManager();
	virtual ~CGameResourceManager();

	//GETINSTANCE(CGameResourceManager);


	/* Á÷¾÷º° ±âº» ½ºÅÝÆ÷ÀÎÆ® */
	CharBasicStats				m_BasicStatsForJob[MAX_JOB];

	/* ¸ó½ºÅÍ Æ÷ÀÎÅÍ */	
	// 0 index»ç¿ë¾ÈÇÔ
	// second index 
	// 0 => -5, 1 => -4, 2 => -3, 3 => -2, ....
	EXPTYPE m_PLAYERxMONSTER_POINT[MAX_PLAYERLEVEL_NUM][MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1];
	

	/* Ä³¸¯ÅÍ »ý¼º ±âº» ¼öÄ¡ */
//	CharMakeCombo* InitCharMakeCombo();

	/* Ä³¸¯ÅÍ °æÇèÄ¡ */
	EXPTYPE m_CharacterExpPoint[MAX_CHARACTER_LEVEL_NUM];
	BOOL LoadExpPoint();
	EXPTYPE GetMaxExpPoint(LEVELTYPE level)
	{ 
		// 06.09.14 RaMa  수정
		if( level > 0 )			return m_CharacterExpPoint[level-1];
		else					return m_CharacterExpPoint[0];
	}
	/* ¸ó½ºÅÍ Æ÷ÀÎÅÍ */
	BOOL LoadPlayerxMonsterPoint();
	EXPTYPE GetPLAYERxMONSTER_POINT(LEVELTYPE level,int levelgap);

	// 080424 NYJ --- 낚시숙련도 추가
	EXPTYPE m_FishingExpPoint[MAX_FISHING_LEVEL];
	BOOL LoadFishingExpPoint();
	EXPTYPE GetFishingMaxExpPoint(LEVELTYPE level)
	{ 
		if( level > 0 )			return m_FishingExpPoint[level-1];
		else					return m_FishingExpPoint[0];
	}

	/* ¸ó½ºÅÍ ¸®½ºÆ® */
	BOOL LoadMonsterList();

	/* Æ¯¼ö ¸ó½ºÅÍ µ¥ÀÌÅÍ ¸®½ºÆ®  */
	CYHHashTable<BASE_MONSTER_LIST> m_SMonsterListTable;
	void LoadSMonsterList();
	void LoadSMonsterInfo( int idx, char * fileName );
	BASE_MONSTER_LIST * GetMonsterListInfo(int idx);
	int GetMonsterIndexForName( char* pStrName );
	void ResetMonsterList();

	void GetMonsterStats(WORD wMonsterKind, monster_stats * stats);				// ¸ó½ºÅÍ °´Ã¼ ÃÊ±âÈ­µÉ¶§
	BOOL LoadNpcChxList();
	char* GetMonsterChxName(DWORD index);
	char* GetNpcChxName(DWORD index);
 
	NPC_LIST m_NpcList[MAX_NPC_NUM];
	BOOL LoadNpcList(NPC_LIST* pNpcList);

	NPC_LIST* GetNpcInfo(WORD kind){return &m_NpcList[kind-1];}

	STATIC_NPCINFO*	GetStaticNpcInfo( WORD wNpcUniqueIdx );

	// 070121 LYW --- Add function to check potal position.
	WORD CheckMapChange( VECTOR3* pVector, WORD curMapNum ) ;

	BOOL LoadMapChangeArea(); //060608 - Add by wonju
	BOOL LoadMapChangeArea(MAPCHANGE_INFO* pMapChangeArea);
	BOOL LoadLoginPoint(LOGINPOINT_INFO* pLogin);
	MAPCHANGE_INFO m_MapChangeArea[MAX_POINT_NUM];
	LOGINPOINT_INFO m_LoginPoint[MAX_POINT_NUM];
	void GetRegenPoint(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex);
	void GetRegenPointFromEventMap(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex,DWORD dwTeam);
	MAPCHANGE_INFO* GetMapChangeInfo(DWORD Kind)
	{
		for(int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_MapChangeArea[i].Kind == Kind)
				return &m_MapChangeArea[i];
		}
		return NULL;
	}
//KES for Cheat
	MAPCHANGE_INFO* GetMapChangeInfoForDestMapNum( WORD wDestMapNum )
	{
		for(int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_MapChangeArea[i].MoveMapNum == wDestMapNum)
				return &m_MapChangeArea[i];
		}
		return NULL;
	}

	MAPCHANGE_INFO* GetMapChangeInfoFromToMapNum( WORD wFromMapNum, WORD wToMapNum )
	{
		for(int i = 0; i < MAX_POINT_NUM; ++i)
		{
			if(m_MapChangeArea[i].CurMapNum == wFromMapNum && m_MapChangeArea[i].MoveMapNum == wToMapNum)
				return &m_MapChangeArea[i];
		}
		return NULL;
	}
	
	LOGINPOINT_INFO* GetLoginPointInfo(DWORD Kind)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].Kind == Kind)
				return &m_LoginPoint[i];
		}
		return NULL;
	}
	LOGINPOINT_INFO* GetLoginPointInfoFromMapNum(WORD MapNum)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].MapNum == MapNum)
				return &m_LoginPoint[i];
		}
		ASSERT(0);
		return &m_LoginPoint[0];
	}
	BOOL CheckCorrectLoginPoint(VECTOR3* pos);

	BOOL LoadDeveloperInfo();

	// Avatar Item
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;
	BOOL LoadAvatarEquipList();
	// ShopItem Dup Option
	CYHHashTable<SHOPITEMDUP>		m_ShopItemDupOptionTable;
	BOOL LoadShopItemDupList();

	
//-----------------------------------------------------------------------------------------------------------//
//		Å¬¶óÀÌ¾ðÆ®¸¸ »ç¿ëÇÏ´Â ºÎºÐ
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

	MOD_LIST			m_ModFileList[RaceType_Max][GENDER_MAX];
	MOD_LIST			m_HairModList[RaceType_Max][GENDER_MAX];
	MOD_LIST			m_FaceModList[RaceType_Max][GENDER_MAX];
	BOOL LoadModFileList(MOD_LIST pModList[RaceType_Max][GENDER_MAX]);
	BOOL LoadHairModFileList(MOD_LIST pHairModList[RaceType_Max][GENDER_MAX]);
	BOOL LoadFaceModFileList(MOD_LIST pFaceModList[RaceType_Max][GENDER_MAX]); //pjscode
	BOOL LoadBodyModFileList(MOD_LIST pBodyModList[RaceType_Max][GENDER_MAX]);

	MOD_LIST			mPetModList;
	BOOL LoadPetModList();

	GAMEDESC_INI		m_GameDesc;
	// 070201 LYW --- Add function to return GAMEDESC_INI.
	void GetDispInfo( DISPLAY_INFO* dispInfo ) { memcpy(dispInfo, &m_GameDesc.dispInfo, sizeof(DISPLAY_INFO)) ; }
	BOOL LoadGameDesc();
	
	void LoadPreDataTable();
	void DeletePreLoadData();
	void LoadPreMonsterData(sPRELOAD_INFO* pPreLoadInfo, MAPTYPE MapNum);	
	CYHHashTable<sPRELOAD_INFO>		m_PreLoadDataTable;
	sPRELOAD_INFO*	GetPreDataInfo(int MapNum)		{	return m_PreLoadDataTable.GetData(MapNum);	}
	cPtrList m_PreLoadEfflist;
	cPtrList* GetPreLoadEfflist()	{	return &m_PreLoadEfflist;	}
	cPtrList m_PreLoadModlist;
	cPtrList* GetPreLoadModlist()	{	return &m_PreLoadModlist;	}
	///////////////////////////////////////////////////////////////////////////////
	// 06. 04. PreLoadData 추가기능 - 이영준
	WORD* m_PreLoadItemlist;
	WORD* GetPreLoadItemlist()	{	return m_PreLoadItemlist;	}
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// 06. 05. PreLoadData 추가기능 - 이영준
	CYHHashTable<sPRELOAD_EFFECT_INFO>		m_PreLoadEffectTable;
	sPRELOAD_EFFECT_INFO*	GetPreEffectInfo(int MapNum)		{	return m_PreLoadEffectTable.GetData(MapNum);	}
	///////////////////////////////////////////////////////////////////////////////

	void PreLoadData();	
	
	// serverlist
	SEVERLIST	m_ServerList[30];
	int			m_nMaxServerList;
	BOOL		LoadServerList();
	char*		GetServerSetName();			// 현재서버셋의 이름을 리턴

#ifdef _TESTCLIENT_
	TESTCLIENTINFO m_TestClientInfo;
	BOOL LoadTestClientInfo();
#endif

//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------//
//		¼­¹ö¸¸ »ç¿ëÇÏ´Â ºÎºÐ
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
	
//	void GetMonsterStats(WORD wMonsterKind, MONSTER_STATS * stats);

	VECTOR3 m_RevivePoint;
	// 071201 KTH - SafetyZone 정보를 담고 있는 Map
	std::map<MAPTYPE, VECTOR3> m_RevivePointMap;

	void LoadRevivePoint();
	VECTOR3* GetRevivePoint()
	{
		//return &m_RevivePoint;
		// 071201 KTH - Find LoadingMap SafetyZone
		std::map<MAPTYPE, VECTOR3>::iterator find_itr = m_RevivePointMap.find( m_LoadingMapNum );

		if( find_itr != m_RevivePointMap.end() )
		{
			VECTOR3* pVector3 = &find_itr->second;
			return pVector3;
		}

		return NULL;
	}
	// 071201 KTH - 해당 맵의 SaftyZone을 가져옴.
	VECTOR3* GetRevivePoint(MAPTYPE MapNum)
	{
		std::map<MAPTYPE, VECTOR3>::iterator find_itr = m_RevivePointMap.find( MapNum );
		if( find_itr != m_RevivePointMap.end() )
		{
			VECTOR3* pVector3 = &find_itr->second;
			return pVector3;
		}

		return NULL;
	}

	// 071221 KTH -- 기억이 가능한 맵리스트.
	CYHHashTable<MAPTYPE>					m_PreLoadMemoryMapList;
	void LoadPreMemoryMapList();
	BOOL IsMemorizableMap(MAPTYPE mapType);

	MAPTYPE m_LoadingMapNum;
	MAPTYPE GetLoadMapNum()
	{
		return m_LoadingMapNum;
	}
	void SetLoadMapNum(MAPTYPE Mapnum);

	BOOL LoadNpcRegen(NPC_REGEN pNpcRegen[MAX_NPC_GROUPNUM][MAX_NPC_REGEN_NUM]);

	MAPCHANGE_INFO* GetMapChangeAreaValue(){return m_MapChangeArea;}
	LOGINPOINT_INFO* GetLoginPointValue(){return m_LoginPoint;}
	
	void SetNpcRegenLoadFlag(BOOL flag){m_bNpcRegenLoad = flag;}
	void SetNpcListLoadFlag(BOOL flag){m_bNpcListLoad = flag;}
	BOOL GetNpcRegenLoadFlag(){return m_bNpcRegenLoad;}
	BOOL GetNpcListLoadFlag(){return m_bNpcListLoad;}
	

	CYHHashTable<STATIC_NPCINFO>	m_StaticNpcTable;
	BOOL	LoadStaticNpc();
	void	ReleaseStaticNpc();
	NPC_LIST* GetNpcList(WORD JobKind);

	
	CYHHashTable<STATIC_NPCINFO>	m_WareNpcTable;
	BOOL	IsInWareNpcPosition( VECTOR3* pvPos );

	// 06. 05 HIDE NPC - 이영준
	CYHHashTable<HIDE_NPC_INFO>		m_HideNpcTable;
	BOOL	LoadHideNpcList();
	HIDE_NPC_INFO* GetHideNpcInfo(WORD UniqueIdx) { return m_HideNpcTable.GetData(UniqueIdx); }
	CYHHashTable<HIDE_NPC_INFO>* GetHideNpcTable() { return &m_HideNpcTable; }

//---KES 스킬초기화돈	071207
private:
	CYHHashTable<SKILL_MONEY>	m_htSkillMoney;
public:
	BOOL LoadSkillMoney();
	DWORD GetSkillMoney( DWORD SkillIdx );
//----------------------------

//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
};
EXTERNGLOBALTON(CGameResourceManager);
#endif // !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
