#include "stdafx.h"
#include "GuildManager.h"
#include "Guild.h"
#include "GuildTournamentMgr.h"
#include "GuildUnionManager.h"
#include "GuildFieldWarMgr.h"
#include "GuildScore.h"
#include "GuildMark.h"

#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "ItemManager.h"
#include "SiegeWarMgr.h"
#include "Monster.h"
// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.15
#include "../hseos/Family/SHFamilyManager.h"
// E �йи� �߰� added by hseos 2007.07.15
// 080417 LUJ, ��� ��ų ȹ������ ����
#include "../[cc]skill/Server/Manager/SkillManager.h"
#include "../[cc]skill/Client/Info/ActiveSkillInfo.h"


CGuildManager::CGuildManager()
{
	m_GuildRoomHashTable.Initialize(256);
	m_GuildMarkTable.Initialize(128);
	//m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	//m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM); //������ ũ�� ��� ������?MAX_ITEM_OPTION_NUM*100-->MAX_ITEM_OPTION_NUM ������ ���� �Ͱ��� �ٿ���.
	//LoadLevelUpInfo();

	//memset(&m_GuildPointRule,0,sizeof(GUILDPOINT_RULE));
	//m_CHECKTIMELEN_GuildHuntedMonsterCounter = 0;
	//m_dwLastDBUpdateTime = gCurTime;

	////LoadGuildPointRule();
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//m_wCurDateCheckForConverCount = st.wDay;
	//m_CheckTimeForGuildPlustimeEnd = gCurTime;

	////LoadGuildPointPlustimeList();

	LoadScript();
}

CGuildManager::~CGuildManager()
{
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while(pGuild = m_GuildRoomHashTable.GetData())
		delete pGuild;
	//m_GuildRoomHashTable.RemoveAll();

	CGuildMark* pMarkInfo;
	m_GuildMarkTable.SetPositionHead();
	while(pMarkInfo = m_GuildMarkTable.GetData())
		delete pMarkInfo;
	//m_GuildMarkTable.RemoveAll();

	//ITEM_OPTION* pOInfo = NULL;
	//m_ItemOptionList.SetPositionHead();
	//while(pOInfo = m_ItemOptionList.GetData())
	//	delete pOInfo;
	//m_ItemOptionList.RemoveAll();

	//ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	//m_ItemRareOptionList.SetPositionHead();
	//while(pRareInfo = m_ItemRareOptionList.GetData())
	//	delete pRareInfo;
	//m_ItemRareOptionList.RemoveAll();

	//if(m_pGuildPlustimeInfo)
	//{
	//	delete[] m_pGuildPlustimeInfo;
	//	m_pGuildPlustimeInfo = NULL;
	//}

	//SAFE_DELETE( mScore );

	for(	GuildMap::iterator it = mGuildScoreMap.begin();
		mGuildScoreMap.end() != it;
		++it )
	{
		CGuildScore* guild = it->second;

		SAFE_DELETE( guild );
	}
}

void CGuildManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_GUILD_CREATE_SYN:
		{
			SEND_GUILD_CREATE_SYN* pmsg = (SEND_GUILD_CREATE_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CreateGuildSyn(pPlayer, pmsg->GuildName, pmsg->Intro);
		}
		break;
	case MP_GUILD_BREAKUP_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			if( GUILDWARMGR->IsGuildWar( pPlayer->GetGuildIdx() ) )
				return;
			BreakUpGuildSyn(pPlayer);
		}
		break;
	case MP_GUILD_DELETEMEMBER_SYN:
		{
			const MSG_DWORD*	message				= (MSG_DWORD*)pMsg;
			const DWORD			playerIndex			= message->dwObjectID;
			const DWORD			kickedPlayerIndex	= message->dwData;

			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( playerIndex );

			// �ڱ� �ڽ� ���� ����
			if( !	player							||
					playerIndex	== kickedPlayerIndex )
			{
				return;
			}

			const BYTE rank = player->GetGuildMemberRank();

			// Ż�� ������ ���� ��ũ�ΰ�
			if(	GUILD_MASTER		!= rank &&
				GUILD_VICEMASTER	!= rank )
			{
				SendNackMsg( player, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NothaveAuthority );
				break;
			}
			// 071213 LUJ, �ǹ̾��� �ڵ� player �����Ͱ� ��ȿ���� üũ�ϴ� ������ ��
			// �ʿ� ���ӵǾ� ���� ���� ���·� ��û�� �Դ°� üũ
			///else if( ! GetGuildScore( player->GetGuildIdx() ) )
			//{
			//	SendNackMsg( player, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NotMember );
			//	break;
			//}
			else if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
			{
				SendNackMsg( player, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember );
				break;
			}

			//db update
			GuildDeleteMember( player->GetGuildIdx(), kickedPlayerIndex );
		}
		break;
	case MP_GUILD_ADDMEMBER_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pMaster == 0)
				return;
			AddMemberSyn(pMaster, pmsg->dwData);		
		}
		break;
	case MP_GUILD_INVITE_ACCEPT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			AddMemberResult(pPlayer, pmsg->dwData);
		}
		break;
	case MP_GUILD_INVITE_DENY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData);
			if(pMaster == 0)
				return;
			MSGBASE msg;
			msg.Category = MP_GUILD;
			msg.Protocol = MP_GUILD_INVITE_DENY;
			pMaster->SendMsg(&msg, sizeof(msg));
		}
		break;
	case MP_GUILD_SECEDE_SYN:
		{
			MSGBASE * pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			SecedeSyn(pPlayer);
		}
		break;
	case MP_GUILD_LEVELUP_SYN:
		{
			const MSGBASE*	message	= ( MSGBASE* )pMsg;
			CPlayer*		player	= ( CPlayer* )g_pUserTable->FindUser( message->dwObjectID );

			if( ! player )
			{
				return;
			}

			const DWORD			playerIndex	= player->GetID();
			const DWORD			guildIndex	= player->GetGuildIdx();
			const CGuildScore*	guild		= GetGuildScore( guildIndex  );

			if( !	guild ||
					guild->GetMasterIndex() != playerIndex )
			{				
				ASSERT( 0 && "It guess two case. One is client test is failed. Other is hacking" );
				return;
			}

			const DWORD			nextLevel	= guild->GetLevel() + 1;
			const LevelSetting* setting		= GetLevelSetting( nextLevel );

			if( !	setting												||
					setting->mRequiredMoney > player->GetMoney()		||
					setting->mRequiredMasterLevel > player->GetLevel() )
			{
				ASSERT( 0 && "It guess two case. One is client test is failed. Other is hacking" );
				return;
			}

			// DB�� �۾� ��û: ������ ����Ʈ ���� ���δ� DB���� ���� ���̴�. ����ȭ ����...
			GuildSetLevel(
				guildIndex,
				playerIndex,
				setting->mRequiredMoney,
				setting->mRequiredScore,
				setting->mRequiredQuest,
				setting->mWarehouseSize,
				nextLevel );
		}
		break;
	case MP_GUILD_CHANGERANK_SYN:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			ChangeMemberRank(pPlayer, pmsg->dwData, pmsg->bData);
		}
		break;
	case MP_GUILD_GIVENICKNAME_SYN:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			GiveNickNameSyn(pPlayer, pmsg->TargetIdx, pmsg->NickName);
		}
		break;
	case MP_GUILD_CREATE_NOTIFY_TO_MAP:
		{
			SEND_GUILD_CREATE_NOTIFY* pmsg = (SEND_GUILD_CREATE_NOTIFY*)pMsg;
			CGuild* pGuild = RegistGuild( pmsg->GuildInfo, 0 );
			if(pGuild == 0)
				return;
			
			pGuild->AddMember( &( pmsg->mMaster ) );
		}
		break;
	case MP_GUILD_BREAKUP_NOTIFY_TO_MAP:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			DoBreakUp(pmsg->dwObjectID);
		}
		break;
	case MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			DoDeleteMember(pmsg->dwObjectID, pmsg->dwData, eGuild_Delete, pmsg->Name);
		}
		break;
	case MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP:
		{
			SEND_GUILD_MEMBER_INFO* pmsg = (SEND_GUILD_MEMBER_INFO*)pMsg;
			CGuild* pGuild = GetGuild(pmsg->GuildIdx);
			if(pGuild == 0)
			{
				ASSERTMSG(0, "Different Guild Info!");
				return;
			}
			
			GUILDMEMBERINFO& member = pmsg->MemberInfo;
			
			if(pGuild->AddMember(&member) == FALSE)
			{
				char buf[128];
				sprintf(buf, "Add Notify Failed! GuildIdx : %u, MemberIdx : %u", pmsg->GuildIdx, pmsg->MemberInfo.MemberIdx);
				ASSERTMSG(0, buf);
			}			
		}
		break;
	case MP_GUILD_SECEDE_NOTIFY_TO_MAP:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			DoDeleteMember(pmsg->dwObjectID, pmsg->dwData, eGuild_Secede, "2000.01.01");
		}
		break;
	case MP_GUILD_LEVELUP_NOTIFY_TO_MAP:
		{
			MSG_GUILD_LEVEL_UP_ACK	message;
			memcpy( &message, pMsg, sizeof( message ) );
			message.Protocol		= MP_GUILD_LEVELUP_ACK;

			const DWORD	guildIndex	= message.mGuildIndex;

			{
				CGuildScore* guild = GetGuildScore( guildIndex );

				if( guild )
				{
					guild->SetLevel( guild->GetLevel() + 1 );
					guild->Send( &message, sizeof( message ) );
				}
			}

			{
				CGuild* guild = GetGuild( guildIndex );

				if( guild )
				{
					guild->LevelUp();
				}
			}

			break;
		}
		break;
	case MP_GUILD_CHANGERANK_NOTIFY_TO_MAP:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;			
			CGuild* pGuild = GetGuild(pmsg->dwObjectID);
			if(pGuild == 0)
			{
				char buf[128];
				sprintf(buf, "ChangeRank Notify Failed! GuildIdx: %d", pmsg->dwObjectID);
				ASSERTMSG(0, buf);
				break;
			}
			if(pGuild->ChangeMemberRank(pmsg->dwData, pmsg->bData) == FALSE)
			{
				ASSERTMSG(0, "Notify Change Rank Err, Must Check.");
				break;
			}
		}
		break;
	case MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP:
		{
			MSG_DWORD3 * pmsg = (MSG_DWORD3*)pMsg;
			CGuild* pGuild = GetGuild(pmsg->dwData1);
			if(pGuild == 0)
				return;
			pGuild->SetMemberLevel(pmsg->dwData2, (LEVELTYPE)pmsg->dwData3);
		}
		break;
	case MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->TargetIdx);
			if(pPlayer == 0)
				return;

			pPlayer->SetNickName(pmsg->NickName);

			// SEND TO 
			SEND_GUILD_NICKNAME msg;
			msg.Category = MP_GUILD;
			msg.Protocol = MP_GUILD_GIVENICKNAME_ACK;
			msg.TargetIdx = pmsg->TargetIdx;
			SafeStrCpy(msg.NickName, pmsg->NickName, MAX_GUILD_NICKNAME+1);
			PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));	
		}
		break;
	case MP_GUILD_LOGINFO_NOTIFY_TO_MAP:
		{
			SEND_GUILDMEMBER_LOGININFO * pmsg = (SEND_GUILDMEMBER_LOGININFO *)pMsg;
			CGuild* pGuild = GetGuild(pmsg->dwObjectID);
			if(pGuild == 0)
				return;
			if(pGuild->IsMember(pmsg->MemberIdx) == FALSE)
				return;
			pGuild->SetLogInfo(pmsg->MemberIdx, pmsg->bLogIn);
		}
		break;
		///////////////////////////////////////////////////////////////////////////
		//MARK
	case MP_GUILDMARK_UPDATE_SYN:
		{
			MSG_GUILDMARK_IMG * pmsg	= ( MSG_GUILDMARK_IMG* )pMsg;
			CPlayer*			pPlayer = ( CPlayer* )g_pUserTable->FindUser(pmsg->dwObjectID );

			if( !	pPlayer		||
					pPlayer->GetGuildMemberRank() != GUILD_MASTER )
			{
				break;
			}

			const CGuildScore* guild = GetGuildScore( pPlayer->GetGuildIdx() );

			if( ! guild )
			{
				SendNackMsg( pPlayer, MP_GUILDMARK_UPDATE_NACK, eGuildErr_Mark_NoGuild);
				break;
			}

			const CGuildManager::LevelSetting* setting = GetLevelSetting( guild->GetLevel() );

			if( ! setting ||
				! setting->mGuildMark )
			{
				ASSERT( 0 && "It maybe be hacking" );
				break;
			}

			//RegistMarkSyn(pmsg->GuildIdx, pmsg->imgData, pmsg->dwObjectID);
			GuildMarkRegist( pmsg->GuildIdx, pmsg->imgData, pmsg->dwObjectID );
		}
		break;
	case MP_GUILDMARK_REQUEST_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;

			RequestMark(pPlayer, pmsg->dwData);
		}
		break;
	case MP_GUILDMARK_NOTIFY_TO_MAP:
		{
			MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
			CGuild* pGuild = GetGuild(pmsg->GuildIdx);
			if(pGuild == 0)
				return;
			DoRegistMark(pGuild, pmsg->MarkName, pmsg->imgData);
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_SYN:
		{
			MsgChangeLocationSyn( pMsg );
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP:
		{
			MsgChangeLocationNotifyToMap( pMsg );
		}
		break;
	case MP_GUILD_NOTE_SYN:
		{
			MsgGuildNoteSyn( pMsg );
			
		}
		break;
	case MP_GUILD_NOTICE_SYN:
		{
			MSG_GUILDNOTICE*	message = (MSG_GUILDNOTICE*)pMsg;
			CPlayer*			player = (CPlayer*)g_pUserTable->FindUser( message->dwObjectID );

			if( ! player )
			{
				return;
			}

			const BYTE rank = player->GetGuildMemberRank();

			if( GUILD_MASTER		!= rank &&
				GUILD_VICEMASTER	!= rank )
			{
				return;
			}

			const DWORD		guildIndex	= player->GetGuildIdx();
			CGuildScore*	guild		= GetGuildScore( guildIndex );

			if( ! guild )
			{
				SendNackMsg( player, MP_GUILD_NOTICE_NACK, eGuildErr_NoGuild );
				return;
			}

			GuildUpdateNotice( guildIndex, message->Msg, player->GetID() );

			// ������ ��ε�ĳ��������
			{
				message->Protocol = MP_GUILD_NOTICE_NOTIFY;

				g_Network.Send2AgentServer( ( char* )message, message->GetMsgLength() );
			}
			
			// ��忡�Ե� ������
			{
				message->Protocol = MP_GUILD_NOTICE_ACK;

				guild->Send( message, message->GetMsgLength() );
			}
		}
		break;
	case MP_GUILD_NOTICE_NOTIFY:
		{
			//MsgGuildNoticeNotify(pMsg);

			MSG_GUILDNOTICE* message = ( MSG_GUILDNOTICE* )pMsg;

			CGuildScore* guild = GetGuildScore( message->dwGuildId );

			if( guild )
			{
				message->Protocol	= MP_GUILD_NOTICE_ACK;

				guild->Send( message, message->GetMsgLength() );
			}
		}
		break;
		//SW060713 ���ϻ�
	case MP_GUILD_ADDSTUDENT_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( ! pMaster )
			{
				return;
			}
			
			CGuild*		pGuild		= GetGuild(pMaster->GetGuildIdx());
			const DWORD guestIndex	= pmsg->dwData;
			CPlayer*	pTarget		= (CPlayer*)g_pUserTable->FindUser( guestIndex );

			// �˻�
			{
				MSG_NAME_DWORD message;
				ZeroMemory( &message, sizeof( message ) );
				message.dwObjectID	= pmsg->dwObjectID;
				message.Category	= MP_GUILD;
				message.Protocol	= MP_GUILD_ADDSTUDENT_NACK;

				if( ! pTarget )
				{
					message.dwData	= eGuildErr_AddMember_NotPlayer;
				}
				else if( pTarget->GetGuildIdx() )
				{
					message.dwData	= eGuildErr_AddMember_OtherGuild;
				}
				else if(pTarget->GetLevel() >= GUILD_STUDENT_MAX_LEVEL)
				{
					message.dwData	= eGuildErr_AddStudent_TooHighLevelAsStudent;
				}
				else if( !	pGuild ||
							SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
				{
					message.dwData	= eGuildErr_Err;
				}
				else if(pMaster->GetGuildMemberRank() < GUILD_SENIOR)
				{
					message.dwData	= eGuildErr_AddMember_NothaveAuthority;
				}			
				//else if( pGuild->IsMember( guestIndex ) )
				//{
				//	message.dwData	= eGuildErr_AddMember_AlreadyMember;
				//}
				if( FALSE == pGuild->CanAddStudent() )	//���ϻ� �ο��� �� á�ų�
				{
					message.dwData	= eGuildErr_AddMember_FullMember;
				}

				// ���� 0�� ������ �˻� �����̴�
				if( message.dwData )
				{
					pMaster->SendMsg( &message, sizeof( message ) );
					return;
				}
			}

			{
				MSGBASE message;
				message.Category = MP_GUILD;
				message.Protocol = MP_GUILD_ADDSTUDENT_INVITE;

				pMaster->SendMsg( &message, sizeof( message ) );
			}
			
			{
				SEND_GUILD_INVITE message;
				message.Category	= MP_GUILD;
				message.Protocol	= MP_GUILD_INVITE_AS_STUDENT;
				message.MasterIdx	= pMaster->GetID();

				SafeStrCpy( message.MasterName, pMaster->GetObjectName(), sizeof( message.MasterName ) );
				SafeStrCpy( message.GuildName, pGuild->GetGuildName(), sizeof( message.GuildName ) );

				pTarget->SendMsg( &message, sizeof( message )) ;
			}
		}
		break;
	case MP_GUILD_INVITE_AS_STUDENT_ACCEPT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			AddStudentResult(pPlayer, pmsg->dwData);
		}
		break;
	case MP_GUILD_OPEN_WAREHOUSE_SYN:
		{
			const MSGBASE*	message		= ( MSGBASE* )pMsg;
			const DWORD		playerIndex = message->dwObjectID;
			CPlayer*		player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

			if( player )
			{
				GuildOpenWarehouse( player->GetGuildIdx(), playerIndex, g_pServerSystem->GetMapNum() );
			}

			break;
		}
	case MP_GUILD_CLOSE_WAREHOUSE:
		{
			const MSGBASE*	message		= ( MSGBASE* )pMsg;
			const DWORD		playerIndex = message->dwObjectID;
			CPlayer*		player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

			if( player )
			{
				GuildCloseWarehouse( player->GetGuildIdx(), playerIndex, g_pServerSystem->GetMapNum() );
			}			

			break;
		}
	case MP_GUILD_SET_WAREHOUSE_RANK_SYN:
		{
			const MSG_GUILD_SET_WAREHOUSE_RANK* message		= ( MSG_GUILD_SET_WAREHOUSE_RANK* )pMsg;
			const DWORD							guildIndex	= message->mGuildIndex;
			const DWORD							playerIndex	= message->dwObjectID;
			CPlayer*							player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );
			
			if( !	GetGuildScore( guildIndex )			||
				!	player								||
					player->GetGuildIdx() != guildIndex	)
			{
				ASSERT( 0 && "it doubt with hacking" );
				return;
			}

			const BYTE rank = player->GetGuildMemberRank();

			if( rank != GUILD_MASTER		&&
				rank != GUILD_VICEMASTER	)
			{
				ASSERT( 0 && "it doubt with hacking" );
				return;
			}

			GuildSetWarehouseRank( guildIndex, playerIndex, message->mRank );
			break;
		}
	case MP_GUILD_RETIRE_ACK:
		{
			const MSG_DWORD* message = ( MSG_DWORD* )pMsg;

			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( message->dwObjectID );

			if( !	player	||
					player->GetGuildMemberRank() == GUILD_STUDENT )
			{
				return;
			}

			const DWORD	guildIndex	= player->GetGuildIdx();
			CGuild*		guild		= GUILDMGR->GetGuild( guildIndex );

			// ����: ������ ���δ� Ŭ���̾�Ʈ���� �˻����� �ʴ´�. �������� �߰��� �� ��������

			if( !	guild									||
					guild->GetUnionIndex()					||
					GUILDWARMGR->IsGuildWar( guildIndex ) )
			{
				// Ŭ���̾�Ʈ���� �˻�����
				return;
			}

			GuildChangeMaster( guildIndex, player->GetID(), GUILD_MASTER, GUILD_MEMBER );
			
			break;
		}
	case MP_GUILD_CHANGE_MASTER_TO_AGENT:
		{
			MSG_DWORD2		message;
			memcpy( &message, pMsg, sizeof( message ) );
			message.Protocol				= MP_GUILD_CHANGE_MASTER_TO_USER;

			const DWORD		guildIndex		= message.dwData1;
			const DWORD		newMasterIndex	= message.dwData2;

			{
				CGuild* guild = GetGuild( guildIndex );

				if( guild )
				{
					guild->SendMsgToAll( &message, sizeof( message ) );

					// ���� �������� ����� ȸ������ �����
					{
						const DWORD			oldMasterIndex	= guild->GetMasterIdx();
						GUILDMEMBERINFO*	info			= guild->GetMemberInfo( oldMasterIndex );

						if( info )
						{
							info->Rank = GUILD_MEMBER;
						}

						CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( oldMasterIndex );

						if( player )
						{
							player->SetGuildMemberRank( GUILD_MEMBER );
						}

						guild->UpdateSkill( oldMasterIndex );

						// 071214 LUJ
						LogGuild(
							oldMasterIndex,
							guildIndex,
							eGuildLog_Retire,
							newMasterIndex,
							0 );
					}

					// ���ο� �������� ��޶��� �����ͷ� �ø���
					{
						GUILDMEMBERINFO* info = guild->GetMemberInfo( newMasterIndex );

						if( info )
						{
							info->Rank = GUILD_MASTER;
						}

						CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( newMasterIndex );

						if( player )
						{
							player->SetGuildMemberRank( GUILD_MASTER );
						}

						guild->UpdateSkill( newMasterIndex );
						guild->SetMasterIndex( newMasterIndex );
					}
				}
			}

			{
				CGuildScore* score	= GetGuildScore( guildIndex );

				if( score )
				{
					score->SetMasterIndex( newMasterIndex );
				}
			}

			break;
		}
		// 080225 LUJ, ȸ���� ���� ������ �����Ѵ�
	case MP_GUILD_SET_MEMBER_TO_MAP:
		{
			SEND_GUILD_MEMBER_INFO* message = ( SEND_GUILD_MEMBER_INFO* )pMsg;

			CGuild* guild = GetGuild( message->GuildIdx );

			if( ! guild	)
			{
				break;
			}

			const GUILDMEMBERINFO& member = message->MemberInfo;

			if( ! guild->SetMember( member ) )
			{
				break;
			}

			message->Protocol	= MP_GUILD_SET_MEMBER_ACK;

			guild->SendMsgToAll( message, sizeof( *message ) );
			break;
		}
		// 080417 LUJ, ��� ��ų ȹ��
	case MP_GUILD_ADD_SKILL_SYN:
		{
			const MSG_SKILL_UPDATE* message = ( MSG_SKILL_UPDATE* )	pMsg;
			CPlayer*				player	= ( CPlayer* )			g_pUserTable->FindUser( message->dwObjectID );

			if( ! player )
			{
				break;
			}

			CGuild* guild = GetGuild( player->GetGuildIdx() );
			
			// 080417 LUJ, ���������� �˻�
			if( !	guild									||
					guild->GetMasterIdx() != player->GetID() )
			{
				SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_NoMaster );
				break;
			}

			const SKILL_BASE& skill	= message->SkillBase;

			// 080417 LUJ, ��� ��ų ������ ��ȿ���� �˻�(��ų �������� �ϴ� ���� �������)
			if( ! guild->GetSkillLevel( skill.wSkillIdx ) )
			{
				const LevelSetting* setting = GetLevelSetting( guild->GetLevel() );

				if( !	setting	||
						setting->mSkillSize <= guild->GetSkillSize() )
				{
					SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_Skill_Max );
					break;
				}
			}

			// 080417 LUJ, ��� ������ �ִ� ��ų���� �˻�
			{
				const SkillSettingMap* settingMap = GetSkillSetting( guild->GetLevel() );

				// 080417 LUJ,	�� ������ ���� ������ ���� ��� �ִ� ������ ������ ����
				//				��ų ��ũ��Ʈ�� ��� ���� ������ �ǳʶٴ� ��ȣ�� ���� ��� �߸��� üũ�� �� ���̴�
				if( ! settingMap )
				{
					settingMap = GetMaxLevelSkillSetting();
					
					if( ! settingMap )
					{
						SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_NoScript );
						break;
					}
				}

				SkillSettingMap::const_iterator settingMap_it	= settingMap->find( skill.wSkillIdx );

				if( settingMap->end() == settingMap_it )
				{
					SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_Skill_NotExist );
					break;
				}

				const SkillSetting& setting = settingMap_it->second;

				if( setting.mLevel < skill.Level )
				{
					SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_Skill_InvalidLevel );
					break;
				}
			}

			// 080417 LUJ, ȹ�� ������ ��ų���� �˻�
			{
				cActiveSkillInfo*			activeSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skill.wSkillIdx - 1 + skill.Level );
				const ACTIVE_SKILL_INFO*	skillInfo		= ( activeSkillInfo ? activeSkillInfo->GetSkillInfo() : 0 );

				if( !	skillInfo	||
						skillInfo->TrainMoney > player->GetMoney() )
				{
					SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_InsufficentMoney );
					break;
				}

				// 080417 LUJ, ������ ���� ������ ������ �𸥴�. ���� ��� ���� �񱳴� �������� �Ѵ�. DB�� ��ġ�Ƿ� �� �˻縦 ���������� �ؾ��Ѵ�
				g_DB.FreeMiddleQuery(
					RGuildAddSkill,
					player->GetID(),
					"EXEC MP_GUILD_ADD_SKILL %d, %d, %d, %d, %d",
					guild->GetIdx(),
					skillInfo->TrainPoint,
					skillInfo->TrainMoney,
					skill.wSkillIdx,
					skill.Level );
			}

			break;
		}
		// 080417 LUJ, ȹ�� ��� ��ų�� �ʿ� ����
	case MP_GUILD_ADD_SKILL_ACK_TO_MAP:
		{
			MSG_SKILL_UPDATE message;
			memcpy( &message, pMsg, sizeof( message ) );
			// 080417 LUJ, ���������� �ٲ��� ������ ������Ʈ���� ��ε�ĳ�����ع�����
			message.Protocol = MP_GUILD_ADD_SKILL_ACK;

			CGuild* guild = GetGuild( message.dwObjectID );

			if( guild )
			{
				const SKILL_BASE& skill = message.SkillBase;

				guild->AddSkill( skill.wSkillIdx, skill.Level );
				guild->SendMsgToAll( &message, sizeof( message ) );
			}

			break;
		}
		// 080602 LUJ, ��� ������ ���ΰ�ħ�Ѵ�
	case MP_GUILD_SCORE_UPDATE_TO_MAP:
		{
			const MSGBASE*	message		= ( MSGBASE* )pMsg;
			const DWORD		guildIndex	= message->dwObjectID;

			GuildAddScore( guildIndex, 0 );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


CGuild* CGuildManager::RegistGuild( const GUILDINFO& info, MONEYTYPE GuildMoney)
{
	if(m_GuildRoomHashTable.GetData(info.GuildIdx))
	{
		char buf[64];
		sprintf(buf, "Error! GuildIdx : %u", info.GuildIdx);
		ASSERTMSG(0, buf);
		return NULL;
	}
	CGuild* pGuild = new CGuild(info, GuildMoney);
	m_GuildRoomHashTable.Add(pGuild, info.GuildIdx);
	return pGuild;
}

void CGuildManager::CreateGuildSyn(CPlayer* player, const char* guildName, const char* guildIntro )
{
	const LevelSetting* setting = GetLevelSetting( 1 );

	// �޽����� �������� �ʴ� ������ �������� ��� Ŭ���̾�Ʈ���� üũ�Ǿ� ���޵��� �ʱ� �����̴�
	if( !	setting												||
			player->GetGuildIdx()								||
			player->GetLevel() < setting->mRequiredMasterLevel	||
			player->GetMoney() < setting->mRequiredMoney )
	{
		return;
	}
	else if( ! CanEntryGuild(player->GetGuildCanEntryDate() ) )
	{
		char	date	[ 11 ]	= { 0 };
		char	number	[ 11 ]	= { 0 };
		char*	number_ch		= number;

		SafeStrCpy( date, player->GetGuildCanEntryDate(), sizeof( date ) );

		// Ÿ������ ����ֳ� ����.. ���ڸ� ���� �������..
		for( const char* date_ch = date; *date_ch; ++date_ch  )
		{
			if( isdigit( *date_ch ) )
			{
				*number_ch++	= *date_ch;
			}
		}

		MSG_DWORD2	message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_CREATE_NACK;
		message.dwData1		= eGuildErr_Create_Unable_Entry_Data;
		message.dwData2		= atoi( number );

		player->SendMsg( &message, sizeof( message ) );
		return;
	}
	else if( ! IsVaildGuildName( guildName ) )
	{
		MSG_DWORD2	message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_CREATE_NACK;
		message.dwData1		= eGuildErr_Create_Invalid_Name;
		
		player->SendMsg( &message, sizeof( message ) );
		return;
	}

	char name	[ MAX_GUILD_NAME + 1]	= { 0 };
	char intro	[ MAX_GUILD_INTRO + 1 ]	= { 0 };

	SafeStrCpy( name, guildName, sizeof( name ) );
	SafeStrCpy( intro, guildIntro, sizeof( intro ) );

	GuildCreate( player->GetID(), player->GetObjectName(), player->GetLevel(), name, intro, GUILD_MASTER );

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.15
	// �йи� ������� �˸���
	g_csFamilyManager.SRV_UpdateMapPlayerInfo( player, name );
	// E �йи� �߰� added by hseos 2007.07.15
}

void CGuildManager::CreateGuildResult(LEVELTYPE MasterLvl, const GUILDINFO& info)
{
	CGuild* pGuild = RegistGuild(info, 0);

	if(pGuild == 0)
	{
		return;
	}

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(info.MasterIdx);

	if(pPlayer)
	{
		pPlayer->SetGuildInfo(info.GuildIdx, GUILD_MASTER, info.GuildName, 0);

		//ack
		SEND_GUILD_CREATE_ACK message;
		message.Category = MP_GUILD;
		message.Protocol = MP_GUILD_CREATE_ACK;
		message.GuildIdx = info.GuildIdx;
		SafeStrCpy(message.GuildName, info.GuildName, sizeof( message.GuildName ) );
		pPlayer->SendMsg(&message, sizeof(message));

		SendGuildName(pPlayer, info.GuildIdx, info.GuildName);
	}

	{
		SEND_GUILD_CREATE_NOTIFY message;
		message.Category				= MP_GUILD;
		message.Protocol				= MP_GUILD_CREATE_NOTIFY_TO_MAP;
		message.GuildInfo.GuildIdx		= info.GuildIdx;
		message.GuildInfo.GuildLevel	= 1;
		message.GuildInfo.MapNum		= GAMERESRCMNGR->GetLoadMapNum();
		message.GuildInfo.MasterIdx		= info.MasterIdx;
		message.GuildInfo.UnionIdx		= 0;
		message.MasterLvl				= MasterLvl;
		message.GuildInfo.MarkName		= info.MarkName;

		// 080225 LUJ, ������ ȸ�� ����
		{
			GUILDMEMBERINFO& member = message.mMaster;

			member.MemberIdx	= info.MasterIdx;
			SafeStrCpy( member.MemberName, info.MasterName, sizeof( member.MemberName ) );
			member.Memberlvl	= MasterLvl;
			member.Rank			= GUILD_MASTER;
			member.bLogged		= TRUE;

			const CHARACTER_TOTALINFO& info = pPlayer->GetCharacterTotalInfo();

			member.mJobGrade	= info.JobGrade;
			member.mRace		= info.Race;
			memcpy( member.mJob, info.Job, sizeof( member.mJob ) );			
		}

		SafeStrCpy(message.GuildInfo.MasterName, info.MasterName, sizeof( message.GuildInfo.MasterName ) );
		SafeStrCpy(message.GuildInfo.GuildName, info.GuildName, sizeof( message.GuildInfo.GuildName ) );

		g_Network.Send2AgentServer((char*)&message, sizeof(message));

		pGuild->AddMember( &( message.mMaster ) );
	}	

	AddPlayer( pPlayer );

	LogGuild( pGuild->GetMasterIdx(), pGuild->GetIdx(), eLogGuild_GuildCreate );
}

void CGuildManager::BreakUpGuildSyn(CPlayer* pMaster)
{
	if(pMaster->GetGuildMemberRank() != GUILD_MASTER)
	{
		SendNackMsg( pMaster, MP_GUILD_BREAKUP_NACK, eGuildErr_BreakUp);
		return;
	}
	
	GuildBreakUp(pMaster->GetGuildIdx(), pMaster->GetID());

	GTMGR->DeleteAddedGuild( pMaster->GetGuildIdx() );

	SIEGEWARMGR->BreakUpGuild( pMaster->GetGuildIdx() );
}

void CGuildManager::BreakUpGuildResult(CPlayer* pMaster, DWORD GuildIdx, char* EntryDate)
{
	CGuild* pGuild = GetGuild(GuildIdx);

	if( ! pMaster ||
		! pGuild )
	{
		return;
	}

	const BYTE rt = DoBreakUp(GuildIdx);

	if( rt )
	{
		SendNackMsg( pMaster, MP_GUILD_BREAKUP_NACK, rt);
		return;
	}

	LogGuild( pGuild->GetMasterIdx(), GuildIdx, eGuildLog_GuildBreakUp);

	// ���Ƽ �ο�
	{
		const EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(pMaster->GetLevel());
		const EXPTYPE minusExp		= EXPTYPE( pointForLevel * 0.05 );

		pMaster->ReduceExpPoint(minusExp, eExpLog_LosebyBreakupGuild);

		SendGuildEntryDate(pMaster, EntryDate);
	}	

	{
		MSGBASE message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_BREAKUP_NOTIFY_TO_MAP;
		message.dwObjectID	= pMaster->GetGuildIdx();

		g_Network.Send2AgentServer((char*)&message, sizeof(message));
	}	
}


BYTE CGuildManager::DoBreakUp( DWORD GuildIdx )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(!pGuild)
	{
		ASSERT(0);
		return eGuildErr_BreakUp; 
	}

	if(pGuild->GetMarkName())
	{
		DeleteMark(pGuild->GetMarkName());
	}

	pGuild->BreakUp();
	m_GuildRoomHashTable.Remove(GuildIdx);

	SAFE_DELETE( pGuild );

	// ��嵵 �����...
	{
		GuildMap::const_iterator it = mGuildScoreMap.find( GuildIdx );

		if( mGuildScoreMap.end() != it )
		{
			const CGuildScore* guild = it->second;

			SAFE_DELETE( guild );

			mGuildScoreMap.erase( GuildIdx );
		}
	}

	return 0;
}


CGuild* CGuildManager::GetGuild(DWORD GuildIdx)
{
	return m_GuildRoomHashTable.GetData(GuildIdx);
}


CGuild* CGuildManager::GetGuildFromMasterIdx( DWORD dwMasterIdx )
{
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while( pGuild = m_GuildRoomHashTable.GetData() )
	{
		if( pGuild->GetMasterIdx() == dwMasterIdx )
			return pGuild;
	}
	return NULL;
}


CGuildMark* CGuildManager::GetMark(MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = NULL;
	pMark = m_GuildMarkTable.GetData(MarkName);
	return pMark;
}

void CGuildManager::SendNackMsg(CPlayer* pPlayer, BYTE Protocol, BYTE errstate)
{
	MSG_BYTE nmsg;
	nmsg.Category	= MP_GUILD;
	nmsg.Protocol	= Protocol;
	nmsg.bData		= errstate;
	pPlayer->SendMsg(&nmsg, sizeof(nmsg));
}

BOOL CGuildManager::IsVaildGuildName( const char* GuildName )
{
	CGuild * pInfo;
	m_GuildRoomHashTable.SetPositionHead();
	while(pInfo = m_GuildRoomHashTable.GetData())
	{
		if(strcmp(pInfo->GetGuildName(), GuildName) == 0)
			return FALSE;
	}
	if(strlen(GuildName) > MAX_GUILD_NAME)
		return FALSE;
	return TRUE;
}


//void CGuildManager::DeleteMemberSyn(CPlayer* pMaster, DWORD MemberIDX)
//{
//	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
//	{
//		SendNackMsg( pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember);
//	}
//	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
//	if(!pGuild)
//		SendNackMsg( pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember);
//	if(pMaster->GetGuildMemberRank() < GUILD_MASTER)
//		SendNackMsg( pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NothaveAuthority);
//	if(!pGuild->IsMember(MemberIDX))
//		SendNackMsg( pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NotMember);
//	
//	//db update
//	GuildDeleteMember(pMaster->GetGuildIdx(), MemberIDX);
//}


void CGuildManager::DeleteMemberResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;

	const BYTE err = DoDeleteMember(GuildIdx, MemberIDX, eGuild_Delete, EntryDay);
	if(err)
	{
		ASSERT(0);
		CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pGuild->GetMasterIdx());

		if(pMaster )
		{
			SendNackMsg( pMaster, MP_GUILD_DELETEMEMBER_NACK, err);
		}

		return;
	}

	LogGuild(MemberIDX, GuildIdx, eGuildLog_MemberBan);

	{
		MSG_NAME_DWORD message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP;
		message.dwObjectID	= GuildIdx;
		message.dwData		= MemberIDX;
		SafeStrCpy(message.Name, EntryDay, 11);
		g_Network.Send2AgentServer((char*)&message, sizeof(message));	
	}

	// 071213 LUJ, Ż�� �ÿ� ��� ���� ��꿡�� ���� ��
	{
		CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( MemberIDX );

		if( player )
		{
			RemovePlayer( player );
		}
	}	
}


BYTE CGuildManager::DoDeleteMember(DWORD GuildIDX, DWORD MemberIDX, BYTE bType, char* EntryDay)
{
	CGuild* pGuild = m_GuildRoomHashTable.GetData(GuildIDX);
	if(pGuild == 0)
	{
		ASSERT(0);
		return eGuildErr_Err;
	}
	if(pGuild->IsMember(MemberIDX) == FALSE)
	{
		ASSERT(0);
		return eGuildErr_DeleteMember;
	}

	//member delete
	if(pGuild->DeleteMember(MemberIDX, bType) == FALSE)
	{
		ASSERT(0);
		return eGuildErr_Err;
	}	

	//TO DO ȣĪ�� ���� �߰�
	CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(MemberIDX);
	if(pMember)
	{
		pMember->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
		pMember->SetNickName("");
		SendGuildEntryDate(pMember, EntryDay);			
		MSG_DWORDBYTE msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_DELETEMEMBER_ACK;
		msg.dwData = MemberIDX;
		msg.bData = eGuild_Delete;
		pMember->SendMsg(&msg, sizeof(msg));

		MarkChange(pMember, 0, 0);
	}

	pGuild->SendDeleteMsg(MemberIDX, eGuild_Delete);

	return 0;
}

void CGuildManager::LoadMembers(GUILDMEMBERINFO_ID* pInfo)
{
	CGuild * pGuild = GetGuild(pInfo->GuildIdx);
	if(pGuild == 0)
	{
		char buf[64];
		sprintf(buf, "GuildIdx : %d, CharacterIdx : %d", pInfo->GuildIdx, pInfo->MemberInfo.MemberIdx);
		ASSERTMSG(0, buf);
		return;
	}
	pGuild->AddMember(&pInfo->MemberInfo);
}

void CGuildManager::LoadMark(MARKNAMETYPE MarkName, char* pImgData)
{
	if(m_GuildMarkTable.GetData(MarkName))
	{
		ASSERTMSG(0, "LoadMark");
		return;
	}
	char Img[GUILDMARK_BUFSIZE];
	char tempBuf[3] = {0,};
	int curpos = 0;
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		strncpy(tempBuf,&pImgData[curpos],2); // "FF"
		Img[n] = HexToByte(tempBuf);
		curpos += 2;
	}
	CGuildMark* pMark = new CGuildMark;
	pMark->Init(MarkName, Img);

	m_GuildMarkTable.Add(pMark, MarkName);
}

void CGuildManager::AddMemberSyn(CPlayer* host, DWORD guestPlayerIndex )
{
	CPlayer*	guest = (CPlayer*)g_pUserTable->FindUser( guestPlayerIndex );
	CGuild*		guild = GetGuild( host->GetGuildIdx() );

	// �˻�
	{
		MSG_NAME_DWORD	message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ADDMEMBER_NACK;
		message.dwObjectID	= host->GetID();
		
		if( !	guild ||
				SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
		{
			message.dwData = eGuildErr_Err;
		}
		if( ! guest )
		{
			message.dwData = eGuildErr_AddMember_NotPlayer;
		}
		else if( guest->GetGuildIdx() )
		{
			message.dwData = eGuildErr_AddMember_OtherGuild;
		}
		else if( GUILD_MEMBER == host->GetGuildMemberRank() )
		{
			message.dwData = eGuildErr_AddMember_NothaveAuthority;
			return;
		}
		else if( ! guild->CanAddMember() )
		{
			message.dwData = eGuildErr_AddMember_FullMember;
		}
		else if( ! CanEntryGuild(guest->GetGuildCanEntryDate()))
		{
			message.dwData		= eGuildErr_AddMember_TargetNotDay;
			SafeStrCpy( message.Name, guest->GetGuildCanEntryDate(), sizeof( message.Name ) );
		}		

		// ���� �ڵ尡 ������ �˸���
		if( message.dwData )
		{
			host->SendMsg( &message, sizeof( message ) );
			return;
		}
	}

	// ��û ���� �˷��ش�
	{
		MSGBASE message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ADDMEMBER_INVITE;
		
		host->SendMsg( &message, sizeof( message ) );
	}
	
	{
		SEND_GUILD_INVITE message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_INVITE;
		message.MasterIdx	= host->GetID();
		SafeStrCpy( message.MasterName, host->GetObjectName(),	sizeof( message.MasterName ) );
		SafeStrCpy( message.GuildName,	guild->GetGuildName(),	sizeof( message.GuildName ) );

		guest->SendMsg( &message, sizeof( message ) );
	}
}

void CGuildManager::AddMemberResult(CPlayer* pTarget, DWORD MasterIdx)
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	
	if(pMaster == 0)
	{
		SendNackMsg( pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_InviteApt_NoMaster);
		return;
	}

	CGuild*	pGuild	= GetGuild(pMaster->GetGuildIdx());

	if(pGuild == 0)
	{
		SendNackMsg( pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_Err);
		return;
	}
	else if(pGuild->IsMember(pTarget->GetID()) == TRUE)
	{
		SendNackMsg( pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_Err);
		return;
	}
	else if(pGuild->CanAddMember() == FALSE)
	{
		SendNackMsg( pMaster, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_AddMember_FullMember);
		return;
	}

	//db update
	GuildAddMember(pMaster->GetGuildIdx(), pTarget->GetID(), pGuild->GetGuildName(), GUILD_MEMBER);

	pTarget->SetGuildInfo(pMaster->GetGuildIdx(), GUILD_MEMBER, pGuild->GetGuildName(), pGuild->GetMarkName());
	MarkChange(pTarget, pMaster->GetGuildIdx(), pGuild->GetMarkName());

	// 080225 LUJ, ���� ���� ����
	GUILDMEMBERINFO member;
	{
		member.MemberIdx	= pTarget->GetID();
		SafeStrCpy( member.MemberName, pTarget->GetObjectName(), sizeof( member.MemberName ) );
		member.Memberlvl	= pTarget->GetLevel();
		member.Rank			= GUILD_MEMBER;
		member.bLogged		= TRUE;

		const CHARACTER_TOTALINFO& info = pTarget->GetCharacterTotalInfo();

		member.mJobGrade	= info.JobGrade;
		member.mRace		= info.Race;
		memcpy( member.mJob, info.Job, sizeof( member.mJob ) );		
	}

	pGuild->AddMember(&member);
	pGuild->SetLogInfo(pTarget->GetID(), TRUE);

	{
		SEND_GUILD_TOTALINFO message;
		ZeroMemory( &message, sizeof( message ) );
		
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ACCEPT_ACK;
		message.InitGuildInfo(pGuild->GetInfo());
		pGuild->GetTotalMember(message.MemberInfo);
		message.membernum	= pGuild->GetMemberNum();
		pTarget->SendMsg(&message, message.GetMsgLength());
	}
	
	{
		SEND_GUILD_MEMBER_INFO message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
		message.GuildIdx	= pMaster->GetGuildIdx();
		SafeStrCpy(message.GuildName, pGuild->GetGuildName(), sizeof( message.GuildName ) );
		message.MemberInfo	= member;

		g_Network.Send2AgentServer((char*)&message, sizeof(message));
	}

	GUILDWARMGR->AddPlayer( pTarget );

	if( pGuild->GetUnionIndex() )
	{
		GUILDUNIONMGR->AddPlayer( pTarget, pGuild->GetUnionIndex() );		
	}

	LogGuild(pTarget->GetID(), pGuild->GetIdx(), eGuildLog_MemberAdd);
}

void CGuildManager::UserLogIn(CPlayer* pPlayer)
{
	if(pPlayer->GetGuildIdx() == 0)
		return;

	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
	{
		ASSERTMSG(0, "UserLogIn - No Guild Info! ");
		return;
	}

	SetLogInfo(pGuild, pPlayer->GetID(), TRUE);

	//SW060719 ��������Ʈ //��PM������ ���Ŀ����� �ÿ� Update.
	pGuild->AddConnectingMapMemberCount(+1);

	//SEND_GUILD_TOTALINFO msg;
	//memset(&msg, 0, sizeof(SEND_GUILD_TOTALINFO));
	//msg.Category = MP_GUILD;
	//msg.Protocol = MP_GUILD_INFO;
	//msg.InitGuildInfo(pGuild->GetInfo());
	//msg.InitPointInfo(pGuild->GetGuildPointInfo());
	//pGuild->GetTotalMember(msg.MemberInfo);
	//
	//msg.membernum = pGuild->GetMemberNum();
	//
	//pPlayer->SendMsg(&msg, msg.GetMsgLength());

	if( pGuild->GetUnionIndex() )
		GUILDUNIONMGR->LoginPlayer( pPlayer, pGuild->GetUnionIndex() );
}

void CGuildManager::UserLogOut(CPlayer* pPlayer)
{
	if(pPlayer->GetGuildIdx() == 0)
		return;
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
		return;
	SetLogInfo(pGuild, pPlayer->GetID(), FALSE);
	//SW060719 ��������Ʈ
	pGuild->AddConnectingMapMemberCount(-1);
}

void CGuildManager::SecedeSyn(CPlayer* pPlayer)
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	CGuild* pGuild = GetGuild(GuildIdx);

	if(pGuild == 0)
	{
		SendNackMsg( pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_NoGuild);
		return;
	}
	else if(pPlayer->GetGuildMemberRank() >= GUILD_MASTER)
	{
		SendNackMsg( pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
		return;
	}
	else if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
		return;
	}
	
	GuildSecedeMember(GuildIdx, pPlayer->GetID());
}

void CGuildManager::SecedeResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MemberIDX);

	if(pPlayer)
	{		
		pPlayer->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
		pPlayer->SetNickName("");
		SendGuildEntryDate(pPlayer, EntryDay);
		MarkChange(pPlayer, 0, 0);

		MSGBASE msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_SECEDE_ACK;
		pPlayer->SendMsg(&msg, sizeof(msg));

	}

	CGuild* pGuild = GetGuild(GuildIdx);

	if( pGuild == NULL )
	{
		return;
	}

	//	BOOL rt = pGuild->DeleteMember(pPlayer->GetID(), eGuild_Secede);
	BOOL rt = pGuild->DeleteMember(MemberIDX, eGuild_Secede);
	if(rt == FALSE)
		return;	

	//	pGuild->SendDeleteMsg(pPlayer->GetID(), eGuildLog_MemberSecede);
	pGuild->SendDeleteMsg(MemberIDX, eGuildLog_MemberSecede);

	//notify
	MSG_DWORD notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_SECEDE_NOTIFY_TO_MAP;
	notify.dwObjectID = GuildIdx;
	//	notify.dwData = pPlayer->GetID();
	notify.dwData = MemberIDX;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));

	LogGuild(MemberIDX, GuildIdx, eGuildLog_MemberSecede);

	// 071213 LUJ, ���� �ÿ� ��� ���� ��꿡�� ���� ��
	{
		CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( MemberIDX );

		if( player )
		{
			RemovePlayer( player );
		}
	}
}


void CGuildManager::SendChatMsg(DWORD GuildIdx, DWORD PlayerIdx, char* pMsg)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;

	TESTMSGID msg;
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_GUILD;
	msg.dwSenderID = PlayerIdx;
	SafeStrCpy(msg.Msg, pMsg, MAX_CHAT_LENGTH+1);

	pGuild->SendMsgToAll(&msg, msg.GetMsgLength());

}

//void CGuildManager::RegistMarkSyn(DWORD GuildIdx, char* ImgData, DWORD PlayerIdx)
//{	
//	GuildMarkRegist(GuildIdx, ImgData, PlayerIdx);
//}

void CGuildManager::RegistMarkResult(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName, char * pImgData)
{	
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;
	pPlayer->SetGuildMarkName(MarkName);

	MSG_GUILDMARK_IMG msg;
	char *pStr = (char*)pImgData;
	char tempBuf[3] = {0,};
	int curpos = 0;
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		strncpy(tempBuf,&pStr[curpos],2); // "FF"
		msg.imgData[n] = HexToByte(tempBuf);
		curpos += 2;
	}

	MarkChange(pPlayer, GuildIdx, MarkName);

	DoRegistMark(pGuild, MarkName, msg.imgData);
	msg.Category = MP_GUILD;
	msg.GuildIdx = GuildIdx;
	msg.MarkName = MarkName;
	msg.Protocol = MP_GUILDMARK_NOTIFY_TO_MAP;
	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
}

void CGuildManager::DoRegistMark(CGuild* pGuild, MARKNAMETYPE MarkName, char* imgData)
{
	pGuild->SetMarkName(MarkName);
	CGuildMark * pMark = new CGuildMark;
	pMark->Init(MarkName, imgData);
	m_GuildMarkTable.Add(pMark, MarkName);
}

int CGuildManager::convertCharToInt(char c)
{
	if('0' <= c && c <= '9')
		return c - '0';
	if('A' <= c && c <= 'F')
		return c - 'A' +10;
	if('a' <= c && c <= 'f')
		return c - 'a' +10;
	return 0;
}

//�ݵ�� "FF" �̷������� �Է��� �Ǿ�� �Ѵ�.
BYTE CGuildManager::HexToByte(char* pStr)
{
	int n1 = convertCharToInt(pStr[0]);
	int n2 = convertCharToInt(pStr[1]);

	return n1 * 16 + n2;
}

void CGuildManager::RequestMark(CPlayer* pPlayer, MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = GetMark(MarkName);
	if(pMark == 0)
	{
		/*	char buf[128];
		sprintf(buf, "GuildMark Info Wrong! MarkIdx : %u", MarkName);
		ASSERTMSG(0, buf);
		MSG_DWORD nmsg;
		nmsg.Category = MP_GUILD;
		nmsg.Protocol = MP_GUILDMARK_REQUEST_NACK;
		nmsg.dwData = MarkName;
		pPlayer->SendMsg(&nmsg, sizeof(nmsg));
		*/	return;
	}
	MSG_GUILDMARK_IMG msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILDMARK_REQUEST_ACK;
	msg.MarkName = MarkName;
	memcpy(msg.imgData, pMark->GetImgData(), GUILDMARK_BUFSIZE);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CGuildManager::DeleteMark(MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = m_GuildMarkTable.GetData(MarkName);
	if(pMark == 0)
		return;

	m_GuildMarkTable.Remove(MarkName);
	delete pMark;
}

void CGuildManager::MarkChange(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName)
{
	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILDMARK_CHANGE;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = GuildIdx;
	msg.dwData3 = MarkName;

	PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
}

void CGuildManager::SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, const char * GuildName)
{
	SEND_GUILD_CREATE_ACK msg;
	msg.Category	= MP_GUILD;
	msg.Protocol	= MP_GUILDNAME_CHANGE;
	msg.dwObjectID	= pPlayer->GetID();
	msg.GuildIdx	= dwGuildIdx;
	SafeStrCpy( msg.GuildName, GuildName, sizeof( msg.GuildName ) );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
}

//void CGuildManager::LevelUpSyn(CPlayer* pPlayer)
//{
//	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
//	if(pGuild == 0)
//		SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//	if(pGuild->GetLevel() == MAX_GUILD_LEVEL)
//		SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//
//	/*�����Ƽ ���� - if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, pPlayer->GetAbilityGroup()) <= pGuild->GetLevel())
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotAbil);*/
//		if( pPlayer->GetMoney() < GetLevelUpMoney(pGuild->GetLevel()) )
//			SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotMoney);
//	if(pGuild->GetLevel() >= MAX_GUILD_LEVEL)
//		SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_Complete);
//	if(pGuild->GetLevel() == GUILD_4LEVEL)
//	{
//		//		if( pPlayer->GetStage() == eStage_Normal )
//		//			SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//	}
//	/*
//	if(pGuild->GetLevel() == GUILD_4LEVEL)
//	{
//	//		if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_GUILD, pPlayer->GetAbilityGroup()) != 1)
//	//			SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//	if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, pPlayer->GetAbilityGroup()) <= pGuild->GetLevel())
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotAbil);
//	if( pPlayer->GetStage() == eStage_Normal )
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//	if( pPlayer->GetMoney() < GetLevelUpMoney(pGuild->GetLevel()) )
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
//	}
//	else
//	{
//	if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, pPlayer->GetAbilityGroup()) <= pGuild->GetLevel())
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotAbil);
//	if( pPlayer->GetMoney() < GetLevelUpMoney(pGuild->GetLevel()) )
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotMoney);
//	if(pGuild->GetLevel() >= MAX_GUILD_LEVEL)
//	SendNackMsg( pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_Complete);
//	}
//	*/
//	pPlayer->SetMoney(GetLevelUpMoney(pGuild->GetLevel()), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseGuildLevelUp);
//	pGuild->LevelUp();
//
//	//Temp!!
//	//	g_Console.LOG( 4, "GuildLevel : %d", pGuild->GetLevel() );
//	//	for(int n=0;n<MAX_GUILD_STEP;++n)
//	//	{
//	//		g_Console.LOG( 4, "m_LevelUpInfo[%d] = %d", n, m_LevelUpInfo[n] );
//	//	}
//
//	//dbupdate
//	GuildLevelUp(pPlayer->GetGuildIdx(), pGuild->GetLevel());
//
//	MSG_BYTE msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_LEVELUP_NOTIFY_TO_MAP;
//	msg.dwObjectID = pPlayer->GetGuildIdx();
//	msg.bData = pGuild->GetLevel();
//	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
//}

//void CGuildManager::LoadLevelUpInfo()
//{
//	CMHFile file;
//	file.Init("System/Resource/GuildLevel.bin","rb");
//	if(file.IsInited() == FALSE)
//	{
//		ASSERTMSG(0, "Fail To Load a File : GuildLevel");
//		return;
//	}
//
//	for(int n=0;n<MAX_GUILD_STEP;++n)
//	{
//		m_LevelUpInfo[n] = file.GetDword();
//		//Temp!!
//		//		g_Console.LOG( 4, "m_LevelUpInfo[%d] = %d", n, m_LevelUpInfo[n] );
//	}
//
//
//
//	file.Release();
//}


//MONEYTYPE CGuildManager::GetLevelUpMoney(BYTE GuildLevel)
//{
//	if((GuildLevel == 0) || (GuildLevel > MAX_GUILD_LEVEL))
//	{
//		return 0;
//	}
//	return m_LevelUpInfo[GuildLevel-1];
//}

BOOL CGuildManager::CanEntryGuild(char* date)
{
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	DWORD year;
	DWORD month;
	DWORD day;

	char buf[5]={ 0, };
	strncpy(buf, &date[0],4);
	year = atoi(buf);
	if( ti.wYear > year)
		return TRUE;
	else if(ti.wYear < year)
		return FALSE;

	char dbuf[3]={ 0, };
	strncpy(dbuf, &date[5], 2);
	month = atoi(dbuf);

	if(ti.wMonth > month)
		return TRUE;
	else if(ti.wMonth < month)
		return FALSE;

	strncpy(dbuf, &date[8], 2);
	dbuf[2] = 0;
	day = atoi(dbuf);
	if(ti.wDay < day)
		return FALSE;

	return TRUE;
}

void CGuildManager::SendGuildEntryDate(CPlayer* pPlayer, char* day)
{
	pPlayer->SetGuildCanEntryDate(day);

	MSG_NAME dmsg;
	dmsg.Category = MP_GUILD;
	dmsg.Protocol = MP_GUILD_SETENTRYDATE;
	SafeStrCpy(dmsg.Name, day, 11);
	pPlayer->SendMsg(&dmsg, sizeof(dmsg));
}

void CGuildManager::ChangeMemberRank(CPlayer* player, DWORD targetPlayerIndex, BYTE Rank)
{	
	CGuild* guild = GetGuild(player->GetGuildIdx());

	if( ! guild )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_NoGuild );
		return;
	}
	else if( ! guild->IsMember( targetPlayerIndex ) )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_NotMember );
		return;
	}
	if( player->GetID() == targetPlayerIndex )	// �ڱ� �ڽ��� ���� �Ұ�
	{
		ASSERT( 0 );
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err );
		return;
	}
	else if( player->GetGuildMemberRank() <= Rank )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_NoAuthority );
		return;
	}
	else if( GUILD_MASTER == Rank )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_Master );
		return;
	}
	
	{
		const BYTE rank = player->GetGuildMemberRank();

		if( rank != GUILD_MASTER &&
			rank != GUILD_VICEMASTER )
		{
			SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err );
			return;
		}
	}	
	
	GUILDMEMBERINFO* targetMember = guild->GetMemberInfo(targetPlayerIndex);
	if(NULL == targetMember) return;

	if( targetMember->Rank == Rank )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_AlreadyHave );
		return;
	}
	// �ڽŰ� ���ų� �� ���� ������ ���� ȸ���� �����ų �� ����
	else if( player->GetGuildMemberRank() <= targetMember->Rank )
	{
		SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_LowerLevel );
		return;
	}
	else if( targetMember->Rank == GUILD_STUDENT )
	{
		if( targetMember->Memberlvl < GUILD_STUDENT_MAX_LEVEL )
		{
			SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_NoGraduatedStudent );
			return;
		}
		else if( ! guild->CanAddMember() )
		{
			SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_Student );
			return;
		}
	}
	
	// ���� ����
	{
		if( ! guild->ChangeMemberRank(targetPlayerIndex, Rank ) )
		{
			SendNackMsg( player, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err);
			return;
		}

		GuildChangeMemberRank(player->GetGuildIdx(), targetPlayerIndex, Rank);

		CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(targetPlayerIndex);

		if(pMember)
		{
			pMember->SetGuildMemberRank(Rank);
		}
	}	

	{
		MSG_DWORDBYTE message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_CHANGERANK_NOTIFY_TO_MAP;
		message.dwObjectID	= player->GetGuildIdx();
		message.dwData		= targetPlayerIndex;
		message.bData		= Rank;
		//g_Network.Send2AgentServer( (char*)&message, sizeof( message ) );
		g_Network.Broadcast2AgentServer( ( char* )&message, sizeof( message ) );
	}

	LogGuild(player->GetID(), player->GetGuildIdx(), eGuildLog_MasterChangeRank, targetPlayerIndex, Rank);
}

void CGuildManager::GiveNickNameSyn(CPlayer* pMaster, DWORD TargetId, char* NickName)
{
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());

	if( ! pGuild )
	{		
		SendNackMsg(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_NoGuild);
		return;
	}
	else if(pMaster->GetGuildMemberRank() == GUILD_MEMBER )
	{
		SendNackMsg(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Err);
		return;
	}
	else if(pGuild->IsMember(TargetId) == FALSE)
	{
		SendNackMsg(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Nick_NotMember);
		return;
	}

	const DWORD length = strlen( NickName );
	if( ! length || (length > MAX_GUILD_NICKNAME) )
	{
		SendNackMsg(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Nick_NotAvailableName);
		return;
	}
	
	GuildGiveMemberNickName(TargetId, NickName);

	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(TargetId);
	if(pTarget)
	{
		pTarget->SetNickName(NickName);

		// SEND TO 
		SEND_GUILD_NICKNAME msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_GIVENICKNAME_ACK;
		msg.TargetIdx = TargetId;
		SafeStrCpy(msg.NickName, NickName, MAX_GUILD_NICKNAME+1);
		PACKEDDATA_OBJ->QuickSend(pTarget, &msg, sizeof(msg));	
	}
	else
	{
		//notify
		SEND_GUILD_NICKNAME nmsg;
		nmsg.Category = MP_GUILD;
		nmsg.Protocol = MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP;
		nmsg.TargetIdx = TargetId;
		SafeStrCpy(nmsg.NickName, NickName, MAX_GUILD_NICKNAME+1);
		g_Network.Broadcast2AgentServer((char*)&nmsg,sizeof(nmsg));
	}
}

//void CGuildManager::RegistGuildItem(DWORD GuildIdx, ITEMBASE* Guilditem)
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if(pGuild == 0)
//	{
//		char buf[64];
//		sprintf(buf, "RegistGuildItem - NO %d Guild Info", GuildIdx);
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	if( pGuild )
//		pGuild->InitGuildItem(Guilditem);
//}

CItemSlot* CGuildManager::GetSlot(DWORD GuildIdx)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild)
	{
		return pGuild->GetSlot();
	}
	return NULL;
}


void CGuildManager::MemberLevelUp(DWORD GuildIdx, DWORD PlayerIdx, LEVELTYPE PlayerLvl)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;
	if(pGuild->IsMember(PlayerIdx) == FALSE)
		return;
	pGuild->SetMemberLevel(PlayerIdx, PlayerLvl);

	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP;
	msg.dwData1 = GuildIdx;
	msg.dwData2 = PlayerIdx;
	msg.dwData3 = PlayerLvl;
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));	
}

void CGuildManager::SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals)
{
	if(pGuild->IsMember(PlayerIdx) == FALSE)
		return;

	pGuild->SetLogInfo(PlayerIdx, vals);

	SEND_GUILDMEMBER_LOGININFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LOGINFO_NOTIFY_TO_MAP;
	msg.dwObjectID = pGuild->GetIdx();
	msg.MemberIdx = PlayerIdx;
	msg.bLogIn = vals;
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
}

BOOL CGuildManager::CheckHack( CPlayer* pPlayer )
{
	//	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage ) != NULL )
	//		return TRUE;
	// ����: 
#ifndef _DEBUG
	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;
#endif

	return TRUE;
}

void CGuildManager::MsgChangeLocationSyn( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	DWORD dwGuildIdx = pPlayer->GetGuildIdx();
	if( !dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_NoGuild );
		return;
	}
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_InviteApt_NoMaster );
		return;
	}

	CGuild* pGuild = m_GuildRoomHashTable.GetData(dwGuildIdx);
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_NoGuild );
		return;
	}
	if( pGuild->GetLocation() != GAMERESRCMNGR->GetLoadMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_Filter );
		return;
	}
	if( pGuild->GetLocation() == pmsg->wData )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotMember );
		return;
	}
	if( !pGuild->IsEmptyGuildWare() )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotAvailableName );
		return;
	}
	//if( pGuild->GetMoney() )	//KES 070604 : from DarkStory
	//{
	//	SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotAvailableName );
	//	return;
	//}

	MONEYTYPE money = pGuild->GetLevel() * 100000;

	if( pPlayer->GetMoney() < money )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotLogIn );
		return;
	}

	pPlayer->SetMoney( money, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseChangeLocation, 0 );
	pGuild->SetLocation( pmsg->wData );
	pGuild->SetZeroMoney();		//KES 070604 : from DarkStory

	MSG_DWORD Money;
	Money.Category = MP_GUILD;
	Money.Protocol = MP_GUILD_CHANGE_LOCATION_MONEY;
	Money.dwData = money;
	pPlayer->SendMsg( &Money, sizeof(Money) );

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD;
	Msg.Protocol = MP_GUILD_CHANGE_LOCATION_ACK;
	Msg.dwData1 = dwGuildIdx;
	Msg.dwData2 = pmsg->wData;
	pGuild->SendMsgToAll( &Msg, sizeof(Msg) );

	MSG_DWORD2 Msg2;
	Msg2.Category = MP_GUILD;
	Msg2.Protocol = MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP;
	Msg2.dwData1 = dwGuildIdx;
	Msg2.dwData2 = pmsg->wData;
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &Msg2, sizeof(Msg2) );		//KES 070604 : from DarkStory

	// dbupdate
	ChangeGuildLocation( dwGuildIdx, pmsg->wData );
}

void CGuildManager::MsgChangeLocationNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CGuild* pGuild = m_GuildRoomHashTable.GetData(pmsg->dwData1);
	if( !pGuild )	return;

	pGuild->SetLocation( (MAPTYPE)pmsg->dwData2 );
	pGuild->SetZeroMoney();	//KES 070604: from DarkStory

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD;
	Msg.Protocol = MP_GUILD_CHANGE_LOCATION_ACK;
	Msg.dwData1 = pmsg->dwData1;
	Msg.dwData2 = pmsg->dwData2;
	pGuild->SendMsgToAll( &Msg, sizeof(Msg) );
}

void CGuildManager::MsgGuildNoteSyn( void* pMsg )
{
	MSG_GUILD_SEND_NOTE* msg = (MSG_GUILD_SEND_NOTE*) pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( msg->FromId );
	if( !pPlayer )	return;

	DWORD dwGuildIdx = pPlayer->GetGuildIdx();
	if( !dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTE_NACK, eGuildErr_NoGuild );
		return;
	}
	CGuild* pGuild = m_GuildRoomHashTable.GetData(dwGuildIdx);
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTE_NACK, eGuildErr_NoGuild );
		return;
	}

	pGuild->SendAllNote(pPlayer, msg->Note);

	MSGBASE ack;
	ack.Category = MP_GUILD;
	ack.Protocol = MP_GUILD_NOTE_ACK;
	pPlayer->SendMsg(&ack, sizeof(ack));
}


//void CGuildManager::AddStudentSyn( CPlayer* pMaster, DWORD TargetPlayerID )
//{
//	CGuild*		pGuild	= GetGuild(pMaster->GetGuildIdx());
//	CPlayer*	pTarget = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
//
//	// �˻�
//	{
//		MSG_NAME_DWORD message;
//		ZeroMemory( &message, sizeof( message ) );
//		message.Category	= MP_GUILD;
//		message.Protocol	= MP_GUILD_ADDMEMBER_NACK;
//
//		if( !	pGuild	||
//				SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum())
//		{
//			message.dwData = eGuildErr_Err;
//		}
//		else if( pMaster->GetGuildMemberRank() < GUILD_SENIOR )
//		{
//			message.dwData = eGuildErr_AddMember_NothaveAuthority;
//		}
//		else if(pTarget->GetGuildIdx())
//		{
//			message.dwData = eGuildErr_AddMember_OtherGuild;
//		}
//		else if( ! pTarget )
//		{
//			message.dwData = eGuildErr_AddMember_NotPlayer;
//		}
//		else if(pTarget->GetLevel() >= GUILD_STUDENT_MAX_LEVEL)
//		{
//			message.dwData = eGuildErr_AddStudent_TooHighLevelAsStudent;
//		}
//		else if( pGuild->IsMember(TargetPlayerID) )
//		{
//			message.dwData = eGuildErr_AddMember_AlreadyMember;
//		}
//		else if( ! pGuild->CanAddStudent() )
//		{
//			message.dwData = eGuildErr_AddStudent_Full;
//		}
//
//		if( message.dwData )
//		{
//			pMaster->SendMsg( &message, sizeof( message ) );
//			return;
//		}
//	}
//
//	{
//		MSGBASE message;
//		message.Category = MP_GUILD;
//		message.Protocol = MP_GUILD_ADDSTUDENT_INVITE;
//
//		pMaster->SendMsg(&message, sizeof(message));
//	}
//
//	{
//		SEND_GUILD_INVITE message;
//		message.Category = MP_GUILD;
//		message.Protocol = MP_GUILD_INVITE_AS_STUDENT;
//		message.MasterIdx = pMaster->GetID();
//		SafeStrCpy(message.MasterName, pMaster->GetObjectName(), sizeof( message.MasterName ) );
//		SafeStrCpy(message.GuildName, pGuild->GetGuildName(), sizeof( message.GuildName ) );
//
//		pTarget->SendMsg(&message, sizeof(message));
//	}
//}

//BOOL CGuildManager::CanEntryGuildForStudent( char* date )
//{
//	//!! DB���� ���� ��¥�� �����ؼ� ������ ���ϴ�. Ȯ���� �۾�.
//	return CanEntryGuild(date);
//}

void CGuildManager::AddStudentResult( CPlayer* pTarget, DWORD MasterID )
{
	CPlayer*	pMaster = (CPlayer*)g_pUserTable->FindUser(MasterID);
	CGuild*		pGuild	= GetGuild( pMaster ? pMaster->GetGuildIdx() : 0 );

	// �˻�
	{
		MSG_NAME_DWORD message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK;
		
		if( ! pMaster )
		{
			message.dwData = eGuildErr_InviteApt_NoMaster;
		}
		else if( !	pGuild ||
					pTarget->GetGuildIdx() )
		{
			message.dwData = eGuildErr_Err;
		}
		else if(  ! pGuild->CanAddStudent() )
		{
			message.dwData = eGuildErr_AddMember_FullMember;
		}
		else if( pTarget->GetLevel() >= GUILD_STUDENT_MAX_LEVEL )
		{
			message.dwData = eGuildErr_AddStudent_TooHighLevelAsStudent;
		}

		if( message.dwData )
		{
			pTarget->SendMsg( &message, sizeof( message ) );
			return;
		}
	}
	
	GuildAddMember(pMaster->GetGuildIdx(), pTarget->GetID(), pGuild->GetGuildName(), GUILD_STUDENT);
	
	pTarget->SetGuildInfo(pMaster->GetGuildIdx(), GUILD_STUDENT, pGuild->GetGuildName(), pGuild->GetMarkName());
	MarkChange(pTarget, pMaster->GetGuildIdx(), pGuild->GetMarkName());

	// 080225 LUJ, ���� ���� ����
	GUILDMEMBERINFO member;
	{
		member.MemberIdx	= pTarget->GetID();
		SafeStrCpy( member.MemberName, pTarget->GetObjectName(), sizeof( member.MemberName ) );
		member.Memberlvl	= pTarget->GetLevel();

		// 080408 LUJ, ���û����� �����Ͽ��µ� �������� �ʿ� ���ĵǴ� ���� ����
		member.Rank			= GUILD_STUDENT;
		member.bLogged		= TRUE;

		const CHARACTER_TOTALINFO& info = pTarget->GetCharacterTotalInfo();

		member.mJobGrade	= info.JobGrade;
		member.mRace		= info.Race;
		memcpy( member.mJob, info.Job, sizeof( member.mJob ) );
	}
	
	pGuild->AddMember(&member);
	pGuild->SetLogInfo(pTarget->GetID(), TRUE);

	{
		SEND_GUILD_TOTALINFO message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ACCEPT_STUDENT_ACK;
		message.membernum	= pGuild->GetMemberNum();
		message.InitGuildInfo(pGuild->GetInfo());
		pGuild->GetTotalMember(message.MemberInfo);
		
		pTarget->SendMsg( &message, message.GetMsgLength() );
	}
	
	{
		SEND_GUILD_MEMBER_INFO message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
		message.GuildIdx	= pMaster->GetGuildIdx();
		SafeStrCpy(message.GuildName, pGuild->GetGuildName(), sizeof( message.GuildName ) );
		message.MemberInfo.MemberIdx = pTarget->GetID();
		message.MemberInfo.Memberlvl = pTarget->GetLevel();
		SafeStrCpy(message.MemberInfo.MemberName, pTarget->GetObjectName(), sizeof( message.MemberInfo.MemberName ) );;
		message.MemberInfo.Rank = GUILD_STUDENT;

		g_Network.Send2AgentServer((char*)&message, sizeof( message ) );
	}
	
	GUILDWARMGR->AddPlayer( pTarget );

	if( pGuild->GetUnionIndex() )
		GUILDUNIONMGR->AddPlayer( pTarget, pGuild->GetUnionIndex() );		

	LogGuild(pTarget->GetID(), pGuild->GetIdx(), eGuildLog_StudentAdd );
}

//BOOL CGuildManager::LoadGuildPointRule()
//{
//	CMHFile file;
//	char szBuf[256] = {0,};
//
//	if(!file.Init("./System/Resource/GuildPointRule.bin", "rb"))
//		return FALSE;
//
//	while(!file.IsEOF())
//	{
//		strcpy(szBuf,strupr(file.GetString()));
//
//		if( szBuf[0] == ';' ) continue;
//
//		if(strcmp(szBuf,"#TIME_CHECK_GUILDHUNTEDMONSTER_COUNT") == 0)
//		{
//			m_CHECKTIMELEN_GuildHuntedMonsterCounter = file.GetDword();
//		}
//
//		if(strcmp(szBuf,"#NOMAL_MONSTER_HUNTCOUNT") == 0)
//		{
//			m_GuildPointRule.NOMAL_MONSTER_HUNTCOUNT = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#FIELDBOSS_MONSTER_HUNTCOUNT") == 0)
//		{
//			m_GuildPointRule.FIELDBOSS_MONSTER_HUNTCOUNT = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#BOSS_MONSTER_HUNTCOUNT") == 0)
//		{
//			m_GuildPointRule.BOSS_MONSTER_HUNTCOUNT = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#QUEST_MONSTER_HUNTCOUNT") == 0)
//		{
//			m_GuildPointRule.QUEST_MONSTER_HUNTCOUNT = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#0000_0999") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount0_999] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#1000_1999") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount1000_1999] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#2000_2999") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount2000_2999] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#3000_3999") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount3000_3999] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#4000_4999") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount4000_4999] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_32") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_32] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_16") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_16] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_8") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_8] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_4") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_4] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_3") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_3] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_2") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_2] = file.GetInt();
//		}
//
//		if(strcmp(szBuf,"#GDT_1") == 0)
//		{
//			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_1] = file.GetInt();
//		}
//	}
//	return TRUE;
//}

void CGuildManager::AddHuntedMonster( CPlayer* player, CObject* object )
{
	ASSERT( player && object );

	if( eObjectKind_Monster != object->GetObjectKind() )
	{
		return;
	}

	CMonster*		monster = ( CMonster* )object;
	CGuildScore*	guild	= GetGuildScore( player->GetGuildIdx() );

	if( guild &&
		player->GetLevel() <= monster->GetLevel() )
	{
		guild->AddScore( mScoreSetting.mHuntScore );
	}

	/*
	int	HuntedCount = 0;

	switch(pHuntedMonster->GetObjectKind())
	{
	case eObjectKind_Monster:
		{
			if( pPlayer->GetMaxLevel() - pHuntedMonster->GetLevel() < 4 )
			{
				HuntedCount = m_GuildPointRule.NOMAL_MONSTER_HUNTCOUNT;
			}
		}
		break;
	case eObjectKind_FieldBossMonster:
		{
			if( pPlayer->GetMaxLevel() - pHuntedMonster->GetLevel() < 9 )
			{
				HuntedCount = m_GuildPointRule.FIELDBOSS_MONSTER_HUNTCOUNT;
			}
		}
		break;
	case eObjectKind_BossMonster:
		{
			HuntedCount = m_GuildPointRule.BOSS_MONSTER_HUNTCOUNT;
		}
		break;
	case eObjectKind_SpecialMonster:	//���� ����
		{
			HuntedCount = m_GuildPointRule.QUEST_MONSTER_HUNTCOUNT;
		}
		break;
	}

	if(HuntedCount)
	{
		pGuild->AddHuntedMonsterCount( HuntedCount );
	}
	*/
}

//void CGuildManager::SendGuildHuntedMonsterTotalCountToMapServerExceptOneself( DWORD GuildIdx, int nTotalCount )
//{
//	DWORD dwCurtime = GetCurTime();
//	//�ش� ��� ���� �������ְ�
//	//�ڽ� ������ ��� �� ������ ����.
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild )
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! SendGuildHuntedMonsterTotalCountToMapServerExceptOneself: pGuild is NULL GuildIdx: %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	SEND_GUILD_HUNTEDMOB_INFO msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_HUNTEDMONSTER_TOTALCOUNT_TO_MAP;
//	msg.dwObjectID = GuildIdx;
//	msg.dwData1 = nTotalCount;
//	msg.dwData2 = dwCurtime;
//	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
//
//	SetGuildHuntedMonsterTotalCountInfo(GuildIdx, nTotalCount, dwCurtime);
//}
//
//void CGuildManager::SetGuildHuntedMonsterTotalCountInfo( DWORD GuildIdx, int nTotalCount, DWORD DBProcessTime )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild ) return;
//
//	pGuild->SetHuntedMonsterTotalCountInfo(nTotalCount, DBProcessTime);
//}
//
//void CGuildManager::GuildConvertHuntedMonsterTotalCountToGuildPoint( DWORD GuildIdx, int nTotalCount )
//{
//	DWORD dwCurtime = GetCurTime();
//
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild ) return;
//
//	int pt = GuildPointFromHuntedMonsterTotalCount(nTotalCount);
//
//	pGuild->SetHuntedMonsterTotalCountInfo( 0, dwCurtime );	//���� �� �ʱ�ȭ.
//
//	//LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildHuntedMonsterTotalCountConverting, pt, nTotalCount, 0 );
//
//	//DB
//	//GuildAddGuildPoint(GuildIdx, pt, eGuildHuntedMonsterTotalCountConverting, nTotalCount);
//
//	//pGuild->AddGuildPoint(pt, eGuildHuntedMonsterTotalCountConverting, nTotalCount);
//}
//
//int CGuildManager::GuildPointFromHuntedMonsterTotalCount( int nTotalCount )
//{
//	if( 0 == nTotalCount )
//	{
//		ASSERT(0);
//		return 0;
//	}
//
//	if( 0 <=  nTotalCount && nTotalCount <= 999 )
//	{
//		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount0_999];
//	}
//	else if( 1000 <=  nTotalCount && nTotalCount <= 1999 )
//	{
//		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount1000_1999];
//	}
//	else if( 2000 <=  nTotalCount && nTotalCount <= 2999 )
//	{
//		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount2000_2999];
//	}
//	else if( 3000 <=  nTotalCount && nTotalCount <= 3999 )
//	{
//		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount3000_3999];
//	}
//	else if( 4000 <=  nTotalCount && nTotalCount <= 4999 )
//	{
//		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount4000_4999];
//	}
//	else if( nTotalCount >= 5000  )
//	{
//		int pt = ((nTotalCount-5000)/50) + 150;
//		return pt;
//	}
//	else
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! GuildPointFromHuntedMonsterTotalCount: TotalCount is Out %d", nTotalCount );
//		ASSERTMSG(0, buf);
//
//		return 0;
//	}
//}

//void CGuildManager::GuildMemberLevelUpToGuildPoint( CPlayer* pPlayer )
//{
//	DWORD GuildIdx = pPlayer->GetGuildIdx();
//	if( 0 == GuildIdx ) return;
//
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild )
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! GuildMemberLevelUpToGuildPoint: pGuild is NULL. PlayerID %d", pPlayer->GetID() );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	int pt = GuildPointFromGuildMemberLevelUp(pPlayer);
//
//	//LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildMemberMaxLevelUp, pt, pPlayer->GetMaxLevel(), pPlayer->GetID());
//
//	//DB
//	//GuildAddGuildPoint(GuildIdx, pt, eGuildMemberMaxLevelUp, pPlayer->GetID());
//}
//
//int CGuildManager::GuildPointFromGuildMemberLevelUp( CPlayer* pPlayer )
//{
//	int AddPoint = 0;
//	//	int stage = pPlayer->GetStage();
//	/*
//	switch(stage)
//	{
//	case eStage_Normal:
//	{
//	*/			if( pPlayer->GetMaxLevel() >= 10 )	// 10���� �̻���� ����
//	{ 
//		AddPoint = pPlayer->GetMaxLevel();
//	}
//	/*		}
//	break;
//	case eStage_Hwa:
//	case eStage_Geuk:
//	{
//	AddPoint = pPlayer->GetMaxLevel() * 3;
//	}
//	break;
//	case eStage_Hyun:
//	case eStage_Tal:
//	{
//	AddPoint = pPlayer->GetMaxLevel() * 5;
//	}
//	break;
//	default:
//	{
//	char buf[128] = {0,};
//	sprintf( buf, "Err! GuildPointFromGuildMemberLevelUp: Out of Stage. PlayerID %d", pPlayer->GetID() );
//	ASSERTMSG(0, buf);
//	}
//	}
//	*/
//	return AddPoint;
//}
//
//void CGuildManager::GuildTournamentResultToGuildPoint( DWORD GuildIdx, WORD wRank )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild )
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! GuildTournamentResultToGuildPoint: pGuild is NULL. GuildIdx %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	//int pt = GuildPointFromGuildTournamentRank(wRank);
//
//	//LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildTournamentMarks, pt, wRank, 0);
//
//	//DB
//	//GuildAddGuildPoint(GuildIdx, pt, eGuildTournamentMarks, wRank);
//}

//int	CGuildManager::GuildPointFromGuildTournamentRank( WORD wRank )
//{
//	int AddPoint = 0;
//
//	switch(wRank)
//	{
//	case eGTStanding_32TH:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_32];
//		}
//		break;
//	case eGTStanding_16TH:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_16];
//		}
//		break;
//	case eGTStanding_8TH:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_8];
//		}
//		break;
//	case eGTStanding_4TH:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_4];
//		}
//		break;
//	case eGTStanding_3RD:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_3];
//		}
//		break;
//	case eGTStanding_2ND:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_2];
//		}
//		break;
//	case eGTStanding_1ST:
//		{
//			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_1];
//		}
//		break;
//	}
//
//	return AddPoint;
//}

//BOOL CGuildManager::LoadGuildPointPlustimeList()
//{
//	CMHFile file;
//	char szBuf[256] = {0,};
//
//	if(!file.Init("./System/Resource/GuildPointPlustimeList.bin", "rb"))
//		return FALSE;
//
//	int KindNum = file.GetInt();
//
//	if(KindNum)
//	{
//		m_pGuildPlustimeInfo = new GUILDPLUSTIME_INFO[KindNum];
//	}
//	else
//		return FALSE;
//
//	int count = 0;
//
//	while(!file.IsEOF())
//	{
//		m_pGuildPlustimeInfo[count].PlusTimeIdx = file.GetInt();
//		m_pGuildPlustimeInfo[count].PlusTimeKind = file.GetDword();
//		SafeStrCpy( m_pGuildPlustimeInfo[count].PlustimeName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
//		m_pGuildPlustimeInfo[count].UsableTimeLen =	file.GetDword();
//		m_pGuildPlustimeInfo[count].NeedPoint =		file.GetDword();
//		m_pGuildPlustimeInfo[count].AddData =		file.GetDword();
//
//		++count;
//
//		if(count == KindNum)
//			break;
//	}
//	return TRUE;
//}

//void CGuildManager::InitGuildPointInfo( DWORD GuildIdx, GUILDPOINT_INFO* pGuildPointInfo )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//
//	if(NULL == pGuild)
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! InitGuildPointInfo: pGuild is NULL GuildIdx: %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	pGuild->InitGuildPointInfo(pGuildPointInfo);
//
//}

//void CGuildManager::InitGuildPlustimeInfo( DWORD GuildIdx, DWORD GuildUsePlustimeIdx, DWORD GuildIUsePlustimeEndtime )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//
//	if(NULL == pGuild)
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! InitGuildPlustimeInfo: pGuild is NULL GuildIdx: %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	AddGuildPlustimeUsingGuildList(pGuild);	//!���� �߿�..pGuild->AddGuildPlustime(
//
//	pGuild->AddGuildPlustime(GuildUsePlustimeIdx, GuildIUsePlustimeEndtime);
//}
//
//void CGuildManager::UseGuildPoint( CPlayer* pPlayer, DWORD UseKind, DWORD ListIdx )
//{
//	DWORD GuildIdx = pPlayer->GetGuildIdx();
//	DWORD NeedPoint = 0;
//
//	if(ForGuildPlusTime == UseKind)
//	{
//		NeedPoint = m_pGuildPlustimeInfo[ListIdx-1].NeedPoint;
//		CGuild* pGuild = GetGuild(GuildIdx);
//		if(NULL == pGuild)
//		{
//			char buf[128] = {0,};
//			sprintf( buf, "Err! UseGuildPoint: pGuild is NULL. PlayerID %d", pPlayer->GetID() );
//			ASSERTMSG(0, buf);
//			return;
//		}
//		//��ȿ�� ���� �÷��� Ÿ�� ��ȣ
//		if( ListIdx > eGuildPlusTime_Max )
//		{
//			char buf[128] = {0,};
//			sprintf( buf, "Err! UseGuildPoint: Out of ListIdx. PlayerID %d", pPlayer->GetID() );
//			ASSERTMSG(0, buf);
//			return;
//		}
//
//		//��밡�� ���� Ȯ��
//		if( pPlayer->GetGuildMemberRank() < GUILD_SENIOR )
//		{
//			char buf[128] = {0,};
//			sprintf( buf, "Err! UseGuildPoint: Under Usable Rank. Rank %d, PlayerID %d", pPlayer->GetGuildMemberRank(), pPlayer->GetID() );
//			ASSERTMSG(0, buf);
//			return;
//		}
//
//		//�÷���Ÿ�� �ߺ� Ȯ��
//		if( pGuild->IsUsingPlustimeIdx(ListIdx) )
//		{
//			char buf[128] = {0,};
//			sprintf( buf, "Err! UseGuildPoint: Dupl_UsingPlustimeIdx. ListIdx %d, PlayerID %d", ListIdx, pPlayer->GetID() );
//			ASSERTMSG(0, buf);
//			return;
//		}
//
//		//�ܿ� ��������Ʈ Ȯ��
//		if(pGuild->GetGuildPoint() < (int)NeedPoint)
//		{
//			char buf[128] = {0,};
//			sprintf( buf, "Err! UseGuildPoint: LessNeedPoint. Point %d, PlayerID %d", pGuild->GetGuildPoint(), pPlayer->GetID() );
//			ASSERTMSG(0, buf);
//			return;
//		}
//
//		//�÷���Ÿ�� ��ȿ�ð� ����
//		SYSTEMTIME systime;
//		stTIME startime, usetime, endtime;
//		GetLocalTime(&systime);
//		startime.SetTime(systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
//
//		DWORD UsableTimeLen = m_pGuildPlustimeInfo[ListIdx-1].UsableTimeLen;
//		endtime = startime;
//		DWORD day = UsableTimeLen/(24*60);
//		DWORD hour = (UsableTimeLen%(24*60))/60;
//		DWORD minute = (UsableTimeLen%(24*60))%60;
//		usetime.SetTime(0, 0, day, hour, minute, 0);
//		endtime += usetime;
//
//		//LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), FALSE, eGuildPlusTimeUse, NeedPoint, ListIdx, pPlayer->GetID());
//
//		//DB
//		//GuildUseGuildPoint(GuildIdx, NeedPoint, ForGuildPlusTime, ListIdx, endtime.value);
//
//		
//	}
//	else if(ForGuildSkill == UseKind)
//	{
//
//	}
//
//
//}

//void CGuildManager::AddGuildPointFromDBReturn( DWORD GuildIdx, DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AdditionalData )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild ) return;
//
//	MSG_DWORD4 msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_POINT_ADD_TO_MAP;
//	msg.dwObjectID = GuildIdx;
//	msg.dwData1 = AddPoint;
//	msg.dwData2 = TotalPoint;
//	msg.dwData3 = eAddKind;
//	msg.dwData4 = AdditionalData;
//
//	g_Network.Send2AgentServer((char*)&msg, sizeof(MSG_DWORD4));
//
//	pGuild->SendGuildPointAddInfoToAll(AddPoint, TotalPoint, eAddKind, AdditionalData);
//
//	SetGuildPoint(GuildIdx, TotalPoint);
//}

//void CGuildManager::UseGuildPointFromDBReturn( DWORD GuildIdx, DWORD UsePoint, DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD Endtime )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if( NULL == pGuild ) return;
//
//	if(eUseKind == ForGuildPlusTime)
//	{
//		//�� ����
//		MSG_DWORD3 msgPlus;
//		msgPlus.Category = MP_GUILD;
//		msgPlus.Protocol = MP_GUILD_PLUSTIME_SET_TO_MAP;
//		msgPlus.dwData1 = GuildIdx;
//		msgPlus.dwData2 = KindIdx;
//		msgPlus.dwData3 = Endtime;
//
//		g_Network.Send2AgentServer((char*)&msgPlus, sizeof(MSG_DWORD3));
//
//		//�÷���Ÿ�� ��븮��Ʈ �߰� !�����߿�.
//		AddGuildPlustimeUsingGuildList(pGuild);
//		//���� �÷��� Ÿ�� ����
//		pGuild->AddGuildPlustime(KindIdx, Endtime);
//
//	}
//	else if(eUseKind == ForGuildSkill)
//	{
//
//	}
//
//	MSG_DWORD4 msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_POINT_USEINFO_TO_MAP;
//	msg.dwObjectID = GuildIdx;
//	msg.dwData1 = TotalPoint;
//	msg.dwData2 = eUseKind;
//	msg.dwData3 = KindIdx;
//	msg.dwData4 = Endtime;
//
//	g_Network.Send2AgentServer((char*)&msg, sizeof(MSG_DWORD4));
//
//	pGuild->SendGuildPointUseInfoToAll(TotalPoint, eUseKind, KindIdx, Endtime);
//
//	SetGuildPoint(GuildIdx, TotalPoint);
//}

//void CGuildManager::SetGuildPoint( DWORD index, DWORD score )
//{
//	CGuild* guild = GetGuild( index );
//
//	if( guild )
//	{
//		guild->SetGuildPoint( score );
//	}
//}

//void CGuildManager::AddGuildPlustimeUsingGuildList( CGuild* pGuild )
//{
//	if(0 != pGuild->GetGuildPlustimeFlg())	return;
//
//	m_GuildPlustimeUsingGuildList.AddTail(pGuild);
//}
//
//void CGuildManager::RemoveGuildPlustimeUsingGuildList( CGuild* pGuild )
//{
//	if(pGuild->GetGuildPlustimeFlg())	return;
//
//	m_GuildPlustimeUsingGuildList.Remove(pGuild);
//}
//
//void CGuildManager::EndGuildPlustimeDBReturn( DWORD GuildIdx, DWORD PlustimeIdx )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//
//	if(NULL == pGuild)
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! EndGuildPlustimeDBReturn: pGuild is NULL. GuildIdx %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//	//�� ����
//	MSG_DWORD msgMap;
//	msgMap.Category = MP_GUILD;
//	msgMap.Protocol = MP_GUILD_PLUSTIME_END_TO_MAP;
//	msgMap.dwObjectID = GuildIdx;
//	msgMap.dwData = PlustimeIdx;
//
//	g_Network.Send2AgentServer((char*)&msgMap,sizeof(MSG_DWORD));
//
//	EndGuildPlustime(GuildIdx, PlustimeIdx);
//}
//
//void CGuildManager::EndGuildPlustime( DWORD GuildIdx, DWORD PlustimeIdx )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//
//	if(NULL == pGuild)
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! EndGuildPlustime: pGuild is NULL. GuildIdx %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return;
//	}
//
//	pGuild->RemoveGuildPlustime(PlustimeIdx);
//
//	RemoveGuildPlustimeUsingGuildList(pGuild);
//}

//DWORD CGuildManager::GetGuildPlustimeKindFromIdx( DWORD GuildPlustimeIdx )
//{
//	GUILDPLUSTIME_INFO* pGPInfo = GetGuildPointPlustimeList();
//
//	DWORD Kind = pGPInfo[GuildPlustimeIdx-1].PlusTimeKind;
//
//	return Kind;
//}

//BOOL CGuildManager::GetGuildPlustimeRt( DWORD GuildIdx, int PlustimeKind, void* pData )
//{
//	CGuild* pGuild = GetGuild(GuildIdx);
//	if(NULL == pGuild)
//	{
//		char buf[128] = {0,};
//		sprintf( buf, "Err! GetGuildPlustimeRt: pGuild is NULL. GuildIdx %d", GuildIdx );
//		ASSERTMSG(0, buf);
//		return FALSE;
//	}
//
//	int GuildPlustime = pGuild->GetGuildPlustimeFlg();
//	if(0 == GuildPlustime)
//	{
//		return FALSE;
//	}
//
//	if( FALSE == pGuild->IsUsingPlustimeKind(PlustimeKind) )
//	{
//		return FALSE;
//	}
//
//	GUILDPOINT_INFO* pPointInfo = pGuild->GetGuildPointInfo();
//
//	DWORD UsingIdxFromKind = 0;
//
//	switch( PlustimeKind )
//	{
//	case eGPT_SuRyun:
//		{
//			DWORD* pAddAbil = (DWORD*)pData;
//			
//			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_SuRyun].PlusTimeIdx;
//
//			*pAddAbil = DWORD((*pAddAbil * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
//		}
//		break;
//	case eGPT_Skill:
//		{
//			DWORD* pAddSkill = (DWORD*)pData;
//
//			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_Skill].PlusTimeIdx;
//
//			*pAddSkill = DWORD((*pAddSkill * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
//		}
//		break;
//	case eGPT_Exp:
//		{
//			DWORD* pAddExp = (DWORD*)pData;
//
//			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_Exp].PlusTimeIdx;
//
//			*pAddExp = DWORD((*pAddExp * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
//		}
//		break;
//	case eGPT_DamageUp:
//		{
//			double* pAttDmg = (double*)pData;
//			double AddDmg = *pAttDmg;
//
//			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_DamageUp].PlusTimeIdx;
//
//			AddDmg = double(AddDmg + (AddDmg * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData/100.f));
//			*pAttDmg = AddDmg;
//		}
//		break;
//	default:
//		{
//			return FALSE;
//		}
//		break;
//	}
//
//	return TRUE;
//}

void CGuildManager::Process()
{
	// Ÿ�� ����Ʈ�� DB�� ������Ʈ �ð��� ���� ���� ��� ������ ����Ǿ� �ִ�
	// ����Ʈ�� head�� �����Ͽ� ó���� �ð��� �Ǿ����� DB�� �����Ѵ�.

	// ��� ���� ���� ó��
	{
		if( mTimeList.empty() )
		{
			return;
		}

		const DWORD		guildIndex	= mTimeList.front();
		CGuildScore*	guild		= GetGuildScore( guildIndex );

		// ��� ������ ������ ó���� ������ ����
		if(	! guild )
		{	
			mTimeList.pop_front();
			return;
		}

		const DWORD tick = GetTickCount();

		// ��� ������ �������� ������ ���� �Ѵ�.
		if( ! guild->IsEnableScore( tick ) )
		{
			return;
		}

		const float score = guild->GetScore( tick );
		
		GuildAddScore( guildIndex, score );

		// 071114 ����, �α�
		InsertLogGuildScore( guildIndex, score, eGuildLog_TimeScoreAdd );

		guild->Reset( tick );

		// ����Ʈ �� �ڷ� �̵���Ų��
		mTimeList.pop_front();
		mTimeList.push_back( guildIndex );
	}
}

//void CGuildManager::GUILDMGR_5MinProcess()
//{
//	GuildHuntedMonsterCountDBUpdate();
//
//	//���� (���� �ѹ���) ����Ʈ ����.
//	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();
//}

//void CGuildManager::GuildHuntedMonsterCountDBUpdate()
//{
//	//10�� ���� �� �ʿ� ������ �ο��� �ִ��� Ȯ���Ͽ� DB �� �ջ갪 �����ֱ�
//	if(gCurTime - m_dwLastDBUpdateTime < m_CHECKTIMELEN_GuildHuntedMonsterCounter)
//	{
//		return;
//	}
//
//	//�� ���� �÷��̾ �ְų� ���� ���� ������ DB ����.
//	int nHuntedMonsterCount = 0;
//	CGuild* pGuild = NULL;
//	m_GuildRoomHashTable.SetPositionHead();
//	while( pGuild = m_GuildRoomHashTable.GetData() )
//	{
//		nHuntedMonsterCount = pGuild->GetHuntedMonsterCount();
//		if( pGuild->GetConnectingMapMemberCount() || nHuntedMonsterCount )
//		{
//			//DB Update
//			//GuildAddHuntedMonsterCount(pGuild->GetIdx(), nHuntedMonsterCount);
//			//pGuild->SetHuntedMonsterCount(0);
//		}
//	}
//
//	m_dwLastDBUpdateTime = gCurTime;
//}
//
//void CGuildManager::CheckDateForGuildHuntedMonsterCountConvertToGuildPoint()
//{
//	if( FALSE == g_pServerSystem->GetMap()->IsVillage() )	return;
//	//��¥ üũ & ���� ��(�������� �����ʸ� ����) üũ
//	//���ǿ� �����ϸ� �ش� ��� �ε����� DB �����Ͽ�
//	//���� ���Ϲ޾� ����Ʈ ��ȯ���ְ� ���� ó��.
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//	CGuild* pGuild = NULL;
//	if( m_wCurDateCheckForConverCount != st.wDay )
//	{
//		m_GuildRoomHashTable.SetPositionHead();
//		while( pGuild = m_GuildRoomHashTable.GetData() )
//		{
//			if( pGuild->GetLocation() == g_pServerSystem->GetMapNum() )	//������ ���� ��常
//			{
//				if( pGuild->GetHuntedMonsterTotalCount() )
//				{
//					//DB Update
//					//GuildGetHuntedMonsterTotalCountWithInit(pGuild->GetIdx());
//				}
//			}
//		}
//
//		m_wCurDateCheckForConverCount = st.wDay;
//	}
//}

//void CGuildManager::CheckGuildPlustimeEnd()
//{
//	if( FALSE == g_pServerSystem->GetMap()->IsVillage() )	return;
//
//	static stTIME curtime;
//	curtime.value = GetCurTime();
//
//	CGuild* pGuild = NULL;
//	PTRLISTPOS pos = m_GuildPlustimeUsingGuildList.GetHeadPosition();
//	while( pos )
//	{
//		pGuild = (CGuild*)m_GuildPlustimeUsingGuildList.GetNext( pos );
//		
//		if( pGuild->GetLocation() == g_pServerSystem->GetMapNum() )	//������ ���� ��常
//		{
//			if( pGuild )
//				if( pGuild->GetGuildPlustimeFlg() )	//����Ʈ ��ϵ� ���ĸ�� ��ȿ��
//				{
//					pGuild->CheckPlustimeEnd(curtime.value);
//				}
//		}
//	}
//}


void CGuildManager::AddPlayer( CPlayer* player )
{
	ASSERT( player );

	const DWORD guildIndex = player->GetGuildIdx();

	if( ! guildIndex )
	{
		return;
	}

	// ��� ������ �߰�
	{
		CGuildScore* guild = GetGuildScore( guildIndex );

		if( ! guild )
		{
			guild = new CGuildScore( mScoreSetting.mUpdateGap, mScoreSetting.mUnitTimeScore / mScoreSetting.mUnitTime );

			mGuildScoreMap.insert( std::pair< DWORD, CGuildScore* >( guildIndex, guild ) );

			mTimeList.push_back( guildIndex );

			// 080417 LUJ, �ʿ� ó�� ������ ������ ������ ��ü ��� ��ų�� �д´�
			g_DB.FreeMiddleQuery(
				RGuildGetSkill,
				guildIndex,
				"SELECT TOP %d SKILL_IDX, SKILL_LEVEL FROM TB_GUILD_SKILL WHERE GUILD_IDX = %d AND SKILL_IDX > 0 ORDER BY SKILL_IDX",
				MAX_MIDDLEROW_NUM,
				guildIndex );
		}
		// 080417 LUJ, ��ų ������ �ʱ�ȭ�� �Ķ�� ���� ��ų ������ �÷��̾�� �����Ѵ�
		else
		{
			CGuild* guild = GetGuild( guildIndex );

			if( guild )
			{
				guild->UpdateSkill( player->GetID() );
				guild->SendSkill( *player );
			}
		}

		ASSERT( guild );
		guild->AddPlayer( player->GetID() );
	}

	// ������ ��û. 
	// ����:	�� �� ���� ���� ���� ������ ���� ������ �޸𸮿� �����͸� �����ϰų� ��ε�ĳ�������� �ʴ´�
	//			�ִ��� DB�� ������ ���������� ����. ������ ���μ����� ȿ�������� �� �� �ִ�.
	{
		const DWORD playerIndex = player->GetID();

		GuildReadData( playerIndex, guildIndex );
		//GuildReadMember( playerIndex, guildIndex );
	}	
}


void CGuildManager::RemovePlayer( const CPlayer* player )
{
	if( ! player )
	{
		ASSERT( 0 );
		return;
	}
	
	const DWORD playerIndex	= player->GetID();
	const DWORD guildIndex	= player->GetGuildIdx();
	
	CGuildScore* guildScore = GetGuildScore( guildIndex );
	
	if( ! guildScore )
	{
		return;
	}

	const float score = guildScore->RemovePlayer( playerIndex );

	if( guildScore->GetPlayerSize() )
	{
		guildScore->AddScore( score );
	}
	// �� �̻� �α��ε� ȸ���� ���� GuildScoreŬ������ ������ ������ �ʿ䰡 ���ٴ� ��.
	// ��� ������ ������ ������Ʈ�ϰ� �޸𸮿��� �����Ѵ�
	else
	{
		GuildAddScore( guildIndex, score + guildScore->GetScore( GetTickCount() ) );

		InsertLogGuildScore( guildIndex, score, eGuildLog_TimeScoreAdd );

		SAFE_DELETE( guildScore );
		mGuildScoreMap.erase( guildIndex );

		// 080417 LUJ, ��� ��ų�� �����Ѵ�
		{
			CGuild* guild = GetGuild( guildIndex );

			if( guild )
			{
				guild->RemoveAllSkill();
			}
		}
	}
	
	GuildCloseWarehouse( guildIndex, playerIndex, g_pServerSystem->GetMapNum() );
}


CGuildManager* CGuildManager::GetInstance()
{
	static CGuildManager instance;

	return &instance;
}


void CGuildManager::LoadScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/guild_setting.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	enum eParseType
	{
		eNone,
		eScore,
		eLevel,
		// 080417 LUJ, ��� ��ų ��ũ��Ʈ
		eSkill,
	}
	parseType = eNone;

	std::list< std::string > text;

	int	openCount	= 0;

	for(	TCHAR line[ MAX_PATH ];
		! file.IsEOF();
		ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( 0 == length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( ! token )
		{
			text.pop_back();
			continue;
		}		
		else if( ! _tcsicmp( token, "score_setting" ) )
		{
			parseType = eScore;
		}
		else if( ! _tcsicmp( token, "level_setting" ) )
		{
			parseType = eLevel;
		}
		// 080417 LUJ, ��� ��ų ��ũ��Ʈ �ε�
		else if( ! _tcsicmp( token, "skill_setting" ) )
		{
			parseType = eSkill;
		}

		if( ! _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( ! _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			if( 0 == --openCount )
			{
				switch( parseType )
				{
				case eScore:
					{
						ParseScoreScript( text );
						break;
					}
				case eLevel:
					{
						ParseLevelScript( text );
						break;
					}
					// 080417 LUJ, ��� ��ų ��ũ��Ʈ �ε�
				case eSkill:
					{
						ParseSkillScript( text );
						break;
					}
				}

				parseType = eNone;

				text.clear();
			}
		}
	}
}


void CGuildManager::ParseScoreScript( const std::list< std::string >& text )
{
	//ZeroMemory( &mScoreSetting, sizeof( mScoreSetting ) );
	{
		mScoreSetting.mUnitTime			= 0;
		mScoreSetting.mUnitTimeScore	= 0;
		mScoreSetting.mHuntScore		= 0;
		mScoreSetting.mUpdateGap		= 0;
		mScoreSetting.mKillerScore		= 0;
		mScoreSetting.mCorpseScore		= 0;
	}

	int					openCount	= 0;
	TCHAR				line[ MAX_PATH ];
	ScoreSetting::Unit*	lostScore	= 0;

	for(	std::list< std::string >::const_iterator it = text.begin();
		text.end() != it;
		++it )
	{
		_tcscpy( line, it->c_str() );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );

		// �̸� �Ľ�
		if( 0 == token )
		{
			continue;
		}

		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			--openCount;
		}

		if( ! _tcsicmp( "unit_time", token ) )
		{
			mScoreSetting.mUnitTime	= _ttoi( _tcstok( 0, separator ) ) * 60 * 1000;
		}
		if( ! _tcsicmp( "unit_time_score", token ) )
		{
			mScoreSetting.mUnitTimeScore	= float( _tstof( _tcstok( 0, separator ) ) );
		}
		else if( ! _tcsicmp( "hunt_score", token ) )
		{
			mScoreSetting.mHuntScore	= float( _tstof( _tcstok( 0, separator ) ) );
		}
		else if( ! _tcsicmp( "update_time", token ) )
		{
			mScoreSetting.mUpdateGap	= _ttoi( _tcstok( 0, separator ) ) * 60 * 1000;
		}
		else if( ! _tcsicmp( "winningValue", token  ) )
		{
			mScoreSetting.mWinScore.mValue		= float( _tstof( _tcstok( 0, separator ) ) );
		}
		else if( ! _tcsicmp( "winningPercent", token  ) )
		{
			mScoreSetting.mWinScore.mPercent	= float( _tstof( _tcstok( 0, separator ) ) ) / 100.0f;
		}
		else if( ! _tcsicmp( "winningMoney", token  ) )
		{
			mScoreSetting.mWinMoneyRate	= float( _tstof( _tcstok( 0, separator ) ) ) / 100.0f;
		}
		else if( ! _tcsicmp( "killerScore", token  ) )
		{
			mScoreSetting.mKillerScore	= float( _tstof( _tcstok( 0, separator ) ) );
		}
		else if( ! _tcsicmp( "corpseScore", token  ) )
		{
			mScoreSetting.mCorpseScore	= float( _tstof( _tcstok( 0, separator ) ) );
		}
		else if( ! _tcsicmp( "scoreRange", token  ) )
		{
			const char* token1		= _tcstok( 0, separator );	// begin score
			const char* token2		= _tcstok( 0, separator );	// ~
			const char* token3		= _tcstok( 0, separator );	// end score

			//const DWORD beginScore	= _ttoi( token1 );
			const DWORD endScore	= token3 ? _ttoi( token3 ) : UINT_MAX;

			// ������ ���ӵǾ� �����Ƿ� ������ �����ϸ� �ȴ�.
			// ���� ��� 0~1000, 1001~2000, 2001~ �̷� ������ ���� �ִٰ� ����.
			// std::map::lower_bound()�� �־��� ������ ���� Ű�� ���� ���� �������Ƿ� ���� ���� ���� �� �ִ�.			
			//
			// ����: stdext:hash_map�� �ùٸ� ����� �������� �ʴ´�.
			lostScore = &( mScoreSetting.mLostScoreMap[ endScore ] );
		}
		else if( ! _tcsicmp( "lostScore", token  ) )
		{
			token = _tcstok( 0, separator );
			ASSERT( 0 );

			if( lostScore && token )
			{
				if( '%' == token[ _tcslen( token ) - 1 ] )
				{
					lostScore->mPercent	= float( _tstof( token ) ) / 100.0f;
				}
				else
				{
					lostScore->mValue	= float( _tstof( token ) );
				}
			}
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}
}


void CGuildManager::ParseLevelScript( const std::list< std::string >& text )
{
	int		openCount	= 0;
	TCHAR	line[ MAX_PATH ];

	LevelSetting* setting = 0;

	for(	std::list< std::string >::const_iterator it = text.begin();
		text.end() != it;
		++it )
	{
		_tcscpy( line, it->c_str() );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );

		// �̸� �Ľ�
		if( 0 == token )
		{
			continue;
		}

		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			--openCount;
		}

		if( ! strcmpi( "level", token ) )
		{
			const DWORD level = _ttoi( _tcstok( 0, separator ) );

			ASSERT( mLevelSettingMap.end() == mLevelSettingMap.find( level ) );

			setting = &( mLevelSettingMap[ level ] );

			// ���� ��� ���� ������ �ִٸ� �״�� �����ϱ� ���� �����Ѵ�
			if( 1 < level && 
				mLevelSettingMap.end() != mLevelSettingMap.find( level - 1 ) )
			{
				*setting = mLevelSettingMap[ level - 1 ];
			}
			else
			{
				ZeroMemory( setting, sizeof( *setting ) );
			}
		}
		else if( ! strcmpi( "master_level", token ) )
		{
			setting->mRequiredMasterLevel = _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "score", token ) )
		{
			setting->mRequiredScore		= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "gold", token ) )
		{
			setting->mRequiredMoney		= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "member_size", token ) )
		{
			setting->mMemberSize		= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "quest", token ) )
		{
			setting->mRequiredQuest		= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "guild_mark", token ) )
		{
			setting->mGuildMark			= ! _tcsicmp( "true", _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "union_size", token ) )
		{
			setting->mUnionSize			= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "apprentice_size", token ) )
		{
			setting->mApprenticeSize	= _ttoi( _tcstok( 0, separator ) );
		}
		else if( ! strcmpi( "warehouse_size", token ) )
		{
			setting->mWarehouseSize	= _ttoi( _tcstok( 0, separator ) );
		}
		// 080417 LUJ, ȹ�� ������ ��� ��ų ����
		else if( ! strcmpi( "skill_size", token ) )
		{
			setting->mSkillSize		= _ttoi( _tcstok( 0, separator ) );
		}
	}
}


CGuildScore* CGuildManager::GetGuildScore( DWORD guildIndex )
{
	GuildMap::iterator it = mGuildScoreMap.find( guildIndex );

	return mGuildScoreMap.end() == it ? 0 : it->second;
}


const CGuildManager::LevelSetting* CGuildManager::GetLevelSetting( DWORD level ) const
{	
	LevelSettingMap::const_iterator it = mLevelSettingMap.find( level );

	return mLevelSettingMap.end() == it ? 0 : &it->second;
}


float CGuildManager::GetLosingScore( float score ) const
{
	ScoreSetting::LostScoreMap::const_iterator it = mScoreSetting.mLostScoreMap.lower_bound( DWORD( score ) );

	if( mScoreSetting.mLostScoreMap.end() == it )
	{
		return 0;
	}

	const ScoreSetting::Unit& unit = it->second;

	return unit.mValue + score * unit.mPercent;
}


const CGuildManager::ScoreSetting& CGuildManager::GetScoreSetting() const
{
	return mScoreSetting;
}

// 080417 LUJ, ��� ��ų ��ũ��Ʈ �ε�
void CGuildManager::ParseSkillScript( const std::list< std::string >& text )
{
	int					openCount	= 0;
	SkillSettingMap*	settingMap	= 0;

	for(
		std::list< std::string >::const_iterator it = text.begin();
		text.end() != it;
		++it )
	{
		char line[ MAX_PATH ] = { 0 };

		strcpy( line, it->c_str() );

		const char* separator	= "~=,+\"() \t";
		const char* token		= strtok( line, separator );

		if( ! token )
		{
			continue;
		}

		const char* markBegin	= _T( "{" );
		const char* markEnd	= _T( "}" );

		if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			--openCount;
		}

		if( ! stricmp( "level", token ) )
		{
			const DWORD level = atoi( strtok( 0, separator ) );

			// 080417 LUJ,	���� ���� ������ �����Ѵ�
			//				������ ���� ���ں��� ū ���ڷ� �����Ǿ�� �Ѵ�. �׷��� ������ ���� ���� ������ ���������� �������� �ʴ´�
			if( mSkillSettingLevelMap.end() != mSkillSettingLevelMap.find( level - 1 ) )
			{
				mSkillSettingLevelMap[ level ] = mSkillSettingLevelMap[ level - 1 ];
			}

			settingMap = &( mSkillSettingLevelMap[ level ] );
		}
		else if( atoi( token ) )
		{
			const int	beginSkillIndex = atoi( token );
			const char* endSkillIndex	= strtok( 0, separator );

			if( settingMap &&
				endSkillIndex )
			{
				SkillSetting& setting = ( *settingMap )[ beginSkillIndex ];
				setting.mLevel = BYTE( abs( beginSkillIndex - atoi( endSkillIndex ) ) ) + 1;

				while( token = strtok( 0, separator ) )
				{
					if( ! stricmp( "master", token ) )
					{
						setting.mRank.insert( GUILD_MASTER );
					}
					else if( ! stricmp( "vice_master", token ) )
					{
						setting.mRank.insert( GUILD_VICEMASTER );
					}
					else if( ! stricmp( "senior", token ) )
					{
						setting.mRank.insert( GUILD_SENIOR );
					}
					else if( ! stricmp( "junior", token ) )
					{
						setting.mRank.insert( GUILD_JUNIOR );
					}
					else if( ! stricmp( "member", token ) )
					{
						setting.mRank.insert( GUILD_MEMBER );
					}
					else if( ! stricmp( "student", token ) )
					{
						setting.mRank.insert( GUILD_STUDENT );
					}
				}
			}
		}
	}
}

// 080417 LUJ, ������ ��� ��ų ���� ��ȯ
const CGuildManager::SkillSettingMap* CGuildManager::GetSkillSetting( DWORD level ) const
{
	SkillSettingLevelMap::const_iterator it = mSkillSettingLevelMap.find( level );

	return mSkillSettingLevelMap.end() == it ? 0 : &( it->second );
}

// 080417 LUJ, �ִ� ��� ������ ��ų ������ ��´�
const CGuildManager::SkillSettingMap*  CGuildManager::GetMaxLevelSkillSetting()	const
{
	DWORD maxLevel = 0;

	for(
		SkillSettingLevelMap::const_iterator it = mSkillSettingLevelMap.begin();
		mSkillSettingLevelMap.end() != it;
		++it )
	{
		maxLevel = max( maxLevel, it->first );
	}

	return GetSkillSetting( maxLevel );
}
