#pragma once


#include "Hero.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Npc.h"
#include "MapObject.h"
#include "Pet.h"
#include "HeroPet.h"
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07
#include "../hseos/Farm/SHFarmRenderObj.h"
// E 농장시스템 추가 added by hseos 2007.05.07


#define OBJECTMGR USINGTON(CObjectManager)
#define HERO OBJECTMGR->GetHero()
#define HEROPET OBJECTMGR->GetHeroPet()
#define HEROPETBACKUP OBJECTMGR->GetHeroPetBackup()


class CMonster;
class CNpc;
class CMapObject;
class CSHFarmRenderObj;
class CBossMonster;
class CObject;
class CSkillArea;
class cSkillObject;
class CCharacterCalcManager;
class CMonsterGuageDlg;
//class CPet;

enum eOPTION_OVERINFO
{
	eOO_NAME_NONE			= 0,
	eOO_NAME_CLICKEDONLY	= 1,
	eOO_NAME_WHOLE			= 2,
	eOO_NAME_PARTY			= 4,
	eOO_NAME_MUNPA			= 8,
	eOO_CHAT_NOBALLOON		= 16,
	eOO_DAMAGE_MEMBERS		= 32,
};

enum eOPTION_SHADOW
{
	eOO_SHADOW_NONE				= 0,
	eOO_SHADOW_ONLYHERO			= 1,
	eOO_SHADOW_MONSTER			= 2,
	eOO_SHADOW_OTHERCHARACTER	= 4,
};


enum eOPTION_EFFECT
{
	eOO_EFFECT_DEFAULT			= 0,
	eOO_EFFECT_ONLYHERO			= 1,
	eOO_EFFECT_NOT_USE			= 2,
};


class CObjectManager  
{
	CHero* m_pHero;
	CHeroPet* mpHeroPet;
	CHeroPet* mpHeroPetBackup;

	CBossMonster * m_pBoss[eBOSSTYPE_MAX];

	HEFFPROC m_hRestHandle[eBOSSTYPE_MAX];

	cPtrList m_GarbageObjectList;
	
	void RemoveObject(CObject** ppObject);
	CCharacterCalcManager * m_pCharCalcMgr;

	CYHHashTable<CObject> m_ObjectTable;
	CYHHashTable<CObject> m_HideObjectTable;

	CMemoryPoolTempl<CPlayer>		m_PlayerPool;
	CMemoryPoolTempl<CMonster>		m_MonsterPool;
	CMemoryPoolTempl<CNpc>			m_NpcPool;
	CMemoryPoolTempl<CBossMonster>	m_BossMonsterPool;
	CMemoryPoolTempl<CMapObject>	m_MapObjectPool;
	CMemoryPoolTempl<CPet>			m_PetPool;

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.07
	CMemoryPoolTempl<CSHFarmRenderObj>	m_csFarmRenderObjPool;
	// E 농장시스템 추가 added by hseos 2007.05.07

	DWORD					m_SelectedObjectID;
	DWORD					m_SelectEffectHandle;
	CObject					* m_pDPObjectName;

//---OverInfoOption
	DWORD		m_dwOverInfoOption;
	BOOL		m_bAllNameShow;
//---ShadowOption
	DWORD		m_dwShadowOption;

	//trustpak 2005/06/27
//---EffectOption
	DWORD		m_dwEffectOption;

	///
	
public:
	//MAKESINGLETON(CObjectManager)
		
	CObjectManager();
	virtual ~CObjectManager();

	void RegisterHero(SEND_HERO_TOTALINFO * sendHeroTotalInfo);
#ifndef _RESOURCE_WORK_NOT_APPLY_
	CPlayer* AddPlayerThroughCache(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo, SHOPITEMOPTION* pShopItemOption, SEND_CHARACTER_TOTALINFO* pMsg);
	CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption);
#else
	CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption);
#endif

	CMonster* AddMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo);
	CBossMonster* AddBossMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo);
	CNpc* AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,NPC_TOTALINFO* pTotalInfo);
	void AddSkillObject(cSkillObject* pSObj);
	CMapObject* AddMapObject(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, char* strData, float fScale, float fAngle );

	CPet* AddPet(DWORD MasterIdx, BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,PET_OBJECT_INFO* pPetObjectInfo, ITEMBASE* pItemInfo);
	CHeroPet* AddHeroPet(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,PET_OBJECT_INFO* pPetObjectInfo, ITEMBASE* pItemInfo);
	void RemoveHeroPet();

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.07
	CSHFarmRenderObj* AddFarmRenderObj(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, CSHFarmRenderObj::stFarmRenderObjInfo* pFarmRenderObjInfo);
	// E 농장시스템 추가 added by hseos 2007.05.07

	CObject* GetObject(DWORD dwObjectID);
	
	void RemoveAllObject();
	
	void AddGarbageObject(CObject* pObject);
	void ProcessGarbage();

	void GetTargetInRange(VECTOR3* pPivotPos,float Radius,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK,WORD PARAM);
	DWORD GetSingleTargetInRange(VECTOR3* pPivotPos,float Radius, BOOL bPK,WORD PARAM);
	void GetTargetInArea(CHero* pHero,CSkillArea* pSkillArea,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK,WORD PARAM);

	void UpdateTargetList_Monster(cSkillObject* pSkillObject);

	CObject*	GetSelectedObject();
	void		SetSelectedObject(CObject* pObject);
	DWORD		GetSelectedObjectID() { return m_SelectedObjectID; }
	void		SetSelectedObjectID(DWORD dwId);
	void		SetSelectedEffect(CObject* pObject);
	// 070509 LYW --- ObjectManager : Add function to setting seletecd info.
	BOOL SeletedLastObject(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	void SelectedPlayer(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	void SelectedPet(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	void SelectedNpc(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	void SelectedMonster(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.22
	void SelectedFarmObj(CObject* pObject, CMonsterGuageDlg* pGuageDlg) ;
	// E 농장시스템 추가 added by hseos 2007.08.22

////
	void		SetDPObject(CObject* pObject) { m_pDPObjectName = pObject; }
	CObject *	GetDPObject() { return m_pDPObjectName; }

	void OnMouseOver( CObject* pObject );
	void OnMouseLeave( CObject* pObject );

///---overinfo
	void SetOverInfoOption( DWORD dwOption );
	void ApplyOverInfoOption( CObject* pObject );
	void ApplyOverInfoOptionToAll();
	BOOL CanOverInfoChange( CObject* pObject );
	DWORD GetObjectNameColor( CObject* pObject );
	void ShowChatBalloon( CObject* pObject, char* pStrChat, DWORD dwColor = RGB_HALF(70,70,70), DWORD dwAliveTime = 5000 );
	void HideChatBalloonAll();
	void SetAllNameShow( BOOL bShow );

//---Shadow
	void SetShadowOption( DWORD dwOption );
	void ApplyShadowOption( CObject* pObject );
	void ApplyShadowOptionToAll();
	void AddShadow(CObject* pObject);
	void RemoveShadow(CObject* pObject);


	//trustpak 2005/06/27
//---Effect
	void SetEffectOption( DWORD dwOption )			{ m_dwEffectOption = dwOption; }
	DWORD GetEffectOption( VOID )					{ return m_dwEffectOption; }
	///	

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.08	2007.09.17
	DWORD	GetSelEffectHandle()					{ return m_SelectEffectHandle; }
	CMemoryPoolTempl<CSHFarmRenderObj>* GetFarmRenderObjPool() { return &m_csFarmRenderObjPool; }
	// E 농장시스템 추가 added by hseos 2007.05.08	2007.09.17

//---pk mode
	void AllPlayerPKOff();
	void RefreshSiegeNamePannel();
//
	inline CHero* GetHero()	{	return m_pHero;	}
	inline CHeroPet* GetHeroPet() { return mpHeroPet; }
	inline CHeroPet* GetHeroPetBackup() { return mpHeroPetBackup; }
	inline CBossMonster * GetGetBoss(eBOSSMONSTERTYPE eBossType)	{	return m_pBoss[eBossType];	}

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	// 06. 02. 타겟팅 - 이영준
	BOOL TargetByName(const char* name);

	// 06. 05 HIDE NPC - 이영준
	void HideNpc(WORD UniqueIdx, BOOL bOn);

	CObject* GetObjectByName(const char* name);
	// 070911 LYW --- ObjectManager : Add function to return object id.
	DWORD GetObjectByChatName(const char* name) ;
	// 070911 LYW --- ObjectManager : Add function to return monster id.
	DWORD GetNearMonsterByName(const char* name) ;								// HERO로 부터 가장 가까이 있는 몬스터 아이디를 반환하는 함수.

	CYHHashTable<CObject>& GetObjectTable()
	{
		return m_ObjectTable;
	}

	// 070528 웅주 - 두 물체의 거리를 구함
	float GetDistance( CObject*, CObject* );

	void SetHide( DWORD id, WORD level );
	void SetDetect( DWORD id, WORD level );
};
EXTERNGLOBALTON(CObjectManager);