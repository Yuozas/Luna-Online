#ifndef __DIALOGUE_LIST__
#define __DIALOGUE_LIST__

#include "../MHFile.h"																	// ���� ���� Ŭ���� ����� �ҷ��´�.

#define MAX_DIALOGUE_COUNT 12800														// �ִ� ���̾�α� ī��Ʈ�� 12800���� �����Ѵ�.

#define STRESS_COLOR	RGB(0,255,255)													// ��Ʈ���� �÷��� �����Ѵ�.
#define NORMAL_COLOR	RGB(255,255,255)												// �Ϲ��÷��� �����Ѵ�.

class cDialogueList																		// ���̾�α� ����Ʈ Ŭ����.
{
protected:
	cPtrList	m_Dialogue[MAX_DIALOGUE_COUNT];											// ����Ʈ�� ���̾�α� �迭�� �ִ� ���̾�α� ī��Ʈ ũ�⸸ŭ �����Ѵ�.	

	DWORD		m_dwDefaultColor;														// �Ϲ� ������ ���� ����.
	DWORD		m_dwStressColor;														// ��Ʈ���� ������ ���� ����.

	void LoadDialogueList(DWORD dwId, CMHFile* fp);										// ���̾�α� ����Ʈ�� �ε��ϴ� �Լ�.

public:
	cDialogueList();																	// ������ �Լ�.
	virtual ~cDialogueList();															// �Ҹ��� �Լ�.

	void LoadDialogueListFile(char* filePath, char* mode = "rt");						// ���̾�α� ����Ʈ ������ �ε��ϴ� �Լ�.

	DIALOGUE* GetDialogue( DWORD dwMsgId, WORD wLine );									// ���̾�α� ������ ��ȯ�ϴ� �Լ�.

	void ParsingLine( DWORD dwId, char* buf );											// ���� ������ �Ľ��ϴ� �Լ�.

	void AddLine( DWORD dwId, char* str, DWORD color, WORD Line, WORD type);			// ������ �߰��ϴ� �Լ�.

};

#endif