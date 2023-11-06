#ifndef __LUNACLIENT__
#define __LUNACLIENT__
class CVersionManager  
{
	char	CLIENT_VERSION[256];
	//BYTE	m_LimitUserLevel;
	//BYTE	m_LimitAge;
public:
	CVersionManager();
	virtual ~CVersionManager();

	//void	LoadDistributeDesc();

	//BOOL	IsValidUserLevel(BYTE UserLevel);	
	BOOL	IsValidVersion(char * szVersion);
	//BYTE	GetLimitAge()				{ return m_LimitAge; }

	void	LoadVersion();
	void	SaveVersion();
	void	SetVersion(char *szV);
	char *	GetVersion();
	
	//void	SetUserLevel( BYTE level )	{ m_LimitUserLevel = level; }
	//BYTE	GetUserLevel()				{ return m_LimitUserLevel; }
};

extern CVersionManager g_CLIENTVERSION;
#endif 
