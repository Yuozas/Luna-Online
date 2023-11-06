/*********************************************************************

	 ����		: SHFarmManager.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���� �Ŵ��� Ŭ������ ���

 *********************************************************************/

#pragma once

class CPlayer;
class CSHFamily;

#include "SHFarmZone.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHFarmManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum FARM_HARVEST_KIND																		// ���۹� ��Ȯ ����
	{								
		FARM_HARVEST_KIND_GREAT = 0,															// ..��ǳ
		FARM_HARVEST_KIND_GOOD,																	// ..ǳ��
		FARM_HARVEST_KIND_NORMAL,																// ..����
		FARM_HARVEST_KIND_BAD,																	// ..����
		FARM_HARVEST_KIND_WORST,																// ..����
		FARM_HARVEST_KIND_MAX,
	};

	enum FARM_ANIMAL_KIND
	{
		FARM_ANIMAL_KIND_GOOD = 0,
		FARM_ANIMAL_KIND_NORMAL,
		FARM_ANIMAL_KIND_BAD,
		FARM_ANIMAL_KIND_MAX
	};

	//----------------------------------------------------------------------------------------------------------------
	enum FARM_TIMEDELAY_KIND																	// Ÿ�� ������ ����
	{								
		FARM_TIMEDELAY_KIND_PLANT = 0,															// ..�ɱ�
		FARM_TIMEDELAY_KIND_MANURE,																// ..����ֱ�
		// 080430 KTH -- Animal Delay Add
		FARM_TIMEDELAY_KIND_BREED,
		FARM_TIMEDELAY_KIND_FEED,
		FARM_TIMEDELAY_KIND_CLEAN,
	};

	//----------------------------------------------------------------------------------------------------------------
	enum FARM_EFFECT_KIND																		// ����Ʈ ����
	{								
		FARM_EFFECT_KIND_CROP_PLANT = 0,														// ..���۹� �ɱ�
		FARM_EFFECT_KIND_CROP_MANURE,															// ..���۹� ����ֱ�
		FARM_EFFECT_KIND_CROP_STEPUP,															// ..���۹� ����
		FARM_EFFECT_KIND_CROP_HARVEST,															// ..���۹� ��Ȯ
		FARM_EFFECT_KIND_CROP_DIE,																// ..���۹� �Ҹ�
		FARM_EFFECT_KIND_UPGRADE_GARDEN,														// ..���׷��̵� �Թ�
		FARM_EFFECT_KIND_UPGRADE_FENCE,															// ..���׷��̵� ��Ÿ��
		FARM_EFFECT_KIND_UPGRADE_ANIMALCAGE,													// ..���׷��̵� ���
		FARM_EFFECT_KIND_UPGRADE_HOUSE,															// ..���׷��̵� ��
		FARM_EFFECT_KIND_UPGRADE_WAREHOUSE,														// ..���׷��̵� â��
		// 080312 KTH --��� ����
		FARM_EFFECT_KIND_ANIMAL_IN_STALL,															// ..���� ��翡 �ֱ�
		FARM_EFFECT_KIND_ANIMAL_FEED,															// ..���� ����ֱ�
		FARM_EFFECT_KIND_CLEANNING,																// ..��� û�� ����Ʈ
		FARM_EFFECT_KIND_ANIMAL_REWARD,														// ..���� ������ ���
		FARM_EFFECT_KIND_ANIMAL_DIE,															// ..���� �Ҹ�
		FARM_EFFECT_KIND_MAX,		
	};

	//----------------------------------------------------------------------------------------------------------------
	enum FARM_DEL_KIND																			// ���� ���� ����
	{								
		FARM_DEL_KIND_BREAKUP_FAMILY = 0,														// ..�йи� ��ü
		FARM_DEL_KIND_TAX,																		// ..������ �̳�
	};

	//----------------------------------------------------------------------------------------------------------------
	enum FARM_UI_NOTIFY_KIND																	// ���� UI �˸� ����
	{								
		FARM_UI_NOTIFY_KIND_NORMAL = 0,															// ..�Ϲ���
		FARM_UI_NOTIFY_KIND_CONN_MAP,															// ..�ʿ� ����
	};

	//----------------------------------------------------------------------------------------------------------------
	static const int		CHECK_DISTANCE;														// ���� ó�� ������ �÷��̾�� ������ �Ÿ� (���� ������Ʈ�� �׸��带 ������� ����)
	static const int		OBJECT_CONTROL_DISTANCE;											// ���� ������Ʈ ���� ���� �Ÿ�

	static const int		RENDER_SALEBOARD_KIND;												// ȭ�� ��� �����Ǹ� �� ���� ��ȣ
	static const int		RENDER_STEWARD_KIND;												// ȭ�� ��� ���� ���� ��ȣ
	static const int		RENDER_FENCE_KIND;													// ȭ�� ��� ��Ÿ�� ���� ��ȣ
	static const int		RENDER_ANIMALCAGE_KIND;												// ȭ�� ��� ��� ���� ��ȣ
	static const int		RENDER_HOUSE_KIND;													// ȭ�� ��� �� ���� ��ȣ
	static const int		RENDER_GARDENPATCH_KIND;											// ȭ�� ��� �Թ������ ���� ��ȣ
	static const int		RENDER_WAREHOUSE_KIND;												// ȭ�� ��� â�� ���� ��ȣ
	static const int		RENDER_CROP_KIND;													// ȭ�� ��� ���۹� ���� ��ȣ
	static const int		RENDER_BASIC_FENCE;													// ȭ�� ��� �⺻ ��Ÿ�� ��ȣ
	static const int		RENDER_BASIC_GARDENPATCH_KIND;										// ȭ�� ��� �⺻ �Թ������ ���� ��ȣ
	// 080307 KTH -- ���� �߰�
	//------------------------------------------------------------------------------------------------------------------------------------------------
	/*enum ANIMAL_BREED_KIND																			// ��� ��Ȯ
	{
		ANIMAL_BREED_KIND_GREAT,																	// ���� ����
		ANIMAL_BREED_KIND_GOOD,																		// ����
		ANIMAL_BREED_KIND_NORMAL,																	// �߰�
		ANIMAL_BREED_KIND_BAD,																		// ������ (���� ����)
		ANIMAL_BREED_KIND_MAX,
	};*/

	//static const int		RENDER_ANIMAL_CAGE_SLOT_KIND;										// ��� ����
	static const int		RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND;									// ��� ��� �⺻ ����
	static const int		RENDER_ANIMAL_KIND;													// ������ ���� ��ȣ
	static UINT				FARM_BREED_CHAR_MOTION;												// ���� ��翡 �ִ� ĳ���� ��� ��ȣ
	static UINT				FARM_FEED_CHAR_MOTION;												// ���� ����ֱ� ĳ���� ��� ��ȣ
	static UINT				FARM_REWARD_ANIMAL_CHAR_MOTION;										// ���� ��Ȯ�ϱ� ĳ���� ��� ��ȣ
	static UINT				FARM_BREED_ANIMAL_GRADE_DIFF;										// ������ ��翡 �ֱ� �Ұ����� ����� ������ �������
	static UINT				FARM_CLEANING_CHAR_MOTION;											// ������ �ı涧 ĳ���� ��� ��ȣ

	static UINT				FARM_ANIMAL_MOTION_01;												// ���� ��� 1
	static UINT				FARM_ANIMAL_MOTION_02;												// ���� ��� 2

	static int				RENDER_ANIMAL_KIND_NUM;												// ���� ���� ����
	static const int		PEN_OBJECT_CONTROL_DISTANCE;										// ��� ������Ʈ ���� ���� �Ÿ�
	//------------------------------------------------------------------------------------------------------------------------------------------------

	static int				RENDER_CROP_KIND_NUM;												// ���۹� ���� ����

	static UINT				FARM_BUY_MAX_NUM;													// �ִ�� ���� �� �ִ� ���� ����
	static UINT				FARM_BUY_FAMILY_MEMNUM;												// ������ �����ϱ� ���� �ʿ��� �йи� �ּ� �ο�
	static UINT				FARM_BUY_MASTER_LEVEL;												// ���� ���� �ݾ�
	static UINT				FARM_BUY_FAMILY_HONORPOINT;											// ���� ���� �йи� �� ����Ʈ
	static UINT				FARM_BUY_MONEY;														// ���� ���� �йи� ������ ����

	static UINT				FARM_PLANT_GARDEN_GRADE_DIFF;										// ���۹��� �ɱ� �Ұ����� ���۹��� �Թ���� ��� ����

	static UINT				FARM_TAX_GOLD;														// ���� ������ �ݾ�
	static UINT				FARM_TAX_PAY_WEEK_INTERVAL;											// ���� ������ ¡�� ���� ����
	static UINT				FARM_TAX_PAY_LIMIT_DAY;												// ���� ������ ¡�� ���� ���� (��:0  ��:1  ȭ:2  ��:3  ��:4  ��:5  ��:6)
	static UINT				FARM_TAX_PAY_LIMIT_HOUR;											// ���� ������ ¡�� ���� �ð� (00 ~ 23)
	static UINT				FARM_TAX_PAY_ARREARAGE_MAX_FREQ;									// ���� ������ ��ü ���� Ƚ��
	static UINT				FARM_TAX_PAY_ARREARAGE_ADD_RATE;									// ���� ������ ��ü�� ���� (%)
	static UINT				FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY;								// ���� ������ ������ �� �� ������ �̳��� �˸� ���ΰ�(-1:�� �˸��� ����) 

	static UINT				FARM_PLANT_CHAR_MOTION_NUM;											// ���۹� �ɱ� ĳ���� ��� ��ȣ
	static UINT				FARM_MANURE_CHAR_MOTION_NUM;										// ���۹� ����ֱ� ĳ���� ��� ��ȣ
	static UINT				FARM_HARVEST_CHAR_MOTION_NUM;										// ���۹� ��Ȯ�ϱ� ĳ���� ��� ��ȣ
	static UINT				FARM_CHAR_MOTION_PROGRESSBAR_TYPE;									// ĳ���� ��� ���α׷����� Ÿ�� ��ȣ
	static UINT				FARM_CHAR_MOTION_PROGRESSBAR_WIDTH;									// ĳ���� ��� ���α׷����� �ʺ�

	static int				FARM_EFFECT_NUM[FARM_EFFECT_KIND_MAX];								// ���� ������Ʈ ����Ʈ	

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_STATE : public MSGBASE													// ���� ���� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nFarmState;															// ..���� ����
		DWORD				nFarmOwner;															// ..���� ������
		WORD				nGardenGrade;														// ..�Թ� ���
		WORD				nFenceGrade;														// ..��Ÿ�� ���
		WORD				nAnimalCageGrade;													// ..��� ���
		WORD				nHouseGrade;														// ..�� ���
		WORD				nWarehouseGrade;													// ..â�� ���
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_STATE_WITH_TAX : public PACKET_FARM_STATE								// ���� ���� ��Ŷ ���� (������ �߰�)
	{
		WORD				nTaxArrearageFreq;													// ..������ ��ü Ƚ��
		char				szTaxPayPlayerName[MAX_NAME_LENGTH+1];								// ..������ ���� �÷��̾�
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_GARDEN_CROP_PLANT : public MSGBASE										// ���۹� �ɱ� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nGardenID;															// ..�Թ� �ε���
		WORD				nCropID;															// ..���۹� �ε���
		DWORD				nCropOwner;															// ..���۹� ������
		WORD				nCropKind;															// ..���۹� ����
		WORD				nCropStep;															// ..���۹� �ܰ�
		WORD				nCropLife;															// ..���۹� ����
		WORD				nCropNextStepTime;													// ..���۹� ���� �ܰ� �ʿ� �ð�
		WORD				nCropSeedGrade;														// ..���� ���
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		DWORD				nOwner;																// ..���� ������
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_GARDEN_CROP_GROW : public MSGBASE										// ���۹� ���� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nGardenID;															// ..�Թ� �ε���
		WORD				nCropID;															// ..���۹� �ε���
		WORD				nValue;																// ..��ġ��
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_GARDEN_CROP_MANURE : public MSGBASE										// ���۹� ����ֱ� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nGardenID;															// ..�Թ� �ε���
		WORD				nCropID;															// ..���۹� �ε���
		WORD				nCropLife;															// ..���۹� ����
		WORD				nManureKind;														// ..��� ����
		WORD				nManureGrade;														// ..��� ���
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_UPGRADE : public MSGBASE													// ���� ��� ���׷��̵� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nObjKind;															// ..���׷��̵� ���
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stBFARM																				// ���� ��ũ��Ʈ ����
	{
		WORD nMapNum;																			// ..�� ��ȣ
		WORD nKind;																				// ..����
		char szName[MAX_NAME_LENGTH+1];															// ..�̸�
		WORD nID;																				// ..ID
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stFARMRENDEROBJLIST																	// ���� ������ ������Ʈ ����Ʈ ����
	{
		int		nID;																			// ..ID
		char	szGroupName[MAX_NPC_NAME_LENGTH+1];												// ..�׷��̸�
		char	szChxName[32];																	// ..chx ���� �̸�
		float	nScale;																			// ..������
		int		nNameYpos;																		// ..�׷��̸� ����
		BOOL	bShowGroupName;																	// ..�׷��̸� ǥ�� ����
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stFARMOBJNAME																		// ���� ������ ������Ʈ �̸� ����
	{
		char szName[64];
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stSelFarmObj																			// ������ ���۹� �ε���
	{
		int					nFarmZone;															// ..��������
		int					nFarmID;															// ..����
		int					nGardenID;															// ..�Թ�
		int					nCropID;															// ..���۹�
		// 080408 KTH
		int					nAnimalCageID;														// ..���
		int					nAnimalID;															// ..����
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stCROPHARVESTREWARD																	// ���۹� ��Ȯ ����
	{
		DWORD				nItemID[FARM_HARVEST_KIND_MAX];										// ..������ ID
		DWORD				nItemNum[FARM_HARVEST_KIND_MAX];									// ..������ ����
	};

	struct stANIMALREWARD
	{
		DWORD				nItemID[FARM_ANIMAL_KIND_MAX*3];
		DWORD				nItemNum[FARM_ANIMAL_KIND_MAX*3];
		DWORD				nPercent[FARM_ANIMAL_KIND_MAX*3];
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stUPGRADECOST																		// ���׷��̵� ���
	{
		UINT				nGold;																// ..���
		UINT				nFamilyHonorPoint;													// ..�йи� �� ����Ʈ
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT : public MSGBASE								// ���� �ֱ� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nAnimalCageID;														// ..��� �ε���
		WORD				nAnimalID;															// ..���� �ε���
		DWORD				nAnimalOwner;														// ..���۹� ������
		WORD				nAnimalKind;														// ..���۹� ����
		WORD				nAnimalStep;														// ..���۹� �ܰ�
		WORD				nAnimalLife;														// ..���۹� ����
		WORD				nAnimalNextStepTime;												// ..���۹� ���� �ܰ� �ʿ� �ð�
		WORD				nAnimalGrade;														// ..���� ���
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		DWORD				nOwner;																// ..���� ������
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
		WORD				nAnimalContentment;													// ..
		WORD				nAnimalInterest;
	};

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_CAGE_ANIMAL_GROW : public MSGBASE										// ���� ���� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nAnimalCageID;															// ..��� �ε���
		WORD				nAnimalID;															// ..���� �ε���
		WORD				nValue;																// ..��ġ��
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	struct PACKET_FARM_CAGE_ANIMAL_FEED : public MSGBASE										// ���� ����ֱ� ��Ŷ ����
	{
		WORD				nFarmZone;															// ..���� ����
		WORD				nFarmID;															// ..���� �ε���
		WORD				nAnimalCageID;														// ..��� �ε���
		WORD				nAnimalID;															// ..���� �ε���
		WORD				nAnimalLife;														// ..���� ����
		WORD				nAnimalContentment;													// ..���� ������
		WORD				nAnimalInterest;													// ..���� ���ɵ�
		WORD				nFoodKind;															// ..��� ����
		WORD				nItemTargetPos;														// ..������ ����
		DWORD				nItemIdx;															// ..������ ����
		WORD				nResult;															// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	struct PACKET_FARM_CAGE_ANIMAL_REWARD : public MSGBASE
	{
		WORD				nFarmZone;
		WORD				nFarmID;
		WORD				nAnimalCageID;
		WORD				nAnimalID;
		WORD				nResult;
	};


private:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarmZone*			m_pcsFarmZone[CSHFarmZone::FARM_ZONE_MAX];							// ���� ����

	//----------------------------------------------------------------------------------------------------------------
	CSHFarmRenderObj*		m_pcsSelFarmObj;													// �����Ϸ��� ������ ���� ������Ʈ
	stSelFarmObj			m_stSelFarmObj;

	stFARMOBJNAME*			m_pszTblFarmCropName;														// ���۹� �̸� ���̺�
	stCROPHARVESTREWARD*	m_pstCropHarvestReward;														// ���۹� ��Ȯ ����

	stBFARM*				m_pstBFarm;																	// ���� ��ũ��Ʈ ����
	stFARMRENDEROBJLIST*	m_pstFarmRenderObjList;														// ���� ������ ������Ʈ ����Ʈ

	stUPGRADECOST			m_stUpgradeCost[CSHFarm::FARM_EVENT_MAX][CSHFarmObj::MAX_GRADE-1];			// ���׷��̵� ���

	DWORD					m_nMotionID;																// ĳ���� ���ID

	UINT					m_nTaxCollectPastWeek;														// ������ ¡���ϰ� �帥 �������� Ƚ��

	// 080410 KTH
	stFARMOBJNAME*			m_pszTblFarmAnimalName;														// ���� �̸� ���̺�
	stANIMALREWARD*			m_pstAnimalReward;															// ���� ����

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarmManager();
	~CSHFarmManager();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//						���� ����
	VOID					MainLoop();														

	//						�ʱ�ȭ
	VOID					Init(int nMapNum);
	//						����
	VOID					Release();

	//						������ ���� �ε�
	BOOL					LoadFarmInfo(int nType);

	//----------------------------------------------------------------------------------------------------------------
	//						���� ���� ��ȣ ���
	BOOL					GetFarmZoneID(int nMapNum, int* pZone);
	//						���� ���� ���
	CSHFarmZone*			GetFarmZone(WORD eFarmZone) { return m_pcsFarmZone[eFarmZone]; }
	//						���� �� ��ȣ ���
	BOOL					GetFarmMapNum(UINT nFarmZone, int* pMapNum);
	//						������ ������Ʈ ���� ���
	int						GetRenderObjKind(int nID);
	//						���۹� �̸� ���
	char*					GetRenderObjNameFromTbl(int nKind, int nStep)	{ return m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(nKind-1)+nStep-CSHCrop::CROP_STEP_1].szName; }
	//						���׷��̵� ��� ���
	stUPGRADECOST*			GetUpgradeCost(CSHFarm::FARM_EVENT eObjKind, int nGrade)	{ return &m_stUpgradeCost[eObjKind-CSHFarm::FARM_EVENT_GARDEN][nGrade-1]; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//	������Ʈ
	//						Ŭ���̾�Ʈ ��û �м�/ó��
	VOID					ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID					ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//						�ʼ����� ����UI���� ��û
	VOID					ASRV_RequestFarmUIInfoToMap(DWORD nPlayerID, CSHFamily* pcsFamily);

	//----------------------------------------------------------------------------------------------------------------
	//	��
	//						Ŭ���̾�Ʈ ��û �м�/ó��
	VOID					SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//						�ֺ��� �ٸ� �÷��̾�鿡�� ���� ������ ������
	VOID					SRV_SendFarmInfoToNearPlayer(CSHFarm* pcsFarm, MSGBASE* pPacket, int nSize);
	//						�̺�Ʈ ó��
	VOID					SRV_ProcessEvent();

	//----------------------------------------------------------------------------------------------------------------
	//						���� ����
	VOID					SRV_BuyFarm(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, int nAgentChecked = 0);
	//						���۹� �ɱ�
	VOID					SRV_PlantCrop		(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx, int nAgentChecked = 0);
	//						���۹� ��� �ֱ�
	VOID					SRV_ManureCrop		(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx);
	//						���۹� ��Ȯ�ϱ�
	VOID					SRV_HarvestCrop		(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx, int nType);
	//						���� ��� ���׷��̵�
	VOID					SRV_UpgradeFarm		(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nObjKind, int nAgentChecked = 0);
	//						���� ������ ����
	VOID					SRV_PayTax			(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, DWORD nOwnerID, int nKind);
	//						���� ������ ¡�� ó��
	VOID					SRV_ProcTaxCollect	();
	//						���� ������ �̳� �˸�
	VOID					SRV_NotifyTaxNonpayment(DWORD nPlayerID, CSHFarm* pcsFarm);

	//						���۹� ����(�йи� Ż�� ����..)
	VOID					SRV_DelCrop(DWORD nOwnerID);
	//						���� ����
	VOID					SRV_DelFarm(DWORD nOwnerID, FARM_DEL_KIND eDelKind);
	//						���� ����
	VOID					SRV_DelFarmByFarmID(DWORD nOwnerID, WORD nFarmZone, WORD nFarmID, FARM_DEL_KIND eDelKind);

	//----------------------------------------------------------------------------------------------------------------
	//						DB�� ���� ������ ����
	VOID					SRV_SaveFarmInfoToDB(WORD nFarmZone, WORD nFarmID, CSHFarm* pcsFarm);
	//						DB�� ���۹� ������ ����
	VOID					SRV_SaveCropInfoToDB(WORD nFarmZone, WORD nFarmID, CSHCrop* pCrop);
	//						DB ť�� �����Ͽ� ���� ���� ����
	VOID					SRV_SetFarmStateFromDBQueue(CPlayer* pPlayer, PACKET_FARM_STATE_WITH_TAX* pPacket);
	//						DB ť�� �����Ͽ� ���۹� ����
	VOID					SRV_SetCropInfoFromDBQueue(CPlayer* pPlayer, PACKET_FARM_GARDEN_CROP_PLANT* pPacket);

	//----------------------------------------------------------------------------------------------------------------
	//						Ŭ���̾�Ʈ���� ���� ���� ������
	VOID					SRV_SendFarmInfoToClient(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID);
	//						Ŭ���̾�Ʈ�� ����UI���� ����
	VOID					SRV_SendFarmUIInfoToClient(DWORD nPlayerID, DWORD nFamilyMasterID, FARM_UI_NOTIFY_KIND eKind = FARM_UI_NOTIFY_KIND_NORMAL);
	//						Ŭ���̾�Ʈ�� ����UI ���۹� ���� ����
	VOID					SRV_SendFarmUIInfoToClient(WORD nFarmZone, WORD nFarmID, WORD nGardenID, CSHCrop* pCrop, DWORD nFarmOwner);
	//						Ŭ���̾�Ʈ�� ������ �� ���� ���� ����
	VOID					SRV_SendPlayerFarmInfo(CPlayer* pPlayer);

	//						DB�� ���� ������ ����
	VOID					SRV_SaveAnimalInfoToDB(WORD nFarmZone, WORD nFarmID, CAnimal* pAnimal);
	//						DB ť�� �����Ͽ� ���� ����
	VOID					SRV_SetAnimalInfoFromDBQueue(CPlayer* pPlayer, PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket);
	//						���� ����
	VOID					SRV_DelAnimal(DWORD nOwnerID);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ
	//
	//----------------------------------------------------------------------------------------------------------------
	//
	//						������ ������Ʈ ����Ʈ ���
	stFARMRENDEROBJLIST*	CLI_GetFarmRenderObjList(int nID);

	//						������ ���۹� ����
	int						CLI_SetTargetObj(CSHFarmRenderObj* pFarmRenderObj, BOOL bShowDlg = TRUE);
	CSHFarmRenderObj*		CLI_GetTargetObj()		{ return m_pcsSelFarmObj; }
	stSelFarmObj*			CLI_GetTargetObjID()	{ return &m_stSelFarmObj; }
	//						ȭ�� ����� ���� ���� ������Ʈ ����
	VOID					CLI_SetFarmObjForRender(CSHFarmRenderObj* pFarmRenderObj);

	//						ȭ�鿡 ������ ������Ʈ �߰�
	VOID					CLI_AddRenderFarmObjToScreen(CSHFarm* pFarm, int nTarget = 0);
	//						ȭ�鿡�� ������ ������Ʈ ����
	VOID					CLI_RemoveRenderFarmObjFromScreen(CSHFarm* pFarm, int nTarget = 0);
	//						ȭ�鿡 ������ ���۹� ������Ʈ �߰�
	VOID					CLI_AddRenderFarmCropObjToScreen(CSHCrop* pCrop);
	//						ȭ�鿡�� ������ ���۹� ������Ʈ ����
	VOID					CLI_RemoveRenderFarmCropObjToScreen(CSHCrop* pCrop);

	// 080417 KTH -- ȭ�鿡 ������ ���� ������Ʈ �߰�	------------------------------
	VOID					CLI_AddRenderFarmAnimalObjToScreen(CAnimal* pAnimal);
	// 080417 KTH -- ȭ�鿡 ������ ���� ������Ʈ ����
	VOID					CLI_RemoveRenderFarmAnimalObjToScreen(CAnimal* pAnimal);
	//--------------------------------------------------------------------------------

	//						���� ���׷��̵� ��ȭ â ó��
	BOOL					CLI_ProcUpgradeDlg(int nObjKind);
	//						���� ������ ���� ��ȭ â
	BOOL					CLI_ProcPayTaxDlg();

	//						ĳ���� ��� �� ����
	VOID					CLI_SetIngCharMotion(DWORD nMotionID)	{ m_nMotionID = nMotionID; }
	//						ĳ���� ��� �� ���
	DWORD					CLI_GetIngCharMotion()					{ return m_nMotionID; }
	
	//						ä�� ��ȯ�ϱ�
	VOID					CLI_ChangeChannel(BOOL bLogin);

	//----------------------------------------------------------------------------------------------------------------
	//						������ ���� ���� ��û
	VOID					CLI_RequestBuyFarmToSrv();
	VOID					CLI_RequestBuyFarmToSrv(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID);
	//						������ ���۹� �ɱ� ��û
	VOID					CLI_RequestPlantCropInGarden(WORD nItemTargetPos, DWORD nItemIdx);
	//						������ ���۹� ����ֱ� ��û
	VOID					CLI_RequestManureCropInGarden(WORD nItemTargetPos, DWORD nItemIdx);
	//						������ ���۹� ��Ȯ�ϱ� ��û
	VOID					CLI_RequestHarvestropInGarden(WORD nItemTargetPos, DWORD nItemIdx, WORD nType = RESULT_OK);
	//						������ ���� ������Ʈ ���׷��̵� ��û
	VOID					CLI_RequestUpgradeFarm(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, int nObjKind);
	//						������ ������ ���� ��û
	VOID					CLI_RequestPayTax(UINT nKind = 0);

	//						������ ���� ���� ��û
	VOID					CLI_RequestFarmInfo(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID);

	//----------------------------------------------------------------------------------------------------------------
	//						�����κ����� ���� �м�/ó��
	VOID					CLI_ParseAnswerFromSrv(void* pMsg);

	//----------------------------------------------------------------------------------------------------------------
	//						������ ���� �ֱ� ��û
	VOID					CLI_RequestInstallAnimalInAnimalCage(WORD nItemTargetPos, DWORD nItemIdx);
	//						���� ��翡 �ֱ�
	VOID					SRV_InstallAnimalInAnimalCage(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx, WORD nContentment, WORD nInterest, int nAgentChecked = 0);
	//						������ ���� ����ֱ� ��û
	VOID					CLI_RequestFeedAnimalInCage(WORD nItemTargetPos, DWORD nItemIdx);
	//						���࿡ �����ֱ�
	VOID					SRV_FeedAnimal(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx);
	//						���� û�� �ϱ� (map server)
	VOID					SRV_CleaningAnimal(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx);
	//						���� ��������
	CAnimal*				CLI_GetAnimalInfo(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID);
	//						���� û�� �ϱ� (client)
	VOID					CLI_RequestCleaningAnimalInCage(WORD nItemTargetPos, DWORD nItemIdx);
	//						���� ���� ������ �ޱ� (client)
	VOID					CLI_RequestAnimalGetItem(WORD nType = RESULT_OK);
	VOID					SRV_AnimalGetItem(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nType);
};

extern CSHFarmManager		g_csFarmManager;
