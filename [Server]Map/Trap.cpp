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


// 080616 LUJ, ���� �ʱ�ȭ.
//
//		����:	Ǯ���� �Ҵ�Ǵµ�, �Ҵ��ڸ� �������̵����� �ʾ� ���������� ������/ȣ���ڴ� �ѹ��� ����ȴ�
//				������ Init(), Release()�� �̿�����
BOOL CTrap::Init( EObjectKind kind, DWORD agentIndex, BASEOBJECT_INFO* baseInfo )
{
	// 080616 LUJ, ���⼭ ���� ����� �ʱ�ȭ�Ѵ�

	// 080616 LUJ, 1HP�� 1�ʾ� ����Ͽ� �����ֵ��� �Ѵ�
	mTimeOutTick		= 0;
	mOwnerObjectIndex	= 0;
	mEnemyObjectIndexSet->clear();

	return CMonster::Init( kind, agentIndex, baseInfo );
}

// 080616 LUJ, ���ѽð��� �ʰ��߰ų� �����ְ� ������ �ڻ��ϱ� ���� ���
void CTrap::StateProcess()
{
	// 080616 LUJ, �̷��� ó�� �����ָ�, ��� ó�� �ð��� ��� �����Ǿ� ���� �ʰ� ��
	if( eObjectState_Die == GetState() )
	{
		return;
	}

	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( mOwnerObjectIndex );

	// 080616 LUJ, ������ ������ �Ҹ��Ų��
	if( !	player ||
			player->GetObjectKind() != eObjectKind_Player )
	{
		Die( 0 );
	}
	// 080616 LUJ, Ÿ�Ӿƿ��Ǿ� �Ҹ��
	else if( mTimeOutTick < gCurTime )
	{
		Die( 0 );

		// 080616 LUJ, �� �޽����� ������ �ڿ������� �������
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

// 080616 LUJ, ������ ��ġ ������ �����ڿ��Ը� �����ش�
void CTrap::SetAddMsg( char* pAddMsg, WORD* pMsgLen,DWORD dwReceiverID, BOOL bLogin )
{
	// 080616 LUJ, Ʈ���� ���� �����ϹǷ�, �Լ� ȣ���� ������ ���� �� ������ �Ѵ�
	*pMsgLen = 0;

	CObject* object = g_pUserTable->FindUser( mOwnerObjectIndex );

	// 080616 LUJ, CGeneralGridTable::ChangeGrid()���� pMsgLen�� 0�̻��̸� ��ü���� ��ε�ĳ�����Ѵ�.
	//				������ ���ο��Ը� ǥ�õ� ���̹Ƿ�, �� ���� 0���� �Ѵ�
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

// 080616 LUJ, �Ʊ��� �������� �ʱ� ���� ���
BOOL CTrap::DoAttack( DWORD attackNum )
{
	if( !	m_pTObject ||
		// 080616 LUJ, ���� ��ų ������Ʈ�� ���������� �����Ǹ� �׻� ���� ��ȯ�Ѵ�
		!	CMonster::DoAttack( attackNum ) )
	{
		return FALSE;
	}

	SetTimeOut( 0 );
	return TRUE;
}

// 080616 LUJ, ������ ���͸� Ÿ������ ���� �� �ִ�.
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

// 080616 LUJ, ������ ������� �� �ֵ��� ����� �����ֿ��� �뺸�Ѵ�
void CTrap::DoDie( CObject* object )
{
	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( mOwnerObjectIndex );

	if( player &&
		player->GetObjectKind() == eObjectKind_Player )
	{
		const BASE_MONSTER_LIST* monsterList = GetSMonsterList();

		player->RemoveTrap( monsterList ? monsterList->MonsterKind : 0 );
	}

	// 080616 LUJ, ���� ȣ������� �Ѵ�
	CMonster::DoDie( object );
}