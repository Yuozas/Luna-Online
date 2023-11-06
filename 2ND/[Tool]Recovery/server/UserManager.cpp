#include "stdafx.h"
#include "UserManager.h"


CUserManager::CUserManager()
{
	// AutorityMap 초기화: 권한이 필요한 것만 설정한다
	{
		// MP_RM_CONNECT
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_CONNECT, MP_RM_CONNECT_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_SEARCH
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SEARCH, MP_RM_SEARCH_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_PLAYER
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_GET_USER_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_GET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_GET_LICENSE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_SET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_SET_EXTENDED_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PLAYER, MP_RM_PLAYER_SET_LICENSE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_ITEM
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_GET_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_GET_OPTION_ACK	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_SET_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_SET_OPTION_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_DELETE_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_DELETE_OPTION_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_DELETE_OPTION_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_FIND_OWNER_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_GET_STORAGE_ACK	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_SET_STORAGE_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_GET_EXTENDED_INVENTORY_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_UPDATE_END_TIME_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM, MP_RM_ITEM_UPDATE_REMAIN_TIME_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_ITEM_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM_LOG, MP_RM_ITEM_LOG_GET_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_ITEM_OPTION_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_ITEM_OPTION_LOG, MP_RM_ITEM_OPTION_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_SKILL
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SKILL, MP_RM_SKILL_GET_ALL_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SKILL, MP_RM_SKILL_SET_LEVEL_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SKILL, MP_RM_SKILL_ADD_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SKILL, MP_RM_SKILL_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_SKILL_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_SKILL_LOG, MP_RM_SKILL_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_FAMILY
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_GET_LIST_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_GET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_SET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_GET_MEMBER_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_GET_FARM_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY, MP_RM_FAMILY_GET_CROP_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_QUEST
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_GET_MAIN_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_GET_SUB_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_GET_ITEM_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_ITEM_UPDATE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_ITEM_DELETE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST, MP_RM_QUEST_FINISH_SUB_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_QUEST_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_QUEST_LOG, MP_RM_QUEST_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_GUILD
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD, MP_RM_GUILD_GET_LIST_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD, MP_RM_GUILD_GET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD, MP_RM_GUILD_SET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD, MP_RM_GUILD_SET_RANK_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD, MP_RM_GUILD_KICK_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_GUILD_SCORE_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD_SCORE_LOG, MP_RM_GUILD_SCORE_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_GUILD_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD_LOG, MP_RM_GUILD_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_GUILD_WAREHOUSE_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_GUILD_WAREHOUSE_LOG, MP_RM_GUILD_WAREHOUSE_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_EXP_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_EXP_LOG, MP_RM_EXP_LOG_GET_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_STAT_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_STAT_LOG, MP_RM_STAT_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_USER
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_USER, MP_RM_USER_GET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_USER, MP_RM_USER_SET_DATA_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_USER, MP_RM_USER_CREATE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_USER, MP_RM_USER_RESTORE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_USER, MP_RM_USER_DELETE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_JOB_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_JOB_LOG, MP_RM_JOB_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_FAMILY_CROP_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY_CROP_LOG, MP_RM_FAMILY_CROP_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_FAMILY_POINT_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_FAMILY_POINT_LOG, MP_RM_FAMILY_POINT_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_PERMISSION
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PERMISSION, MP_RM_PERMISSION_ADD_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_PERMISSION, MP_RM_PERMISSION_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
			}
		}

		// MP_RM_OPERATOR
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_GET_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_SET_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_IP_ADD_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_IP_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_ADD_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_SET_SYN	) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_IP_ADD_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}

			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR, MP_RM_OPERATOR_IP_REMOVE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
			}
		}

		// MP_RM_OPERATOR_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_OPERATOR_LOG, MP_RM_OPERATOR_LOG_SIZE_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}

		// MP_RM_NAME_LOG
		{
			{
				RankSet& rankSet = mAuthorityMap[ GetMessageKey( MP_RM_NAME_LOG, MP_RM_NAME_LOG_GET_SYN ) ];

				rankSet.insert( eGM_POWER_MASTER );
				rankSet.insert( eGM_POWER_PATROLLER );
				rankSet.insert( eGM_POWER_MONITOR );
			}
		}
	}
}


CUserManager::~CUserManager()
{}


CUserManager& CUserManager::GetInstance()
{
	static CUserManager instance;

	return instance;
}

/*
if( ! CUserManager::GetInstance().AddUser(
connectionIndex,
id,
name,
ip,
registedDate,
rank ) )
*/


bool CUserManager::AddUser( DWORD connectionIndex, const char* id, const char* name, const char* ip, const char* registedDate, eGM_POWER power )
{
	if( mUserMap.end() != mUserMap.find( connectionIndex ) )
	{
		return false;
	}

	User& user = mUserMap[ connectionIndex ];

	user.mId			= id;
	user.mName			= name;
	user.mIp			= ip;
	user.mRegistedDate	= registedDate;
	user.mPower			= power;

	return true;
}


void CUserManager::RemoveUser( DWORD connectionIndex )
{
	mUserMap.erase( connectionIndex );
}


bool CUserManager::IsConnect( DWORD connectionIndex ) const
{
	return mUserMap.end() != mUserMap.find( connectionIndex );
}


bool CUserManager::IsConnectedUser( const char* id ) const
{
	for(
		UserMap::const_iterator it = mUserMap.begin();
		mUserMap.end() != it;
		++it )
	{
		const User& user = it->second;

		if( user.mId == id )
		{
			return true;
		}
	}

	return false;
}


bool CUserManager::IsConnectedIp( const char* ip ) const
{
	for(
		UserMap::const_iterator it = mUserMap.begin();
		mUserMap.end() != it;
		++it )
	{
		const User& user = it->second;

		if( user.mIp == ip )
		{
			return true;
		}
	}

	return false;
}


const CUserManager::User& CUserManager::GetUser( DWORD connectionIndex ) const
{
	static const User emptyUser;

	UserMap::const_iterator it = mUserMap.find( connectionIndex );

	return mUserMap.end() == it ? emptyUser : it->second;
}


const CUserManager::User& CUserManager::GetUser( const char* id ) const
{
	static const User emptyUser;

	for(
		UserMap::const_iterator it = mUserMap.begin();
		mUserMap.end() != it;
		++it )
	{
		const User& user = it->second;

		if( user.mId == id )
		{
			return user;
		}
	}

	return emptyUser;
}


bool CUserManager::IsAuthorize( RecoveryCategory category, BYTE protocol, DWORD connectionIndex ) const
{
	const User& user = GetUser( connectionIndex );

	if( user.mId.empty() )
	{
		return false;
	}

	AutorityMap::const_iterator it = mAuthorityMap.find( GetMessageKey( category, protocol ) );

	// 권한 설정이 되지 않는 것은 모두 허용한다는 뜻
	if( mAuthorityMap.end() == it )
	{
		return true;
	}

	const RankSet& rankSet = it->second;

	return rankSet.end() != rankSet.find( user.mPower );
}


CUserManager::MessageKey CUserManager::GetMessageKey( RecoveryCategory category, BYTE protocol ) const
{
	/// double 형이 int의 두배가 아니면 메모리 덮어쓰기 등 잡을 수 없는 오류가 발생할 수 있다.
	assert( sizeof( LONGLONG ) == sizeof( int ) * 2 );

	MessageKey key = 0;

	/// 총 8바이트인 double의 전방 4바이트에 x값을 후방 4바이트에 y값을 복사해서 고유키를 만든다.
	/// 값 자체가 중요하게 아니고 유니크하냐 여부가 중요한 거니까...
	memcpy( &key, &category, sizeof( category ) );
	memcpy( ( RecoveryCategory* )&key + 1, &protocol, sizeof( protocol ) );

	return key;
}