#pragma once
#include "stdafx.h"

class CPlayer;

#define FISHINGMGR USINGTON(CFishingManager)
#define FISHINGRATE_SCALE		1000.0f							// �Ҽ��� ���� 3�ڸ����� ��ȿ.

#define MAX_FISHINGPLACE		10
#define MAX_FISHITEM			10
#define FISHINGTIME_LATENCY		500								// ��Ʈ�������ð�

enum {
	eFISHINGRATE_ITEM_UNKNOWN	= 0,
	eFISHINGRATE_ITEM_UTILITY	= 1,
	eFISHINGRATE_ITEM_BAIT		= 2,
};

enum {
	eFISHMISSION_STATE_READY	= 0,	// �̼��ֱ���
	eFISHMISSION_STATE_STANDBY	= 1,	// �̼��ְ� ������
	eFISHMISSION_STATE_NEW		= 2,	// �̼��ֱ�
	eFISHMISSION_STATE_END		= 3,	// �̼ǳ�����
};

struct stFishingMissionInfo;

extern DWORD g_FishingMissionCode[MAX_FISHINGMISSIONCODE];		// ������ڵ�
extern std::map<DWORD, stFishingMissionInfo> g_mapMissionInfo;	// �̼�����

struct stFishItemInfo
{
	DWORD dwItemIndex;											// ������Index
	float fRate;												// Ȯ��
};

struct stFishRateInfo
{
	int   nGrade;												// �������
	float fRate;												// Ȯ��
};

struct stFishingMissionInfo
{
	DWORD dwCode;												// �̼��ڵ�
	DWORD dwRewardItem;											// ���������
	int   dwRewardItemNum;										// ��������� ����
};

struct stFishingPlaceInfo
{
	char  cNpcKind;												// NPCLIST.bin�� ����� NpcKind�ʵ�
	VECTOR3 vPos;												// ��ġ
	DWORD dwLifeTime;											// ���ӽð�.
	DWORD dwProcessTime;										// ������ ����ð�. (���̵�)
	int   nRepeatCount;											// ������ �ݺ�Ƚ��. (���̵�)
	stFishItemInfo			m_FishItem[MAX_FISHITEM];			// ���������
	std::vector<stFishItemInfo> m_vecPlaceEff;					// �����Ϳ� ���� ����ȿ��
	std::vector<stFishItemInfo> m_vecWeatherEff[eWS_Max];		// ������ ���� ����ȿ��
	DWORD dwBaseDelay;											// �����ֱ� �⺻
	DWORD dwRandDelay;											// �����ֱ� ����
	char  cFishItemNum;											// ���������� ��
	float fHitAddRate[4];										// [0]����, [1]�Ƕ��, [2]�ؾ�, [3]�׾� ; ��Ʈ�� ���߽� �߰�Ȯ��

	stFishingPlaceInfo()	{Clear();}
	void Clear()
	{
		cNpcKind=cFishItemNum = 0;
		vPos.x=vPos.y=vPos.z=0.0f;
		dwLifeTime=dwProcessTime = 0;
		nRepeatCount = 0;
		dwBaseDelay=dwRandDelay = 0;
		m_vecPlaceEff.clear();
		int i;
		for(i=0; i<eWS_Max; i++)
			m_vecWeatherEff[i].clear();

		for(i=0; i<4; i++)
			fHitAddRate[i] = 1.0f;
	}
};

struct stFishingPlaceInst
{
	BYTE  byInfoIndex;											// m_FishingPlaceInfo[]���� ������ index
	DWORD dwObjectIndex;										// g_pUserTable�� ��ϵ� index
	DWORD dwGenTime;											// �����ð�.
	DWORD dwDelTime;											// �Ҹ�ð�.

	stFishingPlaceInst()	{Clear();}
	void Clear()
	{
		byInfoIndex = 0;
		dwObjectIndex=dwGenTime=dwDelTime = 0;
	}
};

struct stFishingRate
{
	DWORD dwItemIndex;
	stFishRateInfo FishList[MAX_FISHITEM];

	stFishingRate()			{Clear();}
	void Clear()
	{
		dwItemIndex = 0;
		int i;
		for(i=0; i<MAX_FISHITEM; i++)
		{
			FishList[i].nGrade = -1;
			FishList[i].fRate = 0.0f;
		}
	}
};

class CFishingManager
{
public:
	CFishingManager(void);
	virtual ~CFishingManager(void);

	void Init();
	void NetworkMsgParse( BYTE Protocol, void* pMsg, DWORD dwLength );

	void Fishing_Ready_Syn(void* pMsg);
	void Fishing_GetFish_Syn(void* pMsg);
	void Fishing_Cancel_Syn(void* pMsg);
	void Fishing_FPChange_Syn(void* pMsg);

	void Process();

	BOOL				m_bActive;
	BOOL				m_bInit;
	BOOL				m_bUseMission;
	stFishingPlaceInfo	m_FishingPlaceInfo[MAX_FISHINGPLACE];
	stFishingPlaceInst	m_FishingPlaceInst[MAX_CHANNEL_NUM][MAX_FISHINGPLACE];
	CYHHashTable< stFishingPlaceInst*>	m_FishingGabagePlace;	//�� ������ �Ҹ�� ������Index �ӽ�����.

	BOOL AddFishingPlace(stFishingPlaceInfo* pFishingPlaceInfo, DWORD dwChannel, BYTE byInfoIndex);
	void DelFishingPlace(stFishingPlaceInst* pInst);
	void SendGetFish(CPlayer* pPlayer, WORD wResCode, void* pMsg=NULL);

	stFishingPlaceInst* GetFishingPlaceInst(DWORD dwPlaceIndex);
	
	void SetPlaceEffect();
	void SetWeatherEffect();

	DWORD GetItemIdxFromFM_Code(int nCode);	// ���ù̼� �ڵ�� ������Index���
	void GetFishCodeFromFM_Code(DWORD dwMissionCode, WORD* pFishCode);	// ���ù̼� �ڵ�� ������ڵ� ���
	void SendFishingMissionInfo(CPlayer* pPlayer, BOOL bNew);

	stFishingRate* GetFishingUtilityRate(DWORD dwItemIndex);
	stFishingRate* GetFishingBaitRate(DWORD dwItemIndex);


	BOOL GetActive() {return m_bActive;}
	char CheckMissionState();
	void ChangeMissionState();
	void ProcessPlayer(CPlayer* pPlayer);

private:
	int m_nChannelNum;
	int m_nFishingPlaceNum;

	int m_nPlaceEff[MAX_FISHINGPLACE][eFishItem_Max];
	int m_nWeatherEff[MAX_FISHINGPLACE][eWS_Max][eFishItem_Max];

	char  m_cMissionState;
	DWORD m_dwMissionSendTime;		// ���� ������ �̼Ǻ�������� ���ð� (Pulling ������ �ð�)
	DWORD m_dwMissionDelayTime;		// �̼Ǻο� ����
	DWORD m_dwMissionEndTime;		// �̼� �ϷḦ ��ٸ��� �ð�.

	std::map<DWORD, stFishingRate*> m_mapUtilRateList;
	std::map<DWORD, stFishingRate*> m_mapBaitRateList;
};

EXTERNGLOBALTON(CFishingManager)