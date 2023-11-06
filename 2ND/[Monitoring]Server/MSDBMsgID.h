DBID(eSearchUserInfoById)			DBRETID(NULL)
DBID(eSearchUserInfoByIdCard)		DBRETID(NULL)	
DBID(eSearchUserInfoByCharName)		DBRETID(NULL)

DBID(eCharacterBaseInfo)			DBRETID(RCharacterBaseInfo)
DBID(eCharacterUpdateBaseInfo)		DBRETID(NULL)
DBID(eCharacterInvenInfo)			DBRETID(RCharacterInvenInfo)
DBID(eCharacterPyogukInfo)			DBRETID(RCharacterPyogukInfo)
DBID(eCharacterUpdatePyogukInfo)	DBRETID(NULL)
DBID(eCharacterPyogukItemInfo)		DBRETID(RCharacterPyogukItemInfo)
DBID(eCharacterMugongInfo)			DBRETID(RCharacterMugongInfo)
DBID(eCharacterAbilityInfo)			DBRETID(NULL)
DBID(eCharacterItemOptionInfo)		DBRETID(RCharacterItemOptionInfo)

DBID(eSearchMunpaByName)			DBRETID(RSearchMunpaIdx)
DBID(eMunpaBaseInfo)				DBRETID(RMunpaBaseInfo)
DBID(eMunpaItemInfo)				DBRETID(RMunpaItemInfo)
DBID(eMunpaMemberInfo)				DBRETID(RMunpaMemberInfo)

DBID(eGameItemMoveLog)				DBRETID(RGameLogItemMoveLog)
DBID(eGameItemObtainLog)			DBRETID(RGameLogItemObtainLog)
DBID(eGameBaseInfoLog)				DBRETID(RGameLogBaseInfoLog)
DBID(eGameMoneyLog)					DBRETID(RGameLogMoneyLog)
DBID(eGameItemFromChrName)			DBRETID(RGameLogItemFromChrName)
DBID(eGameItemFromItemDBIndex)		DBRETID(RGameLogItemFromItemDBIndex)

DBID(eOperInsertIp)					DBRETID(RInsertIpAdress)
DBID(eOperInsertOper)				DBRETID(RInsertOperator)
DBID(eOperDeleteIp)					DBRETID(NULL)
DBID(eOperDeleteOper)				DBRETID(NULL)
DBID(eOperTotalIpInfo)				DBRETID(RTotalIpAdressInfo)
DBID(eOperTotalOperInfo)			DBRETID(RTotalOperatorInfo)
DBID(eOperLoginCheck)				DBRETID(ROperatorLoginCheck)
/// 060829 PKH - gm툴 허용 IP
DBID(eOperInsertGMIp)				DBRETID(RInsertGMIpAdress)
DBID(eOperTotalGMIpInfo)			DBRETID(RTotalGMIpAdressInfo)

DBID(eItemDelete)					DBRETID(NULL)
DBID(eItemUpdate)					DBRETID(NULL)
DBID(eInvenItemInsert)				DBRETID(RInvenItemInsert)
DBID(ePyogukItemInsert)				DBRETID(RPyogukItemInsert)
DBID(eMunpaItemInsert)				DBRETID(RMunpaItemInsert)

DBID(eItemOptionInsert)				DBRETID(NULL)
DBID(eItemOptionUpdate)				DBRETID(NULL)
DBID(eItemOptionDelete)				DBRETID(NULL)

DBID(eMugongInsert)					DBRETID(NULL)
DBID(eMugongUpdate)					DBRETID(NULL)
DBID(eMugongDelete)					DBRETID(NULL)

DBID(eAbilityUpdate)				DBRETID(NULL)
DBID(eCharacterWearItemUpdate)		DBRETID(NULL)


// IP Check
DBID(eTotalIpInfo)					DBRETID(RTotalIpInfo)
DBID(eItemMoneyLogFromType)			DBRETID(RItemMoneyLogFromType)
DBID(eCharacterLog)					DBRETID(RCharacterLog)
DBID(eExpPointLog)					DBRETID(RExpPointLog)
DBID(eMugongLog)					DBRETID(RMugongLog)
DBID(eMugongExpLog)					DBRETID(RMugongExpLog)
DBID(eMoneyLog)						DBRETID(RMoneyLog)
DBID(eMoneyWrongLog)				DBRETID(RMoneyWrongLog)
DBID(eAbilityLog)					DBRETID(RAbilityLog)
DBID(eItemOptionLog)				DBRETID(RItemOptionLog)
DBID(eCheatLog)						DBRETID(RCheatLog)
DBID(eHackingLog)					DBRETID(RHackingLog)
DBID(eToolLog)						DBRETID(RToolLog)
DBID(ePetLogFromType)				DBRETID(RPetLogFromType)
/// 060831 PKH 문파토너먼트 로그 정보
DBID(eGuildTNLogFromType)				DBRETID(RGuildTNLogFromType)
/// 060831 PKH 공성전
DBID(eSiegeWarLogFromType)				DBRETID(RSiegeWarLogFromType)
/// 060915 PKH GMTOOl
DBID(eGMToolLogFromType)				DBRETID(RGMToolLogFromType)

DBID(eSearchMemberNumFromID)		DBRETID(RSearchMemberNumFromID)
DBID(eSearchMemberNumFromCharName)	DBRETID(RSearchMemberNumFromCharName)
DBID(eSearchCharIdxFromCharName)	DBRETID(RSearchCharIdxFromCharName)
DBID(eSimpleCharacterInfoFromMN)	DBRETID(RSimpleCharacterInfoFromMN)
DBID(eCharacterCreateInfoFromMN)	DBRETID(RCharacterCreateInfoFromMN)
DBID(eCheckCharacterName)			DBRETID(RCheckCharacterName)
DBID(eCharacterRecover)				DBRETID(RCharacterRecover)

/// 060831 PKH 문토문파검색
DBID(eSearchGuildIdxFromGuildName)	DBRETID(RSearchGuildIdxFromGuildName)
/// 060831 PKH 공성전문파검색
DBID(eSearchSWGuildIdxFromGuildName)	DBRETID(RSearchSWGuildIdxFromGuildName)


DBID(eCharacterMainQuestInfo)		DBRETID(RCharacterMainQuestInfo)
DBID(eMainQuestDelete)				DBRETID(NULL)
DBID(eCharacterSubQuestInfo)		DBRETID(RCharacterSubQuestInfo)

DBID(eLogItemMoney)					DBRETID(NULL)

DBID(eChangeUserLevel)				DBRETID(RChangeUserLevel)
DBID(eChangeLoginPoint)				DBRETID(RChangeLoginPoint)

DBID(eCharacterInvenInfo2)			DBRETID(RCharacterInvenInfo2)

DBID(eChangeItemPosition)			DBRETID(NULL)
DBID(eChangeMugongPosition)			DBRETID(NULL)
DBID(eInsertLogTool)				DBRETID(NULL)

DBID(eCharacterShopWareItemInfo)	DBRETID(RCharacterShopWareItemInfo)
DBID(eShopWareItemInsert)			DBRETID(RShopWareItemInsert)

DBID(eSearchGuild)					DBRETID(RSearchGuild)
DBID(eSearchGuildInfo)				DBRETID(RSearchGuildInfo)
DBID(eSearchGuildMemberInfo)		DBRETID(RSearchGuildMemberInfo)
DBID(eSearchGuildWareItemInfo)		DBRETID(RSearchGuildWareItemInfo)
DBID(eSearchGuildWareItemOptionInfo)	DBRETID(RSearchGuildWareItemOptionInfo)

// rare item
DBID(eCharacterRareItemOptionInfo)	DBRETID(RCharacterRareItemOptionInfo)

DBID(eCharacterUsingShopItemInfo)	DBRETID(RCharacterUsingShopItemInfo)

DBID(eCharacterUsingShopItemInfoDelete)	DBRETID(NULL)

DBID(eCharacterUsingShopItemInfoEdit)	DBRETID(NULL) 

// pet
DBID(ePetBaseInfo)				DBRETID(RSearchPetInfo)
DBID(ePetList)					DBRETID(RSearchPetList)
DBID(eSearchPetWareItemInfo)		DBRETID(RSearchPetWareItemInfo)
DBID(eSearchPetAccItemInfo)		DBRETID(RSearchPetAccItemInfo)
DBID(eSearchUserLoginChk)		DBRETID(RSearchUserLoginChk)

DBID(eCharacterMainPetList)		DBRETID(RCharacterMainPetList)
DBID(eSearchPetMemberNumFromID)		DBRETID(RSearchPetMemberNumFromID)

//delete character list
DBID(eSearchDelCharList)		DBRETID(RSearchDelCharList)
