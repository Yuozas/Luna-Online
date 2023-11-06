#include "stdafx.h"
#include "MSSystem.h"
#include "Network.h"
#include "DBMessage.h"
#include "UserManager.h"
#include "server.h"
#include "..\common\version.h"


bool CMSSystem::mIsRunning;


CMSSystem::CMSSystem()
{
	//m_bInit = false;

	Start();
}

CMSSystem::~CMSSystem()
{
	mIsRunning = false;

	End();
}

void CMSSystem::Start()
{
	char host[128],ipName[2][20];
	HOSTENT* hostent;

	WSADATA wsaData ;
	WSAStartup(0x0002,&wsaData);
	gethostname(host,128);
	hostent=gethostbyname(host);

	for(int n=0;n<2;++n)
	{
		if(hostent->h_addr_list[n] == NULL)
			break;

		strncpy(ipName[n],inet_ntoa(*(struct in_addr *)hostent->h_addr_list[n]),20);
	}

	WSACleanup();

	CUSTOM_EVENT ev[1] = { 0 };
	{
		ev[0].dwPeriodicTime	= 1;
		ev[0].pEventFunc		= CMSSystem::Process;
	}	

	DESC_NETWORK desc = { 0 };
	{
		desc.OnAcceptServer						= NULL;
		desc.OnDisconnectServer					= NULL;
		desc.OnAcceptUser						= OnAcceptUser;
		desc.OnDisconnectUser					= OnDisconnectUser;
		desc.OnRecvFromServerTCP				= NULL;
		desc.OnRecvFromUserTCP					= ReceivedMsgFromRMTool;
		desc.dwCustomDefineEventNum 			= sizeof( ev ) / sizeof( *ev );
		desc.pEvent								= ev;

		desc.dwMainMsgQueMaxBufferSize			= 512000;
		desc.dwMaxServerNum						= 0;
		desc.dwMaxUserNum						= 50;
		desc.dwServerBufferSizePerConnection	= 0;
		desc.dwServerMaxTransferSize			= 0;
		desc.dwUserBufferSizePerConnection		= 256000;
		desc.dwUserMaxTransferSize				= 65000;
		desc.dwConnectNumAtSameTime				= 100;
		desc.dwFlag								= NULL;
	}

	bool isSucceed = true;

	{
		printf( "... initializing network" );

		if( NETWORK->Init(&desc) )
		{
			printf( " [OK]\n" );
		}
		else
		{
			printf( " [FAIL]\n" );

			isSucceed	= false;
		}		
	}

	{
		printf( "... starting server" );

		if( NETWORK->StartServer() )
		{
			printf( " [OK]\n" );
		}
		else
		{
			printf( " [FAIL]\n" );

			isSucceed	= false;
		}		
	}

	{
		if( g_DB.Init( 1, 1024, true ) )
		{
			printf( "... initializing database [OK]\n" );
		}
		else
		{
			printf( "... initializing database [FAIL]\n" );

			isSucceed	= false;
		}
	}

	mIsRunning = isSucceed;
}


void CMSSystem::End()
{
	//mIsRunning = false;
	//m_bInit = false;

	{
		printf( "... releasing database" );

		g_DB.Release();

		printf( " [OK]\n" );
	}
	
	{
		printf( "... releasing network\n" );

		NETWORK->Release();

		printf( " [OK]\n" );
	}
	
	{
		printf( "... releasing unused library" );

		CoFreeUnusedLibraries();

		printf( " [OK]\n" );
	}
}


void __stdcall CMSSystem::Process( DWORD eventIndex )
{
	if( mIsRunning )
	{
		g_DB.ProcessingDBMessage();
	}
}


void __stdcall CMSSystem::OnAcceptServer(DWORD connectionIndex)
{}


void __stdcall CMSSystem::OnDisconnectServer(DWORD connectionIndex)
{}


void __stdcall CMSSystem::ReceivedMsgFromServer(DWORD connectionIndex,char* pMsg,DWORD dwLength)
{}


void __stdcall CMSSystem::OnAcceptUser(DWORD connectionIndex)
{
	//CMSSystem* system = g_pMSSystem;
	//ASSERT( system );

	//system->mConnectionSet.insert( connectionIndex );
}


void __stdcall CMSSystem::OnDisconnectUser(DWORD connectionIndex )
{
	CUserManager::GetInstance().RemoveUser( connectionIndex );
}


void __stdcall CMSSystem::ReceivedMsgFromRMTool( DWORD connectionIndex,char* pMsg,DWORD size )
{
	const MSGROOT* message = (MSGROOT*)pMsg;

	if( MP_RM_CONNECT == message->Category )
	{
		ConnentMsgParse( connectionIndex, pMsg, size );
		return;
	}

	{
		const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

		if( user.mIp.empty() )
		{
			return;
		}
	}

	switch( message->Category )
	{
	case MP_RM_SEARCH:
		{
			ParseSearch( connectionIndex, message, size );
			break;
		}
	case MP_RM_PLAYER:
		{
			ParsePlayer( connectionIndex, message, size );
			break;
		}
	case MP_RM_ITEM:
		{
			ParseItem( connectionIndex, message, size );
			break;
		}
	case MP_RM_ITEM_FIND:
		{
			ParseItemFind( connectionIndex, message, size );
			break;
		}
	case MP_RM_ITEM_LOG:
		{
			ParseItemLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_ITEM_OPTION_LOG:
		{
			ParseItemOptionLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_SKILL:
		{
			ParseSkill( connectionIndex, message, size );
			break;
		}
	case MP_RM_SKILL_LOG:
		{
			ParseSkillLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_FAMILY:
		{
			ParseFamily( connectionIndex, message, size );
			break;
		}
	case MP_RM_QUEST:
		{
			ParseQuest( connectionIndex, message, size );
			break;
		}
	case MP_RM_QUEST_LOG:
		{
			ParseQuestLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_GUILD:
		{
			ParseGuild( connectionIndex, message, size );
			break;
		}
	case MP_RM_GUILD_LOG:
		{
			ParseGuildLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG:
		{
			ParseGuildWarehouseLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_EXP_LOG:	
		{
			ParseExperienceLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_STAT_LOG:	
		{
			ParseStatLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_USER:
		{
			ParseUser( connectionIndex, message, size );
			break;
		}
	case MP_RM_JOB_LOG:
		{
			ParseJobLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_GUILD_SCORE_LOG:
		{
			ParseGuildScoreLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_FAMILY_CROP_LOG:
		{
			ParseFamilyCropLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_FAMILY_POINT_LOG:
		{
			ParseFamilyPointLog( connectionIndex, message, size );
			break;
		}
	//case MP_RM_USER_CREATE:
	//	{
	//		ParseUserCreate( connectionIndex, message, size );
	//		break;
	//	}
	//case MP_RM_USER_RESTORE:
	//	{
	//		ParseRestoreUser( connectionIndex, message, size );
	//		break;
	//	}
	case MP_RM_PERMISSION:
		{
			ParsePermission( connectionIndex, message, size );
			break;
		}
	case MP_RM_OPERATOR:
		{
			ParseOperator( connectionIndex, message, size );
			break;
		}
	case MP_RM_OPERATOR_LOG:
		{
			ParseOperatorLog( connectionIndex, message, size );
			break;
		}
	case MP_RM_PASSWORD:
		{
			ParsePassword( connectionIndex, message, size );
			break;
		}
	case MP_RM_NAME_LOG:
		{
			ParseNameLog( connectionIndex, message, size );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}

void CMSSystem::ConnentMsgParse(DWORD connectionIndex,char* pMsg, size_t dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_RM_CONNECT_SYN:
		{
			const MSG_RM_LOGIN* m = ( MSG_RM_LOGIN* )pMsg;

			TCHAR ip[ MAX_NAME_LENGTH + 1 ] = { 0 };
			WORD port;
			NETWORK->GetUserAddress( connectionIndex, ip, &port );

			// 080311 버전 비교
			if( MAJORVERSION != m->mVersion.mMajor ||
				MINORVERSION != m->mVersion.mMinor )
			{
				MSG_INT2 message;
				ZeroMemory( &message, sizeof( message ) );

				message.Category	= MP_RM_CONNECT;
				message.Protocol	= MP_RM_CONNECT_NACK_BY_VERSION;
				message.nData1		= MAJORVERSION;
				message.nData2		= MINORVERSION;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );                
				return;
			}

			g_DB.LoginMiddleQuery(
				RLoginOperator,
				connectionIndex,
				"EXEC RP_OperatorLoginCheck \'%s\', \'%s\', \'%s\'",
				m->mId,
				m->mPass,
				ip );
		}
	}
}



bool CMSSystem::IsStopQuery( DWORD connectionIndex ) const
{
	return mStopQuerySet.end() != mStopQuerySet.find( connectionIndex );
}


void CMSSystem::StopQuery( DWORD connectionIndex, RecoveryCategory category, BYTE protocol )
{
	mStopQuerySet.erase( connectionIndex );

	// 쿼리가 스톱되었음을 알린다
	MSGROOT message;

	message.Category	= category;
	message.Protocol	= protocol;

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void CMSSystem::ParseSearch( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_SEARCH_SYN:
		{
			const MSG_RM_OPEN_SYN* m = ( MSG_RM_OPEN_SYN* )message;

			switch( m->mType )
			{
			case MSG_RM_OPEN_SYN::PlayerName:
				{
					g_DB.FreeMiddleQuery(
						RGetUserIndex,
						connectionIndex,
						"EXEC DBO.TP_USER_INDEX_SELECT %d, \'%s\'",
						0,
						m->mKeyword );

					break;
				}
			case MSG_RM_OPEN_SYN::PlayerIndex:
				{
					g_DB.FreeMiddleQuery(
						RGetUserIndex,
						connectionIndex,
						"EXEC DBO.TP_USER_INDEX_SELECT %d, \'%s\'",
						atoi( m->mKeyword ),
						"" );

					break;
				}
			case MSG_RM_OPEN_SYN::LoginId:
				{
					g_DB.LoginMiddleQuery(
						RGetUserList,
						connectionIndex,
						"EXEC DBO.TP_USER_SELECT %d, \'%s\', \'%s\'",
						0,
						m->mKeyword,
						"" );

					break;
				}

			case MSG_RM_OPEN_SYN::RealName:
				{
					g_DB.LoginMiddleQuery(
						RGetUserList,
						connectionIndex,
						"EXEC DBO.TP_USER_SELECT %d, \'%s\', \'%s\'",
						0,
						"",
						m->mKeyword );

					break;
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


void CMSSystem::ParsePlayer( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_PLAYER_GET_DATA_SYN:
		{
			const MSG_RM_GET_PLAYER_SYN* m = ( MSG_RM_GET_PLAYER_SYN* )message;

			// 080313 LUJ,	서비스 서버에는 확장 인벤토리 정보가 CHARACTER_ABILITYEXP에,
			//				테스트 서버에는 CHARACTER_INVENTORY에 담겨있으므로 매크로로 구분하여 컴파일한다.
#ifdef _DEBUG
			g_DB.FreeQuery(
				eGetPlayerData,
				connectionIndex,
				"SELECT C.CHARACTER_IDX, C.CHARACTER_GENDER, C.CHARACTER_STR, C.CHARACTER_DEX,\
				C.CHARACTER_VIT, C.CHARACTER_INT, C.CHARACTER_WIS, C.CHARACTER_LIFE, C.CHARACTER_MANA,\
				C.CHARACTER_GRADE, C.CHARACTER_MAXGRADE, C.CHARACTER_EXPOINT, C.CHARACTER_GRADEUPPOINT,\
				C.CHARACTER_MONEY, C.CHARACTER_PLAYTIME, C.CHARACTER_SKILLPOINT, C.CHARACTER_ATTR,\
				C.CHARACTER_INVENTORY, C.CHARACTER_MAP, C.CHARACTER_POS_X, C.CHARACTER_POS_Y,\
				C.CHARACTER_JOB, C.CHARACTER_JOB1, C.CHARACTER_JOB2,  C.CHARACTER_JOB3, C.CHARACTER_JOB4, C.CHARACTER_JOB5,  C.CHARACTER_JOB6,\
				C.CHARACTER_MUNPAIDX, C.CHARACTER_MUNPAJIKCHEK, ISNULL( G.GuildName, \'\' ) AS MUNPA_NAME, C.USER_IDX\
				FROM TB_CHARACTER C LEFT OUTER JOIN TB_GUILD G ON C.CHARACTER_MUNPAIDX = G.GuildIdx\
				WHERE (C.CHARACTER_IDX = %d)",
				m->mPlayerIndex );
#else
			g_DB.FreeQuery(
				eGetPlayerData,
				connectionIndex,
				"SELECT C.CHARACTER_IDX, C.CHARACTER_GENDER, C.CHARACTER_STR, C.CHARACTER_DEX,\
				C.CHARACTER_VIT, C.CHARACTER_INT, C.CHARACTER_WIS, C.CHARACTER_LIFE, C.CHARACTER_MANA,\
				C.CHARACTER_GRADE, C.CHARACTER_MAXGRADE, C.CHARACTER_EXPOINT, C.CHARACTER_GRADEUPPOINT,\
				C.CHARACTER_MONEY, C.CHARACTER_PLAYTIME, C.CHARACTER_SKILLPOINT, C.CHARACTER_ATTR,\
				C.CHARACTER_ABILITYEXP, C.CHARACTER_MAP, C.CHARACTER_POS_X, C.CHARACTER_POS_Y,\
				C.CHARACTER_JOB, C.CHARACTER_JOB1, C.CHARACTER_JOB2,  C.CHARACTER_JOB3, C.CHARACTER_JOB4, C.CHARACTER_JOB5,  C.CHARACTER_JOB6,\
				C.CHARACTER_MUNPAIDX, C.CHARACTER_MUNPAJIKCHEK, ISNULL( G.GuildName, \'\' ) AS MUNPA_NAME, C.USER_IDX\
				FROM TB_CHARACTER C LEFT OUTER JOIN TB_GUILD G ON C.CHARACTER_MUNPAIDX = G.GuildIdx\
				WHERE (C.CHARACTER_IDX = %d)",
				m->mPlayerIndex );
#endif

			g_DB.FreeMiddleQuery(
				RGetPlayerLicense,
				connectionIndex,
				"SELECT PlayerID, Sex, Age, Location, Favor,\
				PropensityLike01, PropensityLike02, PropensityLike03, PropensityDisLike01, PropensityDisLike02\
				FROM    TB_RESIDENTREGIST\
				WHERE   (PlayerID = %d)",
				m->mPlayerIndex );

			g_DB.FreeMiddleQuery(
				RGetPlayerExtendedData,
				connectionIndex,
				"SELECT A.CHARACTER_NAME, A.CHARACTER_NICKNAME, ISNULL(C.MemberNickname, \'\') AS FAMILY_NICK,\
				A.CHARACTER_LASTMODIFIED, B.CHARACTER_NAME AS ORIGINAL_NAME, B.FLAG, B.CREATE_DATE,\
				B.CREATE_IP, B.DELETE_DATE, B.DELETE_IP, B.RECOVER_DATE, B.RECOVER_IP\
				FROM      TB_CHARACTER A INNER JOIN TB_CharacterCreateInfo B ON A.CHARACTER_IDX = B.CHARACTER_IDX\
				LEFT OUTER JOIN TB_FAMILY_MEMBER C ON A.CHARACTER_IDX = C.MemberID\
				WHERE   (A.CHARACTER_IDX = %d)",
				m->mPlayerIndex );

			break;
		}
	case MP_RM_PLAYER_SET_DATA_SYN:
		{
			const MSG_RM_PLAYER_DATA* m = ( MSG_RM_PLAYER_DATA* )message;

			const MSG_RM_PLAYER_DATA::Player&	player		= m->mPlayer;
			const MSG_RM_PLAYER_DATA::Position&	position	= m->mPosition;
			const MSG_RM_PLAYER_DATA::Job&		job			= m->mJob;

			g_DB.FreeMiddleQuery(
				RSetPlayerData,
				connectionIndex,
				"EXEC DBO.TP_CHARACTER_DATA_UPDATE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %u, %d, %u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				player.mIndex,
				player.mGender,
				player.mStrength,
				player.mDexterity,
				player.mVitality,
				player.mIntelligence,
				player.mWisdom,
				player.mLife,
				player.mMana,
				player.mGrade,
				player.mMaxGrade,
				player.mExp,
				player.mGradeUpPoint,
				player.mMoney,
				player.mSkillPoint,
				player.mExtendedInventorySize,
				position.mMap,
				position.mX,
				position.mY,
				job.mIndex,
				job.mData1,
				job.mData2,
				job.mData3,
				job.mData4,
				job.mData5,
				job.mData6 );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogSetPlayer,
				user.mId.c_str(),
				"playerIndex",
				player.mIndex );
			break;
		}
	case MP_RM_PLAYER_SET_EXTENDED_DATA_SYN:
		{
			const MSG_RM_PLAYER_EXTENDED_DATA*			m		= ( MSG_RM_PLAYER_EXTENDED_DATA* )message;
			const MSG_RM_PLAYER_EXTENDED_DATA::Player&	player	= m->mPlayer;

			g_DB.FreeMiddleQuery(
				RSetPlayerExtendedData,
				connectionIndex,
				"EXEC DBO.TP_CHARACTER_EXTENDED_DATA_UPDATE %d, \'%s\', \'%s\'",
				player.mIndex,
				player.mName,
				player.mGuildNick );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogSetExtendedPlayer,
				user.mId.c_str(),
				"playerIndex",
				player.mIndex );

			break;
		}
	case MP_RM_PLAYER_SET_LICENSE_SYN:
		{
			const MSG_RM_PLAYER_LOVE*	m			= ( MSG_RM_PLAYER_LOVE* )message;
			const DWORD					playerIndex = m->dwObjectID;
			const DATE_MATCHING_INFO&	data		= m->mDateMatching;
			
			g_DB.FreeMiddleQuery( RPlayerSetLicense, connectionIndex, "EXEC DBO.TP_PLAYER_LICENSE_UPDATE %d, %d, %d, %d, %d, %d, %d, %d, %d",
				playerIndex,
				data.dwAge,
				data.dwRegion,
				data.dwGoodFeelingDegree,
				data.pdwGoodFeeling[ 0 ],
				data.pdwGoodFeeling[ 1 ],
				data.pdwGoodFeeling[ 2 ],
				data.pdwBadFeeling[ 0 ],
				data.pdwBadFeeling[ 1 ] );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogSetExtendedPlayer,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID );

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}

void CMSSystem::ParseItem( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_ITEM_GET_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD playerIndex	= m->dwObjectID;
			const DWORD userIndex	= m->dwData;
			
			GetItemListOnInventory( connectionIndex, playerIndex, 0 );
			GetItemListOnShop( connectionIndex, userIndex, 0 );
			GetItemListOnStorage( connectionIndex, userIndex, 0 );
			
			// 080313 LUJ,	서비스 서버에는 확장 인벤토리 정보가 CHARACTER_ABILITYEXP에,
			//				테스트 서버에는 CHARACTER_INVENTORY에 담겨있으므로 매크로로 구분하여 컴파일한다.
#ifdef _DEBUG
			g_DB.FreeMiddleQuery(
				RGetStorageData,
				connectionIndex,
				"SELECT P.User_IDX, P.PyogukNum, P.PyogukMoney, C.CHARACTER_INVENTORY\
				FROM TB_CHARACTER C INNER JOIN TB_PYOGUK P ON C.USER_IDX = P.User_IDX\
				WHERE C.CHARACTER_IDX = %d",
				playerIndex );
#else
			g_DB.FreeMiddleQuery(
				RGetStorageData,
				connectionIndex,
				"SELECT P.User_IDX, P.PyogukNum, P.PyogukMoney, C.CHARACTER_ABILITYEXP\
				FROM TB_CHARACTER C INNER JOIN TB_PYOGUK P ON C.USER_IDX = P.User_IDX\
				WHERE C.CHARACTER_IDX = %d",
				playerIndex );
#endif
			break;
		}
	case MP_RM_ITEM_SET_OPTION_SYN:
		{
			const MSG_RM_ITEM_UPDATE_OPTION* m = ( MSG_RM_ITEM_UPDATE_OPTION* )message;

			const DWORD						playerIndex	= m->dwObjectID;
			const ITEM_OPTION&				option		= m->mOption;
			const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
			const ITEM_OPTION::Mix&			mix			= option.mMix;
			const ITEM_OPTION::Enchant&		enchant		= option.mEnchant;

			g_DB.FreeMiddleQuery(
				0,
				0,
				"EXEC DBO.MP_ITEM_OPTION_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, \'%s\', %d, %d, \'%s\'", 
				option.mItemDbIndex,
				reinforce.mStrength,
				reinforce.mDexterity,
				reinforce.mVitality,
				reinforce.mIntelligence,
				reinforce.mWisdom,
				reinforce.mLife,
				reinforce.mMana,
				reinforce.mLifeRecovery,
				reinforce.mManaRecovery,
				reinforce.mPhysicAttack,
				reinforce.mPhysicDefence,
				reinforce.mMagicAttack,
				reinforce.mMagicDefence,
				reinforce.mMoveSpeed,
				reinforce.mEvade,
				reinforce.mAccuracy,
				reinforce.mCriticalRate,
				reinforce.mCriticalDamage,
				reinforce.mMadeBy,
				mix.mStrength,
				mix.mIntelligence,
				mix.mDexterity,
				mix.mWisdom,
				mix.mVitality,
				mix.mMadeBy,
				enchant.mIndex,
				enchant.mLevel,
				enchant.mMadeBy );

			{
				MSG_RM_ITEM_UPDATE_OPTION	message;
				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_SET_OPTION_ACK;
				message.dwObjectID	= playerIndex;
				message.mOption		= option;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );
			}

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetItemOption,
				user.mId.c_str(),
				"playerIndex",
				playerIndex,
				"itemDbIndex",
				option.mItemDbIndex );

			// 로그를 남기자
			g_DB.LogMiddleQuery(
				0,
				0,
				"EXEC DBO.TP_ITEM_OPTION_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \'%s\'",
				playerIndex,
				option.mItemDbIndex,
				reinforce.mStrength,
				reinforce.mDexterity,
				reinforce.mVitality,
				reinforce.mIntelligence,
				reinforce.mWisdom,
				reinforce.mLife,
				reinforce.mMana,
				reinforce.mLifeRecovery,
				reinforce.mManaRecovery,
				reinforce.mPhysicAttack,
				reinforce.mPhysicDefence,
				reinforce.mMagicAttack,
				reinforce.mMagicDefence,
				reinforce.mCriticalRate,
				reinforce.mCriticalDamage,
				reinforce.mMoveSpeed,
				reinforce.mEvade,
				reinforce.mAccuracy,
				mix.mStrength,
				mix.mIntelligence,
				mix.mDexterity,
				mix.mWisdom,
				mix.mVitality,
				enchant.mIndex,
				enchant.mLevel,
				eLog_ItemOptionAdd_RM,
				user.mId.c_str() );
			break;
		}
	case MP_RM_ITEM_SET_SYN:
		{
			const MSG_RM_ITEM_UPDATE*	m		= ( MSG_RM_ITEM_UPDATE* )message;
			const ITEMBASE&				item	= m->mItem;

			g_DB.FreeMiddleQuery(
				0,
				0,
				"UPDATE TB_ITEM\
				SET		ITEM_IDX = %d, ITEM_POSITION = %d, ITEM_DURABILITY = %d\
				WHERE   (ITEM_DBIDX = %d)",
				item.wIconIdx,
				item.Position,
				item.Durability,				
				item.dwDBIdx );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetItem,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"itemDbIndex",
				m->mItem.dwDBIdx );

			{
				MSG_RM_ITEM_UPDATE message	= *m;
				message.Protocol			= MP_RM_ITEM_SET_ACK;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );
			}

			break;
		}
	case MP_RM_ITEM_DELETE_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD playerIndex	= m->dwObjectID;
			const DWORD itemDbIndex = m->dwData;			

			g_DB.FreeMiddleQuery(
				0,
				0,
				"DELETE FROM TB_ITEM WHERE ITEM_DBIDX = %d",
				itemDbIndex );

			g_DB.FreeMiddleQuery(
				0,
				0,
				"DELETE FROM TB_ITEM_OPTION2 WHERE ITEM_DBIDX = %d",
				itemDbIndex );

			g_DB.FreeMiddleQuery(
				0,
				0,
				"DELETE FROM TB_ITEM_DROP_OPTION WHERE ITEM_DBIDX = %d",
				itemDbIndex );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogRemoveItem,
				user.mId.c_str(),
				"playerIndex",
				playerIndex,
				"itemDbIndex",
				itemDbIndex );

			g_DB.LogMiddleQuery(
				0,
				0,
				"EXEC DBO.up_ItemMoneyLog %d, \'%s\', %d, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %d, %d", 
				0,
				std::string( "*" + user.mId ).c_str(), 
				playerIndex,
				"", 
				eLog_ItemRemove,
				0,
				0,
				0,
				0,
				itemDbIndex,
				0,
				0,
				0,
				0 );

			{
				MSG_DWORD message;
				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_DELETE_ACK;
				message.dwObjectID	= playerIndex;
				message.dwData		= itemDbIndex;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );
			}
			break;
		}
	case MP_RM_ITEM_DELETE_OPTION_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD playerIndex = m->dwObjectID;
			const DWORD itemDbIndex	= m->dwData;

			g_DB.FreeMiddleQuery(
				0,
				0,
				"DELETE FROM TB_ITEM_OPTION2 WHERE ITEM_DBIDX = %d",
				itemDbIndex );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogRemoveItemOption,
				user.mId.c_str(),
				"playerIndex",
				playerIndex,
				"itemDbIndex",
				itemDbIndex );

			g_DB.LogMiddleQuery(
				0,
				0,
				"EXEC DBO.TP_ITEM_OPTION_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \'%s\'",
				0,
				itemDbIndex,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				eLog_ItemOptionRemove_RM,
				user.mId.c_str() );

			{
				MSG_DWORD message;
				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_DELETE_OPTION_ACK;
				message.dwObjectID	= playerIndex;
				message.dwData		= itemDbIndex;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );
			}
			break;
		}
	case MP_RM_ITEM_ADD_SYN:
		{
			const MSG_RM_ITEM_ADD* m = ( MSG_RM_ITEM_ADD* )message;
			
			const Item& item = m->mItem;

			switch( item.mArea )
			{
			case Item::AreaInventory:
				{
					g_DB.FreeMiddleQuery(
						RAddItem,
						connectionIndex,
						"EXEC DBO.TP_ITEM_INSERT %d, %d, %d, %d, %d, %d, %d, %d",
						m->mPlayerIndex,
						item.wIconIdx,
						item.Position,
						item.Durability,
						0,
						0,
						item.nSealed,
						item.mArea );
					break;
				}
			case Item::AreaPrivateStorage:
				{
					g_DB.FreeMiddleQuery(
						RAddItem,
						connectionIndex,
						"EXEC DBO.TP_ITEM_INSERT %d, %d, %d, %d, %d, %d, %d, %d",
						0,
						item.wIconIdx,
						item.Position,
						item.Durability,
						m->mUserIndex,
						0,
						item.nSealed,
						item.mArea );
					break;
				}
			case Item::AreaCashStorage:
				{
					g_DB.FreeMiddleQuery(
						RAddItem,
						connectionIndex,
						"EXEC DBO.TP_ITEM_INSERT %d, %d, %d, %d, %d, %d, %d, %d",
						0,
						item.wIconIdx,
						item.Position,
						item.Durability,						
						0,
						m->mUserIndex,
						item.nSealed,
						item.mArea );
					break;
				}
			}

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2, KEY3, VALUE3 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogAddItem,
				user.mId.c_str(),
				"playerIndex",
				m->mPlayerIndex,
				"itemIndex",
				m->mItem.wIconIdx,
				"quantity",
				m->mItem.Durability );
			break;
		}
	case MP_RM_ITEM_UPDATE_END_TIME_SYN:
		{
			const MSG_INT2* m = ( MSG_INT2* )message;

			const DWORD itemDbIndex	= m->nData1;
			const DWORD second		= m->nData2;

			g_DB.FreeMiddleQuery(
				RSetItemEndTime,
				connectionIndex,
				"EXEC DBO.TP_ITEM_END_TIME_UPDATE %d, %d",
				itemDbIndex,
				second );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogUpdateItemEndTime,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"itemDbIndex",
				m->nData1 );
			break;
		}
	case MP_RM_ITEM_UPDATE_REMAIN_TIME_SYN:
		{
			const MSG_INT2* m = ( MSG_INT2* )message;

			SetItemRemainTime( connectionIndex, m->nData1, m->nData2 );
			break;
		}
	case MP_RM_ITEM_SET_STORAGE_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			g_DB.FreeMiddleQuery(
				RSetStorageData,
				connectionIndex,
				"EXEC DBO.TP_STORAGE_UPDATE %d, %d, %d",
				m->dwObjectID,
				m->dwData1,
				m->dwData2 );
			
			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2, KEY3, VALUE3 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetItemStorage,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"size",
				m->dwData1,
				"money",
				m->dwData2 );

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseItemFind( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM_FIND;
		message.Protocol	= MP_RM_ITEM_FIND_OWNER_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_ITEM_FIND_OWNER_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			FindItemOwner( connectionIndex, m->dwData1, m->dwData2 );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseItemLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM_LOG;
		message.Protocol	= MP_RM_ITEM_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_ITEM_LOG_GET_SIZE_SYN:
		{
			const MSG_RM_ITEM_LOG_REQUEST* m = ( MSG_RM_ITEM_LOG_REQUEST* )message;

			g_DB.LogMiddleQuery(
				RGetItemLogSize,
				connectionIndex,
				"EXEC DBO.TP_ITEM_LOG_SIZE_SELECT %d, %d, %d, \'%s\', \'%s\', %d",
				m->mPlayerIndex,
				m->mItemDbIndex,
				m->mItemIndex,
				m->mBeginDate,
				m->mEndDate,
				m->mRequestTick );

			break;
		}
	case MP_RM_ITEM_LOG_GET_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_ITEM;
			message.Protocol	= MP_RM_ITEM_LOG_GET_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseItemOptionLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM_OPTION_LOG;
		message.Protocol	= MP_RM_ITEM_OPTION_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_ITEM_OPTION_LOG_SIZE_SYN:
		{
			const MSG_RM_ITEM_OPTION_LOG_REQUEST* m = ( MSG_RM_ITEM_OPTION_LOG_REQUEST* )message;

			GetItemOptionLogSize( connectionIndex, m->mItemDbIndex, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_ITEM_OPTION_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_ITEM_OPTION_LOG;
			message.Protocol	= MP_RM_ITEM_OPTION_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseSkill( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL;
		message.Protocol	= MP_RM_SKILL_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_SKILL_GET_ALL_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			GetSkillList( connectionIndex, m->dwObjectID );
			break;
		}
	case MP_RM_SKILL_ADD_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			AddSkillData( connectionIndex, m->dwObjectID, m->dwData );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogAddSkill,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"skillIndex",
				m->dwData );
			break;
		}
	case MP_RM_SKILL_REMOVE_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			RemoveSkillData( connectionIndex, m->dwData );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogRemoveSkill,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"skillIndex",
				m->dwData );
			break;
		}
	case MP_RM_SKILL_SET_LEVEL_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			SetSkillLevel( connectionIndex, m->dwObjectID, m->dwData1, m->dwData2 );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetSkill,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"skillLevel",
				m->dwData2 );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseSkillLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL_LOG;
		message.Protocol	= MP_RM_SKILL_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;

	}
	switch( message->Protocol )
	{
	case MP_RM_SKILL_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			GetSkillLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_SKILL_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_SKILL_LOG;
			message.Protocol	= MP_RM_SKILL_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseFamily( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_FAMILY_GET_LIST_SYN:
		{
			const MSG_NAME_DWORD* m = ( MSG_NAME_DWORD*)message;

			GetFamilyList( connectionIndex, m->Name, 0 < m->dwData );
			break;
		}
	case MP_RM_FAMILY_GET_DATA_SYN:
		{
			const MSGBASE*	m			= ( MSGBASE* )message;
			const DWORD		playerIndex = m->dwObjectID;

			GetFamilyData( connectionIndex, playerIndex );
			GetFamilyFarm( connectionIndex, playerIndex );
			GetFamilyCrop( connectionIndex, playerIndex );
			break;
		}
	case MP_RM_FAMILY_SET_DATA_SYN:
		{
			const MSG_DWORD*	m				= ( MSG_DWORD* )message;
			const DWORD			familyIndex		= m->dwObjectID;
			const DWORD			point			= m->dwData;

			g_DB.FreeMiddleQuery(
				RSetFamilyData,
				connectionIndex,
				"EXEC DBO.TP_FAMILY_UPDATE %d, %d",
				familyIndex,
				point );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetFamily,
				user.mId.c_str(),
				"familyIndex",
				familyIndex,
				"point",
				point );

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseQuest( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_QUEST_GET_MAIN_DATA_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			// 서브 퀘스트 아이템 정보를 읽고 메인 퀘스트 정보를 읽는다

			GetQuestSubData( connectionIndex, m->dwObjectID, 0 );
			
			break;
		}
	case MP_RM_QUEST_REMOVE_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			RemoveQuestData( connectionIndex, m->dwObjectID, m->dwData );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogRemoveQuest,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"mainQuestIdx",
				m->dwData );
			break;
		}
	case MP_RM_QUEST_FINISH_SUB_SYN:
		{
			const MSG_DWORD3* m = ( MSG_DWORD3* )message;

			const DWORD			mainQuestIndex	= m->dwData1;
			const DWORD			subQuestIndex	= m->dwData2;
			const QSTATETYPE	state			= m->dwData3;

			EndSubQuest( connectionIndex, m->dwObjectID, mainQuestIndex, subQuestIndex, state );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogFinishSubQuest,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"subQuestIdx",
				subQuestIndex );
			break;
		}
	case MP_RM_QUEST_ITEM_UPDATE_SYN:
		{
			const MSG_DWORD3* m = ( MSG_DWORD3* )message;

			const DWORD playerIndex		= m->dwObjectID;
			const DWORD questMainIndex	= m->dwData1;
			const DWORD itemIndex		= m->dwData2;
			const DWORD size			= m->dwData3;

			g_DB.FreeMiddleQuery(
				RUpdateQuestItem,
				connectionIndex,
				"EXEC DBO.TP_QUEST_ITEM_UPDATE %d, %d, %d, %d",
				playerIndex,
				questMainIndex,
				itemIndex,
				size );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetQuestItem,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"questItemIdx",
				m->dwData2 );
			break;
		}
	case MP_RM_QUEST_ITEM_DELETE_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			DeleteQuestItem( connectionIndex, m->dwObjectID, m->dwData1, m->dwData2 );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogRemoveQuestItem,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"questItemIdx",
				m->dwData2 );
			break;
		}
	case MP_RM_QUEST_ITEM_ADD_SYN:
		{
			const MSG_DWORD3* m = ( MSG_DWORD3* )message;

			const DWORD playerIndex		= m->dwObjectID;
			const DWORD mainQuestIndex	= m->dwData1;
			const DWORD itemIndex		= m->dwData2;
			const DWORD	quantity		= m->dwData3;

			g_DB.FreeMiddleQuery(
				0,
				0,
				"INSERT INTO TB_QUESTITEM ( CHARACTER_IDX, ITEM_IDX, ITEM_COUNT, QUEST_IDX ) VALUES ( %d, %d, %d, %d )",
				playerIndex,
				itemIndex,
				quantity,
				mainQuestIndex );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2, KEY3, VALUE3 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogAddQuestItem,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"questItemIdx",
				itemIndex,
				"size",
				quantity );

			{
				SEND_QUESTITEM message;
				message.Category	= MP_RM_QUEST;
				message.Protocol	= MP_RM_QUEST_ITEM_ADD_ACK;
				message.wCount		= 1;

				{
					QITEMBASE& item = message.ItemList[ 0 ];

					item.ItemIdx	= WORD( itemIndex );
					item.QuestIdx	= WORD( mainQuestIndex );
					item.Count		= WORD( quantity );
				}

				NETWORK->Send( connectionIndex, message, message.GetSize() );
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


void CMSSystem::ParseQuestLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST_LOG;
		message.Protocol	= MP_RM_QUEST_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_QUEST_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			GetQuestLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_QUEST_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_QUEST_LOG;
			message.Protocol	= MP_RM_QUEST_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseGuild( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_GUILD_GET_LIST_SYN:
		{
			const MSG_NAME_DWORD* m = ( MSG_NAME_DWORD*)message;

			GetGuildList( connectionIndex, m->Name, 0 < m->dwData );
			break;
		}
	case MP_RM_GUILD_GET_DATA_SYN:
		{
			const MSG_NAME_DWORD* m = ( MSG_NAME_DWORD* )message;

			GetGuildData( connectionIndex, m->Name, m->dwData );
			break;
		}
	case MP_RM_GUILD_SET_DATA_SYN:
		{
			const MSG_RM_GUILD_DATA* m = ( MSG_RM_GUILD_DATA* )message;

			GuildUpdateData( connectionIndex, *m );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogSetGuild,
				user.mId.c_str(),
				"guildIndex",
				m->mGuildIndex );
			break;
		}
	case MP_RM_GUILD_SET_RANK_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			GuildSetRank( connectionIndex, m->dwObjectID, m->dwData1, m->dwData2 );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2, KEY3, VALUE3 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetGuildRank,
				user.mId.c_str(),
				"guildIndex",
				m->dwData1,
				"playerIndex",
				m->dwObjectID,
				"rank",
				m->dwData2 );
			break;
		}
	case MP_RM_GUILD_KICK_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			GuildKickMember( connectionIndex, m->dwObjectID );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogKickGuildMember,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID );
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseGuildLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_LOG;
		message.Protocol	= MP_RM_GUILD_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_GUILD_LOG_SIZE_SYN:
		{
			const MSG_RM_GUILD_LOG_REQUEST* m = ( MSG_RM_GUILD_LOG_REQUEST* )message;

			GetGuildLogSize( connectionIndex, m->mGuildIndex, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_GUILD_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_GUILD_LOG;
			message.Protocol	= MP_RM_GUILD_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseGuildWarehouseLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
		message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_GUILD_WAREHOUSE_LOG_SIZE_SYN:
		{
			const MSG_RM_GUILD_LOG_REQUEST* m = ( MSG_RM_GUILD_LOG_REQUEST* )message;

			GetGuildWarehouseLogSize( connectionIndex, m->mGuildIndex, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
			message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseExperienceLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_EXP_LOG;
		message.Protocol	= MP_RM_EXP_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_EXP_LOG_GET_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			//GetExperienceLog( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate, 0 );

			GetExperienceLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_EXP_LOG_GET_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_EXP_LOG;
			message.Protocol	= MP_RM_EXP_LOG_GET_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseStatLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_STAT_LOG;
		message.Protocol	= MP_RM_STAT_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_STAT_LOG_SIZE_SYN:
		{
			const MSG_RM_STAT_LOG_REQUEST* m = ( MSG_RM_STAT_LOG_REQUEST* )message;

			GetStatLogSize( connectionIndex, m->mPlayerIndex, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_STAT_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_STAT_LOG;
			message.Protocol	= MP_RM_STAT_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseUser( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_USER_GET_DATA_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			const DWORD userIndex	= m->dwObjectID;
			const char*	userName	= m->Name;

			g_DB.LoginMiddleQuery(
				RGetUserData,
				connectionIndex,
				"EXEC DBO.TP_USER_DATA_SELECT %d, \'%s\'",
				userIndex,
				userName );
			break;
		}
	case MP_RM_USER_SET_DATA_SYN:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			SetUserData( connectionIndex, m->dwData1, LEVELTYPE( m->dwData2 ) );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryLogSetUser,
				user.mId.c_str(),
				"userIndex",
				m->dwData1,
				"level",
				m->dwData2 );
			break;
		}
	case MP_RM_USER_DELETE_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			DeleteUser( connectionIndex, m->dwObjectID );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%d\' )",
				RecoveryLogRemovePlayerFromUser,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID );

			break;
		}
	case MP_RM_USER_CREATE_SYN:
		{
			const CHARACTERMAKEINFO* m = ( CHARACTERMAKEINFO* )message;

			UserAdd( connectionIndex, *m );
			break;
		}
	case MP_RM_USER_RESTORE_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			RestoreUser( connectionIndex, m->dwObjectID, m->Name );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%s\' )",
				RecoveryLogRestorePlayerToUser,
				user.mId.c_str(),
				"playerIndex",
				m->dwObjectID,
				"playerName",
				m->Name );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseJobLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_JOB_LOG;
		message.Protocol	= MP_RM_JOB_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_JOB_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST*	m			= ( MSG_RM_LOG_REQUEST* )message;
			const DWORD					playerIndex = m->dwObjectID;
			
			g_DB.LogMiddleQuery(
				RGetJobLogSize,
				connectionIndex,
				"EXEC DBO.TP_JOB_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
				playerIndex,
				m->mBeginDate,
				m->mEndDate );

			//GetJobLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_JOB_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_JOB_LOG;
			message.Protocol	= MP_RM_JOB_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseGuildScoreLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_SCORE_LOG;
		message.Protocol	= MP_RM_GUILD_SCORE_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_GUILD_SCORE_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			GetGuildScoreLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_GUILD_SCORE_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_GUILD_SCORE_LOG;
			message.Protocol	= MP_RM_GUILD_SCORE_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseFamilyCropLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY_CROP_LOG;
		message.Protocol	= MP_RM_FAMILY_CROP_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_FAMILY_CROP_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			GetFarmCropLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_FAMILY_CROP_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_FAMILY_CROP_LOG;
			message.Protocol	= MP_RM_FAMILY_CROP_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseFamilyPointLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY_POINT_LOG;
		message.Protocol	= MP_RM_FAMILY_POINT_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_FAMILY_POINT_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			GetFamilyPointLogSize( connectionIndex, m->dwObjectID, m->mBeginDate, m->mEndDate );
			break;
		}
	case MP_RM_FAMILY_POINT_LOG_STOP_SYN:
		{
			//g_pMSSystem->mStopQuerySet.insert( connectionIndex );

			MSGROOT message;
			message.Category	= MP_RM_FAMILY_POINT_LOG;
			message.Protocol	= MP_RM_FAMILY_POINT_LOG_STOP_ACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


//void CMSSystem::ParseUserCreate( DWORD connectionIndex, const MSGROOT* message, size_t size )
//{
//	switch( message->Protocol )
//	{
//	case MP_RM_USER_CREATE_SYN:
//		{
//			const CHARACTERMAKEINFO* m = ( CHARACTERMAKEINFO* )message;
//
//			UserAdd( connectionIndex, *m );
//			break;
//		}
//	default:
//		{
//			ASSERT( 0 );
//			break;
//		}
//	}
//}


//void CMSSystem::ParseRestoreUser( DWORD connectionIndex, const MSGROOT* message, size_t size )
//{
//	switch( message->Protocol )
//	{
//	case MP_RM_USER_RESTORE_SYN:
//		{
//			const MSG_NAME* m = ( MSG_NAME* )message;
//
//			RestoreUser( connectionIndex, m->dwObjectID, m->Name );
//			break;
//		}
//	default:
//		{
//			ASSERT( 0 );
//			break;
//		}
//	}
//}


void CMSSystem::ParsePermission( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_PERMISSION_ADD_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			AddPermission( connectionIndex, m->Name );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%s\' )",
				RecoveryPermissionAddIp,
				user.mId.c_str(),
				"IP",
				m->Name );
			break;
		}
	case MP_RM_PERMISSION_REMOVE_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			RemovePermission( connectionIndex, m->Name );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%s\' )",
				RecoveryPermissionRemoveIp,
				user.mId.c_str(),
				"IP",
				m->Name );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseOperator( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_OPERATOR_GET_SYN:
		{
			char buffer[ MAX_PATH ];

			{
				sprintf(
					buffer,
					"SELECT TOP %d OPERIDX, OPERPOWER, OPERID, OPERNAME, CONVERT( VARCHAR( 6 ), OPERREGDATE, 12 ) FROM TB_OPERATOR",
					MAX_ROW_NUM );

				g_DB.LoginQuery(
					eQueryType_FreeQuery,
					eGetOperator,
					connectionIndex,
					buffer );
			}

			{
				sprintf(
					buffer,
					"SELECT TOP %d IPADRESS FROM TB_OPERIP",
					MAX_ROW_NUM );
				
				g_DB.LoginQuery(
					eQueryType_FreeQuery,
					eGetOperatorIp,
					connectionIndex,
					buffer );
			}

			break;
		}
	case MP_RM_OPERATOR_ADD_SYN:
		{
			const MSG_RM_OPERATOR*					m				= ( MSG_RM_OPERATOR* )message;
			const MSG_RM_OPERATOR::Operator&		data			= m->mOperator[ 0 ];

			const char*		defaultPassword = "luna";
			const eGM_POWER	defaultRank		= eGM_POWER_MONITOR;

			g_DB.LoginMiddleQuery(
				RAddOperator,
				connectionIndex,
				"EXEC DBO.TP_OPERATOR_INSERT \'%s\', \'%s\', \'%s\', %d",
				data.mId,
				data.mName,
				defaultPassword,
				defaultRank );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\' )",
				RecoveryOperatorAdd,
				user.mId.c_str(),
				"ID",
				data.mId,
				"name",
				data.mName );

			break;
		}
	case MP_RM_OPERATOR_REMOVE_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			g_DB.LoginMiddleQuery(
				0,
				0,
				"DELETE TB_OPERATOR WHERE OPERIDX = %d",
				m->dwObjectID );

			{
				MSGBASE message;
				message.Category	= MP_RM_OPERATOR;
				message.Protocol	= MP_RM_OPERATOR_REMOVE_ACK;
				message.dwObjectID	= m->dwObjectID;

				NETWORK->Send( connectionIndex, message, sizeof( message ) );
			}			

			break;
		}
	case MP_RM_OPERATOR_SET_SYN:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;
			
			g_DB.LoginMiddleQuery(
				RUpdateOperator,
				connectionIndex,
				"EXEC DBO.TP_OPERATOR_UPDATE %d, %d",
				m->dwObjectID,	// operator index
				m->dwData );	// rank

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
				RecoveryOperatorSet,
				user.mId.c_str(),
				"index",
				m->dwObjectID,
				"rank",
				m->dwData );

			break;
		}
	case MP_RM_OPERATOR_IP_ADD_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			g_DB.LoginMiddleQuery(
				RAddOperatorIp,
				connectionIndex,
				"EXEC DBO.TP_OPERATOR_IP_INSERT \'%s\'",
				m->Name );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%s\' )",
				RecoveryOperatorAddIp,
				user.mId.c_str(),
				"IP",
				m->Name );
			break;
		}
	case MP_RM_OPERATOR_IP_REMOVE_SYN:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			g_DB.LoginMiddleQuery(
				RRemoveOperatorIp,
				connectionIndex,
				"EXEC DBO.TP_OPERATOR_IP_DELETE \'%s\'",
				m->Name );

			g_DB.LogMiddleQuery(
				0,
				0,
				"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', \'%s\' )",
				RecoveryOperatorRemoveIp,
				user.mId.c_str(),
				"IP",
				m->Name );
			break;
		}
	}
}


void CMSSystem::ParseOperatorLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	if( ! CUserManager::GetInstance().IsAuthorize( RecoveryCategory( message->Category ), message->Protocol, connectionIndex ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR_LOG;
		message.Protocol	= MP_RM_OPERATOR_LOG_NACK_BY_AUTH;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );

		return;
	}

	switch( message->Protocol )
	{
	case MP_RM_OPERATOR_LOG_SIZE_SYN:
		{
			const MSG_RM_LOG_REQUEST* m = ( MSG_RM_LOG_REQUEST* )message;

			g_DB.LogMiddleQuery(
				RGetOperatorLogSize,
				connectionIndex,
				"EXEC DBO.TP_OPERATOR_LOG_SIZE_SELECT \'%s\', \'%s\', %d",
				m->mBeginDate,
				m->mEndDate,
				m->mRequestTick );

			break;
		}
	}
}


void CMSSystem::ParsePassword( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	switch( message->Protocol )
	{
	case MP_RM_PASSWORD_SET_SYN:
		{
			const MSG_NAME2* m = ( MSG_NAME2* )message;

			const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

			if( user.mId.empty() )
			{
				return;
			}

			g_DB.LoginMiddleQuery(
				RSetPassword,
				connectionIndex,
				"EXEC DBO.TP_PASSWORD_UPDATE \'%s\', \'%s\', \'%s\'",
				user.mId.c_str(),
				m->str1,
				m->str2 );

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CMSSystem::ParseNameLog( DWORD connectionIndex, const MSGROOT* message, size_t size )
{
	switch( message->Protocol )
	{
	case MP_RM_NAME_LOG_GET_SYN:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			g_DB.LogQuery(
				eQueryType_FreeQuery,
				eGetNameLog,
				connectionIndex,
				"SELECT TOP %d IDX, CHARACTER_BEFORE, CHARACTER_AFTER, CONVERT( VARCHAR( 6 ), REG_DATE, 12 ) + ' ' + CONVERT( VARCHAR( 8 ), REG_DATE, 8 ) FROM TB_CHARRENAME_LOG WHERE CHARACTER_IDX = %d",
				//"SELECT TOP %d IDX , CHARACTER_IDX, CHARACTER_BEFORE, CHARACTER_AFTER FROM TB_CHARRENAME_LOG WHERE CHARACTER_IDX = %d",
				//"SELECT TOP %d IDX FROM DBO.TB_CHARRENAME_LOG",
				MAX_ROW_NUM,
				m->dwObjectID );

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


bool CMSSystem::IsRunning()
{
	return mIsRunning;
}