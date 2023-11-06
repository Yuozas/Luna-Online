#pragma once


enum //DBMESSAGEIDFUNC
{
	//eGetUserList,
	eGetUserPlayerList,

	eGetPlayerList,
	//eGetPlayerIndex,
	eGetPlayerData,
	//eSetPlayerData,
	eGetExperienceLog,
	eGetStatLog,
	eGetJobLog,

	//eGetItemOptionList,
	eGetItemList,
	eFindItemOwner,
	eGetItemLog,
	eGetItemOptionLog,

	eGetFamilyList,
	eGetFamilyMember,
	eGetFarmCropLog,
	eGetFamilyPointLog,

	eGetQuestMainData,
	eGetQuestSubData,
	eGetQuestItemList,
	eGetQuestLog,

	eGetGuildList,
	eGetGuildStore,
	eGetGuildMember,
	eGetGuildFriendly,
	eGetGuildEnemy,
	eGetGuildLog,
	eGetGuildWarehouseLog,
	eGetGuildScoreLog,

	eGetSkillList,
	eGetSkillLog,

	eGetOperator,
	eGetOperatorIp,
	eGetOperatorLog,

	eGetNameLog,

	MaxQuery,
};


// 로그인
//void LoginOperator( DWORD dwConnectIdx, const char* strId, const char* strPwd, const char* strIp );
void RLoginOperator( LPMIDDLEQUERY, LPDBMESSAGE );

// 로그인 ID를 키워드로 유저 인덱스를 얻어낸다
//void GetUserList( DWORD connectionIndex, bool isId, bool isName, const char* keyword );
void RGetUserList( LPMIDDLEQUERY, LPDBMESSAGE );

// 접속자 인덱스를 가져온다
//void GetUserIndex( DWORD connectionIndex, DWORD playerIndex );
void RGetUserIndex( LPMIDDLEQUERY, LPDBMESSAGE );

// 접속자 정보를 가져온다
//void GetUserData( DWORD connectionIndex, DWORD userIndex, const char* name );
void RGetUserData( LPMIDDLEQUERY, LPDBMESSAGE );

// 접속자의 플레이어 목록을 가져온다
void RGetUserPlayerList( LPQUERY, LPDBMESSAGE );

// 접속자 정보를 변경한다
void SetUserData( DWORD connectionIndex, DWORD userIndex, LEVELTYPE );
void RSetUserData( LPMIDDLEQUERY, LPDBMESSAGE );

// 사용자를 추가한다
void UserAdd( DWORD connectionIndex, const CHARACTERMAKEINFO& );
void RUserAdd( LPMIDDLEQUERY, LPDBMESSAGE );

// 사용자가 보유한 특정 플레이어를 복구한다
void RestoreUser( DWORD connectionIndex, DWORD playerIndex, const char* playerName );
void RRestoreUser( LPMIDDLEQUERY, LPDBMESSAGE );

// 사용자가 보유한 특정 플레이어를 삭제한다
void DeleteUser( DWORD connectionIndex, DWORD playerIndex );
void RDeleteUser( LPMIDDLEQUERY, LPDBMESSAGE );

// 키워드로 플레이어 인덱스와 이름을 얻어낸다
void GetPlayerList( DWORD connectionIndex, DWORD userIndex, const char* userName, const char* keyword, DWORD beginPlayerIndex );
void RGetPlayerList( LPQUERY, LPDBMESSAGE );

// 플레이어 DB 번호로 검색한다
//void GetPlayerIndex( DWORD connectionIndex, DWORD playerDbIndex );
//void RGetPlayerIndex( LPQUERY, LPDBMESSAGE );

// 플레이어의 접속 ID를 가져온다
void GetPlayerLoginId( DWORD connectionIndex, DWORD userIndex, DWORD playerIndex );
void RGetPlayerLoginId( LPMIDDLEQUERY, LPDBMESSAGE );

// 플레이어 상세 정보를 얻어낸다
//void GetPlayerData( DWORD connectionIndex, DWORD playerIndex );
void RGetPlayerData( LPQUERY, LPDBMESSAGE );

// 문자열 등 큰 필드값만 여기서 얻어낸다
void RGetPlayerExtendedData( LPMIDDLEQUERY, LPDBMESSAGE );

// 캐릭터 정보를 갱신한다
//void SetPlayerData( DWORD connectionIndex, const MSG_RM_PLAYER_DATA& );
void RSetPlayerData( LPMIDDLEQUERY, LPDBMESSAGE );

// 캐릭터 확장 정보를 갱신한다
//void SetPlayerExtendedData( DWORD connectionIndex, const MSG_RM_PLAYER_EXTENDED_DATA& );
void RSetPlayerExtendedData( LPMIDDLEQUERY, LPDBMESSAGE );

// 주민증 정보를 읽는다
void RGetPlayerLicense( LPMIDDLEQUERY, LPDBMESSAGE );

// 주민증 정보를 갱신한다
//void PlayerSetLicense( DWORD connectionIndex, DWORD playerIndex, const DATE_MATCHING_INFO& );
void RPlayerSetLicense( LPMIDDLEQUERY, LPDBMESSAGE );

// 아이템 정보를 읽는다
void GetItemListOnInventory( DWORD connectionIndex, DWORD playerIndex, DWORD beginItemDbIndex );
void GetItemListOnStorage( DWORD connectionIndex, DWORD userIndex, DWORD beginItemDbIndex );
void GetItemListOnShop( DWORD connectionIndex, DWORD userIndex, DWORD beginItemDbIndex );

void RGetItemList( LPQUERY, LPDBMESSAGE );

// 아이템 소유자를 찾는다
void FindItemOwner( DWORD connectionIndex, DWORD itemIndex, DWORD playerIndex );
void RFindItemOwner( LPQUERY, LPDBMESSAGE );

// 아이템 추가
void RAddItem( LPMIDDLEQUERY, LPDBMESSAGE );

// 아이템 사용 종료 시점 설정
void RSetItemEndTime( LPMIDDLEQUERY, LPDBMESSAGE );

// 아이템 사용 잔여 시간 설정
void SetItemRemainTime( DWORD connectionIndex, DWORD itemDbIndex, int second );
void RSetItemRemainTime( LPMIDDLEQUERY, LPDBMESSAGE );

// 스킬 정보를 획득한다
void GetSkillList( DWORD connectionIndex, DWORD playerIndex );
void RGetSkillList( LPQUERY, LPDBMESSAGE );

// 스킬 레벨을 바꾼다
void SetSkillLevel( DWORD connectionIndex, DWORD playerIndex, DWORD skillDbIndex, DWORD level );
void RSetSkillLevel( LPMIDDLEQUERY, LPDBMESSAGE );

// 스킬을 추가한다
void AddSkillData( DWORD connectionIndex, DWORD playerIndex, DWORD skillIndex );
void RAddSkillData( LPMIDDLEQUERY, LPDBMESSAGE );

// 스킬을 삭제한다
void RemoveSkillData( DWORD connectionIndex, DWORD skillDbIndex );
void RRemoveSkillData( LPMIDDLEQUERY, LPDBMESSAGE );

// 패밀리를 검색한다
void GetFamilyList( DWORD connectionIndex, const char* keyword, bool isDismissed );
void RGetFamilyList( LPQUERY, LPDBMESSAGE );

// 패밀리 일반 정보를 읽는다
void GetFamilyData( DWORD connectionIndex, DWORD playerIndex );
void RGetFamilyData( LPMIDDLEQUERY, LPDBMESSAGE );

// 패밀리 정보를 갱신한다
void RSetFamilyData( LPMIDDLEQUERY, LPDBMESSAGE );

// 패밀리 멤버 정보를 읽는다
void GetFamilyMember( DWORD connectionIndex, DWORD playerIndex );
void RGetFamilyMember( LPQUERY, LPDBMESSAGE );

// 패밀리 농장 정보를 읽는다
void GetFamilyFarm( DWORD connectionIndex, DWORD playerIndex );
void RGetFamilyFarm( LPMIDDLEQUERY, LPDBMESSAGE );

// 농작물 정보를 읽는다
void GetFamilyCrop( DWORD connectionIndex, DWORD playerIndex );
void RGetFamilyCrop( LPMIDDLEQUERY, LPDBMESSAGE );

// 메인 퀘스트 정보를 읽는다
void GetQuestMainData( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex );
void RGetQuestMainData( LPQUERY, LPDBMESSAGE );

// 서브 퀘스트 정보를 읽는다
void GetQuestSubData( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex );
void RGetQuestSubData( LPQUERY, LPDBMESSAGE );

// 퀘스트 아이템 정보를 읽는다
void GetQuestItemList( DWORD connectionIndex, DWORD playerIndex, DWORD questIndex );
void RGetQuestItemList( LPQUERY, LPDBMESSAGE );

// 서브퀘스트를 완료한다
void EndSubQuest( DWORD connectionIndex, DWORD playerIndex, DWORD mainQuestIndex, DWORD subQuestIndex, QSTATETYPE state );
void REndSubQuest( LPMIDDLEQUERY, LPDBMESSAGE );

// 퀘스트를 삭제한다(다시 진행할 수 있도록)
void RemoveQuestData( DWORD connectionIndex, DWORD playerIndex, DWORD mainQuestIndex );
void RRemoveQuestData( LPMIDDLEQUERY, LPDBMESSAGE );

// 퀘스트 아이템 수량을 변경한다
void RUpdateQuestItem( LPMIDDLEQUERY, LPDBMESSAGE );

// 퀘스트 아이템을 삭제한다
void DeleteQuestItem( DWORD connectionIndex, DWORD playerIndex, DWORD questMainIndex, DWORD itemIndex );
void RDeleteQuestItem( LPMIDDLEQUERY, LPDBMESSAGE );

// 퀘스트 로그 크기를 가져온다
void GetQuestLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetQuestLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 퀘스트 로그를 가져온다
void GetQuestLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetQuestLog( LPQUERY, LPDBMESSAGE );

// 길드를 검색한다
void GetGuildList( DWORD connectionIndex, const char* keyword, bool isDismissed );
void RGetGuildList( LPQUERY, LPDBMESSAGE );

// 길드 기본 정보를 얻는다
void GetGuildData( DWORD connectionIndex, const char* keyword, DWORD guildIndex );
void RGetGuildData( LPMIDDLEQUERY, LPDBMESSAGE );

// 창고 정보를 얻는다
void GetGuildStore( DWORD connectionIndex, DWORD guildIndex );
void RGetGuildStore( LPQUERY, LPDBMESSAGE );

// 회원 정보를 얻는다
void GetGuildMember( DWORD connectionIndex, DWORD guildIndex );
void RGetGuildMember( LPQUERY, LPDBMESSAGE );

// 아군 정보를 얻는다
void GetGuildFriendly( DWORD connectionIndex, DWORD guildIndex );
void RGetGuildFriendly( LPQUERY, LPDBMESSAGE );

// 적군 정보를 얻는다
void GetGuildEnemy( DWORD connectionIndex, DWORD guildIndex );
void RGetGuildEnemy( LPQUERY, LPDBMESSAGE );

// 길드 정보를 갱신한다
void GuildUpdateData( DWORD connectionIndex, const MSG_RM_GUILD_DATA& );
void RGuildUpdateData( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 직위를 변경한다
void GuildSetRank( DWORD connectionIndex, DWORD guildIndex, DWORD playerIndex, DWORD rank );
void RGuildSetRank( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드에서 탈퇴시킨다
void GuildKickMember( DWORD connectionIndex, DWORD playerIndex );
void RGuildKickMember( LPMIDDLEQUERY, LPDBMESSAGE );

// 로그 개수가 몇개인지 알아낸다.
void GetExperienceLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetExperienceLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 경험치 로그 정보를 조회한다
void GetExperienceLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetExperienceLog( LPQUERY, LPDBMESSAGE );

// 아이템 로그 정보 개수 조회
void RGetItemLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 아이템 로그를 가져온다
void RGetItemLog( LPQUERY, LPDBMESSAGE );

// 스탯 로그 크기를 가져온다
void GetStatLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetStatLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 스탯 로그를 가져온다
void GetStatLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetStatLog( LPQUERY, LPDBMESSAGE );

// 스킬 로그 크기를 가져온다
void GetSkillLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetSkillLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 스킬 로그를 가져온다
void GetSkillLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetSkillLog( LPQUERY, LPDBMESSAGE );

// 아이템 옵션 로그 크기를 가져온다
void GetItemOptionLogSize( DWORD connectionIndex, DWORD itemDbIndex, const char* beginDate, const char* endDate );
void RGetItemOptionLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 아이템 옵션 로그를 가져온다
void GetItemOptionLog( DWORD connectionIndex, DWORD itemDbIndex, const char* beginDate, const char* endDate );
void RGetItemOptionLog( LPQUERY, LPDBMESSAGE );

// 길드 로그 크기를 가져온다
void GetGuildLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 회원 로그를 가져온다
void GetGuildLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildLog( LPQUERY, LPDBMESSAGE );

// 길드 창고 로그 크기를 가져온다
void GetGuildWarehouseLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildWarehouseLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 창고 로그를 가져온다
void GetGuildWarehouseLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildWarehouseLog( LPQUERY, LPDBMESSAGE );

// 클래스 로그 크기를 가져온다
//void GetJobLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetJobLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 클래스로그를 가져온다
void GetJobLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetJobLog( LPQUERY, LPDBMESSAGE );

// 패밀리 농작물 로그 크기를 가져온다
void GetFarmCropLogSize( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate );
void RGetFarmCropLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 패밀리 농작물 로그를 가져온다
void GetFarmCropLog( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate );
void RGetFarmCropLog( LPQUERY, LPDBMESSAGE );

// 패밀리 포인트 크기를 가져온다
void GetFamilyPointLogSize( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate );
void RGetFamilyPointLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 패밀리 포인트 로그를 가져온다
void GetFamilyPointLog( DWORD connectionIndex, DWORD familyIndex, const char* beginDate, const char* endDate );
void RGetFamilyPointLog( LPQUERY, LPDBMESSAGE );

// 길드 점수 로그 크기를 가져온다
void GetGuildScoreLogSize( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildScoreLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 점수 로그를 가져온다
void GetGuildScoreLog( DWORD connectionIndex, DWORD guildIndex, const char* beginDate, const char* endDate );
void RGetGuildScoreLog( LPQUERY, LPDBMESSAGE );

// 플레이어 아이템 로그를 가져온다
void GetPlayerItemLogSize( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetPlayerItemLogSize( LPMIDDLEQUERY, LPDBMESSAGE );

// 플레이어 아이템 로그를 가져온다
//void GetPlayerItemLog( DWORD connectionIndex, DWORD playerIndex, int beginLogIndex, int endLogIndex );
void GetPlayerItemLog( DWORD connectionIndex, DWORD playerIndex, const char* beginDate, const char* endDate );
void RGetPlayerItemLog( LPQUERY, LPDBMESSAGE );

// IP를 접속 허용 목록에 추가한다
void AddPermission( DWORD connectionIndex, const char* ipAddress );
void RAddPermission( LPMIDDLEQUERY, LPDBMESSAGE );

// IP를 접속 허용 목록에서 삭제한다
void RemovePermission( DWORD connectionIndex, const char* ipAddress );
void RRemovePermission( LPMIDDLEQUERY, LPDBMESSAGE );

// 창고
void RGetStorageData( LPMIDDLEQUERY, LPDBMESSAGE );
void RSetStorageData( LPMIDDLEQUERY, LPDBMESSAGE );

// operator
void RGetOperator( LPQUERY, LPDBMESSAGE );
void RAddOperator( LPMIDDLEQUERY, LPDBMESSAGE );
void RUpdateOperator( LPMIDDLEQUERY, LPDBMESSAGE );
void RGetOperatorIp( LPQUERY, LPDBMESSAGE );
void RAddOperatorIp( LPMIDDLEQUERY, LPDBMESSAGE );
void RRemoveOperatorIp( LPMIDDLEQUERY, LPDBMESSAGE );

// password
void RSetPassword( LPMIDDLEQUERY, LPDBMESSAGE );

// operator log
void RGetOperatorLogSize( LPMIDDLEQUERY, LPDBMESSAGE );
void RGetOperatorLog( LPQUERY, LPDBMESSAGE );

// name log
void RGetNameLog( LPQUERY, LPDBMESSAGE );