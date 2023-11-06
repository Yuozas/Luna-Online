#ifndef _HYPERTEXTLIST_
#define _HYPERTEXTLIST_

//#include "PtrList.h"

class cHyperTextList													// ������ �ؽ��� ��Ʈ�� Ŭ����.
{
	CYHHashTable<DIALOGUE>	m_HyperText;								// ���̾�α� ������ ��� ���̺�.
//	cPtrList	m_HyperText;
public:
	cHyperTextList();													// ������ �Լ�.
	virtual ~cHyperTextList();											// �Ҹ��� �Լ�.

	void LoadHyperTextFormFile(char* filePath, char* mode = "rt");		// ���Ϸ� ���� ������ �ؽ�Ʈ�� �ε��ϴ� �Լ�.

	DIALOGUE* GetHyperText( DWORD dwIdx );								// ���̾�α� ������ ��ȯ�ϴ� �Լ�.
};

#endif