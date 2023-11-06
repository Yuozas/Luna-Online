// OptionManager.h: interface for the OptionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONMANAGER_H__9E01A939_77F5_419A_BCA4_79C596D7ED48__INCLUDED_)
#define AFX_OPTIONMANAGER_H__9E01A939_77F5_419A_BCA4_79C596D7ED48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct sGAMEOPTION
{
	BOOL bNoDeal;
	BOOL bNoParty;
//	BOOL bNoFriend;		//AGENT
	BOOL bNoShowdown;
//	BOOL bNoWhisper;	//AGENT

	sGAMEOPTION()
	{
		bNoDeal		= 0;
		bNoParty	= 0;
//		bNoFriend	= 0;
		bNoShowdown		= 0;
//		bNoWhisper	= 0;
	}
};



#define OPTIONMGR COptionManager::GetInstance()

class COptionManager  
{

protected:

	void MakeGameOption( WORD wOption, sGAMEOPTION* pOutGameOption );
	void SendShowParts(DWORD dwHidePartsFlag, CPlayer* pPlayer);

public:
	COptionManager();
	virtual ~COptionManager();
	MAKESINGLETON( COptionManager );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
};


#endif // !defined(AFX_OPTIONMANAGER_H__9E01A939_77F5_419A_BCA4_79C596D7ED48__INCLUDED_)
