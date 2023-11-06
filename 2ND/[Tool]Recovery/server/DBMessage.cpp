#include "stdafx.h"
#include "DBMessage.h"
#include "Network.h"
#include "UserManager.h"
#include "MSSystem.h"


typedef	void ( *DBMsgFunc )( LPQUERY, LPDBMESSAGE );


DBMsgFunc g_DBMsgFunc[ MaxQuery ] =
{
	//RGetUserList,
	RGetUserPlayerList,

	RGetPlayerList,
	//RGetPlayerIndex,
	RGetPlayerData,
	//RSetPlayerData,
	RGetExperienceLog,
	RGetStatLog,
	RGetJobLog,
	
	//RGetItemOptionList,
	RGetItemList,
	RFindItemOwner,
	RGetItemLog,
	RGetItemOptionLog,

	RGetFamilyList,		
	RGetFamilyMember,
	RGetFarmCropLog,
	RGetFamilyPointLog,

	RGetQuestMainData,
	RGetQuestSubData,
	RGetQuestItemList,
	RGetQuestLog,

	RGetGuildList,		
	RGetGuildStore,
	RGetGuildMember,
	RGetGuildFriendly,
	RGetGuildEnemy,
	RGetGuildLog,
	RGetGuildWarehouseLog,
	RGetGuildScoreLog,

	RGetSkillList,
	RGetSkillLog,

	RGetOperator,
	RGetOperatorIp,
	RGetOperatorLog,

	RGetNameLog,
};


void RLoginOperator( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	const DWORD connectionIndex = pMessage->dwID;

	if( 0 == pMessage->dwResult )
	{
		MSG_DWORD message;
		message.Category	= MP_RM_CONNECT;
		message.Protocol	= MP_RM_CONNECT_NACK;
		message.dwData		= 0;
		
		NETWORK->Send(connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = pData[ 0 ];

	const DWORD check = atoi( ( char* )record.Data[ 0 ] );

	if( check )
	{
		MSG_DWORD message;
		message.Category		= MP_RM_CONNECT;
		message.Protocol		= MP_RM_CONNECT_NACK;
		message.dwData			= check;

		NETWORK->Send(connectionIndex, message, sizeof( message ) );
		return;
	}
	else
	{
		MSGBASE message;
		message.Category	= MP_RM_CONNECT;
		message.Protocol	= MP_RM_CONNECT_ACK;

		NETWORK->Send(connectionIndex, message, sizeof( message ) );
	}

	const char*	id				= 	( char* )record.Data[ 1 ];
	const char*	name			= 	( char* )record.Data[ 2 ];
	const char*	ip				= 	( char* )record.Data[ 3 ];
	const char*	registedDate	=	( char* )record.Data[ 4 ];

	const eGM_POWER power		=	eGM_POWER( atoi( ( char* )record.Data[ 5 ] ) );
	
	if( ! CUserManager::GetInstance().AddUser(
			connectionIndex,
			id,
			name,
			ip,
			registedDate,
			power ) )
	{
		ASSERT( 0 );
	}
}


void RGetUserList( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	char userName[ MAX_NAME_LENGTH + 1 ];

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const MIDDLEQUERYST&	record		= query[ i ];
		const DWORD				userIndex	= atoi( ( char* )record.Data[ 0 ] );

		// 가져온 사용자 정보로 플레이어 정보를 요청하자
		{
			StringCopySafe( userName, ( char* )record.Data[ 1 ], sizeof( userName ) );
			
			GetPlayerList( connectionIndex, userIndex, userName, "", 0 );
		}		

		// 쿼리된 사용자 정보를 클라이언트로 보내자
		{
			MSG_NAME message;
			ZeroMemory( &message, sizeof( message ) );

			message.Category	= MP_RM_SEARCH;
			message.Protocol	= MP_RM_SEARCH_USER_NAME_ACK;
			message.dwObjectID	= userIndex;

			StringCopySafe( message.Name, userName, sizeof( message.Name ) );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
		}
	}
}


void GetPlayerList( DWORD connectionIndex, DWORD userIndex, const char* userName, const char* keyword, DWORD beginPlayerIndex )
{
	g_DB.FreeQuery(
		eGetPlayerList,
		connectionIndex,
		"EXEC DBO.TP_CHARACTER_SELECT %d, \'%s\', \'%s\', %d",
		userIndex,
		userName,
		keyword,
		beginPlayerIndex );
}


void RGetPlayerList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_OPEN_ACK	message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		MSG_RM_OPEN_ACK::Data&	data	= message.mData[ i ];
		const QUERYST&			record	= query[ i ];

		data.mUserIndex = atoi( ( char* )record.Data[ 0 ] );
		StringCopySafe( data.mUserName,		( char* )record.Data[ 1 ], sizeof( data.mUserName ) );

		data.mPlayerIndex = atoi( ( char* )record.Data[ 2 ] );
		StringCopySafe( data.mPlayerName,	( char* )record.Data[ 3 ], sizeof( data.mPlayerName ) );

		data.mStandIndex = atoi( ( char* )record.Data[ 4 ] );
	}

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		MSG_RM_OPEN_ACK::Data& data = message.mData[ MAX_ROW_NUM - 1 ];

		const char* keyword = ( char* )query[ MAX_ROW_NUM - 1 ].Data[ 5 ];

		GetPlayerList(
			connectionIndex,
			data.mUserIndex,
			data.mUserName,
			keyword,
			data.mPlayerIndex );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetPlayerLoginId( DWORD connectionIndex, DWORD userIndex, DWORD playerIndex )
{
	g_DB.LoginMiddleQuery( RGetPlayerLoginId, connectionIndex, "EXEC DBO.TP_USER_NAME_SELECT %d, %d",
		userIndex,
		playerIndex );
}


void RGetPlayerLoginId( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_USER_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD_NAME message;
	
	message.Category	= MP_RM_PLAYER;
	message.Protocol	= MP_RM_PLAYER_GET_USER_ACK;
	message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

	// 유저 인덱스
	message.dwData		= atoi( ( char* )record.Data[ 1 ] );
	
	StringCopySafe( message.Name, ( char* )record.Data[ 2 ], sizeof( message.Name ) );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void RGetPlayerData( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const QUERYST&	record = query[ 0 ];
	MSG_RM_PLAYER_DATA	message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_DATA_ACK;
	}

	{
		MSG_RM_PLAYER_DATA::Player& player = message.mPlayer;

		player.mIndex					= atoi( ( char* )record.Data[ 0 ] );		
		player.mGender					= atoi( ( char* )record.Data[ 1 ] );
		player.mStrength				= atoi( ( char* )record.Data[ 2 ] );
		player.mDexterity				= atoi( ( char* )record.Data[ 3 ] );
		player.mVitality				= atoi( ( char* )record.Data[ 4 ] );
		player.mIntelligence			= atoi( ( char* )record.Data[ 5 ] );
		player.mWisdom					= atoi( ( char* )record.Data[ 6 ] );
		player.mLife					= atoi( ( char* )record.Data[ 7 ] );
		player.mMana					= atoi( ( char* )record.Data[ 8 ] );
		player.mGrade					= atoi( ( char* )record.Data[ 9 ] );
		player.mMaxGrade				= atoi( ( char* )record.Data[ 10 ] );
		player.mExp						= strtoul( ( char* )record.Data[ 11 ], 0, 10 );
		player.mGradeUpPoint			= atoi( ( char* )record.Data[ 12 ] );
		player.mMoney					= strtoul( ( char* )record.Data[ 13 ], 0, 10 );
		player.mPlayTime				= atoi( ( char* )record.Data[ 14 ] );
		player.mSkillPoint				= atoi( ( char* )record.Data[ 15 ] );
		player.mRace					= atoi( ( char* )record.Data[ 16 ] );
		player.mExtendedInventorySize	= atoi( ( char* )record.Data[ 17 ] );
	}

	{
		MSG_RM_PLAYER_DATA::Position& position = message.mPosition;

		position.mMap	= atoi( ( char* )record.Data[ 18 ] );
		position.mX		= atoi( ( char* )record.Data[ 19 ] );
		position.mY		= atoi( ( char* )record.Data[ 20 ] );
	}

	{
		MSG_RM_PLAYER_DATA::Job& job = message.mJob;

		job.mIndex	= atoi( ( char* )record.Data[ 21 ] );
		job.mData1	= atoi( ( char* )record.Data[ 22 ] );
		job.mData2	= atoi( ( char* )record.Data[ 23 ] );
		job.mData3	= atoi( ( char* )record.Data[ 24 ] );
		job.mData4	= atoi( ( char* )record.Data[ 25 ] );
		job.mData5	= atoi( ( char* )record.Data[ 26 ] );
		job.mData6	= atoi( ( char* )record.Data[ 27 ] );
	}

	{
		MSG_RM_PLAYER_DATA::Guild& guild = message.mGuild;

		guild.mIndex		= atoi( ( char* )record.Data[ 28 ] );
		guild.mRank			= atoi( ( char* )record.Data[ 29 ] );

		StringCopySafe( guild.mName, ( char* )record.Data[ 30 ], sizeof( guild.mName ) );
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	{
		const DWORD userIndex = atoi( ( char* )record.Data[ 31 ] );

		GetPlayerLoginId( connectionIndex, userIndex, message.mPlayer.mIndex );
	}
}


void RGetPlayerExtendedData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	// 주의: MIDDLE QUERY는 필드 20개만 결과로 받을 수 있다
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	MSG_RM_PLAYER_EXTENDED_DATA message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_EXTENDED_DATA_ACK;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_RM_PLAYER_EXTENDED_DATA::Player& player = message.mPlayer;

		strncpy( player.mName, ( char* )record.Data[ 0 ], sizeof( player.mName ) - 1 );
		strncpy( player.mGuildNick, ( char* )record.Data[ 1 ], sizeof( player.mGuildNick ) - 1 );
		strncpy( player.mFamilyNick, ( char* )record.Data[ 2 ], sizeof( player.mFamilyNick ) - 1 );
		strncpy( player.mLastPlayedTime, ( char* )record.Data[ 3 ], sizeof( player.mLastPlayedTime ) - 1 );
	}

	{
		MSG_RM_PLAYER_EXTENDED_DATA::History& history = message.mHistory;

		strncpy( history.mOriginalName, ( char* )record.Data[ 4 ],	sizeof( history.mOriginalName ) - 1 );
		history.mIsEnable	= ( 2 == atoi( ( char* )record.Data[ 5 ] ) );
		strncpy( history.mCreateDate,	( char* )record.Data[ 6 ],	sizeof( history.mCreateDate ) - 1 );
		strncpy( history.mCreateIp,		( char* )record.Data[ 7 ],	sizeof( history.mCreateIp ) - 1 );
		strncpy( history.mDeletedDate,	( char* )record.Data[ 8 ],	sizeof( history.mDeletedDate ) - 1 );
		strncpy( history.mDeletedIp,	( char* )record.Data[ 9 ],	sizeof( history.mDeletedIp ) - 1 );
		strncpy( history.mRecoveredDate,( char* )record.Data[ 10 ], sizeof( history.mRecoveredDate ) - 1 );
		strncpy( history.mRecoveredIp,	( char* )record.Data[ 11 ], sizeof( history.mRecoveredIp ) - 1 );
	}

	//{
	//	MSG_RM_GET_PLAYER_EXTENDED::Guild& guild = message.mGuild;

	//	strncpy( guild.mEntryDate, ( char* )record.Data[ 3 ], sizeof( guild.mEntryDate ) - 1 );
	//	strncpy( guild.mName, ( char* )record.Data[ 4 ], sizeof( guild.mName ) - 1 );
	//}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void RSetPlayerData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( 1 != dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_DATA_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}

	//const DWORD playerIndex = atoi( ( char* )query[ 0 ].Data[ 0 ] );

	//GetPlayerData( connectionIndex, playerIndex );
}


void RSetPlayerExtendedData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( 1 != dbMessage->dwResult )
	{
		return;
	}

	const DWORD playerIndex = atoi( ( char* )query[ 0 ].Data[ 0 ] );

	if( playerIndex )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_EXTENDED_DATA_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_EXTENDED_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RGetPlayerLicense( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_LICENSE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];
	MSG_RM_PLAYER_LOVE message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_GET_LICENSE_ACK;
		message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );
	}

	{
		DATE_MATCHING_INFO&	data = message.mDateMatching;

		strncpy( data.szName, ( char* )record.Data[ 0 ], sizeof( data.szName ) );
		data.bIsValid				= true;
		data.byGender				= atoi( ( char* )record.Data[ 1 ] );
		data.dwAge					= atoi( ( char* )record.Data[ 2 ] );
		data.dwRegion				= atoi( ( char* )record.Data[ 3 ] );
		data.dwGoodFeelingDegree	= atoi( ( char* )record.Data[ 4 ] );
		data.pdwGoodFeeling[0]		= atoi( ( char* )record.Data[ 5 ] );
		data.pdwGoodFeeling[1]		= atoi( ( char* )record.Data[ 6 ] );
		data.pdwGoodFeeling[2]		= atoi( ( char* )record.Data[ 7 ] );
		data.pdwBadFeeling[0]		= atoi( ( char* )record.Data[ 8 ] );
		data.pdwBadFeeling[1]		= atoi( ( char* )record.Data[ 9 ] );
	}	

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void RPlayerSetLicense( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		ASSERT( 0 );

		MSGROOT message;
		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_LICENSE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	MSGROOT message;
	message.Category	= MP_RM_PLAYER;
	message.Protocol	= MP_RM_PLAYER_SET_LICENSE_ACK;

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void GetItemListOnInventory( DWORD connectionIndex, DWORD playerIndex, DWORD beginItemDbIndex )
{
	g_DB.FreeQuery(
		eGetItemList,
		connectionIndex,
		"SELECT  TOP %d A.ITEM_DBIDX, A.ITEM_IDX, A.ITEM_POSITION, A.ITEM_DURABILITY, A.ITEM_SEAL, DATEDIFF(second, GetDate(), A.ITEM_ENDTIME), A.ITEM_REMAINTIME,\
		ISNULL( B.REINFORCE_STRENGTH, 0 ), ISNULL( B.REINFORCE_DEXTERITY, 0 ), ISNULL( B.REINFORCE_VITALITY, 0 ), ISNULL( B.REINFORCE_INTELLIGENCE, 0 ),\
		ISNULL( B.REINFORCE_WISDOM, 0 ), ISNULL( B.REINFORCE_LIFE, 0 ), ISNULL( B.REINFORCE_MANA, 0 ), ISNULL( B.REINFORCE_LIFE_RECOVERY, 0 ),\
		ISNULL( B.REINFORCE_MANA_RECOVERY, 0 ), ISNULL( B.REINFORCE_PHYSIC_ATTACK, 0 ), ISNULL( B.REINFORCE_PHYSIC_DEFENCE, 0 ),\
		ISNULL( B.REINFORCE_MAGIC_ATTACK, 0 ), ISNULL( B.REINFORCE_MAGIC_DEFENCE, 0 ), ISNULL( B.REINFORCE_MOVE_SPEED, 0 ), ISNULL( B.REINFORCE_EVADE, 0 ),\
		ISNULL( B.REINFORCE_ACCURACY, 0 ),ISNULL( B.REINFORCE_CRITICAL_RATE, 0 ), ISNULL( B.REINFORCE_CRITICAL_DAMAGE, 0 ), ISNULL( B.REINFORCE_MADEBY, \'\' ),\
		ISNULL( B.MIX_STRENGTH, 0 ), ISNULL( B.MIX_INTELLIGENCE, 0 ), ISNULL( B.MIX_DEXTERITY, 0 ), ISNULL( B.MIX_WISDOM, 0 ), ISNULL( B.MIX_VITALITY, 0 ),\
		ISNULL( B.MIX_MADEBY, \'\' ),ISNULL( B.ENCHANT_INDEX, 0 ), ISNULL( B.ENCHANT_LEVEL, 0 ), ISNULL( B.ENCHANT_MADEBY, \'\'),\
		ISNULL( C.KEY1, 0 ), CAST( ROUND( ISNULL( C.VALUE1, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY2, 0 ), CAST( ROUND( ISNULL( C.VALUE2, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY3, 0 ), CAST( ROUND( ISNULL( C.VALUE3, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY4, 0 ), CAST( ROUND( ISNULL( C.VALUE4, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY5, 0 ), CAST( ROUND( ISNULL( C.VALUE5, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		%d,\
		%d\
		FROM TB_ITEM A LEFT OUTER JOIN TB_ITEM_DROP_OPTION C ON A.ITEM_DBIDX = C.ITEM_DBIDX LEFT OUTER JOIN TB_ITEM_OPTION2 B ON A.ITEM_DBIDX = B.ITEM_DBIDX\
		WHERE (A.CHARACTER_IDX = %d) AND (A.ITEM_DBIDX > %d) AND (A.ITEM_SHOPIDX = 0) ORDER BY A.ITEM_DBIDX",
		MAX_ROW_NUM,
		Item::AreaInventory,
		playerIndex,
		playerIndex,
		beginItemDbIndex );
}


void GetItemListOnShop( DWORD connectionIndex, DWORD userIndex, DWORD beginItemDbIndex )
{
	g_DB.FreeQuery(
		eGetItemList,
		connectionIndex,
		"SELECT  TOP %d A.ITEM_DBIDX, A.ITEM_IDX, A.ITEM_POSITION, A.ITEM_DURABILITY, A.ITEM_SEAL, DATEDIFF(second, GetDate(), A.ITEM_ENDTIME), A.ITEM_REMAINTIME,\
		ISNULL( B.REINFORCE_STRENGTH, 0 ), ISNULL( B.REINFORCE_DEXTERITY, 0 ), ISNULL( B.REINFORCE_VITALITY, 0 ), ISNULL( B.REINFORCE_INTELLIGENCE, 0 ),\
		ISNULL( B.REINFORCE_WISDOM, 0 ), ISNULL( B.REINFORCE_LIFE, 0 ), ISNULL( B.REINFORCE_MANA, 0 ), ISNULL( B.REINFORCE_LIFE_RECOVERY, 0 ),\
		ISNULL( B.REINFORCE_MANA_RECOVERY, 0 ), ISNULL( B.REINFORCE_PHYSIC_ATTACK, 0 ), ISNULL( B.REINFORCE_PHYSIC_DEFENCE, 0 ),\
		ISNULL( B.REINFORCE_MAGIC_ATTACK, 0 ), ISNULL( B.REINFORCE_MAGIC_DEFENCE, 0 ), ISNULL( B.REINFORCE_MOVE_SPEED, 0 ), ISNULL( B.REINFORCE_EVADE, 0 ),\
		ISNULL( B.REINFORCE_ACCURACY, 0 ),ISNULL( B.REINFORCE_CRITICAL_RATE, 0 ), ISNULL( B.REINFORCE_CRITICAL_DAMAGE, 0 ), ISNULL( B.REINFORCE_MADEBY, \'\' ),\
		ISNULL( B.MIX_STRENGTH, 0 ), ISNULL( B.MIX_INTELLIGENCE, 0 ), ISNULL( B.MIX_DEXTERITY, 0 ), ISNULL( B.MIX_WISDOM, 0 ), ISNULL( B.MIX_VITALITY, 0 ),\
		ISNULL( B.MIX_MADEBY, \'\' ),ISNULL( B.ENCHANT_INDEX, 0 ), ISNULL( B.ENCHANT_LEVEL, 0 ), ISNULL( B.ENCHANT_MADEBY, \'\'),\
		ISNULL( C.KEY1, 0 ), CAST( ROUND( ISNULL( C.VALUE1, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY2, 0 ), CAST( ROUND( ISNULL( C.VALUE2, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY3, 0 ), CAST( ROUND( ISNULL( C.VALUE3, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY4, 0 ), CAST( ROUND( ISNULL( C.VALUE4, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY5, 0 ), CAST( ROUND( ISNULL( C.VALUE5, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		%d,\
		%d\
		FROM TB_ITEM A LEFT OUTER JOIN TB_ITEM_DROP_OPTION C ON A.ITEM_DBIDX = C.ITEM_DBIDX LEFT OUTER JOIN TB_ITEM_OPTION2 B ON A.ITEM_DBIDX = B.ITEM_DBIDX\
		WHERE (A.ITEM_SHOPIDX = %d) AND (A.ITEM_DBIDX > %d) ORDER BY A.ITEM_DBIDX",
		MAX_ROW_NUM,
		Item::AreaCashStorage,
		userIndex,
		userIndex,
		beginItemDbIndex );
}


void GetItemListOnStorage( DWORD connectionIndex, DWORD userIndex, DWORD beginItemDbIndex )
{
	g_DB.FreeQuery(
		eGetItemList,
		connectionIndex,
		"SELECT  TOP %d A.ITEM_DBIDX, A.ITEM_IDX, A.ITEM_POSITION, A.ITEM_DURABILITY, A.ITEM_SEAL, DATEDIFF(second, GetDate(), A.ITEM_ENDTIME), A.ITEM_REMAINTIME,\
		ISNULL( B.REINFORCE_STRENGTH, 0 ), ISNULL( B.REINFORCE_DEXTERITY, 0 ), ISNULL( B.REINFORCE_VITALITY, 0 ), ISNULL( B.REINFORCE_INTELLIGENCE, 0 ),\
		ISNULL( B.REINFORCE_WISDOM, 0 ), ISNULL( B.REINFORCE_LIFE, 0 ), ISNULL( B.REINFORCE_MANA, 0 ), ISNULL( B.REINFORCE_LIFE_RECOVERY, 0 ),\
		ISNULL( B.REINFORCE_MANA_RECOVERY, 0 ), ISNULL( B.REINFORCE_PHYSIC_ATTACK, 0 ), ISNULL( B.REINFORCE_PHYSIC_DEFENCE, 0 ),\
		ISNULL( B.REINFORCE_MAGIC_ATTACK, 0 ), ISNULL( B.REINFORCE_MAGIC_DEFENCE, 0 ), ISNULL( B.REINFORCE_MOVE_SPEED, 0 ), ISNULL( B.REINFORCE_EVADE, 0 ),\
		ISNULL( B.REINFORCE_ACCURACY, 0 ),ISNULL( B.REINFORCE_CRITICAL_RATE, 0 ), ISNULL( B.REINFORCE_CRITICAL_DAMAGE, 0 ), ISNULL( B.REINFORCE_MADEBY, \'\' ),\
		ISNULL( B.MIX_STRENGTH, 0 ), ISNULL( B.MIX_INTELLIGENCE, 0 ), ISNULL( B.MIX_DEXTERITY, 0 ), ISNULL( B.MIX_WISDOM, 0 ), ISNULL( B.MIX_VITALITY, 0 ),\
		ISNULL( B.MIX_MADEBY, \'\' ),ISNULL( B.ENCHANT_INDEX, 0 ), ISNULL( B.ENCHANT_LEVEL, 0 ), ISNULL( B.ENCHANT_MADEBY, \'\'),\
		ISNULL( C.KEY1, 0 ), CAST( ROUND( ISNULL( C.VALUE1, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY2, 0 ), CAST( ROUND( ISNULL( C.VALUE2, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY3, 0 ), CAST( ROUND( ISNULL( C.VALUE3, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY4, 0 ), CAST( ROUND( ISNULL( C.VALUE4, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		ISNULL( C.KEY5, 0 ), CAST( ROUND( ISNULL( C.VALUE5, 0 ), 3, 3 ) AS VARCHAR( 16 ) ),\
		%d,\
		%d\
		FROM TB_ITEM A LEFT OUTER JOIN TB_ITEM_DROP_OPTION C ON A.ITEM_DBIDX = C.ITEM_DBIDX LEFT OUTER JOIN TB_ITEM_OPTION2 B ON A.ITEM_DBIDX = B.ITEM_DBIDX\
		WHERE (A.ITEM_PYOGUKIDX = %d) AND (A.ITEM_DBIDX > %d) ORDER BY A.ITEM_DBIDX",
		MAX_ROW_NUM,
		Item::AreaPrivateStorage,
		userIndex,
		userIndex,
		beginItemDbIndex );
}


void RGetItemList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_ITEM itemMessage;
	{
		ZeroMemory( &itemMessage, sizeof( itemMessage ) );

		itemMessage.Category	= MP_RM_ITEM;
		itemMessage.Protocol	= MP_RM_ITEM_GET_ACK;
		itemMessage.mSize		= dbMessage->dwResult;
	}

	MSG_RM_ITEM_OPTION optionMessage;
	{
		ZeroMemory( &optionMessage, sizeof( optionMessage ) );

		optionMessage.Category		= MP_RM_ITEM;
		optionMessage.Protocol		= MP_RM_ITEM_GET_OPTION_ACK;
		//optionMessage.dwObjectID	= 0;
	}

	for( DWORD  i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&	record	= query[ i ];
		Item&			item	= itemMessage.mData[ i ];

		item.dwDBIdx		= 			atoi( ( char* )record.Data[ 0 ] );
		item.wIconIdx		= 			atoi( ( char* )record.Data[ 1 ] );
		item.Position		= 			atoi( ( char* )record.Data[ 2 ] );
		item.Durability		= 			atoi( ( char* )record.Data[ 3 ] );
		item.nSealed		=			atoi( ( char* )record.Data[ 4 ] );
		item.nRemainSecond	= max( 0,	atoi( ( char* )record.Data[ 5 ] ) );
		item.LastCheckTime	= 			atoi( ( char* )record.Data[ 6 ] );
		item.mArea			= Item::Area( atoi( ( char* )record.Data[ 45 ] ) );

		{
			ITEM_OPTION option = { 0 };

			{
				ITEM_OPTION::Reinforce& data = option.mReinforce;

				data.mStrength			= atoi( ( char* )record.Data[ 7 ] );
				data.mDexterity			= atoi( ( char* )record.Data[ 8 ] );
				data.mVitality			= atoi( ( char* )record.Data[ 9 ] );
				data.mIntelligence		= atoi( ( char* )record.Data[ 10 ] );
				data.mWisdom			= atoi( ( char* )record.Data[ 11 ] );
				data.mLife				= atoi( ( char* )record.Data[ 12 ] );
				data.mMana				= atoi( ( char* )record.Data[ 13 ] );
				data.mLifeRecovery		= atoi( ( char* )record.Data[ 14 ] );
				data.mManaRecovery		= atoi( ( char* )record.Data[ 15 ] );
				data.mPhysicAttack		= atoi( ( char* )record.Data[ 16 ] );
				data.mPhysicDefence		= atoi( ( char* )record.Data[ 17 ] );
				data.mMagicAttack		= atoi( ( char* )record.Data[ 18 ] );
				data.mMagicDefence		= atoi( ( char* )record.Data[ 19 ] );
				data.mMoveSpeed			= atoi( ( char* )record.Data[ 20 ] );
				data.mEvade				= atoi( ( char* )record.Data[ 21 ] );
				data.mAccuracy			= atoi( ( char* )record.Data[ 22 ] );
				data.mCriticalRate		= atoi( ( char* )record.Data[ 23 ] );
				data.mCriticalDamage	= atoi( ( char* )record.Data[ 24 ] );
				StringCopySafe( data.mMadeBy, ( char* )record.Data[ 25 ], sizeof( data.mMadeBy ) );
			}

			{
				ITEM_OPTION::Mix& data = option.mMix;

				data.mStrength		= atoi( ( char* )record.Data[ 26 ] );
				data.mIntelligence	= atoi( ( char* )record.Data[ 27 ] );
				data.mDexterity		= atoi( ( char* )record.Data[ 28 ] );
				data.mWisdom		= atoi( ( char* )record.Data[ 29 ] );
				data.mVitality		= atoi( ( char* )record.Data[ 30 ] );
				StringCopySafe( data.mMadeBy, ( char* )record.Data[ 31 ], sizeof( data.mMadeBy ) );
			}
			
			{
				ITEM_OPTION::Enchant& data = option.mEnchant;

				data.mIndex	= atoi( ( char* )record.Data[ 32 ] );
				data.mLevel	= atoi( ( char* )record.Data[ 33 ] );
				StringCopySafe( data.mMadeBy, ( char* )record.Data[ 34 ], sizeof( data.mMadeBy ) );
			}

			{
				ITEM_OPTION::Drop& data = option.mDrop;

				data.mValue[ 0 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 35 ] ) );
				data.mValue[ 0 ].mValue	= float( atof( ( char* )record.Data[ 36 ] ) );

				data.mValue[ 1 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 37 ] ) );
				data.mValue[ 1 ].mValue	= float( atof( ( char* )record.Data[ 38 ] ) );

				data.mValue[ 2 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 39 ] ) );
				data.mValue[ 2 ].mValue	= float( atof( ( char* )record.Data[ 40 ] ) );

				data.mValue[ 3 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 41 ] ) );
				data.mValue[ 3 ].mValue	= float( atof( ( char* )record.Data[ 42 ] ) );

				data.mValue[ 4 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 43 ] ) );
				data.mValue[ 4 ].mValue	= float( atof( ( char* )record.Data[ 44 ] ) );
			}

			const ITEM_OPTION emptyOption = { 0 };

			if( ! memcmp( &emptyOption, &option, sizeof( option ) ) )
			{
				continue;
			}

			option.mItemDbIndex	= item.dwDBIdx;

			optionMessage.mData[ optionMessage.mSize++ ] = option;
		}
	}

	NETWORK->Send( connectionIndex, optionMessage,	optionMessage.GetSize() );
	NETWORK->Send( connectionIndex, itemMessage,	itemMessage.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&		record			= query[ MAX_ROW_NUM - 1 ];
		const DWORD			lastItemDbIndex	= atoi( ( char* )record.Data[ 0 ] );;
		const Item::Area	area			= Item::Area( atoi( ( char* )record.Data[ 45 ] ) );
		
		switch( area )
		{
		case Item::AreaInventory:
			{
				const DWORD	playerIndex	= atoi( ( char* )record.Data[ 46 ] );;

				GetItemListOnInventory(
					connectionIndex,
					playerIndex,
					lastItemDbIndex );
				break;
			}
		case Item::AreaCashStorage:
			{
				const DWORD	userIndex = atoi( ( char* )record.Data[ 46 ] );;

				GetItemListOnShop(
					connectionIndex,
					userIndex,
					lastItemDbIndex );
				break;
			}
		case Item::AreaPrivateStorage:
			{
				const DWORD	userIndex = atoi( ( char* )record.Data[ 46 ] );;

				GetItemListOnStorage(
					connectionIndex,
					userIndex,
					lastItemDbIndex );
				break;
			}
		}
	}
}


void RRemoveItem( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_DELETE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	const char* deletedSize	= ( char* )record.Data[ 0 ];
	const char* playerIndex = ( char* )record.Data[ 1 ];
	const char* itemDbIndex	= ( char* )record.Data[ 2 ];

	{
		MSG_DWORD message;
		message.Category	= MP_RM_ITEM;
		message.dwObjectID	= atoi( playerIndex );
		message.dwData		= atoi( itemDbIndex );

		if( atoi( deletedSize ) )
		{
			message.Protocol	= MP_RM_ITEM_DELETE_ACK;
		}
		else
		{
			message.Protocol	= MP_RM_ITEM_DELETE_NACK;
		}

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}	

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.up_ItemMoneyLog %d, \'%s\', %s, \'%s\', %d, %u, %u, %u, %d, %s, %d, %d, %d, %d", 
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
}


void FindItemOwner( DWORD connectionIndex, DWORD itemIndex, DWORD playerIndex )
{
	g_DB.FreeQuery( eFindItemOwner, connectionIndex, "EXEC DBO.TP_ITEM_OWNER_SELECT %d, %d", itemIndex, playerIndex );
}


void RFindItemOwner( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_ITEM_OWNER message;
	message.Category	= MP_RM_ITEM_FIND;
	message.Protocol	= MP_RM_ITEM_FIND_OWNER_ACK;
	message.mSize		= dbMessage->dwResult;
	message.mItemIndex	= ( dbMessage->dwResult ? atoi( ( char* )query[ 0 ].Data[ 0 ] ) : 0 );

	{
		const DWORD maxSize = sizeof( message.mPlayer ) / sizeof( message.mPlayer[ 0 ] );

		if( maxSize < message.mSize )
		{
			ASSERT( 0 );
			return;
		}
	}
		
	MSG_RM_ITEM_OWNER::Player* player		= message.mPlayer;

	for( DWORD i = 0; i < dbMessage->dwResult; ++i, ++player )
	{
		const QUERYST& record = query[ i ];
		ZeroMemory( player, sizeof( *player ) );

		player->mItemSize	= atoi( ( char* )record.Data[ 1 ] );
		player->mIndex		= atoi( ( char* )record.Data[ 2 ] );
		strncpy( player->mName, ( char* )record.Data[ 3 ], sizeof( player->mName ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void RAddItem( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_ADD_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_RM_ITEM_ADD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_RM_ITEM;
		message.Protocol		= MP_RM_ITEM_ADD_ACK;
		message.mPlayerIndex	= atoi( ( char* )record.Data[ 0 ] );
	}

	Item& item = message.mItem;
	{
		item.dwDBIdx		= atoi( ( char* )record.Data[ 1 ] );
		item.wIconIdx		= atoi( ( char* )record.Data[ 2 ] );
		item.Position		= atoi( ( char* )record.Data[ 3 ] );
		item.Durability		= atoi( ( char* )record.Data[ 4 ] );
		item.nSealed		= atoi( ( char* )record.Data[ 5 ] );
		item.mArea			= Item::Area( atoi( ( char* )record.Data[ 6 ] ) );

	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.up_ItemMoneyLog %d, \'%s\', %d, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %d, %d", 
		0,
		"",
		message.mPlayerIndex,
		std::string( "*" + user.mId ).c_str(), 
		eLog_ItemAdd,
		0,
		0,
		0,
		item.wIconIdx,
		item.dwDBIdx,
		0,
		item.Position,
		item.Durability,
		0 );

	g_DB.LogMiddleQuery(
		0,
		0,
		"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1, KEY2, VALUE2 ) VALUES ( %d, \'%s\', \'%s\', \'%d\', \'%s\', \'%d\' )",
		RecoveryLogAddItem,
		user.mId.c_str(),
		"playerIndex",
		message.mPlayerIndex,
		"itemDbIndex",
		item.dwDBIdx );
}


void RSetItemEndTime( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		return;
	}

	const MIDDLEQUERYST&	record		= query[ 0 ];
	const int				itemDbIndex	= atoi( ( char* )record.Data[ 0 ] );

	if( itemDbIndex )
	{
		MSG_INT2 message;

		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_END_TIME_ACK;
		message.nData1		= itemDbIndex;
		message.nData2		= atoi( ( char* )record.Data[ 1 ] );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_END_TIME_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void SetItemRemainTime( DWORD connectionIndex, DWORD itemDbIndex, int second )
{
	g_DB.FreeMiddleQuery(
		RSetItemRemainTime,
		connectionIndex,
		"EXEC DBO.TP_ITEM_REMAIN_TIME_UPDATE %d, %d",
		itemDbIndex,
		second );
}


void RSetItemRemainTime( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		return;
	}

	const MIDDLEQUERYST&	record		= query[ 0 ];
	const int				itemDbIndex	= atoi( ( char* )record.Data[ 0 ] );

	if( itemDbIndex )
	{
		MSG_INT2 message;

		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_REMAIN_TIME_ACK;
		message.nData1		= itemDbIndex;
		message.nData2		= atoi( ( char* )record.Data[ 1 ] );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_REMAIN_TIME_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void GetSkillList( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeQuery( eGetSkillList, connectionIndex, "EXEC DBO.MP_CHARACTER_SKILLTREEINFO %d", playerIndex );
}


void RGetSkillList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_SKILL message;
	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_GET_ALL_ACK;
	message.mSize		= dbMessage->dwResult;
	
	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		SKILL_BASE&		data	= message.mData[ i ];
		const QUERYST&	record	= query[ i ];

		data.dwDBIdx	= atoi( ( char* )record.Data[ 1 ] );
		data.wSkillIdx	= atoi( ( char* )record.Data[ 2 ] );
		data.Level		= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void SetSkillLevel( DWORD connectionIndex, DWORD playerIndex, DWORD skillDbIndex, DWORD level )
{
	g_DB.FreeMiddleQuery( RSetSkillLevel, connectionIndex, "EXEC DBO.TP_SKILL_LEVEL_UPDATE %d, %d, %d",
		playerIndex,
		skillDbIndex,
		level );
}


void RSetSkillLevel( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL;
		message.Protocol	= MP_RM_SKILL_SET_LEVEL_NACK;
		
		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD3 message;

	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_SET_LEVEL_ACK;
	message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

	// 스킬 DB 인덱스
	message.dwData1		= atoi( ( char* )record.Data[ 1 ] );

	// 스킬 인덱스
	message.dwData2		= atoi( ( char* )record.Data[ 2 ] );

	// 레벨
	message.dwData3		= atoi( ( char* )record.Data[ 3 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	// 로그를 남기자
	{
		const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );
		
		g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_SKILL_LOG_INSERT %d, %d, %d, %d, %d, %d, \'%s\'",
			message.dwObjectID,
            0,
			message.dwData1,
			message.dwData3,
			message.dwData2,			
			eLog_SkillLevel_RM,
			user.mId.c_str() );
	}
}


void AddSkillData( DWORD connectionIndex, DWORD playerIndex, DWORD skillIndex )
{
	g_DB.FreeMiddleQuery( RAddSkillData, connectionIndex, "EXEC DBO.MP_SKILLTREE_INSERT %d, %d, %d",
		playerIndex,
		skillIndex,
		1 );
}


void RAddSkillData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL;
		message.Protocol	= MP_RM_SKILL_ADD_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_RM_SKILL message;
	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_ADD_ACK;
	message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );
	message.mSize		= 1;

	SKILL_BASE& data = message.mData[ 0 ];

	{
		data.dwDBIdx	= atoi( ( char* )record.Data[ 1 ] );
		data.wSkillIdx	= atoi( ( char* )record.Data[ 2 ] );
		data.Level		= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	// 로그를 남긴다
	{
		const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );
		
		g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_SKILL_LOG_INSERT %d, %d, %d, %d, %d, %d, %s",
			message.dwObjectID,
			0,
			data.dwDBIdx,
			data.Level,
			data.wSkillIdx,
			eLog_SkillLearn_RM,
			user.mId.c_str() );
	}
}


void RemoveSkillData( DWORD connectionIndex, DWORD skillDbIndex )
{
	g_DB.FreeMiddleQuery( RRemoveSkillData, connectionIndex, "EXEC DBO.TP_SKILL_DELETE %d",
		skillDbIndex );
}


void RRemoveSkillData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL;
		message.Protocol	= MP_RM_SKILL_REMOVE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD2 message;
	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_REMOVE_ACK;
	message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

	// 스킬 DB 인덱스
	message.dwData1		= atoi( ( char* )record.Data[ 1 ] );

	// 스킬 인덱스
	message.dwData2		= atoi( ( char* )record.Data[ 2 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	// 로그를 남긴다
	{
		const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

		g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_SKILL_LOG_INSERT %d, %d, %d, %d, %d, %d, %s",
			message.dwObjectID,
			0,
			message.dwData1,
			0,
			message.dwData2,
			eLog_SkillRemove_RM,
			user.mId.c_str() );
	}
}


void GetFamilyList( DWORD connectionIndex, const char* keyword, bool isDimissed )
{
	g_DB.FreeQuery( eGetFamilyList, connectionIndex, "EXEC DBO.TP_FAMILY_LIST_SELECT \'%s\', %d", keyword, isDimissed );
}


void RGetFamilyList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_FAMILY_LIST message;
	{
		const DWORD maxSize	= sizeof( message.mFamily ) / sizeof( message.mFamily[ 0 ] );
		ASSERT( maxSize >= dbMessage->dwResult );

		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_LIST_ACK;
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_FAMILY_LIST::Family&	data	= message.mFamily[ i ];

		data.mIndex			= atoi( ( char* )record.Data[ 0 ] );
		strncpy( data.mName, ( char* )record.Data[ 1 ], sizeof( data.mName ) );
		data.mSize			= atoi( ( char* )record.Data[ 2 ] );
		data.mMasterIndex	= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetFamilyData( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeMiddleQuery( RGetFamilyData, connectionIndex, "EXEC DBO.TP_FAMILY_DATA_SELECT %d", playerIndex );
}


void RGetFamilyData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_RM_FAMILY_DATA message;
	ZeroMemory( &message, sizeof( message ) );

	message.Category		= MP_RM_FAMILY;
	message.Protocol		= MP_RM_FAMILY_GET_DATA_ACK;
	message.mIndex			= atoi( ( char* )record.Data[ 0 ] );
	strncpy( message.mName, ( char* )record.Data[ 1 ], sizeof( message.mName ) );
	message.mHonorPoint		= atoi( ( char* )record.Data[ 2 ] );
	message.mIsEnableNick	= atoi( ( char* )record.Data[ 3 ] );
	strncpy( message.mBuildDate, ( char* )record.Data[ 5 ], sizeof( message.mBuildDate ) );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex = atoi( ( char* )record.Data[ 4 ] );

	GetFamilyMember( connectionIndex, playerIndex );
}


void RSetFamilyData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( 1 != dbMessage->dwResult	||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_SET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		const DWORD point = atoi( ( char* )record.Data[ 0 ] );

		message.Category	= MP_RM_FAMILY;
		message.Protocol	= ( point ? MP_RM_FAMILY_SET_DATA_ACK : MP_RM_FAMILY_SET_DATA_NACK );
		message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );		
		message.dwData		= point;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void GetFamilyMember( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeQuery( eGetFamilyMember, connectionIndex, "EXEC DBO.TP_FAMILY_MEMBER_SELECT %d", playerIndex );
}


void RGetFamilyMember( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_MEMBER_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	MSG_RM_FAMILY_MEMBER message;
	message.Category	= MP_RM_FAMILY;
	message.Protocol	= MP_RM_FAMILY_GET_MEMBER_ACK;
	message.mSize		= dbMessage->dwResult;
	
	MSG_RM_FAMILY_MEMBER::Player* player = message.mPlayer;

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST					record	= query[ i ];
		MSG_RM_FAMILY_MEMBER::Player&	data	= message.mPlayer[ i ];

		data.mIndex		= atoi( ( char* )record.Data[ 0 ] );
		strncpy( data.mNick, ( char* )record.Data[ 1 ], sizeof( data.mNick ) );
		strncpy( data.mName, ( char* )record.Data[ 2 ], sizeof( data.mName ) );
		data.mLevel		= atoi( ( char* )record.Data[ 3 ] );
		data.mIsMaster	= atoi( ( char* )record.Data[ 4 ] );
	}	

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void GetFamilyFarm( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeMiddleQuery( RGetFamilyFarm, connectionIndex, "EXEC DBO.TP_FAMILY_FARM_SELECT %d",
		playerIndex	);
}


void RGetFamilyFarm( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_FARM_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}
	
	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_RM_FAMILY_FARM message;
	message.Category		= MP_RM_FAMILY;
	message.Protocol		= MP_RM_FAMILY_GET_FARM_ACK;
	message.dwObjectID		= atoi( ( char* )record.Data[ 0 ] );
	message.mZone			= atoi( ( char* )record.Data[ 1 ] );
	message.mFarmIndex		= atoi( ( char* )record.Data[ 2 ] );
	message.mState			= atoi( ( char* )record.Data[ 3 ] );
	message.mOwnerIndex		= atoi( ( char* )record.Data[ 4 ] );
	message.mGardenGrade	= atoi( ( char* )record.Data[ 5 ] );
	message.mHouseGrade		= atoi( ( char* )record.Data[ 6 ] );
	message.mWarehouseGrade	= atoi( ( char* )record.Data[ 7 ] );
	message.mCageGrade		= atoi( ( char* )record.Data[ 8 ] );
	message.mFenceGrade		= atoi( ( char* )record.Data[ 9 ] );
	StringCopySafe( message.mBuildDate, ( char* )record.Data[ 10 ], sizeof( message.mBuildDate ) );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void GetFamilyCrop( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeMiddleQuery( RGetFamilyCrop, connectionIndex, "EXEC DBO.TP_FAMILY_CROP_SELECT %d",
		playerIndex	);
}


void RGetFamilyCrop( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_CROP_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	MSG_RM_FAMILY_CROP message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_FAMILY;
		message.Protocol	= MP_RM_FAMILY_GET_CROP_ACK;

		const DWORD maxSize = sizeof( message.mCrop ) / sizeof( *message.mCrop );
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const MIDDLEQUERYST&	record	= query[ i ];
		MSG_RM_FAMILY_CROP::Crop&	data	= message.mCrop[ i ];

		data.mIndex			= atoi( ( char* )record.Data[ 0 ] );
		data.mOwnerIndex	= atoi( ( char* )record.Data[ 1 ] );
		StringCopySafe( data.mOwnerName, ( char* )record.Data[ 2 ], sizeof( data.mOwnerName ) );
		data.mKind			= atoi( ( char* )record.Data[ 3 ] );
		data.mStep			= atoi( ( char* )record.Data[ 4 ] );
		data.mLife			= atoi( ( char* )record.Data[ 5 ] );
		data.mNextStepTime	= atoi( ( char* )record.Data[ 6 ] );
		data.mSeedGrade		= atoi( ( char* )record.Data[ 7 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetQuestMainData( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex )
{
	g_DB.FreeQuery( eGetQuestMainData, connectionIndex, "EXEC DBO.TP_QUEST_MAIN_SELECT %d, %d", playerIndex, questIndex );
}


void RGetQuestMainData( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if( ! dbMessage->dwResult )
	//{
	//	// 퀘스트가 없을 수 있음
	//	return;
	//}

	SEND_MAINQUEST_DATA message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_GET_MAIN_DATA_ACK;
		message.wCount		= WORD( dbMessage->dwResult );
	}	

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		QMBASE&			data	= message.QuestList[ i ];
		const QUERYST&	record	= query[ i ];

		data.QuestIdx		=				atoi( ( char*)record.Data[ 0 ] );
		data.state.value	= ( QSTATETYPE )atoi( ( char*)record.Data[ 1 ] );
		data.EndParam		= 				atoi( ( char*)record.Data[ 2 ] );
		data.Time			= 				atoi( ( char*)record.Data[ 3 ] );
		data.CheckType		= 				atoi( ( char*)record.Data[ 4 ] );
		data.CheckTime		= 				atoi( ( char*)record.Data[ 5 ] );
	}

	// 최대 크기를 초과했으면 계속 쿼리한다
	{
		const DWORD maxSize = sizeof( message.QuestList ) / sizeof( *message.QuestList );

		ASSERT( maxSize >= message.wCount );

		if( maxSize == message.wCount )
		{
			const QUERYST& record = query[ maxSize - 1 ];

			const DWORD playerIndex		= atoi( ( char* )record.Data[ 6 ] );
			const DWORD lastQuestIndex	= atoi( ( char* )record.Data[ 0 ] );

			GetQuestMainData( connectionIndex, playerIndex, lastQuestIndex );
		}
	}
	
	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetQuestSubData( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex )
{
	g_DB.FreeQuery( eGetQuestSubData, connectionIndex, "EXEC DBO.TP_QUEST_SUB_SELECT %d, %d", playerIndex, questIndex );
}


void RGetQuestSubData( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	SEND_SUBQUEST_DATA message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_GET_SUB_DATA_ACK;
		message.wCount		= WORD( dbMessage->dwResult );
	}	

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		QSUBASE&		data	= message.QuestList[ i ];
		const QUERYST&	record	= query[ i ];

		data.QuestIdx		= atoi( ( char* )record.Data[ 0 ] );
		data.SubQuestIdx	= atoi( ( char* )record.Data[ 1 ] );
		data.state			= atoi( ( char* )record.Data[ 2 ] );
		data.time			= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	const DWORD playerIndex = atoi( ( char* )query[ 0 ].Data[ 4 ] );

	// 최대 크기를 초과했으면 계속 쿼리한다
	{
		const DWORD maxSize = sizeof( message.QuestList ) / sizeof( message.QuestList[ 0 ] );

		ASSERT( maxSize >= message.wCount );

		if( maxSize == message.wCount )
		{
			const DWORD lastQuestIndex	= message.QuestList[ maxSize - 1 ].QuestIdx;
			
			GetQuestSubData( connectionIndex, playerIndex, lastQuestIndex );			
			return;
		}
	}
	
	GetQuestMainData( connectionIndex, playerIndex, 0 );
	GetQuestItemList( connectionIndex, playerIndex, 0 );
}


void GetQuestItemList( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex )
{
	g_DB.FreeQuery( eGetQuestItemList, connectionIndex, "EXEC DBO.TP_QUEST_ITEM_SELECT %d, %d", playerIndex, questIndex );
}


void RGetQuestItemList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	SEND_QUESTITEM message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_GET_ITEM_DATA_ACK;
		message.wCount		= WORD( dbMessage->dwResult );
	}	

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&	record	= query[ i ];
		QITEMBASE&		data	= message.ItemList[ i ];

		data.ItemIdx	= atoi( ( char*)record.Data[ 0 ] );
		data.Count		= atoi( ( char*)record.Data[ 1 ] );
		data.QuestIdx	= atoi( ( char*)record.Data[ 2 ] );
	}

	// 최대 크기를 초과했으면 계속 쿼리한다
	{
		const DWORD maxSize = sizeof( message.ItemList ) / sizeof( message.ItemList[ 0 ] );

		ASSERT( maxSize >= message.wCount );

		if( maxSize == message.wCount )
		{
			const QUERYST& record = query[ maxSize - 1 ];

			const DWORD lastQuestIndex	= atoi( ( char* )record.Data[ 2 ] );
			const DWORD	playerIndex		= atoi( ( char* )record.Data[ 3 ] );

			GetQuestItemList( connectionIndex, playerIndex, lastQuestIndex );
		}
	}	

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void EndSubQuest( DWORD connectionIndex, DWORD playerIndex, DWORD mainQuestIndex, DWORD subQuestIndex, QSTATETYPE state )
{
	g_DB.FreeMiddleQuery( REndSubQuest, connectionIndex, "EXEC DBO.TP_QUEST_SUB_DELETE %d, %d, %d, %d",
		playerIndex,
		mainQuestIndex,
		subQuestIndex,
		state );
}


void REndSubQuest( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		ASSERT( 0 );

		MSGROOT message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_FINISH_SUB_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST&	record	= query[ 0 ];

	MSG_DWORD3 message;
	{
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_FINISH_SUB_ACK;

		// 메인 퀘스트 인덱스
		message.dwData1	= atoi( ( char*)record.Data[ 0 ] );

		// 서브 퀘스트 인덱스
		message.dwData2 = atoi( ( char*)record.Data[ 1 ] );

		// 서브 퀘스트 종료 플래그
		message.dwData3	= atoi( ( char*)record.Data[ 2 ] );
	}
	
	
	NETWORK->Send( connectionIndex, message, sizeof( message ));
}


void RemoveQuestData( DWORD connectionIndex, DWORD playerIndex, DWORD mainQuestIndex )
{
	g_DB.FreeMiddleQuery( RRemoveQuestData, connectionIndex, "EXEC DBO.TP_QUEST_MAIN_DELETE %d, %d",
		playerIndex,
		mainQuestIndex );
}


void RRemoveQuestData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_REMOVE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_QUEST;
	message.Protocol	= MP_RM_QUEST_REMOVE_ACK;
	message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

	// 메인 퀘스트 인덱스
	message.dwData		= atoi( ( char* )record.Data[ 1 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void RUpdateQuestItem( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult	||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_ITEM_UPDATE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_DWORD2	message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_ITEM_UPDATE_ACK;
		message.dwData1		= atoi( ( char* )query[ 0 ].Data[ 1 ] );	// 퀘스트 아이템 인덱스
		message.dwData2		= atoi( ( char* )query[ 0 ].Data[ 2 ] );	// 수량

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}	
}


void DeleteQuestItem( DWORD connectionIndex, DWORD playerIndex, DWORD questMainIndex, DWORD itemIndex )
{
	g_DB.FreeMiddleQuery(
		RDeleteQuestItem,
		connectionIndex,
		"EXEC DBO.TP_QUEST_ITEM_DELETE %d, %d, %d",
		playerIndex,
		questMainIndex,
		itemIndex );
}


void RDeleteQuestItem( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult	||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_ITEM_DELETE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_DWORD	message;
		message.Category	= MP_RM_QUEST;
		message.Protocol	= MP_RM_QUEST_ITEM_DELETE_ACK;
		message.dwData		= atoi( ( char* )query[ 0 ].Data[ 1 ] );	// 아이템 인덱스
		
		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void GetQuestLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetQuestLogSize,
		connectionIndex,
		"EXEC DBO.TP_QUEST_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetQuestLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_QUEST_LOG;
		message.Protocol	= MP_RM_QUEST_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_QUEST_LOG;
	message.Protocol	= MP_RM_QUEST_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex		= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate		=		( char* )record.Data[ 2 ];
	const char*	endDate			=		( char* )record.Data[ 3 ];

	GetQuestLog( connectionIndex, playerIndex, beginDate, endDate );
}


void GetQuestLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetQuestLog,
		connectionIndex,
		"EXEC DBO.TP_QUEST_LOG_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetQuestLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_QUEST_LOG, MP_RM_QUEST_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_QUEST_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_QUEST_LOG;
		message.Protocol	= MP_RM_QUEST_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_QUEST_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex			=				atoi( ( char* )record.Data[ 0 ] );
		data.mMainQuestIndex	=				atoi( ( char* )record.Data[ 1 ] );
		data.mSubQuestIndex		= 				atoi( ( char* )record.Data[ 2 ] );
		data.mValue1			= 				atoi( ( char* )record.Data[ 3 ] );
		data.mValue2			=				atoi( ( char* )record.Data[ 4 ] );
		data.mType				= eQuestLog(	atoi( ( char* )record.Data[ 5 ] ) );

		StringCopySafe( data.mDate, ( char* )record.Data[ 6 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 7 ], sizeof( data.mMemo ) );
	}
	
	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST& record = query[ MAX_ROW_NUM - 1 ];

		const char*	beginDate	=		( char* )record.Data[ 6 ];
		const char* endDate		=		( char* )record.Data[ 8 ];
		const DWORD	playerIndex	= atoi( ( char* )record.Data[ 9 ] );
		
		GetQuestLog( connectionIndex, playerIndex, beginDate, endDate );
	}
}


void GetGuildList( DWORD connectionIndex, const char* keyword, bool isDimissed )
{
	g_DB.FreeQuery(
		eGetGuildList,
		connectionIndex,
		"EXEC DBO.TP_GUILD_LIST_SELECT \'%s\', %d",
		keyword,
		isDimissed );
}


void RGetGuildList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;
	
	MSG_RM_GUILD_LIST message;
	{
		const DWORD maxSize	= sizeof( message.mGuild ) / sizeof( message.mGuild[ 0 ] );
		ASSERT( maxSize >= dbMessage->dwResult );

		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_LIST_ACK;
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_GUILD_LIST::Guild&	data	= message.mGuild[ i ];

		data.mIndex	= atoi( ( char* )record.Data[ 0 ] );
		strncpy( data.mName, ( char* )record.Data[ 1 ], sizeof( data.mName ) );
		data.mSize	= atoi( ( char* )record.Data[ 2 ] );
		data.mLevel	= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetGuildData( DWORD connectionIndex, const char* keyword, DWORD guildIndex )
{
	//g_DB.FreeQuery( eGetGuildData, connectionIndex, "EXEC DBO.TP_GUILD_DATA_SELECT \'%s\', %d", keyword, guildIndex );

	g_DB.FreeMiddleQuery( RGetGuildData, connectionIndex, "EXEC DBO.TP_GUILD_DATA_SELECT \'%s\', %d", keyword, guildIndex );
}


//void RGetGuildData( LPQUERY query, LPDBMESSAGE dbMessage )
void RGetGuildData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message) );
		return;
	}

	//const QUERYST&	record		= query[ 0 ];
	const MIDDLEQUERYST&	record		= query[ 0 ];
	
	MSG_RM_GUILD_DATA message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_ACK;
		message.mGuildIndex	= atoi( ( char* )record.Data[ 0 ] );
		message.mLevel		= atoi( ( char* )record.Data[ 1 ] );
		message.mScore		= atoi( ( char* )record.Data[ 2 ] );
		message.mMoney		= atoi( ( char* )record.Data[ 3 ] );
		strncpy( message.mGuildName, ( char* )record.Data[ 4 ], sizeof( message.mGuildName ) );
		strncpy( message.mBuiltDate, ( char* )record.Data[ 5 ], sizeof( message.mBuiltDate ) );
		strncpy( message.mAllianceName, ( char* )record.Data[ 6 ], sizeof( message.mAllianceName ) );
		message.mAllianceEntryDate	= atoi( ( char* )record.Data[ 7 ] );
	}	

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	{
		const DWORD guildIndex = message.mGuildIndex;

		GetGuildEnemy( connectionIndex, guildIndex );
		GetGuildFriendly( connectionIndex, guildIndex );
		GetGuildStore( connectionIndex, guildIndex );
		GetGuildMember( connectionIndex, guildIndex );
	}
}


void GetGuildStore( DWORD connectionIndex, DWORD guildIndex )
{
	g_DB.FreeQuery( eGetGuildStore, connectionIndex, "EXEC DBO.TP_GUILD_STORE_SELECT %d", guildIndex );
}


void RGetGuildStore( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_GUILD_STORE message;
	{
		const DWORD maxSize	= sizeof( message.mItem ) / sizeof( message.mItem[ 0 ] );
		ASSERT( maxSize >= dbMessage->dwResult );

		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_STORE_ACK;
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_GUILD_STORE::Item&	data	= message.mItem[ i ];

		data.mDbIndex	= atoi( ( char* )record.Data[ 0 ] );
		data.mIndex		= atoi( ( char* )record.Data[ 1 ] );
		data.mQuantity	= atoi( ( char* )record.Data[ 2 ] );
	}
	
	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetGuildMember( DWORD connectionIndex, DWORD guildIndex )
{
	g_DB.FreeQuery(
		eGetGuildMember,
		connectionIndex,
		"EXEC DBO.TP_GUILD_MEMBER_SELECT %d",
		guildIndex );
}


void RGetGuildMember( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_GUILD_MEMBER message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_MEMBER_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&					record	= query[ i ];
		MSG_RM_GUILD_MEMBER::Player&	data	= message.mPlayer[ i ];

		data.mIndex	= atoi( ( char* )record.Data[ 0 ] );		
		StringCopySafe( data.mName, ( char* )record.Data[ 1 ], sizeof( data.mName ) );
		data.mLevel	= atoi( ( char* )record.Data[ 2 ] );
		data.mRank	= atoi( ( char* )record.Data[ 3 ] );
		StringCopySafe( data.mDate, ( char* )record.Data[ 4 ], sizeof( data.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetGuildFriendly( DWORD connectionIndex, DWORD guildIndex )
{
	g_DB.FreeQuery( eGetGuildFriendly, connectionIndex, "EXEC DBO.TP_GUILD_FRIENDLY_SELECT %d", guildIndex );
}


void RGetGuildFriendly( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_GUILD_LIST message;
	{
		const DWORD maxSize	= sizeof( message.mGuild ) / sizeof( message.mGuild[ 0 ] );
		ASSERT( maxSize >= dbMessage->dwResult );

		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_FRIENDLY_ACK;
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_GUILD_LIST::Guild&	data	= message.mGuild[ i ];

		data.mIndex	= atoi( ( char* )record.Data[ 0 ] );
		strncpy( data.mName, ( char* )record.Data[ 1 ], sizeof( data.mName ) );
		data.mSize	= atoi( ( char* )record.Data[ 2 ] );
		data.mLevel	= atoi( ( char* )record.Data[ 3 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GetGuildEnemy( DWORD connectionIndex, DWORD guildIndex )
{
	g_DB.FreeQuery( eGetGuildEnemy, connectionIndex, "EXEC DBO.TP_GUILD_ENEMY_SELECT %d", guildIndex );
}


void RGetGuildEnemy( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_GUILD_LIST message;
	{
		const DWORD maxSize	= sizeof( message.mGuild ) / sizeof( message.mGuild[ 0 ] );
		ASSERT( maxSize >= dbMessage->dwResult );

		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_ENEMY_ACK;
		message.mSize		= min( maxSize, dbMessage->dwResult );
	}

	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_GUILD_LIST::Guild&	data	= message.mGuild[ i ];

		data.mIndex	= atoi( ( char* )record.Data[ 0 ] );
		strncpy( data.mName, ( char* )record.Data[ 1 ], sizeof( data.mName ) );
		data.mSize	= atoi( ( char* )record.Data[ 2 ] );
		data.mLevel	= atoi( ( char* )record.Data[ 3 ] );
		data.mMoney	= atoi( ( char* )record.Data[ 4 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void GuildUpdateData( DWORD connectionIndex, const MSG_RM_GUILD_DATA& message )
{
	g_DB.FreeMiddleQuery( RGuildUpdateData, connectionIndex, "EXEC DBO.TP_GUILD_DATA_UPDATE %d, \'%s\', \'%s\', %d, %d, %d",
		message.mGuildIndex,
		message.mGuildName,
		message.mAllianceName,
		message.mLevel,
		message.mMoney,
		message.mScore );
}


void RGuildUpdateData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		ASSERT( 0 );

		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_SET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_SET_DATA_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}	
}


void GuildSetRank( DWORD connectionIndex, DWORD playerIndex, DWORD guildIndex, DWORD rank )
{
	g_DB.FreeMiddleQuery( RGuildSetRank, connectionIndex, "EXEC DBO.TP_GUILD_RANK_UPDATE %d, %d, %d, %d",
		guildIndex,
		playerIndex,
		rank,
		GUILD_MASTER == rank );
}


void RGuildSetRank( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] )  )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_SET_RANK_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD	message;
	{
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_SET_RANK_ACK;
		message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

		// 직위
		message.dwData		= atoi( ( char* )record.Data[ 1 ] );
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	// 로그 남기자
	{
		const DWORD guildIndex		= atoi( ( char* )record.Data[ 2 ] );
		const DWORD previousRank	= atoi( ( char* )record.Data[ 3 ] );

		g_DB.LogQuery(
			0,
			0,
			0,
			"EXEC DBO.UP_GUILDLOG %d, %d, %d, %d, %d",
			message.dwObjectID,
			guildIndex,
			eGuildLog_GM_ChangeRank,
			previousRank,
			message.dwData );
	}
}


void GuildKickMember( DWORD connectionIndex, DWORD playerIndex )
{
	g_DB.FreeMiddleQuery(
		RGuildKickMember,
		connectionIndex,
		"EXEC DBO.TP_GUILD_MEMBER_DELETE %d",
		playerIndex );
}


void RGuildKickMember( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( 1 != dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_KICK_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	// 로그 남기자

	const MIDDLEQUERYST&	record = query[ 0 ];
	const DWORD				result = atoi( ( char* )record.Data[ 0 ] );

	switch( result )
	{
	// 성공
	case 0:
		{
			MSGBASE message;
			message.Category	= MP_RM_GUILD;
			message.Protocol	= MP_RM_GUILD_KICK_ACK;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );

			// 로그 남기자
			{
				const DWORD guildIndex		= atoi( ( char* )record.Data[ 2 ] );
				
				g_DB.LogMiddleQuery(
					0,
					0, 
					"EXEC DBO.UP_GUILDLOG %d, %d, %d, %d, %d",
					message.dwObjectID,
					guildIndex,
					eGuildLog_GM_KIck_Member,
					0,
					0 );
			}

			break;
		}

	// 소속 길드 없음
	case 1:
		{
			MSGBASE message;
			message.Category	= MP_RM_GUILD;
			message.Protocol	= MP_RM_GUILD_KICK_NACK_BY_NO_GUILD;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}

	// 회원 수 없음
	case 2:
		{
			MSGBASE message;
			message.Category	= MP_RM_GUILD;
			message.Protocol	= MP_RM_GUILD_KICK_NACK_BY_ONE_MEMBER;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			MSGROOT message;
			message.Category	= MP_RM_GUILD;
			message.Protocol	= MP_RM_GUILD_KICK_NACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	}
}


void GetExperienceLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetExperienceLogSize,
		connectionIndex,
		"EXEC DBO.TP_EXP_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetExperienceLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
			0 == atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_EXP_LOG;
		message.Protocol	= MP_RM_EXP_LOG_GET_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_EXP_LOG;
	message.Protocol	= MP_RM_EXP_LOG_GET_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );
	
	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex		= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate		=		( char* )record.Data[ 2 ];
	const char*	endDate			=		( char* )record.Data[ 3 ];
	
	GetExperienceLog( connectionIndex, playerIndex, beginDate, endDate );
}


void GetExperienceLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		//RGetExperienceLog,
		eQueryType_FreeQuery,
		eGetExperienceLog,
		connectionIndex,
		"EXEC DBO.TP_EXP_LOG_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetExperienceLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_EXP_LOG, MP_RM_EXP_LOG_GET_STOP_ACK );
	//	return;
	//}

	MSG_RM_EXP_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_EXP_LOG;
		message.Protocol	= MP_RM_EXP_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_EXP_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mType			= atoi( ( char* )record.Data[ 1 ] );
		data.mGrade			= atoi( ( char* )record.Data[ 2 ] );
		data.mVariation		= atoi( ( char* )record.Data[ 3 ] );
		data.mKillerKind	= atoi( ( char* )record.Data[ 4 ] );
		data.mKillerIndex	= atoi( ( char* )record.Data[ 5 ] );
		data.mExperience	= atoi( ( char* )record.Data[ 6 ] );

		StringCopySafe( data.mDate, ( char* )record.Data[ 7 ], sizeof( data.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		playerIndex	= atoi( ( char* )record.Data[ 8 ] );
		const char*		beginDate	=		( char* )record.Data[ 7 ];
		const char*		endDate		=		( char* )record.Data[ 9 ];
		
		GetExperienceLog( connectionIndex, playerIndex, beginDate, endDate );
	}
}


void RGetItemLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM_LOG;
		message.Protocol	= MP_RM_ITEM_LOG_GET_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_ITEM_LOG;
	message.Protocol	= MP_RM_ITEM_LOG_GET_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const char* playerIndex		= ( char* )record.Data[ 1 ];
	const char* itemDbIndex		= ( char* )record.Data[ 2 ];
	const char* itemIndex		= ( char* )record.Data[ 3 ];
	const char*	beginDate		= ( char* )record.Data[ 4 ];
	const char*	endDate			= ( char* )record.Data[ 5 ];
	const char* requestTick		= ( char* )record.Data[ 6 ];

	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetItemLog,
		connectionIndex,
		"EXEC DBO.TP_ITEM_LOG_SELECT %s, %s, %s, \'%s\', \'%s\', %s",
		playerIndex,
		itemDbIndex,
		itemIndex,
		beginDate,
		endDate,
		requestTick );
}


void RGetItemLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_ITEM_LOG, MP_RM_ITEM_LOG_GET_STOP_ACK );
	//	return;
	//}

	MSG_RM_ITEM_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_RM_ITEM_LOG;
		message.Protocol		= MP_RM_ITEM_LOG_GET_ACK;
		message.mSize			= dbMessage->dwResult;

		if( 0 < dbMessage->dwResult )
		{
			message.mRequestTick = atoi( ( char* )query[ 0 ].Data[ 19 ] );
		}		
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&	record			= query[ i ];
		MSG_RM_ITEM_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex	=						atoi( ( char* )record.Data[ 0 ] );
		data.mType		=	( eLogitemmoney )	atoi( ( char* )record.Data[ 1 ] );
		
		{
			MSG_RM_ITEM_LOG::Player& player = data.mFromPlayer;

			player.mIndex	= atoi( ( char* )record.Data[ 2 ] );
			StringCopySafe( player.mName, ( char* )record.Data[ 3 ], sizeof( player.mName ) );
			player.mMoney	= atoi( ( char* )record.Data[ 4 ] );
		}

		{
			MSG_RM_ITEM_LOG::Player& player = data.mToPlayer;

			player.mIndex	= atoi( ( char* )record.Data[ 5 ] );
			StringCopySafe( player.mName, ( char* )record.Data[ 6 ], sizeof( player.mName ) );
			player.mMoney	= atoi( ( char* )record.Data[ 7 ] );
		}

		data.mMoneyVariation	= atoi( ( char* )record.Data[ 8 ] );

		{
			MSG_RM_ITEM_LOG::Item& item = data.mItem;

			item.mIndex			= atoi( ( char* )record.Data[ 9 ] );
			item.mDbIndex		= atoi( ( char* )record.Data[ 10 ] );
			item.mQuantity		= atoi( ( char* )record.Data[ 11 ] );
			item.mFromPosition	= atoi( ( char* )record.Data[ 12 ] );
			item.mToPosition	= atoi( ( char* )record.Data[ 13 ] );
		}

		StringCopySafe( data.mDate, ( char* )record.Data[ 14 ], sizeof( data.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST& record	= query[ MAX_ROW_NUM - 1 ];

		const char*	beginDate		= ( char* )record.Data[ 14 ];
		const char* endDate			= ( char* )record.Data[ 15 ];
		const char* playerIndex		= ( char* )record.Data[ 16 ];
		const char* itemDbIndex		= ( char* )record.Data[ 17 ];
		const char* itemIndex		= ( char* )record.Data[ 18 ];
		
		g_DB.LogQuery(
			eQueryType_FreeQuery,
			eGetItemLog,
			connectionIndex,
			"EXEC DBO.TP_ITEM_LOG_SELECT %s, %s, %s, \'%s\', \'%s\', %d",
			playerIndex,
			itemDbIndex,
			itemIndex,
			beginDate,
			endDate,
			message.mRequestTick );
	}
}


void GetStatLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetStatLogSize,
		connectionIndex,
		"EXEC DBO.TP_STAT_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetStatLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_STAT_LOG;
		message.Protocol	= MP_RM_STAT_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_STAT_LOG;
	message.Protocol	= MP_RM_STAT_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	= ( char* )record.Data[ 2 ];
	const char*	endDate		= ( char* )record.Data[ 3 ];

	GetStatLog( connectionIndex, playerIndex, beginDate, endDate );
}


void GetStatLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetStatLog,
		//RGetStatLog,
		connectionIndex,
		"EXEC DBO.TP_STAT_LOG_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetStatLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_STAT_LOG, MP_RM_STAT_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_STAT_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_STAT_LOG;
		message.Protocol	= MP_RM_STAT_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_STAT_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mLevel			= atoi( ( char* )record.Data[ 1 ] );
		data.mStrength		= atoi( ( char* )record.Data[ 2 ] );
		data.mDexterity		= atoi( ( char* )record.Data[ 3 ] );
		data.mVitality		= atoi( ( char* )record.Data[ 4 ] );
		data.mIntelligence	= atoi( ( char* )record.Data[ 5 ] );
		data.mWisdom		= atoi( ( char* )record.Data[ 6 ] );
		data.mExperience	= atoi( ( char* )record.Data[ 7 ] );

		StringCopySafe( data.mDate, ( char* )record.Data[ 8 ], sizeof( data.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		playerIndex	= atoi( ( char* )record.Data[ 9 ] );
		const char*		beginDate	=		( char* )record.Data[ 8 ];
		const char*		endDate		=		( char* )record.Data[ 10 ];

		GetStatLog( connectionIndex, playerIndex, beginDate, endDate );
	}
}


void GetSkillLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetSkillLogSize,
		connectionIndex,
		"EXEC DBO.TP_SKILL_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetSkillLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_SKILL_LOG;
		message.Protocol	= MP_RM_SKILL_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_SKILL_LOG;
	message.Protocol	= MP_RM_SKILL_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex		= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate		=		( char* )record.Data[ 2 ];
	const char*	endDate			=		( char* )record.Data[ 3 ];

	GetSkillLog( connectionIndex, playerIndex, beginDate, endDate );
}


void GetSkillLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		//RGetSkillLog,
		eQueryType_FreeQuery,
		eGetSkillLog,
		connectionIndex,
		"EXEC DBO.TP_SKILL_LOG_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetSkillLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_SKILL_LOG, MP_RM_SKILL_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_SKILL_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_SKILL_LOG;
		message.Protocol	= MP_RM_SKILL_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_SKILL_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex	=				atoi( ( char* )record.Data[ 0 ] );
		data.mType		= ( eLogSkill )	atoi( ( char* )record.Data[ 1 ] );
		data.mDbIndex	=				atoi( ( char* )record.Data[ 2 ] );
		data.mIndex		= 				atoi( ( char* )record.Data[ 3 ] );
		data.mLevel		=				atoi( ( char* )record.Data[ 4 ] );
		data.mPoint		= 				atoi( ( char* )record.Data[ 5 ] );
		
		StringCopySafe( data.mDate, ( char* )record.Data[ 6 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 7 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		playerIndex	= atoi( ( char* )record.Data[ 8 ] );
		const char*		beginDate	=		( char* )record.Data[ 6 ];
		const char*		endDate		=		( char* )record.Data[ 9 ];

		GetSkillLog( connectionIndex, playerIndex, beginDate, endDate );
	}
}


void GetItemOptionLogSize( DWORD connectionIndex, DWORD itemIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetItemOptionLogSize,
		connectionIndex,
		"EXEC DBO.TP_ITEM_OPTION_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		itemIndex,
		beginDate,
		endDate );
}


void RGetItemOptionLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_ITEM_OPTION_LOG;
		message.Protocol	= MP_RM_ITEM_OPTION_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_ITEM_OPTION_LOG;
	message.Protocol	= MP_RM_ITEM_OPTION_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD itemDbIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	=		( char* )record.Data[ 2 ];
	const char*	endDate		=		( char* )record.Data[ 3 ];

	GetItemOptionLog( connectionIndex, itemDbIndex, beginDate, endDate );
}


void GetItemOptionLog( DWORD connectionIndex, DWORD itemDbIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetItemOptionLog,
		connectionIndex,
		"EXEC DBO.TP_ITEM_OPTION_LOG_SELECT %d, \'%s\', \'%s\'",
		itemDbIndex,
		beginDate,
		endDate );
}


void RGetItemOptionLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_ITEM_OPTION_LOG, MP_RM_ITEM_OPTION_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_ITEM_OPTION_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_ITEM_OPTION_LOG;
		message.Protocol	= MP_RM_ITEM_OPTION_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&					record	= query[ i ];
		MSG_RM_ITEM_OPTION_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mItemDbIndex	= atoi( ( char* )record.Data[ 1 ] );
		
		{
			ITEM_OPTION::Reinforce& reinforce = data.mOption.mReinforce;

			reinforce.mStrength			= atoi( ( char* )record.Data[ 2 ] );
			reinforce.mDexterity		= atoi( ( char* )record.Data[ 3 ] );
			reinforce.mVitality			= atoi( ( char* )record.Data[ 4 ] );
			reinforce.mIntelligence		= atoi( ( char* )record.Data[ 5 ] );
			reinforce.mWisdom			= atoi( ( char* )record.Data[ 6 ] );
			reinforce.mLife				= atoi( ( char* )record.Data[ 7 ] );
			reinforce.mMana				= atoi( ( char* )record.Data[ 8 ] );
			reinforce.mLifeRecovery		= atoi( ( char* )record.Data[ 9 ] );
			reinforce.mManaRecovery		= atoi( ( char* )record.Data[ 10 ] );
			reinforce.mPhysicAttack		= atoi( ( char* )record.Data[ 11 ] );	
			reinforce.mPhysicDefence	= atoi( ( char* )record.Data[ 12 ] );
			reinforce.mMagicAttack		= atoi( ( char* )record.Data[ 13 ] );
			reinforce.mMagicDefence		= atoi( ( char* )record.Data[ 14 ] );
			reinforce.mCriticalRate		= atoi( ( char* )record.Data[ 15 ] );
			reinforce.mCriticalDamage	= atoi( ( char* )record.Data[ 16 ] );
			reinforce.mMoveSpeed		= atoi( ( char* )record.Data[ 17 ] );
			reinforce.mEvade			= atoi( ( char* )record.Data[ 18 ] );
			reinforce.mAccuracy			= atoi( ( char* )record.Data[ 19 ] );
		}

		{
			ITEM_OPTION::Mix& mix = data.mOption.mMix;

			mix.mStrength		= atoi( ( char* )record.Data[ 20 ] );
			mix.mIntelligence	= atoi( ( char* )record.Data[ 21 ] );
			mix.mDexterity		= atoi( ( char* )record.Data[ 22 ] );
			mix.mWisdom			= atoi( ( char* )record.Data[ 23 ] );
			mix.mVitality		= atoi( ( char* )record.Data[ 24 ] );
		}

		{
			ITEM_OPTION::Enchant& enchant = data.mOption.mEnchant;
			
            enchant.mIndex	= atoi( ( char* )record.Data[ 25 ] );
			enchant.mLevel	= atoi( ( char* )record.Data[ 26 ] );
		}

		data.mType	= ( eLogitemmoney )atoi( ( char* )record.Data[ 27 ] );

		StringCopySafe( data.mDate, ( char* )record.Data[ 28 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 29 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		itemDbIndex	= atoi( ( char* )record.Data[ 0 ] );
		const char*		beginDate	=		( char* )record.Data[ 28 ];
		const char*		endDate		=		( char* )record.Data[ 30 ];
		
		GetItemOptionLog( connectionIndex, itemDbIndex, beginDate, endDate );
	}
}


void GetGuildLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetGuildLogSize,
		connectionIndex,
		"EXEC DBO.TP_GUILD_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate );
}


void RGetGuildLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_LOG;
		message.Protocol	= MP_RM_GUILD_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_GUILD_LOG;
	message.Protocol	= MP_RM_GUILD_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD guildIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	=		( char* )record.Data[ 2 ];
	const char*	endDate		=		( char* )record.Data[ 3 ];

	GetGuildLog( connectionIndex, guildIndex, beginDate, endDate );
}


void GetGuildLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetGuildLog,
		//RGetGuildLog,
		connectionIndex,
		"EXEC DBO.TP_GUILD_LOG_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate);
}


void RGetGuildLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_GUILD_LOG, MP_RM_GUILD_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_GUILD_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD_LOG;
		message.Protocol	= MP_RM_GUILD_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_GUILD_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		=				atoi( ( char* )record.Data[ 0 ] );
		data.mType			= ( eGuildLog )	atoi( ( char* )record.Data[ 1 ] );
		data.mPlayerIndex	=				atoi( ( char* )record.Data[ 2 ] );
		data.mValue1		= 				atoi( ( char* )record.Data[ 3 ] );
		data.mValue2		= 				atoi( ( char* )record.Data[ 4 ] );
		
		StringCopySafe( data.mDate, ( char* )record.Data[ 5 ], sizeof( data.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		guildIndex	= atoi( ( char* )record.Data[ 6 ] );
		const char*		beginDate	=		( char* )record.Data[ 5 ];
		const char*		endDate		=		( char* )record.Data[ 7 ];

		GetGuildLog( connectionIndex, guildIndex, beginDate, endDate );
	}
}


void GetGuildWarehouseLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetGuildWarehouseLogSize,
		connectionIndex,
		"EXEC DBO.TP_GUILD_WAREHOUSE_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate );
}


void RGetGuildWarehouseLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
			0 == atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
		message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
	message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD guildIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char* beginDate	=		( char* )record.Data[ 2 ];
	const char* endDate		=		( char* )record.Data[ 3 ];

	GetGuildWarehouseLog( connectionIndex, guildIndex, beginDate, endDate );
}


void GetGuildWarehouseLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetGuildWarehouseLog,
		//RGetGuildWarehouseLog,
		connectionIndex,
		"EXEC DBO.TP_GUILD_WAREHOUSE_LOG_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate );
}


void RGetGuildWarehouseLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_GUILD_WAREHOUSE_LOG, MP_RM_GUILD_WAREHOUSE_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_GUILD_WAREHOUSE_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
		message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&						record	= query[ i ];
		MSG_RM_GUILD_WAREHOUSE_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mPlayerIndex	= atoi( ( char* )record.Data[ 1 ] );

		{
			ITEMBASE& item = data.mItem;

			item.wIconIdx	= atoi( ( char* )record.Data[ 2 ] );
			item.dwDBIdx	= atoi( ( char* )record.Data[ 3 ] );
			item.Durability	= atoi( ( char* )record.Data[ 4 ] );
			item.Position	= atoi( ( char* )record.Data[ 5 ] );
		}

		data.mItemFromPosition	=					atoi( ( char* )record.Data[ 6 ] );
		data.mMoney				=					atoi( ( char* )record.Data[ 7 ] );
		data.mType				= ( eLogitemmoney )	atoi( ( char* )record.Data[ 8 ] );
		
		StringCopySafe( data.mDate, ( char* )record.Data[ 9 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 10 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		guildIndex	= atoi( ( char* )record.Data[ 12 ] );
		const char*		beginDate	=		( char* )record.Data[ 9 ];
		const char*		endDate		=		( char* )record.Data[ 11 ];

		GetGuildWarehouseLog( connectionIndex, guildIndex, beginDate, endDate );
	}
}


void RGetUserIndex( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	for( DWORD row = 0; row < dbMessage->dwResult; ++row )
	{
		const MIDDLEQUERYST& record = query[ row ];

		const DWORD userIndex = atoi( ( char* )record.Data[ 0 ] );

		g_DB.LoginMiddleQuery(
			RGetUserList,
			connectionIndex,
			"EXEC DBO.TP_USER_SELECT %d, \'%s\', \'%s\'",
			userIndex,
			"",
			"" );
	}
}


void RGetUserData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_GET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_RM_USER_DATA message;
	ZeroMemory( &message, sizeof( message ) );
	message.Category	= MP_RM_USER;
	message.Protocol	= MP_RM_USER_GET_DATA_ACK;

	message.mIndex		= atoi( ( char* )record.Data[ 0 ] );
	StringCopySafe( message.mId, ( char* )record.Data[ 1 ], sizeof( message.mName ) );
	StringCopySafe( message.mName, ( char* )record.Data[ 2 ], sizeof( message.mName ) );
	StringCopySafe( message.mJoinedDate, ( char* )record.Data[ 3 ], sizeof( message.mJoinedDate ) );
	StringCopySafe( message.mLogedoutDate, ( char* )record.Data[ 4 ], sizeof( message.mLogedoutDate ) );
	message.mPlayTime	= atoi( ( char* )record.Data[ 5 ] );
	message.mLevel		= atoi( ( char* )record.Data[ 6 ] );
	
	{
		MSG_RM_USER_DATA::Connection& data = message.mConnection;

		StringCopySafe( data.mIp, ( char* )record.Data[ 7 ], sizeof( data.mIp ) );
		data.mAgent	= atoi( ( char* )record.Data[ 8 ] );
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	g_DB.FreeQuery(
		eGetUserPlayerList,
		connectionIndex,
		"EXEC DBO.TP_CHARACTER_SELECT %d, \'%s\', \'%s\', %d",
		message.mIndex,
		"",
		"",
		0 );
}


void RGetUserPlayerList( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_OPEN_ACK	message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_GET_PLAYER_LIST_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		MSG_RM_OPEN_ACK::Data&	data	= message.mData[ i ];
		const QUERYST&			record	= query[ i ];

		data.mUserIndex = atoi( ( char* )record.Data[ 0 ] );
		StringCopySafe( data.mUserName,		( char* )record.Data[ 1 ], sizeof( data.mUserName ) );

		data.mPlayerIndex = atoi( ( char* )record.Data[ 2 ] );
		StringCopySafe( data.mPlayerName,	( char* )record.Data[ 3 ], sizeof( data.mPlayerName ) );

		data.mStandIndex = atoi( ( char* )record.Data[ 4 ] );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST& record = query[ MAX_ROW_NUM - 1 ];

		g_DB.FreeQuery(
			eGetUserPlayerList,
			connectionIndex,
			"EXEC DBO.TP_CHARACTER_SELECT %s, \'%s\', \'%s\', %s",
			( char* )record.Data[ 0 ],
			"",
			"",
			( char* )record.Data[ 5 ] );
	}
}


void SetUserData( DWORD connectionIndex, DWORD userIndex, LEVELTYPE level )
{
	g_DB.LoginMiddleQuery(
		RSetUserData,
		connectionIndex,
		"EXEC DBO.TP_USER_DATA_UPDATE %d, %d",
		userIndex,
		level );
}


void RSetUserData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT	message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_SET_DATA_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD2 message;
	message.Category	= MP_RM_USER;
	message.Protocol	= MP_RM_USER_SET_DATA_ACK;
	message.dwData1		= atoi( ( char* )record.Data[ 0 ] );
	message.dwData2		= atoi( ( char* )record.Data[ 1 ] );
	
	NETWORK->Send( connectionIndex, message, sizeof( message ) );
}


void UserAdd( DWORD connectionIndex, const CHARACTERMAKEINFO& data )
{
	// 에이전트 서버의 CreateCharacter()에서 복사함

	const BYTE StrArray[ 2 ][ 3 ] = { {16, 12,  8 }, { 13, 11,  7 } };
	const BYTE DexArray[ 2 ][ 3 ] = { {10, 14,  7 }, { 12, 15,  8 } };
	const BYTE VitArray[ 2 ][ 3 ] = { {14, 12, 11 }, { 13, 11,  9 } };
	const BYTE IntArray[ 2 ][ 3 ] = { { 6,  7, 13 }, {  7,  8, 14 } };
	const BYTE WisArray[ 2 ][ 3 ] = { { 7,  8, 14 }, {  8,  8, 15 } };

	const BYTE race	= data.RaceType;
	const BYTE job	= data.JobType -1;

	const BYTE Str = StrArray[ race ][ job ] * 5;
	const BYTE Dex = DexArray[ race ][ job ] * 5;
	const BYTE Vit = VitArray[ race ][ job ] * 5;
	const BYTE Int = IntArray[ race ][ job ] * 5;
	const BYTE Wis = WisArray[ race ][ job ] * 5;

	const DWORD item[3][2] =
	{
		11000001,	12000032,
		11000187,	12000032,
		11000249,	12000001
	};

	const int loginPoint = 2019;
	const int map = 19;

	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	g_DB.FreeMiddleQuery(
		RUserAdd,
		connectionIndex,
		"EXEC DBO.MP_CHARACTER_CreateCharacter %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, \'GM:%s\', %u, %u",
		data.UserID,
		Str,
		Dex,
		Vit,
		Int,
		Wis, 
		data.Name,
		data.FaceType,
		data.HairType,
		map,
		data.SexType,
		data.RaceType,
		data.JobType,
		loginPoint,
		0,
		user.mId.c_str(),
		item[ job % 3 ][ 0 ],
		item[ job % 3 ][ 1 ] );
}


void RUserAdd( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_CREATE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	switch( atoi( ( char* )record.Data[ 0 ] ) )
	{
	case 0:
		{
			MSGROOT message;
			{
				message.Category	= MP_RM_USER;
				message.Protocol	= MP_RM_USER_CREATE_ACK;
			}

			NETWORK->Send( connectionIndex, message, sizeof( message ) );

			{
				const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

				if( user.mId.empty() )
				{
					return;
				}

				g_DB.LogMiddleQuery(
					0,
					0,
					"INSERT INTO TB_OPERATOR_LOG ( TYPE, OPER_ID, KEY1, VALUE1 ) VALUES ( %d, \'%s\', \'%s\', %s )",
					RecoveryLogAddPlayerToUser,
					user.mId.c_str(),
					"playerIndex",
					( char* )record.Data[ 1 ] );
			}

			break;
		}
	case 1:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_CREATE_NACK_BY_FULL;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 2:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_CREATE_NACK_BY_DUPLICATED_NAME;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 3:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_CREATE_NACK_BY_GUILD;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			break;
		}
	}
}


void RestoreUser( DWORD connectionIndex, DWORD playerIndex, const char* playerName )
{
	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	g_DB.FreeMiddleQuery(
		RRestoreUser,
		connectionIndex,
		"EXEC DBO.TP_CharacterRecover %d, \'%s\', \'GM:%s\', %d",
		playerIndex,
		playerName,
		user.mId.c_str(),		// IP: 복구해준 RM툴 ID를 넣는다
		0 );			// 서버 번호. RM툴이 복구하므로 0을 넣는다
}


void RRestoreUser( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_RESTORE_NACK;
		
		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	switch( atoi( ( char* )record.Data[ 0 ] ) )
	{
	case 0:
		{
			MSG_NAME message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_ACK;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );
			StringCopySafe( message.Name, ( char* )record.Data[ 2 ], sizeof( message.Name ) );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 1:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK_BY_NOT_EXIST;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 2:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK_BY_FULL;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 3:
		{
			MSG_NAME message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK_BY_DUPLICATED_NAME;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );
			StringCopySafe( message.Name, ( char* )record.Data[ 2 ], sizeof( message.Name ) );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 4:
		{
			MSG_NAME message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK_BY_GUILD;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );
			StringCopySafe( message.Name, ( char* )record.Data[ 2 ], sizeof( message.Name ) );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 5:
		{
			MSG_NAME message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK_BY_FAMILY;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );
			StringCopySafe( message.Name, ( char* )record.Data[ 2 ], sizeof( message.Name ) );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );

			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_RESTORE_NACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	}
}


void DeleteUser( DWORD connectionIndex, DWORD playerIndex )
{
	const CUserManager::User& user = CUserManager::GetInstance().GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return;
	}

	g_DB.FreeMiddleQuery(
		RDeleteUser,
		connectionIndex,
		"EXEC DBO.MP_CHARACTER_DeleteCharacter %d, %d, \'GM:%s\'",
		playerIndex,
		0,				// IP: 복구해준 RM툴 ID를 넣는다
		user.mId.c_str() );		// 서버 번호. RM툴이 복구하므로 0을 넣는다
}


void RDeleteUser( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult )
	{
		MSGROOT message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_DELETE_NACK;
		
		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	switch( atoi( ( char* )record.Data[ 0 ] ) )
	{
	case 0:
		{
			MSGBASE message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_DELETE_ACK;
			message.dwObjectID	= atoi( ( char* )record.Data[ 1 ] );

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 1:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_DELETE_NACK_BY_PARTY;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 2:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_DELETE_NACK_BY_GUILD;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	case 3:
		{
			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_DELETE_NACK_BY_FAMILY;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	default:
		{
			ASSERT( 0 );

			MSGROOT message;
			message.Category	= MP_RM_USER;
			message.Protocol	= MP_RM_USER_DELETE_NACK;

			NETWORK->Send( connectionIndex, message, sizeof( message ) );
			break;
		}
	}
}


//void GetJobLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
//{
//	g_DB.LogMiddleQuery(
//		RGetJobLogSize,
//		connectionIndex,
//		"EXEC DBO.TP_JOB_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
//		playerIndex,
//		beginDate,
//		endDate );
//}


void RGetJobLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_JOB_LOG;
		message.Protocol	= MP_RM_JOB_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_JOB_LOG;
	message.Protocol	= MP_RM_JOB_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD playerIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	=		( char* )record.Data[ 2 ];
	const char*	endDate		=		( char* )record.Data[ 3 ];

	GetJobLog( connectionIndex, playerIndex, beginDate, endDate );
}


void GetJobLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetJobLog,
		//RGetJobLog,
		connectionIndex,
		"EXEC DBO.TP_JOB_LOG_SELECT %d, \'%s\', \'%s\'",
		playerIndex,
		beginDate,
		endDate );
}


void RGetJobLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	/*if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
		!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	{
		g_pMSSystem->StopQuery(	connectionIndex, MP_RM_JOB_LOG, MP_RM_JOB_LOG_STOP_ACK );
		return;
	}*/

	MSG_RM_JOB_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_JOB_LOG;
		message.Protocol	= MP_RM_JOB_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_JOB_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mClassIndex	= atoi( ( char* )record.Data[ 1 ] );
		data.mJobGrade		= atoi( ( char* )record.Data[ 2 ] );
		data.mJobIndex		= atoi( ( char* )record.Data[ 3 ] );
		
		StringCopySafe( data.mDate, ( char* )record.Data[ 4 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 5 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record = query[ MAX_ROW_NUM - 1 ];
		const DWORD		playerIndex	= atoi( ( char* )record.Data[ 6 ] );
		const char*		beginDate	=		( char* )record.Data[ 4 ];
		const char*		endDate		=		( char* )record.Data[ 7 ];

		GetJobLog( connectionIndex, playerIndex, beginDate, endDate );
	}
}


void GetFarmCropLogSize( DWORD connectionIndex, DWORD farmIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetFarmCropLogSize,
		connectionIndex,
		"EXEC DBO.TP_FARM_CROP_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		farmIndex,
		beginDate,
		endDate );
}


void RGetFarmCropLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY_CROP_LOG;
		message.Protocol	= MP_RM_FAMILY_CROP_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_FAMILY_CROP_LOG;
	message.Protocol	= MP_RM_FAMILY_CROP_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD farmIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	=		( char* )record.Data[ 2 ];
	const char*	endDate		=		( char* )record.Data[ 3 ];

	GetFarmCropLog( connectionIndex, farmIndex, beginDate, endDate );
}


void GetFarmCropLog( DWORD connectionIndex, DWORD farmIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		//RGetFarmCropLog,
		eGetFarmCropLog,
		connectionIndex,
		"EXEC DBO.TP_FARM_CROP_LOG_SELECT %d, \'%s\', \'%s\'",
		farmIndex,
		beginDate,
		endDate );
}


void RGetFarmCropLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	/*if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
		!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	{
		g_pMSSystem->StopQuery(	connectionIndex, MP_RM_FAMILY_CROP_LOG, MP_RM_FAMILY_CROP_LOG_STOP_ACK );
		return;
	}*/

	MSG_RM_FAMILY_CROP_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_FAMILY_CROP_LOG;
		message.Protocol	= MP_RM_FAMILY_CROP_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&					record	= query[ i ];
		MSG_RM_FAMILY_CROP_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mOwnerIndex	= atoi( ( char* )record.Data[ 1 ] );
		data.mCropIndex		= atoi( ( char* )record.Data[ 2 ] );
		data.mCropKind		= atoi( ( char* )record.Data[ 3 ] );
		data.mCropStep		= atoi( ( char* )record.Data[ 4 ] );
		data.mCropLife		= atoi( ( char* )record.Data[ 5 ] );
		data.mCropSeedGrade	= atoi( ( char* )record.Data[ 6 ] );
		data.mType			= ( eFamilyLog )	atoi( ( char* )record.Data[ 7 ] );

		StringCopySafe( data.mDate, ( char* )record.Data[ 8 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 9 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		farmIndex	= atoi( ( char* )record.Data[ 10 ] );
		const char*		beginDate	=		( char* )record.Data[ 8 ];
		const char*		endDate		=		( char* )record.Data[ 11 ];

		GetFarmCropLog( connectionIndex, farmIndex, beginDate, endDate );
	}
}


void GetFamilyPointLogSize( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetFamilyPointLogSize,
		connectionIndex,
		"EXEC DBO.TP_FAMILY_POINT_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		familyIndex,
		beginDate,
		endDate );
}


void RGetFamilyPointLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_FAMILY_POINT_LOG;
		message.Protocol	= MP_RM_FAMILY_POINT_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_FAMILY_POINT_LOG;
	message.Protocol	= MP_RM_FAMILY_POINT_LOG_SIZE_ACK;

	// 쿼리 개수
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD familyIndex		= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate		=		( char* )record.Data[ 2 ];
	const char*	endDate			=		( char* )record.Data[ 3 ];

	GetFamilyPointLog( connectionIndex, familyIndex, beginDate, endDate );
}


void GetFamilyPointLog( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		//RGetFamilyPointLog,
		eGetFamilyPointLog,
		connectionIndex,
		"EXEC DBO.TP_FAMILY_POINT_LOG_SELECT %d, \'%s\', \'%s\'",
		familyIndex,
		beginDate,
		endDate );
}


void RGetFamilyPointLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	//if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
	//	!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	//{
	//	g_pMSSystem->StopQuery(	connectionIndex, MP_RM_FAMILY_POINT_LOG, MP_RM_FAMILY_POINT_LOG_STOP_ACK );
	//	return;
	//}

	MSG_RM_FAMILY_POINT_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_FAMILY_POINT_LOG;
		message.Protocol	= MP_RM_FAMILY_POINT_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	// 첫번째 레코드는 다음 쿼리를 위한 정보가 담겨있다
	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&					record	= query[ i ];
		MSG_RM_FAMILY_POINT_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		=				atoi( ( char* )record.Data[ 0 ] );
		data.mPoint			=				atoi( ( char* )record.Data[ 1 ] );
		data.mType			= ( eFamilyLog )atoi( ( char* )record.Data[ 2 ] );

		StringCopySafe( data.mDate, ( char* )record.Data[ 3 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 4 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record		= query[ MAX_ROW_NUM - 1 ];
		const DWORD		familyIndex	= atoi( ( char* )record.Data[ 5 ] );
		const char*		beginDate	=		( char* )record.Data[ 3 ];
		const char*		endDate		=		( char* )record.Data[ 6 ];

		GetFamilyPointLog( connectionIndex, familyIndex, beginDate, endDate );
	}
}


void GetGuildScoreLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogMiddleQuery(
		RGetGuildScoreLogSize,
		connectionIndex,
		"EXEC DBO.TP_GUILD_SCORE_LOG_SIZE_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate );
}


void RGetGuildScoreLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( !	dbMessage->dwResult ||
		0 == atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_GUILD_SCORE_LOG;
		message.Protocol	= MP_RM_GUILD_SCORE_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_DWORD message;
	message.Category	= MP_RM_GUILD_SCORE_LOG;
	message.Protocol	= MP_RM_GUILD_SCORE_LOG_SIZE_ACK;
	message.dwData		= atoi( ( char* )record.Data[ 0 ] );

	NETWORK->Send( connectionIndex, message, sizeof( message ) );

	const DWORD guildIndex	= atoi( ( char* )record.Data[ 1 ] );
	const char*	beginDate	=		( char* )record.Data[ 2 ];
	const char*	endDate		=		( char* )record.Data[ 3 ];

	GetGuildScoreLog( connectionIndex, guildIndex, beginDate, endDate );
}


void GetGuildScoreLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate )
{
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetGuildScoreLog,
		//RGetGuildScoreLog,
		connectionIndex,
		"EXEC DBO.TP_GUILD_SCORE_LOG_SELECT %d, \'%s\', \'%s\'",
		guildIndex,
		beginDate,
		endDate );
}


void RGetGuildScoreLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	/*if(		g_pMSSystem->IsStopQuery( connectionIndex ) ||
		!	CUserManager::GetInstance().IsConnect( connectionIndex ) )
	{
		g_pMSSystem->StopQuery(	connectionIndex, MP_RM_GUILD_SCORE_LOG, MP_RM_GUILD_SCORE_LOG_STOP_ACK );
		return;
	}*/

	MSG_RM_GUILD_SCORE_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD_SCORE_LOG;
		message.Protocol	= MP_RM_GUILD_SCORE_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&					record	= query[ i ];
		MSG_RM_GUILD_SCORE_LOG::Log&	data	= message.mLog[ i ];

		data.mLogIndex		= atoi( ( char* )record.Data[ 0 ] );
		data.mScore			= atoi( ( char* )record.Data[ 1 ] );
		data.mType			= ( eGuildLog )	atoi( ( char* )record.Data[ 2 ] );

		// 날짜 복사
		StringCopySafe( data.mDate, ( char* )record.Data[ 3 ], sizeof( data.mDate ) );
		StringCopySafe( data.mMemo, ( char* )record.Data[ 4 ], sizeof( data.mMemo ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST& record = query[ MAX_ROW_NUM - 1 ];
		const DWORD guildIndex	= atoi( ( char* )record.Data[ 5 ] );
		const char* beginDate	=		( char* )record.Data[ 3 ];
		const char* endDate		=		( char* )record.Data[ 6 ];

		GetGuildScoreLog( connectionIndex, guildIndex, beginDate, endDate );
	}
}


void AddPermission( DWORD connectionIndex, const char* ipAddress )
{
    g_DB.LoginMiddleQuery(
		RAddPermission,
		connectionIndex,
		"EXEC DBO.TP_PERMISSION_INSERT \'%s\'",
		ipAddress );
}


void RAddPermission( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		return;
	}

	const MIDDLEQUERYST&	record		= query[ 0 ];
	const bool				isSuccess	= ( 0 < atoi( ( char* )record.Data[ 0 ] ) );

	if( isSuccess )
	{
		MSGROOT message;
		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_ADD_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_ADD_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RemovePermission( DWORD connectionIndex, const char* ipAddress )
{
	g_DB.LoginMiddleQuery(
		RRemovePermission,
		connectionIndex,
		"EXEC DBO.TP_PERMISSION_DELETE \'%s\'",
		ipAddress );
}


void RRemovePermission( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult )
	{
		return;
	}

	const MIDDLEQUERYST&	record		= query[ 0 ];
	const bool				isSuccess	= ( 0 < atoi( ( char* )record.Data[ 0 ] ) );

	if( isSuccess )
	{
		MSGROOT message;
		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_REMOVE_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
	else
	{
		MSGROOT message;
		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_REMOVE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RGetStorageData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_DWORD3 message;
	ZeroMemory( &message, sizeof( message ) );

	message.Category	= MP_RM_ITEM;
	message.Protocol	= MP_RM_ITEM_GET_STORAGE_ACK;

	if( 1 == dbMessage->dwResult )
	{
		const MIDDLEQUERYST& record = query[ 0 ];

		// 창고 크기
		message.dwData1	= atoi( ( char* )record.Data[ 1 ] );

		// 창고 보유금
		message.dwData2	= atoi( ( char* )record.Data[ 2 ] );

		// 확장 인벤토리 크기
		message.dwData3	= atoi( ( char* )record.Data[ 3 ] );	
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );	
}


void RSetStorageData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_DWORD2 message;
	ZeroMemory( &message, sizeof( message ) );

	message.Category	= MP_RM_ITEM;
	message.Protocol	= MP_RM_ITEM_SET_STORAGE_ACK;

	if( 1 == dbMessage->dwResult )
	{
		const MIDDLEQUERYST& record = query[ 0 ];

		// 창고 크기
		message.dwData1	= atoi( ( char* )record.Data[ 1 ] );

		// 창고 보유금
		message.dwData2	= atoi( ( char* )record.Data[ 2 ] );
	}

	NETWORK->Send( connectionIndex, message, sizeof( message ) );	
}


void RGetOperator( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_OPERATOR message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}
	
	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&				record	= query[ i ];
		MSG_RM_OPERATOR::Operator&	data	= message.mOperator[ i ];

		data.mIndex	=				atoi( ( char* )record.Data[ 0 ] );
		data.mPower	= eGM_POWER(	atoi( ( char* )record.Data[ 1 ] ) );

		StringCopySafe( data.mId,			( char* )record.Data[ 2 ],	sizeof( data.mId ) );
		StringCopySafe( data.mName,			( char* )record.Data[ 3 ],	sizeof( data.mName ) );
		StringCopySafe( data.mRegistedDate,	( char* )record.Data[ 4 ],	sizeof( data.mRegistedDate ) );

		const CUserManager::User& user = CUserManager::GetInstance().GetUser( data.mId );

		if( ! user.mId.empty() )
		{
			StringCopySafe( data.mIp, user.mIp.c_str(), sizeof( data.mIp ) );
		}
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void RAddOperator( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] )  )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_ADD_NACK_BY_ID;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}
	
	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_RM_OPERATOR message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_ADD_ACK;
		message.mSize		= 1;

		MSG_RM_OPERATOR::Operator& data = message.mOperator[ 0 ];
		data.mIndex			=				atoi( ( char* )record.Data[ 0 ] );
		data.mPower			= eGM_POWER(	atoi( ( char* )record.Data[ 1 ] ) );
        
		StringCopySafe( data.mId,			( char* )record.Data[ 2 ],	sizeof( data.mId ) );
		StringCopySafe( data.mName,			( char* )record.Data[ 3 ],	sizeof( data.mName ) );
		StringCopySafe( data.mRegistedDate,	( char* )record.Data[ 4 ],	sizeof( data.mRegistedDate ) );
		
		NETWORK->Send( connectionIndex, message, message.GetSize() );
	}
}


void RUpdateOperator( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] )  )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_SET_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_DWORD message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_SET_ACK;

		// operator index
		message.dwObjectID	= atoi( ( char* )record.Data[ 0 ] );

		// rank
		message.dwData		= atoi( ( char* )record.Data[ 1 ] );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RGetOperatorIp( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_OPERATOR_IP message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&		record	= query[ i ];
		MSG_RM_OPERATOR_IP::Address& address = message.mAddress[ i ];

		StringCopySafe( address.mIp, ( char* )record.Data[ 0 ], sizeof( address.mIp ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}


void RAddOperatorIp( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] )  )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_ADD_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_NAME message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_ADD_ACK;
		StringCopySafe( message.Name, ( char* )record.Data[ 0 ], sizeof( message.Name ) );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RRemoveOperatorIp( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] )  )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_REMOVE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSG_NAME message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_REMOVE_ACK;
		StringCopySafe( message.Name, ( char* )record.Data[ 0 ], sizeof( message.Name ) );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RSetPassword( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult ||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_PASSWORD;
		message.Protocol	= MP_RM_PASSWORD_SET_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	//const MIDDLEQUERYST& record = query[ 0 ];

	{
		MSGROOT message;
		message.Category	= MP_RM_PASSWORD;
		message.Protocol	= MP_RM_PASSWORD_SET_ACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}
}


void RGetOperatorLogSize( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	if( ! dbMessage->dwResult	||
		! atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR_LOG;
		message.Protocol	= MP_RM_OPERATOR_LOG_SIZE_NACK;

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];
	
	MSG_RM_LOG_SIZE message;
	{
		message.Category		= MP_RM_OPERATOR_LOG;
		message.Protocol		= MP_RM_OPERATOR_LOG_SIZE_ACK;
		message.mSize			= atoi( ( char* )record.Data[ 0 ] );
		message.mRequestTick	= atoi( ( char* )record.Data[ 3 ] );

		NETWORK->Send( connectionIndex, message, sizeof( message ) );
	}	

	const char* beginDate	= ( char* )record.Data[ 1 ];
	const char* endDate		= ( char* )record.Data[ 2 ];
	
	g_DB.LogQuery(
		eQueryType_FreeQuery,
		eGetOperatorLog,
		connectionIndex,
		"EXEC DBO.TP_OPERATOR_LOG_SELECT \'%s\', \'%s\', %d",
		beginDate,
		endDate,
		message.mRequestTick );
}


void RGetOperatorLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_OPERATOR_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_RM_OPERATOR_LOG;
		message.Protocol		= MP_RM_OPERATOR_LOG_ACK;
		message.mSize			= dbMessage->dwResult;

		if( 0 < dbMessage->dwResult )
		{
			message.mRequestTick	= atoi( ( char* )query[ 0 ].Data[ 11 ] );
		}
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )	
	{
		const QUERYST&				record	= query [ i ];
		MSG_RM_OPERATOR_LOG::Log&	log		= message.mLog[ i ];

		log.mIndex	= atoi( ( char* )record.Data[ 0 ] );
		log.mType	= RecoveryLog( atoi( ( char* )record.Data[ 1 ] ) );
		StringCopySafe( log.mOperator,	( char* )record.Data[ 2 ], sizeof( log.mOperator ) );
		StringCopySafe( log.mDate,		( char* )record.Data[ 3 ], sizeof( log.mDate ) );
		StringCopySafe( log.mKey1,		( char* )record.Data[ 4 ], sizeof( log.mKey1 ) );
		StringCopySafe( log.mValue1,	( char* )record.Data[ 5 ], sizeof( log.mValue1 ) );
		StringCopySafe( log.mKey2,		( char* )record.Data[ 6 ], sizeof( log.mKey2 ) );
		StringCopySafe( log.mValue2,	( char* )record.Data[ 7 ], sizeof( log.mValue2 ) );
		StringCopySafe( log.mKey3,		( char* )record.Data[ 8 ], sizeof( log.mKey3 ) );
		StringCopySafe( log.mValue3,	( char* )record.Data[ 9 ], sizeof( log.mValue3 ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST& record = query[ MAX_ROW_NUM - 1 ];

		const char* beginDate	= ( char* )record.Data[ 3 ];
		const char* endDate		= ( char* )record.Data[ 10 ];
		
		g_DB.LogQuery(
			eQueryType_FreeQuery,
			eGetOperatorLog,
			connectionIndex,
			"EXEC DBO.TP_OPERATOR_LOG_SELECT \'%s\', \'%s\', %d",
			beginDate,
			endDate,
			message.mRequestTick );
	}
}


void RGetNameLog( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD connectionIndex = dbMessage->dwID;

	MSG_RM_NAME_LOG message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_NAME_LOG;
		message.Protocol	= MP_RM_NAME_LOG_GET_ACK;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&			record	= query[ i ];
		MSG_RM_NAME_LOG::Log&	log		= message.mLog[ i ];

		log.mIndex	= atoi( ( char* )record.Data[ 0 ] );

		StringCopySafe( log.mPreviousName,	( char* )record.Data[ 1 ], sizeof( log.mPreviousName ) );
		StringCopySafe( log.mName,			( char* )record.Data[ 2 ], sizeof( log.mName ) );
		StringCopySafe( log.mDate,			( char* )record.Data[ 3 ], sizeof( log.mDate ) );
	}

	NETWORK->Send( connectionIndex, message, message.GetSize() );
}