#include "stdafx.h"
#include "DistributeDBMsgParser.h"
#include "DataBase.h"
#include "CommonDBMsgParser.h"
#include "UserTable.h"
#include "ServerTable.h"
#include "Network.h"
#include "UserManager.h"

extern int g_nServerSetNum;
extern DWORD g_dwMaxUser;

DBMsgFunc g_DBMsgFunc[] =
{
	NULL,
	RLoginCheckQuery,
	RIpCheckQuery,			/// eIpCheckQuery 
	RLoginCheckQueryTest,
	RCheckDynamicPW,
	RLoginCheckQueryForJP,
};

void LoginCheckInit()
{
	g_DB.FreeQuery(0, 0, "EXEC %s", "MP_CHARACTER_LoginInit");
/* ¾??÷ ??°¡???ö ¸»°?... ??·?½??® ¼ö?¤?? ??°¡ (LBS)
	char txt[128];
	sprintf(txt, "EXEC %s %d", "up_Server_ResetLoginMember", g_nServerSetNum);
	g_DB.LoginQuery(eQueryType_FreeQuery, 0, 0, txt);
*/
}


/*
void LoginCheckDelete(DWORD UserID)//, DWORD dwConnectionIndex)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d", "Up_Member_CheckOut", UserID);
	g_DB.LoginQuery(eQueryType_FreeQuery, eNull, 0, txt);
}


BOOL LoginCheckQuery(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex)
{
	char guery[128];
	// id,pw,AuthKey,ServerGroupNum
	sprintf(guery, "EXEC %s \'%s\', \'%s\', %d, %d", "Up_Member_CheckIn", id, pw, AuthKey, 1);
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eLoginCheckQuery, dwConnectionIndex, guery) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		return FALSE;
	}
	
	g_Console.Log(eLogDisplay,4,"IDPW_QUERY   ID: %s,  ConnectionIndex: %d",id,dwConnectionIndex);
	return TRUE;
}
*/

BOOL IPCheckQuery(char* ip, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 090325 ONS ���͸�����üũ
	char szBufIp[MAX_IPADDRESS_SIZE];
	SafeStrCpy(szBufIp, ip, MAX_IPADDRESS_SIZE);
	if(IsCharInString(szBufIp, "'"))	return FALSE;

	char query[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(query, "EXEC %s \'%s\', %d", "UP_IP_CHECKIN", szBufIp, AuthKey);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eIpCheckQuery, dwConnectionIndex, query) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy(IP_CHECK)  IP:%s  ConnectionIndex:%d", szBufIp, dwConnectionIndex);
		return FALSE;
	}

//	g_Console.Log(eLogDisplay,4,"IP_QUERY   IP: %s,  ConnectionIndex: %d", ip, dwConnectionIndex);
	return TRUE;
}

/// 060911 PKH ȫ��IPüũ
/* --------------------------------------------------------------------------
* �Լ��̸� : IPCheckQuery
* ��    �� : ȫ��IPüũ
* ���ǻ��� :
*---------------------------------------------------------------------------*/
BOOL IPCheckQueryHK(char* ip, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 090325 ONS ���͸�����üũ
	char szBufIp[MAX_IPADDRESS_SIZE];
	SafeStrCpy(szBufIp, ip, MAX_IPADDRESS_SIZE);
	if(IsCharInString(szBufIp, "'"))	return FALSE;
	
	char query[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(query, "EXEC %s \'%s\', %d", "UP_IP_CHECKINHK", szBufIp, AuthKey);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eIpCheckQuery, dwConnectionIndex, query) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy(IP_CHECK)  IP:%s  ConnectionIndex:%d", szBufIp, dwConnectionIndex);
		return FALSE;
	}

	//	g_Console.Log(eLogDisplay,4,"IP_QUERY   IP: %s,  ConnectionIndex: %d", ip, dwConnectionIndex);
	return TRUE;
}

void LoginError(DWORD dwConnectionIdx, DWORD AuthKey,DWORD ErrorCode,DWORD dwParam)
{	
	MSG_DWORD2 msguser;
	msguser.Category = MP_USERCONN;
	msguser.Protocol = MP_USERCONN_LOGIN_NACK;
	msguser.dwData1 = ErrorCode;
	msguser.dwData2 = dwParam;
	
	g_pUserTable->SendToUser(dwConnectionIdx, AuthKey, &msguser, sizeof(msguser) );

	
//	g_Console.Log(eLogDisplay,4,"  LOGINERROR   Errorcode: %d,  ConnectionIndex: %d",ErrorCode,dwConnectionIdx);
}

void RIpCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0)
	{
		// DB Error(DB is busy)
		return;
	}

	WORD Ecode =(WORD)atoi((char*)pData->Data[0]);
	DWORD AuthKey = atoi((char*)pData->Data[1]);

	DWORD dwConnectionIdx	= pMessage->dwID;
	USERINFO* pUserInfo		= g_pUserTable->FindUser(dwConnectionIdx);

	if( pUserInfo == NULL )
		return;

	if( pUserInfo->dwUniqueConnectIdx != AuthKey )
		return;

	if( Ecode == 0 )
	{
		LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_INVALID_IP );
        DisconnectUser( dwConnectionIdx );
		return;
	}


	SERVERINFO* FastInfo = g_pServerTable->GetFastServer( AGENT_SERVER );
	
	if(FastInfo == NULL)
	{
		LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOAGENTSERVER );
        DisconnectUser( dwConnectionIdx );
		return;
	}

	MSG_DWORD3BYTE2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
	msg.dwObjectID	= pUserInfo->dwUserID;
	msg.dwData1		= AuthKey;
	msg.dwData2		= dwConnectionIdx;
	msg.dwData3		= 0;
	msg.bData1		= pUserInfo->UserLevel;
	msg.bData2		= 0;
				
	g_Network.Send2Server(FastInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

DWORD testTime[1024];
BOOL LoginCheckQueryTest(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(id, "'") || IsCharInString(pw, "'"))	return FALSE;
	
	char guery[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(guery, "EXEC %s \'%s\', \'%s\', %d, %d", "UP_MEMBER_CHECKIN", id, pw, AuthKey, 1);

	if(g_DB.LoginQuery(101, eLoginCheckQueryTest, dwConnectionIndex, guery) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		return FALSE;
	}
	
	g_Console.Log(eLogDisplay,4,"IDPW_QUERY   ID: %s,  ConnectionIndex: %d",id,dwConnectionIndex);
	return TRUE;
}

void	RLoginCheckQueryTest(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD AuthKey =atoi((char*)pData->Data[2]);
	
	testTime[AuthKey-1] = GetTickCount() - pMessage->dwID;

	g_Console.Log(eLogDisplay,4,"IDPW TestReceived    ID: %d  Time: %d",AuthKey,testTime[AuthKey-1]);

	if(AuthKey == 1024)
	{
		FILE* fp = fopen("test.txt","w");
		for(int n=0;n<1024;++n)
			fprintf(fp,"%d  ElapsedTime: %d\n",n+1,testTime[n]);
		fclose(fp);
	}
}

//* MemberDB ±³?¼ ??¾÷ 2004.03.22
void LoginCheckDelete(DWORD UserID)
{
	char txt[ MAX_PATH ] = { 0 };
	// 090710 LUJ, �α׾ƿ� �� ������ ��ȣ�� ������
	sprintf(txt, "EXEC dbo.UP_GAMELOGOUT %d, %d", UserID, g_nServerSetNum );
	g_DB.LoginQuery(eQueryType_FreeQuery, eNull, 0, txt);
}
//*/

//=========================================================================
//	NAME : LoginCheckQuery
//	DESC : The function to run procedure to check login infor.
//=========================================================================
BOOL LoginCheckQuery(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, 
	DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pSecurityPW)
{
	if( !id ) return FALSE ;														// ���ڷ� �Ѿ�� ������ ���Ἲ üũ.

	if( !pw ) return FALSE ;

	if( !ip ) return FALSE ;

	if( !pSecurityPW ) return FALSE ;

	char tempSecurityPW[5] = {0, } ;

	if( strlen(pSecurityPW) <= 0 )
	{
		// 080821 LYW --- DistributeDBMsgParser : Stack around ���� ����.
		//strcpy(tempSecurityPW, "''") ;
		SafeStrCpy(tempSecurityPW, "''", 5) ;
	}
	else
	{
		// 080821 LYW --- DistributeDBMsgParser : Stack around ���� ����.
		//strcpy(tempSecurityPW, pSecurityPW) ;
		SafeStrCpy(tempSecurityPW, pSecurityPW, 5) ;
	}

	// 090325 ONS ���͸�����üũ
	if(IsCharInString(id, "'") || IsCharInString(pw, "'") || IsCharInString(ip, "'") )	return FALSE;
	if(strlen(tempSecurityPW) > strlen("''") && IsCharInString(tempSecurityPW, "'")) return FALSE;

	char guery[128] = {0, } ;														// �ӽ� ���۸� �����ϰ� �ʱ�ȭ �Ѵ�.

	sprintf(		guery,															// ���ν��� ���� ��ɾ �ϼ��Ѵ�.
					"EXEC up_gameloginadult_free \'%s\', \'%s\', \'%s\', %d, %d, %d, %d, %d, \'%s\'", 
                    id,																// ���̵�.
					pw,																// ��й�ȣ.
					ip,																// ip.
					ServerNo,														// ���� �� ��ȣ.									
					AuthKey,														// ���� Ű.
					dwMaxUser,														// �ִ� ���� ��.
					gUserMGR.GetLimitAge(),											// ���� ����.
					ServerNum,														// ���� ��ȣ.
					tempSecurityPW													// ���� ��� ��ȣ.
			) ;	

	BOOL bResult = g_DB.LoginQuery(
		eQueryType_FreeQuery,
		eLoginCheckQuery,
		dwConnectionIndex, 
		guery);

	if(!bResult)																	// ���� �ߴٸ�,
	{
		g_Console.Log(	eLogDisplay,												// ���� �ܼ� â�� ���� �޽����� ����Ѵ�.
						4,
						"DB is Busy  ID:%d  ConnectionIndex:%d",
						id,
						dwConnectionIndex) ;

		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY ) ;	// �������� �α��� ������ �����Ѵ�.

		return FALSE ;
	}

	return TRUE;
}

void RLoginCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int		stateNo = atoi((char*)pData->Data[0]);
	WORD	ServerNo = (WORD)atoi((char*)pData->Data[1]);
	DWORD	UserIdx = atoi((char*)pData->Data[2]);
	WORD	wType = (WORD)atoi((char*)pData->Data[3]);
	int		nTime = atoi((char*)pData->Data[4]);
//	WORD	bTen = atoi((char*)pData->Data[5]);
	DWORD	AuthKey = atoi((char*)pData->Data[6]);
	BYTE	UserLevel = (BYTE)atoi((char*)pData->Data[7]);
	WORD	AgentNo = (WORD)atoi((char*)pData->Data[8]);
	DWORD	dwConnectionIdx = pMessage->dwID;


	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	if(pUserInfo == NULL)					//������ ������.
	{
		//���� �������� ���� �α��� delete�� ���ش�.
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)	//�������� ���� �����, �ٸ� ������ ���Դ�.
	{												//***pUserInfo�� ������ �ƴϴ�. ����� �ȵȴ�.***
		//���� �������� ���� �α��� delete�� ���ش�.		
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	// 100226 pdy ��ũ��Ʈäũ�� ������ Ű���� �ѹ��� �е��� ����
	const ScriptCheckValue* check = g_pServerSystem->GetScriptCheckValue( );
 
 	if( check->mValue != pUserInfo->check.mValue && UserLevel > eUSERLEVEL_GM )
 	{
 		//LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_FILE_CRC );//beyond
		LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_INVALID_VERSION );
 		return;
 	}//disable script check

	pUserInfo->dwUserID		= UserIdx;
	pUserInfo->UserLevel	= UserLevel;
	pUserInfo->State		= stateNo;

	// 100706 ShinJS �α��� ���� ���� �α�
	if( stateNo != 1 )
	{
		FILE* fpLog = NULL;
		fpLog = fopen( "./Log/LoginError.txt", "a+" );
		if( fpLog )
		{
			SYSTEMTIME sysTime;
			GetLocalTime( &sysTime );

			fprintf( fpLog, "[%04d-%02d-%02d %02d:%02d:%02d]\t [state : %d , UserIdx : %d]\n", 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
				stateNo,
				UserIdx );

			fclose( fpLog );
		}
	}

	switch(stateNo)
	{
	case -11:	// �� ������ ���
	case -10:	// DB ���� �߻���
	case -9:	// DB ���� �߻���
	case -8:	// �α��� ���� üũ 4ȸ �̻� ��ȸ�� ��� (�ܺο��� �α��� ���� ȣ���� ���)
	case -6:	// ȸ���� ���ȹ�ȣ�� �������� ���� ���
	case -5:	// �α��� ���� ȸ�� ������ȣ�� �ٸ� ���
	case -4:	// �α��� ȸ���� ������ȣ�� �������� ���� ���
	case -3:	// �α��� ���� IP�� �ٸ� ���
	case -2:	// �α��� ���� KEY�� �ٸ� ���
	case -1:	// �α��� ���� KEY�� �������� ���� ���
		{
			LoginError( dwConnectionIdx, pUserInfo->dwUniqueConnectIdx, LOGIN_ERROR_LOGINSESSION_INVALID );
		}
		break;
	case 1:	//�α��� ����
	//[06-02-2012] Death , don't care Overlap Login//beyond
	case 7:
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;	//AAA
			//�������� ����� DeleteLoginCheck�� ���ְ� �ȴ�. 
			//�����κп��� ȣ���ϴ� ���� OndisconnectUser���� ȣ���ϵ��� �����Ѵ�.

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			char strIP[16] = {0,};
			WORD wPort = 0;
			g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

			if( UserLevel == eUSERLEVEL_GM )
			{
				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}

			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser( dwConnectionIdx );
				return;
			}

			LPCTSTR userId = LPCTSTR(pData->Data[9]);

			MSG_NAME loginidMessage;
			ZeroMemory( &loginidMessage, sizeof(loginidMessage) );
			loginidMessage.Category = MP_USERCONN;
			loginidMessage.Protocol = MP_USERCONN_GETLOGINID_ACK;
			loginidMessage.dwObjectID = UserIdx;
			SafeStrCpy( loginidMessage.Name, userId, sizeof(loginidMessage.Name) );
			g_Network.Send2User(dwConnectionIdx, (char*)&loginidMessage, sizeof(loginidMessage));

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= (BYTE)UserLevel;
			msg.bData2		= (BYTE)wType;
			
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
				
			//g_pLoginUser.Remove( pUserInfo->dwUserID );
		}
		break;
	case 2: // ������� ����
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: // ������� ����
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: // �ߺ��α���
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData		= UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;

	case 5: //��
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;

	//[06-02-2012] Death Comment it and Let it same case with Login Success//beyond
	/*
	case 7: // DB Connection Error
		{
			ASSERTMSG(0, "login check error ret 0");
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;
	*/

	case 8: // ? �ƽ�����?
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;

	// 080111 LYW --- DistributeDBMsgParser : ���� ��� ���� ó�� �߰�.
	case 9 :
	case -7:// �α��� ���� ���ȹ�ȣ�� �ٸ� ���
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALID_SEC_PASS) ;
		}
		break ;

	// 080111 LYW --- DistributeDBMsgParser : ��й�ȣ ������ ��ȿ���� ����, ��й�ȣ�� �� ��� �ϼ���.
	case 10 :
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_CHANGE_SEC_PASS) ;
		}
		break ;

//---KES PUNISH
	case 11:
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL,nTime) ;
		}
		break;
//-------------

	case 14:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOT_CLOSEBETAWINNER );
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;
	case 15: //�̼�����
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MINOR_INADULTSERVER );
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;
	case 50: // Ż������
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;
	case 99:		
		{
			LoginGetDynamicPWQuery( (char*)pData->Data[8], dwConnectionIdx );		
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
			DisconnectUser(dwConnectionIdx);	//AAA ������ ���� ���� ����.
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	}
}

BOOL LoginGetDynamicPWQuery( char* id, DWORD dwConnectionIndex )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(id, "'"))	return FALSE;

	char guery[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf( guery, "EXEC %s \'%s\', '', 1", "LP_MAT_OUT", id );

	if(g_DB.LoginMiddleQuery( RLoginGetDynamicPWQuery, dwConnectionIndex, guery) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/// 060920 PKH ���̳���IPüũ
BOOL LoginGetDynamicPWQueryHK( char* strIP, char* id, DWORD dwConnectionIndex )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(strIP, "'") || IsCharInString(id, "'"))	return FALSE;

	char guery[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf( guery, "EXEC %s \'%s\', \'%s\', '', 1", "LP_MAT_OUT_HK", strIP, id );

	if(g_DB.LoginMiddleQuery( RLoginGetDynamicPWQuery, dwConnectionIndex, guery) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

void RLoginGetDynamicPWQuery(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIdx = pMessage->dwID;
	int rt = atoi((char*)pData->Data[0]);

	if( rt == 1 )
	{
		//send mat msg
		MSG_USE_DYNAMIC_ACK msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_USE_DYNAMIC_ACK;
		SafeStrCpy( msg.mat, (char*)pData->Data[1], 32 );

		g_Network.Send2User( dwConnectionIdx, (char*)&msg, sizeof(msg) );			
	}
	else
	{
		//error msg
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_USE_DYNAMIC_NACK;
		g_Network.Send2User( dwConnectionIdx, (char*)&msg, sizeof(msg) );
	}
}

BOOL CheckDynamicPW( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(id, "'") || IsCharInString(pw, "'") || IsCharInString(ip, "'"))	return FALSE;

	char guery[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(guery, "EXEC UP_GAMELOGINMAT_FREE \'%s\', \'%s\', \'%s\', %d, \'%d\', %d, %d, %d",
		id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum );

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eCheckDynamicPW, dwConnectionIndex, guery) == FALSE)
		return FALSE;

	return TRUE;
}

void RCheckDynamicPW( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD stateNo = atoi((char*)pData->Data[0]);
	WORD ServerNo = (WORD)atoi((char*)pData->Data[1]);
	DWORD UserIdx = atoi((char*)pData->Data[2]);
	WORD	wType = (WORD)atoi((char*)pData->Data[3]);
	int		nTime = atoi((char*)pData->Data[4]);
//	WORD bTen = atoi((char*)pData->Data[5]);
	DWORD AuthKey = atoi((char*)pData->Data[6]);
	BYTE UserLevel = (BYTE)atoi((char*)pData->Data[7]);
	WORD AgentNo = (WORD)atoi((char*)pData->Data[8]);
	DWORD dwConnectionIdx = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	if(pUserInfo == NULL)					//������ ������.
	{	
		//���� �������� ���� �α��� delete�� ���ش�.
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)
	{
		//���� �������� ���� �α��� delete�� ���ش�.		
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	pUserInfo->dwUserID		= UserIdx;
	pUserInfo->UserLevel	= UserLevel;
	pUserInfo->State		= stateNo;

	switch(stateNo)
	{
	case 1:
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;	//AAA
			//�������� ����� DeleteLoginCheck�� ���ְ� �ȴ�. 
			//�����κп��� ȣ���ϴ� ���� OndisconnectUser���� ȣ���ϵ��� �����Ѵ�.

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);
				DisconnectUser(dwConnectionIdx);
				return;
			}
			
			if( UserLevel == eUSERLEVEL_GM )
			{
				char strIP[16] = {0,};
				WORD wPort;
				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}

			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= (BYTE)UserLevel;
			msg.bData2		= (BYTE)wType;
				
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
				
			//g_pLoginUser.Remove( pUserInfo->dwUserID );
		}
		break;
	case 2: // ID¾ø?½.
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: // ºñ¹?¹ø?£¿?·ù.
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: // °???¿¡ ?¢¼???.
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData = UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;

	case 5: // ºí·°µ? °è?¤. @state_num = 6??¸é °è?¤ºí·° (?¥ °ü¸®¸ðµå¿¡¼­ »ç¿ë)
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
		}
		break;

	case 7: // DB Connection Error
		{
			ASSERTMSG(0, "login check error ret 0");
		}
		break;

	case 8: // ??¿ø??°ú
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
		}
		break;
	case 14:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOT_CLOSEBETAWINNER );
		}
		break;
	case 15: //¼º??¼·¿¡ ¹?¼º³â??°¡ µé¾î¿?·?°í ??´?.
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MINOR_INADULTSERVER );
		}
		break;
	case 50: // ?¸¿ø?»?ð?? °è?¤??.
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}

// for Japan
void LoginCheckQueryForJP( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(id, "'") || IsCharInString(pw, "'") || IsCharInString(ip, "'"))	return;

	char guery[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(guery, "EXEC UP_GAMELOGINADULT_FREE_JP \'%s\', \'%s\', \'%s\', %d, %d, %d, %d, %d", 
				   id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eLoginCheckQueryForJP, dwConnectionIndex, guery) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY );
		return;
	}
}

void RLoginCheckQueryForJP( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIdx	= pMessage->dwID;
	DWORD stateNo			= atoi((char*)pData->Data[0]);
	WORD ServerNo			= (WORD)atoi((char*)pData->Data[1]);
	DWORD UserIdx			= atoi((char*)pData->Data[2]);
	WORD wType				= (WORD)atoi((char*)pData->Data[3]);
	int nTime				= atoi((char*)pData->Data[4]);
//	WORD bTen				= atoi((char*)pData->Data[5]);
	DWORD AuthKey			= atoi((char*)pData->Data[6]);
	BYTE UserLevel			= (BYTE)atoi((char*)pData->Data[7]);
	WORD AgentNo			= (WORD)atoi((char*)pData->Data[8]);

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);
	if(pUserInfo == NULL)
	{
		//���� �������� ���� �α��� delete�� ���ش�.
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)		// ??¹? ³ª°¡°í ´?¸¥ ³??? µé¾î¿? °æ¿ì
	{
		//���� �������� ���� �α��� delete�� ���ش�.		
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	pUserInfo->dwUserID = UserIdx;
	pUserInfo->UserLevel = UserLevel;
	pUserInfo->State = stateNo;
	
	switch(stateNo)
	{
	case 1: // success
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;	//AAA
			//�������� ����� DeleteLoginCheck�� ���ְ� �ȴ�. 
			//�����κп��� ȣ���ϴ� ���� OndisconnectUser���� ȣ���ϵ��� �����Ѵ�.

			g_LoginUser.Remove( UserIdx );

			// check userlevel
			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);

				DisconnectUser(dwConnectionIdx);
				return;
			}

			if( UserLevel == eUSERLEVEL_GM )
			{
				char strIP[16] = {0,};
				WORD wPort;
				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}

			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= UserLevel;
			msg.bData2		= (BYTE)wType;
				
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
				
			//g_pLoginUser.Remove( pUserInfo->dwUserID );
		}
		break;
	case 2: // id error
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: // password error
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: // already in other server
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData = UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;
	case 5: // block user
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
		}
		break;
	case 7: // DB Connection Error
		{
			ASSERTMSG(0, "login check error ret 0");
		}
		break;
	case 8: // check maxuser
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
		}
		break;
	case 50: // no account
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}
