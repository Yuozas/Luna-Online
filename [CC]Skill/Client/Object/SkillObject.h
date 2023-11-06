#pragma once

#include "Object.h"
#include "../Delay/Delay.h"
#include "../Effect/SkillEffect.h"

/// ��ų�� ����
enum SKILL_STATE
{
	SKILL_STATE_CASTING,
	SKILL_STATE_PROCESS,
	SKILL_STATE_END,
	SKILL_STATE_COOLING,
	SKILL_STATE_DESTROY,
};

/// ������ ����
enum DELAY_KIND
{
	DELAY_KIND_CASTING,
	DELAY_KIND_PROCESS,
	DELAY_KIND_COOLING,
	DELAY_KIND_MAX,
};

class CBattle;
class CObject;
class cSkillInfo;

/// ��ų ���� ����
struct sSKILL_CREATE_INFO
{
	/// ��ų ������Ʈ ID
	DWORD		skillObjectId;
	/// ��ų ����
	WORD		level;

	/// ��ų ����
	cSkillInfo*	pSkillInfo;
	
	/// ������/���
	DWORD		operatorId;
	CObject*	pOperator;
	MAINTARGET	mainTarget;
	
	/// ��ų ��ġ/����
	VECTOR3		pos;
	DIRINDEX	skillDir;

	/// ��Ʋ����
	CBattle*	pBattle;

	DWORD		remainTime;

	/// �ʱ�ȭ �Լ�
	sSKILL_CREATE_INFO()
	{
		skillObjectId = 0;
		level = 0;

		pSkillInfo = NULL;

		pOperator = NULL;

		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		skillDir = 0;
		pBattle = NULL;
	}
};

class cSkillObject	:
	public CObject
{
protected:
	/// ��ų ����
	cSkillInfo*				mpSkillInfo;

	/// ��ų ���� ����
	sSKILL_CREATE_INFO		mSkillObjectInfo;

	/// ��ų ����
	SKILL_STATE				mState;

	/// Ÿ�� ���̺�
	CYHHashTable< CObject >	mTargetTable;
	
	/// ������ ����
//	cDelay					mDelay;

	/// ��ų ����Ʈ
	cSkillEffect			mOperatorEffect;
	cSkillEffect			mSkillEffect;
	cSkillEffect			mTargetEffect;

	cTimeDelay				mTargetDelay;

	void UpdateTarget();

public:
	/// ������/�Ҹ���
	cSkillObject( cSkillInfo* pSkillInfo );
	virtual ~cSkillObject(void);

	virtual void Init( SKILLOBJECT_INFO* pInfo );
	virtual void ReleaseSkillObject();
	virtual void Release();

	virtual void OnReceiveSkillResult( MSG_SKILL_RESULT* pmsg );

	DWORD GetSkillIdx();
	
	virtual void SetPosition(VECTOR3* pPos);
	virtual void GetPosition(VECTOR3* pPos);
	virtual void SetAngle(float AngleRad);
	virtual float GetAngle();

	CObject* GetOperator();
	cSkillInfo* GetSkillInfo();
	sSKILL_CREATE_INFO* GetSkillObjectInfo();

	void SetOperatorState();
	void EndOperatorState();

	void ResetSkillObjectId( DWORD id );

	virtual void Update();

	void Cancel();
};
