#pragma once
/*

2007/04/11 - ������

����.
����Ʈ ��Ī�� ���� �޴���.
�˻��� ���� �ؽ� ���̺��� �����Ѵ�.

��Ÿ.
DATEMATCHINGMGR�� �̱������� �д�.

AddUserInfo()�� ȣ�� �Ǵ� ������ Ŭ���̾�Ʈ�� ����Ʈ��Ī ���̾�α׿��� 'Search'��ư�� ������ ����. 
RemoveUserInfo()�� ȣ�� �Ǵ� ������ Ŭ���̾�Ʈ�� ����Ʈ��Ī ���̾�α��� 'x'�� ������ �����ε�, Search�� ���ϰ� ''x'�� ���� �� �����Ƿ�, ��ȿ�� ���� ���� �� �ִ�.

�˻� ������ �ʹ� �����̰�, �������� ������ �ϹǷ� �ؽô� �ǹ̰� ������.
*/

#include "stdafx.h"
#include "../[Lib]YHLibrary/PtrList.h"
#include "UserTable.h"

#define DATEMATCHINGMGR	USINGTON(CDateMatchingManager)

struct	DATE_MATCHING_DATA
{
	DWORD		dwAgeIndex;			// ���� �ε���.			Ŭ���̾�Ʈ�� DateMatchingDlg���� ������ '10-14��' ������ ī�װ�. �ؽ� Ű�� ���� ��Ÿ�����ͷ� �ִ´�.
	DWORD		dwRegionIndex;		// ���� �ε���.			���ó�...

	USERINFO*	pInfo;
	DWORD		dwAge;				// ���� ����.
	BYTE		bGender;			// ����. GENDER_MALE, GENDER_FEMALE
};

//struct	USER_LIST_HASH
//{
//	//char*	pszName;						// �̸�. ���� USERINFO::DATE_MATCHING_INFO::szName �� ������.
//	//BYTE	byGender;						// ����.
//	//USERINFO*		pUser;
//	PTRLISTPOS	pos;						// ��ũ�帮��Ʈ���� ������.
//	DWORD		dwUserID;					// key�� ����.
//
//};

class CDateMatchingManager
{
	cPtrList		m_MaleList;				// ���� ���� ����Ʈ.
	cPtrList		m_FemaleList;			// ���� ���� ����Ʈ.

	CYHHashTable<___POSITION>			m_Hash;				// �̸����� pUser�� ã�� �ؽ�.

public:
	CDateMatchingManager(void);
	virtual ~CDateMatchingManager(void);

	void	Init(DWORD dwMaxOutput);			// CServerSystem::Start(WORD ServerNum)���� ȣ���. SearchUser�� �� �ִ� ��� ������ ���ڷ� �ش�.
	void	Release(void);		// CServerSystem::End()���� ȣ��.

	void	AddUserInfo( USERINFO* pUser);				// ���� ������ ���. Ŭ���̾�Ʈ���� search�� ������ ���� ȣ��ɵ�. �ι� �� �� ������ �ȿ��� üũ.
	void	RemoveUserInfo( USERINFO* pUser);			// ��ϵ� ���� ������ ����. Ŭ���̾�Ʈ���� DateMatchingDlg�� �ݴ���, USERINFO�� ��ȿȭ �Ǵ� �������� ���� ����� �Ѵ�.
	void	SearchUser( DWORD* pdwOutCount, DATE_MATCHING_SEARCH_OUTPUT* pOut, DWORD dwMaxOutput, USERINFO* pClient, DWORD dwAgeIndex, DWORD dwRegionIndex);	// Gender�� GENDER_MALE, GENDER_FEMALE ���� �ϳ�.? �ƿ�ǲ�� ���? pdwFeeling�� �����������5�� �迭.

	// ��Ʈ�� ���ν���.
	void	OnRequestTargetInfo( DWORD dwTargetUserID, USERINFO* pClient);
	void	OnModifyIntroduce( char* szIntroduce, USERINFO* pUserInfo);
};

EXTERNGLOBALTON(CDateMatchingManager);
