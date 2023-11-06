#include "StdAfx.h"
#include "GuildScore.h"
#include "UserTable.h"
#include "Object.h"


CGuildScore::CGuildScore( DWORD unitTick, float tickScore ) :
mUnitTick( unitTick ),
mTickScore( tickScore ),
mUnitTickScore( tickScore * unitTick )
{
	ZeroMemory( &mData, sizeof( mData ) );
	
	Reset( GetTickCount() );

	//{
	//	const DWORD currentTick = GetTickCount();

	//	mScore.mScore		= 0.0f;
	//	mScore.mBeginTick	= currentTick;
	//	mScore.mEndTick		= currentTick + mUnitTick;
	//}
}


void CGuildScore::AddPlayer( DWORD playerIndex )
{
	//const DWORD currentTick = GetTickCount();

	//mScore.mScore		= 0;
	//mScore.mBeginTick	= currentTick;
	//mScore.mEndTick		= mScore.mBeginTick + mUnitTick;

	ASSERT( mScore.mPrepareMember.end() == mScore.mPrepareMember.find( playerIndex ) );

	//mScore.mPrepareMember[ playerIndex ] = currentTick;

	mScore.mPrepareMember[ playerIndex ] = GetTickCount();
}


float CGuildScore::RemovePlayer( DWORD playerIndex )
{
	Score::ReadyMember&		readyMember		= mScore.mReadyMember;
	Score::PrepareMember&	prepareMember	= mScore.mPrepareMember;

	DWORD beginTick;
	
	// ���� ��� ��Ͽ� �ִ� ��� ���� ������ �ð��� �����Ͽ� ����Ѵ�	
	if( prepareMember.end() != prepareMember.find( playerIndex ) )
	{
		beginTick = prepareMember[ playerIndex ];

		prepareMember.erase( playerIndex );
	}
	// �ϰ� ó�� ����� ���� ��� �ϰ�ó���� �ð��� ����Ѵ�.
	else if( readyMember.end() != readyMember.find( playerIndex ) )
	{
		beginTick = mScore.mBeginTick;

		readyMember.erase( playerIndex );
	}
	else
	{
		ASSERT( 0 && "It means class has fault in AddMemeber()" );
		return 0.0f;
	}

	DWORD		tick;
	const DWORD currentTick = GetTickCount();

	// ���� �÷ε� ��� �ִ� ������ �����Ͽ� ���̸� ����ؾ� �Ѵ�.
	if( currentTick < beginTick )
	{
		tick = UINT_MAX - beginTick + currentTick;
	}
	else
	{
		tick = currentTick - beginTick;
	}

	const float score = float( tick ) / mUnitTick;
	ASSERT( 0 < score );

	return score;
}


BOOL CGuildScore::IsEnableScore( DWORD currentTick ) const
{
	// ƽī��Ʈ�� �����÷ε� ������ ��� ���۰����ٴ� �۰� �������� ƽī��Ʈ�� Ŭ ��� ������ ����� �� �ִ�
	if( mScore.mEndTick < mScore.mBeginTick )
	{
		if(	mScore.mBeginTick > currentTick	&&
			mScore.mEndTick	< currentTick )
		{			
			return TRUE;
		}
	}
	// �����÷ε� ���°� �ƴϸ� �������� ���� ƽī��Ʈ�� ũ�� ����� �� �ִ�.
	else if( mScore.mEndTick < currentTick )
	{
		return TRUE;
	}

	return FALSE;
}


float CGuildScore::GetScore( DWORD currentTick ) const
{
	// ���⿡�� ���, Ż��, �߹� ������ ���� ����� ������ ������ �ִ�.
	float score = mScore.mScore;

	// ������� ���� ������ �ο�
	{
		score += mScore.mReadyMember.size() * mUnitTickScore;
	}

	// 1�ð� �� ����/�α����� ��� ���� ���
	for(	Score::PrepareMember::const_iterator it = mScore.mPrepareMember.begin();
			mScore.mPrepareMember.end() != it;
			++it )
	{
		const DWORD addedTick	= it->second;
		DWORD		tick;

		/*
		���� ������ ���� �� ���� ���̸� ���ϴ� ����� �� ������ �ִ�.

		(1)		  A    B			(2)	   B   A
				+----+----+				+----+----+

		A ������ B�� ��ϵȴٰ� �غ���. (1)�� ���� ���� ���ȴ�.
		(2)�� ���� �ִ밪���� A�� �� ���̿� B�� ���ؼ� ���̸� �����Ѵ�.
		*/
		if( currentTick < addedTick )
		{
			tick = UINT_MAX - addedTick + currentTick;
		}
		else
		{
			tick = currentTick - addedTick;
		}

		const float memberScore = mTickScore * tick;
		ASSERT( 0 < memberScore );

		score += memberScore;
	}

	return score;
}


void CGuildScore::Reset( DWORD currentTick )
{
	// ���� ó�� ����� �ϰ� ó�� ����� �̵���Ų��.
	for(	Score::PrepareMember::const_iterator it = mScore.mPrepareMember.begin();
			mScore.mPrepareMember.end() != it;
			++it )
	{
		const DWORD playerIndex = it->first;

		mScore.mReadyMember.insert( playerIndex );
	}

	// �ʱ�ȭ: �̺�Ʈ�� ����� ������ �ʱ�ȭ�Ѵ�.
	// �ð� ������ 1�ð� �ڷ� �����س���, ����Ʈ �� �ڿ� ���´�. ��? 1�ð��� ������ �ٽ� ����ؾ��ϴϱ�..
	{
		mScore.mScore		= 0;
		mScore.mBeginTick	= currentTick;
		mScore.mEndTick		= currentTick + mUnitTick;

		mScore.mPrepareMember.clear();
	}
}


void CGuildScore::AddScore( float score )
{
	mScore.mScore += score;
}


DWORD CGuildScore::GetPlayerSize() const
{
	return mScore.mPrepareMember.size() + mScore.mReadyMember.size();
}


//void CGuildScore::GetData( MSG_GUILD_MEMBER_LOGIN& message ) const
//{
//	// ��ȿ�� �˻�
//	{
//		const DWORD size = sizeof( message.mPlayerIndex ) / sizeof( DWORD );
//
//		if( size < GetSize() )
//		{
//			message.mSize	= 0;
//
//			ASSERT( 0 );
//			return;
//		}
//	}
//
//	message.mSize	= GetSize();
//	DWORD* data		= message.mPlayerIndex;
//	
//	// ���� ó�� ��� ������ ����
//	for(	Score::PrepareMember::const_iterator it = mScore.mPrepareMember.begin();
//			mScore.mPrepareMember.end() != it;
//			++it, ++data )
//	{
//		const DWORD playerIndex = it->first;
//
//		*data = playerIndex;
//	}
//
//	// �ϰ� ó�� ��� ������ ����
//	// ���� ó�� ��� ������ ����
//	for(	Score::ReadyMember::const_iterator it = mScore.mReadyMember.begin();
//			mScore.mReadyMember.end() != it;
//			++it, ++data )
//	{
//		const DWORD playerIndex = *it;
//
//		*data = playerIndex;
//	}
//}


void CGuildScore::SetData(const MSG_GUILD_DATA& message, DWORD masterIndex )
{
	mData.mLevel		= message.mLevel;
	mData.mMasterIndex	= masterIndex;	
}


void CGuildScore::SetLevel( DWORD level )
{
	mData.mLevel = level;
}


DWORD CGuildScore::GetLevel() const
{
	return mData.mLevel;
}


DWORD CGuildScore::GetMasterIndex() const
{
	return mData.mMasterIndex;
}


void CGuildScore::Send( MSGBASE* message, DWORD size ) const
{
	// ���� ó�� ��� ������ ����
	for(	Score::PrepareMember::const_iterator it = mScore.mPrepareMember.begin();
			mScore.mPrepareMember.end() != it;
			++it )
	{
		const DWORD playerIndex = it->first;
		CObject*	object		= g_pUserTable->FindUser( playerIndex );

		if( object )
		{
			object->SendMsg( message, size );
		}
	}

	// �ϰ� ó�� ��� ������ ����
	// ���� ó�� ��� ������ ����
	for(	Score::ReadyMember::const_iterator it = mScore.mReadyMember.begin();
			mScore.mReadyMember.end() != it;
			++it )
	{
		const DWORD playerIndex = *it;
		CObject*	object		= g_pUserTable->FindUser( playerIndex );

		if( object )
		{
			object->SendMsg( message, size );
		}
	}	
}


void CGuildScore::SetWarehouseUser( DWORD playerIndex )
{
	mData.mWarehouseUser = playerIndex;
}


DWORD CGuildScore::GetWarehouseUser() const
{
	return mData.mWarehouseUser;
}


void CGuildScore::SetMasterIndex( DWORD masterIndex )
{
	mData.mMasterIndex = masterIndex;
}