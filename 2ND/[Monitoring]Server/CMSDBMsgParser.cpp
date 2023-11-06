#include "stdafx.h"
#include "stdio.h"
#include "DataBase.h"
#include "CMSDBMsgParser.h"
#include "MonitorNetwork.h"
#include "UserIPCheckMgr.h"


#ifdef DBID
#undef DBID
#endif
#define DBID(a)

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a) a,

DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	#include "MSDBMsgID.h"
};

char txt[512];

void InsertIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp )
{
	sprintf(txt, "EXEC RP_InsertIpAdress \'%s\'", strIp);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperInsertIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RInsertIpAdress( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	WORD wEcode = atoi((char*)pData[0].Data[0]);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	if( wEcode == 0 )
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTIP_ACK;
	else
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTIP_NACK;

	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

/*-------------------------------------------------------------------------------------------------
 * 함수이름 : InsertGMIpAdress
 * 목    적 : rm툴에서 gm툴에 접근이 가능한 ip 리스트를 RP_InsertGMIpAdress 쿼리를 통하여 등록
 * 주의사항 :
 *-------------------------------------------------------------------------------------------------*/
void InsertGMIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp )
{
	sprintf(txt, "EXEC RP_InsertGMIpAdress \'%s\'", strIp);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperInsertIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

/*-------------------------------------------------------------------------------------------------
 * 함수이름 : RInsertGMIpAdress
 * 목    적 : rm툴에서 gm툴에 접근이 가능한 ip 리스트 등록한 결과를 전송한다
 * 주의사항 :
 *-------------------------------------------------------------------------------------------------*/
void RInsertGMIpAdress( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	WORD wEcode = atoi((char*)pData[0].Data[0]);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	if( wEcode == 0 )
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTGMIP_ACK;
	else
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTGMIP_NACK;

	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void InsertOperator( WORD wConnectIdx, WORD wTempIdx, char* strId, char* strName, char* strPwd, WORD power )
{
	sprintf(txt, "EXEC RP_InsertOperator \'%s\', \'%s\', \'%s\', %d", strId, strName, strPwd, power);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery,eOperInsertOper, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RInsertOperator( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	WORD wEcode = atoi((char*)pData[0].Data[0]);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	if( wEcode == 0 )
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTOPERATOR_ACK;
	else
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTOPERATOR_NACK;

	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void DeleteIpAdress(WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx)
{
	sprintf(txt, "EXEC RP_DeleteIp %d", ipIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperDeleteIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

/*-------------------------------------------------------------------------------------------------
 * 함수이름 : DeleteGMIpAdress
 * 목    적 : rm툴에서 gm툴에 접근이 가능한 ip 리스트를 RP_DeleteGMIp 쿼리를 통해 삭제
 * 주의사항 :
 *-------------------------------------------------------------------------------------------------*/
void DeleteGMIpAdress(WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx)
{
	sprintf(txt, "EXEC RP_DeleteGMIp %d", ipIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperDeleteIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}
void DeleteOperator(WORD wConnectIdx, WORD wTempIdx, DWORD operIdx)
{
	sprintf(txt, "EXEC RP_DeleteOperator %d", operIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperDeleteOper, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void TotalIpAdressInfo(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx)
{
	sprintf(txt, "EXEC RP_IpAdressInfo %d", startIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperTotalIpInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RTotalIpAdressInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_IPTOTALINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_IPINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sIP[Msg.wCount].dwIPIdx = atoi((char*)pData[i].Data[eII_IPIdx]);
		strcpy( Msg.sIP[Msg.wCount].strIP, (char*)pData[i].Data[eII_IPAdress]);

		strcpy(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Date]);
		strcat(Msg.sIP[Msg.wCount].strRegDate, " ");
		strcat(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Time]);

		startIdx = Msg.sIP[Msg.wCount].dwIPIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_IPINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		TotalIpAdressInfo(connectIdx, tempIdx, startIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

/*-------------------------------------------------------------------------------------------------
 * 함수이름 : TotalGMIpAdressInfo
 * 목    적 : rm툴에서 gm툴에 접근이 가능한 ip 리스트를 RP_GMIpAdressInfo 쿼리를 통해 조회
 * 주의사항 :
 *-------------------------------------------------------------------------------------------------*/
void TotalGMIpAdressInfo(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx)
{
	sprintf(txt, "EXEC RP_GMIpAdressInfo %d", startIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperTotalGMIpInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

/*-------------------------------------------------------------------------------------------------
 * 함수이름 : RTotalGMIpAdressInfo
 * 목    적 : rm툴에서 gm툴에 접근이 가능한 ip 리스트를 전송한다.
 * 주의사항 :
 *-------------------------------------------------------------------------------------------------*/
void RTotalGMIpAdressInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_IPTOTALINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_GMIPINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sIP[Msg.wCount].dwIPIdx = atoi((char*)pData[i].Data[eII_IPIdx]);
		strcpy( Msg.sIP[Msg.wCount].strIP, (char*)pData[i].Data[eII_IPAdress]);

		strcpy(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Date]);
		strcat(Msg.sIP[Msg.wCount].strRegDate, " ");
		strcat(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Time]);

		startIdx = Msg.sIP[Msg.wCount].dwIPIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_GMIPINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		TotalIpAdressInfo(connectIdx, tempIdx, startIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}


void TotalOperatorInfo(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, DWORD operIdx)
{
	sprintf(txt, "EXEC RP_OperatorInfo %d, %d", startIdx, operIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperTotalOperInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RTotalOperatorInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_OPERTOTALINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_OPERATORINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;	
		
		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MyIdx = atoi((char*)pData[0].Data[eOI_MyIdx]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sOper[Msg.wCount].dwOperIdx = atoi((char*)pData[i].Data[eOI_OperIdx]);
		strcpy(Msg.sOper[Msg.wCount].strOperID, (char*)pData[i].Data[eOI_OperID]);
		strcpy(Msg.sOper[Msg.wCount].strOperName, (char*)pData[i].Data[eOI_OperName]);
		strcpy(Msg.sOper[Msg.wCount].strOperPWD, (char*)pData[i].Data[eOI_OperPwd]);
		Msg.sOper[Msg.wCount].wOperPower = atoi((char*)pData[i].Data[eOI_OperPower]);

		strcpy(Msg.sOper[Msg.wCount].strRegDate, (char*)pData[i].Data[eOI_Date]);
		strcat(Msg.sOper[Msg.wCount].strRegDate, " ");
		strcat(Msg.sOper[Msg.wCount].strRegDate, (char*)pData[i].Data[eOI_Time]);

		startIdx = Msg.sOper[Msg.wCount].dwOperIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_OPERATORINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		TotalOperatorInfo(connectIdx, tempIdx, startIdx, MyIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void OperatorLoginCheck(DWORD dwConnectIdx, char* strId, char* strPwd, char* strIp)
{
	sprintf(txt, "EXEC RP_OperatorLoginCheck \'%s\', \'%s\', \'%s\'", strId, strPwd, strIp);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperLoginCheck, dwConnectIdx, txt) == FALSE)
	{
	}
}

void ROperatorLoginCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	DWORD connectIdx = pMessage->dwID;

	if( count == 0 ) // ÀÌ»ó ¿À·ù
	{
		TMSG_WORD msg;
		msg.Category = MP_RMTOOL_CONNECT;
		msg.Protocol = MP_RMTOOL_CONNECT_NACK;
		msg.dwTemplateIdx = 0;
		msg.wData = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	WORD check = atoi((char*)pData[0].Data[eOI_ErroCode]);

	if( check != 0 ) // Á¢¼Ó ºÒ°¡
	{
		TMSG_WORD msg;
		msg.Category = MP_RMTOOL_CONNECT;
		msg.Protocol = MP_RMTOOL_CONNECT_NACK;
		msg.dwTemplateIdx = 0;
		msg.wData = check;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	TMSG_OPERATOR_LOGIN Msg;
	Msg.Category = MP_RMTOOL_CONNECT;
	Msg.Protocol = MP_RMTOOL_CONNECT_ACK;

	Msg.sOper.dwOperIdx = atoi((char*)pData[0].Data[eOI_OperIdx]);
	strcpy(Msg.sOper.strOperID, (char*)pData[0].Data[eOI_OperID]);
	strcpy(Msg.sOper.strOperName, (char*)pData[0].Data[eOI_OperName]);
	strcpy(Msg.sOper.strOperPWD, (char*)pData[0].Data[eOI_OperPwd]);
	Msg.sOper.wOperPower = atoi((char*)pData[0].Data[eOI_OperPower]);

	strcpy(Msg.sOper.strRegDate, (char*)pData[0].Data[eOI_Date]);
	strcat(Msg.sOper.strRegDate, " ");
	strcat(Msg.sOper.strRegDate, (char*)pData[0].Data[eOI_Time]);

	Msg.sIP.dwIPIdx = atoi((char*)pData[0].Data[eOI_IPIdx]);
	strcpy(Msg.sIP.strIP, (char*)pData[0].Data[eOI_IPAdress]);
	strcpy(Msg.sIP.strRegDate, (char*)pData[0].Data[eOI_IPDate]);
	strcat(Msg.sIP.strRegDate, " ");
	strcat(Msg.sIP.strRegDate, (char*)pData[0].Data[eOI_IPTime]);

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
	
	// ip regist
	IPCHECKMGR->RegistIP( Msg.sIP.strIP );
	IPCHECKMGR->RegistID( connectIdx, Msg.sOper.dwOperIdx, Msg.sOper.strOperID );
}

void SearchUserInfoById(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strId)
{
	sprintf(txt, "EXEC RP_SearchUserInfoById %d, \'%s\'", wServer, strId);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchUserInfoByIdCard(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strIdCard)
{
	char Card1[7] = {0,};
	char Card2[8] = {0,};
	memcpy(Card1, strIdCard, 6);
	memcpy(Card2, &strIdCard[6], 7);

	sprintf(txt, "EXEC RP_SearchUserInfoByIdCard %d, \'%s\', \'%s\'", wServer, Card1, Card2);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchUserInfoByCharName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strCharName)
{
	sprintf(txt, "EXEC RP_SearchUserInfoByCharName %d, \'%s\'", wServer, strCharName);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchUserInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_USERSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_USERSEARCHRESULT Info;

		Info.sUser.bLoginFlag = atoi((char*)pData[0].Data[eUI_IsLogin]);
		Info.sUser.dwMemberNo = atoi((char*)pData[0].Data[eUI_User_Idx]);
		strcpy(Info.sUser.strID, (char*)pData[0].Data[eUI_Id]);
		strcpy(Info.sUser.strPass, (char*)pData[0].Data[eUI_Pwd]);
		strcpy(Info.sUser.strName, (char*)pData[0].Data[eUI_Name]);
		strcpy(Info.sUser.strIdCard, (char*)pData[0].Data[eUI_Idcard]);
		strcpy(Info.sUser.strPhon, (char*)pData[0].Data[eUI_Phone]);
		strcpy(Info.sUser.strEmail, (char*)pData[0].Data[eUI_Email]);
		strcpy(Info.sUser.strRegDateTime, (char*)pData[0].Data[eUI_Regdate]);
		strcpy(Info.sUser.strLastLoginTime, (char*)pData[0].Data[eUI_LastLogintime]);
		strcpy(Info.sUser.strLastLogoutTime, (char*)pData[0].Data[eUI_LastLogouttime]);

		Info.sUser.nTotalTime = atoi((char*)pData[0].Data[eUI_Totaltime]);
		int temp = Info.sUser.nTotalTime;
		int oo, mm, ss;
		ss = temp%60;
		mm = (temp/60)%60;
		oo = (temp/3600);
		sprintf(Info.sUser.strTotalTime, "%d½Ã°£ %dºÐ %dÃÊ", oo, mm, ss );

		Info.sUser.wUserLevel = atoi((char*)pData[0].Data[eUI_UserLevel]);
		Info.wServer = atoi((char*)pData[0].Data[eUI_Char_Server]);

		Info.Category = MP_RMTOOL_USER;
		Info.Protocol = MP_RMTOOL_USERSEARCH_ACK;
		Info.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}

void CharacterBaseInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
	sprintf(txt, "EXEC RP_CharacterInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if(!count)
	{
		// error
		TMSG_WORD Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTERINFO_NACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.wData = 0; //
		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	TMSG_CHARACTER_BASEINFO Msg;
	memset( &Msg, 0, sizeof(Msg) ); 
		
	Msg.m_sObjinfo.dwObjectID = atoi((char*)pData->Data[eCS_CharacterIdx]);
	Msg.m_sObjinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserIdx]);
	strcpy( Msg.m_sObjinfo.ObjectName, (char*)pData->Data[eCS_Name] );
	
	Msg.m_sTotalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Msg.m_sTotalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Msg.m_sTotalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Msg.m_sTotalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Msg.m_sTotalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Msg.m_sTotalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Msg.m_sTotalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Msg.m_sTotalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIdx]);
	Msg.m_sTotalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_MunpaPositon]);
	Msg.m_sTotalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Msg.m_sTotalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	Msg.m_sTotalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);
	Msg.m_sTotalinfo.MaxLife = atoi((char*)pData->Data[eCS_MaxLife]);

	Msg.m_sHeroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);
	Msg.m_sHeroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Msg.m_sHeroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Msg.m_sHeroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxGrade]);
	Msg.m_sHeroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	strcpy( Msg.m_sTotalinfo.GuildName, (char*)pData->Data[eCS_MunpaName] );
	Msg.m_sHeroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyIdx]);
	Msg.m_sHeroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Msg.m_sHeroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Msg.m_sHeroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Msg.m_sHeroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Msg.m_sHeroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Msg.m_sTotalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);

	// Send
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_BASEINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void UpdateCharacterBaseInfo(WORD wConnectIdx, WORD wTempIdx, TMSG_CHARACTER_UPDATEBASEINFO* Info)
{
	sprintf( txt, "EXEC RP_UpdateCharacterInfo %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				  Info->CharIdx, Info->ExpPoint, Info->Level, Info->BadFame, 
				  Info->wGenGol, Info->wMinChub, Info->wCheRyuk, Info->wSimMek,
				  Info->LevelUpPoint, Info->Money, Info->AbilityExp );
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUpdateBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterInvenInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
/*	sprintf(txt, "EXEC RP_CharacterInvenInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterInvenInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
	*/
}

void RCharacterInvenInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_INVENINFO Msg;
	memset(&Msg.m_sIteminfo, 0, sizeof(Msg.m_sIteminfo));

	POSTYPE p = 0;
	for(DWORD  i = 0; i < count; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		// ±×¸®µå(ÀÎº¥Åä¸®)¾ÆÀÌÅÛ ¼ÂÆÃ
		if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
		{
			ItemPos -= TP_INVENTORY_START;
			Msg.m_sIteminfo.Inventory[ItemPos].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			Msg.m_sIteminfo.Inventory[ItemPos].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			Msg.m_sIteminfo.Inventory[ItemPos].Position = atoi((char*)pData[i].Data[eCI_Position]);
			Msg.m_sIteminfo.Inventory[ItemPos].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			Msg.m_sIteminfo.Inventory[ItemPos].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
		}
		else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
		{				
			POSTYPE ItemGrid = ItemPos - TP_WEAR_START;
			Msg.m_sIteminfo.WearedItem[ItemGrid].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].Position = atoi((char*)pData[i].Data[eCI_Position]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	// Á¤º¸ º¸³»±â
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_INVENINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
	*/
}

void CharacterPyogukInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx)
{
	sprintf(txt, "EXEC RP_CharacterPyogukInfo %d", dwUserIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_PYOGUKINFO Msg;
	DWORD UserIdx;

	if( pMessage->dwResult == 0 )
	{
		UserIdx = 0;
		Msg.m_wPyogukNum = 0;
		Msg.m_dwMoney = 0;
	}
	else
	{
		UserIdx = atoi((char*)pData->Data[eCP_UserIdx]);
		Msg.m_wPyogukNum = atoi((char*)pData->Data[eCP_PyogukNum]);
		Msg.m_dwMoney = atoi((char*)pData->Data[eCP_PyogukMoney]);
	}

	if( Msg.m_wPyogukNum != 0)
		CharacterPyogukItemInfo( connectIdx, tempIdx, UserIdx, TP_PYOGUK_START-1 );

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKINFO;
	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void UpdateCharacterPyogukInfo(WORD wConnectIdx, WORD wTempIdx, DWORD UserIdx, WORD PyogukNum, MONEYTYPE Money)
{
	sprintf(txt, "EXEC RP_UpdatePyogukInfo %d, %d, %d", UserIdx, PyogukNum, Money );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUpdatePyogukInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterPyogukItemInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, POSTYPE startPos)
{
	sprintf(txt, "EXEC RP_CharacterPyogukItemInfo %d, %d", dwUserIdx, startPos);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMINFO_LARGECOUNT Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKITEMINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.m_bEnd = TRUE;
		Msg.m_wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD UserIdx = atoi((char*)pData[0].Data[ePI_UserId]);
	POSTYPE startPos = 0;
	Msg.m_wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[ePI_Position]);
		if(TP_PYOGUK_START<= ItemPos && ItemPos < TP_PYOGUK_END)
		{
			Msg.m_Item[Msg.m_wCount].dwDBIdx			= atoi((char*)pData[i].Data[ePI_DBIDX]);
			Msg.m_Item[Msg.m_wCount].wIconIdx			= atoi((char*)pData[i].Data[ePI_IDX]);
			Msg.m_Item[Msg.m_wCount].Position			= atoi((char*)pData[i].Data[ePI_Position]);
			Msg.m_Item[Msg.m_wCount].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
			Msg.m_Item[Msg.m_wCount].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
			Msg.m_Item[Msg.m_wCount].RareIdx = atoi((char*)pData[i].Data[ePI_Durability+1]);
			
			startPos = Msg.m_Item[Msg.m_wCount].Position;
			Msg.m_wCount++;
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKITEMINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.m_bEnd = FALSE;
		CharacterPyogukItemInfo(connectIdx, tempIdx, UserIdx, startPos);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.m_bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void CharacterMugongInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
	sprintf(txt, "EXEC RP_CharacterMugongInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterMugongInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);
	DWORD count = pMessage->dwResult;
	
	TMSG_CHARACTER_MUGONGINFO Msg;
	TMSG_CHARACTER_MUGONGINFO2 Msg2;
	memset(&Msg, 0, sizeof(TMSG_CHARACTER_MUGONGINFO));
	memset(&Msg2, 0, sizeof(TMSG_CHARACTER_MUGONGINFO2));

	DWORD ec = 0;
	for(DWORD i = 0; i < count; i++)
	{
		WORD Mugongidx = (WORD)atoi((char*)pData[i].Data[eCM_IDX]);
		
#ifdef _KOREA_LOCAL_
		POSTYPE realpos = atoi((char*)pData[i].Data[eCM_Position]);
		POSTYPE MugongPos = 0;
		if( realpos >= TP_JINBUB_START && realpos < TP_JINBUB_END )
			MugongPos = realpos - 440;			
		else
			MugongPos = realpos - TP_MUGONG1_START;
#else
		POSTYPE MugongPos = atoi((char*)pData[i].Data[eCM_Position]) - TP_MUGONG1_START;
#endif
		if( Msg.m_sMugongInfo.mugong[MugongPos].dwDBIdx == 0 )
		{
			Msg.m_sMugongInfo.mugong[MugongPos].wIconIdx = Mugongidx;
			Msg.m_sMugongInfo.mugong[MugongPos].dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
			Msg.m_sMugongInfo.mugong[MugongPos].ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
			Msg.m_sMugongInfo.mugong[MugongPos].Sung = atoi((char*)pData[i].Data[eCM_Sung]);
			Msg.m_sMugongInfo.mugong[MugongPos].Position = atoi((char*)pData[i].Data[eCM_Position]);
			Msg.m_sMugongInfo.mugong[MugongPos].QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
			Msg.m_sMugongInfo.mugong[MugongPos].bWear = atoi((char*)pData[i].Data[eCM_Wear]);
		}
		else
		{
			if( ec < 5 )
			{
				Msg2.m_MugongBase[ec].wIconIdx = Mugongidx;
				Msg2.m_MugongBase[ec].dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
				Msg2.m_MugongBase[ec].ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
				Msg2.m_MugongBase[ec].Sung = atoi((char*)pData[i].Data[eCM_Sung]);
				Msg2.m_MugongBase[ec].Position = atoi((char*)pData[i].Data[eCM_Position]);
				Msg2.m_MugongBase[ec].QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
				Msg2.m_MugongBase[ec].bWear = atoi((char*)pData[i].Data[eCM_Wear]);
				
				++ec;
				Msg2.m_dwCount = ec;
			}
		}
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MUGONGINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));

	if( Msg2.m_dwCount )
	{
		Msg2.Category = MP_RMTOOL_CHARACTER;
		Msg2.Protocol = MP_RMTOOL_CHARACTER_MUGONGINFO2;
		Msg2.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Msg2, sizeof(TMSG_CHARACTER_MUGONGINFO2));
	}
}

void CharacterAbilityInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
	sprintf(txt, "EXEC RP_CharacterAbilityInfo %d", dwCharIdx);	
	if(g_DB.FreeMiddleQuery(RCharacterAbilityInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ABILITYINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 1 )
	{
		for(int kind = 0;kind<eAGK_Max;++kind)
			memcpy(Msg.sAbility.AbilityDataArray[kind], (char*)pData[0].Data[kind+1], MAX_ABILITY_NUM_PER_GROUP+1);
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_ABILLITYINFO;
	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void CharacterItemOptionInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx)
{
	sprintf(txt, "EXEC  RP_CharacterItemOptionInfo %d, %d, %d", dwUserIdx, dwCharIdx, StartIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMOPTIONINFO	Msg;
	memset(&Msg, 0, sizeof(Msg));

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTERINFO_ITEMOPTIONINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( count == 0 )
	{
		// ÇöÀç±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÓÀ» ÀÇ¹ÌÇÑ´Ù.
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD UserIdx = atoi((char*)pData[0].Data[eCIOI_UserID]);
	DWORD CharIdx = atoi((char*)pData[0].Data[eCIOI_CharID]);
	DWORD StartIdx = 0;

	for( DWORD i = 0 ; i < count ; ++i )
	{
		Msg.OptionInfo[i].dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_OptionID]);
		Msg.OptionInfo[i].dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_ItemDBID]);
		Msg.OptionInfo[i].GenGol				= (WORD)atoi((char*)pData[i].Data[eCIOI_GenGol]);
		Msg.OptionInfo[i].MinChub				= (WORD)atoi((char*)pData[i].Data[eCIOI_MinChub]);				
		Msg.OptionInfo[i].CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_CheRyuk]);				
		Msg.OptionInfo[i].SimMek				= (WORD)atoi((char*)pData[i].Data[eCIOI_SimMek]);					
		Msg.OptionInfo[i].Life					= (DWORD)atoi((char*)pData[i].Data[eCIOI_Life]);					
		Msg.OptionInfo[i].NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_NaeRyuk]);				
		Msg.OptionInfo[i].Shield				= (DWORD)atoi((char*)pData[i].Data[eCIOI_Shield]);					
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_FIRE,	(float)atof((char*)pData[i].Data[eCIOI_FireAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_WATER,	(float)atof((char*)pData[i].Data[eCIOI_WaterAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_TREE,	(float)atof((char*)pData[i].Data[eCIOI_TreeAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_IRON,	(float)atof((char*)pData[i].Data[eCIOI_GoldAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atof((char*)pData[i].Data[eCIOI_EarthAttrRegist]) );
		Msg.OptionInfo[i].PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyAttack]);
		Msg.OptionInfo[i].CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_FIRE,	(float)atof((char*)pData[i].Data[eCIOI_FireAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_WATER,	(float)atof((char*)pData[i].Data[eCIOI_WaterAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_TREE,	(float)atof((char*)pData[i].Data[eCIOI_TreeAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_IRON,	(float)atof((char*)pData[i].Data[eCIOI_GoldAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atof((char*)pData[i].Data[eCIOI_EarthAttrAttack]) );
		Msg.OptionInfo[i].PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyDefense]);				

		Msg.wCount++;
		StartIdx = Msg.OptionInfo[i].dwItemDBIdx;
	}

	if( count >= 100 )
	{
		Msg.bEnd = FALSE;
		CharacterItemOptionInfo( connectIdx, tempIdx, UserIdx, CharIdx, StartIdx );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void SearchMunpaIdxByName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strName)
{
	sprintf(txt, "EXEC RP_SearchMunpaByName \'%s\'", strName);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchMunpaByName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchMunpaIdx(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_MUNPA;
		msg.Protocol = MP_RMTOOL_MUNPASEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		DWORD dwMunpaIdx = atoi((char*)pData[0].Data[0]);
		MunpaBaseInfo(connectIdx, tempIdx, dwMunpaIdx);
	}
}

void MunpaBaseInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx)
{
	sprintf(txt, "EXEC RP_MunpaBaseInfo %d", MunpaIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RMunpaBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_MUNPA;
		msg.Protocol = MP_RMTOOL_MUNPASEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_MUNPABASEINFO Info;

		Info.sBaseInfo.dwMunpaIdx = atoi((char*)pData[0].Data[eMB_MunpaId]);
		strcpy(Info.sBaseInfo.strMunpaName, (char*)pData[0].Data[eMB_MunpaName]);
		strcpy(Info.sBaseInfo.strMunjuName, (char*)pData[0].Data[eMB_MunjuName]);
		
		strcpy(Info.sBaseInfo.strRegDate, (char*)pData[0].Data[eMB_RegDate]);
		strcat(Info.sBaseInfo.strRegDate, " ");
		strcat(Info.sBaseInfo.strRegDate, (char*)pData[0].Data[eMB_RegTime]);

		Info.sBaseInfo.wMemberNum = atoi((char*)pData[0].Data[eMB_MemberNum]);
		Info.sBaseInfo.wLocation = atoi((char*)pData[0].Data[eMB_Location]);
		Info.sBaseInfo.bMunpaLevel = atoi((char*)pData[0].Data[eMB_MunpaLevel]);
		Info.sBaseInfo.dwFamous = atoi((char*)pData[0].Data[eMB_Famous]);
		Info.sBaseInfo.bKind = atoi((char*)pData[0].Data[eMB_Kind]);
		Info.sBaseInfo.dwMoney = atoi((char*)pData[0].Data[eMB_MunpaMoney]);

		Info.Category = MP_RMTOOL_MUNPA;
		Info.Protocol = MP_RMTOOL_MUNPABASEINFO;
		Info.dwTemplateIdx = tempIdx;

//		MunpaItemInfo(connectIdx, tempIdx, Info.sBaseInfo.dwMunpaIdx, TP_MUNPAWAREHOUSE_START - 1 );
		MunpaMemberInfo(connectIdx, tempIdx, Info.sBaseInfo.dwMunpaIdx, 0);

		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}

void MunpaItemInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, POSTYPE startPos )
{
	sprintf(txt, "EXEC RP_MunpaItemInfo %d, %d", MunpaIdx, startPos);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RMunpaItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMINFO_LARGECOUNT Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_MUNPAITEMINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.m_bEnd = TRUE;
		Msg.m_wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MunpaIdx = atoi((char*)pData[0].Data[ePI_UserId]);
	POSTYPE startPos = 0;
	Msg.m_wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
/*		POSTYPE ItemPos = atoi((char*)pData[i].Data[ePI_Position]);
		if(TP_MUNPAWAREHOUSE_START<= ItemPos && ItemPos < TP_MUNPAWAREHOUSE_END)
		{
			Msg.m_Item[Msg.m_wCount].dwDBIdx			= atoi((char*)pData[i].Data[ePI_DBIDX]);
			Msg.m_Item[Msg.m_wCount].wIconIdx			= atoi((char*)pData[i].Data[ePI_IDX]);
			Msg.m_Item[Msg.m_wCount].Position			= atoi((char*)pData[i].Data[ePI_Position]);
			Msg.m_Item[Msg.m_wCount].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
			Msg.m_Item[Msg.m_wCount].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
			
			startPos = Msg.m_Item[Msg.m_wCount].Position;
			Msg.m_wCount++;
		}
		else
		{
			ASSERT(0);
			continue;
		}
		*/
	}

	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_MUNPAITEMINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.m_bEnd = FALSE;
		MunpaItemInfo(connectIdx, tempIdx, MunpaIdx, startPos);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.m_bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void MunpaMemberInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, DWORD startIdx)
{
	sprintf(txt, "EXEC RP_MunpaMemberInfo %d, %d", MunpaIdx, startIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaMemberInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RMunpaMemberInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_MUNPA_MEMBERINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_MUNPAMEMBERINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MunpaIdx = atoi((char*)pData[0].Data[eMM_MunpaId]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sMember[Msg.wCount].dwCharno		= atoi((char*)pData[i].Data[eMM_MemberIdx]);
		strcpy(Msg.sMember[Msg.wCount].strName, (char*)pData[i].Data[eMM_MemberName]);
		Msg.sMember[Msg.wCount].level		= atoi((char*)pData[i].Data[eMM_MemberLevel]);
		Msg.sMember[Msg.wCount].bJikChek		= atoi((char*)pData[i].Data[eMM_MemberJikChek]);

		strcpy(Msg.sMember[Msg.wCount].strRegDate, (char*)pData[i].Data[eMM_MemberDate]);
		strcat(Msg.sMember[Msg.wCount].strRegDate, " ");
		strcat(Msg.sMember[Msg.wCount].strRegDate, (char*)pData[i].Data[eMM_MemberTime]);
			
		startIdx = Msg.sMember[Msg.wCount].dwCharno;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_MUNPAMEMBERINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		MunpaMemberInfo(connectIdx, tempIdx, MunpaIdx, startIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void GameLogItemMoveLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
	sprintf(txt, "EXEC RP_GameLogItemMoveLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemMoveLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogItemMoveLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGITEM1 Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGI_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGI_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGI_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGI_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogidx = atoi((char*)pData[i].Data[eGI_LogIdx]);
		Msg.sLog[Msg.wCount].bLogtype = atoi((char*)pData[i].Data[eGI_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGI_CharIdx]);
		Msg.sLog[Msg.wCount].dwTargetIdx = atoi((char*)pData[i].Data[eGI_TargetIdx]); 
		strcpy(Msg.sLog[Msg.wCount].strTargetName, (char*)pData[i].Data[eGI_TargetName]);
		Msg.sLog[Msg.wCount].dwItemIdx = atoi((char*)pData[i].Data[eGI_ItemIdx]);
		Msg.sLog[Msg.wCount].dwItemDBIdx = atoi((char*)pData[i].Data[eGI_ItemDBIdx]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logdate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logtime]);
		Msg.sLog[Msg.wCount].dwMoney = atoi((char*)pData[i].Data[eGI_Money]);

		startIdx = Msg.sLog[Msg.wCount].dwLogidx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEMMOVELOG;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemMoveLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void GameLogItemObtainLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
	sprintf(txt, "EXEC RP_GameLogItemObtainLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemObtainLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogItemObtainLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGITEM1 Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGI_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGI_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGI_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGI_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogidx = atoi((char*)pData[i].Data[eGI_LogIdx]);
		Msg.sLog[Msg.wCount].bLogtype = atoi((char*)pData[i].Data[eGI_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGI_CharIdx]);
		Msg.sLog[Msg.wCount].dwTargetIdx = atoi((char*)pData[i].Data[eGI_TargetIdx]); 
		strcpy(Msg.sLog[Msg.wCount].strTargetName, (char*)pData[i].Data[eGI_TargetName]);
		Msg.sLog[Msg.wCount].dwItemIdx = atoi((char*)pData[i].Data[eGI_ItemIdx]);
		Msg.sLog[Msg.wCount].dwItemDBIdx = atoi((char*)pData[i].Data[eGI_ItemDBIdx]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logdate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logtime]);
		Msg.sLog[Msg.wCount].dwMoney = atoi((char*)pData[i].Data[eGI_Money]);

		startIdx = Msg.sLog[Msg.wCount].dwLogidx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEMOBTAINLOG;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemMoveLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void GameLogBaseInfoLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
	sprintf(txt, "EXEC RP_GameLogBaseInfoLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameBaseInfoLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogBaseInfoLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
}

void GameLogMoneyLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
	sprintf(txt, "EXEC RP_GameLogMoneyLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameMoneyLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogMoneyLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGMONEY Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGLM_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLM_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGLM_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGLM_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogIdx = atoi((char*)pData[i].Data[eGLM_LogIdx]);
		Msg.sLog[Msg.wCount].LogType = atoi((char*)pData[i].Data[eGLM_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGLM_CharIdx]);
		Msg.sLog[Msg.wCount].dwInvenMoney = atoi((char*)pData[i].Data[eGLM_InvenMoney]);
		Msg.sLog[Msg.wCount].dwPyogukMoney = atoi((char*)pData[i].Data[eGLM_PyogukMoney]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGLM_LogDate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGLM_LogTime]);
		startIdx = Msg.sLog[Msg.wCount].dwLogIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_MONEY;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogMoneyLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void GameLogItemFromChrName( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, char* strChrName )
{
	sprintf( txt, "EXEC RP_GameLogItemFromChrName %d, \'%s\', %d, \'%s\', \'%s\'", dwStartIdx, strChrName, bDay, strStart, strEnd );
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemFromChrName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogItemFromChrName( LPQUERY pData, LPDBMESSAGE pMessage )
{
/*	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_GAMELOGITEM Msg;
	memset( &Msg, 0, sizeof(Msg) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
//		msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(msg) );
		return;
	}

	strcpy( Msg.strChrName, (char*)pData[0].Data[eGLI_ChrName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLI_DayOnly]);
	strcpy( Msg.strStart, (char*)pData[0].Data[eGLI_StartDate] );
	strcpy( Msg.strEnd, (char*)pData[0].Data[eGLI_EndDate] );
	Msg.wCount = 0;

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eGLI_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eGLI_LogType]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eGLI_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eGLI_ItemDBIdx]);
		Msg.sLog[i].dwCurChrIdx = atoi((char*)pData[i].Data[eGLI_CurChrIdx]);
		Msg.sLog[i].dwBeforeChrIdx = atoi((char*)pData[i].Data[eGLI_BeforeChrIdx]);
		Msg.sLog[i].dwMoney = atoi((char*)pData[i].Data[eGLI_Money]);
		strcpy( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogDate] );
		strcat( Msg.sLog[i].strLogDate, " ");
		strcat( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogTime] );
		strcpy( Msg.sLog[i].strCurChrName, (char*)pData[i].Data[eGLI_CurChrName] );
		strcpy( Msg.sLog[i].strBeforeChrName, (char*)pData[i].Data[eGLI_BeforeChrName] );
		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemFromChrName( connectIdx, tempIdx, startIdx, 0, Msg.bDayOnly, Msg.strStart, Msg.strEnd, Msg.strChrName );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
	*/
}

void GameLogItemFromItemDBIndex( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, DWORD dwItemDBIdx )
{
	sprintf( txt, "EXEC RP_GameLogItemFromItemDBIdx %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwItemDBIdx, bDay, strStart, strEnd );
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemFromItemDBIndex, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RGameLogItemFromItemDBIndex( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_GAMELOGITEM Msg;
	memset( &Msg, 0, sizeof(Msg) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
//		msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(msg) );
		return;
	}

	Msg.dwItemDBIdx = atoi((char*)pData[0].Data[eGLI_ChrName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLI_DayOnly]);
	strcpy( Msg.strStart, (char*)pData[0].Data[eGLI_StartDate] );
	strcpy( Msg.strEnd, (char*)pData[0].Data[eGLI_EndDate] );
	Msg.wCount = 0;

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eGLI_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eGLI_LogType]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eGLI_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eGLI_ItemDBIdx]);
		Msg.sLog[i].dwCurChrIdx = atoi((char*)pData[i].Data[eGLI_CurChrIdx]);
		Msg.sLog[i].dwBeforeChrIdx = atoi((char*)pData[i].Data[eGLI_BeforeChrIdx]);
		Msg.sLog[i].dwMoney = atoi((char*)pData[i].Data[eGLI_Money]);
		strcpy( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogDate] );
		strcat( Msg.sLog[i].strLogDate, " ");
		strcat( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogTime] );
		strcpy( Msg.sLog[i].strCurChrName, (char*)pData[i].Data[eGLI_CurChrName] );
		strcpy( Msg.sLog[i].strBeforeChrName, (char*)pData[i].Data[eGLI_BeforeChrName] );
		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemFromItemDBIndex( connectIdx, tempIdx, startIdx, 0, Msg.bDayOnly, Msg.strStart, Msg.strEnd, Msg.dwItemDBIdx );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );	
}

void ItemDelete(DWORD dwItemDBIdx, DWORD dwOptionDBIdx)
{
	sprintf(txt, "EXEC RP_ItemDelete %d, %d", dwItemDBIdx, dwOptionDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eItemDelete, 0, txt) == FALSE)
	{
	}
}

void InvenItemInsert( WORD wConnectIdx, WORD wTempIdx, char* sCharName, DWORD dwCharIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo)
{
	sprintf(txt, "EXEC RP_InvenItemInsert \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		sCharName,
		dwCharIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eInvenItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RInvenItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		char name[17];
		strcpy( name, (char*)pData[0].Data[0] );
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		ITEMBASE item;
		item.dwDBIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		item.wIconIdx = (WORD)atoi((char*)pData[0].Data[3]);
		item.Position = (WORD)atoi((char*)pData[0].Data[4]);
		item.QuickPosition = (WORD)atoi((char*)pData[0].Data[5]);
		item.Durability = (WORD)atoi((char*)pData[0].Data[6]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwCharIdx, name,
					   item.dwDBIdx, item.wIconIdx, item.Position, item.Durability );

//		InsertItemMoneyLog( 0, "", dwCharIdx, name, eLog_RMTool, 0, 0, 0, item.wIconIdx, item.dwDBIdx,
//							item.Position, item.Position, item.Durability, 0 );
	}
}

void PyogukItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo )
{
	sprintf(txt, "EXEC RP_PyogukItemInsert %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwUserIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, ePyogukItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RPyogukItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[0]);
		DWORD dwItemDBIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		WORD wItemPos = (WORD)atoi((char*)pData[0].Data[3]);
		WORD wItemQPos = (WORD)atoi((char*)pData[0].Data[4]);
		DWORD dwItemDu = (DWORD)atoi((char*)pData[0].Data[5]);
		DWORD dwMunpaIdx = (DWORD)atoi((char*)pData[0].Data[6]);
		DWORD dwPyogukIdx = (DWORD)atoi((char*)pData[0].Data[7]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwPyogukIdx, "",
					   dwItemDBIdx, dwItemIdx, wItemPos, dwItemDu );

//		InsertItemMoneyLog( 0, "", dwPyogukIdx, "", eLog_RMTool, 0, 0, 0, dwItemIdx, dwItemDBIdx,
//							wItemPos, wItemPos, dwItemDu, 0 );
	}
}

void MunpaItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwMunpaIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo )
{
	sprintf(txt, "EXEC RP_MunpaItemInsert %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwMunpaIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RMunpaItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[0]);
		DWORD dwItemDBIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		WORD wItemPos = (WORD)atoi((char*)pData[0].Data[3]);
		WORD wItemQPos = (WORD)atoi((char*)pData[0].Data[4]);
		DWORD dwItemDu = (DWORD)atoi((char*)pData[0].Data[5]);
		DWORD dwMunpaIdx = (DWORD)atoi((char*)pData[0].Data[6]);
		DWORD dwPyogukIdx = (DWORD)atoi((char*)pData[0].Data[7]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwMunpaIdx, "",
					   dwItemDBIdx, dwItemIdx, wItemPos, dwItemDu );

//		InsertItemMoneyLog( 0, "", dwMunpaIdx, "", eLog_RMTool, 0, 0, 0, dwItemIdx, dwItemDBIdx,
//							wItemPos, wItemPos, dwItemDu, 0 );
	}
}

void ItemUpdate(DWORD dwDBIdx, ITEMBASE* pItem)
{
	sprintf(txt, "EXEC RP_ItemUpdate %d, %d, %d, %d", dwDBIdx, pItem->wIconIdx, pItem->Durability, pItem->Position);
	if(g_DB.Query(eQueryType_FreeQuery, eItemUpdate, 0, txt) == FALSE)
	{
	}
}

void ItemOptionUpdate(DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
	sprintf(txt, "EXEC RP_ItemOptionUpdate %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwItemDBIdx, 
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionUpdate, 0, txt) == FALSE)
	{
	}
}

void ItemOptionInsert(DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
	sprintf(txt, "EXEC RP_ItemOptionInsert %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwItemDBIdx, 
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionInsert, 0, txt) == FALSE)
	{
	}
}

void ItemOptionDelete(DWORD dwItemDBIdx)
{
	sprintf(txt, "EXEC RP_ItemOptionDelete %d", dwItemDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionDelete, 0, txt) == FALSE)
	{
	}
}

void MugongInsert(DWORD dwCharIdx, MUGONGBASE* pMugong)
{
	sprintf(txt, "EXEC RP_MugongInsert %d, %d, %d, %d, %d, %d", dwCharIdx, pMugong->wIconIdx, pMugong->ExpPoint, pMugong->Sung, pMugong->Position, pMugong->bWear);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongInsert, 0, txt) == FALSE)
	{
	}
}

void MugongUpdate(DWORD dwMugongDBIdx, MUGONGBASE* pMugong)
{
	sprintf(txt, "EXEC RP_MugongUpdate %d, %d, %d, %d", dwMugongDBIdx, pMugong->wIconIdx, pMugong->ExpPoint, pMugong->Sung);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongUpdate, 0, txt) == FALSE)
	{
	}
}

void MugongDelete(DWORD dwMugongDBIdx)
{
	sprintf(txt, "EXEC RP_MugongDelete %d", dwMugongDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongDelete, 0, txt) == FALSE)
	{
	}
}

void AbilityUpdate(DWORD dwCharIdx, ABILITY_TOTALINFO* pAbility)
{
	sprintf(txt, "RP_AbilityUpdate %d, \'%s\', \'%s\', \'%s\', \'%s\'",
			dwCharIdx, pAbility->AbilityDataArray[eAGK_Battle], pAbility->AbilityDataArray[eAGK_KyungGong],
			pAbility->AbilityDataArray[eAGK_Society], pAbility->AbilityDataArray[eAGK_Job]);

	if(g_DB.Query(eQueryType_FreeQuery, eAbilityUpdate, 0, txt) == FALSE)
	{
	}
}

void CharacterWearItemUpdate(DWORD dwCharIdx, BYTE type, WORD wItemIdx)
{
	sprintf(txt, "RP_CharacterWearItemUpdate %d, %d, %d", dwCharIdx, type, wItemIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterWearItemUpdate, 0, txt) == FALSE)
	{
	}
}

// jsd add 04.08.25 -----------------------------
// IP Check
void TotalIpInfo( DWORD startIdx )
{
	sprintf(txt, "EXEC RP_IpAdressInfo %d", startIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eTotalIpInfo, 0, txt) == FALSE)
	{
	}
}

void RTotalIpInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	IPCHECKMGR->Release();
	DWORD count = pMessage->dwResult;

	if( count == 0 )
		return;

	DWORD startIdx = 0;

	for( DWORD i = 0; i < count; ++i )
	{
		IPCHECKMGR->RegistIP( (char*)pData[i].Data[eII_IPAdress] );
		startIdx = atoi((char*)pData[i].Data[eII_IPIdx]);
	}

// ip 100°³ ³ÑÀ¸¸é ¾ÈµÊ
//	if( count >= 100)
//		TotalIpInfo( startIdx );
}

// New Log
void ItemMoneyLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ItemMoneyLogFromType %d, %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx, dwItem, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eItemMoneyLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RItemMoneyLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_LOGITEMMONEY Msg;
	memset( &Msg, 0, sizeof(TMSG_LOGITEMMONEY) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = 0;
	Msg.wType = atoi((char*)pData[0].Data[eIML_Type]);
	Msg.dwCharIdx = atoi((char*)pData[0].Data[eIML_ChrName]);
	Msg.dwItem = atoi((char*)pData[0].Data[eIML_Item]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eIML_DayOnly]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[eIML_SDate] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[eIML_EDate] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eIML_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eIML_LogType]);
		strcpy( Msg.sLog[i].sFromChrName, (char*)pData[i].Data[eIML_FromChrName] );
		Msg.sLog[i].dwFromChrIdx = atoi((char*)pData[i].Data[eIML_FromChrIdx]);		
		strcpy( Msg.sLog[i].sToChrName, (char*)pData[i].Data[eIML_ToChrName] );
		Msg.sLog[i].dwToChrIdx = atoi((char*)pData[i].Data[eIML_ToChrIdx]);		
		Msg.sLog[i].dwFromTotalMoney = atoi((char*)pData[i].Data[eIML_FromTotalMoney]);
		Msg.sLog[i].dwToTotalMoney = atoi((char*)pData[i].Data[eIML_ToTotalMoney]);
		Msg.sLog[i].dwChangeMoney = atoi((char*)pData[i].Data[eIML_ChangeMoney]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eIML_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eIML_ItemDBIdx]);
		Msg.sLog[i].dwItemDur = atoi((char*)pData[i].Data[eIML_ItemDur]);
		Msg.sLog[i].dwItemFromPosition = atoi((char*)pData[i].Data[eIML_ItemFromPosition]);
		Msg.sLog[i].dwItemToPosition = atoi((char*)pData[i].Data[eIML_ItemToPosition]);
		strcpy( Msg.sLog[i].sRegDate, (char*)pData[i].Data[eIML_RegDate] );
		strcat( Msg.sLog[i].sRegDate, " ");
		strcat( Msg.sLog[i].sRegDate, (char*)pData[i].Data[eIML_RegTime]);

		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_ITEMMONEYLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ItemMoneyLogFromType( connectIdx, tempIdx, startIdx, Msg.wType, Msg.dwCharIdx, Msg.dwItem, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}

void CharacterLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_CharacterLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eCharacterLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RCharacterLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_CHARACTERLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_CHARACTERLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwGrade = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwGengoal = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwDex = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwStrength = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSimmak = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwPoint = atoi((char*)pData[i].Data[11]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_CHARACTERLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		CharacterLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_CHARACTERLOG) );
}

void ExpPointLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ExpPointLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eExpPointLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RExpPointLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_EXPPOINTLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_EXPPOINTLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwGrade = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwKillerKind = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwKillerIdx = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwExpPoint = atoi((char*)pData[i].Data[11]);
		Msg.sLog[i].dwAbilPoint = atoi((char*)pData[i].Data[12]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[14]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_EXPPOINTLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ExpPointLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_EXPPOINTLOG) );
}

void MugongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MugongLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMugongLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMugongLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MUGONGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MUGONGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwSkillIdx = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwSkillDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwSkillLevel = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSkillExp = atoi((char*)pData[i].Data[10]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MUGONGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MugongLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MUGONGLOG) );

}

void MugongExpLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MugongExpLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMugongExpLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMugongExpLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MUGONGEXPLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MUGONGEXPLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwSkillIdx = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwSkillDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwSkillLevel = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSkillExp = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwSkillPos = atoi((char*)pData[i].Data[11]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MUGONGEXPLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MugongExpLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MUGONGEXPLOG) );
}

void MoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MoneyLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMoneyLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MONEYLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MONEYLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);					
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwInvenMoney = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwTargetMoney = atoi((char*)pData[i].Data[10]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MONEYLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MoneyLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MONEYLOG) );
}

void MoneyWrongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MoneyWrongLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMoneyWrongLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMoneyWrongLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MONEYWRONGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MONEYWRONGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwTotalMoney = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwPyogukMoney = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[10]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MONEYWRONGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MoneyWrongLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MONEYWRONGLOG) );
}

void AbilityLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_AbilityLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eAbilityLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RAbilityLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_ABILITYLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_ABILITYLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwAbilityIdx = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwAbilityLevelTo = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwAbilityExpFrom = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwAbilityExpTo = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_ABILITYLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_ABILITYLOG) );
}

void ItemOptionLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC dbo.TP_ItemOptionLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eItemOptionLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void ItemOptionLog2( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwDBIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC dbo.TP_ItemOptionLog2 %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwDBIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eItemOptionLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RItemOptionLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_ITEMOPTIONLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_ITEMOPTIONLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwOptionDBIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].wGengol = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].wMinchub = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].wCheryuk = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].wSimmek = atoi((char*)pData[i].Data[11]);
		Msg.sLog[i].wLife = atoi((char*)pData[i].Data[12]);
		Msg.sLog[i].wNeryuk = atoi((char*)pData[i].Data[13]);
		Msg.sLog[i].wShield = atoi((char*)pData[i].Data[14]);
		Msg.sLog[i].wFireRegis = atoi((char*)pData[i].Data[15]);
		Msg.sLog[i].wWaterRegist = atoi((char*)pData[i].Data[16]);
		Msg.sLog[i].wTreeRegist = atoi((char*)pData[i].Data[17]);
		Msg.sLog[i].wGoldRegist = atoi((char*)pData[i].Data[18]);
		Msg.sLog[i].wEarthRegist = atoi((char*)pData[i].Data[19]);
		Msg.sLog[i].wPhyAttack = atoi((char*)pData[i].Data[20]);
		Msg.sLog[i].wCritical = atoi((char*)pData[i].Data[21]);
		Msg.sLog[i].wFireAttack = atoi((char*)pData[i].Data[22]);
		Msg.sLog[i].wWaterAttack = atoi((char*)pData[i].Data[23]);
		Msg.sLog[i].wTreeAttack = atoi((char*)pData[i].Data[24]);
		Msg.sLog[i].wGoldAttack = atoi((char*)pData[i].Data[25]);
		Msg.sLog[i].wEarthAttack = atoi((char*)pData[i].Data[26]);		
		Msg.sLog[i].wPhyDefense = atoi((char*)pData[i].Data[27]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[28] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[29]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_ITEMOPTIONLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ItemOptionLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_ITEMOPTIONLOG) );
}

void CheatLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_CheatLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eCheatLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RCheatLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_CHEATLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_CHEATLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwParam = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_CHEATLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_CHEATLOG) );
}

void HackingLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_HackingLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eHackingLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RHackingLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_HACKINGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_HACKINGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwCheatKind = atoi((char*)pData[i].Data[6]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[7] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[8]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_HACKINGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_HACKINGLOG) );
}

void ToolLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwLogType, char* sName, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ToolLog %d, %d, \'%s\', %d, \'%s\', \'%s\'", dwStartIdx, dwLogType, sName, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eToolLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RToolLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_TOOLLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_TOOLLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	int check = atoi((char*)pData[0].Data[0]);
	if( check == -1 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_TOOLLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.wCount = (WORD)pMessage->dwResult;
	Msg.dwLogType = atoi((char*)pData[0].Data[0]);
	strcpy( Msg.sName, (char*)pData[0].Data[1]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[2]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[3] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[4] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwLogKind = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwOperIdx = atoi((char*)pData[i].Data[8]);
		strncpy( Msg.sLog[i].sOperName, (char*)pData[i].Data[9], 17 );
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[10]);
		strncpy( Msg.sLog[i].sTargetName, (char*)pData[i].Data[11], 17 );
		Msg.sLog[i].dwParam1 = atoi((char*)pData[i].Data[12]);
		Msg.sLog[i].dwParam2 = atoi((char*)pData[i].Data[13]);
		Msg.sLog[i].dwParam3 = atoi((char*)pData[i].Data[14]);
		Msg.sLog[i].dwParam4 = atoi((char*)pData[i].Data[15]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[16] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[17]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}	

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ToolLog( connectIdx, tempIdx, startIdx, Msg.dwLogType, Msg.sName, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_TOOLLOG) );
}

void PetLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_PetLogFromType %d, %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx, dwItem, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, ePetLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RPetLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_LOGPET Msg;
	memset( &Msg, 0, sizeof(TMSG_LOGPET) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = 0;
	Msg.wType = atoi((char*)pData[0].Data[ePL_Type]);
	Msg.dwCharIdx = atoi((char*)pData[0].Data[ePL_ChrName]);
	Msg.dwItem = atoi((char*)pData[0].Data[ePL_Item]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[ePL_DayOnly]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[ePL_SDate] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[ePL_EDate] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwIdx = atoi((char*)pData[i].Data[ePL_Idx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[ePL_LogType]);
		Msg.sLog[i].dwPetidx = atoi((char*)pData[i].Data[ePL_PetIdx]);		
		Msg.sLog[i].dwPetSummonItemDBIdx = atoi((char*)pData[i].Data[ePL_PetSummonItemDBIdx]);
		Msg.sLog[i].dwUserIdx = atoi((char*)pData[i].Data[ePL_UserIdx]);		
		Msg.sLog[i].dwCharacterIdx = atoi((char*)pData[i].Data[ePL_CharacterIdx]);
		Msg.sLog[i].wGrade = atoi((char*)pData[i].Data[ePL_Grade]);
		Msg.sLog[i].wFriendShip = atoi((char*)pData[i].Data[ePL_FriendShip]);
		Msg.sLog[i].wStamina = atoi((char*)pData[i].Data[ePL_Stamina]);
		Msg.sLog[i].wAlive = atoi((char*)pData[i].Data[ePL_Alive]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[ePL_LogDate] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[ePL_LogTime]);

		startIdx = Msg.sLog[i].dwIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_PETLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		PetLogFromType( connectIdx, tempIdx, startIdx, Msg.wType, Msg.dwCharIdx, Msg.dwItem, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}


/* --------------------------------------------------------------------------
* 함수이름 : GuildTNLogFromType
* 목    적 : 문파토너먼트 정보 쿼리
* 주의사항 :
*---------------------------------------------------------------------------*/
void GuildTNLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_GuildTNLogFromType %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx,  nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eGuildTNLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : RGuildTNLogFromType
* 목    적 : 문파토너먼트 정보 쿼리 결과전송
* 주의사항 :
*---------------------------------------------------------------------------*/
void RGuildTNLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	MSG_LOGGUILDTN Msg;
	memset( &Msg, 0, sizeof(MSG_LOGGUILDTN) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.count = 0;
	Msg.type = atoi((char*)pData[0].Data[0]);
	Msg.guildidx = atoi((char*)pData[0].Data[1]);
	Msg.dayonly = atoi((char*)pData[0].Data[2]);
	strcpy( Msg.startdate, (char*)pData[0].Data[3] );
	strcpy( Msg.enddate, (char*)pData[0].Data[4] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.logdata[i].idx = atoi((char*)pData[i].Data[5]);
		Msg.logdata[i].tournamentidx = atoi((char*)pData[i].Data[6]);
		Msg.logdata[i].guildidx = atoi((char*)pData[i].Data[7]);		
		Msg.logdata[i].logkind = atoi((char*)pData[i].Data[8]);
		Msg.logdata[i].ranking = atoi((char*)pData[i].Data[9]);		
		strcpy( Msg.logdata[i].registtime, (char*)pData[i].Data[10] );
		strcat( Msg.logdata[i].registtime, " ");
		strcat( Msg.logdata[i].registtime, (char*)pData[i].Data[11]);

		startIdx = Msg.logdata[i].idx;
		++Msg.count;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_GUILDTNLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.end = 0;
		GuildTNLogFromType( connectIdx, tempIdx, startIdx, Msg.type, Msg.guildidx, Msg.dayonly, Msg.startdate, Msg.enddate );
	}
	else
	{
		Msg.end = 1;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}


/* --------------------------------------------------------------------------
* 함수이름 : SiegeWarLogFromType
* 목    적 : 공성전 정보 쿼리
* 주의사항 :
*---------------------------------------------------------------------------*/
void SiegeWarLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_SiegeWarLogFromType %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx,  nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eSiegeWarLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : RSiegeWarLogFromType
* 목    적 : 공성전 정보 쿼리 결과전송
* 주의사항 :
*---------------------------------------------------------------------------*/
void RSiegeWarLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	MSG_LOGSIEGEWAR Msg;
	memset( &Msg, 0, sizeof(MSG_LOGSIEGEWAR) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.count = 0;
	Msg.type = atoi((char*)pData[0].Data[0]);
	Msg.guildidx = atoi((char*)pData[0].Data[1]);
	Msg.dayonly = atoi((char*)pData[0].Data[2]);
	strcpy( Msg.startdate, (char*)pData[0].Data[3] );
	strcpy( Msg.enddate, (char*)pData[0].Data[4] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.logdata[i].idx = atoi((char*)pData[i].Data[5]);
		Msg.logdata[i].siegewaridx = atoi((char*)pData[i].Data[6]);
		Msg.logdata[i].mapnum = atoi((char*)pData[i].Data[7]);
		Msg.logdata[i].guildidx = atoi((char*)pData[i].Data[8]);		
		Msg.logdata[i].logkind = atoi((char*)pData[i].Data[9]);
		Msg.logdata[i].engraveidx = atoi((char*)pData[i].Data[10]);		
		strcpy( Msg.logdata[i].registtime, (char*)pData[i].Data[11] );
		strcat( Msg.logdata[i].registtime, " ");
		strcat( Msg.logdata[i].registtime, (char*)pData[i].Data[12]);

		startIdx = Msg.logdata[i].idx;
		++Msg.count;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_SIEGEWARLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.end = 0;
		SiegeWarLogFromType( connectIdx, tempIdx, startIdx, Msg.type, Msg.guildidx, Msg.dayonly, Msg.startdate, Msg.enddate );
	}
	else
	{
		Msg.end = 1;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}

/// 060915 PKH GMTOOL
/* --------------------------------------------------------------------------
* 함수이름 : GMToolLogFromType
* 목    적 : GMTOOL 로그 정보 쿼리
* 주의사항 :
*---------------------------------------------------------------------------*/
void GMToolLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_GMToolLogFromType %d, %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx,  dwItem, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eGMToolLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

/// 060915 PKH GMTOOL
/* --------------------------------------------------------------------------
* 함수이름 : RGMToolLogFromType
* 목    적 : GMTOOL 로그쿼리 결과전송
* 주의사항 :
*---------------------------------------------------------------------------*/
void RGMToolLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	MSG_LOGGMTOOL Msg;
	memset( &Msg, 0, sizeof(MSG_LOGGMTOOL) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.count = 0;
	Msg.type = atoi((char*)pData[0].Data[0]);
	Msg.character_idx = atoi((char*)pData[0].Data[1]);
	Msg.gmlogtype = atoi((char*)pData[0].Data[2]);
	Msg.dayonly = atoi((char*)pData[0].Data[3]);
	strcpy( Msg.startdate, (char*)pData[0].Data[4] );
	strcpy( Msg.enddate, (char*)pData[0].Data[5] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.logdata[i].dbidx = atoi((char*)pData[i].Data[6]);
		Msg.logdata[i].character_idx = atoi((char*)pData[i].Data[7]);
		Msg.logdata[i].gmlogtype = atoi((char*)pData[i].Data[8]);
		Msg.logdata[i].logkind = atoi((char*)pData[i].Data[9]);		
		Msg.logdata[i].param1 = atoi((char*)pData[i].Data[10]);
		Msg.logdata[i].param2 = atoi((char*)pData[i].Data[11]);		
		strcpy( Msg.logdata[i].registtime, (char*)pData[i].Data[12] );
		strcat( Msg.logdata[i].registtime, " ");
		strcat( Msg.logdata[i].registtime, (char*)pData[i].Data[13]);

		startIdx = Msg.logdata[i].dbidx;
		++Msg.count;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_GMTOOLLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.end = 0;
		GMToolLogFromType( connectIdx, tempIdx, startIdx, Msg.type, Msg.character_idx, Msg.gmlogtype, Msg.dayonly, Msg.startdate, Msg.enddate );
	}
	else
	{
		Msg.end = 1;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}

// user info
void SearchMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID )
{
#ifdef _JAPAN_LOCAL_
	sprintf( txt, "EXEC RP_SearchMemberNumFromID_JP \'%s\'", sID );
	if( g_DB.LoginQuery(eQueryType_FreeQuery, eSearchMemberNumFromID, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
#else
	sprintf( txt, "EXEC RP_SearchMemberNumFromID \'%s\'", sID );
	if( g_DB.LoginQuery(eQueryType_FreeQuery, eSearchMemberNumFromID, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
#endif
}

void RSearchMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// No Data
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchMemberNumFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchMemberNumFromCharName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchMemberNumFromCharName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchMemberNumFromCharName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchCharIdxFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchCharIdxFromCharName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchCharIdxFromCharName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchCharIdxFromCharName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);	// charidx

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : SearchGuildIdxFromGuildName
* 목    적 : 문파토너먼트 문파인덱스 조회쿼리 
* 주의사항 :
*---------------------------------------------------------------------------*/
void SearchGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchGuildIdxFromGuildName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchGuildIdxFromGuildName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : RSearchGuildIdxFromGuildName
* 목    적 : 문파토너먼트 문파인덱스 조회쿼리 결과전송
* 주의사항 :
*---------------------------------------------------------------------------*/
void RSearchGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_QUERY_GUILDTNIDX_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_GUILDTNIDX_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);	// charidx

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

/* --------------------------------------------------------------------------
* 함수이름 : SearchSWGuildIdxFromGuildName
* 목    적 : 공성전 문파인덱스 조회쿼리 
* 주의사항 :
*---------------------------------------------------------------------------*/
void SearchSWGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchGuildIdxFromGuildName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchSWGuildIdxFromGuildName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : RSearchSWGuildIdxFromGuildName
* 목    적 : 공성전 문파인덱스 조회쿼리 결과전송
* 주의사항 :
*---------------------------------------------------------------------------*/
void RSearchSWGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_SIEGEWARLOGFROMTYPE_ACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_SIEGEWARIDX_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);	// charidx

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchUserInfoFromMemberNum( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum, WORD wServer )
{
#ifdef _JAPAN_LOCAL_
	sprintf( txt, "EXEC RP_SearchUserInfoByMemberNo_JP %d, %d", wServer, dwMemberNum );
	if( g_DB.LoginMiddleQuery( RSearchUserInfoFromMemberNum, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
#else
	sprintf( txt, "EXEC RP_SearchUserInfoByMemberNo %d, %d", wServer, dwMemberNum );
	if( g_DB.LoginMiddleQuery( RSearchUserInfoFromMemberNum, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
#endif
}

void RSearchUserInfoFromMemberNum( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_USERSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_USERSEARCHRESULT Info;

		Info.sUser.bLoginFlag = atoi((char*)pData[0].Data[eUI_IsLogin]);
		Info.sUser.dwMemberNo = atoi((char*)pData[0].Data[eUI_User_Idx]);
		strcpy(Info.sUser.strID, (char*)pData[0].Data[eUI_Id]);
		strcpy(Info.sUser.strPass, (char*)pData[0].Data[eUI_Pwd]);
		strcpy(Info.sUser.strName, (char*)pData[0].Data[eUI_Name]);
		strcpy(Info.sUser.strIdCard, (char*)pData[0].Data[eUI_Idcard]);
		strcpy(Info.sUser.strPhon, (char*)pData[0].Data[eUI_Phone]);
		strcpy(Info.sUser.strEmail, (char*)pData[0].Data[eUI_Email]);
		strcpy(Info.sUser.strRegDateTime, (char*)pData[0].Data[eUI_Regdate]);
		strcpy(Info.sUser.strLastLoginTime, (char*)pData[0].Data[eUI_LastLogintime]);
		strcpy(Info.sUser.strLastLogoutTime, (char*)pData[0].Data[eUI_LastLogouttime]);

		Info.sUser.nTotalTime = atoi((char*)pData[0].Data[eUI_Totaltime]);
		int temp = Info.sUser.nTotalTime;
		int oo, mm, ss;
		ss = temp%60;
		mm = (temp/60)%60;
		oo = (temp/3600);
		sprintf(Info.sUser.strTotalTime, "%d:%d:%d", oo, mm, ss );

		Info.sUser.wUserLevel = atoi((char*)pData[0].Data[eUI_UserLevel]);
		Info.wServer = atoi((char*)pData[0].Data[eUI_Char_Server]);

		Info.Category = MP_RMTOOL_USER;
		Info.Protocol = MP_RMTOOL_USERSEARCH_ACK;
		Info.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}

void SimpleCharacterInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum )
{
	sprintf(txt, "EXEC TP_SimpleCharacterInfoFromMN %d", dwMemberNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eSimpleCharacterInfoFromMN, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSimpleCharacterInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_SIMPLERESULT Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = (BYTE)count;

		for( DWORD i = 0; i < count; ++i )
		{
			Msg.sSimple[i].dwUserIdx = atoi((char*)pData[i].Data[0]);
			Msg.sSimple[i].dwCharNo = atoi((char*)pData[i].Data[1]);
			strcpy( Msg.sSimple[i].strName, (char*)pData[i].Data[2]);
			strcpy( Msg.sSimple[i].strCreateDate, (char*)pData[i].Data[3]);
			strcat( Msg.sSimple[i].strCreateDate, " ");
			strcat( Msg.sSimple[i].strCreateDate, (char*)pData[i].Data[4]);
		}

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgSize() );
	}
}

void CharacterCreateInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum )
{
	sprintf(txt, "EXEC TP_CharacterCreateInfoFromMN %d", dwMemberNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterCreateInfoFromMN, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterCreateInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_CREATEINFO_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_CHARACTERCREATEINFO Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_CREATEINFO_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = (BYTE)count;

		for( BYTE i = 0; i < count; ++i )
		{
			Msg.sInfo[i].dwCharIdx = atoi((char*)pData[i].Data[0]);
			Msg.sInfo[i].dwUserIdx = atoi((char*)pData[i].Data[1]);
			strcpy( Msg.sInfo[i].sName, (char*)pData[i].Data[2] );
			Msg.sInfo[i].dwFlag = atoi((char*)pData[i].Data[3]);
			strcpy( Msg.sInfo[i].sCreateDate, (char*)pData[i].Data[4] );
			strcat( Msg.sInfo[i].sCreateDate, " ");
			strcat( Msg.sInfo[i].sCreateDate, (char*)pData[i].Data[5]);
			strcpy( Msg.sInfo[i].sCreateIP, (char*)pData[i].Data[6] );
			strcpy( Msg.sInfo[i].sDeleteDate, (char*)pData[i].Data[7] );
			strcat( Msg.sInfo[i].sDeleteDate, " ");
			strcat( Msg.sInfo[i].sDeleteDate, (char*)pData[i].Data[8]);
			strcpy( Msg.sInfo[i].sDeleteIP, (char*)pData[i].Data[9] );
			strcpy( Msg.sInfo[i].sRecoverDate, (char*)pData[i].Data[10] );
			strcat( Msg.sInfo[i].sRecoverDate, " ");
			strcat( Msg.sInfo[i].sRecoverDate, (char*)pData[i].Data[11]);
			strcpy( Msg.sInfo[i].sRecoverIP, (char*)pData[i].Data[12] );
		}

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgSize() );
	}	
}

void CheckCharacterName( WORD wConnectIdx, WORD wTempIdx, char* sName )
{
	sprintf(txt, "EXEC TP_CheckCharacterName \'%s\'", sName);	
	if(g_DB.Query(eQueryType_FreeQuery, eCheckCharacterName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCheckCharacterName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD result = atoi((char*)pData->Data[0]);
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.dwTemplateIdx = tempIdx;
	
	switch( result )
	{
	case 0:
		Msg.Protocol = MP_RMTOOL_CHECKNAME_ACK;
		break;

	case 1:
	case 2:
	case 3:
		Msg.Protocol = MP_RMTOOL_CHECKNAME_NACK;
		break;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );	
}

void CharacterRecover( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, char* sName )
{
	char ip[32];
	WORD port;
	MNETWORK->GetUserAddress( wConnectIdx, ip, &port );

	sprintf(txt, "EXEC TP_CharacterRecover %d, \'%s\', \'%s\'", dwChrIdx, sName, ip);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterRecover, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterRecover( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD result = atoi((char*)pData->Data[0]);
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.dwTemplateIdx = tempIdx;
	
	switch( result )
	{
	case 0:
		Msg.Protocol = MP_RMTOOL_CHARACTER_RECOVER_ACK;
		Msg.dwData = 0;
		break;

	case 1:	// ²Ë Âü.
	case 2:	// ÀÌ¸§ Áßº¹		
	case 3:	
	case 4:	// Name is Null
	case 5:
		Msg.Protocol = MP_RMTOOL_CHARACTER_RECOVER_NACK;
		Msg.dwData = result;
		break;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}


// quest
void CharacterMainQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx )
{
	sprintf( txt, "EXEC RP_CharacterMainQuestInfo %d", dwCharIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterMainQuestInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}	
}

void RCharacterMainQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_MAINQUESTINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MAINQUESTINFO_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.MainQuestInfo[i].dwCharIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.MainQuestInfo[i].dwQuestIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.MainQuestInfo[i].dwSubQuestDo = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.MainQuestInfo[i].dwEndParam = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.MainQuestInfo[i].dwRegDate = (DWORD)atoi((char*)pData[i].Data[4]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void MainQuestDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx )
{
	sprintf( txt, "EXEC MP_MAINQUEST_Delete %d, %d", dwCharIdx, dwQuestIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eMainQuestDelete, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterSubQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx )
{
	sprintf( txt, "EXEC RP_CharacterSubQuestInfo %d, %d", dwCharIdx, dwQuestIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterSubQuestInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}	
}

void RCharacterSubQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_SUBQUESTINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_SUBQUESTINFO_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.SubQuestInfo[i].dwCharIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.SubQuestInfo[i].dwQuestIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.SubQuestInfo[i].dwSubQuestIdx = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.SubQuestInfo[i].dwData = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.SubQuestInfo[i].dwRegDate = (DWORD)atoi((char*)pData[i].Data[4]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void InsertItemMoneyLog( DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
						 DWORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
						 DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos, DURTYPE ItemDur, DWORD ExpPoint )
{
	sprintf(txt, "EXEC  %s %d, \'%s\', %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", "up_ItemMoneyLog", 
		FromChrIdx, FromChrName, ToChrIdx, ToChrName, 
		LogType, FromTotalMoney, ToTotalMoney, ChangeMoney,
		ItemIdx, ItemDBIdx, ItemFromPos, ItemToPos,
		ItemDur, ExpPoint);
	if(g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt) == FALSE)
	{
	}
}

// userlevel
void ChangeUserLevel( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwUserLevel )
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC TP_ChangeUserLevel_JP %d, %d", dwUserIdx, dwUserLevel);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eChangeUserLevel, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
#else
	sprintf(txt, "EXEC TP_ChangeUserLevel %d, %d", dwUserIdx, dwUserLevel);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eChangeUserLevel, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
#endif
}

void RChangeUserLevel( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD2 Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_CHANGE_USERLEVEL_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwData1 = (DWORD)atoi((char*)pData[0].Data[0]);		// useridx
	Msg.dwData2 = (DWORD)atoi((char*)pData[0].Data[1]);		// userlevel

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_DWORD2) );
}

void ChangeLoginPoint( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharacterIdx, DWORD dwMapNum )
{
	sprintf(txt, "EXEC TP_ChangeLoginPoint %d, %d", dwCharacterIdx, dwMapNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeLoginPoint, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RChangeLoginPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD2 Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwData1 = (DWORD)atoi((char*)pData[0].Data[0]);		// characteridx
	Msg.dwData2 = (DWORD)atoi((char*)pData[0].Data[1]);		// mapnum

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_DWORD2) );
}

void CharacterInvenInfo2(WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx)
{
	sprintf( txt, "EXEC RP_CharacterInvenInfo %d, %d", dwStartIdx, dwCharIdx );	
	if( g_DB.Query(eQueryType_FreeQuery, eCharacterInvenInfo2, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RCharacterInvenInfo2(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ITEMBASE Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_INVENINFO;
	Msg.dwTemplateIdx = tempIdx;

	DWORD dwCharIdx = atoi((char*)pData[0].Data[eCI_ObjectID]);
	DWORD dwStartIdx = 0;
	for( DWORD i = 0; i < count; i++ )
	{
		dwStartIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
	
		ITEMBASE Item;
		Item.dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
		Item.wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
		Item.Position = atoi((char*)pData[i].Data[eCI_Position]);
		Item.Durability = atoi((char*)pData[i].Data[eCI_Durability]);
		Item.QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
		Item.ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
		Item.RareIdx = atoi((char*)pData[i].Data[eCI_Param+1]);

		Msg.Add( &Item );
	}

	if( count >= 100 )
	{
		CharacterInvenInfo2( connectIdx, tempIdx, dwStartIdx, dwCharIdx );
	}
	
	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );
}

void ChangeItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeItemPosition %d, %d, %d", dwChrIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeItemPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void ChangeWareItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeWareItemPosition %d, %d, %d", dwUserIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeItemPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void ChangeMugongPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeMugongPosition %d, %d, %d", dwChrIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeMugongPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	sprintf(txt, "EXEC Up_ToolLog %d, %d, %d, \'%s\', %d, \'%s\', %d, %d, %d, %d", dwLogType, dwLogKind, dwOperIdx, sOperName,
		dwTargetIdx, sTargetName, dwParam1, dwParam2, dwParam3, dwParam4 );	
	if(g_DB.LogQuery(eQueryType_FreeQuery, eInsertLogTool, 0, txt) == FALSE)
	{
	}
}

void CharacterShopWareItemInfo( WORD wConnectionIdx, WORD wTempIdx, DWORD dwUserIdx )
{
	sprintf( txt, "EXEC dbo.TP_CharacterShopWareItemInfo %d", dwUserIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterShopWareItemInfo, MAKEDWORD(wConnectionIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterShopWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwCount = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ITEMBASE Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_SHOPWARE_ITEMINFO;
	Msg.dwTemplateIdx = tempIdx;

	for( DWORD i = 0; i < dwCount; ++i )
	{
		ITEMBASE Item;
		memset( &Item, 0, sizeof(ITEMBASE) );
		Item.dwDBIdx = atoi((char*)pData[i].Data[0]);
		Item.wIconIdx = atoi((char*)pData[i].Data[1]);
		Item.Position = atoi((char*)pData[i].Data[2]);
		Item.Durability = atoi((char*)pData[i].Data[3]);
		Item.ItemParam = atoi((char*)pData[i].Data[4]);

		Msg.Add( &Item );
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );
}

void ShopWareItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo )
{
	sprintf(txt, "EXEC dbo.TP_ShopWareItemInsert %d, %d, %d, %d",
		dwUserIdx, pItem->wIconIdx, pItem->Position, pItem->Durability );
	if(g_DB.Query(eQueryType_FreeQuery, eShopWareItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RShopWareItemInsert( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[0]);
		DWORD dwItemDBIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		WORD wItemPos = (WORD)atoi((char*)pData[0].Data[3]);
		WORD wItemQPos = (WORD)atoi((char*)pData[0].Data[4]);
		DWORD dwItemDu = (DWORD)atoi((char*)pData[0].Data[5]);
		DWORD dwMunpaIdx = (DWORD)atoi((char*)pData[0].Data[6]);
		DWORD dwPyogukIdx = (DWORD)atoi((char*)pData[0].Data[7]);
		DWORD dwShopIdx = (DWORD)atoi((char*)pData[0].Data[8]);
		DWORD dwItemParam = (DWORD)atoi((char*)pData[0].Data[9]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwPyogukIdx, "",
					   dwItemDBIdx, dwItemIdx, wItemPos, dwItemDu );
	}
}

void SearchGuild( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchGuild %d", dwStartIdx );
	if(g_DB.Query(eQueryType_FreeQuery, eSearchGuild, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchGuild( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_GUILD_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
		return;
	}

	TMSG_GUILD Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILD_ACK;
	Msg.dwTemplateIdx = tempIdx;

	DWORD dwStartIdx = 0;
	for( DWORD i = 0; i < count; ++i )
	{
		GUILDBASEINFO Info;
		Info.dwGuildIdx = atoi((char*)pData[i].Data[0]);
		strcpy( Info.sGuildName, (char*)pData[i].Data[1] );
		Info.dwMasterIdx = atoi((char*)pData[i].Data[2]);
		strcpy( Info.sMasterName, (char*)pData[i].Data[3] );
		Info.dwGuildLevel = atoi((char*)pData[i].Data[4]);
		Info.dwGuildMap = atoi((char*)pData[i].Data[5]);
		Info.dwUnionIdx = atoi((char*)pData[i].Data[6]);
		Info.dwGuildMoney = atoi((char*)pData[i].Data[7]);
		strcpy( Info.sCreateDate, (char*)pData[i].Data[8] );
		strcat( Info.sCreateDate, " " );
		strcat( Info.sCreateDate, (char*)pData[i].Data[9]);

		Msg.Add( &Info );

		dwStartIdx = Info.dwGuildIdx;		
	}

	if( count >= 100 )
		SearchGuild( connectIdx, tempIdx, dwStartIdx );
	
	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );
}

void SearchGuildInfo( WORD wConnectIdx, WORD wTempIdx, char* pGuildName )
{
	sprintf(txt, "EXEC dbo.TP_SearchGuildInfo %s", pGuildName );
	if(g_DB.Query(eQueryType_FreeQuery, eSearchGuildInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchGuildInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	int nFlag = atoi((char*)pData[0].Data[0]);

	if( nFlag == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_MUNPA;	
		Msg.Protocol = MP_RMTOOL_GUILDIFNO_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
	}
	else
	{
		TMSG_GUILD Msg;
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_GUILDBASEIFNO;
		Msg.dwTemplateIdx = tempIdx;

		GUILDBASEINFO Info;
		Info.dwGuildIdx = atoi((char*)pData[0].Data[0]);
		strcpy( Info.sGuildName, (char*)pData[0].Data[1] );
		Info.dwMasterIdx = atoi((char*)pData[0].Data[2]);
		strcpy( Info.sMasterName, (char*)pData[0].Data[3] );
		Info.dwGuildLevel = atoi((char*)pData[0].Data[4]);
		Info.dwGuildMap = atoi((char*)pData[0].Data[5]);
		Info.dwUnionIdx = atoi((char*)pData[0].Data[6]);
		Info.dwGuildMoney = atoi((char*)pData[0].Data[7]);
		strcpy( Info.sCreateDate, (char*)pData[0].Data[8] );
		strcat( Info.sCreateDate, " " );
		strcat( Info.sCreateDate, (char*)pData[0].Data[9]);

		Msg.Add( &Info );

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );

		SearchGuildMemberInfo(connectIdx, tempIdx, 0, Info.dwGuildIdx);
		SearchGuildWareItemInfo(connectIdx, tempIdx, 0, Info.dwGuildIdx);
	}
}

void SearchGuildMemberInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchGuildMemberInfo %d, %d", dwStartIdx, dwGuildIdx );
	if(g_DB.Query(eQueryType_FreeQuery, eSearchGuildMemberInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchGuildMemberInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GUILDMEMBERINFO Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILDMEMBERIFNO;
	Msg.dwTemplateIdx = tempIdx;

	DWORD dwStartIdx = 0;
	DWORD dwGuildIdx = 0;
	for( DWORD i = 0; i < count; ++i )
	{
		if( atoi((char*)pData[i].Data[3]) < 5 )
		{
			TGUILDMEMBERINFO Info;
			Info.dwChrIdx = atoi((char*)pData[i].Data[0]);
			strcpy( Info.sName, (char*)pData[i].Data[1] );
			Info.dwLevel = atoi((char*)pData[i].Data[2]);
			dwGuildIdx = atoi((char*)pData[i].Data[4]);
			Info.dwPosition = atoi((char*)pData[i].Data[5]);

			Msg.Add( &Info );

			dwStartIdx = Info.dwChrIdx;		
		}
	}

	if( count >= 100 )
		SearchGuildMemberInfo( connectIdx, tempIdx, dwStartIdx, dwGuildIdx );

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );
}

void SearchGuildWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchGuildWareItemInfo %d, %d", dwStartIdx, dwGuildIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchGuildWareItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchGuildWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ITEMBASE Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILDITEMIFNO;
	Msg.dwTemplateIdx = tempIdx;

	DWORD dwGuildIdx = 0;
	DWORD dwStartIdx = 0;

	for( DWORD i = 0; i < count; ++i )
	{
		ITEMBASE Item;
		Item.ItemParam = atoi((char*)pData[i].Data[0]);		// guildidx
		Item.dwDBIdx = atoi((char*)pData[i].Data[1]);
		Item.wIconIdx = atoi((char*)pData[i].Data[2]);
		Item.Position = atoi((char*)pData[i].Data[3]);
		Item.Durability = atoi((char*)pData[i].Data[4]);

		Msg.Add( &Item );

		dwGuildIdx = Item.ItemParam;
		dwStartIdx = Item.dwDBIdx;
	}

	if( count >= 100)
		SearchGuildWareItemInfo( connectIdx, tempIdx, dwStartIdx, dwGuildIdx );
	else		
		SearchGuildWareItemOptionInfo( connectIdx, tempIdx, 0, dwGuildIdx );

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void SearchGuildWareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchGuildWareItemOptionInfo %d, %d", dwStartIdx, dwGuildIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchGuildWareItemOptionInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchGuildWareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMOPTIONINFO2 Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILDITEMOPTIONIFNO;
	Msg.dwTemplateIdx = tempIdx;

	DWORD dwGuildIdx = 0;
	DWORD dwStartIdx = 0;
	for( DWORD i = 0; i < count; ++i )
	{
		ITEM_OPTION_INFO Info;
		Info.dwOptionIdx = (DWORD)atoi((char*)pData[i].Data[2]);
		Info.dwItemDBIdx = (DWORD)atoi((char*)pData[i].Data[3]);
		Info.GenGol	= (WORD)atoi((char*)pData[i].Data[4]);
		Info.MinChub = (WORD)atoi((char*)pData[i].Data[5]);				
		Info.CheRyuk = (WORD)atoi((char*)pData[i].Data[6]);				
		Info.SimMek	= (WORD)atoi((char*)pData[i].Data[7]);					
		Info.Life = (DWORD)atoi((char*)pData[i].Data[8]);					
		Info.NaeRyuk = (WORD)atoi((char*)pData[i].Data[9]);				
		Info.Shield = (DWORD)atoi((char*)pData[i].Data[10]);					
		Info.AttrRegist.SetElement_Val( ATTR_FIRE, (float)atof((char*)pData[i].Data[11]) );
		Info.AttrRegist.SetElement_Val( ATTR_WATER,	(float)atof((char*)pData[i].Data[12]) );
		Info.AttrRegist.SetElement_Val( ATTR_TREE, (float)atof((char*)pData[i].Data[13]) );
		Info.AttrRegist.SetElement_Val( ATTR_IRON, (float)atof((char*)pData[i].Data[14]) );
		Info.AttrRegist.SetElement_Val( ATTR_EARTH,	(float)atof((char*)pData[i].Data[15]) );
		Info.PhyAttack = (WORD)atoi((char*)pData[i].Data[16]);
		Info.CriticalPercent = (WORD)atoi((char*)pData[i].Data[17]);
		Info.AttrAttack.SetElement_Val( ATTR_FIRE, (float)atof((char*)pData[i].Data[18]) );
		Info.AttrAttack.SetElement_Val( ATTR_WATER,	(float)atof((char*)pData[i].Data[19]) );
		Info.AttrAttack.SetElement_Val( ATTR_TREE, (float)atof((char*)pData[i].Data[20]) );
		Info.AttrAttack.SetElement_Val( ATTR_IRON, (float)atof((char*)pData[i].Data[21]) );
		Info.AttrAttack.SetElement_Val( ATTR_EARTH,	(float)atof((char*)pData[i].Data[22]) );
		Info.PhyDefense = (WORD)atoi((char*)pData[i].Data[23]);		

		Msg.Add( &Info );

		dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		dwStartIdx = (DWORD)atoi((char*)pData[i].Data[1]);
	}

	if( count >= 100)
		SearchGuildWareItemOptionInfo( connectIdx, tempIdx, dwStartIdx, dwGuildIdx );

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void CharacterRareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx )
{
	sprintf( txt, "EXEC TP_CharacterRareItemOptionInfo %d, %d, %d", dwUserIdx, dwCharIdx, StartIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterRareItemOptionInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterRareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_RAREITEMOPTIONINFO	Msg;
	memset( &Msg, 0, sizeof(Msg) );

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTERINFO_RAREITEMOPTIONINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( count == 0 )
	{
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
		return;
	}

	DWORD UserIdx = atoi((char*)pData[0].Data[0]);
	DWORD CharIdx = atoi((char*)pData[0].Data[1]);
	DWORD StartIdx = 0;

	for( DWORD i = 0 ; i < count ; ++i )
	{
		Msg.RareOptionInfo[i].dwRareOptionIdx = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.RareOptionInfo[i].dwItemDBIdx = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.RareOptionInfo[i].GenGol = (WORD)atoi((char*)pData[i].Data[4]);
		Msg.RareOptionInfo[i].MinChub = (WORD)atoi((char*)pData[i].Data[5]);				
		Msg.RareOptionInfo[i].CheRyuk = (WORD)atoi((char*)pData[i].Data[6]);				
		Msg.RareOptionInfo[i].SimMek = (WORD)atoi((char*)pData[i].Data[7]);					
		Msg.RareOptionInfo[i].Life = (DWORD)atoi((char*)pData[i].Data[8]);					
		Msg.RareOptionInfo[i].NaeRyuk = (WORD)atoi((char*)pData[i].Data[9]);				
		Msg.RareOptionInfo[i].Shield = (DWORD)atoi((char*)pData[i].Data[10]);					
		Msg.RareOptionInfo[i].AttrRegist.SetElement_Val( ATTR_FIRE, (float)atof((char*)pData[i].Data[11]) );
		Msg.RareOptionInfo[i].AttrRegist.SetElement_Val( ATTR_WATER, (float)atof((char*)pData[i].Data[12]) );
		Msg.RareOptionInfo[i].AttrRegist.SetElement_Val( ATTR_TREE, (float)atof((char*)pData[i].Data[13]) );
		Msg.RareOptionInfo[i].AttrRegist.SetElement_Val( ATTR_IRON, (float)atof((char*)pData[i].Data[14]) );
		Msg.RareOptionInfo[i].AttrRegist.SetElement_Val( ATTR_EARTH, (float)atof((char*)pData[i].Data[15]) );
		Msg.RareOptionInfo[i].PhyAttack	= (WORD)atoi((char*)pData[i].Data[16]);
		Msg.RareOptionInfo[i].AttrAttack.SetElement_Val( ATTR_FIRE, (float)atof((char*)pData[i].Data[17]) );
		Msg.RareOptionInfo[i].AttrAttack.SetElement_Val( ATTR_WATER, (float)atof((char*)pData[i].Data[18]) );
		Msg.RareOptionInfo[i].AttrAttack.SetElement_Val( ATTR_TREE, (float)atof((char*)pData[i].Data[19]) );
		Msg.RareOptionInfo[i].AttrAttack.SetElement_Val( ATTR_IRON, (float)atof((char*)pData[i].Data[20]) );
		Msg.RareOptionInfo[i].AttrAttack.SetElement_Val( ATTR_EARTH, (float)atof((char*)pData[i].Data[21]) );
		Msg.RareOptionInfo[i].PhyDefense = (WORD)atoi((char*)pData[i].Data[22]);				

		Msg.wCount++;
		StartIdx = Msg.RareOptionInfo[i].dwRareOptionIdx;
	}

	if( count >= 100 )
	{
		Msg.bEnd = FALSE;
		CharacterRareItemOptionInfo( connectIdx, tempIdx, UserIdx, CharIdx, StartIdx );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}

void CharacterUsingShopItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx )
{
	sprintf( txt, "EXEC TP_CharacterShopItemUseInfo %d", dwCharIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUsingShopItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_USINGSHOPITEMINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_USINGSHOPITEMINFO_SYN;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.Info[i].dwCharIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.Info[i].dwItemIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.Info[i].dwItemDBIdx = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.Info[i].dwItemParam = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.Info[i].dwBeginTime = (DWORD)atoi((char*)pData[i].Data[4]);
		Msg.Info[i].dwRemainTime = (DWORD)atoi((char*)pData[i].Data[5]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void CharacterUsingShopItemInfoDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwDelete )
{
	sprintf( txt, "EXEC TP_CharacterShopItemUseInfoDelete %d, %d, %d", dwCharIdx, dwItemDBIdx, dwDelete );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUsingShopItemInfoDelete, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterUsingShopItemInfoEdit( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwParam )
{
	sprintf( txt, "EXEC TP_CharacterShopItemUseInfoEdit %d, %d, %d", dwCharIdx, dwItemDBIdx, dwParam );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUsingShopItemInfoEdit, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

/////////////////////pet////////////////////////
void SearchPetByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD dwPetIdx)
{
	sprintf(txt, "EXEC TP_SearchPetinfo %d", dwPetIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, ePetBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchPetListByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD loginidx)
{
	sprintf(txt, "EXEC TP_SearchPetList %d", loginidx);	
	if(g_DB.Query(eQueryType_FreeQuery, ePetList, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

//펫 정보 검색
void RSearchPetInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	DWORD dwStartIdx, dwEndIdx;
	int nFlag = atoi((char*)pData[0].Data[0]);

	dwStartIdx = TP_PETINVEN_START;
	dwEndIdx = TP_PETWEAR_END;

	if( nFlag == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_PET;	
		Msg.Protocol = MP_RMTOOL_PETSEARCH_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
	}
	else
	{
		TMSG_PETINFO Msg;
		Msg.Category = MP_RMTOOL_PET;
		Msg.Protocol = MP_RMTOOL_PETSEARCH_ACT;
		Msg.dwTemplateIdx = tempIdx;

		PETBASEINFO Info;
		Info.dwPetDBIdx = atoi((char*)pData[0].Data[0]);			//pet index
		Info.dwMasterIdx = atoi((char*)pData[0].Data[1]);			//user id index
		Info.dwSummonItemDBIdx = atoi((char*)pData[0].Data[2]);		//아이템 고유번호
		Info.dwKind = atoi((char*)pData[0].Data[3]);				//pet 종류
		Info.dwGrade = atoi((char*)pData[0].Data[4]);				//pet 단계
		Info.dwStamina = atoi((char*)pData[0].Data[5]);				//pet stamina
		Info.dwFriendShip = atoi((char*)pData[0].Data[6]);			//pet 친밀도
		Info.wAlive = atoi((char*)pData[0].Data[7]);				//pet 생존유무 (0-생존, 1-죽음)
		Info.wSummon = atoi((char*)pData[0].Data[8]);				//pet 맵이동시 사용
		Info.wRest = atoi((char*)pData[0].Data[9]);					//pet 상태(0-휴식,1-활동)
		strcpy(Info.Loginid, (char*)pData[0].Data[10]);				//pet을 소유하고 있는 캐릭터 명	
		Info.dwCharIdx = atoi((char*)pData[0].Data[11]);			//pet을 소유하고 있는 캐릭터 index
		

		Msg.Add( &Info );

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetSize() );

		if (Info.dwCharIdx != 0)
		{
			//item_position 범위 430 ~ 489 까지 (펫 인벤)
			SearchPetWareItemInfo(connectIdx, tempIdx, Info.dwCharIdx, TP_PETINVEN_START, TP_PETINVEN_END-1 );
			//item_position 범위 490 ~ 492 까지 (펫 악세사리)
			SearchPetAccItemInfo(connectIdx, tempIdx, Info.dwCharIdx, TP_PETWEAR_START, TP_PETWEAR_END-1 );
		}
		//유저로그인 상태 확인
		SearchUserLoginChk(connectIdx, tempIdx, Info.dwMasterIdx);

	}
}

//펫 리스트 검색
void RSearchPetList( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_PET;
		Msg.Protocol = MP_RMTOOL_PETLIST_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_PETLISTRESULT Msg;
		Msg.Category = MP_RMTOOL_PET;
		Msg.Protocol = MP_RMTOOL_PETLIST_ACT;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = (BYTE)count;

		for( DWORD i = 0; i < count; ++i )
		{
			Msg.sPetList[i].dwPetDBIdx = atoi((char*)pData[i].Data[0]);		// pet index
			Msg.sPetList[i].dwKind = atoi((char*)pData[i].Data[1]);			// pet 종류
			Msg.sPetList[i].dwGrade = atoi((char*)pData[i].Data[2]);		// pet 등급
			Msg.sPetList[i].dwStamina = atoi((char*)pData[i].Data[3]);		// pet stamina
			Msg.sPetList[i].dwFriendShip = atoi((char*)pData[i].Data[4]);	// pet 친밀도
			Msg.sPetList[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		// pet을 소유한 캐릭터 index
			strcpy(Msg.sPetList[i].dwChr_Name, (char*)pData[i].Data[6]);	// pet을 소유한 캐릭터 name
		}

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );


	}
}


void SearchPetWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwPetIdx, DWORD dwStartIdx, DWORD dwEndIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchPetWareItemInfo %d, %d, %d", dwPetIdx, dwStartIdx, dwEndIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchPetWareItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchPetAccItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwPetIdx, DWORD dwStartIdx, DWORD dwEndIdx )
{
	sprintf(txt, "EXEC dbo.TP_SearchPetWareItemInfo %d, %d, %d", dwPetIdx, dwStartIdx, dwEndIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchPetAccItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}


void SearchUserLoginChk( WORD wConnectIdx, WORD wTempIdx, DWORD user_num )
{
	sprintf(txt, "EXEC dbo.TP_SearchUserLoginChk %d", user_num);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eSearchUserLoginChk, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSearchPetWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ITEMBASE Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PETITEMIFNO;
	Msg.dwTemplateIdx = tempIdx;

	
	DWORD dwPetidx, dwStartIdx, dwEndIdx;
	dwPetidx = atoi((char*)pData[0].Data[0]);

	dwStartIdx = TP_PETINVEN_START;
	dwEndIdx = TP_PETWEAR_END;

	for( DWORD i = 0; i < count; ++i )
	{
		ITEMBASE Item;
		Item.dwDBIdx = atoi((char*)pData[i].Data[1]);		//
		Item.wIconIdx = atoi((char*)pData[i].Data[2]);		
		Item.Position = atoi((char*)pData[i].Data[3]);
		Item.Durability = atoi((char*)pData[i].Data[4]);

		Msg.Add( &Item );

	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RSearchPetAccItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ITEMBASE Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PETACCIFNO;
	Msg.dwTemplateIdx = tempIdx;

	
	DWORD dwPetidx, dwStartIdx, dwEndIdx;
	dwPetidx = atoi((char*)pData[0].Data[0]);

	dwStartIdx = TP_PETWEAR_START;
	dwEndIdx = TP_PETWEAR_END;

	for( DWORD i = 0; i < count; ++i )
	{
		ITEMBASE Item;
		Item.dwDBIdx = atoi((char*)pData[i].Data[1]);
		Item.wIconIdx = atoi((char*)pData[i].Data[2]);
		Item.Position = atoi((char*)pData[i].Data[3]);
		Item.Durability = atoi((char*)pData[i].Data[4]);

		Msg.Add( &Item );

	}

//	if( count >= 100)
//		SearchPetAccItemInfo( connectIdx, tempIdx, dwPetidx, dwStartIdx, dwEndIdx );
//	else		
		//SearchPetAccItemOptionInfo( connectIdx, tempIdx, 0, dwGuildIdx );

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RSearchUserLoginChk( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD connectIdx = LOWORD(pMessage->dwID);
	WORD tempIdx = HIWORD(pMessage->dwID);

	TMSG_USERLOGINCHK Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PETUSERLOGINCHK;
	Msg.dwTemplateIdx = tempIdx;
	
	USERLOGINCHK uchk;
	uchk.UserLoginChk = atoi((char*)pData[0].Data[0]);

	Msg.Add( &uchk );

//	if( count >= 100)
//		SearchPetAccItemInfo( connectIdx, tempIdx, dwPetidx, dwStartIdx, dwEndIdx );
//	else		
		//SearchPetAccItemOptionInfo( connectIdx, tempIdx, 0, dwGuildIdx );

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void PetInfoUpdate(DWORD dwPetDBIdx, TMSG_PET_UPDATEBASEINFO* PetInfo)
{
	sprintf(txt, "TP_PetInfoUpdate %d, %d, %d, %d, %d, %d",
			dwPetDBIdx, PetInfo->wRest, PetInfo->wAlive, PetInfo->wGrade, PetInfo->dwFriendShip, PetInfo->dwStamina);
	if(g_DB.Query(eQueryType_FreeQuery, eAbilityUpdate, 0, txt) == FALSE)
	{
	}
}

void CharacterMainPetList( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx )
{
	sprintf( txt, "EXEC TP_CharacterMainPetList %d", dwCharIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterMainPetList, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}	
}

void RCharacterMainPetList( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_MAINPETLIST Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MAINPETLIST_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.MainPetList[i].dwPetDBIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.MainPetList[i].dwKind = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.MainPetList[i].dwGrade = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.MainPetList[i].dwStamina = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.MainPetList[i].dwFriendShip = (DWORD)atoi((char*)pData[i].Data[4]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void SearchPetMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID )
{
	sprintf( txt, "EXEC TP_SearchPetMemberNumFromID \'%s\'", sID );
	if( g_DB.LoginQuery(eQueryType_FreeQuery, eSearchPetMemberNumFromID, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchPetMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// No Data
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_PET;
		msg.Protocol = MP_RMTOOL_PET_MEMBERNUM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_PET;
		Msg.Protocol = MP_RMTOOL_PET_MEMBERNUM_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

//삭제된 케릭터 조회
void SearchDelCharList( WORD wConnectIdx, WORD wTempIdx, WORD WServer, char* delcharname )
{
	sprintf( txt, "EXEC TP_SEARCHDELCHARLIST \'%s\'", delcharname );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchDelCharList, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchDelCharList( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// No Data
		TMSG_DELCHARLISRESULT msg;
		msg.Category = MP_RMTOOL_DELCHAR;
		msg.Protocol = MP_RMTOOL_DELCHARLIST_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DELCHARLISRESULT Msg;
		Msg.Category = MP_RMTOOL_DELCHAR;
		Msg.Protocol = MP_RMTOOL_DELCHARLIST_ACT;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = count;

		for( DWORD i = 0; i < count; ++i )
		{
			Msg.sDelcharList[i].dwcharacter_idx = (DWORD)atoi((char*)pData[i].Data[0]);
			Msg.sDelcharList[i].dwuser_idx = (DWORD)atoi((char*)pData[i].Data[1]);
			strcpy(Msg.sDelcharList[i].character_name, (char*)pData[i].Data[2]);
			Msg.sDelcharList[i].wflag = (DWORD)atoi((char*)pData[i].Data[3]);
			strcpy(Msg.sDelcharList[i].create_date, (char*)pData[i].Data[4]);
			strcpy(Msg.sDelcharList[i].create_datetime, (char*)pData[i].Data[5]);
			strcpy(Msg.sDelcharList[i].create_ip, (char*)pData[i].Data[6]);
			strcpy(Msg.sDelcharList[i].delete_date, (char*)pData[i].Data[7]);
			strcpy(Msg.sDelcharList[i].delete_datetime, (char*)pData[i].Data[8]);
			strcpy(Msg.sDelcharList[i].delete_ip, (char*)pData[i].Data[9]);
			strcpy(Msg.sDelcharList[i].recover_date, (char*)pData[i].Data[10]);
			strcpy(Msg.sDelcharList[i].recover_datetime, (char*)pData[i].Data[11]);
			strcpy(Msg.sDelcharList[i].recover_ip, (char*)pData[i].Data[12]);
	        
		}

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DELCHARLISRESULT));
	}
}