// BossMonsterInfo.h: interface for the CBossMonsterInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSMONSTERINFO_H__ECED8E50_6782_4877_9C2D_8317902C04F8__INCLUDED_)
#define AFX_BOSSMONSTERINFO_H__ECED8E50_6782_4877_9C2D_8317902C04F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BOSSATTACK_NUM 4
#define MAX_BOSSSTATE_NUM 5

using namespace std;

typedef DWORD ATTACKIDX;

struct BOSSEVENTSTATE
{
	BYTE Condition;
	WORD ConditionValue;
	BYTE Count;
	BYTE Action;
	DWORD ActionValue;
};

struct NEXTATTACKINFO
{
	struct NEXTATTACK
	{
		NEXTATTACK(int r,ATTACKIDX kind)
			:	Rate(r), NextAttackKind(kind) {}
		int Rate;
		ATTACKIDX NextAttackKind;
	};

	NEXTATTACKINFO()
	{
		MaxRate = 0;
	}
	~NEXTATTACKINFO()
	{
		vector<NEXTATTACK*>::iterator iter;
		for(iter = vecNextAttack.begin(); iter != vecNextAttack.end(); ++iter)
		{
			delete *iter;		//free 
		}
		vecNextAttack.clear();
	}

	int MaxRate;
	vector<NEXTATTACK*> vecNextAttack;

	void AddNextAttack(int Rate,ATTACKIDX NextAttackKind)
	{
		MaxRate += Rate;
		vecNextAttack.push_back(new NEXTATTACK(Rate,NextAttackKind));	//alloc
	}

	ATTACKIDX GetNextAttackKind()
	{
		int rValue = rand() % MaxRate;
		vector<NEXTATTACK*>::iterator iter;
		iter = vecNextAttack.begin();
		for(; iter != vecNextAttack.end(); ++iter)
		{
			NEXTATTACK* pNextAttack = *iter;
			if(rValue <= pNextAttack->Rate)
				return pNextAttack->NextAttackKind;

			rValue -= pNextAttack->Rate;
		}

		ASSERT(0);		//���� ���� �ɸ��� �ȵ�.
		return -1;
	}
};
	
struct BOSSUNGIJOSIK
{
	DWORD dwStartTime;
	DWORD dwDelayTime;
	float fLifeRate;
};

class CBossMonsterInfo  
{
	WORD m_MonsterKind;
	DWORD m_RegenMinTime;
	DWORD m_RegenMaxTime;
	DWORD m_FirstAttackIdx;
	WORD m_DieGroupID;
	BYTE m_MaxEventStateNum;
	float m_ChangeTargetMinRange;
	float m_ChangeTargetMaxRange;

	map<ATTACKIDX,NEXTATTACKINFO*> m_mapNextAttackInfo;

	WORD m_SummonFileNum;
	BOSSEVENTSTATE * m_EventState;
	BOSSUNGIJOSIK m_BossUngijosik;
	
	/// 06. 08. 2�� ���� - �̿���
	/// Ÿ���� ���
	/// 0: ���� ��� 1: �� ���ݽ� �������� Ÿ�� ����
	WORD m_TargetingType;

	/// 06. 09. 2�� ���� - �̿���
	/// �Ҹ�ð�
	DWORD m_DeleteTime;
public:
	CBossMonsterInfo();
	virtual ~CBossMonsterInfo();
	
	void SetMonsterKind(WORD MonsterKind);
	void SetRegenDelayTime(DWORD minTime, DWORD maxTime);
	void SetMaxStateNum(BYTE num);
	void AddAttackInfo(DWORD CurAttackIdx, NEXTATTACKINFO* pNextAttackInfo);

	WORD GetMonsterKind();
	BOSSEVENTSTATE* GetEventStateInfo();
	
		
	BYTE GetMaxEventStateNum();
	ATTACKIDX GetNextAttackKind(DWORD CurAttackIdx);

	void Release();
	BOOL IsAttackEmpty();
	void SetFirstAttackIdx(DWORD AttackIdx);
	ATTACKIDX GetFirstAttackIdx();

	void SetSummonFileNum(WORD num);
	WORD GetSummonFileNum();

	void SetDieGroupID(WORD GroupID);
	WORD GetDieGroupID();

	void SetBossUngijosik(DWORD StartTime, DWORD DelayTime, float LifeRate);
	BOSSUNGIJOSIK* GetUngiInfo();

	DWORD CalcRegenDelayTime();

	void SetChangeTargetRange(float MinRange, float MaxRange);
	float GetChangeTargetMinRange();
	float GetChangeTargetMaxRange();

	void SetTargetingType(WORD type) { m_TargetingType = type; }
	WORD GetTargetingType() { return m_TargetingType; }

	void SetDeleteTime( DWORD time ) { m_DeleteTime = time; }
	DWORD GetDeleteTime() { return m_DeleteTime; }
};

#endif // !defined(AFX_BOSSMONSTERINFO_H__ECED8E50_6782_4877_9C2D_8317902C04F8__INCLUDED_)
