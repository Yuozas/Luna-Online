// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL DisconnectUser(DWORD dwConnectionIndex);

#include <AntiCpSvrFunc.h>
#include "Crypt.h"


#ifdef _NPROTECT_
class CCSAuth2;
#endif

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.09
class CSHFamily;
// E 패밀리 추가 added by hseos 2007.07.09

struct aGAMEOPTION
{
	BYTE bNoFriend;
	BYTE bNoWhisper;

	aGAMEOPTION():bNoWhisper(0), bNoFriend(0) {}
};

struct CHARSELECTINFO
{
	DWORD dwCharacterID;
	LEVELTYPE Level;
	MAPTYPE MapNum;
	BYTE Gender;

	// 080331 LYW --- UserTable : Add some variable for chatroom system.
	//WORD wClassIndex ;
	char name[MAX_NAME_LENGTH+1] ;
};

typedef struct tagUSERINFO
{
	BOOL	bForceMove;
	DWORD	dwConnectionIndex;
	DWORD	dwCharacterID;
	DWORD	dwUserID;
	BYTE	UserLevel;
	DWORD	dwMapServerConnectionIndex;
	WORD	wUserMapNum;
	CHARSELECTINFO SelectInfoArray[MAX_CHARACTER_NUM];

	DWORD	DistAuthKey;			// Distribute 서버에서 보내온 AuthKey 보관
//KES 연속 채팅 금지
	DWORD	dwLastChatTime;	

//---KES CheckSum 071003
	BYTE	cbCheckSum;
//----------------------

	WORD	wChannel;
	DWORD	dwUniqueConnectIdx;	// 서버에서의 고유 연결 번호
								// dwConnectionIdx와는 다름. 계속 증가함

	CCrypt crypto;
	aGAMEOPTION GameOption;

	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;

	DATE_MATCHING_INFO		DateMatchingInfo;			
	// 게임내 '검색' 때문에 에이전트의 유저가 플레이어(캐릭터)정보를 임시 저장. 잘 지워주는게 중요할 듯. 
	// 한번에 한개만 쓰이니 캐릭터 밑에 붙일 필요는 없다.	

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.09	2007.10.11
	CSHFamily*		pcsFamily;
	DWORD			nFamilyLeaveDate;
	int				nFamilyLeaveKind;
	// E 패밀리 추가 added by hseos 2007.07.09	2007.10.11

	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.08.03
	int				nSexKind;
	// E 성별선택 추가 added by hseos 2007.08.03

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.25	2007.11.28
	UINT			nChallengeZoneEnterID;
	UINT			nChallengeZoneEnterSection;
	UINT			nChallengeZoneReturnMapNum;
	// E 데이트 존 추가 added by hseos 2007.11.25	2007.11.28

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2008.01.11
	DWORD			nMarriageConnPartnerID;
	// E 결혼 추가 added by hseos 2008.01.11

	// 080414 LYW --- Add state for chatroom system.
	BYTE byAddedChatSystem ;

#ifdef _HACK_SHIELD_
	BYTE m_GUIDInfo[SIZEOF_GUIDREQINFO];
	BYTE m_Info[SIZEOF_REQINFO];
	unsigned long* m_pCrcInfo;
	BOOL m_bHSCheck;
#endif
#ifdef _NPROTECT_
	CCSAuth2	*m_pCSA;
	BOOL		m_bCSA;
	int			m_nCSAInit;
	DWORD		dwLastNProtectCheck;
#endif
} USERINFO;

typedef void (__stdcall *USERDisconnectProc)(USERINFO* pInfo);					// 캐릭터가 disconnect되서 RemoveUser()가 호출될 때 호출 되는 콜백.

void	__stdcall OnRemoveObjProc( USERINFO* pInfo);			// CUserTable중 ObjID쪽에 쓰일 콜백.

class CUserTable : public CYHHashTable<USERINFO>
{

	DWORD m_MaxUserCount;

	DWORD m_dwUserCount;


	DWORD m_addCount;
	DWORD m_removeCount;

	USERDisconnectProc			m_pOnDisconnectProc;
public:
	CUserTable();
	virtual ~CUserTable();
	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }
	void Init(DWORD dwBucket, USERDisconnectProc pProc);								// 두번째 인자는 DisconnectProcedure의 함수포인터.
	USERINFO * FindUser(DWORD dwKey);
	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount(){ return m_dwUserCount; }

	DWORD GetUserMaxCount(){return m_MaxUserCount;}
	void SetCalcMaxCount(DWORD CurCount);
	
	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
	DWORD GetRemoveCount() { return m_removeCount; }
};
  


extern CUserTable * g_pUserTable;							// key : connectionidx
extern CUserTable * g_pUserTableForUserID;					// key : UserID
extern CUserTable * g_pUserTableForObjectID;				// key : dwObjectID

#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
