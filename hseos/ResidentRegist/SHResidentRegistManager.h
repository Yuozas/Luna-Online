/*********************************************************************

	 ����		: SHResidentRegist.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/06/04

	 ���ϼ���	: �ֹε�� Ŭ������ ���

 *********************************************************************/

#pragma once

class CPlayer;

#if !defined(_AGENTSERVER)
struct USERINFO;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHResidentRegistManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static const int				MAX_SERCH_NUM_PER_AGENT;									// ������Ʈ �ϳ��� ����Ʈ ��Ī �˻� �ִ� ����
	static const int				MAX_SERCH_NUM_CLIENT;										// ����Ʈ ��Ī �˻� �ִ� ����(Ŭ���̾�Ʈ��)
	static const int				DATE_MATCHING_RESERCH_TIME;									// ����Ʈ ��Ī �˻� ������ �ð�

	//----------------------------------------------------------------------------------------------------------------
	typedef enum																				// ���� ����
	{
		PROPENSITY_KIND_HAIR_BLACK = 0,
		PROPENSITY_KIND_HAIR_DYEING,
		PROPENSITY_KIND_CLOTHES_SUIT,
		PROPENSITY_KIND_CLOTHES_CASUAL,
		PROPENSITY_KIND_MAKEUP,
		PROPENSITY_KIND_PERFUME,
		PROPENSITY_KIND_BODY_THIN,
		PROPENSITY_KIND_BODY_FAT,
		PROPENSITY_KIND_BEARD,
		PROPENSITY_KIND_GLASSES,
		PROPENSITY_KIND_HAT,
		PROPENSITY_KIND_ACCESSORY,
		PROPENSITY_KIND_SUMMER,
		PROPENSITY_KIND_WINTER,
		PROPENSITY_KIND_BLOOD_A,
		PROPENSITY_KIND_BLOOD_B,
		PROPENSITY_KIND_BLOOD_O,
		PROPENSITY_KIND_BLOOD_AB,
		PROPENSITY_KIND_BOTTLE_STAR,
		PROPENSITY_KIND_FISH_STAR,
		PROPENSITY_KIND_SHEEP_STAR,
		PROPENSITY_KIND_OX_STAR,
		PROPENSITY_KIND_TWINS_STAR,
		PROPENSITY_KIND_CRAB_STAR,
		PROPENSITY_KIND_LION_STAR,
		PROPENSITY_KIND_VIRGIN_STAR,
		PROPENSITY_KIND_WEIGHT_STAR,
		PROPENSITY_KIND_SCORPION_STAR,
		PROPENSITY_KIND_ARCHER_STAR,
		PROPENSITY_KIND_GOAT_STAR,
		PROPENSITY_KIND_CLEAN,
		PROPENSITY_KIND_COOKING,
		PROPENSITY_KIND_PARTY,
		PROPENSITY_KIND_LONELY,
		PROPENSITY_KIND_SPORTS,
		PROPENSITY_KIND_MUSIC,
	} PROPENSITY_KIND;

	//----------------------------------------------------------------------------------------------------------------
	typedef enum																				 // ����Ʈ ��Ī �˻� ����
	{
		DMSAR_UNDER_10 = 0,
		DMSAR_11_TO_14,
		DMSAR_15_TO_19,
		DMSAR_20_TO_24,
		DMSAR_25_TO_29,
		DMSAR_30_TO_34,
		DMSAR_40_TO_49,
		DMSAR_OVER_50,
		DMSAR_MAX,
	} DATE_MATCHING_SERCH_AGE_RANGE;

	//----------------------------------------------------------------------------------------------------------------
	typedef enum																				// ä�� ���
	{
		DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT = 0,											// ..��û�ڿ��� ��ûó���� ��⸦ �˸�
		DATE_MATCHING_CHAT_RESULT_REQUESTED,													// ..����ڿ��� �������� ä�� ��û�� �˸�
		DATE_MATCHING_CHAT_RESULT_CANCEL,														// ..��û�ڱ� ��û ���
		DATE_MATCHING_CHAT_RESULT_BUSY,															// ..����ڰ� �̹� ä�� �� or ä�� ��û�� or ä�� ��û�޴� �� 
		DATE_MATCHING_CHAT_RESULT_LOGOFF,														// ..����ڰ� ���� ���� ������ ����
		DATE_MATCHING_CHAT_RESULT_TIMEOUT,														// ..�ð� �ʰ�
		DATE_MATCHING_CHAT_RESULT_REFUSE,														// ..������
		DATE_MATCHING_CHAT_RESULT_START_CHAT,													// ..�¶��Ͽ� ä�� ����
		DATE_MATCHING_CHAT_RESULT_END_CHAT,														// ..ä�� ��
	} DATE_MATCHING_CHAT_RESULT;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct																				// �ֹε�� ����
	{
		char	szNickName[MAX_NAME_LENGTH+1];
		WORD	nRace;
		WORD	nSex;
		WORD	nAge;
		WORD	nLocation;
		WORD	nFavor;
		WORD	nPropensityLike01;
		WORD	nPropensityLike02;
		WORD	nPropensityLike03;
		WORD	nPropensityDisLike01;
		WORD	nPropensityDisLike02;
	} stRegistInfo;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct																				// �������� ����
	{
		WORD	nID;
		char	szName[MAX_PATH];
	} stLocation;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct																				// ����Ʈ ��Ī �˻� ���� ����
	{
		WORD	nMin;
		WORD	nMax;
	} stSerchAgeRange;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct																				// ����Ʈ ��Ī �˻���� ����
	{
		stRegistInfo	stRegistInfo;															// ..��� ����
		char			szIntroduction[MAX_INTRODUCE_LEN+1];									// ..�Ұ���
		WORD			nMatchingPoint;															// ..��Ī����
		DWORD			nID;																	// ..ID
		DWORD			nAgentID;																// ..������ƮID
	} stSerchResult;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct																				// �ΰ����� ����
	{
		WORD					m_nMaxPropensity;												// ..������ �ִ밪
		WORD					m_nMaxLocation;													// ..���� ������ �ִ밪
		WORD					m_nMaxSerchAgeRange;											// ..����Ʈ ��Ī �˻��� ���̹���

		WORD*					m_pTblPropensity;												// ..���� ���̺�
		stLocation*				m_pTblLocation;													// ..���� ���̺�
		stSerchAgeRange*		m_pTblSerchAgeRange;											// ..�˻� ���̹��� ���̺�
	} stADDInfo;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct : public MSGBASE																// �ֹε�� ��Ŷ ����
	{
		stRegistInfo		stInfo;																// ..��� ����
	} PACKET_RESIDENTREGIST_REGIST;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct : public MSGBASE																// �ڱ�Ұ� ��Ŷ ����
	{
		char		szIntroduction[MAX_INTRODUCE_LEN+1];										// ..�ڱ�Ұ���
	} PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION;

	//----------------------------------------------------------------------------------------------------------------
	typedef struct : public MSGBASE																// ����Ʈ ��Ī �˻� ��Ŷ ����
	{
		stSerchResult	stSerchResult;															// ..�˻����
		int				nGroupIndex;															// ..�׷� �ε��� (Ŭ���̾�Ʈ���� �� ȭ�鿡 �������� ������� �׷����� Ī��)
		int				nGroupMax;																// ..�� �׷� ����
		DWORD			nRequestPlayerID;														// ..�˻� ��û �÷��̾�ID(�ٸ� ������Ʈ�� �˻� ��û �� ����� ���� �� �ʿ�)
	} PACKET_SERCH_DATE_MATCHING;

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_SERCH_DATE_MATCHING_TO_OTHER_AGENT : public MSGBASE							// �ٸ� ������Ʈ�� �˻� ��û
	{
		DWORD 			nAgeMin;
		DWORD 			nAgeMax;
		DWORD 			nLocation;
		DWORD 			nSex;
		DWORD 			nGroupIndex;
		DWORD			nRequestPlayerID;

		stRegistInfo	stRegistInfo;
	};

private:
	stADDInfo						m_stAddInfo;
	stSerchResult*					m_pstSerchResult;											// �˻����
	int								m_nSerchResultNum;											// �˻���� ����
	stSerchResult					m_stChatPartner;											// ä�� ��� ����

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHResidentRegistManager();
	~CSHResidentRegistManager();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//----------------------------------------------------------------------------------------------------------------
	//						��Ī ����Ʈ ���
	UINT					CalcMatchingPoint(DATE_MATCHING_INFO* pstInfo, stRegistInfo* pstTargetInfo);
	UINT					CalcMatchingPoint(DATE_MATCHING_INFO* pstInfo, DATE_MATCHING_INFO* pstTargetInfo);

	//----------------------------------------------------------------------------------------------------------------
	//						��ũ��Ʈ ���� �ε�
	BOOL					LoadScriptFileData();														
	//----------------------------------------------------------------------------------------------------------------
	//						�ΰ����� ���
	stADDInfo*				GetADDInfo()				{ return &m_stAddInfo; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����					
	//
	//----------------------------------------------------------------------------------------------------------------
	//	������Ʈ
	//						Ŭ���̾�Ʈ ��û �м�/ó��
	VOID					ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID					ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//						�α׾ƿ� ����� ��Ȳ�� ���� ä�� ����
	VOID					ASRV_EndDateMatchingChat(USERINFO* pUserInfo);
	//						����Ʈ ��Ī ä�� ���� �ʱ�ȭ
	VOID					ASRV_InitDateMatchingChatState(USERINFO* pUserInfo);
	//						��Ī ����Ʈ ���
//	UINT					ASRV_CalcMatchingPoint(USERINFO* pUserInfo, USERINFO* pSerchInfo);

	//----------------------------------------------------------------------------------------------------------------
	//	��
	//						Ŭ���̾�Ʈ ��û �м�/ó��
	VOID					SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//----------------------------------------------------------------------------------------------------------------
	//						�ֹε�� ó��
	VOID					SRV_RegistResident(CPlayer* pPlayer, stRegistInfo* pInfo);
	//----------------------------------------------------------------------------------------------------------------
	//						�ڱ�Ұ� ������Ʈ ó��
	VOID					SRV_UpdateIntroduction(CPlayer* pPlayer, char* szIntroduction);
	//----------------------------------------------------------------------------------------------------------------
	//						����Ʈ ��Ī �˻�
	VOID					SRV_SerchDateMatching(CPlayer* pPlayer, WORD nAgeRange, WORD nLocation, WORD nSex, WORD nGroupIndex);
	//----------------------------------------------------------------------------------------------------------------
	//						����Ʈ ��Ī ä�� ��û ó��
	VOID					SRV_RequestDateMatchingChat(CPlayer* pPlayer, MSG_DWORD2* pPacket);
	//----------------------------------------------------------------------------------------------------------------
	//						����Ʈ ��Ī ä�� ��� ó��
	VOID					SRV_ResultRequestDateMatchingChat(CPlayer* pPlayer, MSG_DWORD2* pPacket);
	//----------------------------------------------------------------------------------------------------------------
	//						����Ʈ ��Ī ä�� �ð� �ʰ� ó��
	VOID					SRV_ProcDateMatchingChatTimeOut(CPlayer* pPlayer);
	//----------------------------------------------------------------------------------------------------------------
	//						080215 KTH --�ֹε�� �ʱ�ȭ
	BOOL					SRV_ResetRegistResident(CPlayer* pPlayer);
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ
	//
	//----------------------------------------------------------------------------------------------------------------
	//						������ �ֹε�� ��û
	VOID					CLI_RequestRegist(stRegistInfo* pInfo);
	//						������ �ڱ�Ұ� ������Ʈ ��û
	VOID					CLI_RequestUpdateIntroduction(char* szIntroduction);
	//						������ ����Ʈ ��Ī �˻� ��û
	VOID					CLI_RequestSerchDateMatching(WORD nAgeRange, WORD nLocation, WORD nSex, int nGroupIndexDelta = 0);
	//						������ �ٸ� ĳ���� ��õ�ϱ� ��û
	VOID					CLI_RequestRecommendOtherChar(DWORD nTargetPlayerID);
	//						������ ����Ʈ ��Ī ä�� ��û
	VOID					CLI_RequestDateMatchingChat(stSerchResult* pSerchTarget);
	//						������ ����Ʈ ��Ī ä�� ��û ���
	VOID					CLI_CancelDateMatchingChat();
	//						������ ����Ʈ ��Ī ä�� ��û ����
	VOID					CLI_RefuseDateMatchingChat();
	//						������ ����Ʈ ��Ī ä�� ��û �¶�
	VOID					CLI_StartDateMatchingChat();
	//						������ ����Ʈ ��Ī ä�� �� ��û
	VOID					CLI_EndDateMatchingChat();

	//						������ ����Ʈ ��Ī ä�� ���� ������
	VOID					CLI_SendDateMatchingChatText(char* szText);

	//						����Ʈ ��Ī �˻���� �ִ밪 ���
	int						CLI_GetSerchDateMatchingMax()				{ return m_nSerchResultNum; }
	//						����Ʈ ��Ī �˻���� ���
	stSerchResult*			CLI_GetSerchDateMatchingResult(int nIndex)	{ return &m_pstSerchResult[nIndex]; }
	//						ä�� ��� ���� 
	stSerchResult			CLI_GetChatPartner()						{ return m_stChatPartner; }

	//----------------------------------------------------------------------------------------------------------------
	//						�����κ����� ���� �м�/ó��
	VOID					CLI_ParseAnswerFromSrv(void* pMsg);
};

extern CSHResidentRegistManager g_csResidentRegistManager;
