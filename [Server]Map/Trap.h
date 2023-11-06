/*
080616 LUJ, ���� ������Ʈ. ��ų�� �ߵ��ȴ�. �ֱ������� �ڽ��� ������Ų ��ų�� �����ϴ��� üũ�ϰ�
			���� ��� �ڵ� �Ҹ�ȴ�
*/
#pragma once
#include "monster.h"

class CTrap : public CMonster
{
public:
	CTrap(void);
	virtual ~CTrap(void);

	// 080616 LUJ, �ڽ��� ������Ų ��ų�� ������ �ڻ��ϱ� ���� ���
	virtual void StateProcess();
	// 080616 LUJ, �Ʊ��� �������� �ʱ� ���� ���
	virtual BOOL DoAttack( DWORD attackNum );
	// 080616 LUJ, ���� �ʱ�ȭ
	virtual BOOL Init( EObjectKind, DWORD AgentNum, BASEOBJECT_INFO* );
	// 080616 LUJ, ������ ���� �Ҹ���� �����Ѵ�
	inline void SetTimeOut( DWORD tick )			{ mTimeOutTick = tick; }
	// 080616 LUJ, �Ʊ��� �߰��Ѵ�
	inline void AddEnemy( DWORD objectIndex )		{ mEnemyObjectIndexSet->insert( objectIndex ); }
	// 080616 LUJ, ������ �����Ѵ�
	inline void SetOwner( DWORD objectIndex )		{ mOwnerObjectIndex = objectIndex; }
	// 080616 LUJ, ������ ��ġ ������ �����ڿ��Ը� �����ش�
	virtual void SetAddMsg( char* pAddMsg, WORD* pMsgLen,DWORD dwReceiverID, BOOL bLogin );
	// 080616 LUJ, ������ ���͸� Ÿ������ ���� �� �ִ�.
	virtual BOOL SetTObject( CObject* );
	// 080616 LUJ, ������ ������� �� �ֵ��� ����� �����ֿ��� �뺸�Ѵ�
	virtual void DoDie( CObject* );
	// 080616 LUJ, �Ʊ� ���� �Ǵ�
	virtual BOOL IsEnemy( DWORD objectIndex ) const	{ return mEnemyObjectIndexSet->end() != mEnemyObjectIndexSet->find( objectIndex ); }
	// 080616 LUJ, ���� ID ��ȯ
	inline DWORD GetOwnerObjectIndex() const		{ return mOwnerObjectIndex; }

private:
	// 080616 LUJ, Ű�� ���� ��� �Ʊ� ������Ʈ �ε���.
	//
	//	����:	Ǯ���� �����ǳ� ������, �Ҹ��ڰ� ���������� ȣ����� �ʾ�, �޸𸮸� ����ų� �� �� �ִ�.
	//			STL �����̳ʸ� �ݵ�� �Ҵ��ؼ� ����ϵ��� ����
	typedef stdext::hash_set< DWORD >	EnemyObjectIndexSet;
	EnemyObjectIndexSet*				mEnemyObjectIndexSet;
	// 080616 LUJ, �� �ð��� ������ �Ҹ��
	DWORD mTimeOutTick;
	// 080616 LUJ, ������ ������ ������Ʈ �ε���. �������� ���� ��� �Ҹ��Ѵ�
	DWORD mOwnerObjectIndex;
};
