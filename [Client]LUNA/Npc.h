#pragma once


#include "Object.h"


#define MAX_NPCQUEST_LIST 50


struct NPC_LIST;


class CNpc : public CObject  
{
	NPC_TOTALINFO m_NpcInfo;
	int		m_QuestList[MAX_NPCQUEST_LIST];
	int		m_QuestCount;
	
	NPC_LIST * m_pSInfo;

	void InitNpc(NPC_TOTALINFO* pTotalInfo);
public:
	CNpc();
	virtual ~CNpc();
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	NPC_LIST * GetSInfo() { return m_pSInfo; }

	void Process();
	WORD GetNpcJob(){return m_NpcInfo.NpcJob;}
	friend class CObjectManager;

	WORD GetNpcKind()		{	return m_NpcInfo.NpcKind;	}
	WORD GetNpcUniqueIdx()		{	return m_NpcInfo.NpcUniqueIdx;	}
	void AddQuestIndex(int Idx);
	int  GetQuestIdx(int Idx)		{	return m_QuestList[Idx];	}
	int  GetQuestCount()			{	return m_QuestCount;	}

	void OnClicked();

	// 070426 LYW --- Npc : Add function to return npc_totalinfo.
	NPC_TOTALINFO* GetNpcTotalInfo() { return &m_NpcInfo ; }
	// 070426 LYW --- Npc : Add function to return npc_list.
	NPC_LIST* GetNpcListInfo() { return m_pSInfo ; }
	
	virtual void SetMotionInState(BYTE State);


	DWORD m_dwLastMotionTime;	// 080321 NYJ --- IDLE모션변경
	int m_nNextMotionTime;
};