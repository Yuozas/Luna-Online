

#include "stdafx.h"													// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "cPage.h"													// ������ Ŭ���� ����� �ҷ��´�.

/////////////////////////////////
// cPageBase Class
/////////////////////////////////
cPageBase::cPageBase()												// ������ �Լ�.
{
	m_nDialogueCount = 0;											// ���̾�α� ī��Ʈ�� 0���� �����Ѵ�.
	
	m_nNextPageId = -1;												// ���� ������ ���̵� -1�� �����Ѵ�.
	m_nPrevPageId = -1;												// ���� ������ ���̵� -1�� �����Ѵ�.

	mEmotion = 0;													// �̸���� 0���� �����Ѵ�.
}


cPageBase::~cPageBase()												// �Ҹ��� �Լ�.
{
	RemoveAll();													// ��� ���� �Լ��� ȣ���Ѵ�.
}

void cPageBase::RemoveAll()											// ��� ���� �Լ�.
{
	POS pos = m_pDialogue.GetFirstPos();							// ���̾�α� ����Ʈ�� ù ��ġ ������ �޴´�.

	if(!pos) return ;												// ��ġ ������ ��ȿ���� üũ�Ѵ�.

	DWORD* pData = NULL ;											// ������ ���� ������ ������ �����ϰ�, NULL ó���� �Ѵ�.

	while( pData = m_pDialogue.GetNextPos( pos ) )					// ��ġ ������ �ش��ϴ� ������ �޴´�.
	{
		delete pData;												// ������ �����Ѵ�.
		pData = NULL ;												// ������ ��Ÿ���� �����͸� NULL ó���� �Ѵ�.
	}

	m_pDialogue.DeleteAll();										// ���̾�α� ����Ʈ�� ��� ����.
}

void cPageBase::Init( DWORD dwId, BYTE emotion )					// �ʱ�ȭ �Լ�.
{
	m_dwPageId = dwId;												// ������ ���̵� �����Ѵ�.

	mEmotion = emotion;												// �̸���� �����Ѵ�.
}

void cPageBase::AddDialogue(DWORD dwId)								// ���̾�α� �߰��Լ�.
{
	DWORD* dwVal = new DWORD;										// DWORD�� �޸𸮸� �Ҵ��Ѵ�.

	*dwVal = dwId;													// ���̵� �����Ѵ�.

	m_pDialogue.AddTail(dwVal);										// ���̾�α� ����Ʈ�� �߰��Ѵ�.

	++m_nDialogueCount;												// ���̾�α� ī��Ʈ�� �����Ѵ�.
}

DWORD cPageBase::GetDialogue()
{
	if( m_nDialogueCount < 1 ) return 0;							// ���̾�α� ī��Ʈ�� 0�� ������ 0�� �����Ѵ�.
	else
	{
		return *m_pDialogue.GetAt(0) ;
	}
}

DWORD cPageBase::GetDialogue(int index)
{
	if( m_nDialogueCount < 1 ) return 0 ;
	else if( index < 0 || index  > m_nDialogueCount - 1 )
	{
		return 0 ;
	}
	else
	{
		return *m_pDialogue.GetAt(index) ;
	}
}

DWORD cPageBase::GetRandomDialogue()								// ���� ���̾�α׸� ��ȯ�ϴ� �Լ�.
{
	if( m_nDialogueCount == 0 ) return 0;							// ���̾�α� ī��Ʈ�� 0�� ������ 0�� �����Ѵ�.


	if( m_nDialogueCount > 1 )										// ���̾�α� ī��Ʈ�� 1�� ���� ���ٸ�,
	{
		return *(m_pDialogue.GetAt(rand()%m_nDialogueCount));		// �����ϰ� ���̾�α� ������ �����Ѵ�.
	}
	else															// ���̾�α� ī��Ʈ�� 1�� ���ϸ�,
	{
		return *(m_pDialogue.GetAt(0));								// ù ��° ���̾�α� ������ �����Ѵ�.
	}
}

/////////////////////////////////
// cPage Class
/////////////////////////////////
cPage::cPage()														// ������ �Լ�.
{
	m_nHyperLinkCount = 0;											// ������ ��ũ ī��Ʈ�� 0���� �����Ѵ�.
}

cPage::~cPage()														// �Ҹ��� �Լ�.
{
	RemoveAll();													// ��� ���� �Լ��� ȣ���Ѵ�.
}

void cPage::RemoveAll()												// ��� ���� �Լ�.
{
	POS pos = m_pHyperLink.GetFirstPos();							// ������ ��ũ ����Ʈ�� ù ��ġ ������ �޴´�.

	if( !pos ) return ;												// ��ġ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	HYPERLINK* pHL = NULL ;											// ������ ��ũ ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	while( pHL = m_pHyperLink.GetNextPos( pos ) )					// ������ ������ ��ȿ�� ���� while���� ������.
	{
		delete pHL;													// ������ �����Ѵ�.
		pHL = NULL ;												// �����͸� NULL ó���� �Ѵ�.
	}

	m_pHyperLink.DeleteAll();										// ������ ��ũ ����Ʈ�� ��� ����.
}

void cPage::AddHyperLink(HYPERLINK* pLink)							// ������ ��ũ ������ �߰��ϴ� �Լ�.
{
	HYPERLINK* pVal = new HYPERLINK;								// ������ ��ũ�� ���� �޸𸮸� �Ҵ��Ѵ�.

	memcpy( pVal, pLink, sizeof(HYPERLINK) );						// ������ ��ũ ������ �����Ѵ�.

	m_pHyperLink.AddTail( pVal );									// ������ ��ũ ����Ʈ�� ������ �߰��Ѵ�.

	++m_nHyperLinkCount;											// ������ ��ũ ī��Ʈ�� �����Ѵ�.
}

HYPERLINK* cPage::GetHyperText( DWORD dwIdx )						// ������ ��ũ ������ ��ȯ�ϴ� �Լ�.
{
	return m_pHyperLink.GetAt(dwIdx);								// �ε����� �ش��ϴ� ������ ��ũ ������ ��ȯ�Ѵ�.
}