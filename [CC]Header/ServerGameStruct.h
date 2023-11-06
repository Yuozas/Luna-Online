#pragma once


// taiyo
// ȸ�� ����, ȸ�� ����� �ð� ��� --------------------------------------------------
struct RECOVER_TIME
{
	RECOVER_TIME():lastCheckTime(0),bStart(TRUE){}
	BOOL bStart;
	DWORD lastCheckTime;
};
// �๰�� ���� ȸ��
struct YYRECOVER_TIME
{
	YYRECOVER_TIME():bStart(FALSE){}
	BOOL bStart;
	BYTE count;
	DWORD recoverUnitAmout;
	DWORD lastCheckTime;
	DWORD recoverDelayTime;
};



#pragma pack(push, 1)

struct SERVERINFO
{
	SERVERINFO()	{	memset(this,0,sizeof(SERVERINFO));	}
	SERVERINFO(const SERVERINFO* pInfo)	{	memcpy(this,pInfo,sizeof(SERVERINFO));	}
	SERVERINFO(WORD kind,WORD num,char* ipsvr,WORD portsvr,char* ipusr,WORD portusr)
		: wServerKind(kind),wServerNum(num),wPortForServer(portsvr),wPortForUser(portusr)
	{
		strcpy(szIPForServer,ipsvr);
		strcpy(szIPForUser,ipusr);
		dwConnectionIndex = 0;
		wAgentUserCnt = 0;
	}
	WORD	wServerKind;						// ���� ���� : agent, map, distribute
	char	szIPForServer[MAX_IPADDRESS_SIZE];
	char	szIPForUser[MAX_IPADDRESS_SIZE];	
	WORD	wPortForServer;						// ���� ��Ʈ
	WORD	wPortForUser;						// ���� ��Ʈ
	WORD	wServerNum;							// 0 ~ (���ȣ Ȥ�� ������ȣ)

	DWORD	dwConnectionIndex;					// Ŀ�ؼ� �ε��� : �����ε���

	//--------agent field----------------
	WORD	wAgentUserCnt;
};
// powerup ���� ����ü /////////////////////////////////////////////
struct MSG_PWRUP_BOOTUP : public MSGROOT
{
	DWORD dwProcessID;
	SERVERINFO BootInfo;
};

struct MSG_PWRUP_BOOTLIST : public MSGROOT
{
	void	AddList(SERVERINFO * pInfo)
	{
		BootList[Num++] = *pInfo;
	}
	int		GetSize() { return sizeof(MSG_PWRUP_BOOTLIST) - (MAX_SERVERINFO_LIST - Num)*sizeof(SERVERINFO);	}
	WORD		MapServerPort;
	WORD		Num;
	SERVERINFO BootList[MAX_SERVERINFO_LIST];
};
struct MSG_PWRUP_CONNECT_ACK : public MSGROOT
{
	SERVERINFO BootList;
	//WORD Key;
};
struct MSG_PWRUP_CONNECT_SYN : public MSGROOT
{
	SERVERINFO BootList;
};

struct PARTY_RECEIVE_MEMBER
{
	void* pPlayer[MAX_PARTY_LISTNUM];
	DWORD Damage[MAX_PARTY_LISTNUM];
	int count;
};

//---------monitoring server related
struct MSGMC :public MSGROOT
{
	DWORD dwConnectionIdxMC;
};
struct MSGUSERCOUNT :public MSGMC
{
	WORD ServerPort;
	DWORD dwUserCount;
};
struct MSGEXECUTESERVER : public MSGMC
{
	WORD ServerType;
	WORD ServerNum;
	WORD ServerPort;
	char ServerIP[MAX_IPADDRESS_SIZE];
};
struct MSGSHUTDOWNSERVER : public MSGEXECUTESERVER
{
};

struct MSGNOTICE : public MSGMC
{
	char Msg[256];
};

struct MSERVERINFO
{
	MSERVERINFO()
	{
		*szIPForServer = 0;
		wPortForServer = 0;
		dwConnectionIndex = 0;
	}
	char	szIPForServer[MAX_IPADDRESS_SIZE];
	WORD	wPortForServer;						// ���� ��Ʈ
	char	szIPForUser[MAX_IPADDRESS_SIZE];
	WORD	wPortForUser;
	WORD	wServerType;
	WORD	wServerNum;

	DWORD	dwConnectionIndex;					// Ŀ�ؼ� �ε��� : �����ε���

	DWORD	ProcessID;							//mapserver
	DWORD	dwExpiredTime;							//mapserver
	WORD	wServerState;							//mapserver
};



//struct LOAD_GUILD
//{
//	GUILDINFO GuildInfo[MAX_QUERY_RESULT];
//	MONEYTYPE GuildMoney[MAX_QUERY_RESULT];
//};

struct GUILDMEMBERINFO_ID
{
	GUILDMEMBERINFO MemberInfo;
	DWORD GuildIdx;
};

//// 06. 03. ���İ��� - �̿���
//struct LOAD_GUILDNOTICE
//{
//	DWORD	DBIdx[MAX_QUERY_RESULT];
//	DWORD	GuildIdx[MAX_QUERY_RESULT];
//	char	GuildNotice[MAX_QUERY_RESULT][MAX_GUILD_NOTICE+1];
//};

struct LOAD_GUILDMEMBER
{
	GUILDMEMBERINFO_ID Member[MAX_QUERY_RESULT];
};


//struct LOAD_FAMILY
//{
//	FAMILYINFO FamilyInfo[MAX_QUERY_RESULT];
//	MONEYTYPE FamilyMoney[MAX_QUERY_RESULT];
//};
//
//struct FAMILYMEMBERINFO_ID
//{
//	FAMILYMEMBERINFO MemberInfo;
//	DWORD FamilyIdx;
//};
//
//// 06. 03. ���İ��� - �̿���
//struct LOAD_FAMILYNOTICE
//{
//	DWORD	DBIdx[MAX_QUERY_RESULT];
//	DWORD	FamilyIdx[MAX_QUERY_RESULT];
//	char	FamilyNotice[MAX_QUERY_RESULT][MAX_FAMILY_NOTICE+1];
//};
//
//struct LOAD_FAMILYMEMBER
//{
//	FAMILYMEMBERINFO_ID Member[MAX_QUERY_RESULT];
//};

struct MSGMSINFO : public MSGROOT
{
	MSERVERINFO MSInfo;
};

enum
{
	SERVERSTATE_NONE =0,
	SERVERSTATE_OFF=1,
	SERVERSTATE_ON=2,
	SERVERSTATE_ABNORMAL=3,
	SERVERSTATE_RECOVER=4
};

enum
{
	AUTOPATCHSTATE_START = 0,
	AUTOPATCHSTATE_PROC,
	AUTOPATCHSTATE_END,
};

struct SERVERSTATE
{
	WORD ServerType;
	WORD ServerNum;
	WORD ServerPort;
	char ServerIP[MAX_IPADDRESS_SIZE];
	WORD ServerState;
	int	 UserLevel;
	DWORD UserCount;
};

#define MAX_SERVERSTATE_NUM	128
struct MSGSERVERSTATE : public MSGMC
{
	SERVERSTATE serverState;
};

struct MSGMONITORTOOLALLSTATE : public MSGROOT
{

	void AddServerState(SERVERSTATE * pState)
	{
		serverState[num++] = *pState;
	}
	int GetSize()
	{
		return sizeof(MSGMONITORTOOLALLSTATE) - (MAX_SERVERSTATE_NUM - num)*sizeof(SERVERSTATE);
	}
	int num;
	SERVERSTATE serverState[MAX_SERVERSTATE_NUM];
};

struct MSSTATE
{
	char	sServerIP[MAX_IPADDRESS_SIZE];
	WORD	wServerPort;
	WORD	wAutoPatchState;
	char	sAutoPatchVersion[13];
};

struct MSGMSSTATE : public MSGROOT
{
	MSSTATE	MSState;
	MSGMSSTATE()
	{
		memset( &MSState, 0, sizeof(MSSTATE) );
	}
};

struct MSGMSSTATEALL : public MSGROOT
{
	int		nNum;
	MSSTATE	MSState[MAX_SERVERSTATE_NUM];	

	MSGMSSTATEALL()
	{
		memset( MSState, 0, sizeof(MSSTATE)*MAX_SERVERSTATE_NUM );
		nNum = 0;
	}
	void	AddMSServerState( MSSTATE* pState )
	{
		MSState[nNum++] = *pState;
	}
	int		GetSize()
	{
		return sizeof(MSGMSSTATEALL) - (MAX_SERVERSTATE_NUM-nNum)*sizeof(MSSTATE);
	}
};

// 06. 05 HIDE NPC - �̿���
struct HIDE_NPC_INFO
{
	WORD UniqueIdx;
	BOOL ChannelInfo[MAX_CHANNEL_NUM];
};
#pragma pack(pop)
