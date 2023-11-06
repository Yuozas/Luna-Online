//#include "..\..\[CC]Header\CommonStructMS.h"



#define MAX_DATE_SIZE	30


// 080311 LUJ, 문자열 안전 복사
inline void StringCopySafe( TCHAR* rhs, const TCHAR* lhs, size_t size )
{
	_tcsncpy( rhs, lhs, size );

	rhs[ size - 1 ] = 0;
}


struct Item : public ITEMBASE
{
	enum Area
	{
		AreaNone,
		AreaInventory,
		AreaCashStorage,
		AreaPrivateStorage,
	}
	mArea;
};

#pragma once
#pragma pack(push,1)

enum RecoveryCategory
{
	MP_RM_CONNECT,
	MP_RM_SEARCH,
	MP_RM_PLAYER,
	MP_RM_ITEM,
	MP_RM_ITEM_FIND,
	MP_RM_ITEM_LOG,
	MP_RM_ITEM_OPTION_LOG,
	MP_RM_SKILL,
	MP_RM_SKILL_LOG,
	MP_RM_FAMILY,
	MP_RM_QUEST,
	MP_RM_QUEST_LOG,
	MP_RM_GUILD,
	MP_RM_GUILD_LOG,
	MP_RM_GUILD_WAREHOUSE_LOG,
	MP_RM_EXP_LOG,	
	MP_RM_STAT_LOG,	
	MP_RM_USER,
	//MP_RM_USER_CREATE,
	//MP_RM_USER_RESTORE,
	MP_RM_JOB_LOG,
	//MP_RM_PLAYER_ITEM_LOG,
	MP_RM_GUILD_SCORE_LOG,
	MP_RM_FAMILY_CROP_LOG,
	MP_RM_FAMILY_POINT_LOG,
	MP_RM_PERMISSION,
	MP_RM_OPERATOR,
	MP_RM_OPERATOR_LOG,
	MP_RM_PASSWORD,	
	MP_RM_NAME_LOG,
};


//enum RecoveryProtocol
enum
{
	MP_RM_CONNECT_SYN,
	MP_RM_CONNECT_ACK,
	MP_RM_CONNECT_NACK,
	MP_RM_CONNECT_NACK_BY_IP,	// 한 아이피에서 중복 접속 시도
	MP_RM_CONNECT_NACK_BY_ID,	// 한 아이디로 중복 접속 시도
	MP_RM_CONNECT_NACK_BY_AUTH,

	// 080311 LUJ, 버전 차이로 인해 접속 못할 경우
	MP_RM_CONNECT_NACK_BY_VERSION,
};


enum
{
	MP_RM_SEARCH_SYN,
	MP_RM_SEARCH_ACK,
	MP_RM_SEARCH_NACK,
	MP_RM_SEARCH_NACK_BY_AUTH,
	MP_RM_SEARCH_USER_NAME_ACK,
	//MP_RM_SEARCH_NO_RESULT_ACK,	// 속도 저하 요인으로 차단
};


enum
{
	MP_RM_PLAYER_NACK_BY_AUTH,

	MP_RM_PLAYER_GET_USER_SYN,
	MP_RM_PLAYER_GET_USER_ACK,
	MP_RM_PLAYER_GET_USER_NACK,
	
	MP_RM_PLAYER_GET_DATA_SYN,
	MP_RM_PLAYER_GET_DATA_ACK,
	MP_RM_PLAYER_GET_EXTENDED_DATA_ACK,
	MP_RM_PLAYER_GET_DATA_NACK,

	MP_RM_PLAYER_GET_LOGIN_ID_NACK,
	MP_RM_PLAYER_GET_LOGIN_ID_ACK,

	MP_RM_PLAYER_SET_DATA_SYN,
	MP_RM_PLAYER_SET_DATA_ACK,
	MP_RM_PLAYER_SET_DATA_NACK,

	MP_RM_PLAYER_SET_EXTENDED_DATA_SYN,
	MP_RM_PLAYER_SET_EXTENDED_DATA_ACK,
	MP_RM_PLAYER_SET_EXTENDED_DATA_NACK,

	MP_RM_PLAYER_GET_LICENSE_SYN,
	MP_RM_PLAYER_GET_LICENSE_ACK,
	MP_RM_PLAYER_GET_LICENSE_NACK,

	MP_RM_PLAYER_SET_LICENSE_SYN,
	MP_RM_PLAYER_SET_LICENSE_ACK,
	MP_RM_PLAYER_SET_LICENSE_NACK,
};

enum
{
	MP_RM_ITEM_NACK_BY_AUTH,

	MP_RM_ITEM_GET_SYN,
	MP_RM_ITEM_GET_ACK,
	//MP_RM_ITEM_GET_NACK,

	//MP_RM_ITEM_GET_OPTION_SYN,
	MP_RM_ITEM_GET_OPTION_ACK,
	MP_RM_ITEM_GET_OPTION_NACK,

	MP_RM_ITEM_SET_SYN,
	MP_RM_ITEM_SET_ACK,
	MP_RM_ITEM_SET_NACK,

	MP_RM_ITEM_SET_OPTION_SYN,
	MP_RM_ITEM_SET_OPTION_ACK,
	MP_RM_ITEM_SET_OPTION_NACK,

	MP_RM_ITEM_DELETE_SYN,
	MP_RM_ITEM_DELETE_ACK,
	MP_RM_ITEM_DELETE_NACK,

	MP_RM_ITEM_DELETE_OPTION_SYN,
	MP_RM_ITEM_DELETE_OPTION_ACK,
	MP_RM_ITEM_DELETE_OPTION_NACK,

	MP_RM_ITEM_GET_STORAGE_ACK,

	MP_RM_ITEM_SET_STORAGE_SYN,
	MP_RM_ITEM_SET_STORAGE_ACK,

	MP_RM_ITEM_GET_EXTENDED_INVENTORY_SYN,

	MP_RM_ITEM_ADD_SYN,
	MP_RM_ITEM_ADD_ACK,
	MP_RM_ITEM_ADD_NACK,

	MP_RM_ITEM_UPDATE_END_TIME_SYN,
	MP_RM_ITEM_UPDATE_END_TIME_ACK,
	MP_RM_ITEM_UPDATE_END_TIME_NACK,

	MP_RM_ITEM_UPDATE_REMAIN_TIME_SYN,
	MP_RM_ITEM_UPDATE_REMAIN_TIME_ACK,
	MP_RM_ITEM_UPDATE_REMAIN_TIME_NACK,
};


enum
{
	MP_RM_ITEM_FIND_OWNER_SYN,
	MP_RM_ITEM_FIND_OWNER_ACK,
	MP_RM_ITEM_FIND_OWNER_NACK,
	MP_RM_ITEM_FIND_OWNER_NACK_BY_AUTH,
};


enum
{
	MP_RM_ITEM_LOG_NACK_BY_AUTH,

	//MP_RM_ITEM_LOG_GET_SYN,
	MP_RM_ITEM_LOG_GET_ACK,
	//MP_RM_ITEM_LOG_GET_NACK,

	MP_RM_ITEM_LOG_GET_SIZE_SYN,
	MP_RM_ITEM_LOG_GET_SIZE_ACK,
	MP_RM_ITEM_LOG_GET_SIZE_NACK,

	MP_RM_ITEM_LOG_GET_STOP_SYN,
	MP_RM_ITEM_LOG_GET_STOP_ACK,
	//MP_RM_ITEM_LOG_GET_STOP_NACK,
};


enum
{
	MP_RM_ITEM_OPTION_LOG_NACK_BY_AUTH,

	//MP_RM_ITEM_OPTION_LOG_GET_SYN,
	MP_RM_ITEM_OPTION_LOG_GET_ACK,
	//MP_RM_ITEM_OPTION_LOG_GET_NACK,

	MP_RM_ITEM_OPTION_LOG_SIZE_SYN,
	MP_RM_ITEM_OPTION_LOG_SIZE_ACK,
	MP_RM_ITEM_OPTION_LOG_SIZE_NACK,

	MP_RM_ITEM_OPTION_LOG_STOP_SYN,
	MP_RM_ITEM_OPTION_LOG_STOP_ACK,
	//MP_RM_ITEM_OPTION_LOG_STOP_NACK,
};


enum
{
	MP_RM_SKILL_NACK_BY_AUTH,

	MP_RM_SKILL_GET_ALL_SYN,
	MP_RM_SKILL_GET_ALL_ACK,
	MP_RM_SKILL_GET_ALL_NACK,

	MP_RM_SKILL_SET_LEVEL_SYN,
	MP_RM_SKILL_SET_LEVEL_ACK,
	MP_RM_SKILL_SET_LEVEL_NACK,

	MP_RM_SKILL_ADD_SYN,
	MP_RM_SKILL_ADD_ACK,
	MP_RM_SKILL_ADD_NACK,

	MP_RM_SKILL_REMOVE_SYN,
	MP_RM_SKILL_REMOVE_ACK,
	MP_RM_SKILL_REMOVE_NACK,
};


enum
{
	MP_RM_SKILL_LOG_NACK_BY_AUTH,

	//MP_RM_SKILL_LOG_GET_SYN,
	MP_RM_SKILL_LOG_GET_ACK,
	//MP_RM_SKILL_LOG_GET_NACK,

	MP_RM_SKILL_LOG_SIZE_SYN,
	MP_RM_SKILL_LOG_SIZE_ACK,
	MP_RM_SKILL_LOG_SIZE_NACK,

	MP_RM_SKILL_LOG_STOP_SYN,
	MP_RM_SKILL_LOG_STOP_ACK,
	//MP_RM_SKILL_LOG_STOP_NACK,
};


enum
{
	MP_RM_FAMILY_NACK_BY_AUTH,

	MP_RM_FAMILY_GET_LIST_SYN,
	MP_RM_FAMILY_GET_LIST_ACK,
	MP_RM_FAMILY_GET_LIST_NACK,

	MP_RM_FAMILY_GET_DATA_SYN,
	MP_RM_FAMILY_GET_DATA_ACK,
	MP_RM_FAMILY_GET_DATA_NACK,

	MP_RM_FAMILY_SET_DATA_SYN,
	MP_RM_FAMILY_SET_DATA_ACK,
	MP_RM_FAMILY_SET_DATA_NACK,

	MP_RM_FAMILY_GET_MEMBER_SYN,
	MP_RM_FAMILY_GET_MEMBER_ACK,
	MP_RM_FAMILY_GET_MEMBER_NACK,

	MP_RM_FAMILY_GET_FARM_SYN,
	MP_RM_FAMILY_GET_FARM_ACK,
	MP_RM_FAMILY_GET_FARM_NACK,

	MP_RM_FAMILY_GET_CROP_SYN,
	MP_RM_FAMILY_GET_CROP_ACK,
	MP_RM_FAMILY_GET_CROP_NACK,
};


enum
{
	MP_RM_QUEST_NACK_BY_AUTH,

	MP_RM_QUEST_GET_MAIN_DATA_SYN,
	MP_RM_QUEST_GET_MAIN_DATA_ACK,
	MP_RM_QUEST_GET_MAIN_DATA_NACK,

	MP_RM_QUEST_GET_SUB_DATA_SYN,
	MP_RM_QUEST_GET_SUB_DATA_ACK,
	MP_RM_QUEST_GET_SUB_DATA_NACK,

	MP_RM_QUEST_GET_ITEM_DATA_SYN,
	MP_RM_QUEST_GET_ITEM_DATA_ACK,
	MP_RM_QUEST_GET_ITEM_DATA_NACK,

	MP_RM_QUEST_REMOVE_SYN,
	MP_RM_QUEST_REMOVE_ACK,
	MP_RM_QUEST_REMOVE_NACK,

	MP_RM_QUEST_ITEM_ADD_SYN,
	MP_RM_QUEST_ITEM_ADD_ACK,
	MP_RM_QUEST_ITEM_ADD_NACK,

	MP_RM_QUEST_ITEM_UPDATE_SYN,
	MP_RM_QUEST_ITEM_UPDATE_ACK,
	MP_RM_QUEST_ITEM_UPDATE_NACK,

	MP_RM_QUEST_ITEM_DELETE_SYN,
	MP_RM_QUEST_ITEM_DELETE_ACK,
	MP_RM_QUEST_ITEM_DELETE_NACK,

	MP_RM_QUEST_FINISH_SUB_SYN,
	MP_RM_QUEST_FINISH_SUB_ACK,
	MP_RM_QUEST_FINISH_SUB_NACK,
};


enum
{
	MP_RM_QUEST_LOG_NACK_BY_AUTH,

	MP_RM_QUEST_LOG_SIZE_SYN,
	MP_RM_QUEST_LOG_SIZE_ACK,
	MP_RM_QUEST_LOG_SIZE_NACK,

	MP_RM_QUEST_LOG_STOP_SYN,
	MP_RM_QUEST_LOG_STOP_ACK,
	//MP_RM_QUEST_LOG_STOP_NACK,

	//MP_RM_QUEST_LOG_GET_SYN,
	MP_RM_QUEST_LOG_GET_ACK,
	//MP_RM_QUEST_LOG_GET_NACK,
};


enum
{
	MP_RM_GUILD_NACK_BY_AUTH,

	MP_RM_GUILD_GET_LIST_SYN,
	MP_RM_GUILD_GET_LIST_ACK,
	MP_RM_GUILD_GET_LIST_NACK,

	MP_RM_GUILD_GET_DATA_SYN,
	MP_RM_GUILD_GET_DATA_ACK,
	MP_RM_GUILD_GET_DATA_NACK,

	//MP_RM_GUILD_GET_STORE_SYN,
	MP_RM_GUILD_GET_STORE_ACK,
	//MP_RM_GUILD_GET_STORE_NACK,

	//MP_RM_GUILD_GET_MEMBER_SYN,
	MP_RM_GUILD_GET_MEMBER_ACK,
	//MP_RM_GUILD_GET_MEMBER_NACK,

	MP_RM_GUILD_GET_FRIENDLY_ACK,
	MP_RM_GUILD_GET_ENEMY_ACK,

	MP_RM_GUILD_SET_DATA_SYN,
	MP_RM_GUILD_SET_DATA_ACK,
	MP_RM_GUILD_SET_DATA_NACK,

	MP_RM_GUILD_SET_RANK_SYN,
	MP_RM_GUILD_SET_RANK_ACK,
	MP_RM_GUILD_SET_RANK_NACK,

	MP_RM_GUILD_KICK_SYN,
	MP_RM_GUILD_KICK_ACK,
	MP_RM_GUILD_KICK_NACK,
	MP_RM_GUILD_KICK_NACK_BY_NO_GUILD,
	MP_RM_GUILD_KICK_NACK_BY_ONE_MEMBER,
};


enum
{
	MP_RM_GUILD_SCORE_LOG_NACK_BY_AUTH,

	//MP_RM_GUILD_SCORE_LOG_GET_SYN,
	MP_RM_GUILD_SCORE_LOG_GET_ACK,
	//MP_RM_GUILD_SCORE_LOG_GET_NACK,

	MP_RM_GUILD_SCORE_LOG_SIZE_SYN,
	MP_RM_GUILD_SCORE_LOG_SIZE_ACK,
	MP_RM_GUILD_SCORE_LOG_SIZE_NACK,

	MP_RM_GUILD_SCORE_LOG_STOP_SYN,
	MP_RM_GUILD_SCORE_LOG_STOP_ACK,
	//MP_RM_GUILD_SCORE_LOG_STOP_NACK,
};


enum
{
	MP_RM_GUILD_LOG_NACK_BY_AUTH,

	//MP_RM_GUILD_LOG_GET_SYN,
	MP_RM_GUILD_LOG_GET_ACK,
	//MP_RM_GUILD_LOG_GET_NACK,

	MP_RM_GUILD_LOG_SIZE_SYN,
	MP_RM_GUILD_LOG_SIZE_ACK,
	MP_RM_GUILD_LOG_SIZE_NACK,

	MP_RM_GUILD_LOG_STOP_SYN,
	MP_RM_GUILD_LOG_STOP_ACK,
	//MP_RM_GUILD_LOG_STOP_NACK,
};


enum
{
	MP_RM_GUILD_WAREHOUSE_LOG_NACK_BY_AUTH,

	//MP_RM_GUILD_WAREHOUSE_LOG_GET_SYN,
	MP_RM_GUILD_WAREHOUSE_LOG_GET_ACK,
	//MP_RM_GUILD_WAREHOUSE_LOG_GET_NACK,

	MP_RM_GUILD_WAREHOUSE_LOG_SIZE_SYN,
	MP_RM_GUILD_WAREHOUSE_LOG_SIZE_ACK,
	MP_RM_GUILD_WAREHOUSE_LOG_SIZE_NACK,

	MP_RM_GUILD_WAREHOUSE_LOG_STOP_SYN,
	MP_RM_GUILD_WAREHOUSE_LOG_STOP_ACK,
	//MP_RM_GUILD_WAREHOUSE_LOG_STOP_NACK,
};


enum
{
	MP_RM_EXP_LOG_NACK_BY_AUTH,

	//MP_RM_EXT_GET_SYN,
	MP_RM_EXP_LOG_GET_ACK,
	MP_RM_EXP_LOG_GET_NACK,

	MP_RM_EXP_LOG_GET_STOP_SYN,
	MP_RM_EXP_LOG_GET_STOP_ACK,
	MP_RM_EXP_LOG_GET_STOP_NACK,

	MP_RM_EXP_LOG_GET_SIZE_SYN,
	MP_RM_EXP_LOG_GET_SIZE_ACK,
	MP_RM_EXP_LOG_GET_SIZE_NACK,
};


enum
{
	MP_RM_STAT_LOG_NACK_BY_AUTH,

	//MP_RM_STAT_LOG_GET_SYN,
	MP_RM_STAT_LOG_GET_ACK,
	//MP_RM_STAT_LOG_GET_NACK,

	MP_RM_STAT_LOG_SIZE_SYN,
	MP_RM_STAT_LOG_SIZE_ACK,
	MP_RM_STAT_LOG_SIZE_NACK,

	MP_RM_STAT_LOG_STOP_SYN,
	MP_RM_STAT_LOG_STOP_ACK,
	//MP_RM_STAT_LOG_STOP_NACK,
};


enum
{
	MP_RM_USER_NACK_BY_AUTH,

	//MP_RM_USER_GET_DATA_BY_PLAYER_INDEX_SYN,
	MP_RM_USER_GET_DATA_SYN,
	MP_RM_USER_GET_DATA_ACK,
	MP_RM_USER_GET_DATA_NACK,

	MP_RM_USER_GET_PLAYER_LIST_ACK,

	MP_RM_USER_SET_DATA_SYN,
	MP_RM_USER_SET_DATA_ACK,
	MP_RM_USER_SET_DATA_NACK,

	MP_RM_USER_CREATE_SYN,
	MP_RM_USER_CREATE_ACK,
	MP_RM_USER_CREATE_NACK,
	MP_RM_USER_CREATE_NACK_BY_DUPLICATED_NAME,
	MP_RM_USER_CREATE_NACK_BY_GUILD,
	MP_RM_USER_CREATE_NACK_BY_FULL,

	MP_RM_USER_RESTORE_SYN,
	MP_RM_USER_RESTORE_ACK,
	MP_RM_USER_RESTORE_NACK,
	MP_RM_USER_RESTORE_NACK_BY_DUPLICATED_NAME,
	MP_RM_USER_RESTORE_NACK_BY_GUILD,
	MP_RM_USER_RESTORE_NACK_BY_FAMILY,
	MP_RM_USER_RESTORE_NACK_BY_FULL,
	MP_RM_USER_RESTORE_NACK_BY_NOT_EXIST,

	MP_RM_USER_DELETE_SYN,
	MP_RM_USER_DELETE_ACK,
	MP_RM_USER_DELETE_NACK,
	MP_RM_USER_DELETE_NACK_BY_FAMILY,
	MP_RM_USER_DELETE_NACK_BY_PARTY,
	MP_RM_USER_DELETE_NACK_BY_GUILD,
};

enum
{
	MP_RM_JOB_LOG_NACK_BY_AUTH,

	//MP_RM_JOB_LOG_GET_SYN,
	MP_RM_JOB_LOG_GET_ACK,
	//MP_RM_JOB_LOG_GET_NACK,

	MP_RM_JOB_LOG_SIZE_SYN,
	MP_RM_JOB_LOG_SIZE_ACK,
	MP_RM_JOB_LOG_SIZE_NACK,

	MP_RM_JOB_LOG_STOP_SYN,
	MP_RM_JOB_LOG_STOP_ACK,
	//MP_RM_JOB_LOG_STOP_NACK,
};


//enum
//{
//	MP_RM_PLAYER_ITEM_LOG_NACK_BY_AUTH,
//
//	//MP_RM_PLAYER_ITEM_LOG_GET_SYN,
//	MP_RM_PLAYER_ITEM_LOG_GET_ACK,
//	//MP_RM_PLAYER_ITEM_LOG_GET_NACK,
//
//	MP_RM_PLAYER_ITEM_LOG_SIZE_SYN,
//	MP_RM_PLAYER_ITEM_LOG_SIZE_ACK,
//	MP_RM_PLAYER_ITEM_LOG_SIZE_NACK,
//
//	MP_RM_PLAYER_ITEM_LOG_STOP_SYN,
//	MP_RM_PLAYER_ITEM_LOG_STOP_ACK,
//	//MP_RM_PLAYER_ITEM_LOG_STOP_NACK,
//};

	
enum
{
	MP_RM_FAMILY_CROP_LOG_NACK_BY_AUTH,

	//MP_RM_FAMILY_CROP_LOG_GET_SYN,
	MP_RM_FAMILY_CROP_LOG_GET_ACK,
	//MP_RM_FAMILY_CROP_LOG_GET_NACK,

	MP_RM_FAMILY_CROP_LOG_SIZE_SYN,
	MP_RM_FAMILY_CROP_LOG_SIZE_ACK,
	MP_RM_FAMILY_CROP_LOG_SIZE_NACK,

	MP_RM_FAMILY_CROP_LOG_STOP_SYN,
	MP_RM_FAMILY_CROP_LOG_STOP_ACK,
	//MP_RM_FAMILY_CROP_LOG_STOP_NACK,
};


enum
{
	MP_RM_FAMILY_POINT_LOG_NACK_BY_AUTH,

	//MP_RM_FAMILY_POINT_LOG_GET_SYN,
	MP_RM_FAMILY_POINT_LOG_GET_ACK,
	//MP_RM_FAMILY_POINT_LOG_GET_NACK,

	MP_RM_FAMILY_POINT_LOG_SIZE_SYN,
	MP_RM_FAMILY_POINT_LOG_SIZE_ACK,
	MP_RM_FAMILY_POINT_LOG_SIZE_NACK,

	MP_RM_FAMILY_POINT_LOG_STOP_SYN,
	MP_RM_FAMILY_POINT_LOG_STOP_ACK,
	//MP_RM_FAMILY_POINT_LOG_STOP_NACK,
};


enum
{
	MP_RM_PERMISSION_NACK_BY_AUTH,

	MP_RM_PERMISSION_ADD_SYN,
	MP_RM_PERMISSION_ADD_ACK,
	MP_RM_PERMISSION_ADD_NACK,

	MP_RM_PERMISSION_REMOVE_SYN,
	MP_RM_PERMISSION_REMOVE_ACK,
	MP_RM_PERMISSION_REMOVE_NACK,
};


enum
{
	MP_RM_OPERATOR_NACK_BY_AUTH,

	MP_RM_OPERATOR_GET_SYN,
	MP_RM_OPERATOR_GET_ACK,
	
	MP_RM_OPERATOR_ADD_SYN,
	MP_RM_OPERATOR_ADD_ACK,
	MP_RM_OPERATOR_ADD_NACK_BY_ID,

	MP_RM_OPERATOR_REMOVE_SYN,
	MP_RM_OPERATOR_REMOVE_ACK,
	MP_RM_OPERATOR_REMOVE_NACK,

	MP_RM_OPERATOR_SET_SYN,
	MP_RM_OPERATOR_SET_ACK,
	MP_RM_OPERATOR_SET_NACK,

	MP_RM_OPERATOR_IP_GET_ACK,

	MP_RM_OPERATOR_IP_ADD_SYN,
	MP_RM_OPERATOR_IP_ADD_ACK,
	MP_RM_OPERATOR_IP_ADD_NACK,

	MP_RM_OPERATOR_IP_REMOVE_SYN,
	MP_RM_OPERATOR_IP_REMOVE_ACK,
	MP_RM_OPERATOR_IP_REMOVE_NACK,
};


enum
{
	MP_RM_OPERATOR_LOG_NACK_BY_AUTH,

	//MP_RM_OPERATOR_LOG_SYN,
	MP_RM_OPERATOR_LOG_ACK,
	//MP_RM_OPERATOR_LOG_NACK,

	MP_RM_OPERATOR_LOG_SIZE_SYN,
	MP_RM_OPERATOR_LOG_SIZE_ACK,
	MP_RM_OPERATOR_LOG_SIZE_NACK,
};


enum
{
	MP_RM_PASSWORD_SET_SYN,
	MP_RM_PASSWORD_SET_ACK,
	MP_RM_PASSWORD_SET_NACK,
};


enum
{
	MP_RM_NAME_LOG_GET_SYN,
	MP_RM_NAME_LOG_GET_ACK,
	MP_RM_NAME_LOG_GET_NACK,
};


enum RecoveryLog
{
	// 플레이어
	RecoveryLogPlayer				= 100,
	RecoveryLogSetPlayer,
	RecoveryLogSetExtendedPlayer,
	RecoveryLogSetLicense,

	// 아이템
	RecoveryLogItem					= 200,
	RecoveryLogSetItem,
	RecoveryLogSetItemOption,
	RecoveryLogRemoveItem,
	RecoveryLogRemoveItemOption,
	RecoveryLogAddItem,
	RecoveryLogUpdateItemEndTime,
	RecoveryLogUpdateItemRemainTime,
	RecoveryLogSetItemStorage,

	// 스킬
	RecoveryLogSkill				= 300,
	RecoveryLogAddSkill,
	RecoveryLogRemoveSkill,
	RecoveryLogSetSkill,

	// 패밀리
	RecoveryLogFamily				= 400,
	RecoveryLogSetFamily,

	// 퀘스트
	RecoveryLogQuest				= 500,
	RecoveryLogRemoveQuest,
	RecoveryLogFinishSubQuest,
	RecoveryLogSetQuestItem,
	RecoveryLogRemoveQuestItem,
	RecoveryLogAddQuestItem,

	// 길드
	RecoveryLogGuild				= 600,
	RecoveryLogSetGuild,
	RecoveryLogSetGuildRank,
	RecoveryLogKickGuildMember,

	// 사용자
	RecoveryLogUser					= 700,
	RecoveryLogSetUser,
	RecoveryLogAddPlayerToUser,
	RecoveryLogRemovePlayerFromUser,
	RecoveryLogRestorePlayerToUser,

	// 차단한 IP대역내에서 특정 IP 허용
	RecoveryPermission				= 800,
	RecoveryPermissionAddIp,
	RecoveryPermissionRemoveIp,

	// 운영
	RecoveryOperator				= 900,
	RecoveryOperatorAdd,
	RecoveryOperatorRemove,
	RecoveryOperatorSet,
	RecoveryOperatorAddIp,
	RecoveryOperatorRemoveIp,
};


// 키워드로 유저 목록을 요청한다.
struct MSG_RM_OPEN_SYN : public MSGBASE
{
	enum Type
	{
		LoginId,		// 로그인 ID로 검색
		RealName,		// 이름으로 검색
		PlayerIndex,	// 플레이어 DB 번호로 검색
		PlayerName,		// 캐릭터 명으로 검색
	}
	mType;

	TCHAR mKeyword[ MAX_NAME_LENGTH + 1 ];
};


// 검색된 유저 결과를 반환한다
struct MSG_RM_OPEN_ACK : public MSGBASE
{
	DWORD	mSize;
	BOOL	mIsMore;	// 검색결과가 용량보다 많을 경우 세팅됨

	struct Data
	{
		DWORD	mUserIndex;
		TCHAR	mUserName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mPlayerIndex;
		TCHAR	mPlayerName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mStandIndex;
	}
	mData[ MAX_ROW_NUM ];

	int GetSize() const
	{
		return sizeof( MSG_RM_OPEN_ACK ) - ( sizeof( mData ) / sizeof( *mData ) - mSize ) * sizeof( *mData );
	}
};


// 플레이어의 상세 정보를 요청한다
struct MSG_RM_GET_PLAYER_SYN : public MSGBASE
{
	DWORD	mPlayerIndex;
	TCHAR	mPlayerName[ MAX_NAME_LENGTH + 1 ];
};


// 해당 플레이어의 상세 정보를 반환한다
// 이 구조체에는 숫자 정보만 담는다. 문자열 등의 정보는 쿼리 타입이 다르므로
// MSG_RM_GET_PLAYER_DATA_EXTENDED를 사용한다.
struct MSG_RM_PLAYER_DATA : public MSGBASE
{
	struct Player
	{
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mIndex;
		BOOL	mGender;
		DWORD	mStrength;
		DWORD	mDexterity;
		DWORD	mVitality;
		DWORD	mIntelligence;
		DWORD	mWisdom;
		DWORD	mLife;
		DWORD	mMana;
		DWORD	mGrade;
		DWORD	mMaxGrade;
		DWORD	mExp;
		DWORD		mGradeUpPoint;
		MONEYTYPE	mMoney;
		DWORD	mPlayTime;

		DWORD	mSkillPoint;
		DWORD	mRace;
		DWORD	mExtendedInventorySize;

	}
	mPlayer;

	struct Position
	{
		MAPTYPE	mMap;
		DWORD	mX;
		DWORD	mY;
	}
	mPosition;

	struct Job
	{
		DWORD	mIndex;
		DWORD	mData1;
		DWORD	mData2;
		DWORD	mData3;
		DWORD	mData4;
		DWORD	mData5;
		DWORD	mData6;
	}
	mJob;

	struct Guild
	{
		DWORD	mIndex;		
		DWORD	mRank;
		TCHAR	mName[ MAX_GUILD_NAME + 1 ];
	}
	mGuild;

	//DWORD mFamilyIndex;
};


//typedef MSG_RM_GET_PLAYER_DATA MSG_RM_SET_PLAYER_DATA;


// 플레이어의 확장 정보를 반환한다. MIDDLE QUERY를 이용하므로 필드가 20개를 넘으면 안된다
struct MSG_RM_PLAYER_EXTENDED_DATA : public MSGBASE
{
	struct Player
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mGuildNick[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mFamilyNick[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mLastPlayedTime[ MAX_CHAT_LENGTH + 1 ];
	}
	mPlayer;

	struct History
	{
		BOOL	mIsEnable;
		TCHAR	mOriginalName	[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mCreateDate		[ MAX_CHAT_LENGTH + 1 ];
		TCHAR	mCreateIp		[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mDeletedDate	[ MAX_CHAT_LENGTH + 1 ];
		TCHAR	mDeletedIp		[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mRecoveredDate	[ MAX_DATE_SIZE + 1 ];
		TCHAR	mRecoveredIp	[ MAX_NAME_LENGTH + 1 ];
	}
	mHistory;	
};


struct MSG_RM_PLAYER_LOVE : public MSGBASE
{
	DATE_MATCHING_INFO	mDateMatching;
};


// 아이템 정보를 반환한다
struct MSG_RM_ITEM : public MSGBASE
{
	DWORD		mSize;
	Item		mData[ MAX_ROW_NUM  ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_ITEM ) - ( sizeof( mData ) / sizeof( *mData ) - mSize ) * sizeof( *mData );
	}
};


// 옵션 정보를 가져온다
struct MSG_RM_ITEM_OPTION : public MSGBASE
{
	size_t mSize;

	ITEM_OPTION mData[ MAX_ITEM_OPTION_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_ITEM_OPTION ) - ( sizeof( mData ) / sizeof( ITEM_OPTION ) - mSize ) * sizeof( ITEM_OPTION );
	}
};


// 아이템 정보를 갱신한다
struct MSG_RM_ITEM_UPDATE : public MSGBASE
{
	ITEMBASE mItem;
};


// 아이템의 옵션 정보를 갱신한다(추가/삭제/변경)
struct MSG_RM_ITEM_UPDATE_OPTION : public MSGBASE
{
	ITEM_OPTION	mOption;
};


struct MSG_RM_ITEM_OWNER : public MSGBASE
{
	DWORD mItemIndex;
	size_t mSize;

	struct Player
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mItemSize;	// 아이템 가진 개수
	}
	mPlayer[ MAX_ROW_NUM ];	

	size_t GetSize() const
	{
		return sizeof( MSG_RM_ITEM_OWNER ) - ( sizeof( mPlayer ) / sizeof( *mPlayer ) - mSize ) * sizeof( *mPlayer );
	}
};


struct MSG_RM_SKILL : public MSGBASE
{
	DWORD		mSize;
	SKILL_BASE	mData[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_SKILL ) - ( sizeof( mData ) / sizeof( *mData ) - mSize ) * sizeof( *mData );
	}
};


struct MSG_RM_FAMILY_LIST : public MSGBASE
{
	size_t mSize;

	struct Family
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mSize;
		DWORD	mMasterIndex;
	}
	mFamily[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_FAMILY_LIST ) - ( sizeof( mFamily ) / sizeof( *mFamily ) - mSize ) * sizeof( *mFamily );
	}
};


struct MSG_RM_FAMILY_DATA : public MSGBASE
{
	DWORD	mIndex;
	TCHAR	mName[ MAX_FAMILY_NAME + 1 ];
	DWORD	mHonorPoint;
	BOOL	mIsEnableNick;
	TCHAR	mBuildDate[ MAX_DATE_SIZE + 1 ];
};


struct MSG_RM_FAMILY_MEMBER : public MSGBASE
{
	size_t mSize;

	struct Player
	{
		DWORD	mIndex;
		TCHAR	mNick[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mLevel;
		BOOL	mIsMaster;
	}
	mPlayer[ 50 ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_FAMILY_MEMBER ) - ( sizeof( mPlayer ) / sizeof( Player ) - mSize ) * sizeof( Player );
	}
};


struct MSG_RM_FAMILY_FARM : public MSGBASE
{
	DWORD	mZone;
	DWORD	mFarmIndex;
	DWORD	mState;
	DWORD	mOwnerIndex;
	DWORD	mGardenGrade;
	DWORD	mHouseGrade;
	DWORD	mWarehouseGrade;
	DWORD	mCageGrade;
	DWORD	mFenceGrade;
	TCHAR	mBuildDate[ MAX_DATE_SIZE + 1 ];
};


struct MSG_RM_FAMILY_CROP : public MSGBASE
{
	size_t mSize;

	struct Crop
	{
		DWORD	mIndex;
        DWORD	mOwnerIndex;
		TCHAR	mOwnerName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mKind;
		DWORD	mStep;
		DWORD	mLife;
		DWORD	mNextStepTime;
		DWORD	mSeedGrade;
	}
	mCrop[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_FAMILY_CROP ) - ( sizeof( mCrop ) / sizeof( Crop ) - mSize ) * sizeof( Crop );
	}
};


struct MSG_RM_GUILD_LIST : public MSGBASE
{
	size_t mSize;

	struct Guild
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mSize;
		DWORD	mLevel;
		DWORD	mMoney;	// 선포금
	}
	mGuild[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_LIST ) - ( sizeof( mGuild ) / sizeof( Guild ) - mSize ) * sizeof( Guild );
	}
};


struct MSG_RM_GUILD_DATA : public MSGBASE
{	
	DWORD	mGuildIndex;
	DWORD	mLevel;
	DWORD	mScore;
	DWORD	mMoney;
	TCHAR	mGuildName[ MAX_GUILD_NAME + 1 ];
	CHAR	mBuiltDate[ MAX_DATE_SIZE + 1 ];
	TCHAR	mAllianceName[ MAX_GUILD_NAME + 1 ];
	DWORD	mAllianceEntryDate;
};


struct MSG_RM_GUILD_STORE : public MSGBASE
{
	size_t mSize;

	struct Item
	{
		DWORD	mDbIndex;
		DWORD	mIndex;
		DWORD	mQuantity;
	}
	mItem[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_STORE ) - ( sizeof( mItem ) / sizeof( Item ) - mSize ) * sizeof( Item );
	}
};


struct MSG_RM_GUILD_MEMBER : public MSGBASE
{
	size_t mSize;

	struct Player
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mLevel;
		DWORD	mRank;
		TCHAR	mDate[ MAX_DATE_SIZE ];
	}
	mPlayer[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_MEMBER ) - ( sizeof( mPlayer ) / sizeof( Player ) - mSize ) * sizeof( Player );
	}
};


struct MSG_RM_LOG_REQUEST : public MSGBASE
{
	// 클라이언트가 로그를 요청한 시간. 이 수치가 일치하지 않는 로그는 클라이언트에서 무시한다
	DWORD	mRequestTick;
	TCHAR	mBeginDate[ MAX_DATE_SIZE ];
	TCHAR	mEndDate[ MAX_DATE_SIZE ];
};


struct MSG_RM_LOG_SIZE : public MSGROOT
{
	DWORD	mRequestTick;
	size_t	mSize;
};


struct MSG_RM_EXP_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD mLogIndex;
		DWORD mType;
		DWORD mGrade;
		DWORD mVariation;
		DWORD mKillerKind;
		DWORD mKillerIndex;
		DWORD mExperience;
		TCHAR mDate[ MAX_DATE_SIZE ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_EXP_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_ITEM_LOG_REQUEST : public MSG_RM_LOG_REQUEST
{
	DWORD	mItemIndex;
	DWORD	mItemDbIndex;
	DWORD	mPlayerIndex;
};


struct MSG_RM_ITEM_LOG : public MSGBASE
{
	DWORD	mRequestTick;
	size_t	mSize;

	struct Player
	{
		DWORD	mIndex;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
		DWORD	mMoney;
	};

	struct Item
	{
		DWORD	mIndex;
		DWORD	mDbIndex;
		DWORD	mQuantity;
		DWORD	mFromPosition;
		DWORD	mToPosition;
	};

	struct Log
	{
		DWORD	mLogIndex;

		eLogitemmoney	mType;
		
		Player	mFromPlayer;
		Player	mToPlayer;
		
		DWORD	mMoneyVariation;

		Item	mItem;

		TCHAR	mDate[ MAX_DATE_SIZE + 1 ];
		DWORD	mExperience;
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_ITEM_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_STAT_LOG_REQUEST : public MSG_RM_LOG_REQUEST
{
	DWORD	mPlayerIndex;
};


struct MSG_RM_STAT_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD	mLogIndex;
		DWORD	mLevel;
		DWORD	mStrength;
		DWORD	mDexterity;
		DWORD	mVitality;
		DWORD	mIntelligence;
		DWORD	mWisdom;
		DWORD	mExperience;
		TCHAR	mDate[ MAX_DATE_SIZE + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_STAT_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_SKILL_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD		mLogIndex;
		eLogSkill	mType;
		DWORD		mDbIndex;
		DWORD		mIndex;
		DWORD		mLevel;
		DWORD		mPoint;
		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR		mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_SKILL_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_ITEM_OPTION_LOG_REQUEST : public MSG_RM_LOG_REQUEST
{
	DWORD	mItemDbIndex;
};


struct MSG_RM_ITEM_OPTION_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD			mLogIndex;
		DWORD			mItemDbIndex;
		eLogitemmoney	mType;

		ITEM_OPTION	mOption;

		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR		mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_ITEM_OPTION_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_GUILD_LOG_REQUEST : public MSG_RM_LOG_REQUEST
{
	DWORD	mGuildIndex;
};


struct MSG_RM_GUILD_LOG : public MSGBASE
{
	DWORD	mSize;

	struct Log
	{
		DWORD		mLogIndex;
		eGuildLog	mType;
		DWORD		mPlayerIndex;
		DWORD		mValue1;
		DWORD		mValue2;
		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_GUILD_WAREHOUSE_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD			mLogIndex;
		DWORD			mPlayerIndex;
		POSTYPE			mItemFromPosition;
		ITEMBASE		mItem;
		MONEYTYPE		mMoney;
		eLogitemmoney	mType;
		TCHAR			mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR			mMemo[ MAX_CHAT_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_WAREHOUSE_LOG ) - ( sizeof( mLog ) / sizeof( Log ) - mSize ) * sizeof( Log );
	}
};


struct MSG_RM_USER_DATA : public MSGBASE
{
	DWORD	mIndex;
	TCHAR	mId[ MAX_NAME_LENGTH + 1 ];
	TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
	TCHAR	mJoinedDate[ MAX_NAME_LENGTH + 1 ];
	TCHAR	mLogedoutDate[ MAX_NAME_LENGTH + 1 ];
	DWORD	mPlayTime;
	DWORD	mLevel;
	
	struct Connection
	{
		TCHAR	mIp[ MAX_NAME_LENGTH + 1 ];
		DWORD	mAgent;
	}
	mConnection;
};


struct MSG_RM_USER_PLAYER_LIST : public MSGBASE
{
	size_t mSize;

	struct Player
	{
		DWORD	mIndex;
		DWORD	mLevel;
		TCHAR	mName[ MAX_NAME_LENGTH + 1 ];
	}
	mPlayer[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_USER_PLAYER_LIST ) - ( sizeof( mPlayer ) / sizeof( mPlayer[ 0 ] ) - mSize ) * sizeof( mPlayer[ 0 ] );
	}
};


struct MSG_RM_JOB_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD	mLogIndex;
		DWORD	mClassIndex;
		DWORD	mJobGrade;
		DWORD	mJobIndex;
		TCHAR	mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR	mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_JOB_LOG ) - ( sizeof( mLog ) / sizeof( mLog[ 0 ] ) - mSize ) * sizeof( mLog[ 0 ] );
	}
};


struct MSG_RM_GUILD_SCORE_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD	mLogIndex;
		eGuildLog	mType;
		DWORD	mScore;
		//DWORD	mLoginMember;
		//DWORD	mMonsterHunt;
		TCHAR	mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR	mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_GUILD_SCORE_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


struct MSG_RM_FAMILY_POINT_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD		mLogIndex;
		eFamilyLog	mType;
		DWORD		mPoint;
		DWORD		mMonsterExp;
		DWORD		mLoginMember;
		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR		mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_FAMILY_POINT_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


struct MSG_RM_FAMILY_CROP_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD		mLogIndex;
		eFamilyLog	mType;
		DWORD		mFarmIndex;
		DWORD		mOwnerIndex;
		DWORD		mCropIndex;
		DWORD		mCropKind;
		DWORD		mCropStep;
		DWORD		mCropLife;
		DWORD		mCropSeedGrade;
		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR		mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_FAMILY_CROP_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


struct MSG_RM_QUEST_LOG : public MSGBASE
{
	size_t mSize;

	struct Log
	{
		DWORD		mLogIndex;
		eQuestLog	mType;
		DWORD		mMainQuestIndex;
		DWORD		mSubQuestIndex;
		int			mValue1;		// 몬스터 혹은 아이템 번호
		int			mValue2;		// 몬스터 사냥 수 혹은 아이템 개수
		TCHAR		mDate[ MAX_DATE_SIZE + 1 ];
		TCHAR		mMemo[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_QUEST_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


struct MSG_RM_OPERATOR : public MSGBASE
{
	size_t mSize;

	struct Operator
	{
		DWORD		mIndex;
		TCHAR		mId				[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mName			[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mIp				[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mRegistedDate	[ MAX_NAME_LENGTH + 1 ];
		eGM_POWER	mPower;
		//enum Rank
		//{
		//	None,
		//	Monitor,
		//	Staff,
		//	Manager,
		//}
		//mRank;
	}
	mOperator[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_OPERATOR ) - ( sizeof( mOperator ) / sizeof( *mOperator ) - mSize ) * sizeof( *mOperator );
	}
};


struct MSG_RM_OPERATOR_IP : public MSGBASE
{
	size_t mSize;

	struct Address
	{
		TCHAR mIp[ MAX_NAME_LENGTH + 1 ];
	}
	mAddress[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_OPERATOR_IP ) - ( sizeof( mAddress ) / sizeof( *mAddress ) - mSize ) * sizeof( *mAddress );
	}
};


struct MSG_RM_OPERATOR_LOG : public MSGROOT
{
	DWORD	mRequestTick;
	size_t	mSize;
	
	struct Log
	{
		int			mIndex;
		RecoveryLog	mType;
		TCHAR		mDate		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mOperator	[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mKey1		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mValue1		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mKey2		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mValue2		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mKey3		[ MAX_NAME_LENGTH + 1 ];
		TCHAR		mValue3		[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_OPERATOR_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


struct MSG_RM_NAME_LOG : public MSGROOT
{
	DWORD mRequestTick;
	DWORD mSize;
	
	struct Log
	{
		DWORD	mIndex;
		TCHAR	mPreviousName	[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mName			[ MAX_NAME_LENGTH + 1 ];
		TCHAR	mDate			[ MAX_NAME_LENGTH + 1 ];
	}
	mLog[ MAX_ROW_NUM ];

	size_t GetSize() const
	{
		return sizeof( MSG_RM_NAME_LOG ) - ( sizeof( mLog ) / sizeof( *mLog ) - mSize ) * sizeof( *mLog );
	}
};


// 080303 LUJ, 아이템 추가 메시지 형식
struct MSG_RM_ITEM_ADD : public MSGROOT
{
	DWORD	mUserIndex;
	DWORD	mPlayerIndex;
	Item	mItem;
};


// 080311 LUJ, 로그인 메시지
struct MSG_RM_LOGIN : public MSGROOT
{
	TCHAR	mId		[ MAX_NAME_LENGTH + 1 ];
	TCHAR	mPass	[ MAX_NAME_LENGTH + 1 ];
	
	struct Version
	{
		int mMajor;
		int mMinor;
	}
	mVersion;
};


#pragma pack(pop)