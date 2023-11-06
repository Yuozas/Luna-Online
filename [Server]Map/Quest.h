// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__8126C9A7_14D6_4D64_87FC_8BAE17A44978__INCLUDED_)
#define AFX_QUEST_H__8126C9A7_14D6_4D64_87FC_8BAE17A44978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"

class CPlayer;
class CQuestInfo;
class CSubQuestInfo;
class CQuestGroup;
class CQuestEvent;

class CQuest 
{
protected:
	CPlayer*			m_pPlayer;
	CQuestInfo*			m_pQuestInfo;							// script ����
	CSubQuestInfo*		m_CurSubQuestInfoArray[MAX_SUBQUEST];	// ���� ������ subquest
	
	DWORD				m_dwSubQuestCount;
	SUBQUEST			m_SubQuestTable[MAX_SUBQUEST];			// subquest ����
	DWORD				m_dwSubQuestFlag;
	DWORD				m_dwData;								// 0�� �ƴϸ� ����Ʈ ��
	DWORD				m_dwTime;
	
	BOOL				m_bCheckTime;
	BYTE				m_CheckType;
	stTIME				m_CheckTime;

public:
	CQuest( CPlayer* pPlayer, CQuestInfo* pQuestInfo );
	virtual ~CQuest();

	// ���� ���� �� DB������ ���� 
	void	SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime );
	void	SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime );

	// trigger
	void	SetSubQuestFlag( DWORD dwSubQuestIdx );
	BOOL	EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, BOOL bEnd = TRUE );
	BOOL	StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	BOOL	EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	BOOL	ChangeSubQuestValue( DWORD dwSubQuestIdx, DWORD dwKind );
	void	SetStartCheckTime( BOOL bStart );
	
	// event
	void	OnQuestEvent( CPlayer* pPlayer, CQuestGroup* pQuestGroup, CQuestEvent* pQuestEvent, DWORD dwQuestIdx );

	// getfn
	inline DWORD	IsQuestComplete()						{ return m_dwData; }
	inline DWORD	GetQuestTime()							{ return m_dwTime; }
	inline DWORD	GetSubQuestData( DWORD dwSubQuestIdx )	{ return m_SubQuestTable[dwSubQuestIdx].dwData; }
	inline DWORD	GetSubQuestTime( DWORD dwSubQuestIdx )	{ return m_SubQuestTable[dwSubQuestIdx].dwTime; }
	inline DWORD	GetSubQuestFlag()						{ return m_dwSubQuestFlag; }
	BOOL			IsSubQuestComplete( DWORD dwSubQuestIdx );
	DWORD			GetQuestIdx();	
	DWORD			GetSubQuestCount()						{ return m_dwSubQuestCount;	}
	
	// etc
	BOOL			CheckTime( DWORD dwEventKind );
	void			RegistCheckTime( BOOL bStart, BYTE bCheckType, stTIME sTime );

	CSubQuestInfo*	GetSubQuestInfo(int nIdx) { return m_CurSubQuestInfoArray[nIdx] ; }

	BOOL			IsRepeat();
};

#endif // !defined(AFX_QUEST_H__8126C9A7_14D6_4D64_87FC_8BAE17A44978__INCLUDED_)
