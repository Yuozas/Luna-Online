/*********************************************************************

	 파일		: SHMarriageManager.h
	 작성자		: hseos
	 작성일		: 2007/12/10

	 파일설명	: 결혼 클래스의 헤더

 *********************************************************************/

#pragma once

class CPlayer;

#if !defined(_AGENTSERVER)
struct USERINFO;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHMarriageManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static UINT							DIVORCE_PENALTY_REMARRIAGE_TIME;		// 이혼 후 결혼에 필요한 시간
	static UINT							TELEPORT_TO_PARTNER_FREQ;				// 배우자 위치로 텔레포트 가능 회수
	static UINT							NICKNAME_CHARACTER_NUM;					// 호칭 글자(바이트) 수

	struct MARRIAGE_ITEM
	{
		DWORD							nItemID;
		UINT							nItemNum;
	};

	struct MARRIAGE_NEED_ITEM
	{
		DWORD*							pstWearItemID;
		UINT							nWearItemNum;
	};

	struct MARRIAGE_TAKE_ITEM
	{
		MARRIAGE_ITEM*					pstItem;
		UINT							nItemNum;
	};

	struct PACKET_MARRIAGE_INFO : public MSGBASE
	{
		DWORD			nPartnerID;													// ..배우자 ID
		BYTE			nRace;														// ..종족
		BYTE			nJobFirst;													// ..직업(처음)
		BYTE			nJobCur;													// ..직업(현재)
		BYTE			nJobGrade;													// ..직업등급
		WORD			nLv;														// ..레벨
		WORD			nTeleportFreq;												// ..텔레포트 횟수
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	MARRIAGE_NEED_ITEM					m_stMarriageNeedItem;
	MARRIAGE_TAKE_ITEM					m_stMarriageTakeItem;

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHMarriageManager();
	~CSHMarriageManager();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 공용
	//
	//----------------------------------------------------------------------------------------------------------------
	//									데이터 읽기
	BOOL								LoadMarriageInfo();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 서버					
	//
	//----------------------------------------------------------------------------------------------------------------
	// 에이전트
	//									클라이언트 요청 분석/처리
	VOID								ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID								ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//									배우자 위치로 텔레포트
	VOID								ASRV_TeleportToPartner(USERINFO* pUserInfo);

	//----------------------------------------------------------------------------------------------------------------
	// 맵
	//									클라이언트 요청 분석/처리
	VOID								SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//									결혼
	VOID								SRV_Marriage(CPlayer* pPlayer, DWORD nPartnerID);
	//									결혼 결과
	VOID								SRV_MarriageResult(CPlayer* pPlayer, BOOL bAccept);
	//									합의 이혼
	VOID								SRV_AgreeDivorce(CPlayer* pPlayer, DWORD nPartnerID);
	//									합의 이혼 결과
	VOID								SRV_AgreeDivorceResult(CPlayer* pPlayer, BOOL bAccept);
	//									강제 이혼
	VOID								SRV_ForceDivorce(CPlayer* pPlayer);
	//									강제 이혼 당함
	VOID								SRV_ForceDivorced(CPlayer* pPlayer);
	//									배우자 위치로 텔레포트
	VOID								SRV_TeleportToPartner(CPlayer* pPlayer);
	//									호칭 변경
	VOID								SRV_ChangeNickname(CPlayer* pPlayer, char* pszName);
	//									결혼 정보 보내기
	VOID								SRV_SendMarriageInfo(CPlayer* pPlayer);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 클라이언트
	//
	//----------------------------------------------------------------------------------------------------------------
	//									결혼 요청
	VOID								CLI_RequestMarriage(BOOL bWin);
	//									결혼 수락
	VOID								CLI_RequestAcceptMarriage(BOOL bAccept);
	//									합의 이혼 요청
	VOID								CLI_RequestAgreeDivorce();
	//									합의 이혼 수락
	VOID								CLI_RequestAcceptAgreeDivorce(BOOL bAccept);
	//									강제 이혼 요청
	VOID								CLI_RequestForceDivorce();
	//									배우자 위치로 텔레포트 요청
	VOID								CLI_RequestTeleportToPartner();
	//									호칭 변경 요청
	VOID								CLI_RequestNickname(char* pszNickname);

	//									이름 앞에 호칭 붙이기
	VOID								CLI_SetNickname(CPlayer* pPlayer, char* pszPartnerName, char* pszPartnerNickname = NULL);

	//----------------------------------------------------------------------------------------------------------------
	//									서버로부터의 응답 분석/처리
	VOID								CLI_ParseAnswerFromSrv(void* pMsg);
};

extern CSHMarriageManager g_csMarriageManager;
