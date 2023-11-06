// cJackpotManager.cpp: implementation of the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cJackpotManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "Network.h"
#include "packedData.h"
#include "UserTable.h"
#include "MapDBMsgParser.h"

#include "Battle.h"
#include "Player.h"
#include "Monster.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cJackpotManager)

cJackpotManager::cJackpotManager()
{
	srand((unsigned)time(NULL));
}

cJackpotManager::~cJackpotManager()
{

}

BOOL	cJackpotManager::IsAppMap( WORD MapNum )
{
	//���ܵǴ� ���� idx �迭�� �� ó��
	//??swich �� ������..
//	WORD wExcpMap[EXCP_MAP_COUNT] =
//	{
//		28,		//��� ��ʸ�Ʈ
//		44,		//������
//		59,		//����Ȳ�� �߾�
//		5,		//�������
//		62,		//��Ȱ���
//		16,		//��������
//		24,		//�ϰ����
//	};
	
	WORD wExcpMap[EXCP_MAP_COUNT] =
	{
		6,		//����
		7,		//����
		12,		//���
//		17,		//����
		28,		//��ʸ�Ʈ_�����
		44,		//������
		58,		//������
		72,		//��ȣ�屺�� ��
		73,		//�⿬�� ��
	};
	
	for(int i = 0; i < EXCP_MAP_COUNT; ++i)
	{
		if(wExcpMap[i] == MapNum)
		{
//			m_bIsAppMap = FALSE;
//			return FALSE;
			return m_bIsAppMap = FALSE;
		}
	}
	return m_bIsAppMap = TRUE;
}

BOOL	cJackpotManager::IsAppMon( CPlayer* pPlayer, CMonster* pMonster )
{
	//������ 6�̻��̰� �̺�Ʈ ���� �ƴϸ�(?�̺�Ʈ���� �����ġ)
	if( pPlayer->GetLevel() - pMonster->GetLevel() > 5 || !(pMonster->GetSMonsterList()->ExpPoint) )
		return FALSE;
	
    return TRUE;
}

BOOL	cJackpotManager::IsUserIn()
{
	if( g_pUserTable->GetUserCount() ) 
		return TRUE;
	
	return FALSE;
}

BOOL	cJackpotManager::LoadJackpotInfo()
{
	CMHFile fp;
	char szBuf[256] = {0,};

	char filename[256];
	sprintf(filename,"System/Resource/Jackpot_Info.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;

	while(TRUE)
	{
		if(fp.IsEOF())
			break;

		strcpy(szBuf,strupr(fp.GetString()));

		if(strcmp(szBuf,"#CHIPSPERMON") == 0)
		{
			m_dwChipPerMon = fp.GetDword();
		}
		else if(strcmp(szBuf,"#UPDATETIMELEN") == 0)
		{
			m_dwUpDateTimeLen = fp.GetDword() * 1000;
		}
		else if(strcmp(szBuf,"#GRADE") ==0)
		{
			fp.GetLine(szBuf,256);
			for( int i = 0; i < NUM_PRIZE; ++i )
			{
				char* cDum = fp.GetString();	// kindname
				DWORD dwAtom = fp.GetDword();	 
				cDum = fp.GetString();	// '/'
				DWORD dwDenomi = fp.GetDword();
				ASSERT(dwDenomi != 0);
				m_stPrize[i].m_dwProbability = SetProbToDword(&dwAtom, &dwDenomi);
				m_stPrize[i].m_dwPercent = fp.GetDword();
				//SW050905 4�� �߰� �۾� (��� ����ġ)
				//DB ���ν��� ������ AbsMoney�� �ֿ켱���� �Ǿ��ִ�. 0�̸� Ȯ�������� ���.
				m_stPrize[i].AbsMoney = fp.GetDword();
			}
			break;
		}
	}
	fp.Release();
	return TRUE;
}

void	cJackpotManager::Process()
{
	SendMsgDBMapMoney();
}

DWORD	cJackpotManager::SetProbToDword( DWORD* atom, DWORD* denomi )
{
//	return DWORD(MAX_PROBABILITY_LIMIT / *denomi * *atom );
	return DWORD( *atom  * MAX_PROBABILITY_LIMIT / *denomi );
}

DWORD	cJackpotManager::MakeRndValue()
{
//	DWORD dwBig = rand()%10000;	//0 ~ 9999 //0, 0~99990000
//	DWORD dwLit = rand()%10000 + 1;	//1 ~ 10000
//
//	return (dwBig * 10000 + dwLit);

	return (rand()%10000) * 10000 + (rand()%10000 + 1);
}

int		cJackpotManager::CheckPrize( int kind )
{
	if( CalcAddProb(kind) >= MakeRndValue() )
	{
		return kind;
	}
	else
		return NO_PRIZE;
}

int		cJackpotManager::CheckPrize()
{
	DWORD Rnd = MakeRndValue();

	
	int kind = NO_PRIZE;
/*
	if( Rnd > CalcAddProb(kind) )
		return NO_PRIZE;
	// Ȯ�� ���� 3�� ���� ���Ѵ�.	// �ణ�̶� ���� ���̱�����..
	while( Rnd <= CalcAddProb(kind) )
	{
		kind--;
	}

	return ++kind;*/
	if( Rnd > CalcAddProb(FTH_PRIZE)) return kind;

	for( int i = 0; i < NUM_PRIZE; ++ i )
	{
		if( Rnd <= CalcAddProb(i) )
			return i;
	}
	/*
	if( Rnd <= CalcAddProb(FST_PRIZE) )
	{
		kind = FST_PRIZE;
	}
	else if( Rnd <= CalcAddProb(SND_PRIZE) )
	{
		kind = SND_PRIZE;
	}
	else if( Rnd <= CalcAddProb(THR_PRIZE) )
	{
		kind = THR_PRIZE;
	}
	else if( Rnd <= CalcAddProb(FTH_PRIZE) )
	{
		kind = FTH_PRIZE;
	}*/
	return kind;
}

void	cJackpotManager::DoJackpot(CPlayer* pPlayer, CMonster* pMonster)
{
//	if(!GetIsAppMap())	return;
	if(!m_bIsAppMap)	return;

//	m_pPlayer = pPlayer;
//	m_pMonster = pMonster;

	if(!IsAppMon( pPlayer, pMonster ))	return;

	m_dwMapJPMoney += m_dwChipPerMon;

	//SW050803 for cheat
	if(!m_bDoPrize) return;

	m_nPrizeKind = CheckPrize();

	if(m_nPrizeKind != NO_PRIZE)	//if(m_nPrizeKind > NO_PRIZE)
	{
		ASSERT(m_nPrizeKind<NUM_PRIZE);
		JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), m_nPrizeKind, m_stPrize[m_nPrizeKind].m_dwPercent, m_stPrize[m_nPrizeKind].AbsMoney );
		m_dwMapJPMoney = 0;
	}
	else m_dwPercentage = 0;
}

void	cJackpotManager::SendMsgDBMapMoney()
{//SendMapMoneyInfo to DB regularly
	if( gCurTime - m_dwLastDBUpdateTime < m_dwUpDateTimeLen )
		return;
	// 050726 ������� ����
	// ������ ���� �ÿ� ������ ����	
	if(!IsUserIn())
	{
		m_dwLastDBUpdateTime = gCurTime;
		return;
	}
	else
	{
		//DB�� �ʸӴ� ������ (�������� �Ѿ� �޾ƿ´�.RJackpotAddTotalMoney())
		JackpotAddTotalMoney( m_dwMapJPMoney );
		m_dwMapJPMoney = 0;
		m_dwLastDBUpdateTime = gCurTime;
	}
}

//SW050802 for Cheat
void	cJackpotManager::SendMsgDBPrizeInfo( CPlayer* pPlayer, int PrizeKind )
{
	JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), PrizeKind, m_stPrize[PrizeKind].m_dwPercent, m_stPrize[PrizeKind].AbsMoney );
	m_dwMapJPMoney = 0;
}

void	cJackpotManager::SendMsgAllUserPrizeNotify( DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind )
{
	// SendMsg To AllUser
	// Msg description : PlayerId, CharName, PrizeMoney, RestMoney

//	SetTotalMoney(TotalMoney);
//	m_dwPrizeMoney = PrizeMoney;	//!������ ��¿�(����)
	
	MSG_JACKPOT_PRIZE_NOTIFY msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_PRIZE_NOTIFY);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	ASSERT(pPlayer);
	if(!pPlayer) return;	//!��÷�� �ٽ� DB�� �����ٱ�...
	SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
//	msg.dwPrizeUserID = m_pPlayer->GetID();	// �ش� ĳ���� ����Ʈ ����	// ������ ������ ��츦 ������ mPlayer ��� ����
	msg.dwPrizeUserID = PlayerID;
	ASSERT(PrizeKind < 3);
	msg.nPrizeKind = PrizeKind;
	msg.dwPrizeMoney = PrizeMoney;
	msg.dwRestTotalMoney = TotalMoney;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );

	// ������� ���� 050725
	// merge SendMsgPrizeEffect() 
	//PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(MSG_JACKPOT_PRIZE_NOTIFY));	
}

/*
void	cJackpotManager::SendMsgMapUserTotalMoney( DWORD TotalMoney )
{
	//Send Msg To MapConnectingUsers
	// Time Control is up to RJackpotAddTotalMoney()

	SetTotalMoney(TotalMoney);

	MSG_DWORD msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);
	msg.dwData = m_dwTotalJPMoney;
	
	g_pUserTable->SetPositionUserHead();
	while(CObject* pObject = g_pUserTable->GetData())
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		CPlayer* pPlayer = (CPlayer*) pObject;
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}

}*/

/*
void	cJackpotManager::SendMsgAddUserTotalMoney( CObject* pObject )
{	// Send TotalMoney Msg To AddUser
	MSG_DWORD msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);
	msg.dwData = m_dwTotalJPMoney;
	
	CPlayer* pPlayer = (CPlayer*) pObject;
	pPlayer->SendMsg( &msg, sizeof(msg) );
}*/

/*
void	cJackpotManager::SendMsgPrizeEffect(DWORD PlayerID)
{
	MSG_DWORD msg;
	SetProtocol(&msg, MP_JACKPOT, MP_JACKPOT_PRIZE_EFFECT);
	msg.dwObjectID = PlayerID;
	msg.dwData = PlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	if(!pPlayer) return;

	PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(MSG_DWORD));	
}*/

void	cJackpotManager::SetPrizeInfo( DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind )
{
	//�ش� ID�� â��� MONEY ���
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	if(!pPlayer) return;
	pPlayer->SetMoney( PrizeMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetPrize, 0 );
	//�� ������ ����
	SendMsgAllUserPrizeNotify( TotalMoney, PrizeMoney, PlayerID, PrizeKind );
	// ����Ʈ ����(In Grid)
	// SW050808 �������� ó��
//	SendMsgPrizeEffect(PlayerID);	
}

DWORD	cJackpotManager::CalcAddProb(int kind)
{//������ ��� Ȯ�� �߰�
//	return m_stPrize[kind].m_dwProbability * (1 + (m_pMonster->GetLevel())/10 );
	// 050728 ���ƾ� ��û
	return m_stPrize[kind].m_dwProbability;
}

void	cJackpotManager::Init( WORD Mapnum )
{
	m_dwTotalJPMoney	= 0;	//���� �������� Fr_DB
	m_dwMapJPMoney		= 0;	//�� ������
	m_dwPrizeMoney		= 0;	//��÷ �ݾ�(Fr_DB)
	m_dwPercentage		= 0;

	m_pPlayer			= NULL;
	m_pMonster			= NULL;

	m_nPrizeKind		= NO_PRIZE;	//��÷ ����(NO_PRIZE = -1)
	
	m_dwChipPerMon		= 0;	//���� ������(Fr_Script) //-��������
	m_dwUpDateTimeLen	= 0;	//���� �ݾ� Update �ð� ����(Fr_Script)
	m_dwLastDBUpdateTime	= 0;	//���� Update �ð�
	
//	m_bActive			= TRUE;		//JPMgr active
	m_bDoPrize			= TRUE;		//Prize active
	m_bIsAppMon			= TRUE;	//���� ������
	m_bIsAppMap			= IsAppMap(Mapnum);

	// DB Query
//	JackpotLoadTotalMoney();
}
