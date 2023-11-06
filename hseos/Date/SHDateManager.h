/*********************************************************************

	 ����		: SHDateManager.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/11/16

	 ���ϼ���	: CSHDateManager Ŭ������ ���

 *********************************************************************/

#pragma once

class CPlayer;

#if !defined(_AGENTSERVER)
struct USERINFO;
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHDateManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static UINT					NOTICE_HEART_MATCHING_POING;							// ��Ʈ �˸��� ��Ī ����Ʈ
	static UINT					ENTER_CHALLENGE_ZONE_MATCHING_POINT;					// ç���� �� ���� ���� ��Ī ����Ʈ
	static UINT					ENTER_CHALLENGE_ZONE_FREQ_PER_DAY;						// ç���� �� ���� ���� ȸ��(�Ϸ�)
	static UINT					ENTER_CHALLENGE_ZONE_SECTION_NUM;						// ç���� �� ���� ����
	static DWORD				CHALLENGE_ZONE_START_DELAY_TIME;						// ç���� �� ���� ��� �ð�
	static DWORD				CHALLENGE_ZONE_END_DELAY_TIME;							// ç���� �� ���� ��� �ð�

	static UINT					CHALLENGE_ZONE_MOTION_NUM_START;						// ç���� �� ���� ��� ��ȣ
	static UINT					CHALLENGE_ZONE_MOTION_NUM_SUCCESS;						// ç���� �� ���� ��� ��ȣ
	static UINT					CHALLENGE_ZONE_MOTION_NUM_SUCCESS_LEAST_CLEAR_TIME;		// ç���� �� �ִ� �ð� Ŭ���� ���� ��� ��ȣ
	static UINT					CHALLENGE_ZONE_MOTION_NUM_FAIL;							// ç���� �� ���� ��� ��ȣ

	enum CHALLENGEZONE_STATE
	{
		CHALLENGEZONE_START				= 1,
		CHALLENGEZONE_END				= 10000000,										// == �� �ƴ϶� > �� �� END ��
		CHALLENGEZONE_END_TIMEOUT,
		CHALLENGEZONE_END_PARTNER_OUT,
		CHALLENGEZONE_END_ALL_DIE,
		CHALLENGEZONE_END_SUCCESS,
		CHALLENGEZONE_END_START_COUNTDOWN,
	};

	struct stCHALLENGEZONE_MONSTER
	{
		UINT	nMonsterKind;
		UINT	nMonsterNum;
		float	nPosX;
		float	nPosZ;
		DWORD	nDelayTime;
	};

	struct stCHALLENGEZONE_MONSTER_GROUP
	{
		stCHALLENGEZONE_MONSTER*		pstMonster;
		UINT							nMonsterNum;
	};

	struct stCHALLENGEZONE_MONSTER_GROUP_SECTION
	{
		stCHALLENGEZONE_MONSTER_GROUP*	pstGroup;
		UINT							nGroupNum;
		UINT							nTotalMonsterNum;
	};

	struct stCHALLENGEZONE_MAPINFO
	{
		UINT							nMapNum;
		UINT							nMoveIndex;
	};

	struct stCHALLENGEZONE_SECTION_MONLEVEL
	{
		DWORD							nStart;
		DWORD							nEnd;
		UINT							nLimitTime;
		UINT							nLeastClearTime;
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	BOOL									m_bIsChallengeZone;

	UINT*									m_pnDateZoneMoveIndexList;
	UINT									m_nDateZoneMoveIndexNum;

	stCHALLENGEZONE_MAPINFO*				m_pstChallengeZoneMoveIndexList;
	UINT									m_nChallengeZoneMoveIndexNum;

	UINT									m_nChallengeZoneEnterNum;

	stCHALLENGEZONE_MONSTER_GROUP_SECTION*	m_pstChallengeZoneMonsterGroupSection;
	UINT									m_nChallengeZoneMonsterGroupSectionNum;

	stCHALLENGEZONE_SECTION_MONLEVEL*		m_pstChallengeZoneSectionMonLevel;
	UINT									m_nChallengeZoneSectionNum;

	UINT									m_nChallengeZoneState;
	DWORD									m_nChallengeZoneTime;
	DWORD									m_nChallengeZoneTimeTick;

	BOOL									m_bChallengeZoneStart;
	DWORD									m_nChallengeZoneFirstEnterPlayerID;

	DWORD									m_nChallengeZoneLeastClearTime;

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHDateManager();
	~CSHDateManager();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//								�ʱ�ȭ
	VOID							Init();
	//								������ �б�
	BOOL 							LoadDateInfo();
	//								ç���� �� ���� ������ �б�
	BOOL 							LoadChallengeMonsterInfo(int nMapNum);
	//								ç���� �� �ΰ�? 
	//								..���� �ʿ� ���� ������ Ȯ���� ���� IsChallengeZoneHere �� ���
	//								..�� ��ȣ�� üũ�ϰų� üũ������ �ָ��� ��� IsChallengeZone ���
	BOOL							IsChallengeZone(int nMapNum);
	BOOL							IsChallengeZoneHere()					{ return m_bIsChallengeZone; }

	//								����Ʈ �� �̵� �ε��� ���
	BOOL 							GetDateZoneMoveIndex(UINT nSelIndex, int* pnIndex);
	//								����Ʈ �� ���� ���
	UINT 							GetDateZoneMoveIndexNum()				{ return m_nDateZoneMoveIndexNum; }
	//								ç���� �� �̵� �ε��� ���
	BOOL 							GetChallengeZoneMoveIndex(UINT nSelIndex, int* pnIndex);
	//								ç���� �� ���� ���
	UINT 							GetChallengeZoneMoveIndexNum()			{ return m_nChallengeZoneMoveIndexNum; }

	//								ç���� �� ���� ȸ�� ���
	UINT							GetChallengeZoneEnterNum()				{ return m_nChallengeZoneEnterNum; }
	//								ç���� �� ���� ���� ���
	stCHALLENGEZONE_MONSTER_GROUP*	GetChallengeZoneMonsterGroup(int nSection, int nIndex)	
									{ return &m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nIndex]; }

	//								ç���� �� ���� ����
	VOID							SetChallengeZoneStart(BOOL bStart)					{ m_bChallengeZoneStart = TRUE; }
	VOID							SetChallengeZoneFirstEnterPlayerID(DWORD nPlayerID)	{ m_nChallengeZoneFirstEnterPlayerID = nPlayerID; }

	//								���� ����
	VOID							MainLoop();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����					
	//
	//----------------------------------------------------------------------------------------------------------------
	//	������Ʈ
	//								Ŭ���̾�Ʈ ��û �м�/ó��
	VOID							ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID							ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//----------------------------------------------------------------------------------------------------------------
	//	��
	//								Ŭ���̾�Ʈ ��û �м�/ó��
	VOID							SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//----------------------------------------------------------------------------------------------------------------

	//								����Ʈ �� ����
	VOID							SRV_EnterDateZone(CPlayer* pPlayer, DWORD nTargetPlayerID, int nZoneIndex);
	//								..������Ʈ�� �ٸ� �÷��̾��� ���
	VOID							SRV_EnterDateZone2(CPlayer* pPlayer, MSG_DWORD4* pPacket);
	//								ç���� �� ����
	VOID							SRV_EnterChallengeZone(CPlayer* pPlayer, DWORD nTargetPlayerID, int nZoneIndex, int nKind = 0);
	//								..������Ʈ�� �ٸ� �÷��̾��� ���
	VOID							SRV_EnterChallengeZone2(CPlayer* pPlayer, MSG_DWORD4* pPacket, int nKind = 0);

	//								ç���� �� ���� ���μ���
	VOID							SRV_Process(CPlayer* pPlayer);
	//								ç���� �� ����
	VOID							SRV_StartChallengeZone(CPlayer* pPlayer, UINT nChannelNumFromAgent, UINT nSection);
	//								ç���� �� ���� �˸�
	VOID							SRV_NotifyStartChallengeZone(CPlayer* pPlayer, char* pszName1, char* pszName2, DWORD nLeastClearTime, UINT nSection);

	//								ç���� �� ���� ȸ�� ó��
	VOID							SRV_ProcChallengeZoneEnterFreq(CPlayer* pPlayer);
	//								ç���� �� ���� ȸ�� ����
	VOID							SRV_SendChallengeZoneEnterFreq(CPlayer* pPlayer);
	//								ç���� �� ���� ���� ó��
	VOID							SRV_ProcRegenMonster(CPlayer* pPlayer);
	//								ç���� �� ���� ���� ó��
	VOID							SRV_ProcMonsterDie(CPlayer* pPlayer);
	//								ç���� �� ���� ���ʽ� ȸ�� ����
	VOID							SRV_SetChallengeZoneEnterBonusFreq(CPlayer* pPlayer, UINT nFreq);

	//								ç���� ���� ����
	VOID							SRV_EndChallengeZone(CPlayer* pPlayer, CHALLENGEZONE_STATE eEndState);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ
	//
	//----------------------------------------------------------------------------------------------------------------
	//								����Ʈ �� ������ �����Ѱ�?
	BOOL							CLI_IsPossibleEnterDateZone();
	//								ç���� �� ������ �����Ѱ�?
	BOOL							CLI_IsPossibleEnterChallengeZone();

	//								����Ʈ �� ���� ��û
	VOID							CLI_RequestDateMatchingEnterDateZone(int nZoneIndex);
	//								ç���� �� ���� ��û
	VOID							CLI_RequestDateMatchingEnterChallengeZone(int nZoneIndex);

	//----------------------------------------------------------------------------------------------------------------
	//								�����κ����� ���� �м�/ó��
	VOID							CLI_ParseAnswerFromSrv(void* pMsg);
};

extern CSHDateManager g_csDateManager;
