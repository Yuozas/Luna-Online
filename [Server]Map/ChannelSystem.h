// ChannelSystem.h: interface for the CChannelSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)
#define AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Channel.h"

#define CHANNELSYSTEM CChannelSystem::GetInstance()

class CChannelSystem  
{
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	static const int	MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM	=	2000;
	// E ����Ʈ �� �߰� added by hseos 2008.01.10

	CChannel*	m_Channel[MAX_CHANNEL_NUM];
	char		m_ChannelName[MAX_CHANNEL_NAME+1];
	DWORD		m_dwCount;
	
public:
	CChannelSystem();
	virtual ~CChannelSystem();
	GETINSTANCE(CChannelSystem);

	void	LoadChannelInfo();
	void	CreateChannel(BYTE Kind, BYTE num);
	void	IncreasePlayerNum(DWORD ChannelID);
	void	DecreasePlayerNum(DWORD ChannelID);
	void	SendChannelInfo(MSG_DWORD* pInfo, DWORD dwConnectionIndex);
	DWORD	GetChannelID(int num);
	DWORD	GetChannelCount()			{ return m_dwCount;	}
	
	// jsd
	void	SendChannelInfoToMS( DWORD dwConnectionIndex );

protected:
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	// CChannel	m_QMChannel[1000];
	CChannel	m_QMChannel[MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM];
	// E ����Ʈ �� �߰� added by hseos 2008.01.10
	BOOL		m_bQuestMap;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25	2007.11.28
	DWORD		m_nChallengeZoneLastChannelID;
	DWORD		m_nChallengeZoneAllocatedCH[1000][2];
	DWORD		m_nChallengeZoneAgentChannelID[1000];
	DWORD		m_nChallengeZoneAllocatedCHNum;
	DWORD		m_nChallengeZonePlayerAgentChannelID;
	// E ����Ʈ �� �߰� added by hseos 2007.11.25	2007.11.28

public:
	void	InitQuestMapChannel();
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	// DWORD	CreateQuestMapChannel();
	DWORD	CreateQuestMapChannel(DWORD nFirstEnterPlayerID);
	// E ����Ʈ �� �߰� added by hseos 2007.11.27
	void	DestroyQuestMapChannel( DWORD dwChannel );
	BOOL	IsQuestMap()		{ return m_bQuestMap; }
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
	DWORD	GetChallengeZoneLastChannalID()						{ return m_nChallengeZoneLastChannelID; }
	BOOL	GetChallengeZonePartnerCH(DWORD nChallengeZoneAgentChannelID);
	void	SetChallengeZoneAgentChannelID(DWORD nChannelID)	{ m_nChallengeZonePlayerAgentChannelID = nChannelID; }
	// E ����Ʈ �� �߰� added by hseos 2007.11.25
	
	void	Init( WORD wMapNum, WORD wChannelNum );
	DWORD	GetRandomChannelID();
	DWORD	GetLastChannelID();
};

#endif // !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)
