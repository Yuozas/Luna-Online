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
//		GM�� ���� ���� �� �� ���� �޽����� ����ϱ� ���� ���� �޽��� ����
//=========================================================================
#ifdef _GMTOOL_

// ERROR MESSAGE CATION PART.
#define INIT_CATION		"cTutorialManager::Init"							// �ʱ�ȭ �Լ�.

#define LT_CATION		"cTutorialManager::LoadTutorial"					// Ʃ�丮�� �ε� ĸ�� ��¿� ���ڿ�.
#define LMT_CATION		"cTutorialManager::LoadMainTutorial"				// ���� Ʃ�丮�� �ε� ĸ�� ��¿� ���ڿ�.
#define LST_CATION		"cTutorialManager::LoadSubTutorial"					// ���� Ʃ�丮�� �ε� ĸ�� ��¿� ���ڿ�.

#define LM_CATION		"cTutorialManager::LoadMsg"							// �޽��� �ε� ĸ�� ��¿� ���ڿ�.
#define RML_CATION		"cTutorialManager::ReadMsgLine"						// �޽��� ���� ���� ĸ�� ��¿� ���ڿ�.

#define LHI_CATION		"cTutorialManager::LoadHelperInfo"					// ���� ���� �ε� ĸ�� ��¿� ���ڿ�.

#define NMP_CATION		"cTutorialManager::NetworkMsgParse"					// ��Ʈ��ũ ���� �߻� �Լ� ��¿� ���ڿ�.

#define CSC_CATION		"cTutorialManager::Check_StartCondition"			// ���� ���� üũ ĸ�� ��¿� ���ڿ�.
#define CNT_CATION		"cTutorialManager::Check_NpcTalk"					// NPC ��ȭ ���� üũ ĸ�� ��¿� ���ڿ�.
#define CGI_CATION		"cTutorialManager::Check_GetItem"					// ������ üũ ĸ�� ��¿� ���ڿ�.
#define COD_CATION		"cTutorialManager::Check_OpenDialog"				// ������ ���̾�α� ���� üũ ĸ�� ��¿� ���ڿ�.

#define SCT_CATION		"cTutorialManager::Start_CurTutorial"				// Ʃ�丮�� ���� ĸ�� ��¿� ���ڿ�.

#define	SHT_CATION		"cTutorialManager::Start_HelpTutorial"				// ���� Ʃ�丮�� ���� ĸ�� ��¿� ���ڿ�.

// INITIALIZE PART.
#define ERR_MSG			"Failed to load tutorial message"					// Ʃ�丮�� �޽��� �ε忡 �����ߴٴ� ���� �޽���.
#define ERR_TUTORIAL	"Failed to load tutorial info"						// Ʃ�丮�� ���� �ε忡 �����ߴٴ� ���� �޽���.
#define ERR_HELPERLINK	"Failed to load helper info"						// ���� ���� �ε忡 �����ߴٴ� ���� �޽���.

// COMMON PART.
#define CERR_MSG1		"Invalid file pointer"								// ���ڷ� �Ѿ�� ���� �����Ͱ� �߸��Ǿ��ٴ� ���� �޽���.
#define CERR_MSG2		"Over angle bracket } : %d - %d"					// } ��ȣ �ݱⰡ �߸� �Ǿ��ٴ� ���� �޽���.
#define CERR_MSG3		"Over angle bracket { : %d - %d"					// { ��ȣ ���Ⱑ �߸� �Ǿ��ٴ� ���� �޽���.	
#define CERR_MSG4		"Invalid parameter"									// �߸��� ���ڰ� �Ѿ�Դٴ� ���� �޽���.
#define CERR_MSG5		"Invalid tutorial dialog."							// ��� Ʃ�丮�� ���̾�α� ���� ����.

// TUTORIAL PART.
#define TERR_MSG1		"Invalid file name~!!"								// Ʃ�丮�� ��ũ��Ʈ ���� ���� �߸� �Ǿ��ٴ� ���� �޽���.
#define TERR_MSG2		"Failed to open file : %s"							// ���� ���⿡ ���� �ߴٴ� ���� �޽���.
#define TERR_MSG3		"Invalid Tutorial Info : %d"						// Ʃ�丮�� ������ �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG4		"Invalid Tutorial Index : %d"						// Ʃ�丮�� �ε����� �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG5		"Invalid start condition 1 : %d"					// �� 1 �ߵ� ������ �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG6		"Failed to create tutorial : %d"					// Ʃ�丮�� ������ ���� �ߴٴ� ���� �޽���.
#define TERR_MSG7		"Invalid tutorial pointer"							// ���ڷ� �Ѿ�� Ʃ�丮�� �����Ͱ� �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG8		"Invalid speech index : %d - %d"					// ��ȭ �ε����� �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG9		"Invalid sound index : %d - %d"						// ���� �ε����� �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG10		"Invalid npc index : %d - %d"						// NPC �ε����� �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG11		"Invalid face index : %d - %d"						// �� �ε����� �߸� �Ǿ��ٴ� ���� �޽���.
#define TERR_MSG12		"Invalid first condition index : %d - %d"			// ù��° �Ϸ� ������ �߸��Ǿ��ٴ� ���� �޽���.
#define TERR_MSG13		"Invalid Sub Tutorial Info : %d - %d"				// ���� Ʃ�丮�� ������ �߸� �Ǿ��ٴ� ���� �޽���.
#define TERR_MSG14		"Failed to create sub tutorial : %d - %d"			// ��Ʈ Ʃ�丮�� ������ ���� �ߴٴ� ���� �޽���.
#define TERR_MSG15		"Failed to receive completed tutorial : %d"			// �Ϸ��� Ʃ�丮�� ������ �޴µ� ���� �ߴٴ� ����.
#define TERR_MSG16		"Failed to receive current sub tutorial"			// ���� �������� ���� Ʃ�丮�� �޴µ� ���� �ߴٴ� ����.
#define TERR_MSG17		"Failed to receive window id : %s"					// ������ ���ڿ��� ������ ���̵� �ޱ� ����.

// TUTORIAL MESSAGE PART.
#define MERR_MSG1		"Over string - Msg %d"								// �޽��� ���̸� �Ѿ��ٴ� ���� �޽���.
#define MERR_MSG2		"Failed to create message - Msg %d"					// �޽��� ���̸� �Ѿ��ٴ� ���� �޽���.
#define MERR_MSG3		"Invalid message Info : %d"							// �߸��� �޽��� ������� ���� �޽���.
#define MERR_MSG4		"Invalid message index : %d"						// �߸��� �޽��� �ε������ ���� �޽���.

// HELPER PART.
#define HERR_MSG1		"Invalid link index : %d"							// ���� ��ũ �ε����� �߸� �Ǿ��ٴ� ���� �޽���.
#define HERR_MSG2		"Invalid link title : %d"							// ���� ��ũ ������ �߸� �Ǿ��ٴ� ���� �޽���.
#define HERR_MSG3		"Invalid linked tutorial index : %d"				// ���� ��ũ�� Ʃ�丮�� �ε����� ��ȿ���� �ʴٴ� ���� �޽���.
#define HERR_MSG4		"Failed to create help link : %d"					// ���� ��ũ ������ ���� �ߴٴ� ���� �޽���.

// PROCESS PART.
#define PERR_MSG1		"Invalid tutorial Info. Can't start tutorial! : %d"	// �߸��� Ʃ�丮�� ������ ���� Ʃ�丮�� ������ ���� ���Ѵٴ� ����.
#define PERR_MSG2		"Failed to receive npc info : %d npc"				// �� �� npc ������ �޴µ� ���� �ߴٴ� ���� �޽���.
#define PERR_MSG3		"Invalid pointer pItem~!!"							// ���ڷ� �Ѿ�� pItem�� �߸��Ǿ��ٴ� ����.
#define PERR_MSG4		"Failed to receive inventory dialog."				// �κ��丮 ���̾�α׸� ���� ���ߴٴ� ���� �޽���.
#define PERR_MSG5		"Failed to receive list dialog of help dialog."		// ���� â�� ����Ʈ ���̾�α׸� ���� ���ߴٴ� ����.

// NETWORK PART.
#define NERR_MSG1		"Failed to casting tutorial data in networpaser"	// ��Ʈ��ũ �ļ����� Ʃ�丮�� ������ �� ��ȯ�� ���� �ߴٴ� ����.
#define NERR_MSG2		"Invalid tutorial index"							// �߸� �� Ʃ�丮�� �ε��� ����.
#define NERR_MSG3		"Failed to receive tutorial position : %d"			// �� �� �ε����� Ʃ�丮�� ������ �޴µ� ���� �ߴٴ� ����.
#define NERR_MSG4		"Failed to receive tutorial info : %d"				// �ε����� �ش��ϴ� Ʃ�丮�� ������ �޴µ� ���� �ߴٴ� ����.

#endif





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "../MHFile.h"														// ���� ���� �������̽��� �����Ѵ�.
#include "..\[lib]yhlibrary\PtrList.h"										// Ʃ�丮�� ���� ������ �����ϱ� ���� ����Ʈ �������̽��� �����Ѵ�.





//=========================================================================
//		�ʼ� ���� ��Ʈ.
//=========================================================================
#define TUTORIALMGR		USINGTON(cTutorialManager)

#define TUTORIAL_MSG_PATH		"./Data/Script/Tutorial/Tutorial_Msg.bin"	// Ʃ�丮�� �޽��� ��θ� �����Ѵ�.
#define TUTORIAL_SCRIPT_PATH	"./Data/Script/Tutorial/Tutorial_Script.bin"// Ʃ�丮�� ��ũ��Ʈ ��θ� �����Ѵ�.	
#define HELPER_LINKLIST_PATH	"./Data/Script/Tutorial/Help_Link_List.bin" // ���� ��ũ ����Ʈ ��θ� �����Ѵ�.

#define NPC_EMOTION_MAX		5												// ��ǿ� ����� �� �ִ� �ִ� �ε���.

#define FIRST_GAMEIN_TUTORIAL_IDX	0										// ó�� ���ӿ� ���� ���� ��, ������ �����ϴ� Ʃ�丮�� �ε���.

enum ENUM_TUTORIAL_TYPE
{
	e_TT_FirstGameIn = 0 ,
	e_TT_LevelUp,
	e_TT_Max,
} ;





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class CItem ;																// ������ Ŭ������ �����Ѵ�.
class cTutorial ;															// Ʃ�丮�� Ŭ������ �����Ѵ�.
class cTutorial_Msg ;														// Ʃ�丮�� �޽��� Ŭ������ �����Ѵ�.

class cTutorialDlg ;														// Ʃ�丮�� ������ ���� Ʃ�丮�� ���̾�α� Ŭ������ �����Ѵ�.
class cHelperDlg ;															// ���� ������ ���� ���� ���̾�α� Ŭ������ �����Ѵ�.
class cTutorialBtnDlg ;														// Ʃ�丮�� �˸� ��ư ���̾�α� Ŭ������ �����Ѵ�.

struct SUB_TUTORIAL ;														// ���� Ʃ�丮�� ����ü�� �����Ѵ�.

struct HELPLINK																// ���� ��ũ ����ü�� �����Ѵ�.
{	
	int nIndex ;															// ��� �� ����Ʈ �ε���.
	int nTutorial_Idx ;														// Ŭ���� ���� �� Ʃ�丮�� �ε���.
	char Title[128] ;														// ��� �� ����Ʈ Ÿ��Ʋ.
} ;





//=========================================================================
//		Ʃ�丮�� �Ŵ��� Ŭ���� ����
//=========================================================================
class cTutorialManager														// Ʃ�丮���� ������ �Ŵ��� Ŭ������ �����Ѵ�.
{
	cPtrList			m_Tutorial_Msg ;									// Ʃ�丮�� �޽����� �����ϴ� ����Ʈ.
	cPtrList			m_Tutorial_List ;									// Ʃ�丮�� ������ �����ϴ� ����Ʈ.
	cPtrList			m_Helper_List ;										// ���� Ŭ������ �����ϴ� Ʃ�丮�� ������ �����ϴ� ����Ʈ.

	BOOL				m_bOnOff_Tutorial ;									// Ʃ�丮���� ���̱� ���� ����.

	cTutorialDlg*		m_pTutorialDlg ;									// Ʃ�丮�� ������ ���� ���̾�α� ������.
	cHelperDlg*			m_pHelperDlg ;										// ���� ��� ������ ���� ���̾�α� ������.
	cTutorialBtnDlg*	m_pBtnDlg ;											// Ʃ�丮�� �˸� ��ư ���̾�α� ������.

	BOOL				m_bActiveTutorial ;									// Ʃ�丮���� ���̱� ����.
	BOOL				m_bUpdated ;										// Ʃ�丮�� �̺�Ʈ�� ������Ʈ �Ǿ����� üũ�ϴ� ����.

	int					m_nCurTutorialIdx ;									// ���� �������� Ʃ�丮�� �ε���.
	int					m_nCompletedIdx ;									// �Ϸ� �� Ʃ�丮�� �ε���.

	int					m_nShow_BtnSndIdx ;									// Ʃ�丮�� �˸� ��ư�� ������ �� play �Ǵ� ���� �ε���.
	int					m_nEndTutorial_SndIdx ;								// Ʃ�丮���� �Ϸ� ���� �� play �Ǵ� ���� �ε���.

public:
	// BASE PART.
	cTutorialManager(void);													// ������ �Լ�.
	virtual ~cTutorialManager(void);										// �Ҹ��� �Լ�.

	void Init() ;															// �ʱ�ȭ �Լ�.
	void Release() ;														// ���� �Լ�.


	// TUTORIAL PART.
	BOOL LoadTutorial(char* strFileName) ;									// Ʃ�丮�� ������ �ε��ϴ� �Լ�.
	void ReleaseTutorial() ;												// Ʃ�丮�� ������ �����ϴ� �Լ�.

	cTutorial* LoadMainTutorial(CMHFile* pFile) ;							// ���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.
	BOOL LoadSubTutorial(CMHFile* pFile, cTutorial* pTutorial) ;			// ���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.

	cTutorial* Get_Tutorial(int nIndex) ;									// �ε����� �ش��ϴ� Ʃ�丮�� ������ �����ϴ� �Լ�.


	// TUTORIAL MESSAGE PART.
	BOOL LoadMsg(char* strFileName) ;										// Ʃ�丮�� �޽����� �ε��ϴ� �Լ�.
	void ReleaseMsg() ;														// Ʃ�丮�� �޽����� �����ϴ� �Լ�.

	BOOL ReadMsgLine(CMHFile* pFile, cTutorial_Msg* pMsg) ;					// Ʃ�丮�� �޽����� ������ �д� �Լ�.

	cTutorial_Msg* Get_TutorialMsg(int nIndex) ;							// �ε����� �ش��ϴ� Ʃ�丮�� �޽��� ������ �����ϴ� �Լ�.


	// HELPER PART.
	BOOL LoadHelperInfo(char* strFileName) ;								// ���� ������ �ε��ϴ� �Լ�.
	void ReleaseHelperInfo() ;												// ���� ������ �����ϴ� �Լ�.

	HELPLINK* Get_HelpLink(int nIndex) ;									// �ε����� �ش��ϴ� ���� ��ũ�� �����ϴ� �Լ�.

	void Start_HelpTutorial(cTutorial* pTutorial) ;							// ���� ��ũ���� ���� �� Ʃ�丮���� �����ϴ� �Լ�.


	// NETWORK PART.
	void NetworkMsgParse(BYTE Protocol,void* pMsg) ;						// ��Ʈ��ũ �޽��� �Ľ� �Լ�.


	// PROCESS PART.
	//void Process() ;														// ���μ��� �Լ�.
	BOOL Check_StartCondition(cTutorial* pTutorial) ;						// Ʃ�丮�� �ߵ� ������ üũ�ϴ� �Լ�.

	BOOL Check_NpcTalk(void* pMsg) ;										// npc�� ��ȭ�� �ϴ� ������ üũ�ϴ� �Լ�.
	BOOL Check_NpcTalk_StartCondition(DWORD dwNpcIdx) ;						// npc�� ��ȭ ���� ��, ���� ������ üũ�ϴ� �Լ�.
	BOOL Check_NpcTalk_EndCondition(DWORD dwNpcIdx) ;						// npc�� ��ȭ ���� ��, �Ϸ� ������ üũ�ϴ� �Լ�.
	// 080506 LYW --- TutorialManager : ������ ��, ���� ������ tutorial�� �ִ��� Ȯ�� ��, ������ ����.
	void Check_LevelUp() ;

	void Check_GetItem(CItem* pItem) ;										// �������� �������� �� Ʃ�丮�� ���� ���θ� üũ�ϴ� �Լ�.

	void Check_OpenDialog(DWORD dwWindowID, BOOL bVal) ;					// â�� ���� �Ϸ� ������ üũ�ϴ� �Լ�.


	// ETC
	void Set_ActiveTutorial(BOOL bActive) { m_bActiveTutorial = bActive ; }	// Ʃ�丮���� Ȱ��ȭ ���θ� �����ϴ� �Լ�.
	BOOL Is_ActiveTutorial() { return m_bActiveTutorial ; }					// Ʃ�丮���� Ȱ��ȭ ���θ� ��ȯ�ϴ� �Լ�.

	void Set_CurTutorialIdx(int nIndex) { m_nCurTutorialIdx = nIndex ; }	// ���� Ʃ�丮�� �ε����� �����ϴ� �Լ�.
	int  Get_CurTutorialIdx() { return m_nCurTutorialIdx ; }				// ���� Ʃ�丮�� �ε����� ��ȯ�ϴ� �Լ�.

	void Set_TutorialDlg(cTutorialDlg* pDlg) { m_pTutorialDlg = pDlg ; }	// ��� Ʃ�丮�� ���̾�α׸� �����ϴ� �Լ�.
	void Set_HelperDlg(cHelperDlg* pDlg) { m_pHelperDlg = pDlg ; }			// ��� ���� ���̾�α׸� �����ϴ� �Լ�.
	void Set_TutorialBtnDlg(cTutorialBtnDlg* pDlg) { m_pBtnDlg = pDlg ; }	// ��� Ʃ�丮�� �˸� ��ư ���̾�α׸� �����ϴ� �Լ�.
	cTutorialBtnDlg* Get_TutorialBtnDlg() { return m_pBtnDlg ; }			// Ʃ�丮�� �˸� ��ư ���̾�α׸� ��ȯ�ϴ� �Լ�.

	cPtrList* GetHelperList() { return &m_Helper_List ; }					// ���� ��ũ ����Ʈ�� ��ȯ�ϴ� �Լ�.

	void UpdateTutorial(int nCompleteIndex) ;								// Ʃ�丮�� ������ ������Ʈ �ϴ� �Լ�.

	void Start_CurTutorial(cTutorial* pTutorial) ;							// ���� Ʃ�丮���� �����϶�� �Լ�.

	void Reset_MemberDlg() ;												// ��� ���̾�α׵��� ������ �ϴ� �Լ�.

	int Get_ShowBtnSndIdx() { return m_nShow_BtnSndIdx ; }					// ��ư �⿬ ���� �ε����� ��ȯ�ϴ� �Լ�.
	int Get_EndTutorialSndIdx() { return m_nEndTutorial_SndIdx ; }			// Ʃ�丮�� �Ϸ� ���� �ε����� ��ȯ�ϴ� �Լ�.

	// 080502 LYW --- TutorialManager : ���� �� Ÿ���� Ʃ�丮���� �Ϸ� �Ǿ����� ��ȯ�ϴ� �Լ� �߰�.
	BYTE IsCompleteTutorial(BYTE byTutorialType) ;
	// 080502 LYW --- TutorialManager : ���� �� Ÿ���� Ʃ�丮���� �����ϴ� �Լ� �߰�.
	void Start_Specification_Tutorial(BYTE byTutorialType) ;
};





EXTERNGLOBALTON(cTutorialManager)