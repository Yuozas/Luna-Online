// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)
#define AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
//#include "StateNPC.h"
//#include "GameResourceManager.h"
//#include "AIDefine.h"
#include "AIParam.h"
#include "Distributer.h"


//---KES Aggro 070918
	struct AGGRO
	{
		DWORD	dwObjectID;
		int		nAggro;
	};
//-------------------

class CAIBase;
class CPlayer;

class CMonster : public CObject
{
/////
	DWORD				m_DropItemId;		// ?Ì°ª?Ì 0?Ì ¾Æ´Ï¸é ?Ì ¾Æ?ÌÅÛ?» µå¶øÇÏ°í 0?Ì¸é ¿ø·¡´ë·Î Ã³¸®ÇÑ´Ù.
//KES Ãß°¡
	DWORD				m_dwDropItemRatio;	//m_DropItemId?Ç µå¶øÈ®·ü(?Ó½Ã·Î ¹éºÐ?²)
//
	DWORD				m_SubID;// Ã¤³Î·Î ³ª´µ±â ?ü?Ç ID
	WORD				m_RegenNum;
	MONSTER_TOTALINFO	m_MonsterInfo;
//	CStateNPC*			m_MonsterState;
	CDistributer		m_Distributer;
	VECTOR3*			m_TargetObjectPos;

	monster_stats m_mon_stats;
	
	//YH
	BOOL m_bEventMob;
	int m_SuryunGroup;

	//KES
	//�������� �ڽ��� ������ �÷��̾ ����Ѵ�.
	CObject* m_pLastAttackPlayer;
	
	cDelay*		mpBattleStateDelay;

	DWORD m_DieTime;
protected:
	DWORD m_KillerPlayer;		// Á×?Î »ç¶÷ ¾Æ?Ìµð, (°æÇèÄ¡ ¾Æ?ÌÅÆ µ· µî?» º¸³»¸é 0?¸·Î ¼ÂÆÃÇÑ´Ù.)

	// AI °ü·Ã
	//CStateMachine * m_pState;
	//CAIBase * m_pAIBase;
	
	friend class CStateMachinen;
	friend class CBattle;

protected:
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife);
	BASE_MONSTER_LIST * m_pSInfo;
	CObject	*			m_pTObject;

//---KES Aggro 070918
	int								m_nMaxAggro;			//������� �ְ� ��׷�.
	CYHHashTable<AGGRO>				m_htAggroTable;			//AGGRO ��׷μ�ġ ����Ʈ
	static CMemoryPoolTempl<AGGRO>* m_mpAggro;				//new delete �ּ�ȭ�� ���� �޸𸮸� �̸� ��´�.


public:
	static void InitAggroMemoryPool();
	static void ReleaseAggroMemoryPool();

	void CalcAggro( CObject* pAttacker, int nAggroAdd );
	void RemoveFromAggro( DWORD dwObjectID );
	void RemoveAllAggro();
	AGGRO* FindMaxAggro();
//-------------------

//---KES 071120
protected:
	BOOL	m_bNoCheckCollision;
public:
	void SetNoCheckCollision( BOOL bNoCheck ) { m_bNoCheckCollision = bNoCheck; }
	BOOL IsNoCheckCollision() { return m_bNoCheckCollision; }
//--------------

public:
	void SetSuryunGroup(int group)		{	m_SuryunGroup = group;	}
	int GetSuryunGroup()				{	return m_SuryunGroup;	}
	void SetEventMob(BOOL bEventMob)	{	m_bEventMob = bEventMob;	}
	void SetDropItemID( DWORD DropItemID, DWORD dwDropRatio = 100 )	{ m_DropItemId = DropItemID; m_dwDropItemRatio = dwDropRatio; }
	
	CMonster();
	virtual ~CMonster();

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();
	
	void InitMonster(MONSTER_TOTALINFO* pTotalInfo);
	void GetMonsterTotalInfo(MONSTER_TOTALINFO* pRtInfo);
	
	virtual void GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList);
	BASE_MONSTER_LIST * GetSMonsterList(){ return m_pSInfo;	}
	void SetSMonsterList(BASE_MONSTER_LIST * inf){ m_pSInfo = inf;	}
	//////////////////////////////////////////////////////////////////////////
	// ±âº» Á¤º¸ ¾ò¾î¿?´Â ÇÔ¼ö
	inline WORD GetMonsterKind()	{	return m_MonsterInfo.MonsterKind;	}
	

	// µ¥¹ÌÁö ?ÔÈù °´Ã¼ °ü¸® //////////////////////////////
	void AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage);
	void Drop();
	///////////////////////////////////////////////////////////////////////
	DWORD GetCurAttackIndex();

	// 080616 LUJ, ���� �� ���� ���ʹ� ��ġ ������ ������ ������Ʈ�ϱ� virtual�� ������
	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	
//	CStateNPC* GetMonsterState(){return m_MonsterState;}

	//void SetTargetObjectPos(VECTOR3* pPos){m_TargetObjectPos = pPos;}
	//VECTOR3* GetTargetObjectPos(){return m_TargetObjectPos;}

	//////////////////////////////////////////////////////////////////////////
	// Monster ½Ã°£º¯¼ö¼ÂÆÃ
	/*
	void SetCurSearchTime(DWORD Time){m_dwCurSearchTime = Time;}
		void SetTmpSearchTime(DWORD Time){m_dwTmpSearchTime = Time;}
		void SetLastSearchTime(DWORD Time){m_dwLastSearchTime = Time;}
		void SetFirstSearch(BOOL flag){m_bFirstSearch = flag;}
		DWORD GetCurSearchTime(){return m_dwCurSearchTime;}
		DWORD GetTmpSearchTime(){return m_dwTmpSearchTime;}
		DWORD GetLastSearchTime(){return m_dwLastSearchTime;}
		DWORD GetFirstSearch(){return m_bFirstSearch;}*/


	///
public:
	CAIParam			m_aiParam;
	CStateParam			m_stateParam;
	void SetSubID(DWORD id){ m_SubID = id;	}
	DWORD GetSubID() { return m_SubID; }
	// 080616 LUJ, ������ Ÿ�� ó���� ������ �ϱ� ���� ���� �Լ��� ó���ϰ�, ���ڵ� CObject*�� ��
	virtual BOOL SetTObject( CObject* );
	CObject *	GetTObject()						{	return m_pTObject;		}
	virtual DWORD GetGravity()
	{
		return m_pSInfo->Gravity;
	}

	//void MoveToPosition(VECTOR3* pPos);
	//void WalkAround(int domainRange);
	void MoveStop();

	virtual void			OnStop();
	virtual void			OnMove( VECTOR3 * pPos );

	virtual void			DoStand();
	virtual void			DoRest(BOOL bStart);
	virtual BOOL			DoAttack( DWORD attackNum );
	virtual CObject *		DoSearch();
	virtual BOOL			DoWalkAround();
	virtual void			DoRunaway();
	virtual BOOL			DoPursuit(CObject * pTObject=NULL);
	virtual CObject	*		OnCollisionObject();
	virtual CMonster *		DoFriendSearch(DWORD Range);
	/*
	void SetPeaceRestTime(DWORD dwMaxTime);
		void SetAttackRestTime(DWORD dwMaxTime);
	BOOL Rest(DWORD dwTickTime);		// ´Ù ½¬¾ú?¸¸é TRUE ¸®ÅÏ
	*/
	
	void AddChat(char * str);
	//SW050902
	void AddSpeech(DWORD SpeechType, DWORD SpeechIdx);

	monster_stats * GetMonsterStats() { return &m_mon_stats; }

	virtual WORD GetMonsterGroupNum()
	{
		return m_MonsterInfo.Group;
	}
		
	void OnStartObjectState(BYTE State,DWORD dwParam);
	virtual void OnEndObjectState(BYTE State);

	// RegenNum?» ?ú?å/°¡Á®¿?±â
	void SetRegenNum(WORD RegenNum){m_RegenNum = RegenNum;}
	WORD GetRegenNum(){return m_RegenNum;}

	virtual float GetRadius() const; 
	
	virtual void StateProcess();
	
	int GetObjectTileSize();
	
	void DistributePerDamage();
	// �ʵ庸�� - 05.12 �̿���
	// �ʵ庸���� ������ �й� �Լ� ȣ�� ��
	void DistributeItemPerDamage();
	void DistributeDamageInit();
	void DistributeDeleteDamagedPlayer(DWORD CharacterID);




	// AI ¸â¹ö
//	virtual void ProcessAI();
//	void InitAIParam() { memset(&m_AIBase, 0, sizeof(m_AIBase)); }
//	CAIBase * GetAIBase() { return m_pAIBase;	}
//	CStateMachine * GetStateMachine() { return m_pState; }
	
//	virtual void AddStatus(CStatus* pStatus);
//	virtual void RemoveStatus(CStatus* pStatus);
	
	//////////////////////////////////////////////////////////////////////////
	// ¿?¹ö¶ó?Ìµù ÇÔ¼öµé
	virtual void DoDie(CObject* pAttacker);
	
	//////////////////////////////////////////////////////////////////////////
	// Á¤º¸ ¾ò¾î¿?´Â ÇÔ¼öµé
	virtual LEVELTYPE GetLevel(){ return m_pSInfo->Level; }
	virtual void SetLevel(LEVELTYPE level)	{}

	virtual DWORD GetLife()									{ return m_MonsterInfo.Life; }
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	
	virtual DWORD DoGetMaxLife()							{ return m_pSInfo->Life; }
	virtual void SetMaxLife(DWORD life)						{}

	virtual DWORD GetMana(){ return 0; }
	virtual void SetMana(DWORD val,BOOL bSendMsg = TRUE)	{}
	virtual DWORD DoGetMaxMana(){ return 0; }
	virtual void SetMaxMana(DWORD val)	{}
	virtual DWORD DoGetPhyDefense(){ return GetMonsterStats()->PhysicalDefense; }
	virtual float DoGetAttDefense(WORD Attrib){ return GetMonsterStats()->AttributeResist.GetElement_Val(Attrib); }
	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();

	virtual float DoGetMoveSpeed();

	CObject* GetLastAttackPlayer() { return m_pLastAttackPlayer; }
	void SetLastAttackPlayer( CObject* pLastAttackPlayer ) { m_pLastAttackPlayer = pLastAttackPlayer; }

	virtual	void SetObjectBattleState( BYTE state );

	// 080122 LYW --- Monster : ������ ų���� ���̵� ��ȯ�ϴ� �Լ� �߰�.
	DWORD GetLastKillerID() { return m_KillerPlayer ; }

	void SetForeAttack(BOOL bForeAttack) {m_pSInfo->bForeAttack = bForeAttack;}

	void SetDieTime(DWORD time);
	virtual void Process();
};

#endif // !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)
