#ifndef _HELPDICTIONARYMANAGER_
#define _HELPDICTIONARYMANAGER_

#include "../MHFile.h"


#define HELPDICMGR USINGTON(cHelpDicManager)

#define HELP_SCRIPT_PATH		"./Data/Script/Npc/Help_Script.bin"
#define HELP_DIALOGUE_PATH		"./Data/Script/Npc/Help_Msg.bin"
#define HELP_HYPERTEXT_PATH		"./Data/Script/Npc/Help_HyperText.bin"

class cPage;
class cDialogueList;
class cHyperTextList;

class cHelpDicManager 
{
	cPtrList			m_HelpDicList;

	cDialogueList*		m_pDialogue;
	cHyperTextList*		m_pHyperText;
	

	int nTest;


public:
//	//MAKESINGLETON(cHelpDicManager);

	cHelpDicManager();
	virtual ~cHelpDicManager();

	void Init();

	// Help µ¥ÀÌÅÍ ·Îµù
	virtual void LoadHelpDicInfo();
	virtual void LoadPageInfo(cPage* pPage, CMHFile* fp);

	virtual void AddPage( cPage* pPage );
	void DeleteAllPageInfo();


public:
	cPage* GetMainPage();					// ½ÃÀÛ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.
	cPage* GetPage( DWORD dwPageId );		// ¸µÅ©µÈ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.	

	cDialogueList* GetDialogueList() { return m_pDialogue; }
	cHyperTextList* GetHyperTextList() { return m_pHyperText; }
};
EXTERNGLOBALTON(cHelpDicManager)

#endif // _HELPDICTIONARYMANAGER_