#pragma once
//=================================================================================================
//	FILE		: TutorialManager.h
//	PURPOSE		: Tutorial manager to management all party of TUTORIAL.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		GM툴 모드로 실행 할 때 에러 메시지를 출력하기 위한 에러 메시저 정의
//=========================================================================
#ifdef _GMTOOL_

// ERROR MESSAGE CATION PART.
#define INIT_CATION		"cTutorialManager::Init"							// 초기화 함수.

#define LT_CATION		"cTutorialManager::LoadTutorial"					// 튜토리얼 로드 캡션 출력용 문자열.
#define LMT_CATION		"cTutorialManager::LoadMainTutorial"				// 메인 튜토리얼 로드 캡션 출력용 문자열.
#define LST_CATION		"cTutorialManager::LoadSubTutorial"					// 서브 튜토리얼 로드 캡션 출력용 문자열.

#define LM_CATION		"cTutorialManager::LoadMsg"							// 메시지 로드 캡션 출력용 문자열.
#define RML_CATION		"cTutorialManager::ReadMsgLine"						// 메시지 라인 리드 캡션 출력용 문자열.

#define LHI_CATION		"cTutorialManager::LoadHelperInfo"					// 헬프 정보 로드 캡션 출력용 문자열.

#define NMP_CATION		"cTutorialManager::NetworkMsgParse"					// 네트워크 에러 발생 함수 출력용 문자열.

#define CSC_CATION		"cTutorialManager::Check_StartCondition"			// 시작 조건 체크 캡션 출력용 문자열.
#define CNT_CATION		"cTutorialManager::Check_NpcTalk"					// NPC 대화 조건 체크 캡션 출력용 문자열.
#define CGI_CATION		"cTutorialManager::Check_GetItem"					// 아이템 체크 캡션 출력용 문자열.
#define COD_CATION		"cTutorialManager::Check_OpenDialog"				// 지정된 다이얼로그 열기 체크 캡션 출력용 문자열.

#define SCT_CATION		"cTutorialManager::Start_CurTutorial"				// 튜토리얼 시작 캡션 출력용 문자열.

#define	SHT_CATION		"cTutorialManager::Start_HelpTutorial"				// 도움말 튜토리얼 시작 캡션 출력용 문자열.

// INITIALIZE PART.
#define ERR_MSG			"Failed to load tutorial message"					// 튜토리얼 메시지 로드에 실패했다는 에러 메시지.
#define ERR_TUTORIAL	"Failed to load tutorial info"						// 튜토리얼 정보 로드에 실패했다는 에러 메시지.
#define ERR_HELPERLINK	"Failed to load helper info"						// 도움말 정보 로드에 실패했다는 에러 메시지.

// COMMON PART.
#define CERR_MSG1		"Invalid file pointer"								// 인자로 넘어온 파일 포인터가 잘못되었다는 에러 메시지.
#define CERR_MSG2		"Over angle bracket } : %d - %d"					// } 괄호 닫기가 잘못 되었다는 에러 메시지.
#define CERR_MSG3		"Over angle bracket { : %d - %d"					// { 괄호 열기가 잘못 되었다는 에러 메시지.	
#define CERR_MSG4		"Invalid parameter"									// 잘못된 인자가 넘어왔다는 에러 메시지.
#define CERR_MSG5		"Invalid tutorial dialog."							// 멤버 튜토리얼 다이얼로그 정보 에러.

// TUTORIAL PART.
#define TERR_MSG1		"Invalid file name~!!"								// 튜토리얼 스크립트 파일 명이 잘못 되었다는 에러 메시지.
#define TERR_MSG2		"Failed to open file : %s"							// 파일 열기에 실패 했다는 에러 메시지.
#define TERR_MSG3		"Invalid Tutorial Info : %d"						// 튜토리얼 정보가 잘못되었다는 에러 메시지.
#define TERR_MSG4		"Invalid Tutorial Index : %d"						// 튜토리얼 인덱스가 잘못되었다는 에러 메시지.
#define TERR_MSG5		"Invalid start condition 1 : %d"					// 제 1 발동 조건이 잘못되었다는 에러 메시지.
#define TERR_MSG6		"Failed to create tutorial : %d"					// 튜토리얼 생성에 실패 했다는 에러 메시지.
#define TERR_MSG7		"Invalid tutorial pointer"							// 인자로 넘어온 튜토리얼 포인터가 잘못되었다는 에러 메시지.
#define TERR_MSG8		"Invalid speech index : %d - %d"					// 대화 인덱스가 잘못되었다는 에러 메시지.
#define TERR_MSG9		"Invalid sound index : %d - %d"						// 사운드 인덱스가 잘못되었다는 에러 메시지.
#define TERR_MSG10		"Invalid npc index : %d - %d"						// NPC 인덱스가 잘못되었다는 에러 메시지.
#define TERR_MSG11		"Invalid face index : %d - %d"						// 얼굴 인덱스가 잘못 되었다는 에러 메시지.
#define TERR_MSG12		"Invalid first condition index : %d - %d"			// 첫번째 완료 조건이 잘못되었다는 에러 메시지.
#define TERR_MSG13		"Invalid Sub Tutorial Info : %d - %d"				// 서브 튜토리얼 정보가 잘못 되었다는 에러 메시지.
#define TERR_MSG14		"Failed to create sub tutorial : %d - %d"			// 서트 튜토리얼 생성에 실패 했다는 에러 메시지.
#define TERR_MSG15		"Failed to receive completed tutorial : %d"			// 완료한 튜토리얼 정보를 받는데 실패 했다는 에러.
#define TERR_MSG16		"Failed to receive current sub tutorial"			// 현재 진행중인 서브 튜토리얼 받는데 실패 했다는 에러.
#define TERR_MSG17		"Failed to receive window id : %s"					// 지정된 문자열로 윈도우 아이디 받기 실패.

// TUTORIAL MESSAGE PART.
#define MERR_MSG1		"Over string - Msg %d"								// 메시지 길이를 넘었다는 에러 메시지.
#define MERR_MSG2		"Failed to create message - Msg %d"					// 메시지 길이를 넘었다는 에러 메시지.
#define MERR_MSG3		"Invalid message Info : %d"							// 잘못된 메시지 정보라는 에러 메시지.
#define MERR_MSG4		"Invalid message index : %d"						// 잘못된 메시지 인덱스라는 에러 메시지.

// HELPER PART.
#define HERR_MSG1		"Invalid link index : %d"							// 도움말 링크 인덱스가 잘못 되었다는 에러 메시지.
#define HERR_MSG2		"Invalid link title : %d"							// 도움말 링크 제목이 잘못 되었다는 에러 메시지.
#define HERR_MSG3		"Invalid linked tutorial index : %d"				// 도움말 링크의 튜토리얼 인덱스가 유효하지 않다는 에러 메시지.
#define HERR_MSG4		"Failed to create help link : %d"					// 도움말 링크 생성에 실패 했다는 에러 메시지.

// PROCESS PART.
#define PERR_MSG1		"Invalid tutorial Info. Can't start tutorial! : %d"	// 잘못된 튜토리얼 정보로 인해 튜토리얼 진행을 하지 못한다는 에러.
#define PERR_MSG2		"Failed to receive npc info : %d npc"				// 몇 번 npc 정보를 받는데 실패 했다는 에러 메시지.
#define PERR_MSG3		"Invalid pointer pItem~!!"							// 인자로 넘어온 pItem이 잘못되었다는 에러.
#define PERR_MSG4		"Failed to receive inventory dialog."				// 인벤토리 다이얼로그를 얻지 못했다는 에러 메시지.
#define PERR_MSG5		"Failed to receive list dialog of help dialog."		// 도움말 창의 리스트 다이얼로그를 받지 못했다는 에러.

// NETWORK PART.
#define NERR_MSG1		"Failed to casting tutorial data in networpaser"	// 네트워크 파서에서 튜토리얼 데이터 형 변환에 실패 했다는 에러.
#define NERR_MSG2		"Invalid tutorial index"							// 잘못 된 튜토리얼 인덱스 에러.
#define NERR_MSG3		"Failed to receive tutorial position : %d"			// 몇 번 인덱스로 튜토리얼 정보를 받는데 실패 했다는 에러.
#define NERR_MSG4		"Failed to receive tutorial info : %d"				// 인덱스에 해당하는 튜토리얼 정보를 받는데 실패 했다는 에러.

#endif





//=========================================================================
//		필요한 헤더파일을 불러온다.
//=========================================================================
#include "../MHFile.h"														// 묵향 파일 인터페이스를 포함한다.
#include "..\[lib]yhlibrary\PtrList.h"										// 튜토리얼 관련 정보를 관리하기 위한 리스트 인터페이스를 포함한다.





//=========================================================================
//		필수 정의 파트.
//=========================================================================
#define TUTORIALMGR		USINGTON(cTutorialManager)

#define TUTORIAL_MSG_PATH		"./Data/Script/Tutorial/Tutorial_Msg.bin"	// 튜토리얼 메시지 경로를 정의한다.
#define TUTORIAL_SCRIPT_PATH	"./Data/Script/Tutorial/Tutorial_Script.bin"// 튜토리얼 스크립트 경로를 정의한다.	
#define HELPER_LINKLIST_PATH	"./Data/Script/Tutorial/Help_Link_List.bin" // 도움말 링크 리스트 경로를 정의한다.

#define NPC_EMOTION_MAX		5												// 모션에 사용할 수 있는 최대 인덱스.

#define FIRST_GAMEIN_TUTORIAL_IDX	0										// 처음 게임에 접속 했을 때, 무조건 실행하는 튜토리얼 인덱스.

enum ENUM_TUTORIAL_TYPE
{
	e_TT_FirstGameIn = 0 ,
	e_TT_LevelUp,
	e_TT_Max,
} ;





//=========================================================================
//		필요한 컨트롤 인터페이스를 선언한다.
//=========================================================================
class CItem ;																// 아이템 클래스를 선언한다.
class cTutorial ;															// 튜토리얼 클래스를 선언한다.
class cTutorial_Msg ;														// 튜토리얼 메시지 클래스를 선언한다.

class cTutorialDlg ;														// 튜토리얼 진행을 위한 튜토리얼 다이얼로그 클래스를 선언한다.
class cHelperDlg ;															// 도움말 진행을 위한 도움말 다이얼로그 클래스를 선언한다.
class cTutorialBtnDlg ;														// 튜토리얼 알림 버튼 다이얼로그 클래스를 선언한다.

struct SUB_TUTORIAL ;														// 서브 튜토리얼 구조체를 선언한다.

struct HELPLINK																// 도움말 링크 구조체를 정의한다.
{	
	int nIndex ;															// 출력 될 리스트 인덱스.
	int nTutorial_Idx ;														// 클릭시 진행 할 튜토리얼 인덱스.
	char Title[128] ;														// 출력 될 리스트 타이틀.
} ;





//=========================================================================
//		튜토리얼 매니져 클래스 정의
//=========================================================================
class cTutorialManager														// 튜토리얼을 관리할 매니져 클래스를 정의한다.
{
	cPtrList			m_Tutorial_Msg ;									// 튜토리얼 메시지를 관리하는 리스트.
	cPtrList			m_Tutorial_List ;									// 튜토리얼 정보를 관리하는 리스트.
	cPtrList			m_Helper_List ;										// 도움말 클릭으로 진행하는 튜토리얼 정보를 관리하는 리스트.

	BOOL				m_bOnOff_Tutorial ;									// 튜토리얼의 보이기 여부 변수.

	cTutorialDlg*		m_pTutorialDlg ;									// 튜토리얼 진행을 위한 다이얼로그 포인터.
	cHelperDlg*			m_pHelperDlg ;										// 도움말 모드 진행을 위한 다이얼로그 포인터.
	cTutorialBtnDlg*	m_pBtnDlg ;											// 튜토리얼 알림 버튼 다이얼로그 포인터.

	BOOL				m_bActiveTutorial ;									// 튜토리얼의 보이기 여부.
	BOOL				m_bUpdated ;										// 튜토리얼 이벤트가 업데이트 되었는지 체크하는 변수.

	int					m_nCurTutorialIdx ;									// 현재 진행중인 튜토리얼 인덱스.
	int					m_nCompletedIdx ;									// 완료 한 튜토리얼 인덱스.

	int					m_nShow_BtnSndIdx ;									// 튜토리얼 알림 버튼이 출현할 때 play 되는 사운드 인덱스.
	int					m_nEndTutorial_SndIdx ;								// 튜토리얼을 완료 했을 때 play 되는 사운드 인덱스.

public:
	// BASE PART.
	cTutorialManager(void);													// 생성자 함수.
	virtual ~cTutorialManager(void);										// 소멸자 함수.

	void Init() ;															// 초기화 함수.
	void Release() ;														// 해제 함수.


	// TUTORIAL PART.
	BOOL LoadTutorial(char* strFileName) ;									// 튜토리얼 정보를 로딩하는 함수.
	void ReleaseTutorial() ;												// 튜토리얼 정보를 해제하는 함수.

	cTutorial* LoadMainTutorial(CMHFile* pFile) ;							// 메인 튜토리얼 정보를 로딩하는 함수.
	BOOL LoadSubTutorial(CMHFile* pFile, cTutorial* pTutorial) ;			// 서브 튜토리얼 정보를 로딩하는 함수.

	cTutorial* Get_Tutorial(int nIndex) ;									// 인덱스에 해당하는 튜토리얼 정보를 리턴하는 함수.


	// TUTORIAL MESSAGE PART.
	BOOL LoadMsg(char* strFileName) ;										// 튜토리얼 메시지를 로딩하는 함수.
	void ReleaseMsg() ;														// 튜토리얼 메시지를 해제하는 함수.

	BOOL ReadMsgLine(CMHFile* pFile, cTutorial_Msg* pMsg) ;					// 튜토리얼 메시지의 라인을 읽는 함수.

	cTutorial_Msg* Get_TutorialMsg(int nIndex) ;							// 인덱스에 해당하는 튜토리얼 메시지 정보를 리턴하는 함수.


	// HELPER PART.
	BOOL LoadHelperInfo(char* strFileName) ;								// 도움말 정보를 로딩하는 함수.
	void ReleaseHelperInfo() ;												// 도움말 정보를 해제하는 함수.

	HELPLINK* Get_HelpLink(int nIndex) ;									// 인덱스에 해당하는 도움말 링크를 리턴하는 함수.

	void Start_HelpTutorial(cTutorial* pTutorial) ;							// 도움말 링크에서 선택 된 튜토리얼을 실행하는 함수.


	// NETWORK PART.
	void NetworkMsgParse(BYTE Protocol,void* pMsg) ;						// 네트워크 메시지 파싱 함수.


	// PROCESS PART.
	//void Process() ;														// 프로세스 함수.
	BOOL Check_StartCondition(cTutorial* pTutorial) ;						// 튜토리얼 발동 조건을 체크하는 함수.

	BOOL Check_NpcTalk(void* pMsg) ;										// npc와 대화를 하는 조건을 체크하는 함수.
	BOOL Check_NpcTalk_StartCondition(DWORD dwNpcIdx) ;						// npc와 대화 조건 중, 시작 조건을 체크하는 함수.
	BOOL Check_NpcTalk_EndCondition(DWORD dwNpcIdx) ;						// npc와 대화 조건 중, 완료 조건을 체크하는 함수.
	// 080506 LYW --- TutorialManager : 레벨업 시, 진행 가능한 tutorial이 있는지 확인 후, 있으면 진행.
	void Check_LevelUp() ;

	void Check_GetItem(CItem* pItem) ;										// 아이템을 습득했을 때 튜토리얼 진행 여부를 체크하는 함수.

	void Check_OpenDialog(DWORD dwWindowID, BOOL bVal) ;					// 창을 여는 완료 조건을 체크하는 함수.


	// ETC
	void Set_ActiveTutorial(BOOL bActive) { m_bActiveTutorial = bActive ; }	// 튜토리얼의 활성화 여부를 세팅하는 함수.
	BOOL Is_ActiveTutorial() { return m_bActiveTutorial ; }					// 튜토리얼의 활성화 여부를 반환하는 함수.

	void Set_CurTutorialIdx(int nIndex) { m_nCurTutorialIdx = nIndex ; }	// 현재 튜토리얼 인덱스를 세팅하는 함수.
	int  Get_CurTutorialIdx() { return m_nCurTutorialIdx ; }				// 현재 튜토리얼 인덱스를 반환하는 함수.

	void Set_TutorialDlg(cTutorialDlg* pDlg) { m_pTutorialDlg = pDlg ; }	// 멤버 튜토리얼 다이얼로그를 세팅하는 함수.
	void Set_HelperDlg(cHelperDlg* pDlg) { m_pHelperDlg = pDlg ; }			// 멤버 도움말 다이얼로그를 세팅하는 함수.
	void Set_TutorialBtnDlg(cTutorialBtnDlg* pDlg) { m_pBtnDlg = pDlg ; }	// 멤버 튜토리얼 알림 버튼 다이얼로그를 세팅하는 함수.
	cTutorialBtnDlg* Get_TutorialBtnDlg() { return m_pBtnDlg ; }			// 튜토리얼 알림 버튼 다이얼로그를 반환하는 함수.

	cPtrList* GetHelperList() { return &m_Helper_List ; }					// 도움말 링크 리스트를 반환하는 함수.

	void UpdateTutorial(int nCompleteIndex) ;								// 튜토리얼 정보를 업데이트 하는 함수.

	void Start_CurTutorial(cTutorial* pTutorial) ;							// 현재 튜토리얼을 진행하라는 함수.

	void Reset_MemberDlg() ;												// 멤버 다이얼로그들을 리셋팅 하는 함수.

	int Get_ShowBtnSndIdx() { return m_nShow_BtnSndIdx ; }					// 버튼 출연 사운드 인덱스를 반환하는 함수.
	int Get_EndTutorialSndIdx() { return m_nEndTutorial_SndIdx ; }			// 튜토리얼 완료 사운드 인덱스를 반환하는 함수.

	// 080502 LYW --- TutorialManager : 지정 된 타입의 튜토리얼이 완료 되었는지 반환하는 함수 추가.
	BYTE IsCompleteTutorial(BYTE byTutorialType) ;
	// 080502 LYW --- TutorialManager : 지정 된 타입의 튜토리얼을 시작하는 함수 추가.
	void Start_Specification_Tutorial(BYTE byTutorialType) ;
};





EXTERNGLOBALTON(cTutorialManager)