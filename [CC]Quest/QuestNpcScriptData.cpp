// QuestNpcScriptData.cpp: implementation of the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestNpcScriptData.h"												// ����Ʈ npc ��ũ��Ʈ ������ ����� �ҷ��´�.

#include "QuestScriptLoader.h"												// ���� ��ũ��Ʈ �δ� ����� �ҷ��´�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScriptData::CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )		// ������ �Լ�.
{
	GetScriptParam( m_dwNpcIdx, pTokens );						// ��ũ��Ʈ�� ���� npc �ε����� �޴´�.
	GetScriptParam( m_dwScriptPage, pTokens );					// ��ũ��Ʈ�� ���� ��ũ��Ʈ �������� �޴´�.
	GetScriptParam( m_dwNpcMarkType, pTokens );					// ��ũ��Ʈ�� ���� npc ��ũ Ÿ���� �޴´�.

	GetScriptParam( m_dwDialogType, pTokens ) ;					// ���̾�α� ���� ��� ���θ� �����Ѵ�.
}

CQuestNpcScriptData::~CQuestNpcScriptData()						// �Ҹ��� �Լ�.
{
}

