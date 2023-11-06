#ifndef __NPCSCRIPT_MANAGER__
#define __NPCSCRIPT_MANAGER__

#include "../MHFile.h"																// 묵향 파일 클래스 헤더를 불러온다.

#define NPCSCRIPTMGR USINGTON(cNpcScriptManager)									// NPC 스크립트 매니져를 유징톤 한다.

#define MAX_NPC_COUNT 1000															// 최대 NPC 수를 1000으로 디파인한다.

#define NPC_SCRIPT_PATH		"./Data/Script/Npc/Npc_Script.bin"						// 스크립트 파일 패스를 디파인 한다.
#define NPC_DIALOGUE_PATH	"./Data/Script/Npc/Npc_Msg.bin"							// 다이얼로그 패스를 디파인 한다.
#define NPC_HYPERTEXT_PATH	"./Data/Script/Npc/Npc_HyperText.bin"					// 하이퍼 텍스프 패스를 디파인 한다.

class cPage;																		// 페이지 클래스를 선언한다.
class cDialogueList;																// 다이얼로그 리스트 클래스를 선언한다.
class cHyperTextList;																// 하이퍼 텍스트 리스트를 선언한다.
class CNpc;																			// NPC 클래스 를 선언한다.

class cNpcScriptManager																// NPC 스크립트 매니져 클래스.
{
protected:
	cPtrList			m_NpcScriptList[MAX_NPC_COUNT];								// 최대 NPC 수 만큼 스크립트 리스트를 선언한다.

	int					m_nRegistNpcCount;											// 등록된 NPC 카운트.

	cDialogueList*		m_pDialogue;												// 다이얼로그 리스트 포인터.

	cHyperTextList*		m_pHyperText;												// 하이퍼 텍스트 리스트 포인터.

	CNpc*				m_pCurSelectedNpc;											// 현재 선택 된 NPC 포인터.

	CNpc*				m_pLastNpc ;												// 마지막 선택 한 NPC 포인터.

public:
	cNpcScriptManager();															// 생성자 함수.
	virtual ~cNpcScriptManager();													// 소멸자 함수.
	
	void Init();																	// 초기화 함수.

	void AddNpcScriptInfo( DWORD dwNpcId, cPage* pPage );							// NPC 스크립트 정보를 추가하는 함수.
	void DeleteAllScriptInfo();														// 모든 스크립트 정보를 삭제하는 함수.

	cPage* GetMainPage( DWORD dwNpcId );											// 메인 페이지 정보를 반환하는 함수.
	cPage* GetPage( DWORD dwNpcId, DWORD dwPageId );								// 원하는 페이지 정보를 반환하는 함수.

	void LoadNpcScriptInfo();														// NPC 스크립트 정보를 로딩하는 함수.
	void LoadPageInfo(cPage* pPage, CMHFile* fp);									// 페이지 정보를 로딩하는 함수.

	cDialogueList* GetDialogueList() { return m_pDialogue; }						// 다이얼로그 리스트 정보를 반환하는 함수.
	cHyperTextList* GetHyperTextList() { return m_pHyperText; }						// 하이퍼 텍스트 리트스 정보를 반환하는 함수.

	void SetSelectedNpc( CNpc* pNpc ) { m_pCurSelectedNpc = pNpc; }					// 선택 된 NPC를 세팅하는 함수.
	CNpc* GetSelectedNpc() { return m_pCurSelectedNpc; }							// 선택 된 NPC를 반환하는 함수.

	void StartNpcScript( CNpc* pNpc );												// NPC 스크립트를 시작하는 함수.
	void StartNpcBusiness(DWORD dwData, DWORD dwParam=0);							// NPC 비지니스를 시작하는 함수.

	void SetLastNpc( CNpc* pNpc ) { m_pLastNpc = pNpc ; }							// 마지막으로 선택한 NPC를 세팅하는 함수.
	CNpc* GetLastNpc() { return m_pLastNpc ; }										// 마지막으로 선택한 NPC를 반환하는 함수.

	void Business_Dealer() ;														// 딜러 비지니스.
	void Business_Castle(DWORD dwData) ;											// (공성)성 비지니스.
	void Business_Changgo(DWORD dwData, DWORD dwParam) ;							// 창고 비지니스.
	void Business_Guild(DWORD dwData) ;												// 길드 비지니스.
	void Business_Wanted(DWORD dwData) ;											// 현상수배 비지니스.
	void Business_Suryun(DWORD dwData) ;											// 수련 비지니스.
	void Business_MapChange(DWORD dwData) ;											// 맵 체인지 비지니스.
	void Business_Symbol() ;														// 심볼 비지니스.
	void Business_Family(DWORD dwData) ;											// 패밀리 비지니스.
	void Business_Identifycation() ;												// 주민등록 비지니스.
};
EXTERNGLOBALTON(cNpcScriptManager)
#endif



//#ifndef __NPCSCRIPT_MANAGER__
//#define __NPCSCRIPT_MANAGER__
//
//#include "../MHFile.h"
//
//
//#define NPCSCRIPTMGR USINGTON(cNpcScriptManager)
//
//#define MAX_NPC_COUNT 1000 // Á»´õ »ý°¢¿ä!!
//
//#define NPC_SCRIPT_PATH		"./Data/Script/Npc/Npc_Script.bin"
//#define NPC_DIALOGUE_PATH	"./Data/Script/Npc/Npc_Msg.bin"
//#define NPC_HYPERTEXT_PATH	"./Data/Script/Npc/Npc_HyperText.bin"
//
//class cPage;
//class cDialogueList;
//class cHyperTextList;
//class CNpc;
//
//class cNpcScriptManager 
//{
//protected:
//	// NPC ¸®½ºÆ® ÇÊ¿ä
//	cPtrList			m_NpcScriptList[MAX_NPC_COUNT];
//	int					m_nRegistNpcCount;
//
//	cDialogueList*		m_pDialogue;
//	cHyperTextList*		m_pHyperText;
//
//	CNpc*				m_pCurSelectedNpc; // ¼±ÅÃ Npc¿¡ ´ëÇÑ Á¤º¸
//	// 070326 LYW --- NpcScriptManager : Add npc pointer.
//	CNpc*				m_pLastNpc ;
//public:
////	//MAKESINGLETON(cNpcScriptManager);
//
//	cNpcScriptManager();
//	virtual ~cNpcScriptManager();
//	
//	void Init();
//
//	void AddNpcScriptInfo( DWORD dwNpcId, cPage* pPage );
//	void DeleteAllScriptInfo();
//
//	// ÆäÀÌÁö Á¤º¸
//	cPage* GetMainPage( DWORD dwNpcId );					// ½ÃÀÛ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.
//	cPage* GetPage( DWORD dwNpcId, DWORD dwPageId );		// ¸µÅ©µÈ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.
//
//	// 1. NPC Script µ¥ÀÌÅÍ ·Îµù.. 
//	void LoadNpcScriptInfo();
//	void LoadPageInfo(cPage* pPage, CMHFile* fp);
//
//	cDialogueList* GetDialogueList() { return m_pDialogue; }
//	cHyperTextList* GetHyperTextList() { return m_pHyperText; }
//
//	void SetSelectedNpc( CNpc* pNpc ) { m_pCurSelectedNpc = pNpc; }
//	CNpc* GetSelectedNpc() { return m_pCurSelectedNpc; }
//
//	// 
//	void StartNpcScript( CNpc* pNpc );
//	void StartNpcBusiness(DWORD dwData, DWORD dwParam=0);
//
//	// 070326 LYW --- Add function to setting and return last npc.
//	void SetLastNpc( CNpc* pNpc ) { m_pLastNpc = pNpc ; }
//	CNpc* GetLastNpc() { return m_pLastNpc ; }
//};
//EXTERNGLOBALTON(cNpcScriptManager)
//#endif