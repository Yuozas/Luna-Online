#pragma once

/*
070730 ����. ���  ���� ��� Ŭ����

��� ���� ó���� ��ε�ĳ�����̳� �ݺ��� ���Ÿ� ���ο� �ΰ� ����Ǿ���. ����, �ٸ� �ʰ� ���⸦ �ϰų� ���� �ʴ´�.
��� �� ���� ���� ������ ���� ���� ���Ѵ�. �� ������ �����Ϸ��� DB ���ν����� ���ľ��Ѵ�. 



ó�� ����:	�� ���� ������ ������ ���� �����ϰ�, �߰��� ������ ����Ѵ�. 1�ð� ������ �� ������ DB�� ������Ʈ�ǰ� 0���� �ʱ�ȭ�ȴ�.
			������ ������Ʈ�� ������ ��ε�ĳ�����Ѵ�.

			���μ��� Ÿ���� �ö����� ����Ʈ�� �� ó���� �ִ� ��� ������ ���� ����� �ð��� �Ǿ����� üũ�Ѵ�. 
			�Ǿ����� ó���ϰ�, ���� ó���ð��� ���� �ð��� 1�ð� �ڷ� �س��´�.

ó�� ���:
(1) ��� ȸ�� �α���, ����
	- �ϴ� ��� ��Ͽ� �ִ´�. 

(2) ��� ȸ�� �α׾ƿ�, Ż��, �߹�
	- ��� ȸ������ 1 ���ҽ�Ű��, ����� �ð���ŭ �Ǽ� ������ ����Ʈ�� �߰��صд�.

(3) ��� ����
	- ���� Ÿ�� �䱸�� ���� �ﰢ ó���Ѵ�

(4) 1�ð� ���
	- ( ��� ȸ���� * 1 ) + ( ������� ������ ȸ���� ����Ʈ) + ���� ����Ʈ = ������ DB�� ������Ʈ�Ѵ�.
	`������Ʈ�� �����ϸ� ������� �������� �ش� ��忡 ��ε�ĳ�����Ѵ�.

*/


class CGuildScore
{
	friend class CGuildManager;

private:
	CGuildScore( DWORD unitTime, float tickScore );
	virtual ~CGuildScore()	{};

public:
	// �α���, ����.
	void	AddPlayer	( DWORD playerIndex );

	// , �α׾ƿ�, Ż��, �߹�� ȣ��. ȸ���� �׶����� ������Ų ������ ��ȯ�Ѵ�
	float	RemovePlayer( DWORD playerIndex );

	// ������ ������Ų��
	void AddScore( float score );

	// ������ ��� ������ �������� �˻��Ѵ�
	BOOL IsEnableScore( DWORD currentTick ) const;

	// ������ ������ ����Ѵ�
	float GetScore( DWORD currentTick ) const;

	// �ٽ� ��� ������ ���·� �����
	void Reset( DWORD currentTick );

	// ���� ȸ������ ��ȯ
	DWORD GetPlayerSize() const;

	//void GetData( MSG_GUILD_MEMBER_LOGIN& ) const;
	void SetData( const MSG_GUILD_DATA&, DWORD masterIndex );

	void SetLevel( DWORD level );
	DWORD GetLevel() const;
	DWORD GetMasterIndex() const;
	void Send( MSGBASE*, DWORD size ) const;

	void SetWarehouseUser( DWORD playerIndex );
	DWORD GetWarehouseUser() const;

	void SetMasterIndex( DWORD masterIndex );


private:
	// ����ȭ�� �߿伺�� �������� ������ �޸𸮿� �����Ѵ�.
	struct Data
	{
		DWORD mLevel;		// ��� ����
		DWORD mMasterIndex;	// ��帶���� �÷��̾� �ε���
		DWORD mWarehouseUser;
	}
	mData;

	struct Score
	{
		float	mScore;		// DB�� ������ų ����. ���� ������ �ƴ�!

		DWORD	mBeginTick;	// ������ ������ ƽī��Ʈ
		DWORD	mEndTick;	// ������ ������ ƽī��Ʈ

		// �ϰ� ó���Ǵ� ȸ��
		// Ű: �÷��̾� �ε���
		typedef std::set< DWORD >	ReadyMember;
		ReadyMember					mReadyMember;

		// ���� ó���Ǵ� ȸ��. ���� �ð��� �ٸ��Ƿ� ���������� ó���Ǿ�� �Ѵ�
		// Ű: �÷��̾� �ε���, ��: ����� ������ ƽī��Ʈ
		typedef std::pair< DWORD, DWORD >			EventUnit;
		typedef stdext::hash_map< DWORD, DWORD >	PrepareMember;
		PrepareMember								mPrepareMember;
	}
	mScore;

	// ���� ����
	const DWORD mUnitTick;

	// 1 ƽī��Ʈ�� ����
	const float mTickScore;

	// ���� ���ݴ� ��� ����
	const float mUnitTickScore;
};