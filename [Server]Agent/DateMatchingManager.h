#pragma once
/*

2007/04/11 - 이진영

개요.
데이트 매칭을 위한 메니져.
검색을 위한 해시 테이블을 관리한다.

기타.
DATEMATCHINGMGR를 싱글톤으로 둔다.

AddUserInfo()가 호출 되는 순간은 클라이언트의 데이트매칭 다이얼로그에서 'Search'버튼을 누르는 순간. 
RemoveUserInfo()가 호출 되는 순간은 클라이언트의 데이트매칭 다이얼로그의 'x'를 누르는 순간인데, Search를 안하고 ''x'를 누를 수 있으므로, 무효한 값이 나올 수 있다.

검색 조건이 너무 대충이고, 점수별로 리턴을 하므로 해시는 의미가 없을듯.
*/

#include "stdafx.h"
#include "../[Lib]YHLibrary/PtrList.h"
#include "UserTable.h"

#define DATEMATCHINGMGR	USINGTON(CDateMatchingManager)

struct	DATE_MATCHING_DATA
{
	DWORD		dwAgeIndex;			// 나이 인덱스.			클라이언트의 DateMatchingDlg에서 선택한 '10-14세' 따위의 카테고리. 해시 키를 위해 메타데이터로 넣는다.
	DWORD		dwRegionIndex;		// 지역 인덱스.			역시나...

	USERINFO*	pInfo;
	DWORD		dwAge;				// 실제 나이.
	BYTE		bGender;			// 성별. GENDER_MALE, GENDER_FEMALE
};

//struct	USER_LIST_HASH
//{
//	//char*	pszName;						// 이름. 대충 USERINFO::DATE_MATCHING_INFO::szName 의 포인터.
//	//BYTE	byGender;						// 성별.
//	//USERINFO*		pUser;
//	PTRLISTPOS	pos;						// 링크드리스트에서 포지션.
//	DWORD		dwUserID;					// key로 쓴다.
//
//};

class CDateMatchingManager
{
	cPtrList		m_MaleList;				// 남자 유저 리스트.
	cPtrList		m_FemaleList;			// 여자 유저 리스트.

	CYHHashTable<___POSITION>			m_Hash;				// 이름으로 pUser를 찾는 해시.

public:
	CDateMatchingManager(void);
	virtual ~CDateMatchingManager(void);

	void	Init(DWORD dwMaxOutput);			// CServerSystem::Start(WORD ServerNum)에서 호출됨. SearchUser할 때 최대 출력 갯수를 인자로 준다.
	void	Release(void);		// CServerSystem::End()에서 호출.

	void	AddUserInfo( USERINFO* pUser);				// 유저 인포를 등록. 클라이언트에서 search를 누르는 순간 호출될듯. 두번 들어갈 수 있으니 안에서 체크.
	void	RemoveUserInfo( USERINFO* pUser);			// 등록된 유저 인포를 해제. 클라이언트에서 DateMatchingDlg를 닫던가, USERINFO가 무효화 되는 시점에서 해제 해줘야 한다.
	void	SearchUser( DWORD* pdwOutCount, DATE_MATCHING_SEARCH_OUTPUT* pOut, DWORD dwMaxOutput, USERINFO* pClient, DWORD dwAgeIndex, DWORD dwRegionIndex);	// Gender는 GENDER_MALE, GENDER_FEMALE 둘중 하나.? 아웃풋은 어떻게? pdwFeeling은 좋고싫은성향5개 배열.

	// 네트웍 프로시저.
	void	OnRequestTargetInfo( DWORD dwTargetUserID, USERINFO* pClient);
	void	OnModifyIntroduce( char* szIntroduce, USERINFO* pUserInfo);
};

EXTERNGLOBALTON(CDateMatchingManager);
