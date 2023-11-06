#pragma once


class CUserManager  
{
public:	
	CUserManager();
	virtual ~CUserManager();

	static CUserManager& GetInstance();

	bool IsConnect( DWORD connectionIndex ) const;
	bool IsConnectedUser( const char* id ) const;
	bool IsConnectedIp( const char* ip ) const;


public:
	struct User
	{
		eGM_POWER	mPower;
		std::string	mId;
		std::string	mName;
		std::string mIp;
		std::string	mRegistedDate;
	};

	bool		AddUser( DWORD connectionIndex, const char* id, const char* name, const char* ip, const char* registedDate, eGM_POWER );
	void		RemoveUser( DWORD connectionIndex );
	const User& GetUser( DWORD connectionIndex )	const;
	const User& GetUser( const char* id )			const;

    
protected:
	// Ű: ���� �ε���
	typedef stdext::hash_map< DWORD, User >	UserMap;
	UserMap									mUserMap;


	// ��� ���� ����
protected:
	typedef	LONGLONG	MessageKey;
	// �������ݿ� ���� ��밡���� ������ ����������. �������� ��� ���������� ����������.
	typedef stdext::hash_set< eGM_POWER >			RankSet;
	typedef stdext::hash_map< MessageKey, RankSet >	AutorityMap;

	AutorityMap										mAuthorityMap;

protected:
	// �� ���� �޾� ������ Ű�� �����Ѵ�
	MessageKey GetMessageKey( RecoveryCategory, BYTE protocol ) const;


public:
	bool IsAuthorize( RecoveryCategory, BYTE protocol, DWORD connectionIndex ) const;
};