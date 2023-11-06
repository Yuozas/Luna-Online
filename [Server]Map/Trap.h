/*
080616 LUJ, 함정 오브젝트. 스킬로 발동된다. 주기적으로 자신을 생성시킨 스킬이 존재하는지 체크하고
			없을 경우 자동 소멸된다
*/
#pragma once
#include "monster.h"

class CTrap : public CMonster
{
public:
	CTrap(void);
	virtual ~CTrap(void);

	// 080616 LUJ, 자신을 생성시킨 스킬이 없으면 자살하기 위해 상속
	virtual void StateProcess();
	// 080616 LUJ, 아군을 공격하지 않기 위해 상속
	virtual BOOL DoAttack( DWORD attackNum );
	// 080616 LUJ, 함정 초기화
	virtual BOOL Init( EObjectKind, DWORD AgentNum, BASEOBJECT_INFO* );
	// 080616 LUJ, 함정이 언제 소멸될지 설정한다
	inline void SetTimeOut( DWORD tick )			{ mTimeOutTick = tick; }
	// 080616 LUJ, 아군을 추가한다
	inline void AddEnemy( DWORD objectIndex )		{ mEnemyObjectIndexSet->insert( objectIndex ); }
	// 080616 LUJ, 주인을 설정한다
	inline void SetOwner( DWORD objectIndex )		{ mOwnerObjectIndex = objectIndex; }
	// 080616 LUJ, 함정은 위치 정보를 소유자에게만 보내준다
	virtual void SetAddMsg( char* pAddMsg, WORD* pMsgLen,DWORD dwReceiverID, BOOL bLogin );
	// 080616 LUJ, 함정은 몬스터를 타겟으로 잡을 수 있다.
	virtual BOOL SetTObject( CObject* );
	// 080616 LUJ, 함정을 재생성할 수 있도록 사망을 소유주에게 통보한다
	virtual void DoDie( CObject* );
	// 080616 LUJ, 아군 여부 판단
	virtual BOOL IsEnemy( DWORD objectIndex ) const	{ return mEnemyObjectIndexSet->end() != mEnemyObjectIndexSet->find( objectIndex ); }
	// 080616 LUJ, 주인 ID 반환
	inline DWORD GetOwnerObjectIndex() const		{ return mOwnerObjectIndex; }

private:
	// 080616 LUJ, 키와 값이 모두 아군 오브젝트 인덱스.
	//
	//	주의:	풀에서 생성되나 생성자, 소멸자가 정상적으로 호출되지 않아, 메모리를 덮어쓰거나 할 수 있다.
	//			STL 컨테이너를 반드시 할당해서 사용하도록 하자
	typedef stdext::hash_set< DWORD >	EnemyObjectIndexSet;
	EnemyObjectIndexSet*				mEnemyObjectIndexSet;
	// 080616 LUJ, 이 시간이 지나면 소멸됨
	DWORD mTimeOutTick;
	// 080616 LUJ, 함정을 소유한 오브젝트 인덱스. 존재하지 않을 경우 소멸한다
	DWORD mOwnerObjectIndex;
};
