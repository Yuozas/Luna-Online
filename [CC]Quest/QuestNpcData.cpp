// QuestNpcData.cpp: implementation of the CQuestNpcData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"														// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestNpcData.h"												// ����Ʈ NPC ������ ��������� �ҷ��´�.

#include "QuestScriptLoader.h"											// ����Ʈ ��ũ��Ʈ �δ� ��������� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcData::CQuestNpcData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )	// ������ �Լ�.
{
	GetScriptParam( m_dwNpcIdx, pTokens );	// ��ũ��Ʈ�� �Ķ���͸� �޴´�.
}

CQuestNpcData::~CQuestNpcData()				// �Ҹ��� �Լ�.
{
}
