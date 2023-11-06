// QuestInfo.cpp: implementation of the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestInfo.h"																// ����Ʈ ���� ��������� �ҷ��´�.

#include "SubQuestInfo.h"															// ���� ����Ʈ ���� ��� ������ �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestInfo::CQuestInfo( DWORD dwQuestIdx )											// ������ �Լ�.
{
	m_dwQuestIdx = dwQuestIdx;														// ����Ʈ �ε����� �����Ѵ�.

	m_dwSubQuestCount = 0;															// ���� ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
	m_dwEndParam = 0;																// ���� �Ķ���͸� 0���� �����Ѵ�.
	m_SubQuestTable.Initialize( MAX_SUBQUEST );										// ���� ����Ʈ ���� ���̺��� �ִ� ���� ����Ʈ ������ �ʱ�ȭ �Ѵ�.

	m_bRepeat = FALSE;
}

CQuestInfo::~CQuestInfo()															// �Ҹ��� �Լ�.
{
	Release();																		// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestInfo::Release()															// ���� �Լ�.
{
	CSubQuestInfo* pSubQuestInfo = NULL;											// ���� ����Ʈ ������ ���� ������ ������ �����ϰ�, null ó���� �Ѵ�.

	m_SubQuestTable.SetPositionHead();												// ���� ����Ʈ ���̺��� ���� �����Ѵ�.

	while( pSubQuestInfo = m_SubQuestTable.GetData() )								// ���� ����Ʈ ������ �ִ� ���� while ���� ������.
	{
		delete pSubQuestInfo;														// ���� ����Ʈ ������ �����Ѵ�.
	}

	m_SubQuestTable.RemoveAll();													// ���� ����Ʈ ���� ���̺��� ��� ����.
}

void CQuestInfo::AddSubQuestInfo( DWORD dwSubQuestIdx, CSubQuestInfo* pSubQuest )	// ���� ����Ʈ ������ �߰��ϴ� �Լ�.
{
	if( dwSubQuestIdx >= MAX_SUBQUEST ) return;										// ���� ����Ʈ �ε�����, �ִ� ���� ����Ʈ �ε����� �Ѿ��, ���� ó���� �Ѵ�.

	m_SubQuestTable.Add( pSubQuest, dwSubQuestIdx );								// ���� ����Ʈ ���� ���̺� ���� ����Ʈ�� �߰��Ѵ�.
	
	++m_dwSubQuestCount;															// ���� ����Ʈ ī���͸� �����Ѵ�.
}
