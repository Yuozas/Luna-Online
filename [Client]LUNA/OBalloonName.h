#pragma once


class cMultiLineText;

class COBalloonName  
{
public:

	COBalloonName();
	virtual ~COBalloonName();

	void InitName( const char* );
	void SetName( const char* );
	void SetNickName( const char* );
	void SetFamilyNickName( const char* );

	BOOL IsActive()						{ return m_bActive;	}
	BOOL IsShow()						{ return m_bShow;	}
	void SetActive(BOOL val)			{ m_bActive = val;	}
	void Show( BOOL val )				{ m_bShow	= val; 	}
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)			{ m_lTall = Tall;	}
	void SetPosX(LONG	pos);
	void SetNickNameTall(LONG	nickTall)			{ m_lNickTall = nickTall;	}
	void SetFGColor(DWORD col)			{ m_fgColor = col;	}
	
	LONG GetPosX()			const	{ return m_lPosX; }
	LONG GetTall()			const	{ return m_lTall; }
	LONG GetFamilyNickPosX() const	{ return mFamilyNickPosX; }

	struct Data
	{
		LONG mX;
		LONG mLength;
	}
	mNickData;

	const Data& GetNickData() const;

	DWORD GetNameLength();

	// SiegeWar
	void SetSiegeName( DWORD NameType, DWORD dwLength );

protected:

	BOOL m_bActive;			//on/off option
	BOOL m_bShow;			//show/hide
	LONG m_lTall;
	LONG m_lPosX;
	LONG m_lNickTall;
	LONG mFamilyNickPosX;
	LONG m_lMasterTall;
	LONG m_lMasterPosX;
	char mObjectName[MAX_MONSTER_NAME_LENGTH +1];
	BOOL m_wFontIdx;
	DWORD m_fgColor;
	char mGuildNickName[MAX_GUILD_NICKNAME+1];
	char mFamilyNickName[MAX_GUILD_NICKNAME+1];
	
	BOOL m_bSiegeMap;
	cMultiLineText* m_pSiegeName;
};
