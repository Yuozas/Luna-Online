
#include "stdafx.h"
#include "GuildManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
//#include "GuildCreateDialog.h"
#include "CharacterDialog.h"
#include "GuildInviteDialog.h"
#include "GuildDialog.h"
#include "GuildLevelupDialog.h"
//#include "GuildRankDialog.h"
//#include "GuildNickNameDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildNoticeDlg.h"
#include "GuildPlusTimeDialog.h"
#include "MHMap.h"
#include "GuildMarkManager.h"
#include "MHFile.h"
#include "FilteringTable.h"
#include "GuildFieldWar.h"
#include "SiegeWarMgr.h"
#include "GuildNoteDlg.h"
#include "ObjectStateManager.h"
#include "StatusIconDlg.h"
#include "ItemManager.h"
#include "cMsgBox.h"

#include "../FamilyDialog.h"

// 080417 LUJ, 길드 스킬 획득 위해 참조
#include "../[cc]skill/Client/Manager/SkillManager.h"
#include "../[cc]skill/Client/Info//ActiveSkillInfo.h"
#include "cSkillTreeManager.h"
#include "cSkillTrainingDlg.h"
#include "QuickManager.h"
#include "cResourceManager.h"


CGuildManager::CGuildManager()
{
	LoadScript();
}


CGuildManager::~CGuildManager()
{} 


void CGuildManager::Init()
{
	//LoadLevelUpInfo();
	GUILDMARKMGR->Init();

	mSkillMap.clear();
}

void CGuildManager::Release()
{
	GUILDMARKMGR->Release();

	//if(m_pGuildPlustimeInfo)
	//{
	//	delete[] m_pGuildPlustimeInfo;
	//	m_pGuildPlustimeInfo = NULL;
	//}
}

//void CGuildManager::InitGameIn()
//{
//	//m_Guild.Init(NULL);
//	//SetWaiting(FALSE);
//	//SetRequestPlayerId(0);
//	//m_bActiveFunc = FALSE;
//	//m_wTempLocation = 0;
//	//m_InvitedKind = 0;
//}

void CGuildManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	CHero* hero = OBJECTMGR->GetHero();

	if( ! hero	||
		! hero->IsInited() )
	{
		return;
	}

	switch(Protocol)
	{
	case MP_GUILD_CREATE_ACK:
		{
			SEND_GUILD_CREATE_ACK * pmsg = (SEND_GUILD_CREATE_ACK*)pMsg;

			{
				HERO->SetGuildIdxRank( pmsg->GuildIdx, GUILD_MASTER);
				HERO->SetGuildName( pmsg->GuildName );

				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();

				//GUILDINFO GInfo;
				//GInfo.GuildIdx = guildIndex;
				//SafeStrCpy(GInfo.guildName, guildName, MAX_GUILD_NAME+1);
				//SafeStrCpy(GInfo.MasterName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
				//memset(GInfo.GuildNotice, 0, MAX_GUILD_NOTICE+1);
				//GInfo.MapNum = MAP->GetMapNum();
				//GInfo.GuildLevel = 1;
				//GInfo.UnionIdx = 0;
				//GInfo.MasterIdx = HEROID;
				//m_Guild.Init(&GInfo);

				//GUILDMEMBERINFO Info;
				//Info.InitInfo(HEROID, HERO->GetObjectName(), HERO->GetLevel(), GUILD_MASTER, TRUE);
				//m_Guild.AddMember(&Info);
				//m_Guild.RefreshMember();

				//GAMEIN->GetGuildRankDlg()->ShowGuildRankMode(GInfo.GuildLevel);
				//GAMEIN->GetGuildDlg()->Refresh(&m_Guild);

				//GUILDPOINT_INFO NullInfo;
				//m_Guild.SetGuildPointInfo(&NullInfo);

				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(593));
			}

			{
				CSHFamily* pFamily = HERO->GetFamily();

				if( pFamily && pFamily->Get()->nMasterID )
				{
					CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg();

					if( pDlg )
					{
						pDlg->UpdateAllInfo();
					}
				}
			}
			
			{
				// CGuildCreateDialog
				cDialog* dialog = WINDOWMGR->GetWindowForID( GD_CREATEDLG );
				ASSERT( dialog );

				dialog->SetDisable( FALSE );
				dialog->SetActive( FALSE );
			}
		}
		break;
	case MP_GUILD_BREAKUP_ACK:
		{			
			//BreakUpGuildResult();

			HERO->ClearGuildInfo();
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			GAMEIN->GetGuildDlg()->SetActive(FALSE);
			GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(594));
			
			OBJECTMGR->ApplyOverInfoOptionToAll();

			// 080417 LUJ, 스킬 학습 창을 닫는다
			{
				cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

				if( dialog	&&
					dialog->IsGuildMode() )
				{
					dialog->SetActive( FALSE );
				}
			}

			// 080417 LUJ, 플레이어에 관한 정보가 바뀌면 길드 스킬 정보가 갱신해야 한다
			UpdateSkill();
		}
		break;
	case MP_GUILD_ADDMEMBER_INVITE:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(557));
		}
		break;
	case MP_GUILD_ADDMEMBER_ACK:
		{
			SEND_GUILD_MEMBER_INFO* pmsg = (SEND_GUILD_MEMBER_INFO*)pMsg;
			//m_Guild.AddMember(&pmsg->MemberInfo);
			//m_Guild.RefreshMember();
			// create, addmember
			if(pmsg->MemberInfo.MemberIdx == HEROID)
			{
				hero->SetGuildIdxRank(pmsg->GuildIdx, pmsg->MemberInfo.Rank);
				hero->SetGuildName(pmsg->GuildName);
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			}
			//GAMEIN->GetGuildDlg()->Refresh(&m_Guild);
			//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(37), pmsg->MemberInfo.MemberName);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(558), pmsg->MemberInfo.MemberName, GetRankName(pmsg->MemberInfo.Rank));

			CGuildDialog* dialog = GAMEIN->GetGuildDlg();
			ASSERT( dialog );

			{
				const GUILDMEMBERINFO& member = pmsg->MemberInfo;

				dialog->AddMember( member );
			}
		}
		break;
	case MP_GUILD_DELETEMEMBER_ACK:
		{
			CGuildDialog*						dialog		= ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
			ASSERT( dialog );

			const MSG_DWORDBYTE*	pmsg		= ( MSG_DWORDBYTE* )pMsg;
			const DWORD				playerIndex = pmsg->dwData;
			const GUILDMEMBERINFO*	member		= dialog->GetMember( playerIndex );

			if( ! member )
			{
				return;
			}

			if( playerIndex == HEROID)
			{
				//m_Guild.Release();
				hero->ClearGuildInfo();
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
				
				GAMEIN->GetGuildDlg()->SetActive(FALSE);				
				GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);

				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 559 ) );

				GUILDWARMGR->Init();

				// 080417 LUJ, 플레이어에 관한 정보가 바뀌면 길드 스킬 정보가 갱신해야 한다
				UpdateSkill();
				return;
			}

			//GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			//GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);

			{
				cDialog* dialog = WINDOWMGR->GetWindowForID( GDR_RANKDLG );
				ASSERT( dialog );

				dialog->SetActive( FALSE );
			}

			{
				cDialog* dialog = WINDOWMGR->GetWindowForID( GD_NICKNAMEDLG );
				ASSERT( dialog );

				dialog->SetActive( FALSE );
			}

			// ban
			if(pmsg->bData == eGuild_Delete)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(560), member->MemberName );
			}
			//secede
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(561), member->MemberName );
			}

			// 위에서 member 정보를 포인터로 사용하므로, 미리 지워서 포인터가 무효화되지 않도록 유의하자
			dialog->RemoveMember( playerIndex );
		}
		break;
	case MP_GUILD_INVITE:
		{
			CGuildInviteDialog* dialog = GAMEIN->GetGuildInviteDlg();
			ASSERT( dialog )

			if( dialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 897 ) );
				return;
			}			

			//SetInvitedKind(AsMember);

			//const SEND_GUILD_INVITE* pmsg = (SEND_GUILD_INVITE*)pMsg;
			//SetRequestPlayerId(pmsg->MasterIdx);

			//dialog->SetInfo(pmsg->GuildName, pmsg->MasterName, AsMember);
			dialog->SetData( ( SEND_GUILD_INVITE* )pMsg );
			dialog->SetActive(TRUE);
		}
		break;
	case MP_GUILD_INVITE_DENY:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(562));
		}
		break;
	//case MP_GUILD_INFO:
	//	{
	//		SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
	//		//m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
	//		//SW060719 문파포인트
	//		//m_Guild.SetGuildPointInfo(&pmsg->PointInfo);
	//		//GAMEIN->GetGuildDlg()->Refresh(&m_Guild);
	//		//GAMEIN->GetGuildRankDlg()->ShowGuildRankMode(pmsg->GuildInfo.GuildLevel);
	//	}
	//	break;
	case MP_GUILD_ACCEPT_ACK:
		{
			SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
			hero->SetGuildIdxRank(pmsg->GuildInfo.GuildIdx, GUILD_MEMBER);
			hero->SetGuildName(pmsg->GuildInfo.GuildName);
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			
			//m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
			//SW060719 문파포인트
			//m_Guild.SetGuildPointInfo(&pmsg->PointInfo);
		
			//GAMEIN->GetGuildDlg()->Refresh(&m_Guild);

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(563), GetRankName(hero->GetGuildMemberRank()));			
		}
		break;
	case MP_GUILD_ACCEPT_STUDENT_ACK:
		{
			SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
			hero->SetGuildIdxRank(pmsg->GuildInfo.GuildIdx, GUILD_STUDENT);
			hero->SetGuildName(pmsg->GuildInfo.GuildName);
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			
			//m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
			//SW060719 문파포인트
			//m_Guild.SetGuildPointInfo(&pmsg->PointInfo);

			//GAMEIN->GetGuildDlg()->Refresh(&m_Guild);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(563), GetRankName(hero->GetGuildMemberRank()));			
		}
		break;
	case MP_GUILD_SECEDE_ACK:
		{
			hero->ClearGuildInfo();
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			//m_Guild.Release();
			GAMEIN->GetGuildDlg()->SetActive(FALSE);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(564));

			// guildfieldwar
			GUILDWARMGR->Init();

			// 080417 LUJ, 플레이어에 관한 정보가 바뀌면 길드 스킬 정보가 갱신해야 한다
			UpdateSkill();
		}
		break;
	case MP_GUILD_LEVELUP_ACK:
		{
			CHero*							hero	= OBJECTMGR->GetHero();
			const MSG_GUILD_LEVEL_UP_ACK*	message = ( MSG_GUILD_LEVEL_UP_ACK* )pMsg;

			ASSERT( hero->GetGuildIdx() == message->mGuildIndex );

			DWORD guildLevel = 0;

			// 레벨 세팅
			{
				CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
				ASSERT( dialog );

				dialog->SetLevel( dialog->GetLevel() + 1 );
				dialog->SetScore( message->mGuildScore );

				guildLevel = dialog->GetLevel();

				// 길드 마스터(길마와 이름이 같은가로 구분)라면 돈을 차감한다
				if( ! strcmp( GetMasterName(), hero->GetNickName() ) )
				{
					ASSERT( hero->GetMoney() > message->mUsedMoney );

					hero->SetMoney( hero->GetMoney() - message->mUsedMoney );
				}
			}

			{
				CGuildLevelUpDialog* dialog = ( CGuildLevelUpDialog* )WINDOWMGR->GetWindowForID( GD_LEVELUPDLG );
				ASSERT( dialog );

				dialog->SetLevel( guildLevel );
				dialog->SetDisable( FALSE );
			}

			// 더 이상 길드 세팅이 없을 경우 완성으로 간주한다
			if( mLevelSettingMap.end() == mLevelSettingMap.find( guildLevel + 1 ) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 565 ) );
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 566 ), guildLevel );
			}

			// 080417 LUJ, 스킬 학습 창을 닫는다
			{
				cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

				if( dialog	&&
					dialog->IsGuildMode() )
				{
					dialog->SetActive( FALSE );
				}
			}
		}
		break;
	case MP_GUILD_LEVELUP_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 898 ) );
			break;
		}
	case MP_GUILD_SETENTRYDATE:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			hero->SetGuildEntryDate(pmsg->Name);
		}
		break;
	case MP_GUILD_CHANGERANK_ACK:
		{
			MSG_DWORDBYTE * pmsg = (MSG_DWORDBYTE*)pMsg;

			const DWORD playerIndex = pmsg->dwData;
			const BYTE	rank		= pmsg->bData;

			if( HEROID == playerIndex )
			{
				hero->SetGuildMemberRank(rank);
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(567), GetRankName(rank));

				// 080417 LUJ, 플레이어에 관한 정보가 바뀌면 길드 스킬 정보가 갱신해야 한다
				UpdateSkill();
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(568), GetMemberName(playerIndex), GetRankName(rank));
			}
			//m_Guild.ChangeRank(playerIndex, rank);
			//m_Guild.RefreshMember();

			{
				CGuildDialog* dialog = GAMEIN->GetGuildDlg();
				ASSERT( dialog );

				dialog->SetRank( playerIndex, rank );

				if( dialog->IsActive() )
				{
					dialog->SetActive( TRUE );
				}
			}			
		}
		break;
	case MP_GUILD_CHANGERANK_NACK:
		{
			const MSG_BYTE* message = ( MSG_BYTE* )pMsg;

			// TODO: 같은 메시지가 다른 소스에도 있는지 반드시 검색해볼 것

			switch( message->bData )
			{
			case eGuildErr_ChangeRank_LowerLevel:
			case eGuildErr_ChangeRank_NoAuthority:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "[길드] 대상 회원보다 직위가 낮습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1287 ) );
					break;
				}
			case eGuildErr_ChangeRank_AlreadyHave:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "[길드] 이미 해당 직위를 가지고 있습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 607 ) );
					break;
				}
			case eGuildErr_ChangeRank_Master:
			case eGuildErr_ChangeRank_NotMember:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "[길드] 자신보다 같거나 높은 직위를 부여할 수 없습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1096 ) );
					break;
				}
			case eGuildErr_ChangeRank_NoGraduatedStudent:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "[길드] 수련생은 %d레벨까지 직위를 바꿀 수 없습니다", GUILD_STUDENT_MAX_LEVEL );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1273 ), GUILD_STUDENT_MAX_LEVEL );
					break;
				}
			case eGuildErr_ChangeRank_Student:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "[길드] 길드 인원이 가득 차서 수련생의 직위를 바꿀 수 없습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1286 ) );
					break;
				}
			default:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 899 ) );
					break;
				}
			}

			break;
		}
	//case MP_GUILD_PUTIN_MONEY_ACK: case MP_GUILD_PUTOUT_MONEY_ACK:
	//	{
	//		const MSG_DWORD2* pmsg = ( MSG_DWORD2* )pMsg;

	//		HERO->SetMoney( pmsg->dwData1 );

	//		{
	//			CGuildWarehouseDialog* dialog = GAMEIN->GetGuildWarehouseDlg();
	//			ASSERT( dialog );

	//			dialog->SetMoney( pmsg->dwData2 );
	//		}
	//	}
	//	break;
	case MP_GUILD_CREATE_NACK:
		{
			const MSG_DWORD2 * message = ( MSG_DWORD2*)pMsg;

			switch( message->dwData1 )
			{
				case eGuildErr_Create_Invalid_Name:
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 569 ) );
						break;
					}
				//case eGuildErr_Create_Intro:
				//	{
				//		break;
				//	}
				case eGuildErr_Create_Already:
					{
						//CHATMGR->AddMsg( CTC_SYSMSG, "길드가 있거나 가입된 상태에서는 다시 만들 수 없습니다" );
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 327 ) );
						break;
					}
				case eGuildErr_Create_Low_Level:
					{
						//CHATMGR->AddMsg( CTC_SYSMSG, "길드를 창설하려면 최소 %d레벨이어야 합니다", message->dwData2 );
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1087 ), message->dwData2 );
						break;
					}
				case eGuildErr_Create_Insufficient_Money:
					{
						//CHATMGR->AddMsg( CTC_SYSMSG, "길드를 창설하려면 %s골드가 필요합니다", AddComma( message->dwData2 ) );
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1289 ), AddComma( message->dwData2 ) );
						break;
					}
				case eGuildErr_Create_Unable_Entry_Data:
					{
						const DWORD year	= message->dwData2 / 10000;
						const DWORD month	= message->dwData2 / 100 - year * 100;
						const DWORD day		= message->dwData2 - year * 10000 - month * 100;

						//CHATMGR->AddMsg( CTC_SYSMSG, "길드를 창설하려면 %d년 %d월 %d일 이후에 가능합니다 ", year, month, day );
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1288 ), year, month, day );
						break;
					}
				default:
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 570 ) );
						break;
					}
			}

			// 창을 다시 활성화
			{
				// CGuildCreateDialog
				cDialog* dialog = WINDOWMGR->GetWindowForID( GD_CREATEDLG );
				ASSERT( dialog );

				dialog->SetDisable( FALSE );
			}
		}
		break;
	case MP_GUILD_BREAKUP_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(571));
		}
		break;
	case MP_GUILD_DELETEMEMBER_NACK:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "길드원 제명이 실패했습니다" );
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1290 ) );
		}
		break;
	case MP_GUILD_INVITE_ACCEPT_NACK:
		{
			//문파 해체
			//CHATMGR->AddMsg(CTC_SYSMSG, "길드 해체가 실패했습니다." );
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1291 ) );
		}
		break;
	case MP_GUILDMARK_UPDATE_ACK:
		{
			const MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
			((CPlayer*)HERO)->SetGuildMarkName(pmsg->MarkName);
			//GUILDMARKMGR->OnRecvGuildMarkImg(pmsg);

			GUILDMARKMGR->UpdateGuildMarkImg( pmsg->GuildIdx, pmsg->MarkName, pmsg->imgData );
				
			//마크 갱신
			OBJECTMGR->ApplyOverInfoOptionToAll();
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(572));
		}
		break;
	case MP_GUILDMARK_UPDATE_NACK:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "길드 마크 갱신이 실패했습니다" );
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1292 ) );
		}
		break;
	case MP_GUILDMARK_REQUEST_ACK:
		{
			const MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
			//GUILDMARKMGR->OnRecvGuildMarkImg(pmsg);

			GUILDMARKMGR->UpdateGuildMarkImg( pmsg->GuildIdx, pmsg->MarkName, pmsg->imgData );
			
			//마크 갱신
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_GUILDMARK_REQUEST_NACK:
		{
	//		MSG_DWORD* pmsg = (MSG_DWORD*)pmsg;
			int a = 1;
		}
		break;
	case MP_GUILDMARK_CHANGE:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData1);
			ASSERT(pObject);
			if( !pObject ) return;
			
			if(pmsg->dwData2 == 0) // not in guild
			{
				SetPlayerGuildName(pObject, 0, "");
				SetPlayerNickName(pObject, "");
			}
			MarkChange(pmsg);
			if((pmsg->dwData3 > 0) && (HEROID == pmsg->dwData1))
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(572));
		}
		break;
	case MP_GUILD_MEMBERLEVEL_NOTIFY:
		{
			const MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			
			if( hero->GetGuildIdx() )
			{
				CGuildDialog* dialog = GAMEIN->GetGuildDlg();

				dialog->SetMemberLevel( pmsg->dwData1, pmsg->dwData2 );
			}
		}
		break;
	case MP_GUILDNAME_CHANGE:
		{
			SEND_GUILD_CREATE_ACK * pmsg = (SEND_GUILD_CREATE_ACK*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			ASSERT(pObject);
			if( !pObject ) return;
			SetPlayerGuildName(pObject, pmsg->GuildIdx, pmsg->GuildName);			
		}
		break;
	case MP_GUILD_LOGINFO:
		{
			// 로그아웃 처리는 이걸 사용할 것! 기존 로그아웃 체크 처리 모두 제거할 것...

			CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
			ASSERT( dialog );

			const SEND_GUILDMEMBER_LOGININFO* message = (SEND_GUILDMEMBER_LOGININFO*)pMsg;

			if( message->bLogIn )
			{
				dialog->LoginMember( message->MemberIdx );
			}
			else
			{
				dialog->LogoutMember( message->MemberIdx  );
			}
		}
		break;
	case MP_GUILD_GIVENICKNAME_ACK:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->TargetIdx);
			ASSERT(pObject);
			if( !pObject ) return;
			SetPlayerNickName(pObject, pmsg->NickName);
			OBJECTMGR->ApplyOverInfoOptionToAll();
			if(HEROID == pmsg->TargetIdx)
			{				
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(573));
			}
		}
		break;
	case MP_GUILD_GIVENICKNAME_NACK:
		{
			MSG_BYTE * pmsg = (MSG_BYTE *)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(551));	
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( hero->GetGuildIdx() == pmsg->dwData1 )
			{
				//m_Guild.SetGuildPosition( pmsg->dwData2 );
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(574), GetMapName( (MAPTYPE)pmsg->dwData2 ) );

				// 06.09.09 RaMa 창고가 열려 있으면 닫는다.
				if( GAMEIN->GetGuildWarehouseDlg()->IsActive() )
				{
					GAMEIN->GetGuildWarehouseDlg()->SetActive( FALSE );
					if( hero->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				}
				// 문주의 스테이트도 풀어준다.
				if( hero->GetGuildMemberRank() == GUILD_MASTER )
				{
					if( hero->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				}
			}
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
			case eGuildErr_NoGuild:				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(578) );	break;
			case eGuildErr_InviteApt_NoMaster:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(579) );	break;
			case eGuildErr_Nick_Filter:			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(580) );	break;
			case eGuildErr_Nick_NotMember:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(581) );	break;
			case eGuildErr_Nick_NotAvailableName:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(582) );	break;
			}

			if( hero->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case  MP_GUILD_CHANGE_LOCATION_MONEY:
		{
			MSG_DWORD* pmsg =(MSG_DWORD*)pMsg;
			
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(583), pmsg->dwData );
		}
		break;
	case MP_GUILD_NOTE_ACK:
		{
			GAMEIN->GetGuildNoteDlg()->Use();
			CHATMGR->AddMsg(CTC_TOWHOLE, CHATMGR->GetChatMsg(584));
		}
		break;
	case MP_GUILD_NOTICE_ACK:
		{
			const MSG_GUILDNOTICE*	message =(MSG_GUILDNOTICE*)pMsg;
			const char*				notice	= message->Msg;
			
			// 길드 공지 창에도 세팅. 왜? 부마스타 등이 관리하기 편하게
			{
				CGuildNoticeDlg* dialog = ( CGuildNoticeDlg* )WINDOWMGR->GetWindowForID( GUILDNOTICE_DLG );
				ASSERT( dialog );

				dialog->SetNotice( notice );
			}
			
			if( strlen( notice ) )
			{
				CHATMGR->AddMsg(CTC_GUILDCHAT, CHATMGR->GetChatMsg(585), notice );
			}
		}
		break;
	case MP_GUILD_ADDSTUDENT_INVITE:
		{
			//AddChatMsg //
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(557));
		}
		break;

	case MP_GUILD_ADDMEMBER_NACK:
	case MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK:
	case MP_GUILD_ADDSTUDENT_NACK:
		{
			const MSG_NAME_DWORD* message = ( MSG_NAME_DWORD* )pMsg;

			switch( message->dwData )
			{
			case eGuildErr_AddMember_OtherGuild:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 599 ) );
					break;
				}
			case eGuildErr_AddMember_AlreadyMember:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 600 ) );
					break;
				}
			case eGuildErr_AddMember_TargetNotDay:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "초대한 분은 탈퇴 혹은 추방 등으로 인해 %s까지 가입할 수 없습니다", message->Name );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1293 ), message->Name );
					break;
				}				
			case eGuildErr_AddMember_FullMember:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 602 ) );
					break;
				}				
			case eGuildErr_AddStudent_TooHighLevelAsStudent:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 603 ) );
					break;
				}
			case eGuildErr_AddMember_NothaveAuthority:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 323 ) );
					break;
				}
			case eGuildErr_InviteApt_NoMaster:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "의장이 접속해 있지 않습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1294 ) );
					break;
				}
			case eGuildErr_AddMember_NotPlayer:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 211 ) );
					break;
				}
			case eGuildErr_AddStudent_Full:
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "수련생을 더 받을 수 없습니다" );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1295 ) );
					break;
				}
			default:
				{
					ASSERT( 0 && "It's no defined error about guild" );
					break;
				}
			}
		}
		break;
	case MP_GUILD_INVITE_AS_STUDENT:
		{
			CGuildInviteDialog* dialog = GAMEIN->GetGuildInviteDlg();
			ASSERT( dialog );

			if( dialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 900 ) );
				return;
			}

			dialog->SetData( ( SEND_GUILD_INVITE* )pMsg );
			dialog->SetActive(TRUE);
		}
		break;
	case MP_GUILD_SCORE_UPDATE:
		{
			const MSG_DWORD2*	pmsg	= (MSG_DWORD2*)pMsg;
			
			SetScore( pmsg->dwData2 );
		}
		break;
	case MP_GUILD_DATA_READ:
		{
			const MSG_GUILD_DATA* message = ( MSG_GUILD_DATA* )pMsg;

			{
				CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
				ASSERT( dialog );

				dialog->SetData( *message );
			}

			{
				CGuildNoticeDlg* dialog = ( CGuildNoticeDlg* )WINDOWMGR->GetWindowForID( GUILDNOTICE_DLG );
				ASSERT( dialog );

				dialog->SetNotice( message->mNotice );
			}

			if( strlen( message->mNotice ) )
			{
				CHATMGR->AddMsg( CTC_GUILDCHAT, CHATMGR->GetChatMsg(585), message->mNotice );
			}
			else
			{
				CHATMGR->AddMsg( CTC_GUILDCHAT, CHATMGR->GetChatMsg( 901 ) );
			}

			// 080417 LUJ, 스킬 목록이나 길드 정보 중 어떤 것이 올지 모르므로 한번 더 초기화해주자
			UpdateSkill();
			break;
		}
	// 창고 오픈 승인
	case MP_GUILD_OPEN_WAREHOUSE_ACK:
		{
			const MSG_GUILD_OPEN_WAREHOUSE* message = ( MSG_GUILD_OPEN_WAREHOUSE* )pMsg;

			// todo: 창고 탭 켜고 끌 수 있게 개수 세팅

			{
				CGuildWarehouseDialog* dialog = GAMEIN->GetGuildWarehouseDlg();

				dialog->SetTabSize( message->mWarehouseSize );
				dialog->SetMoney( message->mMoney );
			}

			break;
		}
	// 창고 오픈 거부
	case MP_GUILD_OPEN_WAREHOUSE_NACK:
		{
			const MSG_GUILD_OPEN_WAREHOUSE* message = ( MSG_GUILD_OPEN_WAREHOUSE* )pMsg;

			CHero* hero = OBJECTMGR->GetHero();
			ASSERT( hero );

			if( ! message->mWarehouseSize )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 613 ) );
				return;
			}
			else if( strcmp( message->mName, hero->GetObjectName() ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 903 ), message->mName );
			}
			else if( message->mWarehouseRank > hero->GetGuildMemberRank() )
			{
				CHATMGR->AddMsg( 
					CTC_SYSMSG,
					CHATMGR->GetChatMsg( 902 ),
					GetRankName( message->mWarehouseRank ),
					GetRankName( hero->GetGuildMemberRank() ) );
			}
			else
			{
				CGuildWarehouseDialog* dialog = GAMEIN->GetGuildWarehouseDlg();
				ASSERT( dialog );

				dialog->SetLock( FALSE );
				dialog->SetActive( FALSE );
			}
			
			break;
		}
	case MP_GUILD_ITEM_READ:
		{
			CGuildWarehouseDialog* dialog = GAMEIN->GetGuildWarehouseDlg();

			dialog->AddItem( ( MSG_GUILD_ITEM* )pMsg );
			dialog->SetLock( FALSE );
			dialog->SetActive( TRUE );
			break;
		}
	case MP_GUILD_SET_WAREHOUSE_RANK_ACK:
		{
			const MSG_GUILD_SET_WAREHOUSE_RANK* message = ( MSG_GUILD_SET_WAREHOUSE_RANK* )pMsg;
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 614 ), GetRankName( message->mRank ) );

			break;
		}
	case MP_GUILD_RETIRE_SYN:
		{
			if( WINDOWMGR->GetWindowForID( MBI_GUILD_RETIRE_REQUEST ) )
			{
				// 이미 처리 중
				return;
			}

			WINDOWMGR->MsgBox( MBI_GUILD_RETIRE_REQUEST, MBT_YESNO, CHATMGR->GetChatMsg( 615 ) );
			break;
		}
	case MP_GUILD_RETIRE_NACK:
		{
			const MSG_DWORD*	message = ( MSG_DWORD* )pMsg;
			CGuildDialog*		dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
			ASSERT( dialog );

			const GUILDMEMBERINFO* member = dialog->GetMember( message->dwData );

			if( member )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1100 ), member->MemberName );
			}
			else
			{
				ASSERT( 0 );
			}

			break;
		}
	case MP_GUILD_CHANGE_MASTER_TO_USER:
		{
			const MSG_DWORD2*	message		= ( MSG_DWORD2* )pMsg;
			const DWORD			guildIndex	= message->dwData1;
			const DWORD			masterIndex = message->dwData2;

			CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
			
			if( ! dialog )
			{
				break;
			}

			const DWORD oldMasterIndex = dialog->GetMasterIndex();

			CHero* hero = OBJECTMGR->GetHero();
			ASSERT( hero );

			if( hero->GetID() == oldMasterIndex )
			{
				hero->SetGuildMemberRank( GUILD_MEMBER );

				if( dialog->IsActive() )
				{
					dialog->SetActive( TRUE );
				}
			}

			// 예전 마스터는 member로 변경하는 것으로 정해져 있음
			dialog->SetRank( dialog->GetMasterIndex(), GUILD_MEMBER );
			dialog->SetRank( masterIndex, GUILD_MASTER );
			
			if( hero->GetID() == masterIndex )
			{
				hero->SetGuildMemberRank( GUILD_MASTER );

				if( dialog->IsActive() )
				{
					dialog->SetActive( TRUE );
				}

				CHATMGR->AddMsg( CTC_GUILDCHAT, CHATMGR->GetChatMsg( 1101 ) );
			}
			else
			{
				const GUILDMEMBERINFO* member = dialog->GetMember( masterIndex );
				ASSERT( member );

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 988 ), member->MemberName );
			}
			
			// 080417 LUJ, 스킬 학습 창을 닫는다
			{
				cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

				if( dialog	&&
					dialog->IsGuildMode() )
				{
					dialog->SetActive( FALSE );
				}
			}

			// 080417 LUJ, 플레이어에 관한 정보가 바뀌면 길드 스킬 정보를 갱신해야 한다
			UpdateSkill();
			break;
		}
	// 다른 길드의 멤버를 살해하여 포인트를 얻은 경우
	case MP_GUILD_SCORE_UPDATE_BY_KILL:
		{
			const MSG_DWORD3*			message		= ( MSG_DWORD3* )pMsg;
			CPlayer*					killer		= ( CPlayer* )OBJECTMGR->GetObject( message->dwData1 );
			CPlayer*					corpse		= ( CPlayer* )OBJECTMGR->GetObject( message->dwData2 );

			const DWORD					killScore	= DWORD( mScoreSetting.mKillerScore );
			const DWORD					score		= message->dwData3;
			const MSG_GUILD_LIST::Data*	info		= GUILDWARMGR->GetEnemy( corpse ? corpse->GetGuildIdx() : 0 );

			const char*					anonymous	= CHATMGR->GetChatMsg( 1119 );
			const char*					killerName	= ( killer ? killer->GetObjectName() : anonymous );
			const char*					corpseName	= ( corpse ? corpse->GetObjectName() : anonymous );
			const char*					guildName	= ( info ? info->GuildName : CHATMGR->GetChatMsg( 1120 ) );
			
			if( UINT_MAX > score )
			{
				SetScore( score );

				CHATMGR->AddMsg(
					CTC_GUILDCHAT,
					CHATMGR->GetChatMsg( 1121 ),
					killerName,
					guildName,
					corpseName,
					killScore );
			}
			else
			{
				CHATMGR->AddMsg(
					CTC_GUILDCHAT,
					CHATMGR->GetChatMsg( 1122 ),
					killerName,
					guildName,
					corpseName );
			}

			// 080417 LUJ, 스킬 학습 창을 닫는다
			{
				cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

				if( dialog	&&
					dialog->IsGuildMode() )
				{
					dialog->SetActive( FALSE );
				}
			}

			break;
		}
	// 다른 길드의 멤버에게 살해당해 포인트를 잃은 경우
	case MP_GUILD_SCORE_UPDATE_BY_DEAD:
		{
			const MSG_DWORD3*			message		= ( MSG_DWORD3* )pMsg;
			CPlayer*					killer		= ( CPlayer* )OBJECTMGR->GetObject( message->dwData1 );
			CPlayer*					corpse		= ( CPlayer* )OBJECTMGR->GetObject( message->dwData2 );

			const DWORD					score		= message->dwData3;
			const DWORD					corpseScore	= DWORD( mScoreSetting.mCorpseScore );
			const MSG_GUILD_LIST::Data*	info		= GUILDWARMGR->GetEnemy( killer ? killer->GetGuildIdx() : 0 );

			const char*					anonymous	= CHATMGR->GetChatMsg( 1119 );
			const char*					guildName	= ( info ? info->GuildName : CHATMGR->GetChatMsg( 1120 ) );
			const char*					killerName	= ( killer ? killer->GetObjectName() : anonymous );
			const char*					corpseName	= ( corpse ? corpse->GetObjectName() : anonymous );

			if( UINT_MAX > score )
			{
				SetScore( score );

				CHATMGR->AddMsg(
					CTC_GUILDCHAT, 
					CHATMGR->GetChatMsg( 1123 ),
					corpseName,
					guildName,
					killerName,
					corpseScore );
			}
			else
			{
				CHATMGR->AddMsg(
					CTC_GUILDCHAT,
					CHATMGR->GetChatMsg( 1124 ),
					corpseName,
					guildName,
					killerName );
			}

			// 080417 LUJ, 스킬 학습 창을 닫는다
			{
				cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

				if( dialog	&&
					dialog->IsGuildMode() )
				{
					dialog->SetActive( FALSE );
				}
			}
            
			break;
		}
	// 080225 LUJ, 길드 회원에 대한 정보가 도착한 경우 정보를 갱신해야한다
	case MP_GUILD_SET_MEMBER_ACK:
		{
			const SEND_GUILD_MEMBER_INFO* message = ( SEND_GUILD_MEMBER_INFO* )pMsg;

			CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );

			if( ! dialog )
			{
				return;
			}

			dialog->SetMember( message->MemberInfo );
			break;
		}
	// 080417 LUJ, 길드가 보유한 스킬 정보
	case MP_GUILD_GET_SKILL_ACK:
		{
			const MSG_SKILL_LIST* message = ( MSG_SKILL_LIST* )pMsg;

			for( DWORD i = 0; i < message->mSize; ++i )
			{
				const SKILL_BASE& skill = message->mData[ i ];

				mSkillMap[ skill.wSkillIdx ] = skill.Level;
			}

			UpdateSkill();
			break;
		}
	// 080417 LUJ, 길드 스킬 획득
	case MP_GUILD_ADD_SKILL_ACK:
		{
			const MSG_SKILL_UPDATE*		message			= ( MSG_SKILL_UPDATE* )pMsg;
			const SKILL_BASE&			skill			= message->SkillBase;
			cActiveSkillInfo*			activeSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skill.wSkillIdx -1 + skill.Level );
			const ACTIVE_SKILL_INFO*	skillInfo		= ( activeSkillInfo ? activeSkillInfo->GetSkillInfo() : 0 );

			if( ! skillInfo )
			{
				break;
			}

			mSkillMap[ skill.wSkillIdx ] = skill.Level;

			SetScore( max( 0, int( GUILDMGR->GetScore() ) - int( skillInfo->TrainPoint ) ) );
			
			// 080417 LUJ, 스킬 학습 창 갱신
			{
				cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );

				if( dialog &&
					dialog->IsGuildMode() )
				{
					dialog->SetDisable( FALSE );
					dialog->OpenGuildSkill();
				}
			}

			// 080417 LUJ, 메시지 표시
			{
				static const std::string textGuild( RESRCMGR->GetMsg( 131 ) );
				static const std::string textLevel( RESRCMGR->GetMsg( 309 ) );

				char text[ MAX_PATH * 5 ] = { 0 };

				sprintf(
					text, 
					"%s(%d%s)",
					skillInfo->Name,
					skill.Level,
					textLevel.c_str() );

				// 080417 LUJ, 스킬 획득 메시지
				CHATMGR->AddMsg( CTC_GUILD_NOTICE, CHATMGR->GetChatMsg( 116 ), text );
				
				// 080417 LUJ, 비용 지불은 마스터가 하므로, 마스터에게만 표시해준다.
				if( GUILD_MASTER == HERO->GetGuildMemberRank() )
				{
					sprintf( text, CHATMGR->GetChatMsg( 378 ), AddComma( skillInfo->TrainMoney ) );
					CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", textGuild.c_str(), text );
				}				
			}

			UpdateSkill();
			break;
		}
	// 080417 LUJ, 길드 스킬 획득 실패
	case MP_GUILD_ADD_SKILL_NACK:
		{
			// 080417 LUJ, 오류 메시지 표시
			{
				const MSG_BYTE*				message = ( MSG_BYTE* )pMsg;
				static const std::string	textGuild( RESRCMGR->GetMsg( 131 ) );

				switch( message->bData )
				{
				case eGuildErr_InsufficentMoney:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", textGuild.c_str(), CHATMGR->GetChatMsg( 324 ) );
						break;
					}
				case eGuildErr_InsufficentScore:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", textGuild.c_str(), CHATMGR->GetChatMsg( 616 ) );
						break;
					}
				case eGuildErr_NoMaster:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, CHATMGR->GetChatMsg( 976 ) );
						break;
					}
				case eGuildErr_NoScript:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", textGuild.c_str(), CHATMGR->GetChatMsg( 538 ) );
						break;
					}
				case eGuildErr_Skill_Max:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s(size over)", textGuild.c_str(), CHATMGR->GetChatMsg( 1444 ) );
						break;
					}
				case eGuildErr_Skill_InvalidLevel:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s(invalid level)", textGuild.c_str(), CHATMGR->GetChatMsg( 1444 ) );
						break;
					}
				case eGuildErr_Skill_NotExist:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s(not exist)", textGuild.c_str(), CHATMGR->GetChatMsg( 1444 ) );
						break;
					}
				default:
					{
						CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", CHATMGR->GetChatMsg( 1444 ) );
						break;
					}
				}
			}
			
			// 080417 LUJ, 스킬 학습 창 갱신
			{
				cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );

				if( dialog &&
					dialog->IsGuildMode() )
				{
					dialog->SetDisable( FALSE );
				}
			}

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


const char* CGuildManager::GetMemberName(DWORD MemberID) const
{
	//return m_Guild.GetMemberName(MemberID);
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	const GUILDMEMBERINFO* member = dialog->GetMember( MemberID );

	return member ? member->MemberName : "";
}


void CGuildManager::MarkChange(MSG_DWORD3 * pInfo)
{
	CObject* pObject = OBJECTMGR->GetObject(pInfo->dwData1);
	ASSERT(pObject);
	if( !pObject ) return;

	((CPlayer*)pObject)->SetGuildIdx(pInfo->dwData2);
	((CPlayer*)pObject)->SetGuildMarkName(pInfo->dwData3);
	
	pObject->SetGuildMark();
	
	OBJECTMGR->ApplyOverInfoOptionToAll();
}


BOOL CGuildManager::CanEntryGuild(char* date)
{
	// fot testserver siegewar test
	return TRUE;

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	DWORD year;
	DWORD month;
	DWORD day;

	char buf[5];
	strncpy(buf, &date[0],4);
	year = atoi(buf);
	if( ti.wYear > year)
		return TRUE;
	else if(ti.wYear < year)
		return FALSE;

	char dbuf[8];
	strncpy(dbuf, &date[5], 2);
	month = atoi(dbuf);
	
	if(ti.wMonth > month)
		return TRUE;
	else if(ti.wMonth < month)
		return FALSE;
	
	strncpy(dbuf, &date[8], 2);
	day = atoi(dbuf);
	if(ti.wDay < day)
		return FALSE;
	
	return TRUE;
}



const GUILDMEMBERINFO* CGuildManager::GetSelectedMember() const
{
	CGuildDialog* dialog = GAMEIN->GetGuildDlg();
	ASSERT( dialog );

	return dialog->GetSelectedMember();
}


const char* CGuildManager::GetRankName(BYTE rank)
{
	switch(rank)
	{
	case GUILD_MASTER:
		{
			return CHATMGR->GetChatMsg(749);
		}
	case GUILD_VICEMASTER:
		{
			return CHATMGR->GetChatMsg(750);
		}
	case GUILD_SENIOR:
		{
			return CHATMGR->GetChatMsg(751);
		}
	case GUILD_JUNIOR:
		{
			return CHATMGR->GetChatMsg(752);
		}
	case GUILD_MEMBER:
		{
			return CHATMGR->GetChatMsg(753);
		}
	case GUILD_STUDENT:
		{
			return CHATMGR->GetChatMsg(754);
		}
	default:
		{
			ASSERTMSG(0, "해당되는 문파 지위가 없습니다. Invalid Guild Rank.");
			return "";
		}
	}
}


const char* CGuildManager::GetGuildName() const
{
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetGuildName();
}


DWORD CGuildManager::GetScore() const
{ 
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetScore();
}


DWORD CGuildManager::GetLevel() const
{ 
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetLevel();
}


const char* CGuildManager::GetMasterName() const
{ 
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetMasterName();

	//return m_Guild.GetMasterName();
}

DWORD CGuildManager::GetMemberSize() const
{ 
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetMemberSize();
}


DWORD CGuildManager::GetStudentSize() const
{ 
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	return dialog->GetStudentSize();
}



void CGuildManager::SetPlayerGuildName(CObject* pObject, DWORD dwGuildIdx, char* GuildName)
{
	((CPlayer*)pObject)->SetGuildName(GuildName);
	if(strlen(GuildName) == 0)
	{
		((CPlayer*)pObject)->SetGuildIdx(0);
		((CPlayer*)pObject)->SetGuildUnionIdx(0);
		((CPlayer*)pObject)->SetGuildUnionName("");
	}
	if( dwGuildIdx )
		((CPlayer*)pObject)->SetGuildIdx(dwGuildIdx);
}

void CGuildManager::SetPlayerNickName(CObject* pObject, char* NickName)
{
	((CPlayer*)pObject)->SetNickName(NickName);
	pObject->SetNickName();
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
		// 080417 LUJ, 길드 스킬 스크립트
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

		// 중간의 주석 제거
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
		// 080417 LUJ, 길드 스킬 스크립트 로딩
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
					// 080417 LUJ, 길드 스킬 스크립트 로딩
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

		// 이름 파싱
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

			// 구간이 연속되어 있으므로 끝값만 설정하면 된다.
			// 예를 들어 0~1000, 1001~2000, 2001~ 이런 식으로 같이 있다고 하자.
			// std::map::lower_bound()는 주어진 값보다 작은 키를 가진 값을 가져오므로 쉽게 값을 얻을 수 있다.			
			//
			// 주의: stdext:hash_map은 올바른 결과를 보장하지 않는다.
			lostScore = &( mScoreSetting.mLostScoreMap[ endScore ] );
		}
		else if( ! _tcsicmp( "lostScore", token  ) )
		{
			token = _tcstok( 0, separator );
			ASSERT( token );
			
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

		// 이름 파싱
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

			// 이전 길드 레벨 정보가 있다면 그대로 적용하기 위해 복사한다
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
		// 080417 LUJ, 획득 가능한 길드 스킬 개수
		else if( ! strcmpi( "skill_size", token ) )
		{
			setting->mSkillSize		= _ttoi( _tcstok( 0, separator ) );
		}
	}
}


const CGuildManager::LevelSetting* CGuildManager::GetLevelSetting( DWORD level ) const
{
	LevelSettingMap::const_iterator it = mLevelSettingMap.find( level );

	return mLevelSettingMap.end() == it ? 0 : &( it->second );
}


void CGuildManager::SetScore( DWORD score )
{
	CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	dialog->SetScore( score );
}

// 080417 LUJ, 길드 스킬 스크립트 로딩
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

			// 080417 LUJ,	이전 설정 정보를 복사한다
			//				레벨은 작은 숫자부터 큰 숫자로 증가되어야 한다. 그렇지 않으면 이전 설정 정보가 정상적으로 설정되지 않는다
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

// 080417 LUJ, 레벨별 길드 스킬 설정 반환
const CGuildManager::SkillSettingMap* CGuildManager::GetSkillSetting( DWORD level ) const
{
	SkillSettingLevelMap::const_iterator it = mSkillSettingLevelMap.find( level );

	return mSkillSettingLevelMap.end() == it ? 0 : &( it->second );
}

// 080417 LUJ, 최대 길드 레벨의 스킬 정보를 얻는다
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

// 080417 LUJ, 길드 스킬 변경
void CGuildManager::UpdateSkill()
{
	CHero* hero	= OBJECTMGR->GetHero();

	if( ! hero )
	{
		return;
	}

	const SkillSettingMap* settingMap = GetSkillSetting( GetLevel() );

	// 080417 LUJ, 현재 레벨에 대한 설정이 없을 경우 최대 레벨 시의 설정 정보를 읽는다
	if( ! settingMap )
	{
		settingMap = GetMaxLevelSkillSetting();

		if( ! settingMap )
		{
			return;
		}
	}
	
	// 080417 LUJ, 보유한 길드 스킬을 초기화한다
	for(
		SkillMap::const_iterator it = mSkillMap.begin();
		mSkillMap.end() != it;
		++it )
	{
		const DWORD index = it->first;
		
		SKILLTREEMGR->RemoveSetSkill( index );
		QUICKMGR->RefreshSkillItem( index );
	}

	// 080417 LUJ, 직위에 맞게 길드 스킬을 부여한다
	for(
		SkillMap::const_iterator it = mSkillMap.begin();
		mSkillMap.end() != it;
		++it )
	{
		const DWORD index	= it->first;
		const BYTE	level	= it->second;

		SkillSettingMap::const_iterator setting_it = settingMap->find( index );

        if( settingMap->end() == setting_it )
		{
			continue;
		}

		const SkillSetting& setting = setting_it->second;

		// 080417 LUJ, 보유 가능한지 체크하고 부여한다
		if( setting.mRank.end() != setting.mRank.find( hero->GetGuildMemberRank() ) )
		{
			SKILLTREEMGR->AddSetSkill( index, level );
			QUICKMGR->RefreshSkillItem( index );
		}
	}
}

// 080417 LUJ, 스킬이 없다면 0이 반환된다
BYTE CGuildManager::GetSkillLevel( DWORD index ) const
{
	SkillMap::const_iterator it = mSkillMap.find( index );

	return mSkillMap.end() == it ? 0 : it->second;
}
