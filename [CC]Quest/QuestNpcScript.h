// QuestNpcScript.h: interface for the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)
#define AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;																			// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.

class CQuestNpcScript																			// NPC ��ũ��Ʈ Ŭ����.
{
protected:
	DWORD		m_dwQuestIdx;																	// ����Ʈ �ε����� ��� ����.
	DWORD		m_dwSubQuestIdx;																// ���� ����Ʈ �ε����� ��� ����.
	cPtrList	m_NpcScriptList;																// NPC ��ũ��Ʈ ����Ʈ.

public:
	CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );			// ������ �Լ�.
	virtual ~CQuestNpcScript();																	// �Ҹ��� �Լ�.

	void	Release();																			// ���� �Լ�.
	// script
	cPtrList* GetNpcScriptList() { return &m_NpcScriptList ; }
	BOOL	ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );		// NPC ��ũ��Ʈ�� �д� �Լ�.
	// getfn
	DWORD	IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );					// npc ��ũ��Ʈ �������� �����ϴ� �Լ�.
	DWORD	IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );				// npc ��ũ�� �����ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)
