#pragma once
#include "object.h"

/// 스킬의 상태
enum SKILL_STATE
{
	SKILL_STATE_CASTING,
	SKILL_STATE_PROCESS,
	SKILL_STATE_ON,
	SKILL_STATE_END,
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
class cTarget;
class cSkillUnit;
class cSkillInfo;
class cDelay;

/// 스킬 생성 정보
struct sSKILL_CREATE_INFO
{
	/// 스킬 오브젝트 ID
	DWORD		skillObjectId;
	/// 스킬 레벨
	WORD		level;

	/// 시전자/대상
	DWORD		operatorId;
//	CObject*	pOperator;
	MAINTARGET	mainTarget;
	
	/// 스킬 위치/방향
	VECTOR3		pos;
	DIRINDEX	skillDir;

	/// 배틀정보
	//DWORD		battle;
	//CBattle*	pBattle;

	DWORD		remainTime;

	WORD		count;

	/// 초기화 함수
	sSKILL_CREATE_INFO()
	{
		skillObjectId = 0;
		level = 0;

//		pOperator = NULL;

		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		skillDir = 0;
		//pBattle = NULL;
		remainTime = 0;
		count = 0;
	}
};

class cSkillObject :
	public CObject
{
	// 080602 LUJ, 스킬 오브젝트 타입
public:	
	enum Type
	{
		TypeNone,
		TypeActive,
		TypeBuff,
		TypeOnOff,
	};

	inline Type GetType() const { return mType; }

protected:
	Type mType;


protected:
	/// 스킬 정보
	cSkillInfo*			mpSkillInfo;

	/// 스킬 생성 정보
	sSKILL_CREATE_INFO	mSkillObjectInfo;

	/// 스킬 상태
	SKILL_STATE			mState;

	/// 타겟
	cTarget*			mpTarget;
	

public:
	/// 생성자 / 소멸자
	cSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cSkillObject( void );

	/// 접근자
	DWORD				GetSkillIdx();
	WORD				GetSkillKind();
	WORD				GetSkillLevel();
	CObject*			GetOperator();
	cSkillInfo*			GetSkillInfo();
	void				GetPosition( VECTOR3* pPos );
	DIRINDEX			GetDirectionIndex();
	

	/// 초기화/해제
	virtual void		Init( sSKILL_CREATE_INFO* pInfo );
	virtual void		Release() = 0;

	/// 프로세스
	virtual SKILL_STATE	Update();
	virtual void EndState();

	/// 타겟 업데이트
	virtual void		UpdateTargetList( MAINTARGET* pTarget );
	
	/// Grid 시스템에 추가/삭제
	virtual void		SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void		SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

	void				SetMultiTarget( CTargetList* pTList , WORD AreaData);

	SKILL_STATE			GetSkillState() { return mState; }
	virtual	void		SetEndState();

};
