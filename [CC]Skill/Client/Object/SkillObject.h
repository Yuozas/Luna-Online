#pragma once

#include "Object.h"
#include "../Delay/Delay.h"
#include "../Effect/SkillEffect.h"

/// 스킬의 상태
enum SKILL_STATE
{
	SKILL_STATE_CASTING,
	SKILL_STATE_PROCESS,
	SKILL_STATE_END,
	SKILL_STATE_COOLING,
	SKILL_STATE_DESTROY,
};

/// 딜레이 종류
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

/// 스킬 생성 정보
struct sSKILL_CREATE_INFO
{
	/// 스킬 오브젝트 ID
	DWORD		skillObjectId;
	/// 스킬 레벨
	WORD		level;

	/// 스킬 정보
	cSkillInfo*	pSkillInfo;
	
	/// 시전자/대상
	DWORD		operatorId;
	CObject*	pOperator;
	MAINTARGET	mainTarget;
	
	/// 스킬 위치/방향
	VECTOR3		pos;
	DIRINDEX	skillDir;

	/// 배틀정보
	CBattle*	pBattle;

	DWORD		remainTime;

	/// 초기화 함수
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
	/// 스킬 정보
	cSkillInfo*				mpSkillInfo;

	/// 스킬 생성 정보
	sSKILL_CREATE_INFO		mSkillObjectInfo;

	/// 스킬 상태
	SKILL_STATE				mState;

	/// 타겟 테이블
	CYHHashTable< CObject >	mTargetTable;
	
	/// 딜레이 관리
//	cDelay					mDelay;

	/// 스킬 이펙트
	cSkillEffect			mOperatorEffect;
	cSkillEffect			mSkillEffect;
	cSkillEffect			mTargetEffect;

	cTimeDelay				mTargetDelay;

	void UpdateTarget();

public:
	/// 생성자/소멸자
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
