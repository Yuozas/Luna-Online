// UserInfoManager.h: interface for the CUserInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_)
#define AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum eUSERINFO_KIND
{
	eUIK_NONE		= 0,
	eUIK_INTERFACE	= 1,
	eUIK_USERSTATE	= 2,
	eUIK_USERQUEST	= 3,
	eUIK_ALL		= 0xFFFFFFFF,
};


enum eSAVE_DIALOG
{
	eSD_HeroGuage,			//CG_GUAGEDLG
	eSD_MonsterGuage,		//CG_MONSTERGUAGEDLG
	eSD_MiniMap,			//MNM_DIALOG
	eSD_Quick,				//QI_QUICKDLG
	eSD_MainBar,			//MI_MAINDLG
	eSD_Chat,				//CTI_DLG
	eSD_Inventory,			//IN_INVENTORYDLG
	eSD_Character,			//CI_CHARDLG
	eSD_Skill,				//MGI_SKILLSURYUNDLG
//	eSD_Note,				//NOTE_NOTEDLG
//	eSD_Friend,				//FRI_FRIENDDLG
	eSD_Option,				//OTI_TABDLG
	eSD_Quest,				//QUE_TOTALDLG
	eSD_EventNotify,		//
	eSD_Party,				// partydlg
//상점
//파티
//노점
//교환

	eSD_Max,
};

enum eUSERINFO_FLAG
{
	eUIF_NONACTIVE	= 0,
	eUIF_ACTIVE		= 1,
	eUIF_SECONDFORM	= 2,

	eUIF_TABNUM2	= 4,
	eUIF_TABNUM3	= 8,
	eUIF_TABNUM4	= 16,
	
	eUIF_SUBTABNUM2	= 32,
	eUIF_SUBTABNUM3	= 64,
	eUIF_SUBTABNUM4	= 128,

	eUIF_SUBACTIVE	= 256,

};

struct sDIALOG_INFO
{
	LONG	lPosX;
	LONG	lPosY;
	DWORD	dwFlag;
	DWORD	dwParam;
};

struct sUSERINFO_INTERFACE
{
	sDIALOG_INFO	DIALOG_INFO[eSD_Max];
};

struct sUSERINFO_USERSTATE
{
	WORD wKyungGongIdx;
	BOOL bIsKyungGongMode;
	float fZoomDistance;
};

class CPlayer ;

#define MAX_QUICKVIEW_QUEST 5
#define MAX_QUEST_COUNT 20

struct sQUEST_INFO																		// 퀘스트 정보 구조체.
{
	DWORD dwQuestIdx ;																	// 메인 퀘스트 인덱스.
	//DWORD dwSubQuestIdx[32] ;															// 서브 퀘스트 인덱스 배열.

	BOOL  bCompleteMainQuest ;

    BOOL  bComplete[32] ;																// 서브 퀘스트별 완료 여부 배열.

	int   nCount[32] ;																	// 서브 퀘스트별 진행 카운트 배열.
	int   nTotalCount[32] ;																// 서브 퀘스트별 목표 카운트 배열.

	sQUEST_INFO()																		// 초기화 함수.
	{
		dwQuestIdx = 0 ;																// 메인 퀘스트 인덱스를 0으로 세팅한다.

		for( int count = 0 ; count < 32; ++count )										// 최대 크기의 서브 퀘스트는 32, 만큼 for문을 돌린다.
		{
		//	dwSubQuestIdx[count] = 0 ;													// 서브 퀘스트 인덱스를 0으로 세팅한다.

			bComplete[count] = FALSE ;													// 서브 퀘스트 별 완료 여부를 false로 세팅한다.

			nCount[count] = 0 ;															// 서브 퀘스트 별 진행 카운트를 0으로 세팅한다.
			nTotalCount[count] = 0 ;													// 서브 퀘스트 별 목표 카운트를 0으로 세팅한다.
		}
	} 
} ;

struct sUSERINFO_QUESTQUICKVIEW															// 퀘스트 알림이에 등록 된 퀘스트 아이디를 저장하는 구조체.
{
	DWORD dwQuestID[MAX_QUICKVIEW_QUEST] ;												// 퀘스트 최대 등록 가능 수 크기의 배열을 멤버로 갖는다.

	//cPtrList list ;

	//sQUEST_INFO	questInfo[MAX_QUEST_COUNT] ;

	//sQUEST_INFO quickInfo[MAX_QUICKVIEW_QUEST] ;
} ;

// 071028 LYW --- UserInfoManager : Add struct to write error of give item function of quest group.
struct sUSERINFO_ERR_GIVEITEM												// 퀘스트 그룹에서 GiveItem 에러를 기록할 구조체.
{
	char UserName[32] ;														// 유저 이름을 담는 변수.
	BYTE bCaption ;															// 에러 발생 함수 캡션을 담을 변수.
	BYTE bErrNum ;															// 에러 번호를 담을 변수.
	DWORD dwQuestIdx ;														// 퀘스트 인덱스를 담을 변수.
	DWORD dwItemIdx ;														// 아이템 인덱스를 담을 변수.
} ;


#define USERINFOMGR USINGTON(CUserInfoManager)

class CUserInfoManager  
{
protected:
	char m_strUserID[MAX_NAME_LENGTH+1];
	DWORD m_dwSaveFolderName;

//for interface
//	cPtrList m_listInterface;
	BOOL m_bMapChange;
	
//임시 하드코딩
	BOOL m_bMinimapActive;

public:
	CUserInfoManager();
	virtual ~CUserInfoManager();

	void LoadUserInfo( DWORD dwFlag );
	void SaveUserInfo( DWORD dwFlag );
//**	
	void LoadInterfaceInfo( char* strFilePath );
	void SaveInterfaceInfo( char* strFilePath );

	void LoadUserState( char* strFilePath );
	void SaveUserState( char* strFilePath );

	void SaveUserQuestQuickView( char* strFilePath ) ;									// 유저의 퀘스트 알림이 정보를 저장하는 함수.
	void LoadUserQuestQuickView( char* strFilePath ) ;									// 유저의 퀘스트 알림이 정보를 로딩하는 함수.
	void DeleteUserQuestInfo(CPlayer* pPlayer) ;

//**
	void SetUserID( char* strID );
	char* GetUserID() { return m_strUserID; }

//for Interface
	BOOL IsValidInterfacePos( RECT* prcCaption, LONG lX, LONG lY );
//	void AddSaveDialog( cDialog* pDlg );

	void SetMapChage( BOOL bMapChange ) { m_bMapChange = bMapChange; }

	void SetSaveFolderName( DWORD dwUserID );
};


EXTERNGLOBALTON(CUserInfoManager)

#endif // !defined(AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_)
