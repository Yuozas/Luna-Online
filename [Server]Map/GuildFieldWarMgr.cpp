#include "stdafx.h"
#include "GuildFieldWarMgr.h"
#include "UserTable.h"
#include "Player.h"
#include "Network.h"
#include "LootingManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"
#include "GuildUnionManager.h"
#include "GuildUnion.h"
#include "Guild.h"
#include "GuildManager.h"


extern int g_nServerSetNum;


CGuildWar::CGuildWar()
{	
	m_dwGuildIdx = 0;
	m_EnemyGuildTable.Initialize( 20 );
	m_dwVictoryCount = m_dwDrawCount = m_dwLooseCount = 0;
}

CGuildWar::~CGuildWar()
{
	Release();
}

void CGuildWar::Release()
{
	sGFWENEMY* pData = NULL;
	m_EnemyGuildTable.SetPositionHead();
	while( pData = m_EnemyGuildTable.GetData() )
		delete pData;
	m_EnemyGuildTable.RemoveAll();
}

void CGuildWar::RegistGuildWar( DWORD dwEnemyGuildIdx, DWORD dwMoney )
{
	if( m_EnemyGuildTable.GetData( dwEnemyGuildIdx ) )
		return;

	sGFWENEMY* pData = new sGFWENEMY;
	pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
	pData->dwGFWMoney = dwMoney;
	m_EnemyGuildTable.Add( pData, dwEnemyGuildIdx );
}

void CGuildWar::UnregistGuildWar( DWORD dwEnemyGuildIdx )
{
	sGFWENEMY* pData = m_EnemyGuildTable.GetData( dwEnemyGuildIdx );
	if( !pData )	return;

	delete pData;
	m_EnemyGuildTable.Remove( dwEnemyGuildIdx );
}


BOOL CGuildWar::IsEnemy( DWORD dwEnemyGuildIdx )
{
	return m_EnemyGuildTable.GetData( dwEnemyGuildIdx ) > 0;
}


void CGuildWar::AddPlayer( CPlayer* player )
{
	// �� ��� ��� ����
	{
		MSG_GUILD_LIST message;
		message.Category	= MP_GUILD_WAR;
		message.Protocol	= MP_GUILD_WAR_PROC;
		message.mSize		= 0;

		m_EnemyGuildTable.SetPositionHead();

		for( const sGFWENEMY* enemy; enemy = m_EnemyGuildTable.GetData(); )
		{
			const CGuild* guild = GUILDMGR->GetGuild( enemy->dwEnemyGuildIdx );

			if( guild )
			{
				MSG_GUILD_LIST::Data& dest	= message.mData[ message.mSize++ ];
				const GUILDINFO&			source	= guild->GetInfo();

				// �ùٷ� ����ǳ� �� �׽�Ʈ�� ��!
				memcpy( &dest, &source, sizeof( source ) );

				// MarkName�� ������ �뵵�� ����ϰ� �ִ�.
				dest.MarkName	= enemy->dwGFWMoney;

				CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( source.UnionIdx );

				if( alliance )
				{
					SafeStrCpy( dest.mUnionName, alliance->GetName(), sizeof( dest.mUnionName ) );
				}
				else
				{
					dest.mUnionName[ 0 ] = 0;
				}
			}
		}

		if( message.mSize )
		{
			player->SendMsg( &message, message.GetSize() );
		}
	}

	// ���� ����
	{
		MSG_DWORD3 message;
		message.Category	= MP_GUILD_WAR;
		message.Protocol	= MP_GUILD_WAR_RECORD;
		message.dwData1		= m_dwVictoryCount;
		message.dwData2		= m_dwDrawCount;
		message.dwData3		= m_dwLooseCount;

		player->SendMsg( &message, sizeof( message ) );
	}
}


BOOL CGuildWar::IsGuildWar()
{
	return m_EnemyGuildTable.GetDataNum() > 0;
}


void CGuildWar::UpdateGuildWarRecord()
{
	UpdateGuildWarRecordToDB( m_dwGuildIdx, m_dwVictoryCount, m_dwDrawCount, m_dwLooseCount );
}


DWORD CGuildWar::GetWarMoney( DWORD dwEnemyGuildIdx )
{
	const sGFWENEMY* pData = m_EnemyGuildTable.GetData( dwEnemyGuildIdx );

	return pData ? pData->dwGFWMoney : 0;
}


void CGuildWar::GetEnemy( std::list< sGFWENEMY* >& enemyList )
{
	m_EnemyGuildTable.SetPositionHead();

    for(	sGFWENEMY* enemy;
			enemy = m_EnemyGuildTable.GetData(); )
	{
		enemyList.push_back( enemy );
	}
}



/**********************************************************************

�̸�: CGuildWarManager
                                                                     
***********************************************************************/


CGuildWarManager::CGuildWarManager()
{
	m_GuildWarTable.Initialize( 100 );
	//m_GFWCheckTable.Initialize( 100 );
}

CGuildWarManager::~CGuildWarManager()
{
	Release();
}

void CGuildWarManager::Init()
{
//	LoadGuildWar( 0 );
}

void CGuildWarManager::Release()
{
	{
		m_GuildWarTable.SetPositionHead();

		for(	CGuildWar* pData;
				pData = m_GuildWarTable.GetData();
				)
		{
			SAFE_DELETE( pData );
		}

		m_GuildWarTable.RemoveAll();
	}
	
	//CGFWCheck* pCheck = NULL;
	//m_GFWCheckTable.SetPositionHead();
	//while( pCheck = m_GFWCheckTable.GetData() )
	//	delete pCheck;
	//m_GFWCheckTable.RemoveAll();
}

void CGuildWarManager::RegistGuildWarFromRecord( DWORD dwGuildIdx, DWORD dwVictory, DWORD dwDraw, DWORD dwLoose )
{
	CGuildWar* pGuildWar = m_GuildWarTable.GetData( dwGuildIdx );

	if( pGuildWar == NULL )
	{
		pGuildWar = new CGuildWar;
		pGuildWar->Init( dwGuildIdx );
		m_GuildWarTable.Add( pGuildWar, dwGuildIdx );
	}

	pGuildWar->SetGuildWarRecord( dwVictory, dwDraw, dwLoose );
}

void CGuildWarManager::UpdateGuildWarRecord( BOOL bDB, WORD wFlag, DWORD dwGuildIdx )
{
	CGuildWar* pGuildWar = m_GuildWarTable.GetData( dwGuildIdx );
	if( pGuildWar == NULL )	return;
	switch( wFlag )
	{
	case 0:		pGuildWar->LooseGuildWar();		break;	// loose
	case 1:		pGuildWar->VictoryGuildWar();		break;	// victory
	case 2:		pGuildWar->DrawGuildWar();		break;	// draw	
	}

	if( bDB )
		pGuildWar->UpdateGuildWarRecord();
}

BOOL CGuildWarManager::RegistGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney )
{
	CGuildWar* pGuildWar1 = m_GuildWarTable.GetData( dwGuildIdx1 );
	if( pGuildWar1 == NULL )
	{
		pGuildWar1 = new CGuildWar;
		pGuildWar1->Init( dwGuildIdx1 );
		m_GuildWarTable.Add( pGuildWar1, dwGuildIdx1 );
	}
	pGuildWar1->RegistGuildWar( dwGuildIdx2, dwMoney );

	CGuildWar* pGuildWar2 = m_GuildWarTable.GetData( dwGuildIdx2 );
	if( pGuildWar2 == NULL )
	{
		pGuildWar2 = new CGuildWar;
		pGuildWar2->Init( dwGuildIdx2 );
		m_GuildWarTable.Add( pGuildWar2, dwGuildIdx2 );
	}
	pGuildWar2->RegistGuildWar( dwGuildIdx1, dwMoney );

	return TRUE;
}

BOOL CGuildWarManager::UnregistGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	CGuildWar* pGuildWar1 = m_GuildWarTable.GetData( dwGuildIdx1 );
	if( pGuildWar1 )
		pGuildWar1->UnregistGuildWar( dwGuildIdx2 );

	CGuildWar* pGuildWar2 = m_GuildWarTable.GetData( dwGuildIdx2 );
	if( pGuildWar2 )
		pGuildWar2->UnregistGuildWar( dwGuildIdx1 );

	return TRUE;
}

BOOL CGuildWarManager::JudgeGuildWar( CPlayer* pPlayer, CPlayer* pAttacker )
{
	// pPlayer is Die...
	if( pAttacker->GetObjectKind() != eObjectKind_Player ||
		IsEnemy( pPlayer, pAttacker ) == FALSE )
	{
		return FALSE;
	}
	
	return TRUE;

//	if( pPlayer->GetGuildMemberRank() == GUILD_MASTER )
//	{
//		DWORD dwGuildIdxWinner	= pAttacker->GetGuildIndex();
//		DWORD dwGuildIdxLoser	= pPlayer->GetGuildIdx();
//
//		End( dwGuildIdxWinner, dwGuildIdxLoser );
//
////		if( g_pServerSystem->GetNation() == eNATION_CHINA || g_nServerSetNum != 4 )
////			LOOTINGMGR->AutoLooting( pPlayer, pAttacker );		
//	}
//	else
//	{
////		if( g_pServerSystem->GetNation() == eNATION_CHINA || g_nServerSetNum != 4 )
////			LOOTINGMGR->AutoLooting( pPlayer, pAttacker );
//	}
}


BOOL CGuildWarManager::IsEnemy( CPlayer* pOper, CPlayer* pTarget )
{
	CGuildWar* pGuildWar = m_GuildWarTable.GetData( pOper->GetGuildIdx() );

	if( ! pGuildWar ||
		! pGuildWar->IsEnemy( pTarget->GetGuildIdx() ) )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CGuildWarManager::IsEnemyInField( CPlayer* pOper, CPlayer* pTarget )
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return FALSE;

	CGuildWar* pGuildWar = m_GuildWarTable.GetData( pOper->GetGuildIdx() );
	if( pGuildWar == NULL )	return FALSE;
	if( !pGuildWar->IsEnemy( pTarget->GetGuildIdx() ) )	return FALSE;

	return TRUE;
}

void CGuildWarManager::AddPlayer( CPlayer* pPlayer )
{
	CGuildWar* pGuildWar = m_GuildWarTable.GetData( pPlayer->GetGuildIdx() );
	if( pGuildWar )
		pGuildWar->AddPlayer( pPlayer );
}

BOOL CGuildWarManager::IsGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	CGuildWar* pGuildWar1 = m_GuildWarTable.GetData( dwGuildIdx1 );
	CGuildWar* pGuildWar2 = m_GuildWarTable.GetData( dwGuildIdx2 );

	if( pGuildWar1 == NULL || pGuildWar2 == NULL )	return FALSE;
	if( pGuildWar1->IsEnemy( dwGuildIdx2 ) && pGuildWar2->IsEnemy( dwGuildIdx1 ) )
		return TRUE;
	return FALSE;
}

BOOL CGuildWarManager::IsGuildWar( DWORD dwGuildIdx )
{
	CGuildWar* pGuildWar = m_GuildWarTable.GetData( dwGuildIdx );
	if( pGuildWar == NULL )	return FALSE;
	
	return pGuildWar->IsGuildWar();
}

//int CGuildWarManager::CanGuildWar( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
//{
//	CGFWCheck* pData = m_GFWCheckTable.GetData( dwGuildIdx );
//	if( !pData )	return 0;	// can
//
//	return pData->CanGuildWar( dwEnemyGuildIdx );
//}

//void CGuildWarManager::RegistDeny( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
//{
//	CGFWCheck* pData = m_GFWCheckTable.GetData( dwGuildIdx );
//	if( !pData )
//	{
//		pData = new CGFWCheck;
//		pData->Init( dwGuildIdx );
//		m_GFWCheckTable.Add( pData, dwGuildIdx );
//	}
//	pData->RegistDenyTime( dwEnemyGuildIdx );
//}
//
//void CGuildWarManager::RegistEnd( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
//{
//	CGFWCheck* pData1 = m_GFWCheckTable.GetData( dwGuildIdx1 );
//	if( !pData1 )
//	{
//		pData1 = new CGFWCheck;
//		pData1->Init( dwGuildIdx1 );
//		m_GFWCheckTable.Add( pData1, dwGuildIdx1 );
//	}
//	pData1->RegistEndTime( dwGuildIdx2 );
//
//	CGFWCheck* pData2 = m_GFWCheckTable.GetData( dwGuildIdx2 );
//	if( !pData2 )
//	{
//		pData2 = new CGFWCheck;
//		pData2->Init( dwGuildIdx2 );
//		m_GFWCheckTable.Add( pData2, dwGuildIdx2 );
//	}
//	pData2->RegistEndTime( dwGuildIdx1 );
//}

DWORD CGuildWarManager::GetWarMoney( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
{
	CGuildWar* pGFW = m_GuildWarTable.GetData( dwGuildIdx );

	return pGFW ? pGFW->GetWarMoney( dwEnemyGuildIdx ) : 0;
}

void CGuildWarManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_GUILD_WAR_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			CObject* object = g_pUserTable->FindUser( pmsg->dwObjectID );
			
			if( object )
			{
				object->SendMsg( pmsg, sizeof( MSG_BYTE ) );
			}

			break;
		}
	case MP_GUILD_WAR_DECLARE:
		{
			Declare( pMsg );
			break;
		}
	case MP_GUILD_WAR_DECLARE_NACK:
		{
			break;
		}
	case MP_GUILD_WAR_DECLARE_ACCEPT:
		{
			DeclareAccept( pMsg );
			break;
		}
	case MP_GUILD_WAR_DECLARE_DENY:
		{
			DeclareDeny( pMsg );
			break;
		}
	case MP_GUILD_WAR_DECLARE_DENY_NOTIFY_TOMAP:
		{
			DeclareDenyNotifyToMap( pMsg );
			break;
		}
	case MP_GUILD_WAR_START:
		{
			break;
		}
	case MP_GUILD_WAR_START_NOTIFY_TOMAP:
		{
			StartNotifyToMap( pMsg );
			break;
		}
	case MP_GUILD_WAR_PROC:
		{
			break;
		}
	case MP_GUILD_WAR_END:
		{
			break;
		}
	case MP_GUILD_WAR_END_NOTIFY_TOMAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			DWORD dwGuildIdxWinner = pmsg->dwData1;
			DWORD dwGuildIdxLoser = pmsg->dwData2;
			if( dwGuildIdxWinner == 0 || dwGuildIdxLoser == 0 )	return;

			CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
			CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );

			if( pGuildWinner == NULL || pGuildLoser == NULL )	return;

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_END;

			// send guildwinner
			Msg.dwData1 = 1;	// 0 = lose, 1 = win
			Msg.dwData2 = dwGuildIdxLoser; 
			pGuildWinner->SendMsgToAll( &Msg, sizeof(Msg) );

			// send guildloser
			Msg.dwData1 = 0;
			Msg.dwData2 = dwGuildIdxWinner; 
			pGuildLoser->SendMsgToAll( &Msg, sizeof(Msg) );

			// unregist
			UnregistGuildWar( dwGuildIdxWinner, dwGuildIdxLoser );
			//	PenaltyForEndNotifyToMap( 0, dwGuildIdxLoser );

			// guildfieldwarrecord
			UpdateGuildWarRecord( FALSE, 0, dwGuildIdxLoser );
			UpdateGuildWarRecord( FALSE, 1, dwGuildIdxWinner );

			// registend
			//RegistEnd( dwGuildIdxWinner, dwGuildIdxLoser );
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND:
		{
			SuggestEnd( pMsg );
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_NOTIFY_TOMAP:
		{
			const MSG_DWORD2*	pmsg	= (MSG_DWORD2*)pMsg;
			CObject*			object	= g_pUserTable->FindUser( pmsg->dwData1 );
			if( object )
			{
				MSG_DWORD message;
				message.Category	= MP_GUILD_WAR;
				message.Protocol	= MP_GUILD_WAR_SUGGESTEND;
				message.dwData		= pmsg->dwData2;

				object->SendMsg( &message, sizeof(message) );
			}

			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_NACK:
		{
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_ACCEPT:
		{
			SuggestEndAccept( pMsg );
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP:
		{
			SuggestEndAcceptNotifyToMap( pMsg );
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_DENY:
		{
			SuggestEndDeny( pMsg );
			break;
		}
	case MP_GUILD_WAR_SUGGESTEND_DENY_NOTIFY_TOMAP:
		{
			SuggestEndDenyNotifyToMap( pMsg );
			break;
		}
	case MP_GUILD_WAR_SURRENDER:
		{
			Surrender( pMsg );
			break;
		}
	case MP_GUILD_WAR_SURRENDER_NACK:
		{
			break;
		}
	case MP_GUILD_WAR_SURRENDER_NOTIFY_TOMAP:
		{
			SurrenderNotifyToMap( pMsg );
			break;
		}
	case MP_GUILD_WAR_ADDMONEY_TOMAP:
		{
			AddMoneyToMap( pMsg );
			break;
		}
	default:
		{
			ASSERT( 0 && "It is not defined" );
			break;
		}
	}
}


void CGuildWarManager::Declare( void* pMsg )
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CGuild* pGuild1 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild* pGuild2 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );

	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	// ����! ����� �׽�Ʈ�� ���� ��� ���� �˻����� �ʵ��� ��
	if( pGuild1->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 2 );
		return;
	}
	else if( pGuild2->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 3 );
		return;
	}
	else if( IsGuildWar( pGuild1->GetInfo().GuildIdx, pGuild2->GetInfo().GuildIdx ) )
	{
		return;
	}	

	// 071009 ����, ���1�� ���տ� ���ϳ� ������ ��尡 �ƴ� ���, Ŭ���̾�Ʈ �˻簡 �����߰ų� ��ŷ��
	{
		CGuildUnion* guildUnion = GUILDUNIONMGR->GetUnion( pGuild1->GetUnionIndex() );

		if( guildUnion && guildUnion->GetMasterGuild() != pGuild1 )
		{
			return;
		}
	}

	CPlayer* guild1Master	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	CPlayer* guild2Master	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );

	// 071009 ����, ���2�� ���տ� ���ϳ� ������ ��尡 �ƴ� ��� ���1���� �˷��ش�
	if( guild1Master )
	{
		CGuildUnion* guildUnion = GUILDUNIONMGR->GetUnion( pGuild2->GetUnionIndex() );

		if( guildUnion && guildUnion->GetMasterGuild() != pGuild2 )
		{
			CGuild* masterGuild = guildUnion->GetMasterGuild();

			MSG_NAME message;
			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_DECLARE_NACK_BY_UNION;			
			SafeStrCpy( message.Name, masterGuild->GetGuildName(), sizeof( message.Name ) );

			guild1Master->SendMsg( &message, sizeof( message ) );
			
			return;
		}
	}

	//int rt = CanGuildWar( pGuild2->GetIdx(), pGuild1->GetIdx() );
	//if( rt != 0 )
	//{
	//	SendDeclareNackMsg( pmsg->dwData1, rt );
	//	return;
	//}	
	
	const DWORD money = pmsg->dwData3;

	if( guild1Master )
	{
		if( money > guild1Master->GetMoney() )
		{
			SendDeclareNackMsg( pmsg->dwData1, 98 );
			return;
		}

		MSG_BYTE message;
		message.Category = MP_GUILD_WAR;
		message.Protocol = MP_GUILD_WAR_WAIT;
		message.bData = MP_GUILD_WAR_DECLARE;		
		guild1Master->SendMsg( &message, sizeof( message ) );
	}

	if( guild2Master )
	{
		MSG_GUILD_LIST message;
		message.Category	= MP_GUILD_WAR;
		message.Protocol	= MP_GUILD_WAR_DECLARE;
		message.mSize		= 1;

		MSG_GUILD_LIST::Data&	dest	= message.mData[ 0 ];
		const GUILDINFO&		source	= pGuild1->GetInfo();

		// ����� ����ǳ� �׽�Ʈ�� ��
		memcpy( &dest, &source, sizeof( source ) );

		dest.MarkName = money;

		CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( source.UnionIdx );

		if( alliance )
		{
			SafeStrCpy( dest.mUnionName, alliance->GetName(), sizeof( dest.mUnionName ) );
		}
		else
		{
			dest.mUnionName[ 0 ] = 0;
		}

		guild2Master->SendMsg( &message, message.GetSize() );
	}
}


void CGuildWarManager::DeclareAccept( void* pMsg )
{
	/*
	071009 ����
	�䱸����:	����<->���, ���<->���, ����<->���� ���� ������ �����ؾ��Ѵ�.
	ó������:	���� ��� A���� B������ ������ �Ѵٰ� ����. ������ ���� ������ ��� �ٸ� �ڷᱸ�� A, B�� �ִ´�.
				���� B�� �ִ� ��� ��ü�� ������ ����Ѵ�. ������ A�� �ִ� ��� ��ü�� ������ ����Ѵ�.
				�̷� ������ ���� ���μ����� �̿��Ͽ� �������� ���� ó���� �� �ִ�.
	����:		�پ��� �׽�Ʈ ���̽��� ����Ͽ� �����غ����Ѵ�. 
	*/

	MSG_DWORD2* pmsg				= ( MSG_DWORD2* )pMsg;
	CPlayer*	friendlyGuildMaster	= ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

	if( ! friendlyGuildMaster )
	{
		return;	
	}

	const DWORD friendlyGuildIndex	= friendlyGuildMaster->GetGuildIdx();
	const DWORD enemyGuildIndex		= pmsg->dwData1;
	CGuild*		friendlyGuild		= GUILDMGR->GetGuildFromMasterIdx( pmsg->dwObjectID );
	CGuild*		enemyGuild			= GUILDMGR->GetGuild( enemyGuildIndex );

	if( ( ! friendlyGuild || ! enemyGuild )					||
			IsGuildWar( friendlyGuildIndex, enemyGuildIndex ) )
	{
		return;
	}

	Alliance		friendlyAlliance;
	Alliance		enemyAlliance;
	const DWORD		money = pmsg->dwData2;

	// 071008 ����, ��尡 �����̶�� ���������� �˻��� �� �Ʊ��� ����Ѵ�.
	{
		{
			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( friendlyGuild->GetUnionIndex() );

			if( alliance )
			{
				if( alliance->GetMasterGuild() != friendlyGuild )
				{
					// ó�� ���ʿ�. �̹� �������� ��û�� �� �˻�����
					return;
				}

				AddGuildToGroup( alliance, friendlyAlliance );
			}

			// ���� �������� ��� ���� �� ����� �������� 0���� �����Ѵ�. ���� ������ ��常 �������� �����Ǿ�� �ϹǷ� �������� �����Ѵ�
			friendlyAlliance[ friendlyGuild ] = money;
		}

		{
			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( enemyGuild->GetUnionIndex() );

			if( alliance )
			{
				if( alliance->GetMasterGuild() != enemyGuild )
				{
					// ó�� ���ʿ�. �̹� �������� ��û�� �� �˻�����
					return;
				}

				AddGuildToGroup( alliance, enemyAlliance );
			}

			// ���� �������� ��� ���� �� ����� �������� 0���� �����Ѵ�. ���� ������ ��常 �������� �����Ǿ�� �ϹǷ� �������� �����Ѵ�
			enemyAlliance[ enemyGuild ] = money;
		}
	}

	{
		CPlayer* enemyGuildMaster = ( CPlayer* )g_pUserTable->FindUser( enemyGuild->GetMasterIdx() );

		SendMoneyMsg( friendlyGuildMaster, enemyGuildMaster, money, MP_GUILD_WAR_REMOVEMONEY );
	}	

	AddEnemyToFriendly( friendlyAlliance, enemyAlliance );
	AddEnemyToFriendly( enemyAlliance, friendlyAlliance );
}


void CGuildWarManager::DeclareDeny( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData1;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild == NULL )	return;

	pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuild->GetMasterIdx() );
	if( pPlayer )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_WAR;
		Msg.Protocol = MP_GUILD_WAR_DECLARE_DENY;
		Msg.dwData = dwGuildIdx1;

		pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else
	{
		MSG_DWORD2 Notify;
		Notify.Category = MP_GUILD_WAR;
		Notify.Protocol = MP_GUILD_WAR_DECLARE_DENY_NOTIFY_TOMAP;
		Notify.dwData1 = pGuild->GetMasterIdx();
		Notify.dwData2 = dwGuildIdx1;
	
		g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );	
	}

	// regist deny
	//RegistDeny( dwGuildIdx1, dwGuildIdx2 );
}

void CGuildWarManager::DeclareDenyNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( pPlayer == NULL )	return;	

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_DECLARE_DENY;
	Msg.dwData = pmsg->dwData2;

	pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );

	// regist deny
	//RegistDeny( pmsg->dwData2, pPlayer->GetGuildIdx() );
}

void CGuildWarManager::StartNotifyToMap( void* pMsg )
{
	const MSG_DWORD3*	pmsg		= ( MSG_DWORD3* )pMsg;
	const DWORD			guild1Index	= pmsg->dwData1;
	const DWORD			guild2Index	= pmsg->dwData2;
	const MONEYTYPE		money		= pmsg->dwData3;
	
	CGuild*				guild1		= GUILDMGR->GetGuild( guild1Index );
	CGuild*				guild2		= GUILDMGR->GetGuild( guild2Index );

	if( !	guild1 ||
		!	guild2 || 
			IsGuildWar( guild1Index, guild2Index ) )
	{
		return;
	}

	{
		CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( guild2->GetMasterIdx() );

		if( player )
		{
			SendMoneyMsg( NULL, player, money, MP_GUILD_WAR_REMOVEMONEY );
		}
	}

	// ��ø �Լ�
	struct
	{
		void operator() ( CGuild* sourceGuild, CGuild* destGuild, DWORD money )
		{
			if( ! sourceGuild ||
				! destGuild )
			{
				ASSERT( 0 );
				return;
			}

			MSG_GUILD_LIST message;

			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_START;
			message.mSize		= 1;

			MSG_GUILD_LIST::Data&	dest	= message.mData[ 0 ];
			const GUILDINFO&		source	= sourceGuild->GetInfo();

			// ���������� ������ üũ����.
			memcpy( &dest, &source, sizeof( source ) );

			dest.MarkName = money;
			
			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( sourceGuild->GetUnionIndex() );

			if( alliance )
			{
				SafeStrCpy( dest.mUnionName, alliance->GetName(), sizeof( dest.mUnionName ) );
			}
			else
			{
				dest.mUnionName[ 0 ] = 0;
			}

			destGuild->SendMsgToAll( &message, message.GetSize() );
		}
	}
	SendMessage;

	SendMessage( guild1, guild2, money );
	SendMessage( guild2, guild1, money );

	RegistGuildWar( guild1Index, guild2Index, money );
}

/*
void CGuildWarManager::End( DWORD dwGuildIdxWinner, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );

	if( pGuildWinner == NULL || pGuildLoser == NULL )	return;

	// money
	DWORD dwMoney = GetWarMoney( dwGuildIdxWinner, dwGuildIdxLoser );
	if( dwMoney )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildWinner->GetMasterIdx() );
		if( pPlayer )
			SendMoneyMsg( pPlayer, NULL, dwMoney*2, MP_GUILD_WAR_ADDMONEY );
		else
			SendToAgentAddMoneyMsg( pGuildWinner->GetMasterIdx(), dwMoney*2 );
	}		

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_END;

	// send guildwinner
	Msg.dwData1 = 1;	// 0 = lose, 1 = win
	Msg.dwData2 = dwGuildIdxLoser; 
	pGuildWinner->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guildloser
	Msg.dwData1 = 0;
	Msg.dwData2 = dwGuildIdxWinner; 
	pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send to othermap
	MSG_DWORD2 Notify;
	Notify.Category = MP_GUILD_WAR;
	Notify.Protocol = MP_GUILD_WAR_END_NOTIFY_TOMAP;
	Notify.dwData1 = dwGuildIdxWinner;		// winner
	Notify.dwData2 = dwGuildIdxLoser;		// loser
	g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );

	// db delete
	DeleteGuildWar( dwGuildIdxWinner, dwGuildIdxLoser );
	// unregist
	UnregistGuildWar( dwGuildIdxWinner, dwGuildIdxLoser );
//	PenaltyForEnd( 0, dwGuildIdxLoser );

	// guildfieldwarrecord
	UpdateGuildWarRecord( TRUE, 0, dwGuildIdxLoser );
	UpdateGuildWarRecord( TRUE, 1, dwGuildIdxWinner );

	// registend
	//RegistEnd( dwGuildIdxWinner, dwGuildIdxLoser );	

	//SendToAllUser( 0, pGuildWinner->GetGuildName(), pGuildLoser->GetGuildName() );
}
*/


void CGuildWarManager::SuggestEnd( void* pMsg )
{
	// ���ǻ� ������ ��û�� ���� �Ʊ�, ��û���� ���� �����̶� Ī����

	const MSG_DWORD3*	pmsg			= (MSG_DWORD3*)pMsg;
	CGuild*				friendlyGuild	= GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild*				enemyGuild		= GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );

	if( !	friendlyGuild	||
		!	enemyGuild		||
		!	IsGuildWar( friendlyGuild->GetIdx(), enemyGuild->GetIdx() ) )
	{
		return;
	}

	// �����ε� ������ ��尡 �ƴ� ��� ��ŷ �Ǵ� Ŭ���̾�Ʈ �˻� ����
	{
		CGuildUnion* friendlyAlliance	= GUILDUNIONMGR->GetUnion( friendlyGuild->GetUnionIndex() );
		CGuildUnion* enemyAlliance		= GUILDUNIONMGR->GetUnion( enemyGuild->GetUnionIndex() );

		if( ( friendlyAlliance	&& friendlyAlliance->GetMasterGuild()	!= friendlyGuild	)	||
			( enemyAlliance		&& enemyAlliance->GetMasterGuild()		!= enemyGuild		)	)
		{
			return;
		}
	}

	// ��û ���� ����϶�� �˸���
	{
		CObject* friendlyPlayer	= g_pUserTable->FindUser( pmsg->dwData1 );

		if( friendlyPlayer )
		{
			MSG_BYTE message;
			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_WAIT;
			message.bData		= MP_GUILD_WAR_SUGGESTEND;

			friendlyPlayer->SendMsg( &message, sizeof( message ) );
		}
	}
	
	// ���� ���� ���� �޽��� ������
	{
		CObject* enemyPlayer	= g_pUserTable->FindUser( pmsg->dwData2 );

		if( enemyPlayer )
		{
			MSG_DWORD message;
			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_SUGGESTEND;
			message.dwData		= friendlyGuild->GetIdx();

			enemyPlayer->SendMsg( &message, sizeof( message ) );
		}
	}
}


void CGuildWarManager::SuggestEndAccept( void* pMsg )
{
	// ���ǻ� ������ ���� �Ʊ�, �޾Ƶ��̴� ���� �����̶� ����

	MSG_DWORD*	pmsg				= ( MSG_DWORD* )pMsg;
	CPlayer*	friendlyGuildMaster	= ( CPlayer* )	g_pUserTable->FindUser( pmsg->dwObjectID );

	if( !	friendlyGuildMaster ||
			friendlyGuildMaster->GetGuildMemberRank() != GUILD_MASTER )
	{
		return;	
	}

	const DWORD friendlyGuildIndex	= friendlyGuildMaster->GetGuildIdx();
	const DWORD enemyGuildIndex		= pmsg->dwData;
	CGuild*		friendlyGuild		= GUILDMGR->GetGuild( friendlyGuildIndex );
	CGuild*		enemyGuild			= GUILDMGR->GetGuild( enemyGuildIndex );

	if( !	friendlyGuild	||
		!	enemyGuild		||
		!	IsGuildWar( friendlyGuildIndex, enemyGuildIndex ) )
	{
		return;
	}

	// �����ε� ������ ��尡 �ƴ� ��� ��ŷ �Ǵ� Ŭ���̾�Ʈ �˻� ����
	{
		CGuildUnion* friendlyAlliance	= GUILDUNIONMGR->GetUnion( friendlyGuild->GetUnionIndex() );
		CGuildUnion* enemyAlliance		= GUILDUNIONMGR->GetUnion( enemyGuild->GetUnionIndex() );

		if( ( friendlyAlliance	&& friendlyAlliance->GetMasterGuild()	!= friendlyGuild	)	||
			( enemyAlliance		&& enemyAlliance->GetMasterGuild()		!= enemyGuild		)	)
		{
			return;
		}
	}

	// ���ǻ� ������ ��û�� ���� �Ʊ�, ��û���� ���� �������� ����
	Alliance friendlyAlliance;
	Alliance enemyAlliance;

	// ������ ��� �ش� ��带 ��� �ڷᱸ���� ��´�. 
	{
		// �Ʊ�
		{
			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( friendlyGuild->GetUnionIndex() );

			if( alliance )
			{
				// ������ �ִµ��� ������ ��尡 �ƴ� ��� ����
				if( alliance->GetMasterGuild() != friendlyGuild )
				{
					return;
				}

				AddGuildToGroup( alliance, friendlyAlliance );
			}

			friendlyAlliance[ friendlyGuild ] = GetWarMoney( friendlyGuildIndex, enemyGuildIndex );
		}

		// ����
		{
			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( enemyGuild->GetUnionIndex() );

			if( alliance )
			{
				// ������ �ִµ��� ������ ��尡 �ƴ� ��� ����
				if( alliance->GetMasterGuild() != enemyGuild )
				{
					return;
				}

				AddGuildToGroup( alliance, enemyAlliance );
			}

			enemyAlliance[ enemyGuild ] = GetWarMoney( friendlyGuildIndex, enemyGuildIndex );
		}
	}
	
	// �������� �ִٸ� ��ȯ������
	{
		const DWORD money = GetWarMoney( friendlyGuildIndex, enemyGuildIndex );

		if( money )
		{
			SendMoneyMsg( friendlyGuildMaster, NULL, money, MP_GUILD_WAR_ADDMONEY );

			CPlayer* enemyGuildMaster = ( CPlayer* )g_pUserTable->FindUser( enemyGuild->GetMasterIdx() );

			if( enemyGuildMaster )
			{
				SendMoneyMsg( enemyGuildMaster, NULL, money, MP_GUILD_WAR_ADDMONEY );
			}
			else
			{
				SendToAgentAddMoneyMsg( enemyGuild->GetMasterIdx(), money );
			}
		}
	}
	
	// �ڷᱸ���� ��ȯ�ϸ�, ������ ������ �����Ų��.
	for( Alliance::const_iterator it = friendlyAlliance.begin(); friendlyAlliance.end() != it; ++it )
	{
		// ���� ����
		CGuild* friendly = it->first;

		if( ! friendly )
		{
			ASSERT( 0 );
			continue;
		}

		const DWORD friendlyGuildIndex = friendly->GetIdx();

		for( Alliance::const_iterator inner = enemyAlliance.begin(); enemyAlliance.end() != inner; ++inner )
		{
			CGuild* enemy = inner->first;

			if( ! enemy )
			{
				ASSERT( 0 );
				continue;
			}

			const DWORD enemyGuildIndex = enemy->GetIdx();

			// ����� ������ �ش� �ʿ� �˸���
			{
				MSG_DWORD2 message;
				message.Category	= MP_GUILD_WAR;
				message.Protocol	= MP_GUILD_WAR_END;
				message.dwData1		= 2;		// 2 = draw

				message.dwData2		= enemyGuildIndex;
				friendly->SendMsgToAll( &message, sizeof( message ) );

				message.dwData2		= friendlyGuildIndex;
				enemy->SendMsgToAll( &message, sizeof( message ) );
			}

			// ������Ʈ�� ���� �ٸ� �ʿ��Ե� ������
			{
				MSG_DWORD2 message;
				message.Category	= MP_GUILD_WAR;
				message.Protocol	= MP_GUILD_WAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP;
				message.dwData1		= friendlyGuildIndex;
				message.dwData2		= enemyGuildIndex;

				g_Network.Send2AgentServer( (char*)&message, sizeof( message ) );
			}

			DeleteGuildWar( friendlyGuildIndex, enemyGuildIndex );
			UnregistGuildWar( friendlyGuildIndex, enemyGuildIndex );

			UpdateGuildWarRecord( TRUE, 2, friendlyGuildIndex );
			UpdateGuildWarRecord( TRUE, 2, enemyGuildIndex );
		}
	}

	//SendToAllUser( 1, friendlyGuild->GetGuildName(), enemyGuild->GetGuildName() );
}


void CGuildWarManager::SuggestEndAcceptNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	DWORD dwGuildIdx1 = pmsg->dwData1;
	DWORD dwGuildIdx2 = pmsg->dwData2;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuild( dwGuildIdx1 );
	CGuild* pGuild2 = GUILDMGR->GetGuild( dwGuildIdx2 );

	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_END;
	Msg.dwData1 = 2;		// 2 = draw

	// send guild1
	Msg.dwData2 = dwGuildIdx2;
	pGuild1->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guild2
	Msg.dwData2 = dwGuildIdx1;
	pGuild2->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );	

	// unregist
	UnregistGuildWar( dwGuildIdx1, dwGuildIdx2 );

	// guildfieldwarrecord
	UpdateGuildWarRecord( FALSE, 2, dwGuildIdx1 );
	UpdateGuildWarRecord( FALSE, 2, dwGuildIdx2 );

	// registend
	//RegistEnd( dwGuildIdx1, dwGuildIdx2 );
}

void CGuildWarManager::SuggestEndDeny( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild == NULL )	return;

	pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuild->GetMasterIdx() );
	if( pPlayer )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_WAR;
		Msg.Protocol = MP_GUILD_WAR_SUGGESTEND_DENY;
		Msg.dwData = dwGuildIdx1;

		pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else
	{
		MSG_DWORD2 Notify;
		Notify.Category = MP_GUILD_WAR;
		Notify.Protocol = MP_GUILD_WAR_SUGGESTEND_DENY_NOTIFY_TOMAP;
		Notify.dwData1 = pGuild->GetMasterIdx();
		Notify.dwData2 = dwGuildIdx1;

		g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );
	}
}

void CGuildWarManager::SuggestEndDenyNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( pPlayer == NULL )	return;	

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_SUGGESTEND_DENY;
	Msg.dwData = pmsg->dwData2;

	pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
}

void CGuildWarManager::Surrender( void* pMsg )
{
	const MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CGuild* winnerGuild = GUILDMGR->GetGuild( pmsg->dwData );
	CGuild* loserGuild	= GUILDMGR->GetGuildFromMasterIdx( pmsg->dwObjectID );

	if( !	winnerGuild	||
		!	loserGuild	||
        !	IsGuildWar( winnerGuild->GetIdx(), loserGuild->GetIdx() ) )
	{
		return;
	}

	// �����ε� ������ ��尡 �ƴ� ��� ��ŷ �Ǵ� Ŭ���̾�Ʈ �˻� ����
	{
		CGuildUnion* winnerAlliance	= GUILDUNIONMGR->GetUnion( winnerGuild->GetUnionIndex() );
		CGuildUnion* loseAlliance	= GUILDUNIONMGR->GetUnion( loserGuild->GetUnionIndex() );

		if( ( winnerAlliance	&& winnerAlliance->GetMasterGuild()	!= winnerGuild	)	||
			( loseAlliance		&& loseAlliance->GetMasterGuild()	!= loserGuild	)	)
		{
			return;
		}
	}

	// ������ ����
	{
		const CGuildManager::ScoreSetting& setting = GUILDMGR->GetScoreSetting();
		
		const DWORD money = DWORD( setting.mWinMoneyRate * 2.0f * GetWarMoney( winnerGuild->GetIdx(), loserGuild->GetIdx() ) );

		if( money )
		{
			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( winnerGuild->GetMasterIdx() );

			if( player )
			{
				SendMoneyMsg( player, 0, money, MP_GUILD_WAR_ADDMONEY );
			}
			else
			{
				SendToAgentAddMoneyMsg( winnerGuild->GetMasterIdx(), money );
			}
		}
	}

	// �׺��Ҷ� �ٸ� ����� ����Ʈ�� �ϰ������� �������־�� �Ѵ�. �ϴ� ��Ȯ�� ���� ��� ���� 
	// DB ������ �䱸�� �� ����� ���� ó���ؾ��Ѵ�. ó�� �� �ʰ� ������Ʈ�ǹǷ� ���� ������
	GuildSelectForRewardByWar( winnerGuild->GetIdx(), winnerGuild->GetUnionIndex(), loserGuild->GetIdx(), loserGuild->GetUnionIndex() );
}


void CGuildWarManager::SurrenderNotifyToMap( void* pMsg )
{
	const MSG_GUILD_WAR_RESULT*	pmsg				= ( MSG_GUILD_WAR_RESULT* )pMsg;
	const DWORD					winnnerGuildIndex	= pmsg->mWinner.mGuildIndex;
	const DWORD					loserGuildIndex		= pmsg->mLoser.mGuildIndex;

	MSG_DWORD4					message;
	{
		message.Category = MP_GUILD_WAR;
		message.Protocol = MP_GUILD_WAR_SURRENDER;
	}

	// �¸� ��忡�� �޽��� ����
	{
		CGuild* guild	= GUILDMGR->GetGuild( winnnerGuildIndex );

		if( guild )
		{
			const MSG_GUILD_WAR_RESULT::Data& data = pmsg->mWinner;

			message.dwData1 = 1;	// 1 = win
			message.dwData2 = loserGuildIndex;
			message.dwData3	= data.mScoreValue;
			message.dwData4	= DWORD( data.mScoreVariation );

			guild->SendMsgToAll( &message, sizeof( message ) );
		}
	}

	// �й� ��忡�� ����
	{
		CGuild* guild	= GUILDMGR->GetGuild( loserGuildIndex );

		if( guild )
		{
			const MSG_GUILD_WAR_RESULT::Data& data = pmsg->mLoser;

			message.dwData1 = 0;	// 0 = surrender, 
			message.dwData2 = winnnerGuildIndex;
			message.dwData3	= data.mScoreValue;
			message.dwData4	= DWORD( data.mScoreVariation );

			guild->SendMsgToAll( &message, sizeof( message ) );
		}
	}

	UnregistGuildWar( winnnerGuildIndex, loserGuildIndex );
//	PenaltyForEndNotifyToMap( 1, loserGuildIndex );

	UpdateGuildWarRecord( FALSE, 0, loserGuildIndex );
	UpdateGuildWarRecord( FALSE, 1, winnnerGuildIndex );
}

void CGuildWarManager::AddMoneyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( !pPlayer )	return;

	SendMoneyMsg( pPlayer, NULL, pmsg->dwData2, MP_GUILD_WAR_ADDMONEY );
}

void CGuildWarManager::PenaltyForEnd( DWORD dwKind, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );
	if( pGuildLoser == NULL )	return;

	CPlayer* pPlayer = NULL;
	switch( dwKind )
	{
	case 0:		// fight
		{
// �ӽ÷� ����...---------------------------------------------------------------------------
/*			pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildLoser->GetMasterIdx() );
			if( pPlayer )
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );
				EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.1);
				pPlayer->ReduceExpPoint( minusExp, eExpLog_LosebyGuildWar );
			}
			*/
//-------------------------------------------------------------------------------------------
			// level down
/*			pGuildLoser->LevelDown();

			// dbupdate
			GuildLevelUp( pGuildLoser->GetInfo().GuildIdx, pGuildLoser->GetLevel() );

			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_LEVELDOWN;
			Msg.bData = pGuildLoser->GetLevel();
			pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );
			*/
		}
		break;
	case 1:		// surrend
		{
// �ӽ÷� ����...---------------------------------------------------------------------------
/*			pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildLoser->GetMasterIdx() );
			if( pPlayer )
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );
				EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.05);
				pPlayer->ReduceExpPoint( minusExp, eExpLog_LosebyGuildWar );
			}
			*/
//-------------------------------------------------------------------------------------------
		}
		break;
	}
}

void CGuildWarManager::PenaltyForEndNotifyToMap( DWORD dwKind, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );
	if( pGuildLoser == NULL )	return;

	switch( dwKind )
	{
	case 0:		// fight
		{
			// level down
/*			pGuildLoser->LevelDown();

			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_LEVELDOWN;
			Msg.bData = pGuildLoser->GetLevel();
			pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );
			*/
		}
		break;
	case 1:		// surrend
		{
		}
		break;
	}
}

void CGuildWarManager::SendDeclareNackMsg( DWORD dwReceiver, DWORD dwState )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_DECLARE_NACK;
	Msg.dwData1 = dwReceiver;
	Msg.dwData2 = dwState;

	g_Network.Broadcast2AgentServer( (char*)&Msg, sizeof(Msg) );
}

void CGuildWarManager::SendMoneyMsg( CPlayer* pPlayer1, CPlayer* pPlayer2, DWORD dwMoney, BYTE Protocol )
{
	if( !dwMoney )	return;

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = Protocol;
	Msg.dwData = dwMoney;
	if( pPlayer1 )
	{
		if( Protocol == MP_GUILD_WAR_ADDMONEY )
			pPlayer1->SetMoney( dwMoney, MONEY_ADDITION );
		else if( Protocol == MP_GUILD_WAR_REMOVEMONEY )
			pPlayer1->SetMoney( dwMoney, MONEY_SUBTRACTION );
		pPlayer1->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	if( pPlayer2 )
	{
		if( Protocol == MP_GUILD_WAR_ADDMONEY )
			pPlayer2->SetMoney( dwMoney, MONEY_ADDITION );
		else if( Protocol == MP_GUILD_WAR_REMOVEMONEY )
			pPlayer2->SetMoney( dwMoney, MONEY_SUBTRACTION );
		pPlayer2->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
}

void CGuildWarManager::SendToAgentAddMoneyMsg( DWORD dwPlayerIdx, DWORD dwMoney )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_WAR;
	Msg.Protocol = MP_GUILD_WAR_ADDMONEY_TOMAP;
	Msg.dwData1 = dwPlayerIdx;
	Msg.dwData2 = dwMoney;

	g_Network.Send2AgentServer( (char*)&Msg, sizeof(Msg) );
}


/*
void CGuildWarManager::Declare( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
//	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	CPlayer* pReceiver = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
	if( pReceiver == NULL )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild* pGuild2 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;
	if( pGuild1->GetLevel() < 3 )	
	{
		SendDeclareNackMsg( pmsg->dwData1, 0 );
		return;
	}
	if( pGuild2->GetLevel() < 3 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 1 );
		return;
	}
	if( pGuild1->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 2 );
		return;
	}
	if( pGuild2->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 3 );
		return;
	}
	int rt = CanGuildWar( pGuild2->GetIdx(), pGuild1->GetIdx() );
	if( rt != 0 )
	{
		SendDeclareNackMsg( pmsg->dwData1, rt );
		return;
	}	
	if( IsGuildWar( pGuild1->GetInfo().GuildIdx, pGuild2->GetInfo().GuildIdx ) )
		return;

	SEND_GUILD_CREATE_NOTIFY message;
	message.Category	= MP_GUILD_WAR;
	message.Protocol	= MP_GUILD_WAR_DECLARE;
	message.GuildInfo	= pGuild1->GetInfo();
	pReceiver->SendMsg( (MSGBASE*)&message, sizeof(message) );
}
*/

void CGuildWarManager::SendToAllUser( DWORD dwFlag, const char* pGuildName1, const char* pGuildName2 )
{
	MSG_NAME2 message;
	message.Category = MP_GUILD_WAR;
	message.Protocol = MP_GUILD_WAR_RESULT_TOALLUSER;
	message.dwObjectID = dwFlag;
	SafeStrCpy( message.str1, pGuildName1, sizeof( message.str1 ) );
	SafeStrCpy( message.str2, pGuildName2, sizeof( message.str2 ) );

	g_Network.Broadcast2AgentServer( (char*)&message, sizeof(message) );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CGFWCheck::CGFWCheck()
//{
//	m_dwGuildIdx = 0;
//	m_GFWDenyTable.Initialize( 50 );
//	m_GFWEndTable.Initialize( 20 );
//}
//
//CGFWCheck::~CGFWCheck()
//{
//	Release();
//}
//
//void CGFWCheck::Release()
//{
//	sGFWTIME* pData = NULL;
//	m_GFWDenyTable.SetPositionHead();
//	while( pData = m_GFWDenyTable.GetData() )
//		delete pData;
//	m_GFWDenyTable.RemoveAll();
//
//	pData = NULL;
//	m_GFWEndTable.SetPositionHead();
//	while( pData = m_GFWEndTable.GetData() )
//		delete pData;
//	m_GFWEndTable.RemoveAll();
//}
//
//int CGFWCheck::CanGuildWar( DWORD dwEnemyGuildIdx )
//{
//	stTIME CurTime;
//	CurTime.value = GetCurTime();
//	sGFWTIME* pDenyTime = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
//	sGFWTIME* pEndTime = m_GFWEndTable.GetData( dwEnemyGuildIdx );
//
//	if( pDenyTime && pEndTime == NULL )
//	{
//		if( CurTime > pDenyTime->Time )
//		{
//			pDenyTime->Time = CurTime;
//			pDenyTime->dwCount = 0;
//			return 0;
//		}
//		else if( pDenyTime->dwCount > 2 )
//			return 4;
//		else
//			return 0;		
//	}
//	else if( pDenyTime == NULL && pEndTime )
//	{
//		if( CurTime > pEndTime->Time )	return 0;
//		else							return 5;
//	}
//	else if( pDenyTime && pEndTime )
//	{
//		if( CurTime > pEndTime->Time )	return 0;
//		else							return 5;
//
//		if( CurTime > pDenyTime->Time )
//		{
//			pDenyTime->Time = CurTime;
//			pDenyTime->dwCount = 0;
//			return 0;
//		}
//		else if( pDenyTime->dwCount > 2 )
//			return 4;
//		else
//			return 0;		
//	}
//
//	return 0;	// can
//}
//
//void CGFWCheck::RegistDenyTime( DWORD dwEnemyGuildIdx )
//{
//	sGFWTIME* pData = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
//	if( !pData )
//	{
//		pData = new sGFWTIME;
//		pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
//		m_GFWDenyTable.Add( pData, dwEnemyGuildIdx );
//
//		stTIME time;
//		time.SetTime( 0, 0, 1, 0, 0, 0 );
//		pData->Time.value = GetCurTime();
//		pData->Time += time;
//
//		pData->dwCount = 1;
//	}
//	else
//	{
//		++pData->dwCount;		
//	}
//}
//
//void CGFWCheck::RegistEndTime( DWORD dwEnemyGuildIdx )
//{
//	sGFWTIME* pData = m_GFWEndTable.GetData( dwEnemyGuildIdx );
//	if( !pData )
//	{
//		pData = new sGFWTIME;
//		pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
//		m_GFWEndTable.Add( pData, dwEnemyGuildIdx );
//	}
//
//	stTIME time;
//	time.SetTime( 0, 0, 1, 0, 0, 0 );
//	pData->Time.value = GetCurTime();
//	pData->Time += time;
//
//	sGFWTIME* pDeny = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
//	if( pDeny )
//	{
//		pDeny->Time.SetTime( 0, 0, 0, 0, 0, 0 );
//		pDeny->dwCount = 0;
//	}
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CGuildWarManager::AddGuildToGroup( CGuildUnion* guildUnion, Alliance& friendly )
{
	if( ! guildUnion )
	{
		return;
	}

	cPtrList ptrList;

	guildUnion->GetGuildListInUnion( &ptrList );

	for(	PTRLISTPOS position = ptrList.GetHeadPosition();
			position; )
	{
		CGuild* guild = ( CGuild* )ptrList.GetNext( position );
		ASSERT( guild );

		if( guild )
		{
			friendly[ guild ] = 0;
		}
	}
}


void CGuildWarManager::AddEnemyToFriendly( Alliance& friendlyMap, Alliance& enemyMap )
{
	MSG_GUILD_LIST	messageToClient;
	messageToClient.Category	= MP_GUILD_WAR;
	messageToClient.Protocol	= MP_GUILD_WAR_START;
	messageToClient.mSize		= 0;

	// ��� 1�� ��(���2) ��ü ������ �ִ´�
	for( Alliance::iterator it = enemyMap.begin(); enemyMap.end() != it; ++it )
	{
		CGuild*			enemy = it->first;
		const MONEYTYPE	money = it->second;

		if( ! enemy )
		{
			ASSERT( 0 );
			continue;
		}

		MSG_GUILD_LIST::Data&	dest	= messageToClient.mData[ messageToClient.mSize++ ];
		const GUILDINFO&		source	= enemy->GetInfo();
		
		// ����� ����ǳ� �� Ȯ������. �� ������ �ڵ�
		memcpy( &dest, &source, sizeof( source ) );

		dest.MarkName = money;

		CGuildUnion* allianace = GUILDUNIONMGR->GetUnion( source.UnionIdx );

		if( allianace )
		{
			SafeStrCpy( dest.mUnionName, allianace->GetName(), sizeof( dest.mUnionName ) );
		}
		else
		{
			dest.mUnionName[ 0 ] = 0;
		}
	}

	MSG_DWORD3 messageToAgent;
	messageToAgent.Category	= MP_GUILD_WAR;
	messageToAgent.Protocol	= MP_GUILD_WAR_START_NOTIFY_TOMAP;

	// ��� 2�� ��, �� ��� 1�� �� ���ο��� ������.
	for( Alliance::iterator it = friendlyMap.begin(); friendlyMap.end() != it; ++it )
	{
		CGuild*			friendly		= it->first;
		const MONEYTYPE	friendlyMoney	= it->second;
		
		if( ! friendly )
		{
			ASSERT( 0 );
			continue;
		}
		
		friendly->SendMsgToAll( &messageToClient, messageToClient.GetSize() );

		// �Ʊ� ��� ������ ��� ������ ������ ����ϰ� �Ѵ�.
		for( Alliance::iterator inner = enemyMap.begin(); enemyMap.end() != inner; ++inner )
		{
			CGuild*			enemy		= inner->first;
			const MONEYTYPE	enemyMoney	= inner->second;

			if( ! enemy )
			{
				ASSERT( 0 );
				continue;
			}

			// �� ��� ��� �������� ���ϰ� �������� ���� ������Ʈ�ؾ��Ѵ�. �׷��� ���� ���� �������� �� ����ڰ� �ƴϴ�.
			const MONEYTYPE money = ( friendlyMoney && enemyMoney ? friendlyMoney : 0 ) ;

			InsertGuildWar( friendly->GetIdx(), enemy->GetIdx(), money );
			RegistGuildWar( friendly->GetIdx(), enemy->GetIdx(), money );

			// �ٸ� �ʿ��� ����ϰ� �Ѵ�.
			{
				messageToAgent.dwData1		= friendly->GetIdx();
				messageToAgent.dwData2		= enemy->GetIdx();
				messageToAgent.dwData3		= money;

				g_Network.Send2AgentServer( (char*)&messageToAgent, sizeof( messageToAgent ) );	
			}
		}
	}
}


void CGuildWarManager::GetEnemy( DWORD guildIndex, std::list< sGFWENEMY* >& enemyList )
{
	CGuildWar* data = m_GuildWarTable.GetData( guildIndex );

	if( data )
	{
		data->GetEnemy( enemyList );
	}
}


void CGuildWarManager::AddUnionEnemyToGuild( CGuildUnion* friendlyUnion, CGuild* friendlyGuild )
{
	MSG_GUILD_LIST	messageToFriendly;
	MSG_GUILD_LIST	messageToEnemey;
	{
		messageToFriendly.Category	= MP_GUILD_WAR;
		messageToFriendly.Protocol	= MP_GUILD_WAR_START;
		messageToFriendly.mSize		= 0;

		messageToEnemey.Category	= MP_GUILD_WAR;
		messageToEnemey.Protocol	= MP_GUILD_WAR_START;
		messageToEnemey.mSize		= 1;

		MSG_GUILD_LIST::Data&	dest	= messageToEnemey.mData[ 0 ];
		const GUILDINFO&		source	= friendlyGuild->GetInfo();

		// �ùٷ� ����Ǵ��� �� �˻��� ��
		memcpy( &dest, &source, sizeof( source ) );

		// �߰��� ���������Ƿ� �������� ����
		dest.MarkName = 0;

		CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( source.UnionIdx );

		if( alliance )
		{
			SafeStrCpy( dest.mUnionName, alliance->GetName(), sizeof( dest.mUnionName ) );
		}
		else
		{
			dest.mUnionName[ 0 ] = 0;
		}
	}	

	const DWORD				friendlyGuildIndex = friendlyGuild->GetIdx();
	std::list< sGFWENEMY* > enemeyList;

	GetEnemy( friendlyUnion->GetMasterGuildIdx(), enemeyList );

	// ���� ��� ����� �޽����� ���, ���� ��� ���� �� ���� �Ͽ��� ����ϰ� �Ѵ�.
	for( std::list< sGFWENEMY* >::const_iterator it = enemeyList.begin(); enemeyList.end() != it; ++it )
	{
		const sGFWENEMY*	enemyData		= *it;
		const DWORD			enemyGuildIndex = enemyData->dwEnemyGuildIdx;
		CGuild*				enemyGuild		= GUILDMGR->GetGuild( enemyGuildIndex );

		if( ! enemyGuild )
		{
			ASSERT( 0 );
			continue;
		}

		// ���� ��� ������ ��´�
		// ���� ��带 ������ ����ϵ��� ��� ���� ��忡�� ������.
		{
			MSG_GUILD_LIST::Data&	dest	= messageToFriendly.mData[ messageToFriendly.mSize++ ];
			const GUILDINFO&		source	= enemyGuild->GetInfo();

			// ����� ����ǳ� �׽�Ʈ�� ��
			memcpy( &dest, &source, sizeof( source ) );

			// �߰��� �����Ͽ� �������� ����
			dest.MarkName = 0;

			CGuildUnion* alliance = GUILDUNIONMGR->GetUnion( source.UnionIdx );

			if( alliance )
			{
				SafeStrCpy( dest.mUnionName, alliance->GetName(), sizeof( dest.mUnionName ) );
			}
			else
			{
				dest.mUnionName[ 0 ] = 0;
			}

			enemyGuild->SendMsgToAll( &messageToEnemey, messageToEnemey.GetSize() );
		}

		// DB�� �޸𸮿��� �����Ѵ�
		{
			InsertGuildWar( friendlyGuildIndex, enemyGuildIndex, 0 );
			RegistGuildWar( friendlyGuildIndex, enemyGuildIndex, 0 );
		}
		
		// �ٸ� �ʿ��� ����ϰ� �Ѵ�.
		{
			MSG_DWORD3 message;
			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_START_NOTIFY_TOMAP;
			message.dwData1		= friendlyGuildIndex;
			message.dwData2		= enemyGuildIndex;
			message.dwData3		= 0;

			g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );	
		}
	}

	if( messageToFriendly.mSize )
	{
		// ���� ��� ����� �� ���� �� ���� ��忡�Ե� ������
		friendlyGuild->SendMsgToAll( &messageToFriendly, messageToFriendly.GetSize() );
	}
}


void CGuildWarManager::RemoveUnionEnemyFromGuild( CGuildUnion* friendlyUnion, CGuild* friendlyGuild )
{
	std::list< sGFWENEMY* > enemeyList;
	GetEnemy( friendlyUnion->GetMasterGuildIdx(), enemeyList );

	const DWORD	friendlyGuildIndex = friendlyGuild->GetIdx();

	MSG_DWORD2 messageToFriendy;
	{
		messageToFriendy.Category	= MP_GUILD_WAR;
		messageToFriendy.Protocol	= MP_GUILD_WAR_END;
		messageToFriendy.dwData1	= 2;
	}

	MSG_DWORD2 messageToEnemy;
	{
		messageToEnemy.Category	= MP_GUILD_WAR;
		messageToEnemy.Protocol	= MP_GUILD_WAR_END;
		messageToEnemy.dwData1	= 2;	// draw
		messageToEnemy.dwData2	= friendlyGuildIndex;
	}

	// ��� �� ��Ͽ��� �ش� ��带 ������ �����ϵ��� �Ѵ�.
	for( std::list< sGFWENEMY* >::const_iterator it = enemeyList.begin(); enemeyList.end() != it; ++it )
	{
		const sGFWENEMY*	enemyData		= *it;
		const DWORD			enemyGuildIndex = enemyData->dwEnemyGuildIdx;
		CGuild*				enemyGuild		= GUILDMGR->GetGuild( enemyGuildIndex );

		if( ! enemyGuild )
		{
			ASSERT( 0 );
			continue;
		}

		// �� ��忡�� �ش� ������ �ο��� ���� ���� �˸���
		{
			enemyGuild->SendMsgToAll( &messageToEnemy, sizeof( messageToEnemy ) );
		}

		// �Ʊ����Ե� �˸���
		{
			messageToFriendy.dwData2 = enemyGuildIndex;

			friendlyGuild->SendMsgToAll( &messageToFriendy, sizeof( messageToFriendy ) );
		}

		// �ٸ� �ʿ��� �˸���(�������� ǥ�õ� ���̴�
		{
			MSG_DWORD2 message;
			message.Category	= MP_GUILD_WAR;
			message.Protocol	= MP_GUILD_WAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP;
			message.dwData1		= friendlyGuildIndex;
			message.dwData2		= enemyGuildIndex;

			g_Network.Send2AgentServer( (char*)&message, sizeof( message ) );
		}

		// DB �� �޸� ����
		{
			DeleteGuildWar( friendlyGuildIndex, enemyGuildIndex );
			UnregistGuildWar( friendlyGuildIndex, enemyGuildIndex );

			UpdateGuildWarRecord( TRUE, 2, enemyGuildIndex );
			UpdateGuildWarRecord( TRUE, 2, friendlyGuildIndex );
		}

		//SendToAllUser( 1, friendlyGuild->GetGuildName(), enemyGuild->GetGuildName() );
	}
}
