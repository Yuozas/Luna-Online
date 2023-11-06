

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "../MHFile.h"														// ���� ���� Ŭ���� ����� �ҷ��´�.

#include "cHyperTextList.h"													// ������ �ؽ�Ʈ ����Ʈ Ŭ���� ����� �ҷ��´�.


cHyperTextList::cHyperTextList()											// ������ �Լ�.
{
	m_HyperText.Initialize(1000);											// ���̾�α� ������ ��� ���̺��� 1000���� �ʱ�ȭ �Ѵ�.
}

cHyperTextList::~cHyperTextList()											// �Ҹ��� �Լ�.
{
	m_HyperText.SetPositionHead();											// ���̺��� ��� ���������� �����Ѵ�.

	DIALOGUE* value;														// ���̾�α� ������ ���� �����͸� �����Ѵ�.

	while(value = m_HyperText.GetData())									// ���̾�α� ������ ���� ������ while���� ������.
	{
		delete value;														// ���̾�α� ������ �����Ѵ�.
	}
			
	m_HyperText.RemoveAll();												// ���̺��� ��� ����.
}

void cHyperTextList::LoadHyperTextFormFile(char* filePath, char* mode)		// ���Ϸ� ���� ������ �ؽ�Ʈ ������ �ε��ϴ� �Լ�.
{
	int nLen;																// ���� ������ ���� ����.

	DIALOGUE* pTemp;														// ���̾�α� ������ ���� ������.

	CMHFile fp;																// ���� ������ �����Ѵ�.

	if(!fp.Init(filePath, mode))											// ���� ����� ������ ���� ���� ���⿡ �����ϸ�,
	{
		return;																// ���� ó���� �Ѵ�.
	}

	char buff[256]={0,};													// �ӽ� ���۸� �����Ѵ�.

	DWORD idx;																// �ε��� ������ ���� ������ �����Ѵ�.

	while(1)																// while���� ������.
	{
		idx = fp.GetInt();													// �ε����� �޴´�.

		if(fp.IsEOF())														// �����Ͱ� ������ ���� ����Ű��,
		{
			break;															// while���� Ż���Ѵ�.
		}

		fp.GetLine( buff, 256 );											// �ӽ� ���ۿ� ������ �о���δ�.

		if( nLen = strlen(buff) )											// ���� ������ ������ ũ�Ⱑ ������,
		{
			pTemp = new DIALOGUE;											// ���̾�α� ���� �޸𸮸� �Ҵ��Ѵ�.

			pTemp->Init();													// ���̾�α� ������ �ʱ�ȭ �Ѵ�.

			strcpy(pTemp->str, buff);										// ���̾�α� ������ ��Ʈ���� ������ ������ �����Ѵ�.

			m_HyperText.Add(pTemp, idx);									// ������ �ؽ�Ʈ�� �߰��Ѵ�.
		}
	}
}

DIALOGUE* cHyperTextList::GetHyperText( DWORD dwIdx )						// ���̾�α� ������ ��ȯ�ϴ� �Լ�.
{
	return (DIALOGUE*)m_HyperText.GetData(dwIdx);							// �޾Ƶ��� �ε����� �ش��ϴ� ���̾�α� ������ ��ȯ�Ѵ�.
}