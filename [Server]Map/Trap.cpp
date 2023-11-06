#include "StdAfx.h"
#include ".\trap.h"
#include "UserTable.h"
#include "PackedData.h"
#include "AttackManager.h"

CTrap::CTrap() :
mEnemyObjectIndexSet( new EnemyObjectIndexSet )
{}

CTrap::~CTrap()
{
	SAFE_DELETE( mEnemyObjectIndexSet );
}


// 080616 LUJ, 함정 초기화.
//
//		주의:	풀에서 할당되는데, 할당자를 오버라이딩하지 않아 정상적으로 생성자/호출자는 한번만 실행된다
//				가능한 Init(), Release()를 이용하자
BOOL CTrap::Init( EObjectKind kind, DWORD agentIndex, BASEOBJECT_INFO* baseInfo )
{
	// 080616 LUJ, 여기서 함정 멤버를 초기화한다

	// 080616 LUJ, 1HP당 1초씩 계산하여 남아있도록 한다
	mTimeOutTick		= 0;
	mOwnerObjectIndex	= 0;
	mEnemyObjectIndexSet->clear();

	return CMonster::Init( kind, agentIndex, baseInfo );
}

// 080616 LUJ, 제한시간을 초과했거나 소유주가 없으면 자살하기 위해 상속
void CTrap::StateProcess()
{
	// 080616 LUJ, 이렇게 처리 안해주면, 사망 처리 시간이 계속 지연되어 죽지 않게 된
	if( eObjectState_Die == GetState() )
	{
		return;
	}

	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( mOwnerObjectIndex );

	// 080616 LUJ, 주인이 없으면 소멸시킨다
	if( !	player ||
			player->GetObjectKind() != eObjectKind_Player )
	{
		Die( 0 );
	}
	// 080616 LUJ, 타임아웃되어 소멸됨
	else if( mTimeOutTick < gCurTime )
	{
		Die( 0 );

		// 080616 LUJ, 이 메시지를 보내야 자연스럽게 사라진다
		MSG_DWORD2 message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_USERCONN;
		message.Protocol	= MP_USERCONN_MONSTER_DIE;
		message.dwObjectID	= mOwnerObjectIndex;
		message.dwData1		= mOwnerObjectIndex;
		message.dwData2		= GetID();

		player->SendMsg( &message, sizeof( message ) );
		player->RemoveTrap( GetMonsterKind() );
	}

	CMonster::StateProcess();
}

// 080616 LUJ, 함정은 위치 정보를 소유자에게만 보내준다
void CTrap::SetAddMsg( char* pAddMsg, WORD* pMsgLen,DWORD dwReceiverID, BOOL bLogin )
{
	// 080616 LUJ, 트랩은 직접 전송하므로, 함수 호출한 측에서 보낼 수 없도록 한다
	*pMsgLen = 0;

	CObject* object = g_pUserTable->FindUser( mOwnerObjectIndex );

	// 080616 LUJ, CGeneralGridTable::ChangeGrid()에서 pMsgLen이 0이상이면 전체에게 브로드캐스팅한다.
	//				함정은 주인에게만 표시될 것이므로, 이 값을 0으로 한다
	if( ! object )
	{
		return;
	}

	WORD					length = 0;
	SEND_MONSTER_TOTALINFO	message;
	ZeroMemory( &message, sizeof( message ) );
	CMonster::SetAddMsg( ( char* )&message, &length, dwReceiverID, bLogin );

	object->SendMsg( &message, message.GetMsgLength() );
}

// 080616 LUJ, 아군을 공격하지 않기 위해 상속
BOOL CTrap::DoAttack( DWORD attackNum )
{
	if( !	m_pTObject ||
		// 080616 LUJ, 공격 스킬 오브젝트가 정상적으로 생성되면 항상 참을 반환한다
		!	CMonster::DoAttack( attackNum ) )
	{
		return FALSE;
	}

	SetTimeOut( 0 );
	return TRUE;
}

// 080616 LUJ, 함정은 몬스터를 타겟으로 잡을 수 있다.
BOOL CTrap::SetTObject( CObject* object )
{
	if( !	object ||
			object->GetID() == mOwnerObjectIndex )
	{
		m_pTObject = 0;
		return FALSE;
	}

	m_pTObject = object;	
	return TRUE;
}

// 080616 LUJ, 함정을 재생성할 수 있도록 사망을 소유주에게 통보한다
void CTrap::DoDie( CObject* object )
{
	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( mOwnerObjectIndex );

	if( player &&
		player->GetObjectKind() == eObjectKind_Player )
	{
		const BASE_MONSTER_LIST* monsterList = GetSMonsterList();

		player->RemoveTrap( monsterList ? monsterList->MonsterKind : 0 );
	}

	// 080616 LUJ, 직접 호출해줘야 한다
	CMonster::DoDie( object );
}