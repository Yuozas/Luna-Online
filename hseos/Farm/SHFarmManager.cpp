/*********************************************************************

	 파일		: SHFarmManager.cpp
	 작성자		: hseos
	 작성일		: 2007/04/10

	 파일설명	: 농장 매니저 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "../SHMain.h"
#include "../Common/SHMath.h"
#include "SHFarmManager.h"

#include "MHFile.h"

#if defined(_AGENTSERVER)
	#include "Network.h"
	#include "AgentDBMsgParser.h"
	#include "AgentNetworkMsgParser.h"
	#include "../Family/SHFamilyManager.h"
#elif defined(_MAPSERVER_)
	#include "Network.h"
	#include "MapDBMsgParser.h"
	#include "UserTable.h"
	#include "Player.h"
	#include "PackedData.h"
	#include "ItemManager.h"
	#include "..\[CC]Header\GameResourceStruct.h"
	#include "MHTimeManager.h"
#else
	#include "ObjectManager.h"
	#include "Object.h"
	#include "AppearanceManager.h"
	#include "GameIn.h"
	#include "MonsterGuageDlg.h"
	#include "ObjectBalloon.h"
	#include "OBalloonName.h"
	#include "ObjectGuagen.h"
	#include "ChatManager.h"
	#include "SHFarmBuyDlg.h"
	#include "MHMap.h"
	#include "NpcScriptDialog.h"
	#include "SHFarmUpgradeDlg.h"
	#include "SHFarmManageDlg.h"
	#include "../Monstermeter/SHMonstermeterDlg.h"
	#include "MiniMapDlg.h"
	#include "../Interface/cWindowManager.h"
	#include "../WindowIDEnum.h"	
	#include "../cMsgBox.h"
	#include "../Farm/SHFarmManageDlg.h"
	#include "MiniMapDlg.h"
	#include "BigMapDlg.h"
	#include "FarmAnimalDlg.h"
	#include "MoveManager.h"
#endif

const int CSHFarmManager::CHECK_DISTANCE					= 10000;					// 농장 처리 사항을 플레이어에게 적용할 거리 (농장 오브젝트는 그리드를 사용하지 않음)
const int CSHFarmManager::OBJECT_CONTROL_DISTANCE			= 100;						// 농장 오브젝트 조작 가능 거리
const int CSHFarmManager::PEN_OBJECT_CONTROL_DISTANCE		= 600;						// 가축 오브젝트 조작 가능 거리

const int CSHFarmManager::RENDER_SALEBOARD_KIND				= 801;						// 화면 출력 농장판매 팻말 종류 번호. NpcList.bin 에서 정의
const int CSHFarmManager::RENDER_STEWARD_KIND				= 802;						// 화면 출력 집사 종류 번호
const int CSHFarmManager::RENDER_FENCE_KIND					= 101;						// 화면 출력 울타리 종류 번호
const int CSHFarmManager::RENDER_ANIMALCAGE_KIND			= 301;						// 화면 출력 축사 종류 번호
const int CSHFarmManager::RENDER_HOUSE_KIND					= 201;						// 화면 출력 집 종류 번호
const int CSHFarmManager::RENDER_GARDENPATCH_KIND			= 501;						// 화면 출력 텃밭흙더미 종류 번호
const int CSHFarmManager::RENDER_WAREHOUSE_KIND				= 401;						// 화면 출력 창고 종류 번호
const int CSHFarmManager::RENDER_CROP_KIND					= 901;						// 화면 출력 농작물 종류 번호. 
const int CSHFarmManager::RENDER_BASIC_GARDENPATCH_KIND		= 601;						// 화면 출력 기본 텃밭흙더미 종류 번호
const int CSHFarmManager::RENDER_BASIC_FENCE				= 701;						// 화면 출력 기본 울타리 종류 번호

// 080307 KTH -- 가축 추가
//-----------------------------------------------------------------------------------------------------------------------------------------------------
const int CSHFarmManager::RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND	= 2001;						// 화면 출력 기본 가축 슬롯 번호
//const int CSHFarmManager::RENDER_ANIMAL_CAGE_SLOT_KIND			= 2001;						// 화면 출력 가축 슬롯 번호
const int CSHFarmManager::RENDER_ANIMAL_KIND					= 2501;						// 화면 출력 가축 번호
UINT	CSHFarmManager::FARM_BREED_CHAR_MOTION;												// 축사에 가축을 배치하는 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_FEED_CHAR_MOTION;												// 가축 사료주기 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_REWARD_ANIMAL_CHAR_MOTION;										// 축사에서 아이템 얻기 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_BREED_ANIMAL_GRADE_DIFF;										// 축사와 가축의 등급차이
UINT	CSHFarmManager::FARM_CLEANING_CHAR_MOTION;											// 가축을 씻길때 캐릭터 모션 번호
int		CSHFarmManager::RENDER_ANIMAL_KIND_NUM					= 3;

UINT	CSHFarmManager::FARM_ANIMAL_MOTION_01					= 0;
UINT	CSHFarmManager::FARM_ANIMAL_MOTION_02					= 1;
//-----------------------------------------------------------------------------------------------------------------------------------------------------

int		CSHFarmManager::RENDER_CROP_KIND_NUM					= 0;						// 농작물 종류 개수

UINT	CSHFarmManager::FARM_BUY_MAX_NUM;												// 최대로 가질 수 있는 농장 개수
UINT	CSHFarmManager::FARM_BUY_FAMILY_MEMNUM;											// 농장을 구입하기 위해 필요한 패밀리 최소 인원
UINT	CSHFarmManager::FARM_BUY_MASTER_LEVEL;											// 농장 구입 금액
UINT	CSHFarmManager::FARM_BUY_FAMILY_HONORPOINT;										// 농장 구입 패밀리 명성 포인트
UINT	CSHFarmManager::FARM_BUY_MONEY;													// 농장 구입 패밀리 마스터 레벨

UINT	CSHFarmManager::FARM_TAX_GOLD;													// 농장 관리비 금액
UINT	CSHFarmManager::FARM_TAX_PAY_WEEK_INTERVAL;										// 농장 관리비 징수 주일 간격
UINT	CSHFarmManager::FARM_TAX_PAY_LIMIT_DAY;											// 농장 관리비 징수 기한 요일 (일:0  월:1  화:2  수:3  목:4  금:5  토:6)
UINT	CSHFarmManager::FARM_TAX_PAY_LIMIT_HOUR;										// 농장 관리비 징수 기한 시간 (00 ~ 23)
UINT	CSHFarmManager::FARM_TAX_PAY_ARREARAGE_MAX_FREQ;								// 농장 관리비 연체 가능 횟수
UINT	CSHFarmManager::FARM_TAX_PAY_ARREARAGE_ADD_RATE;								// 농장 관리비 연체료 비율 (%)
UINT	CSHFarmManager::FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY;							// 농장 관리비 기한일 몇 일 전부터 미납을 알릴 것인가(-1:은 알리지 않음) 

UINT	CSHFarmManager::FARM_PLANT_CHAR_MOTION_NUM;										// 농작물 심기 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_MANURE_CHAR_MOTION_NUM;									// 농작물 비료주기 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_HARVEST_CHAR_MOTION_NUM;									// 농작물 수확하기 캐릭터 모션 번호
UINT	CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_TYPE;								// 캐릭터 모션 프로그레스바 타입
UINT	CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_WIDTH;								// 캐릭터 모션 프로그레스바 너비

UINT	CSHFarmManager::FARM_PLANT_GARDEN_GRADE_DIFF;									// 농작물을 심기 불가능한 농작물과 텃밭과의 등급 차이

int		CSHFarmManager::FARM_EFFECT_NUM[FARM_EFFECT_KIND_MAX];							// 농장 오브젝트 이펙트	

CSHFarmManager	g_csFarmManager;

int g_tbl_farm[][7] = 
{
	{ CSHFarmZone::FARM_ZONE_ALKER, 2, 60, 1, 15, 1, 5 },				// 알케르 농장. 2번 맵, 농장 60개, 텃밭 1개, 텃밭 농작물 15개, 축사 1개, 가축 5개 (080308 KTH)
};

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarmManager Method																										  생성자
//
CSHFarmManager::CSHFarmManager()
{
	ZeroMemory(m_pcsFarmZone, sizeof(m_pcsFarmZone));

	m_pcsSelFarmObj = NULL;

	ZeroMemory(m_stUpgradeCost, sizeof(m_stUpgradeCost));

	m_nMotionID = NULL;

	m_pstBFarm = NULL;
	m_pstFarmRenderObjList= NULL;
	m_pszTblFarmCropName = NULL;
	m_pstCropHarvestReward = NULL;
	m_pstAnimalReward = NULL;

	ZeroMemory(FARM_EFFECT_NUM, sizeof(FARM_EFFECT_NUM));

	m_nTaxCollectPastWeek = 0;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFarmManager Method																										  파괴자
//
CSHFarmManager::~CSHFarmManager()
{
	// 농장 상태를 파일로 남긴다.
#if defined(_MAPSERVER_)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		char szFile[256];
		sprintf(szFile, "./Log/FARM_LOAD_END_%4d%02d%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
		FILE* fp;
		fp = fopen(szFile, "a+");

		sprintf(szFile, "./Log/FARMCROP_LOAD_END_%4d%02d%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
		FILE* fp2;
		fp2 = fopen(szFile, "a+");

		for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
		{
			if (m_pcsFarmZone[i] == NULL) continue;
			// 농장 지역
 			for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
			{
				// 농장
				CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);

				if (fp)
				{
					fprintf(fp, "ZONE:%d FARM:%d OWNSTATE:%d OWNER:%d GARDENGRD:%d HOUSEGRD:%d WAREHOUSEGRD:%d ANIMALCAGEGRD:%d FENCEGRD:%d\n",
						i,
						pFarm->GetID(),
						pFarm->GetOwnState(),
						pFarm->GetOwner(),
						pFarm->GetGarden(0)->GetGrade(),
						pFarm->GetHouse(0)->GetGrade(),
						pFarm->GetWarehouse(0)->GetGrade(),
						pFarm->GetAnimalCage(0)->GetGrade(),
						pFarm->GetFence(0)->GetGrade());
				}

 				for(int k=0; k<pFarm->GetGardenNum(); k++)
				{
					// 텃밭
					CSHGarden* pGarden = pFarm->GetGarden(k);
					for(int p=0; p<pGarden->GetCropNum(); p++)
					{
						// 농작물
						CSHCrop* pCrop = pGarden->GetCrop(p);
						if (fp2)
						{
							fprintf(fp2, "ZONE:%d FARM:%d CROP:%d OWNER:%d STEP:%d LIFE:%d SEEDGRD:%d KIND:%d NEXTSTEPTIME:%d\n",
								i,
								pFarm->GetID(),
								pCrop->GetID(),
								pCrop->GetOwner(),
								pCrop->GetStep(),
								pCrop->GetLife(),
								pCrop->GetSeedGrade(),
								pCrop->GetKind(),
								pCrop->GetNextStepTime());
						}
					}
				}
				
				for(int k=0; k<pFarm->GetAnimalCageNum(); k++)
				{
					CPen* pPen = pFarm->GetAnimalCage(k);

					for( int p=0; p<pPen->GetAnimalNum(); p++ )
					{
						CAnimal* pAnimal = pPen->GetAnimal(p);
						if(fp2)
						{
							fprintf(fp2, "ZONE:%d PEN:%d ANIMAL:%d OWNER:%d STEP:%d LIFE:%d SEEDGRD:%d KIND:%d NEXTSTEPTIME:%d\n",
								i,
								pPen->GetID(),
								pAnimal->GetID(),
								pAnimal->GetOwner(),
								pAnimal->GetStep(),
								pAnimal->GetLife(),
								pAnimal->GetGrade(),
								pAnimal->GetKind(),
								pAnimal->GetNextStepTime());
						}
					}
				}
			}
		}							

		fprintf(fp2, "\n\n");
		fprintf(fp, "\n\n");

		fclose(fp2);
		fclose(fp);
	}
#endif

	Release();
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  Init Method																													  초기화
//
VOID CSHFarmManager::Init(int nMapNum)
{
	// 변수 초기화
  	m_pcsSelFarmObj = NULL;
	m_nMotionID = NULL;

	// 자원 해제
	Release();

	// 데이터 읽기
	LoadFarmInfo(0);

	// 맵 정보 생성
	// ..해당 맵이 아니면 건너뛰기
	if (g_tbl_farm[0][1] != nMapNum) return;

	// ..랜더링 오브젝트 초기화
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	OBJECTMGR->GetFarmRenderObjPool()->Init(MAX_FARM_RENDER_OBJ_NUM, 50, "CSHFarmRenderObj");
#endif

	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 농장 생성 (농장 지역에 속하는)
		m_pcsFarmZone[i] = new CSHFarmZone((CSHFarmZone::FARM_ZONE)g_tbl_farm[i][0], g_tbl_farm[i][1], g_tbl_farm[i][2]);
		m_pcsFarmZone[i]->SetID(i);
		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			// 텃밭 생성 (농장에 속하는)
			CSHFarm* pcsFarm = m_pcsFarmZone[i]->GetFarm(j);
			pcsFarm->CreateGarden(g_tbl_farm[i][3]);
			for(int k=0; k<pcsFarm->GetGardenNum(); k++)
			{
				// 농작물 생성 (텃밭에 속하는)
				CSHGarden* pcsGarden = pcsFarm->GetGarden(k);
				pcsGarden->Create(g_tbl_farm[i][4]);
			}
			// 울타리 생성
			pcsFarm->CreateFence(1);
			// 축사 생성
			pcsFarm->CreateAnimalCage(g_tbl_farm[i][5]);
			for(int k=0; k<pcsFarm->GetAnimalCageNum(); k++)
			{
				CPen* pcsAnimalCage = pcsFarm->GetAnimalCage(k);
				pcsAnimalCage->Create(g_tbl_farm[i][6]);
			}
			// 집 생성
			pcsFarm->CreateHouse(1);
			// 창고 생성
			pcsFarm->CreateWarehouse(1);
		}
	}

	// 데이터 읽기
	LoadFarmInfo(1);

	// 파일에 기록된 농작물 데이터를 정리한다. DB에서 받아올 떄와 비교하기 위해서
#if defined(_MAPSERVER_)
	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		if (m_pcsFarmZone[i] == NULL) continue;
		// 농장 지역
 		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			// 농장
			CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);
			for(int k=0; k<pFarm->GetGardenNum(); k++)
			{
				// 텃밭
				CSHGarden* pGarden = pFarm->GetGarden(k);

				for(int p=0; p<pGarden->GetCropNum(); p++)
				{
					// 농작물
					CSHCrop* pCrop = pGarden->GetCrop(p);

					char szFile[256];
					sprintf(szFile, "./Log/FARMCROP_VERIFY_%02d%02d%02d.txt", i, j, pCrop->GetID());
					FILE* fp = fopen(szFile, "rt");
					if (fp)
					{
						fclose(fp);

						sprintf(szFile, "./Log/FARMCROP_VERIFY_CHECK_%02d%02d%02d.txt", i, j, pCrop->GetID());
						FILE *fp = fopen(szFile, "wt");
						if (fp)
						{
							fprintf(fp, "DATA_FROM_FILE NOT DB!!!!\n");
							fclose(fp);
						}
					}
					else
					{
						sprintf(szFile, "./Log/FARMCROP_VERIFY_CHECK_%02d%02d%02d.txt", i, j, pCrop->GetID());
						DeleteFile(szFile);
					}
				}

				// 축사
				CPen* pPen = pFarm->GetAnimalCage(k);

				for( int p = 0; p < pPen->GetAnimalNum(); p++)
				{
					CAnimal* pAnimal = pPen->GetAnimal(p);

					char szFile[256];
					memset(szFile, NULL, sizeof(szFile));
					sprintf(szFile, "./Log/FARMANIMAL_VERIFY_%02d%02d%02d.txt", i, j, pAnimal->GetID());
					FILE* fp = fopen(szFile, "rt");
					if(fp)
					{
						fclose(fp);

						sprintf(szFile, "./Log/FARMANIMAL_VERIFY_CHECK_%02d%02d%02d.txt", i, j, pAnimal->GetID());
						FILE *fp = fopen(szFile, "wt");
						if(fp)
						{
							fprintf(fp, "DATA_FROM_FILE NOT DB!!!!\n");
							fclose(fp);
						}
					}
					else
					{
						sprintf(szFile, "./Log/FARMANIMAL_VERIFY_CHECK_%02d%02d%02d.txt", i, j, pAnimal->GetID());
						DeleteFile(szFile);
					}
				}
			}
		}
	}							
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  Release Method																													해제
//
VOID CSHFarmManager::Release()
{
	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		SAFE_DELETE(m_pcsFarmZone[i]);
	}

 	SAFE_DELETE_ARRAY(m_pstBFarm);
	SAFE_DELETE_ARRAY(m_pstFarmRenderObjList);
	SAFE_DELETE_ARRAY(m_pszTblFarmCropName);
	SAFE_DELETE_ARRAY(m_pstCropHarvestReward);
	SAFE_DELETE_ARRAY(m_pstAnimalReward);
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadFarmInfo Method																									데이터 파일 로드
//
BOOL CSHFarmManager::LoadFarmInfo(int nType)
{
#if !defined(_AGENTSERVER)
	// 농장 수치 데이터 읽기
 	char szFile[MAX_PATH];
	char szLine[MAX_PATH];
	int	 nKind = 0, nCnt = 0;
	CMHFile fp;

	if (nType == 0)
	{
		// 080410 KTH -------------------------------------------------------------------------------------------
		// 가축 이름 bin 파일 읽기 bin 파일을 따로 두지 않고 Farm.bin 파일에서 불러 오게 변경 되었음.

		/*sprintf(szFile, "./System/Resource/FarmAnimalObjName.bin");
		fp.init(szFile, "rb");

		if(!fp.IsInited())
		{
			char szTmp[256];
			sprintf(szTmp, "%s 파일이 존재하지 안ㅅ습니다.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if( strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;
			}
			nCnt++;
		}
		fp.Release();

		RENDER_ANIMAL_KIND = nCnt / CAnimal::ANIMAL_STEP_MAX;

		SAFE_DELETE_ARRAY(m_pszTblFarmAnimalName);
		m_pszTblFarmAnimalName = new stFARMOBJNAME[nCnt];

		fp.Init(szFile, "rb");
		if(!fp.IsInited()) return FALSE;
		for(int i=0; i<nCnt; i++)
		{
			if (fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				i--;
				continue;			
			}

			SafeStrCpy(m_pszTblFarmAnimalName[i].szName, szLine, sizeof(m_pszTblFarmAnimalName[i].szName));
		}
		fp.Release();*/

		// ------------------------------------------------------------------------------------------------------

		// 농작물 이름 bin 파일 읽기
		sprintf(szFile, "./System/Resource/FarmRenderObjName.bin");
		fp.Init(szFile, "rb");
		if(!fp.IsInited())
		{
			char szTmp[256];
			sprintf(szTmp, "%s 파일이 존재하지 않습니다.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		// ..먼저 개수 체크
		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}

			nCnt++;
		}
		fp.Release();

		RENDER_CROP_KIND_NUM = nCnt / CSHCrop::CROP_STEP_MAX;

		SAFE_DELETE_ARRAY(m_pszTblFarmCropName);
		m_pszTblFarmCropName = new stFARMOBJNAME[nCnt];

		// ..데이터 읽기
		fp.Init(szFile, "rb");
		if(!fp.IsInited()) return FALSE;
		for(int i=0; i<nCnt; i++)
		{
			if (fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				i--;
				continue;			
			}

			SafeStrCpy(m_pszTblFarmCropName[i].szName, szLine, sizeof(m_pszTblFarmCropName[i].szName));
		}

		fp.Release();

		SAFE_DELETE_ARRAY(m_pstCropHarvestReward);
		m_pstCropHarvestReward = new stCROPHARVESTREWARD[RENDER_CROP_KIND_NUM];
		ZeroMemory(m_pstCropHarvestReward, sizeof(stCROPHARVESTREWARD)*RENDER_CROP_KIND_NUM);

		SAFE_DELETE_ARRAY(m_pstAnimalReward);
		m_pstAnimalReward = new stANIMALREWARD[FARM_ANIMAL_KIND_MAX];
		ZeroMemory(m_pstAnimalReward, sizeof(stANIMALREWARD)*FARM_ANIMAL_KIND_MAX);

		// 각종 수치 정보 읽기
		sprintf(szFile, "./System/Resource/Farm.bin");
		fp.Init(szFile, "rb");
		if(!fp.IsInited())
		{
			char szTmp[256];
			sprintf(szTmp, "%s 파일이 존재하지 않습니다.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		BOOL bVerify = FALSE;
		while(TRUE)
		{
			if (fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}
			else if (strstr(szLine, "END_KIND")) 
			{
			// 데이터 검증
			#if defined(_DEBUG)
				if (bVerify && nCnt != RENDER_CROP_KIND_NUM)
				{
					char szTmp[256];
					sprintf(szTmp, "농작물의 이름 개수와 농작물의 수확 시 생성 아이템 개수가 다릅니다.\n데이터 파일을 확인해 주세요.[%s]", szFile);
					ASSERTMSG(0, szTmp);
				}
			#endif

				nCnt = 0;
				nKind++;
				bVerify = FALSE;
				continue;
			}

			switch(nKind)
			{
			case 0:
//				sscanf(szLine, "%d %d %d %d %d",	&FARM_PLANT_CHAR_MOTION_NUM,
				sscanf(szLine, "%d %d %d %d %d %d %d %d %d",	&FARM_PLANT_CHAR_MOTION_NUM,
													&FARM_MANURE_CHAR_MOTION_NUM,
													&FARM_HARVEST_CHAR_MOTION_NUM,
													&FARM_CHAR_MOTION_PROGRESSBAR_TYPE,
													&FARM_CHAR_MOTION_PROGRESSBAR_WIDTH,
													&FARM_BREED_CHAR_MOTION,
													&FARM_FEED_CHAR_MOTION,
													&FARM_REWARD_ANIMAL_CHAR_MOTION,
													&FARM_CLEANING_CHAR_MOTION);
				break;
			case 1:		
				sscanf(szLine, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",	&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_PLANT],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_MANURE],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP],		
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_HARVEST],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_DIE],	
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_GARDEN],	
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_FENCE],	
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_ANIMALCAGE],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_HOUSE],	
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_WAREHOUSE],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_IN_STALL],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_FEED],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_CLEANNING],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_REWARD],
																				&FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_DIE]);
				break;
			case 2:		
				sscanf(szLine, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",	&CSHCrop::CROP_LIFE_DECREASE_TIME,
																		&CSHCrop::CROP_PLANT_SEEDING_TIME,	
																		&CSHCrop::CROP_PLANT_RETRY_TIME,
																		&CSHCrop::CROP_MANURE_TIME,		
																		&CSHCrop::CROP_MANURE_RETRY_TIME,
																		&CSHCrop::CROP_HARVEST_TIME,
																		&CAnimal::ANIMAL_LIFE_DECREASE_TIME,
																		&CAnimal::ANIMAL_IN_STALL_TIME,
																		&CAnimal::ANIMAL_RETRY_TIME,
																		&CAnimal::ANIMAL_FEED_TIME,
																		&CAnimal::ANIMAL_FEED_DELAY_TIME,
																		&CAnimal::ANIMAL_STALL_REWARD_TIME,
																		&CAnimal::ANIMAL_CONTENTMENT_DECREASE_TIME,
																		&CAnimal::ANIMAL_INTEREST_DECREASE_TIME,
																		&CAnimal::ANIMAL_CLEANING_TIME,
																		&CAnimal::ANIMAL_CLEANING_RETRY_TIME);
				break;
			case 3:
				sscanf(szLine, "%d %d",				&FARM_PLANT_GARDEN_GRADE_DIFF,
													&FARM_BREED_ANIMAL_GRADE_DIFF);
				break;
			case 4:
				sscanf(szLine, "%d %d %d %d %d",	&FARM_BUY_MAX_NUM,		
													&FARM_BUY_FAMILY_MEMNUM,
													&FARM_BUY_MASTER_LEVEL,			
													&FARM_BUY_FAMILY_HONORPOINT,
													&FARM_BUY_MONEY);
				break;
			case 5:
				sscanf(szLine, "%d %d %d %d %d %d %d",	&FARM_TAX_GOLD,					
														&FARM_TAX_PAY_WEEK_INTERVAL,		
														&FARM_TAX_PAY_LIMIT_DAY,			
														&FARM_TAX_PAY_LIMIT_HOUR,		
														&FARM_TAX_PAY_ARREARAGE_MAX_FREQ,
														&FARM_TAX_PAY_ARREARAGE_ADD_RATE,
														&FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY);
				break;

			case 6:
				sscanf(szLine, "%d %d %d",			&CSHCrop::CROP_NEXT_STEP_TIME[nCnt][0],
													&CSHCrop::CROP_NEXT_STEP_TIME[nCnt][1],
													&CSHCrop::CROP_NEXT_STEP_TIME[nCnt][2]);
				break;
			case 7:
				sscanf(szLine, "%d %d %d %d %d %d",	&CSHCrop::CROP_STEP_MIN_LIFE[nCnt][0], &CSHCrop::CROP_STEP_MAX_LIFE[nCnt][0],
													&CSHCrop::CROP_STEP_MIN_LIFE[nCnt][1], &CSHCrop::CROP_STEP_MAX_LIFE[nCnt][1],
													&CSHCrop::CROP_STEP_MIN_LIFE[nCnt][2], &CSHCrop::CROP_STEP_MAX_LIFE[nCnt][2]);
				break;
			case 8:
				sscanf(szLine, "%d %d %d %d %d %d",	&CSHCrop::CROP_STEP_MANURE_MIN_LIFE[nCnt][0], &CSHCrop::CROP_STEP_MANURE_MAX_LIFE[nCnt][0],
													&CSHCrop::CROP_STEP_MANURE_MIN_LIFE[nCnt][1], &CSHCrop::CROP_STEP_MANURE_MAX_LIFE[nCnt][1],
													&CSHCrop::CROP_STEP_MANURE_MIN_LIFE[nCnt][2], &CSHCrop::CROP_STEP_MANURE_MAX_LIFE[nCnt][2]);
				break;
			case 9:
				sscanf(szLine, "%d %d %d %d %d %d %d",	&CSHCrop::CROP_HARVEST_PROBA[nCnt][0], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][1], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][2], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][3], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][4], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][5], 
														&CSHCrop::CROP_HARVEST_PROBA[nCnt][6]);
				break;
			case 10:
				sscanf(szLine, "%d %d %d %d %d",	&CSHCrop::CROP_HARVEST_DELTA_PROBA[nCnt][0],
													&CSHCrop::CROP_HARVEST_DELTA_PROBA[nCnt][1],
													&CSHCrop::CROP_HARVEST_DELTA_PROBA[nCnt][2],
													&CSHCrop::CROP_HARVEST_DELTA_PROBA[nCnt][3],
													&CSHCrop::CROP_HARVEST_DELTA_PROBA[nCnt][4]);
				break;
			case 11:
				sscanf(szLine, "%d %d",				&m_stUpgradeCost[CSHFarm::FARM_EVENT_GARDEN-1][nCnt].nGold,
													&m_stUpgradeCost[CSHFarm::FARM_EVENT_GARDEN-1][nCnt].nFamilyHonorPoint);
				break;
			case 12:
				sscanf(szLine, "%d %d",				&m_stUpgradeCost[CSHFarm::FARM_EVENT_FENCE-1][nCnt].nGold,
													&m_stUpgradeCost[CSHFarm::FARM_EVENT_FENCE-1][nCnt].nFamilyHonorPoint);
				break;
			case 13:
				sscanf(szLine, "%d %d",				&m_stUpgradeCost[CSHFarm::FARM_EVENT_HOUSE-1][nCnt].nGold,
													&m_stUpgradeCost[CSHFarm::FARM_EVENT_HOUSE-1][nCnt].nFamilyHonorPoint);
				break;
			case 14:
				sscanf(szLine, "%d %d",				&m_stUpgradeCost[CSHFarm::FARM_EVENT_WAREHOUSE-1][nCnt].nGold,
													&m_stUpgradeCost[CSHFarm::FARM_EVENT_WAREHOUSE-1][nCnt].nFamilyHonorPoint);
				break;
			case 15:
				sscanf(szLine, "%d %d",				&m_stUpgradeCost[CSHFarm::FARM_EVENT_ANIMALCAGE-1][nCnt].nGold,
													&m_stUpgradeCost[CSHFarm::FARM_EVENT_ANIMALCAGE-1][nCnt].nFamilyHonorPoint);
				break;
			case 16:
				
				sscanf(szLine, "%u %u %u %u %u %u %u %u %u %u",	&m_pstCropHarvestReward[nCnt].nItemID[0], &m_pstCropHarvestReward[nCnt].nItemNum[0],
																&m_pstCropHarvestReward[nCnt].nItemID[1], &m_pstCropHarvestReward[nCnt].nItemNum[1],
																&m_pstCropHarvestReward[nCnt].nItemID[2], &m_pstCropHarvestReward[nCnt].nItemNum[2],
																&m_pstCropHarvestReward[nCnt].nItemID[3], &m_pstCropHarvestReward[nCnt].nItemNum[3],
																&m_pstCropHarvestReward[nCnt].nItemID[4], &m_pstCropHarvestReward[nCnt].nItemNum[4]);
				bVerify = TRUE;
				break;
			// 축사등급에 따른 기본 만족도와 관심도
			case 17:
				sscanf(szLine, "%u %u",	&CAnimal::ANIMAL_STEP_MIN_LIFE[nCnt][0], &CAnimal::ANIMAL_STEP_MIN_LIFE[nCnt][1]);
				break;
			// 가축 먹이 관련 증가량.
			case 18:
				sscanf(szLine, "%d %d",	&CAnimal::ANIMAL_STEP_FEED_LIFE[nCnt][0], &CAnimal::ANIMAL_STEP_FEED_LIFE[nCnt][1]);
				break;
			// 가축 보상 테이블
			case 19:
				sscanf(szLine,"%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",	&m_pstAnimalReward[nCnt].nItemID[0], &m_pstAnimalReward[nCnt].nItemNum[0], &m_pstAnimalReward[nCnt].nPercent[0],
															&m_pstAnimalReward[nCnt].nItemID[1], &m_pstAnimalReward[nCnt].nItemNum[1], &m_pstAnimalReward[nCnt].nPercent[1],
															&m_pstAnimalReward[nCnt].nItemID[2], &m_pstAnimalReward[nCnt].nItemNum[2], &m_pstAnimalReward[nCnt].nPercent[2],
															&m_pstAnimalReward[nCnt].nItemID[3], &m_pstAnimalReward[nCnt].nItemNum[3], &m_pstAnimalReward[nCnt].nPercent[3],
															&m_pstAnimalReward[nCnt].nItemID[4], &m_pstAnimalReward[nCnt].nItemNum[4], &m_pstAnimalReward[nCnt].nPercent[4],
															&m_pstAnimalReward[nCnt].nItemID[5], &m_pstAnimalReward[nCnt].nItemNum[5], &m_pstAnimalReward[nCnt].nPercent[5],
															&m_pstAnimalReward[nCnt].nItemID[6], &m_pstAnimalReward[nCnt].nItemNum[6], &m_pstAnimalReward[nCnt].nPercent[6],
															&m_pstAnimalReward[nCnt].nItemID[7], &m_pstAnimalReward[nCnt].nItemNum[7], &m_pstAnimalReward[nCnt].nPercent[7],
															&m_pstAnimalReward[nCnt].nItemID[8], &m_pstAnimalReward[nCnt].nItemNum[8], &m_pstAnimalReward[nCnt].nPercent[8]
															);
				break;
			// 가축 이름 테이블
			case 20:
				SafeStrCpy(CAnimal::ANIMAL_NAME_TBL[nCnt], szLine, 256);
				//sscanf(szLine,"%s", CAnimal::ANIMAL_NAME_TBL[nCnt]);
				break;
			}

			nCnt++;
		}

		fp.Release();
	}
	else
	{
		// 농장오브젝트의 좌표만 읽기 위해..
		sprintf(szFile, "System/Resource/FarmRenderObjP.bin");
		fp.Init(szFile, "rb");
		if(fp.IsInited() == FALSE)
		{
			char szTmp[256];
			sprintf(szTmp, "%s 파일이 존재하지 않습니다.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		int nFarmID = 0, nFarmID2 = 0, nCropID = 0;
		// 080410 KTH 
		int nPenID = 0;
		VECTOR3 stPos;

		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, 256);

			if (strstr(szLine, "01_jakmul03_lv2.MOD"))
			{
				char szTmp[256];
				float nValue[13];

				sscanf(szLine, "%s %u %f %f %f %f %f %f %f %f %f %f %f", szTmp,	&nValue[1], &nValue[2], &nValue[3], &nValue[4],
																				&nValue[5], &nValue[6], &nValue[7], &nValue[8],
																				&nValue[9], &nValue[10], &nValue[11], &nValue[12]);
				stPos.x = nValue[5];
				stPos.y = 0;
				stPos.z = nValue[7];

				m_pcsFarmZone[0]->GetFarm(nCropID/15)->GetGarden(0)->GetCrop(nCropID%15)->SetPos(&stPos);
				m_pcsFarmZone[0]->GetFarm(nCropID/15)->GetGarden(0)->GetCrop(nCropID%15)->SetDir((float)CSHMath::GetRandomNum(1, 20000));
				nCropID++;
			}
			else if (strstr(szLine, "01_farm_fence_lv1.MOD"))
			{
				char szTmp[256];
				float nValue[13];

				sscanf(szLine, "%s %u %f %f %f %f %f %f %f %f %f %f %f", szTmp,	&nValue[1], &nValue[2], &nValue[3], &nValue[4],
																				&nValue[5], &nValue[6], &nValue[7], &nValue[8],
																				&nValue[9], &nValue[10], &nValue[11], &nValue[12]);
				stPos.x = nValue[5];
				stPos.y = 0;
				stPos.z = nValue[7];

				m_pcsFarmZone[0]->GetFarm(nFarmID)->SetPos(&stPos);
				m_pcsFarmZone[0]->GetFarm(nFarmID)->SetDir(nValue[11]);
				nFarmID++;
			}
			else if (strstr(szLine, "01_farm_housepost.MOD"))
			{
				char szTmp[256];
				float nValue[13];

				sscanf(szLine, "%s %u %f %f %f %f %f %f %f %f %f %f %f", szTmp,	&nValue[1], &nValue[2], &nValue[3], &nValue[4],
																				&nValue[5], &nValue[6], &nValue[7], &nValue[8],
																				&nValue[9], &nValue[10], &nValue[11], &nValue[12]);
				stPos.x = nValue[5];
				stPos.y = 0;
				stPos.z = nValue[7];

				m_pcsFarmZone[0]->GetFarm(nFarmID2)->GetFence(0)->SetPos(&stPos);
				m_pcsFarmZone[0]->GetFarm(nFarmID2)->GetFence(0)->SetDir(nValue[11]);
				nFarmID2++;
			}
			else if(strstr(szLine, "m030_lod00.MOD"))
			{
				char szTmp[256];
				float nValue[13];

				sscanf(szLine, "%s %u %f %f %f %f %f %f %f %f %f %f %f", szTmp,	&nValue[1], &nValue[2], &nValue[3], &nValue[4],
																				&nValue[5], &nValue[6], &nValue[7], &nValue[8],
																				&nValue[9], &nValue[10], &nValue[11], &nValue[12]);

				stPos.x = nValue[5];
				stPos.y = 0;
				stPos.z = nValue[7];

				m_pcsFarmZone[0]->GetFarm(nPenID/5)->GetAnimalCage(0)->GetAnimal(nPenID%5)->SetPos(&stPos);
				m_pcsFarmZone[0]->GetFarm(nPenID/5)->GetAnimalCage(0)->GetAnimal(nPenID%5)->SetDir(nValue[11]);
				nPenID++;
			}
		}

		fp.Release();

	#if !defined(_MAPSERVER_)
		// 렌더링 오브젝트 리스트 정보 읽기
		sprintf(szFile, "System/Resource/FarmRenderObjList.bin");
		fp.Init(szFile, "rb");
		if(fp.IsInited() == FALSE)
		{
			char szTmp[256];
			sprintf(szTmp, "%s 파일이 존재하지 않습니다.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		// ..먼저 개수 체크
		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}

			nCnt++;
		}

		fp.Release();

		SAFE_DELETE_ARRAY(m_pstFarmRenderObjList);
		m_pstFarmRenderObjList = new stFARMRENDEROBJLIST[nCnt];
		ZeroMemory(m_pstFarmRenderObjList, sizeof(stFARMRENDEROBJLIST)*nCnt);

		// ..데이터 읽기
		fp.Init(szFile, "rb");
		if(fp.IsInited() == FALSE)
			return FALSE;

 		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine), FALSE);
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}

			sscanf(szLine, "%d %s %s %f %d %d",	&m_pstFarmRenderObjList[nCnt].nID,
												m_pstFarmRenderObjList[nCnt].szGroupName,
												m_pstFarmRenderObjList[nCnt].szChxName,
												&m_pstFarmRenderObjList[nCnt].nScale,
												&m_pstFarmRenderObjList[nCnt].nNameYpos,
												&m_pstFarmRenderObjList[nCnt].bShowGroupName);
			char szTmp[256];
			_parsingKeywordString(m_pstFarmRenderObjList[nCnt].szGroupName, szTmp);
			lstrcpy(m_pstFarmRenderObjList[nCnt].szGroupName, szTmp);
			strcat(m_pstFarmRenderObjList[nCnt].szChxName, ".chx");
			nCnt++;
		}

   		fp.Release();

		// 데이터 검증
		#if defined(_DEBUG)
			int nCropNum = 0;
			for(int i=0; i<nCnt; i++)
			{
				if (GetRenderObjKind(m_pstFarmRenderObjList[i].nID) == RENDER_CROP_KIND)
					nCropNum++;
			}
			if (nCropNum != RENDER_CROP_KIND_NUM*CSHCrop::CROP_STEP_MAX)
			{
				char szTmp[256];
				sprintf(szTmp, "농작물의 이름 개수와 농작물 3D 그래픽 데이터 정보 개수가 다릅니다.\n데이터 파일을 확인해 주세요.[%s]", szFile);
				ASSERTMSG(0, szTmp);
			}

			for(int i=0; i<nCnt-1; i++)
			{
				for(int j=i+1; j<nCnt; j++)
				{
					if (m_pstFarmRenderObjList[i].nID == m_pstFarmRenderObjList[j].nID)
					{
						char szTmp[256];
						sprintf(szTmp, "농작물 3D 그래픽 데이터 파일의 정보 중 ID가 중복되는 항목이 있습니다.[ID:%d]\n데이터 파일을 확인해 주세요.[%s]", m_pstFarmRenderObjList[i].nID, szFile);
						ASSERTMSG(0, szTmp);
					}
				}
			}
		#endif


		// 렌더링 오브젝트 정보 읽기
		sprintf(szFile, "System/Resource/FarmRenderObj.bin");
		fp.Init(szFile, "rb");
		if(fp.IsInited() == FALSE)
			return FALSE;

		// ..먼저 개수 체크
		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}

			nCnt++;
		}

		fp.Release();

		SAFE_DELETE_ARRAY(m_pstBFarm);
		// nCnt 를 +1 해서 메모리 공간을 잡는다. 왜인지는 모르겠지만, nCnt 로 new 하면 delete 시 메모리 에러가 발생함.
		// 배열의 범위를 넘어서서 값을 쓴 것으로 처리됨. (DAMAGE: after Normal block ..... 하는 에러)
		// 마지막 값(m_pstBFarm[nCnt].nID)을 읽는 부분이 delete 시에 에러를 발생시켰음. 이 부분만 제거하면 에러 발생 안 함.
		// 이상한 것은 sscanf 를 fscanf 로 바꿔서 코드를 작성해서 테스트하면 문제 없음.
		// 왜 ???
		nCnt += 1;
		m_pstBFarm = new stBFARM[nCnt];
		ZeroMemory(m_pstBFarm, sizeof(stBFARM)*nCnt);

		// ..데이터 읽기
		fp.Init(szFile, "rb");
		if(fp.IsInited() == FALSE)
			return FALSE;

		nCnt = 0;
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine), FALSE);
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}

			sscanf(szLine, "%d %d %s %d",	&m_pstBFarm[nCnt].nMapNum,
											&m_pstBFarm[nCnt].nKind,
											m_pstBFarm[nCnt].szName,
											&m_pstBFarm[nCnt].nID);
			char szTmp[256];
			_parsingKeywordString(m_pstBFarm[nCnt].szName, szTmp);
			lstrcpy(m_pstBFarm[nCnt].szName, szTmp);

			nCnt++;
		}

		fp.Release();
	#endif
	}

#endif

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  GetFarmZoneID Method																							 농장 지역 번호 얻기
//
BOOL CSHFarmManager::GetFarmZoneID(int nMapNum, int* pZone)
{
	BOOL bResult = FALSE;

	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		if (g_tbl_farm[i][1] == nMapNum)
		{
			if (pZone) *pZone = i;
			bResult = TRUE;
		}
	}

	return bResult;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  GetFarmMapNum Method																							   농장 맵 번호 얻기
//
BOOL CSHFarmManager::GetFarmMapNum(UINT nFarmZone, int* pZone)
{
	if (nFarmZone < CSHFarmZone::FARM_ZONE_MAX)
	{
		if (pZone) *pZone = g_tbl_farm[nFarmZone][1];
		return TRUE;
	}

	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  MainLoop Method																											   메인 루프
//
VOID CSHFarmManager::MainLoop()
{
#if defined(_AGENTSERVER)
	int nMapNum = 0;
#elif defined(_MAPSERVER_)
	int nMapNum = g_pServerSystem->GetMapNum();
#else
 	int nMapNum = MAP->GetMapNum();
#endif

	// 현재 맵이 해당 농장의 맵이 아니면 건너뛰기
 	if (m_pcsFarmZone[0] == NULL || m_pcsFarmZone[0]->GetMapID() != nMapNum) return;

   	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		m_pcsFarmZone[i]->MainLoop();
	}

	// 이벤트 처리
	SRV_ProcessEvent();

	// 농장 관리비 처리
#if defined(_MAPSERVER_)
	SRV_ProcTaxCollect();
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  GetRenderObjKind Method																					   렌더링 오브젝트 종류 얻기
//
int CSHFarmManager::GetRenderObjKind(int nID)
{
	int nKind = 0;

	if (RENDER_SALEBOARD_KIND  == nID)
	{
		nKind = RENDER_SALEBOARD_KIND;
	}
	else if (RENDER_STEWARD_KIND == nID)
	{
		nKind = RENDER_STEWARD_KIND;
	}
	else if (RENDER_FENCE_KIND <= nID && nID < RENDER_FENCE_KIND+CSHFarmObj::MAX_GRADE)
	{
		nKind = RENDER_FENCE_KIND;
	}
	else if (RENDER_ANIMALCAGE_KIND <= nID && nID < RENDER_ANIMALCAGE_KIND+CSHFarmObj::MAX_GRADE)
	{
		nKind = RENDER_ANIMALCAGE_KIND;
	}
	else if (RENDER_HOUSE_KIND <= nID && nID < RENDER_HOUSE_KIND+CSHFarmObj::MAX_GRADE)
	{
		nKind = RENDER_HOUSE_KIND;
	}
	else if (RENDER_GARDENPATCH_KIND <= nID && nID < RENDER_GARDENPATCH_KIND+CSHFarmObj::MAX_GRADE)
	{
		nKind = RENDER_GARDENPATCH_KIND;
	}
	else if (RENDER_WAREHOUSE_KIND <= nID && nID < RENDER_WAREHOUSE_KIND+CSHFarmObj::MAX_GRADE)
	{
		nKind = RENDER_WAREHOUSE_KIND;
	}
	else if (RENDER_CROP_KIND <= nID && nID < RENDER_CROP_KIND+100*(RENDER_CROP_KIND_NUM-1)+CSHCrop::CROP_STEP_MAX)
	{
		nKind = RENDER_CROP_KIND;
	}
	else if (RENDER_BASIC_FENCE == nID)
	{
		nKind = RENDER_BASIC_FENCE;
	}
	else if (RENDER_BASIC_GARDENPATCH_KIND == nID)
	{
		nKind = RENDER_BASIC_GARDENPATCH_KIND;
	}
	else if (RENDER_ANIMAL_KIND <= nID && nID < RENDER_ANIMAL_KIND+(RENDER_ANIMAL_KIND_NUM))
	{
		nKind = RENDER_ANIMAL_KIND;
	}
	else if (RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND == nID)
	{
		nKind = RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND;
	}
	//else if (RENDER_ANIMAL_CAGE_SLOT_KIND <= nID && nID < RENDER_ANIMAL_CAGE_SLOT_KIND+CSHFarmObj::MAX_GRADE)
	//{
	//	nKind = RENDER_ANIMAL_CAGE_SLOT_KIND;
	//}

	return nKind;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_BuyFarm Method																										   농장 구입
//
VOID CSHFarmManager::SRV_BuyFarm(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, int nAgentChecked)
{
#if defined(_MAPSERVER_)
	CSHMain::RESULT eResult = CSHMain::RESULT_OK;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}


	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pcsFarm = pFarmZone->GetFarm(nFarmID);

	// 비어있는 농장인가?
	if (pcsFarm->GetOwnState() != CSHFarmObj::OWN_STATE_EMPTY)
	{
		return;
	}

	// 이미 소유하고 있는 농장 개수 체크
	UINT nOwnFarmNum = 0;
	for(int i=0; i<m_pcsFarmZone[eFarmZone]->GetFarmNum(); i++)
	{
		CSHFarm* pFarm = pFarmZone->GetFarm(i);
		if (pFarm->GetOwner() == pPlayer->GetID())
		{
			nOwnFarmNum++;
			if (nOwnFarmNum >= FARM_BUY_MAX_NUM)
			{
				eResult = CSHMain::RESULT_FAIL_01;
				goto FAILED;
			}
		}
	}

	// 레벨 체크
	if (pPlayer->GetLevel() < FARM_BUY_MASTER_LEVEL)
	{
		eResult = CSHMain::RESULT_FAIL_02;
		goto FAILED;
	}

	// 돈 체크
	if (pPlayer->GetMoney() < FARM_BUY_MONEY)
	{
		eResult = CSHMain::RESULT_FAIL_03;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pcsFarm->GetPos(), &stPlayerPos); 
	if (nDistance > OBJECT_CONTROL_DISTANCE*15)
	{
		eResult = CSHMain::RESULT_FAIL_04;
		goto FAILED;
	}

	if (nAgentChecked == 0)
	{
		// 에이전트에서 패밀리 상태 확인
		MSG_DWORD4					stPacket;

		stPacket.Category			= MP_FARM;
		stPacket.Protocol			= MP_FARM_BUY_AGENT_CHECK;
		stPacket.dwObjectID 		= pPlayer->GetID();
		stPacket.dwData1			= eFarmZone;
		stPacket.dwData2			= nFarmID;
		stPacket.dwData3			= FARM_BUY_FAMILY_MEMNUM;
		stPacket.dwData4			= FARM_BUY_FAMILY_HONORPOINT;

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	// 농장 구입
	// ..돈 소비
	if( pPlayer->SetMoney(FARM_BUY_MONEY, MONEY_SUBTRACTION, MF_NOMAL, eItemTable_Inventory, eMoneyLog_LoseFarmBuy) != FARM_BUY_MONEY )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_MONEY_ERROR;
		msg.dwObjectID = pPlayer->GetID();
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	pcsFarm->SetOwnState(CSHFarmObj::OWN_STATE_OWNED);
	pcsFarm->SetOwner(pPlayer->GetID());
	pcsFarm->SetGrade(CSHFarmObj::GRADE_1);
	pcsFarm->GetGarden(0)->SetGrade(CSHFarmObj::GRADE_1);
	pcsFarm->GetHouse(0)->SetGrade(CSHFarmObj::GRADE_1);
	pcsFarm->GetWarehouse(0)->SetGrade(CSHFarmObj::GRADE_1);
	pcsFarm->GetAnimalCage(0)->SetGrade(CSHFarmObj::GRADE_1);
	pcsFarm->GetFence(0)->SetGrade(CSHFarmObj::GRADE_1);

	// DB에 저장
	SRV_SaveFarmInfoToDB(eFarmZone, nFarmID, pcsFarm);

	// 로그
	InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_BUY, pPlayer->GetID());

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_STATE			stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_BUY;
	stPacket.dwObjectID 		= pPlayer->GetID();
	stPacket.nFarmZone			= (int)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nFarmOwner 		= pPlayer->GetID();
	stPacket.nGardenGrade		= pcsFarm->GetGarden(0)->GetGrade();
	stPacket.nFenceGrade		= pcsFarm->GetFence(0)->GetGrade();
	stPacket.nAnimalCageGrade	= pcsFarm->GetAnimalCage(0)->GetGrade();
	stPacket.nHouseGrade		= pcsFarm->GetHouse(0)->GetGrade();
	stPacket.nWarehouseGrade	= pcsFarm->GetWarehouse(0)->GetGrade();
	stPacket.nResult			= (int)eResult;

	if (eResult == CSHMain::RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pcsFarm, &stPacket, sizeof(stPacket));
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_PlantToGarden Method																								 농작물 심기
//
VOID CSHFarmManager::SRV_PlantCrop(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx, int nAgentChecked)
{

#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	int nCropKind = 0;
	UINT nCropSeedGrade = 0;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 텃밭 범위 체크
	if (nGardenID >= pFarm->GetGardenNum())
	{
		return;
	}

	CSHGarden* pGarden = pFarm->GetGarden(nGardenID);

	// 농작물 범위 체크
	if (nCropID >= pGarden->GetCropNum())
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_FARM_PLANT)
	{
		return;
	}
	nCropKind = pItemInfo->SupplyValue;
	nCropSeedGrade = pItemInfo->Grade;

	CSHCrop* pCrop = pGarden->GetCrop(nCropID);

	// 씨앗 종류 범위 체크
	if (nCropKind == 0 || nCropKind > RENDER_CROP_KIND_NUM)
	{
		return;
	}

	// 씨앗 등급 범위 체크
	if (nCropSeedGrade == 0 || nCropSeedGrade > CSHCrop::CROP_SEED_GRADE_MAX)
	{
		return;
	}

	// 패밀리 상태
	// ..에이전트에서 관리하므로 에이전트로 보낸다.
	if (nAgentChecked == 0)
	{
		PACKET_FARM_GARDEN_CROP_PLANT stPacket;

		stPacket.Category 		= MP_FARM;
		stPacket.Protocol 		= MP_FARM_GARDEN_PLANT_AGENT_CHECK;
		stPacket.dwObjectID		= pPlayer->GetID();
		stPacket.nFarmZone		= eFarmZone;
		stPacket.nFarmID		= nFarmID;
		stPacket.nGardenID		= nGardenID;
		stPacket.nCropID		= nCropID;
		stPacket.nItemTargetPos	= nItemTargetPos;
		stPacket.nItemIdx		= nItemIdx;
		stPacket.nOwner			= pFarm->GetOwner();
		// 멤버 수 체크를 위해 nResult 변수에다가...
		stPacket.nResult		= FARM_BUY_FAMILY_MEMNUM;

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	// 비어있는 농작물 공간인가?
	if (pCrop->GetStep() != CSHCrop::CROP_STEP_EMPTY)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 텃밭의 등급보다 FARM_PLANT_GARDEN_GRADE_DIFF등급 이상의 씨앗은 심을 수 없다.
	if (nCropSeedGrade >= pGarden->GetGrade()+FARM_PLANT_GARDEN_GRADE_DIFF)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 다시 심는데 필요한 시간이 있나?
	if (pPlayer->GetFarmInfo()->nCropPlantRetryTime)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pCrop->GetPos(), &stPlayerPos); 
	if (nDistance > OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	// 농작물 심기
	pCrop->Plant(pPlayer->GetID(), nCropSeedGrade, nCropKind);

	// DB에 저장
	SRV_SaveCropInfoToDB(eFarmZone, nFarmID, pCrop);

	// 다시 심는데 필요한 시간 설정
	pPlayer->SetFarmCropPlantRetryTime(CSHCrop::CROP_PLANT_RETRY_TIME);
	Farm_SetTimeDelay(pPlayer->GetID(), FARM_TIMEDELAY_KIND_PLANT, CSHCrop::CROP_PLANT_RETRY_TIME);

	// 071114 웅주, 로그
	InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmPutCrop );

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_GARDEN_CROP_PLANT stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_GARDEN_PLANT;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nGardenID			= nGardenID;
	stPacket.nCropID			= nCropID;
	stPacket.nCropOwner			= pPlayer->GetID();
	stPacket.nCropSeedGrade		= nCropSeedGrade;
	stPacket.nCropKind			= nCropKind;
	stPacket.nCropLife			= pCrop->GetLife();
	stPacket.nResult			= (WORD)eResult;

	if (eResult == CSHMain::RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ManureCrop Method																								농작물 비료 주기
//
VOID CSHFarmManager::SRV_ManureCrop(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	int nManureKind = 0;
	int nManureGrade = 0;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 텃밭 범위 체크
	if (nGardenID >= pFarm->GetGardenNum())
	{
		return;
	}

	CSHGarden* pGarden = pFarm->GetGarden(nGardenID);

	// 농작물 범위 체크
	if (nCropID >= pGarden->GetCropNum())
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_FARM_MANURE)
	{
		return;
	}
	nManureKind = pItemInfo->SupplyValue;
	nManureGrade = pItemInfo->Grade;

	// 비료 종류 범위 체크
	if (nManureKind > CSHCrop::CROP_MANURE_KIND_MAX)
	{
		return;
	}

	// 씨앗 등급 범위 체크
	if (nManureGrade > CSHCrop::CROP_SEED_GRADE_MAX)
	{
		return;
	}

	CSHCrop* pCrop = pGarden->GetCrop(nCropID);

	// 농작물이 심어져 있나?
	if (pCrop->GetStep() < CSHCrop::CROP_STEP_1)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 농작물 주인만 비료를 줄 수 있다.
	if (pCrop->GetOwner() != pPlayer->GetID())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 농작물이 다 자랐나?
	if (pCrop->GetStep() == CSHCrop::CROP_STEP_COMPLETE)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 다시 비료를 주는 데 필요한 시간이 있나?
	if (pPlayer->GetFarmInfo()->nCropManureRetryTime)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pCrop->GetPos(), &stPlayerPos); 
	if (nDistance > OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_05;
		goto FAILED;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	// 농작물 비료주기
	pCrop->Manure(nManureGrade, nManureKind);

	// DB에 저장
	SRV_SaveCropInfoToDB(eFarmZone, nFarmID, pCrop);

	// 다시 비료를 주는 데 필요한 시간 설정
	pPlayer->SetFarmCropManureRetryTime(CSHCrop::CROP_MANURE_RETRY_TIME);
	Farm_SetTimeDelay(pPlayer->GetID(), FARM_TIMEDELAY_KIND_MANURE, CSHCrop::CROP_MANURE_RETRY_TIME);

	// 071114 웅주, 로그
	InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmCareCrop );

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_GARDEN_CROP_MANURE stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_GARDEN_MANURE;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nGardenID			= nGardenID;
	stPacket.nCropID			= nCropID;
	stPacket.nCropLife			= pCrop->GetLife();
	stPacket.nManureKind		= nManureKind;
	stPacket.nManureGrade		= nManureGrade;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_HarvestCrop Method																								 농작물 수확하기
//
VOID CSHFarmManager::SRV_HarvestCrop(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nGardenID, WORD nCropID, WORD nItemTargetPos, DWORD nItemIdx, int nType)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 텃밭 범위 체크
	if (nGardenID >= pFarm->GetGardenNum())
	{
		return;
	}

	CSHGarden* pGarden = pFarm->GetGarden(nGardenID);

	// 농작물 범위 체크
	if (nCropID >= pGarden->GetCropNum())
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_FARM_HARVEST)
	{
		return;
	}

	CSHCrop* pCrop = pGarden->GetCrop(nCropID);

	// 농작물 주인만..
	if (pCrop->GetOwner() != pPlayer->GetID())
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 농작물이 다 자랐나?
	if (pCrop->GetStep() != CSHCrop::CROP_STEP_COMPLETE)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pCrop->GetPos(), &stPlayerPos); 
	if (nDistance > OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 아이템 공간 체크
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"올바른 아이템 슬롯을 얻을 수 없습니다.");

	// 071211 KTH -- pPlayer 추가
	DWORD nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);
	if(nNum == 0)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 처음 수확 요청 시
	if (nType == RESULT_OK)
	{
		goto FAILED;
	}
	// 모션 종료 후 실제 수확 요청 시
	else
	{
		eResult = RESULT_OK_02;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	// 수확하기
	int nHarvestKind = (int)pCrop->Harvest();

	// 아이템 지급
	int nItemID = m_pstCropHarvestReward[pCrop->GetKind()-1].nItemID[nHarvestKind];
	int nItemNum = m_pstCropHarvestReward[pCrop->GetKind()-1].nItemNum[nHarvestKind];

	// ..유효 아이템인가?
	// ..스크립트 데이터 파일이나 기타등등의 오류로 유효하지 않은 아이템이라면 처리하지 않는다.
	ITEM_INFO* pstItem = ITEMMGR->GetItemInfo(nItemID);
	if (pstItem == NULL)
	{
		return;
	}

	// 071114 LUJ, 로그
	InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmHarvestCrop );

	// 수확 했으니 제거
	pCrop->SetCrop(CSHCrop::CROP_RESULT_DIE);

	// DB에 저장
	SRV_SaveCropInfoToDB(eFarmZone, nFarmID, pCrop);

	ITEMMGR->ObtainMonstermeterItem(pPlayer, nItemID, nItemNum);
	
FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_GARDEN_CROP_GROW stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_GARDEN_HARVEST;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nGardenID			= nGardenID;
	stPacket.nCropID			= nCropID;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK ||
		eResult == RESULT_OK_02)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_UpgradeFarm Method																								 농장 업그레이드
//
VOID CSHFarmManager::SRV_UpgradeFarm(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nObjKind, int nAgentChecked)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 업그레이드
	CSHFarmObj* pFarmObj;
	switch(nObjKind)
	{
	case CSHFarm::FARM_EVENT_GARDEN:		pFarmObj = (CSHFarmObj*)pFarm->GetGarden(0);		break;
	case CSHFarm::FARM_EVENT_ANIMALCAGE:	pFarmObj = (CSHFarmObj*)pFarm->GetAnimalCage(0);	break;
	case CSHFarm::FARM_EVENT_FENCE:			pFarmObj = (CSHFarmObj*)pFarm->GetFence(0);			break;
	case CSHFarm::FARM_EVENT_HOUSE:			pFarmObj = (CSHFarmObj*)pFarm->GetHouse(0);			break;
	case CSHFarm::FARM_EVENT_WAREHOUSE:		pFarmObj = (CSHFarmObj*)pFarm->GetWarehouse(0);		break;
	}

	int nOldGrade = pFarmObj->GetGrade();
	if (nOldGrade >= CSHFarmObj::MAX_GRADE-2)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	if (nOldGrade >= CSHFarmObj::MAX_GRADE-4 && nObjKind == CSHFarm::FARM_EVENT_GARDEN)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}
		
	// ..패밀리 상태
	if (nAgentChecked == 0)
	{
		// ..에이전트에서 관리하므로 에이전트로 체크 보냄..
		int nNeedPoint = m_stUpgradeCost[nObjKind-1][nOldGrade-1].nFamilyHonorPoint;
		MSG_DWORD5 stPacket;

		stPacket.Category			= MP_FARM;
		stPacket.Protocol			= MP_FARM_UPGRADE_AGENT_CHECK_1;
		stPacket.dwObjectID			= pPlayer->GetID();
		stPacket.dwData1			= (WORD)eFarmZone;
		stPacket.dwData2			= nFarmID;
		stPacket.dwData3			= nObjKind;
		stPacket.dwData4			= pFarm->GetOwner();
		stPacket.dwData5			= FARM_BUY_FAMILY_MEMNUM;
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	// 비용 체크
	// ..골드
	MONEYTYPE nNeedMoney = m_stUpgradeCost[nObjKind-1][nOldGrade-1].nGold;
	if (pPlayer->GetMoney() < nNeedMoney)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// ..패밀리 명성 포인트
	if (nAgentChecked == 1)
	{
		// ..에이전트에서 관리하므로 에이전트로 체크 보냄..
		int nNeedPoint = m_stUpgradeCost[nObjKind-1][nOldGrade-1].nFamilyHonorPoint;
		MSG_DWORD5 stPacket;

		stPacket.Category			= MP_FARM;
		stPacket.Protocol			= MP_FARM_UPGRADE_AGENT_CHECK_2;
		stPacket.dwObjectID			= pPlayer->GetID();
		stPacket.dwData1			= (WORD)eFarmZone;
		stPacket.dwData2			= nFarmID;
		stPacket.dwData3			= nObjKind;
		stPacket.dwData4			= nNeedPoint;
		stPacket.dwData5			= FARM_BUY_FAMILY_MEMNUM;
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	// ..골드 소비
	if( pPlayer->SetMoney(nNeedMoney, MONEY_SUBTRACTION, MF_NOMAL, eItemTable_Inventory, eMoneyLog_LoseFarmUpgrade) != nNeedMoney )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_MONEY_ERROR;
		msg.dwObjectID = pPlayer->GetID();
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	// 등급 설정
	pFarmObj->SetGrade(nOldGrade+1);

	// DB에 저장
	SRV_SaveFarmInfoToDB(eFarmZone, nFarmID, pFarm);

	// 로그
	switch(nObjKind)
	{
	case CSHFarm::FARM_EVENT_GARDEN:		InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_GARDEN,		pFarmObj->GetGrade());		break;
	case CSHFarm::FARM_EVENT_ANIMALCAGE:	InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_FENCE,			pFarmObj->GetGrade());		break;
	case CSHFarm::FARM_EVENT_FENCE:			InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_HOUSE,			pFarmObj->GetGrade());		break;
	case CSHFarm::FARM_EVENT_HOUSE:			InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_WAREHOUSE,		pFarmObj->GetGrade());		break;
	case CSHFarm::FARM_EVENT_WAREHOUSE:		InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_ANIMALCAGE,	pFarmObj->GetGrade());		break;
	default:								InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_UPGRADE, pPlayer->GetID(), FARM_UPGRADE_LOG_KIND_MYSTERY,		pFarmObj->GetGrade());		break;
	}

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_UPGRADE stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_UPGRADE;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nObjKind			= nObjKind;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_PayTax Method																									농장 관리비 납부
//
VOID CSHFarmManager::SRV_PayTax(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, DWORD nOwnerID, int nKind)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pFarm->GetPos(), &stPlayerPos); 
	if (nDistance > OBJECT_CONTROL_DISTANCE*15)
	{
		return;
	}

	// 권한 확인
	// ..패밀리 멤버가 아님
	if (pFarm->GetOwner() != nOwnerID)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 납부 상태 확인
	if (pFarm->IsPayTax())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 비용 체크
	// ..단리식 이자로 계산
	DWORD nNeedMoney = FARM_TAX_GOLD + FARM_TAX_GOLD*pFarm->GetTaxArrearageFreq() + FARM_TAX_GOLD*pFarm->GetTaxArrearageFreq()*FARM_TAX_PAY_ARREARAGE_ADD_RATE/100;

	// 먼저 클라이언트에 관리비 금액을 알리고 납부할 것인지 묻는다.
	if (nKind == 0)
	{
		MSG_NAME_DWORD3 stPacket;

		stPacket.Category			= MP_FARM;
		stPacket.Protocol			= MP_FARM_PAY_TAX;
		stPacket.dwObjectID			= pPlayer->GetID();
		stPacket.dwData1			= RESULT_OK_02;
		stPacket.dwData2			= nNeedMoney;

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	if (pPlayer->GetMoney() < nNeedMoney)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 골드 소비
	if( pPlayer->SetMoney(nNeedMoney, MONEY_SUBTRACTION, MF_NOMAL, eItemTable_Inventory, eMoneyLog_LoseFarmTax) != nNeedMoney )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_MONEY_ERROR;
		msg.dwObjectID = pPlayer->GetID();
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	// 납부 설정
	pFarm->SetTaxArrearageFreq(0);
	pFarm->SetTaxPayPlayerName(pPlayer->GetObjectName());

	// DB에 저장
	Farm_SetTaxInfo(eFarmZone, nFarmID, pFarm->GetTaxArrearageFreq(), pPlayer->GetID());
	// 081205 LUJ, 로그
	LogItemMoney(
		pPlayer->GetID(),
		pPlayer->GetObjectName(),
		0,
		"*farmTax",
		eLog_FarmTaxPaid,
		pPlayer->GetMoney(),
		0,
		nNeedMoney,
		0,
		0,
		0,
		0,
		0,
		0 );
	// 로그
	InsertLogFarm(eFarmZone, nFarmID, FARM_LOG_KIND_PAY_TAX, pPlayer->GetID());

FAILED:
	MSG_NAME_DWORD3 stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_PAY_TAX;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= eResult;
	stPacket.dwData2			= eFarmZone;
	stPacket.dwData3			= nFarmID;
	SafeStrCpy(stPacket.Name, pFarm->GetTaxPayPlayerName(), MAX_NAME_LENGTH+1);

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcTaxCollect Method																				       농장 관리비 징수 처리
//
VOID CSHFarmManager::SRV_ProcTaxCollect()
{
#if defined(_MAPSERVER_)
	// 징수 시간, 요일이 됐나?
	if (MHTIMEMGR_OBJ->GetOldLocalTime()->wHour			!= MHTIMEMGR_OBJ->GetCurLocalTime()->wHour	&&
		MHTIMEMGR_OBJ->GetCurLocalTime()->wHour			== FARM_TAX_PAY_LIMIT_HOUR					&&
		MHTIMEMGR_OBJ->GetCurLocalTime()->wDayOfWeek	== FARM_TAX_PAY_LIMIT_DAY)
	{
		// 전에 징수한 이후로 1주가 지났다.
		m_nTaxCollectPastWeek++;

		// 다시 징수할 차례의 주가 지났나?
		if (m_nTaxCollectPastWeek >= FARM_TAX_PAY_WEEK_INTERVAL)
		{
			m_nTaxCollectPastWeek = 0;
			for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
			{
 				for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
				{
					CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);

					if (pFarm->GetOwnState() == CSHFarmObj::OWN_STATE_EMPTY) continue;
					if (pFarm->IsPayTax())
					{
						// 관리비를 냈다. 다음에 징수할 관리비를 위해 정보 초기화
						pFarm->SetTaxArrearageFreq(0);
						pFarm->SetTaxPayPlayerName(0);
						// DB 저장
						Farm_SetTaxInfo(i, j, pFarm->GetTaxArrearageFreq(), 0);
					}
					else
					{
						// 관리비를 내지 않았다.
						// ..허용 가능한 연체 횟수를 넘었나?
						if (pFarm->GetTaxArrearageFreq() >= FARM_TAX_PAY_ARREARAGE_MAX_FREQ)
						{
							// 더 이상 연체 불가. 농장 삭제
							SRV_DelFarmByFarmID(pFarm->GetOwner(), i, j, FARM_DEL_KIND_TAX);
							// 관리비 정보 DB 저장
							Farm_SetTaxInfo(i, j, pFarm->GetTaxArrearageFreq(), 0);
						}
						else
						{
							// 연체 증가
							pFarm->SetTaxArrearageFreq(pFarm->GetTaxArrearageFreq()+1);
							// DB 저장
							Farm_SetTaxInfo(i, j, pFarm->GetTaxArrearageFreq(), 0);
						}
					}
				}
			}
		}

		// 파일에 징수 주기 저장. (2주 이상 적용 시 작업 필요)
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_NotifyTaxNonpayment Method																				   농장 관리비 미납 알림
//
VOID CSHFarmManager::SRV_NotifyTaxNonpayment(DWORD nPlayerID, CSHFarm* pcsFarm)
{
#if defined(_MAPSERVER_)
	//  관리비 납부 기한 주 중에 기한 일 하루 전, 또는 당일이면서 아직 기한 시간 전이라면 맵에 접속할 때마다 관리비 미납을 알린다.
	if (pcsFarm->IsPayTax() == FALSE &&
		m_nTaxCollectPastWeek == FARM_TAX_PAY_WEEK_INTERVAL-1)
	{
		if (FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY == 999) return;

		BOOL bNotiryTaxPay = FALSE;
		int nTblDay[] = { 0,1,2,3,4,5,6,  0,1,2,3,4,5,6 };
		if (FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY > 5) FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY = 1;

		for(int i=FARM_TAX_PAY_NOTIFY_NONPAYMENT_DDAY; i>= 0; i--)
		{
			if (MHTIMEMGR_OBJ->GetCurLocalTime()->wDayOfWeek == nTblDay[7+FARM_TAX_PAY_LIMIT_DAY - i])
			{
				// 기한 당일이면 기한 시간 전까지만 알림
				if (MHTIMEMGR_OBJ->GetCurLocalTime()->wDayOfWeek == FARM_TAX_PAY_LIMIT_DAY)
				{
					if (MHTIMEMGR_OBJ->GetCurLocalTime()->wHour < FARM_TAX_PAY_LIMIT_HOUR)
					{
						bNotiryTaxPay = TRUE;
					}
				}
				else
				{
					bNotiryTaxPay = TRUE;
				}

				break;
			}
		}

		// 관리비 미납 알림
		if (bNotiryTaxPay)
		{
			MSG_DWORD stPacket;
			stPacket.Category = MP_FARM;
			stPacket.Protocol = MP_FARM_TAX_NONPAYMENT_NOTIFY;
			stPacket.dwObjectID = nPlayerID;
			stPacket.dwData		= pcsFarm->GetTaxArrearageFreq();

			g_Network.Broadcast2AgentServer((char*)&stPacket, sizeof(stPacket));
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_DelCrop Method																										 농작물 삭제
//
VOID CSHFarmManager::SRV_DelCrop(DWORD nOwnerID)
{
#if defined(_MAPSERVER_)
	// 농장맵인가?
	if (GetFarmZoneID(g_pServerSystem->GetMapNum(), NULL) == FALSE) return;

	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 농장 지역
 		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			// 농장
			CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);
 			for(int k=0; k<pFarm->GetGardenNum(); k++)
			{
				// 텃밭
				CSHGarden* pGarden = pFarm->GetGarden(k);
				for(int p=0; p<pGarden->GetCropNum(); p++)
				{
					// 농작물
					CSHCrop* pCrop = pGarden->GetCrop(p);
					if (pCrop->GetOwner() == nOwnerID)
					{
						// 071114 LUJ, 로그
						InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmLostRight );

						pCrop->AddEvent(NULL, CSHCrop::CROP_RESULT_DIE);
						pCrop->SetCrop(CSHCrop::CROP_RESULT_DIE);
					}
				}
			}
		}
	}							
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_DelFarm Method																										   농장 삭제
//
VOID CSHFarmManager::SRV_DelFarm(DWORD nOwnerID, FARM_DEL_KIND eDelKind)
{
#if defined(_MAPSERVER_)
	// 농장맵인가?
	if (GetFarmZoneID(g_pServerSystem->GetMapNum(), NULL) == FALSE) return;

	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 농장 지역
 		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			// 농장
			CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);
			if (pFarm->GetOwner() == nOwnerID)
			{
				// 삭제
				pFarm->Init();

				// DB에 저장
				SRV_SaveFarmInfoToDB(i,	j, pFarm);

				// 로그
				switch(eDelKind)
				{
				case FARM_DEL_KIND_BREAKUP_FAMILY:	InsertLogFarm(i, j, FARM_LOG_KIND_DEL_FAMILY_BREAKUP, nOwnerID);	break;
				default:							InsertLogFarm(i, j, FARM_LOG_KIND_MYSTERY, nOwnerID);				break;
				}

				// 클라이언트에 알리기
				PACKET_FARM_STATE			stPacket;

				stPacket.Category			= MP_FARM;
				stPacket.Protocol			= MP_FARM_DEL;
				stPacket.dwObjectID 		= nOwnerID;
				stPacket.nFarmZone			= i;
				stPacket.nFarmID			= j;
				stPacket.nFarmState			= CSHFarm::OWN_STATE_EMPTY;
				stPacket.nFarmOwner 		= 0;
				stPacket.nGardenGrade		= 0;
				stPacket.nFenceGrade		= 0;
				stPacket.nAnimalCageGrade	= 0;
				stPacket.nHouseGrade		= 0;
				stPacket.nWarehouseGrade	= 0;
				stPacket.nResult			= eDelKind;

				SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

 				for(int k=0; k<pFarm->GetGardenNum(); k++)
				{
					// 텃밭
					CSHGarden* pGarden = pFarm->GetGarden(k);
					for(int p=0; p<pGarden->GetCropNum(); p++)
					{
						// 농작물
						CSHCrop* pCrop = pGarden->GetCrop(p);
						if (pCrop->GetLife())
						{
							// 071114 웅주, 로그
							InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmDisband );

							pCrop->SetCrop(CSHCrop::CROP_RESULT_DIE);
							SRV_SaveCropInfoToDB(i, pFarm->GetID(), pCrop);
						}
					}
				}
				// 080423 KTH
				for( int k=0; k<pFarm->GetAnimalCageNum(); k++ )
				{
					CPen* pAnimalCage = pFarm->GetAnimalCage(k);
					for( int p=0; p<pAnimalCage->GetAnimalNum(); p++ )
					{
						CAnimal* pAnimal = pAnimalCage->GetAnimal(p);
						if( pAnimal->GetLife() )
						{
							pAnimal->SetAnimal( CAnimal::ANIMAL_RESULT_DIE );
							SRV_SaveAnimalInfoToDB(i, pFarm->GetID(), pAnimal);
						}
					}
				}
			}
		}
	}							
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_DelFarmByFarmID Method																								   농장 삭제
//
VOID CSHFarmManager::SRV_DelFarmByFarmID(DWORD nOwnerID, WORD nFarmZone, WORD nFarmID, FARM_DEL_KIND eDelKind)
{
#if defined(_MAPSERVER_)
	// 농장 지역 체크
	if (nFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[nFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 삭제
	pFarm->Init();
	
	// DB에 저장
	SRV_SaveFarmInfoToDB(nFarmZone,	nFarmID, pFarm);
	// 로그, 쪽지
	switch(eDelKind)
	{
	case FARM_DEL_KIND_TAX:	
		InsertLogFarm(nFarmZone, nFarmID, FARM_LOG_KIND_DEL_TAX_ARREARAGE, nOwnerID);	
		break;
	default:				
		InsertLogFarm(nFarmZone, nFarmID, FARM_LOG_KIND_MYSTERY, nOwnerID);
	}

	// 클라이언트에 알리기
	PACKET_FARM_STATE			stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_DEL;
	stPacket.dwObjectID 		= nOwnerID;
	stPacket.nFarmZone			= nFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nFarmState			= CSHFarm::OWN_STATE_EMPTY;
	stPacket.nFarmOwner 		= nOwnerID;
	stPacket.nGardenGrade		= 0;
	stPacket.nFenceGrade		= 0;
	stPacket.nAnimalCageGrade	= 0;
	stPacket.nHouseGrade		= 0;
	stPacket.nWarehouseGrade	= 0;
	stPacket.nResult			= eDelKind;

	// 주변에 있는 플레이어들에게 알리기 (오브젝트 표시 사항만)
	SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
	// 에이전트로 Broadcast 해서 멤버들에게 알리기
	{
		g_pServerTable->SetPositionHead();
		SERVERINFO* pInfo = g_pServerTable->GetNextAgentServer();

		MSG_DWORD2 stPacket;

		stPacket.Category		= MP_FARM;
		stPacket.Protocol		= MP_FARM_DEL_NOTIFY_TO_MEMBER;
		stPacket.dwObjectID		= nOwnerID;
		stPacket.dwData1		= eDelKind;
		stPacket.dwData2		= pInfo->wServerNum;

		g_Network.Broadcast2AgentServer((char*)&stPacket, sizeof(stPacket));
	}

 	for(int k=0; k<pFarm->GetGardenNum(); k++)
	{
		// 텃밭
		CSHGarden* pGarden = pFarm->GetGarden(k);
		for(int p=0; p<pGarden->GetCropNum(); p++)
		{
			// 농작물
			CSHCrop* pCrop = pGarden->GetCrop(p);
			if (pCrop->GetLife())
			{
				switch(eDelKind)
				{
				case FARM_DEL_KIND_TAX:
					InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmTax );
					break;
				default:
					InsertLogFarmCrop( pFarm, pCrop, eLog_FamilyFarmMystery );
				}

				pCrop->SetCrop(CSHCrop::CROP_RESULT_DIE);
				SRV_SaveCropInfoToDB(nFarmZone, pFarm->GetID(), pCrop);
			}
		}

		// 080423 KTH
		CPen* pAnimalCage = pFarm->GetAnimalCage(k);
		for(int p=0; p<pAnimalCage->GetAnimalNum(); p++)
		{
			CAnimal* pAnimal = pAnimalCage->GetAnimal(p);
			if( pAnimal->GetLife() )
			{
				switch(eDelKind)
				{
				case FARM_DEL_KIND_TAX:
					break;
				default:
					break;
				}

				pAnimal->SetAnimal(CAnimal::ANIMAL_RESULT_DIE);
				SRV_SaveAnimalInfoToDB(nFarmZone, pFarm->GetID(), pAnimal);
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendFarmInfoToNearPlayer Method															주변의 다른 플레이어들에게 데이터 보내기
//
VOID CSHFarmManager::SRV_SendFarmInfoToNearPlayer(CSHFarm* pcsFarm, MSGBASE* pPacket, int nSize)
{
#if defined(_MAPSERVER_)
	CObject* pObject;
	g_pUserTable->SetPositionUserHead();
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		VECTOR3 pos;
		pObject->GetPosition(&pos);
		float nDis1 = abs(pos.x - pcsFarm->GetPos()->x);
		float nDis2 = abs(pos.z - pcsFarm->GetPos()->z);

		nDis1 = max(nDis1, nDis2);
		if (nDis1 < CHECK_DISTANCE)
		{
			pObject->SendMsg(pPacket, nSize);
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcessEvent Method																									 이벤트 처리
//
VOID CSHFarmManager::SRV_ProcessEvent()
{
#if !defined(_AGENTSERVER)
	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 농장 지역
 		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			/*
			for(int jj=0; jj<m_pcsFarmZone[i]->GetEvent()->nEventNum; jj++)
			{
				switch(m_pcsFarmZone[i]->GetEvent()->nEvent[jj])
				{
				default:
					break;
				}
			}
			*/
			// 농장
			CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);

		#if !defined(_MAPSERVER_)
 			// 플레이어와의 거리 체크
			VECTOR3 pos = HERO->GetCurPosition();
			float nDis1 = abs(pos.x - pFarm->GetPos()->x);
			float nDis2 = abs(pos.z - pFarm->GetPos()->z);
			nDis1 = max(nDis1, nDis2);

			if (pFarm->GetIsRendering() == FALSE)
			{
				// 표시 거리 안에 들어오면 서버에 렌더링 오브젝트 추가 요청
				if (nDis1 < CHECK_DISTANCE)
				{
					pFarm->SetIsRendering(TRUE);
					CLI_RequestFarmInfo((CSHFarmZone::FARM_ZONE)i, j);
				}
			}
			else
			{
				// 표시 거리 밖으로 나가면 렌더링 오브젝트 제거
				if (nDis1 > CHECK_DISTANCE)
				{
					pFarm->SetIsRendering(FALSE);
					CLI_RemoveRenderFarmObjFromScreen(pFarm);
				}
			}
		#endif

			// 
			for(int k=0; k<pFarm->GetAnimalCageNum(); k++)
			{
				// 축사
				CPen* pPen = pFarm->GetAnimalCage(k);
				for(int p=0; p<pPen->GetAnimalNum(); p++)
				{
					BOOL bSetToDB = FALSE;
					CAnimal* pAnimal = pPen->GetAnimal(p);

					// 가축
					for( int q=0; q<pAnimal->GetEvent()->nEventNum; q++ )
					{
						switch( pAnimal->GetEvent()->nEvent[q])
						{
						case CAnimal::ANIMAL_RESULT_DIE:
						case CAnimal::ANIMAL_RESULT_LIFE:
						case CAnimal::ANIMAL_RESULT_CONTENTMENT:
						case CAnimal::ANIMAL_RESULT_INTEREST:
						//case CAnimal::ANIMAL_RESULT_REWARD:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_CAGE_ANIMAL_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_ANIMAL_GROW;
								stPacket.dwObjectID			= pAnimal->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nAnimalCageID		= pPen->GetID();
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nAnimalID			= pAnimal->GetID();
								stPacket.nResult			= (WORD)pAnimal->GetEvent()->nEvent[q];
								switch( pAnimal->GetEvent()->nEvent[q] )
								{
								case CAnimal::ANIMAL_RESULT_LIFE:
									stPacket.nValue			= pAnimal->GetLife();
									break;
								case CAnimal::ANIMAL_RESULT_CONTENTMENT:
									stPacket.nValue			= pAnimal->GetContentment();
									break;
								case CAnimal::ANIMAL_RESULT_INTEREST:
									stPacket.nValue			= pAnimal->GetInterest();
									break;
								}

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								//SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						case CAnimal::ANIMAL_RESULT_IN_STALL:
							#if !defined(_MAPSERVER_)
							{
								// 플레이어와의 거리 체크
								VECTOR3 pos = HERO->GetCurPosition();
								float nDis1 = abs(pos.x - pFarm->GetPos()->x);
								float nDis2 = abs(pos.z - pFarm->GetPos()->z);
								nDis1 = max(nDis1, nDis2);

								// 체크 거리 안에 있을 경우에 렌더링 오브젝트 출력
								if (nDis1 < CHECK_DISTANCE)
								{
									pPen->GetRenderObjEx(pAnimal->GetID()+5)->GetEngineObject()->DisablePick();
									// 기존 오브젝트가 선택되어 있었다면..
									BOOL bSel = FALSE;
									if ((CObject*)pPen->GetRenderObjEx(pAnimal->GetID()+5) == OBJECTMGR->GetSelectedObject())
									{
										// 타켓 해제
 										if(OBJECTMGR->GetSelEffectHandle())
										{
											EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
										}

										if (OBJECTMGR->GetSelectedObject())
										{
											OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
											OBJECTMGR->GetSelectedObject()->OnDeselected();
											OBJECTMGR->SetSelectedObjectID(0);
										}

										CLI_SetTargetObj(NULL);
										bSel = TRUE;
									}

									// ..렌더링용 오브젝트 추가
									CLI_AddRenderFarmAnimalObjToScreen(pAnimal);

									// ..이름 변경
									char* pszName = CAnimal::ANIMAL_NAME_TBL[pAnimal->GetKind()];
                                    pAnimal->GetRenderObj()->SetName(pszName);
									pAnimal->GetRenderObj()->GetObjectBalloon()->GetOBalloonName()->SetName(pszName);
									if (bSel)
									{
										CActionTarget Target;
										Target.InitActionTarget( pAnimal->GetRenderObj(), NULL );
										// 조작할 농작물 선택
										g_csFarmManager.CLI_SetTargetObj(pAnimal->GetRenderObj());
										pAnimal->GetRenderObj()->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
										OBJECTMGR->SetSelectedObject( pAnimal->GetRenderObj() );
									}
									// ..다이얼로그에 이름 변경
									CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
									if (pGuageDlg)
									{
										if ((CObject*)pAnimal->GetRenderObj() == OBJECTMGR->GetSelectedObject())
										{
											pGuageDlg->SetName( (CObject*)pAnimal->GetRenderObj(), CAnimal::ANIMAL_NAME_TBL[pAnimal->GetKind()] ) ; 
										}
									}
									// ..이펙트
									if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_IN_STALL])
									{
										TARGETSET set;
										set.pTarget = pAnimal->GetRenderObj();
										EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_IN_STALL], pAnimal->GetRenderObj(), &set, 0, pAnimal->GetRenderObj()->GetID());
									}
								}
							}
							#endif
							break;
						case CAnimal::ANIMAL_RESULT_REWARD:
							#if !defined(_MAPSERVER_)
							{
								CLI_RequestAnimalGetItem(RESULT_OK_02);
							}
							#endif
							break;
						}
						#if defined(_MAPSERVER_)
						if( bSetToDB )
						{
							SRV_SaveAnimalInfoToDB(i, pFarm->GetID(), pAnimal);
						}
						#endif
					}
					// 이벤트 초기화
					pAnimal->InitEvent();
				}

				// 텃밭
				CSHGarden* pGarden = pFarm->GetGarden(k);
				for(int p=0; p<pGarden->GetCropNum(); p++)
				{
					/*
					for(int pp=0; pp<pFarm->GetEvent()->nEventNum; pp++)
					{
						switch(pGarden->GetEvent()->nEvent[pp])
						{
						default:
							break;
						}
					}
					*/
					// 농작물
					CSHCrop* pCrop = pGarden->GetCrop(p);
					for(int q=0; q<pCrop->GetEvent()->nEventNum; q++)
					{
						BOOL bSetToDB = FALSE;
						switch(pCrop->GetEvent()->nEvent[q])
						{
						// 생명력 변화
						case CSHCrop::CROP_RESULT_LIFE:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_GARDEN_CROP_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_GARDEN_GROW;
								stPacket.dwObjectID			= pCrop->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nGardenID			= pGarden->GetID();
								stPacket.nCropID			= pCrop->GetID();
								stPacket.nValue				= pCrop->GetLife();
								stPacket.nResult			= (WORD)pCrop->GetEvent()->nEvent[q];

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						// 농작물 죽음
						case CSHCrop::CROP_RESULT_DIE:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_GARDEN_CROP_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_GARDEN_GROW;
								stPacket.dwObjectID			= pCrop->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nGardenID			= pGarden->GetID();
								stPacket.nCropID			= pCrop->GetID();
								stPacket.nResult			= (WORD)pCrop->GetEvent()->nEvent[q];

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						// 농작물 단계 상승
						case CSHCrop::CROP_RESULT_STEP_UP:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_GARDEN_CROP_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_GARDEN_GROW;
								stPacket.dwObjectID			= pCrop->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nGardenID			= pGarden->GetID();
								stPacket.nCropID			= pCrop->GetID();
								stPacket.nResult			= (WORD)pCrop->GetEvent()->nEvent[q];

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						// 농작물 단계 상승 완료
						case CSHCrop::CROP_RESULT_STEP_UP_COMPLETE:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_GARDEN_CROP_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_GARDEN_GROW;
								stPacket.dwObjectID			= pCrop->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nGardenID			= pGarden->GetID();
								stPacket.nCropID			= pCrop->GetID();
								stPacket.nResult			= (WORD)pCrop->GetEvent()->nEvent[q];

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						// 농작물 다음 단계 시간
						case CSHCrop::CROP_RESULT_NEXT_STEP_TIME:
							#if defined(_MAPSERVER_)
							{
								// DB 저장 설정
								bSetToDB = TRUE;
								// 클라이언트에 알리기
								PACKET_FARM_GARDEN_CROP_GROW stPacket;
								stPacket.Category			= MP_FARM;
								stPacket.Protocol			= MP_FARM_GARDEN_GROW;
								stPacket.dwObjectID			= pCrop->GetEvent()->nOwnerID;
								stPacket.nFarmZone			= i;
								stPacket.nFarmID			= pFarm->GetID();
								stPacket.nGardenID			= pGarden->GetID();
								stPacket.nCropID			= pCrop->GetID();
								stPacket.nValue				= pCrop->GetNextStepTime();
								stPacket.nResult			= (WORD)pCrop->GetEvent()->nEvent[q];

								SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));

								// 농장UI 정보 전송
								SRV_SendFarmUIInfoToClient(i, pFarm->GetID(), pGarden->GetID(), pCrop, pFarm->GetOwner());
							}
							#endif
							break;
						// 농작물 심기
						case CSHCrop::CROP_RESULT_PLANT:
							#if !defined(_MAPSERVER_)
							{
 								// 플레이어와의 거리 체크
								VECTOR3 pos = HERO->GetCurPosition();
								float nDis1 = abs(pos.x - pFarm->GetPos()->x);
								float nDis2 = abs(pos.z - pFarm->GetPos()->z);
								nDis1 = max(nDis1, nDis2);

								// 체크 거리 안에 있을 경우에 렌더링 오브젝트 출력
								if (nDis1 < CHECK_DISTANCE)
								{
									pGarden->GetRenderObjEx(pCrop->GetID()+15)->GetEngineObject()->DisablePick();
									// 기존 오브젝트가 선택되어 있었다면..
									BOOL bSel = FALSE;
									if ((CObject*)pGarden->GetRenderObjEx(pCrop->GetID()+15) == OBJECTMGR->GetSelectedObject())
									{
										// 타켓 해제
 										if(OBJECTMGR->GetSelEffectHandle())
										{
											EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
										}

										if (OBJECTMGR->GetSelectedObject())
										{
											OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
											OBJECTMGR->GetSelectedObject()->OnDeselected();
											OBJECTMGR->SetSelectedObjectID(0);
										}

										CLI_SetTargetObj(NULL);
										bSel = TRUE;
									}

									// ..렌더링용 오브젝트 추가
									CLI_AddRenderFarmCropObjToScreen(pCrop);

									// ..이름 변경
									char* pszName = m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(pCrop->GetKind()-1)+pCrop->GetStep()-CSHCrop::CROP_STEP_1].szName;
									pCrop->GetRenderObj()->SetName(pszName);
									pCrop->GetRenderObj()->GetObjectBalloon()->GetOBalloonName()->SetName(pszName);
									if (bSel)
									{
										CActionTarget Target;
										Target.InitActionTarget( pCrop->GetRenderObj(), NULL );
										// 조작할 농작물 선택
										g_csFarmManager.CLI_SetTargetObj(pCrop->GetRenderObj());
										pCrop->GetRenderObj()->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
										OBJECTMGR->SetSelectedObject( pCrop->GetRenderObj() );
									}
									// ..다이얼로그에 이름 변경
									CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
									if (pGuageDlg)
									{
										if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
										{
											pGuageDlg->SetName( (CObject*)pCrop->GetRenderObj(), pszName ) ; 
										}
									}
									// ..이펙트
									if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP])
									{
										TARGETSET set;
										set.pTarget = pCrop->GetRenderObj();
										EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
									}
								}
							}
							#endif
							break;
						case CSHCrop::CROP_RESULT_MANURE:
							#if !defined(_MAPSERVER_)
							{
							}
							#endif
							break;
						case CSHCrop::CROP_RESULT_HARVEST:
							#if !defined(_MAPSERVER_)
							{
								CLI_RequestHarvestropInGarden(NULL, NULL, RESULT_OK_02);
							}
							#endif
							break;
						}

						// DB에 저장
					#if defined(_MAPSERVER_)
						if (bSetToDB)
						{
							SRV_SaveCropInfoToDB(i, pFarm->GetID(), pCrop);
						}
					#endif
					}
					// 이벤트 초기화
					pCrop->InitEvent();
				}
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SaveFarmInfoToDB Method																					   DB에 농장 데이터 저장
//
VOID CSHFarmManager::SRV_SaveFarmInfoToDB(WORD nFarmZone, WORD nFarmID, CSHFarm* pcsFarm)
{
#if defined(_MAPSERVER_)
	char szText[256];
	sprintf(szText, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d",	STORED_FARM_SET_FARMSTATE,
																	nFarmZone, 
																	nFarmID, 
																	pcsFarm->GetOwnState(), 
																	pcsFarm->GetOwner(),
																	pcsFarm->GetGarden(0)->GetGrade(),
																	pcsFarm->GetHouse(0)->GetGrade(),
																	pcsFarm->GetWarehouse(0)->GetGrade(),
																	pcsFarm->GetAnimalCage(0)->GetGrade(),
																	pcsFarm->GetFence(0)->GetGrade());

	g_DB.Query(eQueryType_FreeQuery, eFarm_SetFarmState, 0, szText);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SaveCropInfoToDB Method																					 DB에 농작물 데이터 저장
//
VOID CSHFarmManager::SRV_SaveCropInfoToDB(WORD nFarmZone, WORD nFarmID, CSHCrop* pCrop)
{
#if defined(_MAPSERVER_)
	char szText[256];
	sprintf(szText, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d",	STORED_FARM_SET_CROPINFO, 
																	nFarmZone,
																	nFarmID, 
																	pCrop->GetID(),
																	pCrop->GetOwner(),
																	pCrop->GetKind(), 
																	pCrop->GetStep(), 
																	pCrop->GetLife(), 
																	pCrop->GetNextStepTime(), 
																	pCrop->GetSeedGrade());
	g_DB.Query(eQueryType_FreeQuery, eFarm_SetCropInfo, 0, szText);

	// 점검 후에 농작물이 사라진다는 제보가 있어, 파일로도 데이터를 남긴다.
	// 이 데이터와 서버를 시작할 때 DB에서 로드한 데이터와 비교한다.
	char szFile[256];
	sprintf(szFile, "./Log/FARMCROP_VERIFY_%02d%02d%02d.txt", nFarmZone, nFarmID, pCrop->GetID());
	if (pCrop->GetLife())
	{
		FILE* fp;
		fp = fopen(szFile, "wt");
		if (fp)
		{
			fprintf(fp, "%d %d %d %d %d %d\n",
				pCrop->GetOwner(),
				pCrop->GetStep(),
				pCrop->GetLife(),
				pCrop->GetSeedGrade(),
				pCrop->GetKind(),
				pCrop->GetNextStepTime());
			fclose(fp);
		}
	}
	else
	{
		DeleteFile(szFile);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SetFarmStateFromDBQueue Method															 		  DB큐를 참조하여 농장 상태 설정
//
VOID CSHFarmManager::SRV_SetFarmStateFromDBQueue(CPlayer* pPlayer, PACKET_FARM_STATE_WITH_TAX* pPacket)
{
#if defined(_MAPSERVER_)
	// 범위 체크
	if (pPacket->nFarmZone >= CSHFarmZone::FARM_ZONE_MAX) return;
	if (pPacket->nFarmID >= m_pcsFarmZone[pPacket->nFarmZone]->GetFarmNum()) return;

	CSHFarm* pcsFarm = m_pcsFarmZone[pPacket->nFarmZone]->GetFarm(pPacket->nFarmID);
	pcsFarm->SetOwnState((CSHFarm::OWN_STATE)pPacket->nFarmState);
	pcsFarm->SetOwner(pPacket->nFarmOwner);
	pcsFarm->GetGarden(0)->SetGrade(pPacket->nGardenGrade);
	pcsFarm->GetHouse(0)->SetGrade(pPacket->nHouseGrade);
	pcsFarm->GetWarehouse(0)->SetGrade(pPacket->nWarehouseGrade);
	pcsFarm->GetAnimalCage(0)->SetGrade(pPacket->nAnimalCageGrade);
	pcsFarm->GetFence(0)->SetGrade(pPacket->nFenceGrade);
	pcsFarm->SetTaxArrearageFreq(pPacket->nTaxArrearageFreq);
	if (pPacket->szTaxPayPlayerName[0])
	{
		pcsFarm->SetTaxPayPlayerName(pPacket->szTaxPayPlayerName);
	}

	// 농장 로드 상태를 파일로 남긴다.
	SYSTEMTIME st;
	GetLocalTime(&st);

	char szFile[256];
	sprintf(szFile, "./Log/FARM_LOAD_START_%4d%02d%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	FILE* fp;

	fp = fopen(szFile, "a+");
	if (fp)
	{
		fprintf(fp, "ZONE:%d FARM:%d OWNSTATE:%d OWNER:%d GARDENGRD:%d HOUSEGRD:%d WAREHOUSEGRD:%d ANIMALCAGEGRD:%d FENCEGRD:%d\n",
			pPacket->nFarmZone,
			pcsFarm->GetID(),
			pcsFarm->GetOwnState(),
			pcsFarm->GetOwner(),
			pcsFarm->GetGarden(0)->GetGrade(),
			pcsFarm->GetHouse(0)->GetGrade(),
			pcsFarm->GetWarehouse(0)->GetGrade(),
			pcsFarm->GetAnimalCage(0)->GetGrade(),
			pcsFarm->GetFence(0)->GetGrade());
		fclose(fp);
	}
#endif
}
// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SetCropInfoFromDBQueue Method																 	     DB큐를 참조하여 농작물 설정
//
VOID CSHFarmManager::SRV_SetCropInfoFromDBQueue(CPlayer* pPlayer, PACKET_FARM_GARDEN_CROP_PLANT* pPacket)
{
#if defined(_MAPSERVER_)
	// 범위 체크
	if (pPacket->nFarmZone  >= CSHFarmZone::FARM_ZONE_MAX) return;
	if (pPacket->nFarmID >= m_pcsFarmZone[pPacket->nFarmZone]->GetFarmNum()) return;

	CSHFarm* pcsFarm = m_pcsFarmZone[pPacket->nFarmZone]->GetFarm(pPacket->nFarmID);
	if (pPacket->nGardenID >= pcsFarm->GetGardenNum()) return;

	CSHGarden* pcsGarden = pcsFarm->GetGarden(pPacket->nGardenID);
	if (pPacket->nCropID >= pcsGarden->GetCropNum()) return;

	CSHCrop* pcsCrop = pcsGarden->GetCrop(pPacket->nCropID);

	pcsCrop->SetOwner(pPacket->nCropOwner);
	pcsCrop->SetStep((CSHCrop::CROP_STEP)pPacket->nCropStep);
	pcsCrop->SetLife(pPacket->nCropLife);
	pcsCrop->SetSeedGrade(pPacket->nCropSeedGrade);
	pcsCrop->SetKind(pPacket->nCropKind);
	pcsCrop->SetNextStepTime(pPacket->nCropNextStepTime);

	// 농작물 로드 상태를 파일로 남긴다.
	SYSTEMTIME st;
	GetLocalTime(&st);

	FILE* fp;
	char szFile[256];

	// 파일로 남기는 마지막 농작물의 데이터와 비교한다.
	BOOL bWrongData = FALSE;
	sprintf(szFile, "./Log/FARMCROP_VERIFY_%02d%02d%02d.txt", pPacket->nFarmZone, pPacket->nFarmID, pcsCrop->GetID());
	fp = fopen(szFile, "rt");
	if (fp)
	{
		int nOwner = 0, nStep = 0, nLife = 0, nSeedGrade = 0, nKind = 0, nNextStepTime = 0;
		fscanf(fp, "%d %d %d %d %d %d", 
			&nOwner,
			&nStep,
			&nLife,
			&nSeedGrade,
			&nKind,
			&nNextStepTime);
		fclose(fp);

		if (pcsCrop->GetOwner()			!=	nOwner			||
			pcsCrop->GetStep()			!=	nStep			||
			pcsCrop->GetLife()			!=	nLife			||
			pcsCrop->GetSeedGrade()		!=	nSeedGrade		||
			pcsCrop->GetKind()			!=	nKind			||
			pcsCrop->GetNextStepTime()	!=	nNextStepTime)
		{
			bWrongData = TRUE;
		}
	}
	else
	{
		// DB에서는 가져왔는 데 파일로는 안 남았다?
		bWrongData = 2;
	}

	sprintf(szFile, "./Log/FARMCROP_VERIFY_CHECK_%02d%02d%02d.txt", pPacket->nFarmZone, pPacket->nFarmID, pcsCrop->GetID());
	fp = fopen(szFile, "wt");
	if (fp)
	{
		if (bWrongData == FALSE)		fprintf(fp, "DATA_FROM_FILE and FROM_DB OK!!");
		else if (bWrongData == TRUE)	fprintf(fp, "DATA_FROM_FILE and FROM_DB DATA DIFF!!");
		else if (bWrongData == 2)		fprintf(fp, "FROM_DB Only...??");
		fclose(fp);
	}

	// DB에서 로드한 데이터 기록
	sprintf(szFile, "./Log/FARMCROP_LOAD_START_%4d%02d%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	fp = fopen(szFile, "a+");
	if (fp)
	{
		fprintf(fp, "ZONE:%d FARM:%d CROP:%d OWNER:%d STEP:%d LIFE:%d SEEDGRD:%d KIND:%d NEXTSTEPTIME:%d\n",
			pPacket->nFarmZone,
			pcsFarm->GetID(),
			pcsCrop->GetID(),
			pcsCrop->GetOwner(),
			pcsCrop->GetStep(),
			pcsCrop->GetLife(),
			pcsCrop->GetSeedGrade(),
			pcsCrop->GetKind(),
			pcsCrop->GetNextStepTime());
		fclose(fp);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromClient Method																 클라이언트 요청 분석/처리(에이전트)
//
VOID CSHFarmManager::ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	// 클라이언트에서 패킷 조작으로 남의 캐릭터 ID 를 보낼 수 있기 때문에
	// g_pUserTableForObjectID->FindUser 로 UserInfo 를 구하면 안 됨!!
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionID );
	if( pUserInfo == NULL ) return;
	pTempMsg->dwObjectID = pUserInfo->dwCharacterID;

	switch( pTempMsg->Protocol )
	{
	// 아래 프로토콜은 에이전트->맵서버 이기 때문에 유저로부터의 해킹에 대비해 리턴하도록 한다.
	case MP_FARM_BUY_AGENT_CHECK:
	case MP_FARM_GARDEN_PLANT_AGENT_CHECK:
	case MP_FARM_UPGRADE_AGENT_CHECK_1:
	case MP_FARM_UPGRADE_AGENT_CHECK_2:
	// 080428 KTH 
	case MP_FARM_ANIMAL_INSTALL_AGENT_CHECK:
		return;
	case MP_FARM_PAY_TAX:
		{
			RESULT eResult = RESULT_OK;
 			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// 패밀리 체크
			if (pUserInfo->pcsFamily == NULL)
			{
				// 패밀리가 없으면 패밀리 마스터 ID 는 0
				pPacket->dwData3 = 0;;
			}
			else
			{
				// 권한 체크를 위해 패밀리 마스터 ID 설정
				pPacket->dwData3 = pUserInfo->pcsFamily->Get()->nMasterID;
			}

			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
			break;
		}
		break;
	default:
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromServer Method																	   서버 요청 분석/처리(에이전트)
//
VOID CSHFarmManager::ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	// 농장이 삭제되어 멤버에게 알릴 경우
	if (pTempMsg->Protocol == MP_FARM_DEL_NOTIFY_TO_MEMBER)
	{
		MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

		// 쪽지는 에이전트 하나에서만 보내야 한당.
		if (g_pServerSystem->GetServerNum() == pPacket->dwData2)
		{
			Farm_SendNote_DelFarm(pPacket->dwObjectID);
		}

		CSHFamily* pcsFamily = g_csFamilyManager.GetFamilyByMasterID(pPacket->dwObjectID);
		if (pcsFamily)
		{
			MSG_DWORD2 stPacket;
			for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
			{
				USERINFO* pMemberInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
				if (pMemberInfo == NULL) continue;

				// 메세지
				memcpy(&stPacket, pMsg, dwLength);
				g_Network.Send2User( pMemberInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
		}
		return;
	}


	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL)
	{
		switch( pTempMsg->Protocol )
		{
		// 농장UI 정보
		case MP_FARM_MANAGE_UI_CROP_DATA:
			{
 				PACKET_FARM_GARDEN_CROP_PLANT stPacket;
				memcpy(&stPacket, pMsg, sizeof(PACKET_FARM_GARDEN_CROP_PLANT));
				
				CSHFamily* pcsFamily = g_csFamilyManager.GetFamilyByMasterID(stPacket.nOwner);
				if (pcsFamily)
				{
					for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
					{
						USERINFO* pMemberInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
						if (pMemberInfo == NULL) continue;

						memcpy(&stPacket, pMsg, sizeof(PACKET_FARM_GARDEN_CROP_PLANT));
						g_Network.Send2User( pMemberInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					}
				}
				break;
			}
		// 080416 KTH --
		//case MP_FARM_MANAGE_UI_ANIMAL_DATA:
		//	{
		//	}
		//	break;
		}
		return;
	}

	switch( pTempMsg->Protocol )
	{
	// 농장 구입
	case MP_FARM_BUY_AGENT_CHECK:
		{
			RESULT eResult = RESULT_OK;
 			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// 패미리가 없음.
			if (pUserInfo->pcsFamily == NULL)
			{
				eResult = RESULT_FAIL_05;
				goto FAILED_BUY;
			}

			// 패미리 마스터가 아님
			if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID)
			{
				eResult = RESULT_FAIL_06;
				goto FAILED_BUY;
			}

			// 필요 패밀리 멤버 수 체크
			if (pUserInfo->pcsFamily->Get()->nMemberNum < pPacket->dwData3)
			{
				eResult = RESULT_FAIL_07;
				goto FAILED_BUY;
			}

			// 명예 포인트 부족
			if (pUserInfo->pcsFamily->GetEx()->nHonorPoint < pPacket->dwData4)
			{
				eResult = RESULT_FAIL_08;
				goto FAILED_BUY;
			}

FAILED_BUY:
			PACKET_FARM_STATE			stPacket;

			stPacket.Category			= MP_FARM;
			stPacket.Protocol			= MP_FARM_BUY_AGENT_CHECK;
			stPacket.dwObjectID 		= pUserInfo->dwCharacterID;
			stPacket.nFarmZone			= (WORD)pPacket->dwData1;
			stPacket.nFarmID			= (WORD)pPacket->dwData2;
			stPacket.nResult			= eResult;

			if (eResult == RESULT_OK)
			{
				// 명예 포인트 절감
				g_csFamilyManager.ASRV_UpdateHonorPoint(pUserInfo, (DWORD)(-1*(int)pPacket->dwData4));
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				// 맵서버에 농장UI 정보 요청
				for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
				{
					ASRV_RequestFarmUIInfoToMap(pUserInfo->pcsFamily->GetMember(i)->Get()->nID, pUserInfo->pcsFamily);
				}
			}
			else
			{
				stPacket.Protocol = MP_FARM_BUY;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}
	// 농작물 심기
	case MP_FARM_GARDEN_PLANT_AGENT_CHECK:
		{
			RESULT eResult = RESULT_OK;
 			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;

			// ..패미리가 없음.
			if (pUserInfo->pcsFamily == NULL)
			{
				eResult = RESULT_FAIL_05;
				goto FAILED_PLANT;
			}

			// ..해당 농장의 패밀리 멤버가 아님
			if (pUserInfo->pcsFamily->Get()->nMasterID != pPacket->nOwner)
			{
				eResult = RESULT_FAIL_06;
				goto FAILED_PLANT;
			}

			// 필요 패밀리 멤버 수 체크
			if (pUserInfo->pcsFamily->Get()->nMemberNum < pPacket->nResult)
			{
				eResult = RESULT_FAIL_07;
				goto FAILED_PLANT;
			}

FAILED_PLANT:
			PACKET_FARM_GARDEN_CROP_PLANT stPacket;

			stPacket.Category			= MP_FARM;
			stPacket.Protocol			= MP_FARM_GARDEN_PLANT_AGENT_CHECK;
			stPacket.dwObjectID 		= pUserInfo->dwCharacterID;
			stPacket.nFarmZone			= pPacket->nFarmZone;
			stPacket.nFarmID			= pPacket->nFarmID;
			stPacket.nGardenID			= pPacket->nGardenID;
			stPacket.nCropID			= pPacket->nCropID;
			stPacket.nItemTargetPos		= pPacket->nItemTargetPos;
			stPacket.nItemIdx			= pPacket->nItemIdx;
			stPacket.nResult			= eResult;

			if (eResult == RESULT_OK)
			{
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				stPacket.Protocol = MP_FARM_GARDEN_PLANT;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}

	// 농장 업그레이드1
	case MP_FARM_UPGRADE_AGENT_CHECK_1:
		{
			RESULT eResult = RESULT_OK;
 			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;

			// ..패미리가 없음.
			if (pUserInfo->pcsFamily == NULL)
			{
				eResult = RESULT_FAIL_03;
				goto FAILED_UPGRADE_1;
			}

			// ..패미리 마스터가 아님
			if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID ||
				pUserInfo->dwCharacterID != pPacket->dwData4 )
			{
				eResult = RESULT_FAIL_04;
				goto FAILED_UPGRADE_1;
			}

			// 필요 패밀리 멤버 수 체크
			if (pUserInfo->pcsFamily->Get()->nMemberNum < pPacket->dwData5)
			{
				eResult = RESULT_FAIL_06;
				goto FAILED_UPGRADE_1;
			}

FAILED_UPGRADE_1:
			PACKET_FARM_UPGRADE			stPacket;

			stPacket.Category			= MP_FARM;
			stPacket.Protocol			= MP_FARM_UPGRADE_AGENT_CHECK_1;
			stPacket.dwObjectID 		= pUserInfo->dwCharacterID;
			stPacket.nFarmZone			= (WORD)pPacket->dwData1;
			stPacket.nFarmID			= (WORD)pPacket->dwData2;
			stPacket.nObjKind			= (WORD)pPacket->dwData3;
			stPacket.nResult			= eResult;

			if (eResult == RESULT_OK)
			{
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				stPacket.Protocol = MP_FARM_UPGRADE;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}
	// 농장 업그레이드2
	case MP_FARM_UPGRADE_AGENT_CHECK_2:
		{
			RESULT eResult = RESULT_OK;
 			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;

			// ..패미리가 없음.
			if (pUserInfo->pcsFamily == NULL)
			{
				eResult = RESULT_FAIL_03;
				goto FAILED_UPGRADE_2;
			}

			// ..패미리 마스터가 아님
			if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID)
			{
				eResult = RESULT_FAIL_04;
				goto FAILED_UPGRADE_2;
			}

			// 명예 포인트 부족
			if (pUserInfo->pcsFamily->GetEx()->nHonorPoint < pPacket->dwData4)
			{
				eResult = RESULT_FAIL_05;
				goto FAILED_UPGRADE_2;
			}

			// 필요 패밀리 멤버 수 체크
			if (pUserInfo->pcsFamily->Get()->nMemberNum < pPacket->dwData5)
			{
				eResult = RESULT_FAIL_06;
				goto FAILED_UPGRADE_2;
			}

FAILED_UPGRADE_2:
			PACKET_FARM_UPGRADE			stPacket;

			stPacket.Category			= MP_FARM;
			stPacket.Protocol			= MP_FARM_UPGRADE_AGENT_CHECK_2;
			stPacket.dwObjectID 		= pUserInfo->dwCharacterID;
			stPacket.nFarmZone			= (WORD)pPacket->dwData1;
			stPacket.nFarmID			= (WORD)pPacket->dwData2;
			stPacket.nObjKind			= (WORD)pPacket->dwData3;
			stPacket.nResult			= eResult;

			if (eResult == RESULT_OK)
			{
				// 명예 포인트 절감
				g_csFamilyManager.ASRV_UpdateHonorPoint(pUserInfo, (DWORD)(-1*(int)pPacket->dwData4));
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				stPacket.Protocol = MP_FARM_UPGRADE;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}
	// 농장UI 정보
	case MP_FARM_MANAGE_UI_CROP_DATA:
		{
 			PACKET_FARM_GARDEN_CROP_PLANT stPacket;
			memcpy(&stPacket, pMsg, sizeof(PACKET_FARM_GARDEN_CROP_PLANT));
			
			if (stPacket.nOwner == NULL)
			{
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
			}
			else
			{
				CSHFamily* pcsFamily = g_csFamilyManager.GetFamilyByMasterID(stPacket.nOwner);
				if (pcsFamily)
				{
					for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
					{
						USERINFO* pMemberInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
						if (pMemberInfo == NULL) continue;

						memcpy(&stPacket, pMsg, sizeof(PACKET_FARM_GARDEN_CROP_PLANT));
						g_Network.Send2User( pMemberInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					}
				}
			}
			break;
		}
	// 관리비
	case MP_FARM_PAY_TAX:
		{
			MSG_NAME_DWORD3* pPacket = (MSG_NAME_DWORD3*)pMsg;
			MSG_NAME_DWORD3	 stPacket = *pPacket;

			if (pPacket->dwData1 == RESULT_OK)
			{
				// 관리비를 납부했으면 멤버들에게 알림
				CSHFamily* pcsFamily = pUserInfo->pcsFamily;
				if (pcsFamily)
				{
					for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
					{
						USERINFO* pMemberInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
						if (pMemberInfo == NULL) continue;

						stPacket = *pPacket;
						g_Network.Send2User( pMemberInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					}
				}
			}
			else
			{
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}
	// 가축 넣기
	case MP_FARM_ANIMAL_INSTALL_AGENT_CHECK:
		{
			RESULT eResult = RESULT_OK;
 			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket = (PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT*)pMsg;

			// ..패미리가 없음.
			if (pUserInfo->pcsFamily == NULL)
			{
				eResult = RESULT_FAIL_05;
				goto FAILED_INSTALL_ANIMAL;
			}

			// ..해당 농장의 패밀리 멤버가 아님
			if (pUserInfo->pcsFamily->Get()->nMasterID != pPacket->nOwner)
			{
				eResult = RESULT_FAIL_06;
				goto FAILED_INSTALL_ANIMAL;
			}

			// 필요 패밀리 멤버 수 체크
			if (pUserInfo->pcsFamily->Get()->nMemberNum < pPacket->nResult)
			{
				eResult = RESULT_FAIL_07;
				goto FAILED_INSTALL_ANIMAL;
			}

FAILED_INSTALL_ANIMAL:
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT stPacket;

			stPacket.Category			= MP_FARM;
			stPacket.Protocol			= MP_FARM_ANIMAL_INSTALL_AGENT_CHECK;
			stPacket.dwObjectID 		= pUserInfo->dwCharacterID;
			stPacket.nFarmZone			= pPacket->nFarmZone;
			stPacket.nFarmID			= pPacket->nFarmID;
			stPacket.nAnimalCageID		= pPacket->nAnimalCageID;
			stPacket.nAnimalID			= pPacket->nAnimalID;
			stPacket.nItemTargetPos		= pPacket->nItemTargetPos;
			stPacket.nItemIdx			= pPacket->nItemIdx;
			stPacket.nAnimalContentment = pPacket->nAnimalContentment;
			stPacket.nAnimalInterest	= pPacket->nAnimalInterest;
			stPacket.nResult			= eResult;

			if (eResult == RESULT_OK)
			{
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				stPacket.Protocol = MP_FARM_ANIMAL_INSTALL;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			break;
		}
	default:
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_RequestFarmUIInfoToMap Method																		  	맵서버에 농장UI정보 요청
//
VOID CSHFarmManager::ASRV_RequestFarmUIInfoToMap(DWORD nPlayerID, CSHFamily* pcsFamily)
{
#if defined(_AGENTSERVER)
	MSG_DWORD stPacket;

	stPacket.Category	= MP_FARM;
	stPacket.Protocol	= MP_FARM_MANAGE_UI_FARM_DATA;
	stPacket.dwObjectID	= nPlayerID;
	stPacket.dwData		= pcsFamily->Get()->nMasterID;
	g_Network.Broadcast2MapServer((char*)&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ParseRequestFromClient Method																			클라이언트 요청 분석/처리
//
VOID CSHFarmManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_MAPSERVER_)
	MSGBASE* pPacket = (MSGBASE*)pMsg;

	// 잘못된 플레이어 처리
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pPacket->dwObjectID );
	if( !pPlayer )	
	{
		switch(pPacket->Protocol)
		{
		// 농장UI 정보 - 농장 맵에서 데이터를 보내줘야 하는데, 접속 플레이어가 농장이 아닌 다른 맵에 접속 했을 수 있다. 
		case MP_FARM_MANAGE_UI_FARM_DATA:
			{
				MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
				// 농장 관리비 미납을 알리기 위해 FARM_UI_NOTIFY_KIND_CONN_MAP 를 설정.
				// 정확하게는 맵 접속 시, 농장 구입 시에 관리비 미납 상태를 알리는 것이 맞지만,
				// 농장 구입 후 패밀리 멤버 초대 완료 시에도 이곳이 수행되기도 한다.
				// 추가 작업을 하기엔 크게 중요하지 않은 사항이기에 그냥 이렇게 처리.
				SRV_SendFarmUIInfoToClient(pPacket->dwObjectID, pPacket->dwData, FARM_UI_NOTIFY_KIND_CONN_MAP);
			}
			break;
		}
		return;
	}

	switch(pPacket->Protocol)
	{
	// 농장 구입
	case MP_FARM_BUY:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;
			SRV_BuyFarm(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID);
		}
		break;
	// 농장 구입 - 에이전트 체크 후
	case MP_FARM_BUY_AGENT_CHECK:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;
			SRV_BuyFarm(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, 1);
		}
		break;
	// 농작물 심기
	case MP_FARM_GARDEN_PLANT:
		{
			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;
			SRV_PlantCrop(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nGardenID, pPacket->nCropID, pPacket->nItemTargetPos, pPacket->nItemIdx);
		}
		break;
	// 농작물 심기 - 에이전트 체크 후
	case MP_FARM_GARDEN_PLANT_AGENT_CHECK:
		{
			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;
			SRV_PlantCrop(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nGardenID, pPacket->nCropID, pPacket->nItemTargetPos, pPacket->nItemIdx, 1);
		}
		break;
	// 농작물 비료주기
	case MP_FARM_GARDEN_MANURE:
		{
			PACKET_FARM_GARDEN_CROP_MANURE* pPacket = (PACKET_FARM_GARDEN_CROP_MANURE*)pMsg;
			SRV_ManureCrop(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nGardenID, pPacket->nCropID, pPacket->nItemTargetPos, pPacket->nItemIdx);
		}
		break;
	// 농작물 수확하기
	case MP_FARM_GARDEN_HARVEST:
		{
			PACKET_FARM_GARDEN_CROP_GROW* pPacket = (PACKET_FARM_GARDEN_CROP_GROW*)pMsg;
			SRV_HarvestCrop(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nGardenID, pPacket->nCropID, pPacket->nItemTargetPos, pPacket->nItemIdx, pPacket->nResult);
		}
		break;
	// 농장 등급 업그레이드
	case MP_FARM_UPGRADE:
		{
			PACKET_FARM_UPGRADE* pPacket = (PACKET_FARM_UPGRADE*)pMsg;
			SRV_UpgradeFarm(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nObjKind);
		}
		break;
	// 농장 등급 업그레이드 - 에이전트 체크1 후
	case MP_FARM_UPGRADE_AGENT_CHECK_1:
		{
			PACKET_FARM_UPGRADE* pPacket = (PACKET_FARM_UPGRADE*)pMsg;
			SRV_UpgradeFarm(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nObjKind, 1);
		}
		break;
	// 농장 등급 업그레이드 - 에이전트 체크2 후
	case MP_FARM_UPGRADE_AGENT_CHECK_2:
		{
			PACKET_FARM_UPGRADE* pPacket = (PACKET_FARM_UPGRADE*)pMsg;
			SRV_UpgradeFarm(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nObjKind, 2);
		}
		break;
	// 농장 정보
	case MP_FARM_FARMSTATE_LOAD_FROM_DB:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;
			SRV_SendFarmInfoToClient(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID);
		}
		break;
	// 농장UI 정보
	case MP_FARM_MANAGE_UI_FARM_DATA:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			// 농장 관리비 미납을 알리기 위해 FARM_UI_NOTIFY_KIND_CONN_MAP 를 설정.
			// 정확하게는 맵 접속 시, 농장 구입 시에 관리비 미납 상태를 알리는 것이 맞지만,
			// 농장 구입 후 패밀리 멤버 초대 완료 시에도 이곳이 수행되기도 한다.
			// 추가 작업을 하기엔 크게 중요하지 않은 사항이기에 그냥 이렇게 처리.
			SRV_SendFarmUIInfoToClient(pPacket->dwObjectID, pPacket->dwData, FARM_UI_NOTIFY_KIND_CONN_MAP);
		}
		break;
	// 관리비 납부
	case MP_FARM_PAY_TAX:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			SRV_PayTax(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->dwData1, (WORD)pPacket->dwData2, pPacket->dwData3, pPacket->dwData4);
		}
		break;
	// 080415 KTH
	// 축사에 가축 넣기
	case MP_FARM_ANIMAL_INSTALL:
		{
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket = (PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT*)pMsg;
			SRV_InstallAnimalInAnimalCage(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nAnimalCageID, pPacket->nAnimalID, pPacket->nItemTargetPos, pPacket->nItemIdx, pPacket->nAnimalContentment, pPacket->nAnimalInterest);
		}
		break;
	// 축사에 가축 넣기 에어전트 체크
	case MP_FARM_ANIMAL_INSTALL_AGENT_CHECK:
		{
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket = (PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT*)pMsg;
			SRV_InstallAnimalInAnimalCage(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nAnimalCageID, pPacket->nAnimalID, pPacket->nItemTargetPos, pPacket->nItemIdx, pPacket->nAnimalContentment, pPacket->nAnimalInterest, 1);
		}
		break;
	// 가축 먹이 주기
	case MP_FARM_ANIMAL_FEED:
		{
			PACKET_FARM_CAGE_ANIMAL_FEED* pPacket = (PACKET_FARM_CAGE_ANIMAL_FEED*)pMsg;
			SRV_FeedAnimal(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nAnimalCageID, pPacket->nAnimalID, pPacket->nItemTargetPos, pPacket->nItemIdx);
		}
		break;
	// 축사 청소
	case MP_FARM_ANIMAL_CLEANING:
		{
			PACKET_FARM_CAGE_ANIMAL_FEED* pPacket = (PACKET_FARM_CAGE_ANIMAL_FEED*)pMsg;
			SRV_CleaningAnimal(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nAnimalCageID, pPacket->nAnimalID, pPacket->nItemTargetPos, pPacket->nItemIdx);
		}
		break;
	// 가축 보상
	case MP_FARM_ANIMAL_REWARD:
		{
			PACKET_FARM_CAGE_ANIMAL_REWARD* pPacket = (PACKET_FARM_CAGE_ANIMAL_REWARD*)pMsg;
			SRV_AnimalGetItem(pPlayer, (CSHFarmZone::FARM_ZONE)pPacket->nFarmZone, pPacket->nFarmID, pPacket->nAnimalCageID, pPacket->nAnimalID, pPacket->nResult);
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendFarmInfoToClient Method																		 클라이언트에게 농장 정보 보내기
//
VOID CSHFarmManager::SRV_SendFarmInfoToClient(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID)
{
#if defined(_MAPSERVER_)
	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	CSHFarmManager::PACKET_FARM_STATE				stPacket;
	CSHFarmManager::PACKET_FARM_GARDEN_CROP_PLANT	stPacket2;
	CSHFarmManager::PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT	stPacket3;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_FARMSTATE_LOAD_FROM_DB;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= pFarm->GetID();
	stPacket.nFarmState			= pFarm->GetOwnState();
	stPacket.nFarmOwner			= pFarm->GetOwner();
	stPacket.nGardenGrade		= pFarm->GetGarden(0)->GetGrade();
	stPacket.nHouseGrade		= pFarm->GetHouse(0)->GetGrade();
	stPacket.nWarehouseGrade	= pFarm->GetWarehouse(0)->GetGrade();
	stPacket.nAnimalCageGrade	= pFarm->GetAnimalCage(0)->GetGrade();
	stPacket.nFenceGrade		= pFarm->GetFence(0)->GetGrade();

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));

	for(int k=0; k<pFarm->GetGardenNum(); k++)
	{
		CSHGarden* pGarden = pFarm->GetGarden(k);
		for(int p=0; p<pGarden->GetCropNum(); p++)
		{
			CSHCrop* pCrop = pGarden->GetCrop(p);

			stPacket2.Category 			= MP_FARM;
			stPacket2.Protocol 			= MP_FARM_CROPINFO_LOAD_FROM_DB;
			stPacket2.dwObjectID		= pPlayer->GetID();
			stPacket2.nFarmZone			= (WORD)eFarmZone;
			stPacket2.nFarmID			= pFarm->GetID();
			stPacket2.nGardenID			= pGarden->GetID();
			stPacket2.nCropID			= pCrop->GetID();
			stPacket2.nCropOwner		= pCrop->GetOwner();
			stPacket2.nCropKind			= pCrop->GetKind();
			stPacket2.nCropStep			= pCrop->GetStep();
			stPacket2.nCropLife			= pCrop->GetLife();
			stPacket2.nCropNextStepTime	= pCrop->GetNextStepTime();
			stPacket2.nCropSeedGrade	= pCrop->GetSeedGrade();

			pPlayer->SendMsg(&stPacket2, sizeof(stPacket2));
		}
	}

	// 080417 KTH -- 축사 정보를 전송
	for(int k = 0; k < pFarm->GetAnimalCageNum(); k++)
	{
		CPen* pAnimalCage = pFarm->GetAnimalCage(k);

		for(int p = 0; p < pAnimalCage->GetAnimalNum(); p++)
		{
			CAnimal* pAnimal = pAnimalCage->GetAnimal(p);

			stPacket3.Category				= MP_FARM;
			stPacket3.Protocol				= MP_FARM_ANIMALINFO_LOAD_FROM_DB;
			stPacket3.dwObjectID			= pPlayer->GetID();
			stPacket3.nFarmZone				= (WORD)eFarmZone;
			stPacket3.nAnimalCageID			= pAnimalCage->GetID();
			stPacket3.nFarmID				= pFarm->GetID();
			stPacket3.nAnimalID				= pAnimal->GetID();
			stPacket3.nAnimalOwner			= pAnimal->GetOwner();
			stPacket3.nAnimalKind			= pAnimal->GetKind();
			stPacket3.nAnimalStep			= pAnimal->GetStep();
			stPacket3.nAnimalLife			= pAnimal->GetLife();
			stPacket3.nAnimalNextStepTime	= pAnimal->GetNextStepTime();
			stPacket3.nAnimalContentment	= pAnimal->GetContentment();
			stPacket3.nAnimalInterest		= pAnimal->GetInterest();

			pPlayer->SendMsg(&stPacket3, sizeof(stPacket3));
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendFarmUIInfoToClient Method																		클라이언트에 농장UI정보 전송
//
VOID CSHFarmManager::SRV_SendFarmUIInfoToClient(DWORD nPlayerID, DWORD nFamilyMasterID, FARM_UI_NOTIFY_KIND eKind)
{
#if defined(_MAPSERVER_)
	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		CSHFarmZone* pcsFarmZone = g_csFarmManager.GetFarmZone(i);
		if (pcsFarmZone == NULL) continue;

		for(int j=0; j<pcsFarmZone->GetFarmNum(); j++)
		{
			CSHFarm* pcsFarm = pcsFarmZone->GetFarm(j);
			if (pcsFarm == NULL) continue;

			if (pcsFarm->GetOwner() == nFamilyMasterID)
			{
				CSHFarmManager::PACKET_FARM_STATE_WITH_TAX		stPacket;
				CSHFarmManager::PACKET_FARM_GARDEN_CROP_PLANT	stPacket2;

				stPacket.Category			= MP_FARM;
				stPacket.Protocol			= MP_FARM_MANAGE_UI_FARM_DATA;
				stPacket.dwObjectID			= nPlayerID;
				stPacket.nFarmZone			= i;
				stPacket.nFarmID			= pcsFarm->GetID();
				stPacket.nFarmState			= pcsFarm->GetOwnState();
				stPacket.nGardenGrade		= pcsFarm->GetGarden(0)->GetGrade();
				stPacket.nFenceGrade		= pcsFarm->GetFence(0)->GetGrade();
				stPacket.nAnimalCageGrade	= pcsFarm->GetAnimalCage(0)->GetGrade();
				stPacket.nHouseGrade		= pcsFarm->GetHouse(0)->GetGrade();
				stPacket.nWarehouseGrade	= pcsFarm->GetWarehouse(0)->GetGrade();
				stPacket.nTaxArrearageFreq	= pcsFarm->GetTaxArrearageFreq();
				SafeStrCpy(stPacket.szTaxPayPlayerName, pcsFarm->GetTaxPayPlayerName(), MAX_NAME_LENGTH+1);

				g_Network.Broadcast2AgentServer((char*)&stPacket, sizeof(stPacket));
				// 맵에 접속해서 수행되는 경우라면, 농장 관리비 납부 사실을 체크해서 미납 사실을 알린다.
				if (eKind == FARM_UI_NOTIFY_KIND_CONN_MAP)
				{
					SRV_NotifyTaxNonpayment(nPlayerID, pcsFarm);
				}

				for(int k=0; k<pcsFarm->GetGardenNum(); k++)
				{
					CSHGarden* pcsGarden = pcsFarm->GetGarden(k);
					for(int p=0; p<pcsGarden->GetCropNum(); p++)
					{
						CSHCrop* pcsCrop = pcsGarden->GetCrop(p);

						stPacket2.Category 			= MP_FARM;
						stPacket2.Protocol 			= MP_FARM_MANAGE_UI_CROP_DATA;
						stPacket2.dwObjectID		= nPlayerID;
						stPacket2.nFarmZone			= i;
						stPacket2.nFarmID			= pcsFarm->GetID();
						stPacket2.nGardenID			= pcsGarden->GetID();
						stPacket2.nCropID			= pcsCrop->GetID();
						stPacket2.nCropOwner		= pcsCrop->GetOwner();
						stPacket2.nCropKind			= pcsCrop->GetKind();
						stPacket2.nCropStep			= pcsCrop->GetStep();
						stPacket2.nCropLife			= pcsCrop->GetLife();
						stPacket2.nCropNextStepTime	= pcsCrop->GetNextStepTime();
						stPacket2.nCropSeedGrade	= pcsCrop->GetSeedGrade();
						stPacket2.nOwner			= NULL;

						g_Network.Broadcast2AgentServer((char*)&stPacket2, sizeof(stPacket2));
					}
				}
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendFarmUIInfoToClient Method																		클라이언트에 농장UI정보 전송
//
VOID CSHFarmManager::SRV_SendFarmUIInfoToClient(WORD nFarmZone, WORD nFarmID, WORD nGardenID, CSHCrop* pCrop, DWORD nFarmOwner)
{
#if defined(_MAPSERVER_)
	// 농장UI 정보 전송
	PACKET_FARM_GARDEN_CROP_PLANT stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_MANAGE_UI_CROP_DATA;
	stPacket.dwObjectID			= NULL;
	stPacket.nFarmZone			= nFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nGardenID			= nGardenID;
	stPacket.nCropID			= pCrop->GetID();
	stPacket.nCropOwner			= pCrop->GetOwner();
	stPacket.nCropSeedGrade		= pCrop->GetSeedGrade();
	stPacket.nCropStep			= pCrop->GetStep();
	stPacket.nCropKind			= pCrop->GetKind();
	stPacket.nCropLife			= pCrop->GetLife();
	stPacket.nCropNextStepTime  = pCrop->GetNextStepTime();
	stPacket.nOwner				= nFarmOwner;

	g_Network.Broadcast2AgentServer((char*)&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendFarmUIInfoToClient Method															   클라이언트의 맵접속 시 농장 정보 전송
//
VOID CSHFarmManager::SRV_SendPlayerFarmInfo(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	MSG_DWORD3 msg;

	// 심기 딜레이 시간
	if (pPlayer->GetFarmInfo()->nCropPlantRetryTime)
	{
		msg.Category = MP_FARM;
		msg.Protocol = MP_FARM_TIMEDELAY;
		msg.dwData1	 = FARM_TIMEDELAY_KIND_PLANT;
		msg.dwData2	 = CSHCrop::CROP_PLANT_RETRY_TIME;
		msg.dwData3	 = pPlayer->GetFarmInfo()->nCropPlantRetryTime;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}

	// 비료주기 딜레이 시간
	if (pPlayer->GetFarmInfo()->nCropManureRetryTime)
	{
		msg.Category = MP_FARM;
		msg.Protocol = MP_FARM_TIMEDELAY;
		msg.dwData1	 = FARM_TIMEDELAY_KIND_MANURE;
		msg.dwData2	 = CSHCrop::CROP_MANURE_RETRY_TIME;
		msg.dwData3	 = pPlayer->GetFarmInfo()->nCropManureRetryTime;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_GetFarmRenderObjList Method																			 렌더링 오브젝트 리스트 얻기
//
CSHFarmManager::stFARMRENDEROBJLIST* CSHFarmManager::CLI_GetFarmRenderObjList(int nID)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	int nCnt = 0;
	while(m_pstFarmRenderObjList[nCnt].nID)
	{
		if (m_pstFarmRenderObjList[nCnt].nID == nID)
			return &m_pstFarmRenderObjList[nCnt];

		nCnt++;
	}
#endif
	return NULL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_SetTargetCrop Method																				   조작할 농장 오브젝트 선택
//
int CSHFarmManager::CLI_SetTargetObj(CSHFarmRenderObj* pFarmRenderObj, BOOL bShowDlg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	m_pcsSelFarmObj = pFarmRenderObj;
 	if (m_pcsSelFarmObj)
	{
		int nFarmZone = 0;
		int nObjKind = GetRenderObjKind(pFarmRenderObj->GetInfo()->nKind);
		int nObjID = pFarmRenderObj->GetInfo()->nID;


		BOOL bResult = GetFarmZoneID(pFarmRenderObj->GetInfo()->nMapNum, &nFarmZone);
		if (bResult == FALSE)
		{
			return 0;
		}

		m_stSelFarmObj.nFarmZone = nFarmZone;

		switch(nObjKind)
		{
		// 창고
		case RENDER_WAREHOUSE_KIND:
		// 집
		case RENDER_HOUSE_KIND:
		// 축사
		case RENDER_ANIMALCAGE_KIND:
		// 울타리
		case RENDER_FENCE_KIND:
		// 기본 울타리
		case RENDER_BASIC_FENCE:
		// 농장 판매 팻말
		case RENDER_SALEBOARD_KIND:
		// 집사
		case RENDER_STEWARD_KIND:
			{
				m_stSelFarmObj.nFarmID		= nObjID/100-1;

				if (bShowDlg)
				{
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if (pGuageDlg)
					{
						pGuageDlg->GetMonsterGuage()->SetActive(FALSE);
					}
				}
			}
			break;
		// 텃밭흙더미
		case RENDER_GARDENPATCH_KIND:
		case RENDER_BASIC_GARDENPATCH_KIND:
			{
  				m_stSelFarmObj.nFarmID		= nObjID/100-1;
				m_stSelFarmObj.nGardenID	= 0;
				m_stSelFarmObj.nCropID		= (nObjID%10000)-(m_stSelFarmObj.nFarmID+1)*100-1;

				// 심기 딜레이 게이지 설정
				CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
				if (pGuageDlg)
				{
 					CSHFarmZone*	pcsFarmZone		= m_pcsFarmZone[m_stSelFarmObj.nFarmZone];
					CSHFarm*		pcsFarm			= pcsFarmZone->GetFarm(m_stSelFarmObj.nFarmID);
					CSHGarden*		pcsGarden		= pcsFarm->GetGarden(0);
					CSHCrop*		pcsCrop			= pcsGarden->GetCrop(m_stSelFarmObj.nCropID);

					if (bShowDlg)
					{
						if (pcsCrop->GetStep() == CSHCrop::CROP_STEP_SEEDING)
						{
							pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						}
						else
						{
							pGuageDlg->GetMonsterGuage()->SetActive(FALSE);
						}
					}
				}
			}
			break;
		// 축사 슬롯
		case RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND:
			{
				m_stSelFarmObj.nFarmID			= nObjID/100-1;
				m_stSelFarmObj.nAnimalCageID	= 0;
				m_stSelFarmObj.nAnimalID		= (nObjID%10000)-(m_stSelFarmObj.nFarmID+1)*100-1;

				// 심기 딜레이 게이지 설정
				CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
				if (pGuageDlg)
				{
 					CSHFarmZone*	pcsFarmZone		= m_pcsFarmZone[m_stSelFarmObj.nFarmZone];
					CSHFarm*		pcsFarm			= pcsFarmZone->GetFarm(m_stSelFarmObj.nFarmID);
					CPen*			pcsPen			= pcsFarm->GetAnimalCage(0);
					CAnimal*		pcsAnimal		= pcsPen->GetAnimal(m_stSelFarmObj.nAnimalID);

					if (bShowDlg)
					{
						if (pcsAnimal->GetStep() == CAnimal::ANIMAL_STEP_IN_STALL)
						{
							pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						}
						else
						{
							pGuageDlg->GetMonsterGuage()->SetActive(FALSE);
							// 여기 쯤에 다이얼로그 불러 오면 될거 같음...;
						}
					}
				}
			}
			break;
		// 가축 설정
		case RENDER_ANIMAL_KIND:
			{
				m_stSelFarmObj.nFarmID			= nObjID/100-1;
				m_stSelFarmObj.nAnimalCageID	= 0;
				// 080618 Fix nAnimalCageID -> nFarmID
				m_stSelFarmObj.nAnimalID		= (nObjID%10000)-(m_stSelFarmObj.nFarmID+1)*100-1;

				if( bShowDlg )
				{
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if( pGuageDlg )
					{
						CSHFarmZone*	pcsFarmZone		= m_pcsFarmZone[m_stSelFarmObj.nFarmZone];
						CSHFarm*		pcsFarm			= pcsFarmZone->GetFarm(m_stSelFarmObj.nFarmID);
						CPen*			pcsAnimalCage	= pcsFarm->GetAnimalCage(0);
						CAnimal*		pcsAnimal		= pcsAnimalCage->GetAnimal(m_stSelFarmObj.nAnimalID);

						pGuageDlg->SetActive(TRUE);
						pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						pGuageDlg->SetMonsterLife(pcsAnimal->GetLife(), pcsAnimal->GetMaxLife());
						// 이름 설정
						pGuageDlg->SetName(pcsAnimal->GetRenderObj(), CAnimal::ANIMAL_NAME_TBL[pcsAnimal->GetKind()]);
					}
				}
			}
			break;
		// 농작물 설정
		case RENDER_CROP_KIND:
			{
				m_stSelFarmObj.nFarmID		= nObjID/100-1;
				m_stSelFarmObj.nGardenID	= 0;
				m_stSelFarmObj.nCropID		= (nObjID%10000)-(m_stSelFarmObj.nFarmID+1)*100-1;

				// 농작물 생명력 게이지 설정
				if (bShowDlg)
				{
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if (pGuageDlg)
					{
 						CSHFarmZone*	pcsFarmZone		= m_pcsFarmZone[m_stSelFarmObj.nFarmZone];
						CSHFarm*		pcsFarm			= pcsFarmZone->GetFarm(m_stSelFarmObj.nFarmID);
						CSHGarden*		pcsGarden		= pcsFarm->GetGarden(0);
						CSHCrop*		pcsCrop			= pcsGarden->GetCrop(m_stSelFarmObj.nCropID);

						pGuageDlg->SetActive(TRUE);
						pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						pGuageDlg->SetMonsterLife(pcsCrop->GetLife(), pcsCrop->GetMaxLife());
						pGuageDlg->SetName(pcsCrop->GetRenderObj(), m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(pcsCrop->GetKind()-1)+pcsCrop->GetStep()-CSHCrop::CROP_STEP_1].szName);
					}
				}
			}
			break;
		}

		return nObjKind;
	}

#endif
	return 0;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_SetFarmObjForRender Method																	 화면 출력을 위한 농장 오브젝트 설정
//
VOID CSHFarmManager::CLI_SetFarmObjForRender(CSHFarmRenderObj* pFarmRenderObj)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	CSHFarmRenderObj* pcsLastSetFarmObj = m_pcsSelFarmObj;
	// m_stSelFarmObj 를 설정하기 위해 아래 함수 사용
	CLI_SetTargetObj(pFarmRenderObj, FALSE);

 	CSHFarmZone*	pcsFarmZone		= m_pcsFarmZone[m_stSelFarmObj.nFarmZone];
	CSHFarm*		pcsFarm			= pcsFarmZone->GetFarm(m_stSelFarmObj.nFarmID);
	CSHGarden*		pcsGarden		= pcsFarm->GetGarden(0);
	CSHFarmObj*		pcsFence		= pcsFarm->GetFence(0);
	//CSHFarmObj*		pcsAnimalCage	= pcsFarm->GetAnimalCage(0);
	CSHFarmObj*		pcsHouse		= pcsFarm->GetHouse(0);
	CSHFarmObj*		pcsWarehouse	= pcsFarm->GetWarehouse(0);

	CSHCrop*		pcsCrop			= pcsGarden->GetCrop(m_stSelFarmObj.nCropID);

	// 080408 KTH 
	CPen*			pcsPen			= pcsFarm->GetAnimalCage(0);
	CAnimal*		pcsAnimal		= pcsPen->GetAnimal(m_stSelFarmObj.nAnimalID);

	// 오브젝트의 종류에 따라 출력 오브젝트 설정
	int nKind = GetRenderObjKind(pFarmRenderObj->GetInfo()->nKind);

 	switch(nKind)
	{
	// ..창고
	case RENDER_WAREHOUSE_KIND:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsWarehouse->SetRenderObj(pFarmRenderObj);
		break;
	// ..집
	case RENDER_HOUSE_KIND:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsHouse->SetRenderObj(pFarmRenderObj);
		break;
	// ..축사
	case RENDER_ANIMALCAGE_KIND:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		//pFarmRenderObj->GetEngineObject()->
		pcsPen->SetRenderObj(pFarmRenderObj);
		break;
	// ..울타리
	case RENDER_FENCE_KIND:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsFence->SetRenderObj(pFarmRenderObj);
		break;
	// ..판매 팻말
	case RENDER_SALEBOARD_KIND:
		pcsFarm->SetRenderObjEx(pFarmRenderObj, 0);
		break;
	// ..기본 울타리
	case RENDER_BASIC_FENCE:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsFarm->SetRenderObjEx(pFarmRenderObj, 1);
		break;
	// ..집사
	case RENDER_STEWARD_KIND:
		pcsFarm->SetRenderObjEx(pFarmRenderObj, 2);
		break;
	// ..텃밭흙더미
	// ..텃밭은 텃밭 자체의 그래픽이 아니고 농작물 개수만큼의 흙더미 그래픽으로 표현하므로 Ex 함수를 사용
	case RENDER_GARDENPATCH_KIND:
		pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsGarden->SetRenderObjEx(pFarmRenderObj, m_stSelFarmObj.nCropID);
		break;
	case RENDER_BASIC_GARDENPATCH_KIND:
		if (pcsCrop->GetStep() > 0) pFarmRenderObj->GetEngineObject()->DisablePick();
		pcsGarden->SetRenderObjEx(pFarmRenderObj, m_stSelFarmObj.nCropID+15);
		break;
	// ..농작물
	case RENDER_CROP_KIND:
		// 이름 변경
		{
			char* pszName = m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(pcsCrop->GetKind()-1)+pcsCrop->GetStep()-CSHCrop::CROP_STEP_1].szName;
 			pFarmRenderObj->SetName(pszName);
			pFarmRenderObj->GetObjectBalloon()->GetOBalloonName()->SetName(pszName);
			pcsCrop->SetRenderObj(pFarmRenderObj);
			if (pcsCrop->GetStep() > 0) pcsGarden->GetRenderObjEx(pcsCrop->GetID()+15)->GetEngineObject()->DisablePick();
		}
		break;
	//case RENDER_ANIMAL_CAGE_SLOT_KIND:
	//	pFarmRenderObj->GetEngineObject()->DisablePick();
	//	pcsPen->SetRenderObjEx(pFarmRenderObj, m_stSelFarmObj.nAnimalID);
	//	break;
	case RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND:
		{
			if( pcsAnimal->GetStep() > 0 ) 
			{
				pFarmRenderObj->GetEngineObject()->DisablePick();
			}
			else
			{
				pFarmRenderObj->GetEngineObject()->Show();
			}
			pcsPen->SetRenderObjEx(pFarmRenderObj, m_stSelFarmObj.nAnimalID+5);
		}
		break;
	case RENDER_ANIMAL_KIND:
		{
			/*if( m_stSelFarmObj.nAnimalID  )
			{
				m_stSelFarmObj.nAnimalID
				OBJECTMGR->GetSelectedObject()->DisablePick();
				OBJECTMGR->GetSelectedObject()->GetEngineObject()->Hide();
			}*/
			 if( pcsAnimal ) 
			{
				pcsPen->GetRenderObjEx(m_stSelFarmObj.nAnimalID+5)->GetEngineObject()->DisablePick();
				pcsPen->GetRenderObjEx(m_stSelFarmObj.nAnimalID+5)->GetEngineObject()->Hide();
				//OBJECTMGR->AddGarbageObject(pcsPen->GetRenderObjEx(pcsAnimal->GetID()+5));
			}

			pFarmRenderObj->SetName(CAnimal::ANIMAL_NAME_TBL[pcsAnimal->GetKind()]);
			pFarmRenderObj->GetObjectBalloon()->GetOBalloonName()->SetName(CAnimal::ANIMAL_NAME_TBL[pcsAnimal->GetKind()]);
			pcsAnimal->SetRenderObj(pFarmRenderObj);
		}
		break;
	}

	// 원래 선택되어 있던 obj 복구
	CLI_SetTargetObj(pcsLastSetFarmObj, FALSE);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_AddRenderFarmObjToScreen Method																		 화면에 렌더링 오브젝트 추가
//
VOID CSHFarmManager::CLI_AddRenderFarmObjToScreen(CSHFarm* pFarm, int nTarget)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	BASEOBJECT_INFO		binfo;
	BASEMOVE_INFO		minfo;
	CSHFarmRenderObj::stFarmRenderObjInfo stFarmRenderObj;

	int nCnt = 0;
	DWORD n = 0;
	while(m_pstBFarm[nCnt].nID)
	{
		stFarmRenderObj.nKind = m_pstBFarm[nCnt].nKind;
		SafeStrCpy( binfo.ObjectName, m_pstBFarm[nCnt].szName, MAX_NAME_LENGTH+1 );
		stFarmRenderObj.nID = m_pstBFarm[nCnt].nID + pFarm->GetID()*100;
		stFarmRenderObj.nMapNum = m_pstBFarm[nCnt].nMapNum;

		if (nTarget)
		{
			if (nTarget != stFarmRenderObj.nKind)
			{
				goto NEXT;
			}
		}

		// 비어있는 농장이라면 팻말과 기본 울타리만 추가
		if (pFarm->GetOwnState() == CSHFarm::OWN_STATE_EMPTY)
		{
			if (stFarmRenderObj.nKind == RENDER_SALEBOARD_KIND ||
				stFarmRenderObj.nKind == RENDER_BASIC_FENCE)
			{
			}
			else
			{
				goto NEXT;
			}
		}
		// 농장이 활성화 되었으면 팻말과 기본 울타리 제외
		else
		{
			if (stFarmRenderObj.nKind == RENDER_SALEBOARD_KIND ||
				stFarmRenderObj.nKind == RENDER_BASIC_FENCE)
			{
				goto NEXT;
			}
		}
	
		float fDir;
		int nFarmObjID = 0;
 		if (stFarmRenderObj.nKind == RENDER_GARDENPATCH_KIND)
		{
			nFarmObjID = (stFarmRenderObj.nID%10000)-stFarmRenderObj.nID/100*100-1;
			minfo.CurPosition.x = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->x;
			minfo.CurPosition.y = 0;
 			minfo.CurPosition.z = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->z;
			fDir = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetDir();

			stFarmRenderObj.nKind += pFarm->GetGarden(0)->GetGrade()-CSHCrop::GRADE_1;
		}
		//else if ( stFarmRenderObj.nKind == RENDER_ANIMAL_CAGE_SLOT_KIND )
		//{
		//	nFarmObjID = (stFarmRenderObj.nID%10000)-stFarmRenderObj.nID/100*100-1;
		//	minfo.CurPosition.x = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetPos()->x;
		//	minfo.CurPosition.y = 0;
		//	minfo.CurPosition.z = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetPos()->z;
		//	fDir = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetDir();

		//	stFarmRenderObj.nKind += pFarm->GetAnimalCage(0)->GetGrade()-CAnimal::GRADE_1;
		//}
 		else if (stFarmRenderObj.nKind == RENDER_CROP_KIND)
		{
			goto NEXT;
/*
			nFarmObjID = (stFarmRenderObj.nID%10000)-stFarmRenderObj.nID/100*100-1;
			minfo.CurPosition.x = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->x;
			minfo.CurPosition.y = 0;
 			minfo.CurPosition.z = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->z;
			fDir = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetDir();

			int nStep = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetStep() - CSHCrop::CROP_STEP_1;
			// 농작물이 심어져 있지 않으면 건너뛴다.
			if (nStep < 0) 
			{
				goto NEXT;
			}

			if (nStep > CSHCrop::CROP_STEP_MAX - 1) nStep = CSHCrop::CROP_STEP_MAX - 1;
			stFarmRenderObj.nKind += ((pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetKind()-1)*CSHCrop::CROP_STEP_MAX + nStep);
			fDir = DEGTORAD(CSHMath::GetRandomNum(1, 20000));
*/
		}
		else if (stFarmRenderObj.nKind == RENDER_BASIC_GARDENPATCH_KIND)
		{
			nFarmObjID = (stFarmRenderObj.nID%10000)-stFarmRenderObj.nID/100*100-1;
			minfo.CurPosition.x = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->x;
			minfo.CurPosition.y = 0;
 			minfo.CurPosition.z = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetPos()->z;
			fDir = pFarm->GetGarden(0)->GetCrop(nFarmObjID)->GetDir();
		}
		else if( stFarmRenderObj.nKind == RENDER_ANIMAL_KIND )
		{
			goto NEXT;
		}
		else if (stFarmRenderObj.nKind == RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND)
		{
			nFarmObjID = (stFarmRenderObj.nID%10000)-stFarmRenderObj.nID/100*100-1;
			minfo.CurPosition.x = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetPos()->x;
			minfo.CurPosition.y = 0;
			minfo.CurPosition.z = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetPos()->z;
			fDir = pFarm->GetAnimalCage(0)->GetAnimal(nFarmObjID)->GetDir();
		}
		else if (stFarmRenderObj.nKind == RENDER_SALEBOARD_KIND ||
				 stFarmRenderObj.nKind == RENDER_STEWARD_KIND)
		{
			minfo.CurPosition.x = pFarm->GetFence(0)->GetPos()->x;
			minfo.CurPosition.y = 0;
 			minfo.CurPosition.z = pFarm->GetFence(0)->GetPos()->z;
 			fDir = pFarm->GetFence(0)->GetDir();
			if (stFarmRenderObj.nKind == RENDER_STEWARD_KIND) fDir += 1.5f;
		}
		else
		{
			minfo.CurPosition.x = pFarm->GetPos()->x;
			minfo.CurPosition.y = 0;
 			minfo.CurPosition.z = pFarm->GetPos()->z;
			fDir = pFarm->GetDir();

			// 등급에 따른 오브젝트 종류 설정
 			switch(stFarmRenderObj.nKind)
			{
			// ..창고
			case RENDER_WAREHOUSE_KIND:
 				stFarmRenderObj.nKind += pFarm->GetWarehouse(0)->GetGrade()-CSHCrop::GRADE_1;
				break;
			// ..집
			case RENDER_HOUSE_KIND:
				stFarmRenderObj.nKind += pFarm->GetHouse(0)->GetGrade()-CSHCrop::GRADE_1;
				break;
			// ..축사
			case RENDER_ANIMALCAGE_KIND:
				stFarmRenderObj.nKind += pFarm->GetAnimalCage(0)->GetGrade()-CSHCrop::GRADE_1;
				break;
			// ..울타리
			case RENDER_FENCE_KIND:
				stFarmRenderObj.nKind += pFarm->GetFence(0)->GetGrade()-CSHCrop::GRADE_1;
				break;
			}
		}


		binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		while(OBJECTMGR->GetObject(binfo.dwObjectID))
		{
			binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		}

		binfo.ObjectState = 0;
		minfo.bMoving = FALSE;
		minfo.KyungGongIdx = 0;
		minfo.MoveMode = 0;

		CSHFarmRenderObj* pFarmRenderObj = OBJECTMGR->AddFarmRenderObj(&binfo, &minfo, &stFarmRenderObj);

 		pFarmRenderObj->SetAngle(fDir);
		CLI_SetFarmObjForRender(pFarmRenderObj);
NEXT:
		nCnt++;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RemoveRenderFarmObjFromScreen Method															   화면에서 렌더링 오브젝트 제거
//
VOID CSHFarmManager::CLI_RemoveRenderFarmObjFromScreen(CSHFarm* pFarm, int nTarget)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (nTarget)
	{
		switch(nTarget)
		{
		// 농작물
		case RENDER_CROP_KIND :
			for(int i=0; i<15; i++)
			{
				if (pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj())
				{
					if((CObject*)pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj() == OBJECTMGR->GetSelectedObject())
					{
						OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
						OBJECTMGR->GetSelectedObject()->OnDeselected();
						OBJECTMGR->SetSelectedObjectID(0);
					}

					OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj());
					pFarm->GetGarden(0)->GetCrop(i)->SetRenderObj(NULL);						
				}
			}
			break;
		// 흙더미
		case RENDER_GARDENPATCH_KIND :	
			for(int i=0; i<15; i++)
			{
				if (pFarm->GetGarden(0)->GetRenderObjEx(i))
				{
					if((CObject*)pFarm->GetGarden(0)->GetRenderObjEx(i) == OBJECTMGR->GetSelectedObject())
					{
						OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
						OBJECTMGR->GetSelectedObject()->OnDeselected();
						OBJECTMGR->SetSelectedObjectID(0);
					}

					OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetRenderObjEx(i));
					pFarm->GetGarden(0)->SetRenderObjEx(NULL, i);
				}
			}
			break;
		// 기본 흙더미
		case RENDER_BASIC_GARDENPATCH_KIND :	
			for(int i=15; i<30; i++)
			{
				if (pFarm->GetGarden(0)->GetRenderObjEx(i))
				{
					if((CObject*)pFarm->GetGarden(0)->GetRenderObjEx(i) == OBJECTMGR->GetSelectedObject())
					{
						OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
						OBJECTMGR->GetSelectedObject()->OnDeselected();
						OBJECTMGR->SetSelectedObjectID(0);
					}

					OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetRenderObjEx(i));
					pFarm->GetGarden(0)->SetRenderObjEx(NULL, i);
				}
			}
			break;
		//case RENDER_ANIMAL_CAGE_SLOT_KIND:
		//	for(int i=0; i<5; i++)
		//	{
		//		if( pFarm->GetAnimalCage(0)->GetRenderObjEx(i) )
		//		{
		//			OBJECTMGR->AddGarbageObject(pFarm->GetAnimalCage(0)->GetRenderObjEx(i));
		//			pFarm->GetAnimalCage(0)->SetRenderObjEx(NULL, i);
		//		}
		//	}
		//	break;
		case RENDER_HOUSE_KIND :			
			// 집
			if(pFarm->GetHouse(0)->GetRenderObj() && ((CObject*)pFarm->GetHouse(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
			{
				OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				OBJECTMGR->GetSelectedObject()->OnDeselected();
				OBJECTMGR->SetSelectedObjectID(0);
			}

			OBJECTMGR->AddGarbageObject(pFarm->GetHouse(0)->GetRenderObj());
			pFarm->GetHouse(0)->SetRenderObj(NULL);
			break;
		case RENDER_WAREHOUSE_KIND :
			// 창고
			if(pFarm->GetWarehouse(0)->GetRenderObj() && ((CObject*)pFarm->GetWarehouse(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
			{
				OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				OBJECTMGR->GetSelectedObject()->OnDeselected();
				OBJECTMGR->SetSelectedObjectID(0);
			}

			OBJECTMGR->AddGarbageObject(pFarm->GetWarehouse(0)->GetRenderObj());
			pFarm->GetWarehouse(0)->SetRenderObj(NULL);
			break;
		case RENDER_ANIMALCAGE_KIND :	
			// 축사
			if(pFarm->GetAnimalCage(0)->GetRenderObj() && ((CObject*)pFarm->GetAnimalCage(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
			{
				OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				OBJECTMGR->GetSelectedObject()->OnDeselected();
				OBJECTMGR->SetSelectedObjectID(0);
			}

			OBJECTMGR->AddGarbageObject(pFarm->GetAnimalCage(0)->GetRenderObj());
			pFarm->GetAnimalCage(0)->SetRenderObj(NULL);
			break;
		case RENDER_FENCE_KIND :			
			// 울타리
			if(pFarm->GetFence(0)->GetRenderObj() && ((CObject*)pFarm->GetFence(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
			{
				OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				OBJECTMGR->GetSelectedObject()->OnDeselected();
				OBJECTMGR->SetSelectedObjectID(0);
			}

			OBJECTMGR->AddGarbageObject(pFarm->GetFence(0)->GetRenderObj());
			pFarm->GetFence(0)->SetRenderObj(NULL);
			break;
		}
	}
	else
	{
		for(int i=0; i<15; i++)
		{
			// 농작물
			if (pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj())
			{
				if((CObject*)pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj() == OBJECTMGR->GetSelectedObject())
				{
					OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
					OBJECTMGR->GetSelectedObject()->OnDeselected();
					OBJECTMGR->SetSelectedObjectID(0);
				}

				OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetCrop(i)->GetRenderObj());
				pFarm->GetGarden(0)->GetCrop(i)->SetRenderObj(NULL);
			}
		}

		for(int i=0; i<15; i++)
		{
			// 흙더미
			if (pFarm->GetGarden(0)->GetRenderObjEx(i))
			{
				if((CObject*)pFarm->GetGarden(0)->GetRenderObjEx(i) == OBJECTMGR->GetSelectedObject())
				{
					OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
					OBJECTMGR->GetSelectedObject()->OnDeselected();
					OBJECTMGR->SetSelectedObjectID(0);
				}

				OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetRenderObjEx(i));
				pFarm->GetGarden(0)->SetRenderObjEx(NULL, i);
			}
			if (pFarm->GetGarden(0)->GetRenderObjEx(i+15))
			{
				if((CObject*)pFarm->GetGarden(0)->GetRenderObjEx(i+15) == OBJECTMGR->GetSelectedObject())
				{
					OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
					OBJECTMGR->GetSelectedObject()->OnDeselected();
					OBJECTMGR->SetSelectedObjectID(0);
				}

				OBJECTMGR->AddGarbageObject(pFarm->GetGarden(0)->GetRenderObjEx(i+15));
				pFarm->GetGarden(0)->SetRenderObjEx(NULL, i+15);
			}
		}

		
		for(int i = 0; i < 5; i++ )
		{
			if( pFarm->GetAnimalCage(0)->GetAnimal(i)->GetRenderObj())
			{
				OBJECTMGR->AddGarbageObject(pFarm->GetAnimalCage(0)->GetAnimal(i)->GetRenderObj());
				pFarm->GetAnimalCage(0)->GetAnimal(i)->SetRenderObj(NULL);
			}
		}
	
		for(int i=0; i<5; i++)
		{
			if( pFarm->GetAnimalCage(0)->GetRenderObjEx(i) )
			{
				OBJECTMGR->AddGarbageObject(pFarm->GetAnimalCage(0)->GetRenderObjEx(i));
				pFarm->GetAnimalCage(0)->SetRenderObjEx(NULL, i);
			}
		}

		// 집
		if(pFarm->GetHouse(0)->GetRenderObj() && ((CObject*)pFarm->GetHouse(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetHouse(0)->GetRenderObj());
		pFarm->GetHouse(0)->SetRenderObj(NULL);
		
		// 창고
		if(pFarm->GetWarehouse(0)->GetRenderObj() && ((CObject*)pFarm->GetWarehouse(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetWarehouse(0)->GetRenderObj());
		pFarm->GetWarehouse(0)->SetRenderObj(NULL);
		
		// 축사
		if(pFarm->GetAnimalCage(0)->GetRenderObj() && ((CObject*)pFarm->GetAnimalCage(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetAnimalCage(0)->GetRenderObj());
		pFarm->GetAnimalCage(0)->SetRenderObj(NULL);
		
		// 울타리
		if(pFarm->GetFence(0)->GetRenderObj() && ((CObject*)pFarm->GetFence(0)->GetRenderObj() == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetFence(0)->GetRenderObj());
		pFarm->GetFence(0)->SetRenderObj(NULL);

		// 팻말
		if(pFarm->GetRenderObjEx(0) && ((CObject*)pFarm->GetRenderObjEx(0) == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
 		OBJECTMGR->AddGarbageObject(pFarm->GetRenderObjEx(0));
		pFarm->SetRenderObjEx(NULL, 0);

		// 기본 울타리
		if(pFarm->GetRenderObjEx(1) && ((CObject*)pFarm->GetRenderObjEx(1) == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetRenderObjEx(1));
		pFarm->SetRenderObjEx(NULL, 1);

		// 집사
		if(pFarm->GetRenderObjEx(2) && ((CObject*)pFarm->GetRenderObjEx(2) == OBJECTMGR->GetSelectedObject()))
		{
			OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
			OBJECTMGR->GetSelectedObject()->OnDeselected();
			OBJECTMGR->SetSelectedObjectID(0);
		}
		OBJECTMGR->AddGarbageObject(pFarm->GetRenderObjEx(2));
		pFarm->SetRenderObjEx(NULL, 2);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_AddRenderFarmCropObjToScreen Method															  화면에 렌더링 농작물 오브젝트 추가
//
VOID CSHFarmManager::CLI_AddRenderFarmCropObjToScreen(CSHCrop* pCrop)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (pCrop->GetStep() < CSHCrop::CROP_STEP_1) return;

	BASEOBJECT_INFO		binfo;
	BASEMOVE_INFO		minfo;
	CSHFarmRenderObj::stFarmRenderObjInfo stFarmRenderObj;

	int nCnt = 0;
	DWORD n = 0;
	while(m_pstBFarm[nCnt].nID)
	{
		if (m_pstBFarm[nCnt].nKind != RENDER_CROP_KIND)
		{
			nCnt++;
			continue;
		}

		stFarmRenderObj.nKind = m_pstBFarm[nCnt].nKind;
		int nStep = pCrop->GetStep() - CSHCrop::CROP_STEP_1;
		if (nStep < 0) nStep = 0;
		if (nStep > CSHCrop::CROP_STEP_MAX - 1) nStep = CSHCrop::CROP_STEP_MAX - 1;
		stFarmRenderObj.nKind += ((pCrop->GetKind()-1)*100 + nStep);

		SafeStrCpy( binfo.ObjectName, m_pstBFarm[nCnt].szName, MAX_NAME_LENGTH+1 );
		stFarmRenderObj.nID = (pCrop->GetParent()->GetParent()->GetID()+1)*100 + pCrop->GetID()+1;
		stFarmRenderObj.nMapNum = m_pstBFarm[nCnt].nMapNum;

		binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		while(OBJECTMGR->GetObject(binfo.dwObjectID))
		{
			binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		}

		binfo.ObjectState = 0;
		minfo.bMoving = FALSE;
		minfo.KyungGongIdx = 0;
		minfo.MoveMode = 0;

		float fDir = DEGTORAD(CSHMath::GetRandomNum(1, 20000));

		minfo.CurPosition.x = pCrop->GetPos()->x;
		minfo.CurPosition.y = 0;
 		minfo.CurPosition.z = pCrop->GetPos()->z;
		pCrop->GetDir();


		CSHFarmRenderObj* pFarmRenderObj = OBJECTMGR->AddFarmRenderObj(&binfo, &minfo, &stFarmRenderObj);

 		pFarmRenderObj->SetAngle(fDir);
		CLI_SetFarmObjForRender(pFarmRenderObj);
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RemoveRenderFarmCropObjToScreen Method														  화면에 렌더링 농작물 오브젝트 제거
//
VOID CSHFarmManager::CLI_RemoveRenderFarmCropObjToScreen(CSHCrop* pCrop)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	// 농작물
	OBJECTMGR->AddGarbageObject(pCrop->GetRenderObj());
	pCrop->SetRenderObj(NULL);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ProcUpgradeDlg Method																				농장 업그레이드 대화 창 처리
//
BOOL CSHFarmManager::CLI_ProcUpgradeDlg(int nObjKind)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj == NULL || 
		m_pcsSelFarmObj->GetInfo()->nKind != RENDER_STEWARD_KIND)
	{
		return FALSE;
	}

	int nGrade = 0;
	switch(nObjKind)
	{
	case CSHFarm::FARM_EVENT_GARDEN:
		nGrade = g_csFarmManager.GetFarmZone(m_stSelFarmObj.nFarmZone)->GetFarm(m_stSelFarmObj.nFarmID)->GetGarden(0)->GetGrade();
		break;
	case CSHFarm::FARM_EVENT_FENCE:
		nGrade = g_csFarmManager.GetFarmZone(m_stSelFarmObj.nFarmZone)->GetFarm(m_stSelFarmObj.nFarmID)->GetFence(0)->GetGrade();
		break;
	case CSHFarm::FARM_EVENT_HOUSE:
		nGrade = g_csFarmManager.GetFarmZone(m_stSelFarmObj.nFarmZone)->GetFarm(m_stSelFarmObj.nFarmID)->GetHouse(0)->GetGrade();
		break;
	case CSHFarm::FARM_EVENT_WAREHOUSE:
		nGrade = g_csFarmManager.GetFarmZone(m_stSelFarmObj.nFarmZone)->GetFarm(m_stSelFarmObj.nFarmID)->GetWarehouse(0)->GetGrade();
		break;
	case CSHFarm::FARM_EVENT_ANIMALCAGE:
		nGrade = g_csFarmManager.GetFarmZone(m_stSelFarmObj.nFarmZone)->GetFarm(m_stSelFarmObj.nFarmID)->GetAnimalCage(0)->GetGrade();
		break;
	default:
		return FALSE;
	}

	// 등급이 최대값 이상이면 대화창을 열 필요 없다.
	if (nGrade >= CSHFarmObj::MAX_GRADE-2)
	{
		GAMEIN->GetFarmUpgradeDlg()->SetActive(FALSE);
 		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1063 ) );
		return TRUE;
	}

	if (nGrade >= CSHFarmObj::MAX_GRADE-4 && nObjKind == CSHFarm::FARM_EVENT_GARDEN)
	{
		GAMEIN->GetFarmUpgradeDlg()->SetActive(FALSE);
 		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1063 ) );
		return TRUE;
	}

	GAMEIN->GetFarmUpgradeDlg()->SetUpgradeTarget(m_stSelFarmObj.nFarmZone, m_stSelFarmObj.nFarmID, nObjKind);
	GAMEIN->GetFarmUpgradeDlg()->SetActive(TRUE);

#endif
	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ProcPayTaxDlg Method																					농장 관리비 납부 대화 창
//
BOOL CSHFarmManager::CLI_ProcPayTaxDlg()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj == NULL || 
		m_pcsSelFarmObj->GetInfo()->nKind != RENDER_STEWARD_KIND)
	{
		return FALSE;
	}

	CLI_RequestPayTax();
#endif

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ChangeChannel Method																							   채널 변환하기
//
VOID CSHFarmManager::CLI_ChangeChannel(BOOL bLogin)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	// 농장 맵이라면 채널을 무조건 0 으로 한다.
	static int nOriginalChannelNum = gChannelNum;
	static BOOL bOriginal = FALSE;

	// ..로그인 시점이라면 원래 채널번호 초기화
	//---KES 채널이동 수정
	if (bLogin || gCheatMove)
	{
		nOriginalChannelNum = gChannelNum;
		gCheatMove = FALSE;
	}

	// ..농장맵이라면 채널을 0 으로
	if (GetFarmZoneID(MAP->GetMapNum(), NULL))
	{
		if (bOriginal) nOriginalChannelNum = gChannelNum;
		gChannelNum = 0;
		bOriginal = FALSE;
	}
	else
	{
		bOriginal = TRUE;
		gChannelNum = nOriginalChannelNum;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestBuyFarmToSrv Method																				   서버에 농장 구입 요청
//
VOID CSHFarmManager::CLI_RequestBuyFarmToSrv(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj == NULL) return;

	PACKET_FARM_STATE stPacket;
	stPacket.Category 		= MP_FARM;
	stPacket.Protocol 		= MP_FARM_BUY;
	stPacket.dwObjectID		= HEROID;
	stPacket.nFarmZone		= (WORD)eFarmZone;
	stPacket.nFarmID		= nFarmID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestBuyFarmToSrv Method																				   서버에 농장 구입 요청
//
VOID CSHFarmManager::CLI_RequestBuyFarmToSrv()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj == NULL) return;

	PACKET_FARM_STATE stPacket;

	stPacket.Category 		= MP_FARM;
	stPacket.Protocol 		= MP_FARM_BUY;
	stPacket.dwObjectID		= HEROID;
	stPacket.nFarmZone		= m_stSelFarmObj.nFarmZone;
	stPacket.nFarmID		= m_stSelFarmObj.nFarmID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestPlantCropInGarden Method																			 서버에 농작물 심기 요청
//
VOID CSHFarmManager::CLI_RequestPlantCropInGarden(WORD nItemTargetPos, DWORD nItemIdx)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_BASIC_GARDENPATCH_KIND)
		{
			PACKET_FARM_GARDEN_CROP_PLANT stPacket;

			stPacket.Category 		= MP_FARM;
			stPacket.Protocol 		= MP_FARM_GARDEN_PLANT;
			stPacket.dwObjectID		= HEROID;
			stPacket.nFarmZone		= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID		= m_stSelFarmObj.nFarmID;
			stPacket.nGardenID		= m_stSelFarmObj.nGardenID;
			stPacket.nCropID		= m_stSelFarmObj.nCropID;
			stPacket.nItemTargetPos	= nItemTargetPos;
			stPacket.nItemIdx		= nItemIdx;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestManureCropInGarden Method																	 서버에 농작물 비료주기 요청
//
VOID CSHFarmManager::CLI_RequestManureCropInGarden(WORD nItemTargetPos, DWORD nItemIdx)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_CROP_KIND)
		{
			PACKET_FARM_GARDEN_CROP_MANURE stPacket;

			stPacket.Category 		= MP_FARM;
			stPacket.Protocol 		= MP_FARM_GARDEN_MANURE;
			stPacket.dwObjectID		= HEROID;
			stPacket.nFarmZone		= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID		= m_stSelFarmObj.nFarmID;
			stPacket.nGardenID		= m_stSelFarmObj.nGardenID;
			stPacket.nCropID		= m_stSelFarmObj.nCropID;
			stPacket.nItemTargetPos	= nItemTargetPos;
			stPacket.nItemIdx		= nItemIdx;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestHarvestropInGarden Method																	 서버에 농작물 수확하기 요청
//
VOID CSHFarmManager::CLI_RequestHarvestropInGarden(WORD nItemTargetPos, DWORD nItemIdx, WORD nType)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	static WORD _nItemTargetPos = nItemTargetPos;
	static DWORD _nItemIdx = nItemIdx;

	if (nItemTargetPos)
	{
		_nItemTargetPos = nItemTargetPos;
		_nItemIdx = nItemIdx;
	}

	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_CROP_KIND)
		{
			PACKET_FARM_GARDEN_CROP_GROW stPacket;

			stPacket.Category 		= MP_FARM;
			stPacket.Protocol 		= MP_FARM_GARDEN_HARVEST;
			stPacket.dwObjectID		= HEROID;
			stPacket.nFarmZone		= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID		= m_stSelFarmObj.nFarmID;
			stPacket.nGardenID		= m_stSelFarmObj.nGardenID;
			stPacket.nCropID		= m_stSelFarmObj.nCropID;
			stPacket.nItemTargetPos	= _nItemTargetPos;
			stPacket.nItemIdx		= _nItemIdx;
			stPacket.nResult		= nType;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// 080414 KTH
// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestInstallAnimalInAnimalCage Method																		서버에 가축 넣기 요청
//
VOID CSHFarmManager::CLI_RequestInstallAnimalInAnimalCage(WORD nItemTargetPos, DWORD nItemIdx)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND)
		{
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT stPacket;

			stPacket.Category 		= MP_FARM;
			stPacket.Protocol 		= MP_FARM_ANIMAL_INSTALL;
			stPacket.dwObjectID		= HEROID;
			stPacket.nFarmZone		= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID		= m_stSelFarmObj.nFarmID;
			stPacket.nAnimalCageID	= m_stSelFarmObj.nAnimalCageID;
			stPacket.nAnimalID		= m_stSelFarmObj.nAnimalID;
			stPacket.nItemTargetPos	= nItemTargetPos;
			stPacket.nItemIdx		= nItemIdx;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
		else
		{
			// 가축이 아닌 아이템은 넣을수 없다.
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 91 ) );
		}
	}
#endif
}

// 080415 KTH
// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_InstallAnimalInAnimalCage Method																					가축 넣기 요청
//
VOID CSHFarmManager::SRV_InstallAnimalInAnimalCage(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx, WORD nAnimalContentment, WORD nAnimalInterest, int nAgentChecked)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	int nAnimalKind = 0;
	//UINT nCropSeedGrade = 0;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 축사 범위 체크
	if( nAnimalCageID >= pFarm->GetAnimalCageNum() )
	{
		return;
	}

	CPen* pPen = pFarm->GetAnimalCage(nAnimalCageID);

	// 축사 슬롯 체크
	if( nAnimalID >= pPen->GetAnimalNum() )
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_ANIMAL)
	{
		return;
	}

	nAnimalKind = pItemInfo->SupplyValue;

	CAnimal* pAnimal = pPen->GetAnimal(nAnimalID);

	// 가축 종류 범위 체크
	if( nAnimalKind > RENDER_ANIMAL_KIND_NUM )
	//if (nCropKind == 0 || nCropKind > RENDER_ANIMAL_KIND_NUM)
	{
		return;
	}

	// 패밀리 상태
	// ..에이전트에서 관리하므로 에이전트로 보낸다.
	if (nAgentChecked == 0)
	{
		PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT stPacket;

		stPacket.Category 		= MP_FARM;
		stPacket.Protocol 		= MP_FARM_ANIMAL_INSTALL_AGENT_CHECK;
		stPacket.dwObjectID		= pPlayer->GetID();
		stPacket.nFarmZone		= eFarmZone;
		stPacket.nFarmID		= nFarmID;
		stPacket.nAnimalCageID	= nAnimalCageID;
		stPacket.nAnimalID		= nAnimalID;
		stPacket.nItemTargetPos	= nItemTargetPos;
		stPacket.nItemIdx		= nItemIdx;
		stPacket.nOwner			= pFarm->GetOwner();
		// 멤버 수 체크를 위해 nResult 변수에다가...
		stPacket.nResult		= FARM_BUY_FAMILY_MEMNUM;
		stPacket.nAnimalContentment = nAnimalContentment;
		stPacket.nAnimalInterest = nAnimalInterest;

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		return;
	}

	// 비어 있는 축사 인가?
	if( pAnimal->GetStep() != CAnimal::ANIMAL_STEP_EMPTY )
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 텃밭의 등급보다 FARM_PLANT_GARDEN_GRADE_DIFF등급 이상의 씨앗은 심을 수 없다.
	//if (nCropSeedGrade >= pGarden->GetGrade()+FARM_PLANT_GARDEN_GRADE_DIFF)
	//{
	//	eResult = RESULT_FAIL_02;
	//	goto FAILED;
	//}

	// 다시 심는데 필요한 시간이 있나?
/*	if (pPlayer->GetFarmInfo()->nCropPlantRetryTime)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}*/

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pAnimal->GetPos(), &stPlayerPos); 
	if (nDistance > PEN_OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	pAnimal->Breed(pPlayer->GetID(), pPen->GetGrade(), nAnimalKind);

	// DB에 저장
	SRV_SaveAnimalInfoToDB(eFarmZone, nFarmID, pAnimal);

	// 다시 심는데 필요한 시간 설정
	//pPlayer->SetFarmCropPlantRetryTime(CSHCrop::CROP_PLANT_RETRY_TIME);
	//Farm_SetTimeDelay(pPlayer->GetID(), FARM_TIMEDELAY_KIND_PLANT, CSHCrop::CROP_PLANT_RETRY_TIME);

	// Log
	InsertLogFarmAnimal( pFarm, pAnimal, eLog_FamilyFarmInstallAnimal );

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_ANIMAL_INSTALL;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nAnimalCageID		= nAnimalCageID;
	stPacket.nAnimalID			= nAnimalID;
	stPacket.nOwner				= pPlayer->GetID();
	stPacket.nAnimalKind		= nAnimalKind;
	stPacket.nAnimalLife		= pAnimal->GetLife();
	stPacket.nAnimalOwner		= pPlayer->GetID();
	stPacket.nResult			= (WORD)eResult;
	stPacket.nAnimalContentment = pAnimal->GetContentment();
	stPacket.nAnimalInterest	= pAnimal->GetInterest();

	if (eResult == CSHMain::RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		//SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestUpgradeFarm Method																 	서버에 농장 오브젝트 업그레이드 요청
//
VOID CSHFarmManager::CLI_RequestUpgradeFarm(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, int nObjKind)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	PACKET_FARM_UPGRADE stPacket;

	stPacket.Category 		= MP_FARM;
	stPacket.Protocol 		= MP_FARM_UPGRADE;
	stPacket.dwObjectID		= HEROID;
	stPacket.nFarmZone		= eFarmZone;
	stPacket.nFarmID		= nFarmID;
	stPacket.nObjKind		= nObjKind;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestPayTax Method																 					 서버에 관리비 납부 요청
//
VOID CSHFarmManager::CLI_RequestPayTax(UINT nKind)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD4 stPacket;

	stPacket.Category 		= MP_FARM;
	stPacket.Protocol 		= MP_FARM_PAY_TAX;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= m_stSelFarmObj.nFarmZone;
	stPacket.dwData2		= m_stSelFarmObj.nFarmID;
	stPacket.dwData3		= 0;
	stPacket.dwData4		= nKind;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestFarmInfo Method																 					   서버에 농장 정보 요청
//
VOID CSHFarmManager::CLI_RequestFarmInfo(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	PACKET_FARM_STATE stPacket;

	stPacket.Category 		= MP_FARM;
	stPacket.Protocol 		= MP_FARM_FARMSTATE_LOAD_FROM_DB;
	stPacket.dwObjectID		= HEROID;
	stPacket.nFarmZone		= eFarmZone;
	stPacket.nFarmID		= nFarmID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ParseAnswerFromSrv Method																			  서버로부터의 응답 분석/처리
//
VOID CSHFarmManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
 	MSGBASE* pTmp = (MSGBASE*)pMsg;

	switch(pTmp->Protocol)
	{
	// DB 큐로부터 농장 상태 설정
	case MP_FARM_FARMSTATE_LOAD_FROM_DB:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);

			pFarm->SetOwner(pPacket->nFarmOwner);
			pFarm->SetOwnState((CSHFarmObj::OWN_STATE)pPacket->nFarmState);
			pFarm->GetGarden(0)->SetGrade(pPacket->nGardenGrade);
			pFarm->GetFence(0)->SetGrade(pPacket->nFenceGrade);
			pFarm->GetAnimalCage(0)->SetGrade(pPacket->nAnimalCageGrade);
			pFarm->GetHouse(0)->SetGrade(pPacket->nHouseGrade);
			pFarm->GetWarehouse(0)->SetGrade(pPacket->nWarehouseGrade);
			// 새로운 오브젝트 추가
			if (pFarm->GetIsRendering()) CLI_AddRenderFarmObjToScreen(pFarm);
		}
		break;

	// DB 큐로부터 농작물 상태 설정
	case MP_FARM_CROPINFO_LOAD_FROM_DB:
		{
 			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(pPacket->nGardenID);
			CSHCrop*	 pCrop	   = pGarden->GetCrop(pPacket->nCropID);

			pCrop->SetOwner(pPacket->nCropOwner);
			pCrop->SetStep((CSHCrop::CROP_STEP)pPacket->nCropStep);
			pCrop->SetLife(pPacket->nCropLife);	
			pCrop->SetSeedGrade(pPacket->nCropSeedGrade);
			pCrop->SetKind(pPacket->nCropKind);
			pCrop->SetNextStepTime(pPacket->nCropNextStepTime);
			// 새로운 오브젝트 추가
			if (pFarm->GetIsRendering()) CLI_AddRenderFarmCropObjToScreen(pCrop);
		}
		break;
	// 080417 KTH -- DB 큐로부터 가축 상태 설정
	case MP_FARM_ANIMALINFO_LOAD_FROM_DB:
		{
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket = (PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if( pFarmZone == NULL ) return;
			CSHFarm*	pFarm			= pFarmZone->GetFarm(pPacket->nFarmID);
			CPen*		pAnimalCage		= pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			CAnimal*	pAnimal			= pAnimalCage->GetAnimal(pPacket->nAnimalID);

			pAnimal->SetOwner(pPacket->nAnimalOwner);
			pAnimal->SetStep((CAnimal::ANIMAL_STEP)pPacket->nAnimalStep);
			pAnimal->SetLife(pPacket->nAnimalLife);
			pAnimal->SetKind(pPacket->nAnimalKind);
			pAnimal->SetNextStepTime(pPacket->nAnimalNextStepTime);
			pAnimal->SetContentment(pPacket->nAnimalContentment);
			pAnimal->SetInterest(pPacket->nAnimalInterest);

			if( pFarm->GetIsRendering() ) CLI_AddRenderFarmAnimalObjToScreen(pAnimal);
		}
		break;

	// 농장UI 농장 정보 
	case MP_FARM_MANAGE_UI_FARM_DATA:
		{
			PACKET_FARM_STATE_WITH_TAX* pPacket = (PACKET_FARM_STATE_WITH_TAX*)pMsg;
			CSHFarmManageDlg* pDlg = GAMEIN->GetFarmManageDlg();

			if (HERO == NULL) return;

			if (pDlg)
			{
				pDlg->SetFarm(pPacket->nFarmZone, pPacket->nFarmID);
				CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
				if (pMiniMapDlg)
				{
					CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
					if (pFarmZone == NULL) return;
					CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);

					pMiniMapDlg->AddFarmIcon(pFarm);

					// 관리비 납부 상태
					pFarm->SetTaxArrearageFreq(pPacket->nTaxArrearageFreq);
					pFarm->SetTaxPayPlayerName(pPacket->szTaxPayPlayerName);
				}
			}			
		}
		break;

	// 농장UI 농작물 정보 
	case MP_FARM_MANAGE_UI_CROP_DATA:
		{
 			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;
			CSHCrop* pCrop = GAMEIN->GetFarmManageDlg()->GetFarm()->GetGarden(0)->GetCrop(pPacket->nCropID);

			pCrop->SetID(pPacket->nCropID);
			pCrop->SetOwner(pPacket->nCropOwner);
			pCrop->SetStep((CSHCrop::CROP_STEP)pPacket->nCropStep);
			pCrop->SetLife(pPacket->nCropLife);	
			pCrop->SetSeedGrade(pPacket->nCropSeedGrade);
			pCrop->SetKind(pPacket->nCropKind);
 			pCrop->SetNextStepTime(pPacket->nCropNextStepTime);
			GAMEIN->GetFarmManageDlg()->SetCropImg(pPacket->nCropID, pPacket->nCropKind, pPacket->nCropOwner);
			GAMEIN->GetFarmManageDlg()->SetCropViewInfo(pPacket->nCropID, TRUE);
		}
		break;

	// 농장 구입
	case MP_FARM_BUY:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);

			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
				pFarm->SetOwnState(CSHFarmObj::OWN_STATE_OWNED);
				pFarm->SetOwner(pPacket->nFarmOwner);
				pFarm->GetGarden(0)->SetGrade(pPacket->nGardenGrade);
				pFarm->GetHouse(0)->SetGrade(pPacket->nHouseGrade);
				pFarm->GetFence(0)->SetGrade(pPacket->nFenceGrade);
				pFarm->GetAnimalCage(0)->SetGrade(pPacket->nAnimalCageGrade);
				pFarm->GetWarehouse(0)->SetGrade(pPacket->nWarehouseGrade);

				// 팻말 타겟 해제
 				if(OBJECTMGR->GetSelEffectHandle())
				{
					EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
				}

				// 농장구입시 농장에 있던 유저의 타겟이 해제되는 문제가 있어 아래코드를 주석처리
				// 타겟해제는 CLI_RemoveRenderFarmObjFromScreen() 함수내에서 처리함.
				//if (OBJECTMGR->GetSelectedObject())
				//{
				//	OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				//	OBJECTMGR->GetSelectedObject()->OnDeselected();
				//	OBJECTMGR->SetSelectedObjectID(0);
				//}

				CLI_SetTargetObj(NULL);
				// 구입 창 닫기
				if (GAMEIN->GetFarmBuyDlg())
				{
					GAMEIN->GetFarmBuyDlg()->SetActive(FALSE);
				}

				// 기존 오브젝트 제거
				CLI_RemoveRenderFarmObjFromScreen(pFarm);
				// 새로운 오브젝트 추가
				CLI_AddRenderFarmObjToScreen(pFarm);
				if (HERO->GetID() == pFarm->GetOwner())
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1105 ) );
				break;

			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1044 ) );
				break;
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1045 ) );
				break;
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1046 ) );
				break;
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1047 ) );
				break;
			case CSHMain::RESULT_FAIL_05:	
			case CSHMain::RESULT_FAIL_06:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1048 ) );
				break;
			case CSHMain::RESULT_FAIL_07:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1049 ) );
				break;
			case CSHMain::RESULT_FAIL_08:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1050 ) );
				break;
			}
		}
		break;
	// 농장 삭제
	case MP_FARM_DEL:
		{
			PACKET_FARM_STATE* pPacket = (PACKET_FARM_STATE*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(0);
			// 080423 KTH
			CPen* pAnimalCage = pFarm->GetAnimalCage(0);

			// 삭제
			pFarm->Init();
			for(int i=0; i<pGarden->GetCropNum(); i++)
			{
				pGarden->GetCrop(i)->SetCrop(CSHCrop::CROP_RESULT_DIE);
			}
			// 080423 KTH
			for( int i=0; i<pAnimalCage->GetAnimalNum(); i++ )
			{
				pAnimalCage->GetAnimal(i)->SetAnimal(CAnimal::ANIMAL_RESULT_DIE);
			}

			if (CLI_GetTargetObj())
			{
				// 타켓 해제
 				if(OBJECTMGR->GetSelEffectHandle())
				{
					EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
				}

				// 농장구입시 농장에 있던 유저의 타겟이 해제되는 문제가 있어 아래코드를 주석처리
				// 타겟해제는 CLI_RemoveRenderFarmObjFromScreen() 함수내에서 처리함.
				//if (OBJECTMGR->GetSelectedObject())
				//{
				//	OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
				//	OBJECTMGR->GetSelectedObject()->OnDeselected();
				//	OBJECTMGR->SetSelectedObjectID(0);
				//}

				CLI_SetTargetObj(NULL);
			}

			// 기존 오브젝트 제거
			CLI_RemoveRenderFarmObjFromScreen(pFarm);
			// 새로운 오브젝트 추가
			CLI_AddRenderFarmObjToScreen(pFarm);
		}
		break;
	// 텃밭에 농작물 심기
	case MP_FARM_GARDEN_PLANT:
		{
 			PACKET_FARM_GARDEN_CROP_PLANT* pPacket = (PACKET_FARM_GARDEN_CROP_PLANT*)pMsg;

 			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(pPacket->nGardenID);
			CSHCrop*	 pCrop	   = pGarden->GetCrop(pPacket->nCropID);

 			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
				{
					// 심기
					pCrop->Plant(pPacket->nCropOwner, pPacket->nCropSeedGrade, pPacket->nCropKind);
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pCrop->GetOwner());
					if( !pPlayer )	return;
					pPlayer->StartFarmMotion(FARM_PLANT_CHAR_MOTION_NUM, CSHCrop::CROP_PLANT_SEEDING_TIME);
					// 다시 심기위해 필요한 시간 아이콘 표시
					if (HERO->GetID() == pPlayer->GetID())
					{
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
						if (pDlg) pDlg->GetFarmRePlantTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_REPLANT, NULL, CSHCrop::CROP_PLANT_RETRY_TIME*1000*60);
					}

					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if (pGuageDlg)
					{
						if ((CObject*)pGarden->GetRenderObjEx(pCrop->GetID()+15) == OBJECTMGR->GetSelectedObject())
						{
							pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						}
					}
				}
				break;
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1051 ), 2 );
				break;
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1052 ), CSHCrop::CROP_PLANT_RETRY_TIME );
				break;
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1053 ) );
				break;
			case CSHMain::RESULT_FAIL_05:	
			case CSHMain::RESULT_FAIL_06:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1054 ) );
				break;
			case CSHMain::RESULT_FAIL_07:	
 				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1160 ), FARM_BUY_FAMILY_MEMNUM );
				break;
			}
		}
		break;

	// 텃밭의 농작물 가꾸기
	case MP_FARM_GARDEN_GROW:
		{
			PACKET_FARM_GARDEN_CROP_GROW* pPacket = (PACKET_FARM_GARDEN_CROP_GROW*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(pPacket->nGardenID);
			CSHCrop*	 pCrop	   = pGarden->GetCrop(pPacket->nCropID);

			switch(pPacket->nResult)
			{
			case CSHCrop::CROP_RESULT_LIFE:
				{
					// 농작물 설정
					pCrop->SetLife(pPacket->nValue);
					// 다이얼로그에 변화 적용
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if (pGuageDlg)
					{
						if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
						{
							pGuageDlg->SetMonsterLife(pCrop->GetLife(), pCrop->GetMaxLife());
						}
					}
				}
				break;

			case CSHCrop::CROP_RESULT_DIE:
				{
					// 오브젝트 표시 설정
					// 타켓 해제
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					if (pGuageDlg)
					{
						if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
						{
							pGuageDlg->GetMonsterGuage()->SetActive(FALSE);
 							if(OBJECTMGR->GetSelEffectHandle())
							{
								EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
							}

							if (OBJECTMGR->GetSelectedObject())
							{
								OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
								OBJECTMGR->GetSelectedObject()->OnDeselected();
								OBJECTMGR->SetSelectedObjectID(0);
							}

							CLI_SetTargetObj(NULL);
						}
					}

					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_DIE])
					{
						TARGETSET set;
						set.pTarget = pCrop->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_DIE], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
					}

					// 농작물 렌더링 오브젝트 제거
					CLI_RemoveRenderFarmCropObjToScreen(pCrop);
					// 농작물 설정
					pCrop->SetCrop((CSHCrop::CROP_RESULT)pPacket->nResult);
					pGarden->GetRenderObjEx(pPacket->nCropID+15)->GetEngineObject()->EnablePick();
				}
				break;

			case CSHCrop::CROP_RESULT_STEP_UP:
				{
					// 기존 오브젝트가 선택되어 있었다면..
					BOOL bSel = FALSE;
					if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
					{
						// 타켓 해제
 						if(OBJECTMGR->GetSelEffectHandle())
						{
							EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
						}

						if (OBJECTMGR->GetSelectedObject())
						{
							OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
							OBJECTMGR->GetSelectedObject()->OnDeselected();
							OBJECTMGR->SetSelectedObjectID(0);
						}

						CLI_SetTargetObj(NULL);
						bSel = TRUE;
					}
					// 기존 오브젝트 제거
					CLI_RemoveRenderFarmCropObjToScreen(pCrop);
					// 농작물 설정
					pCrop->SetCrop((CSHCrop::CROP_RESULT)pPacket->nResult);
					// 새로운 오브젝트 추가
					CLI_AddRenderFarmCropObjToScreen(pCrop);
					if (bSel)
					{
						CActionTarget Target;
						Target.InitActionTarget( pCrop->GetRenderObj(), NULL );
						// 조작할 농작물 선택
						g_csFarmManager.CLI_SetTargetObj(pCrop->GetRenderObj());
						pCrop->GetRenderObj()->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
						OBJECTMGR->SetSelectedObject( pCrop->GetRenderObj() );
					}
					// 이름 변경
					char* pszName = m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(pCrop->GetKind()-1)+pCrop->GetStep()-CSHCrop::CROP_STEP_1].szName;
					pCrop->GetRenderObj()->SetName(pszName);
					pCrop->GetRenderObj()->GetObjectBalloon()->GetOBalloonName()->SetName(pszName);
					// 다이얼로그에 이름 변경
					{
						CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
						if (pGuageDlg)
						{
							if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
							{
								pGuageDlg->SetName( (CObject*)pCrop->GetRenderObj(), pszName) ; 
								pGuageDlg->SetMonsterLife(pCrop->GetLife(), pCrop->GetMaxLife());
							}
						}
					}
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP])
					{
						TARGETSET set;
						set.pTarget = pCrop->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
					}
				}
				break;

			case CSHCrop::CROP_RESULT_STEP_UP_COMPLETE:
				{
					BOOL bSel = FALSE;
					if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
					{
						// 타켓 해제
 						if(OBJECTMGR->GetSelEffectHandle())
						{
							EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
						}

						if (OBJECTMGR->GetSelectedObject())
						{
							OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
							OBJECTMGR->GetSelectedObject()->OnDeselected();
							OBJECTMGR->SetSelectedObjectID(0);
						}

						CLI_SetTargetObj(NULL);
						bSel = TRUE;
					}
					// 기존 오브젝트 제거
					CLI_RemoveRenderFarmCropObjToScreen(pCrop);
					// 농작물 설정
					pCrop->SetCrop((CSHCrop::CROP_RESULT)pPacket->nResult);
					// 새로운 오브젝트 추가
					CLI_AddRenderFarmCropObjToScreen(pCrop);
					if (bSel)
					{
						CActionTarget Target;
						Target.InitActionTarget( pCrop->GetRenderObj(), NULL );
						// 조작할 농작물 선택
						g_csFarmManager.CLI_SetTargetObj(pCrop->GetRenderObj());
						pCrop->GetRenderObj()->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
						OBJECTMGR->SetSelectedObject( pCrop->GetRenderObj() );
					}
					// 이름 변경
					char* pszName = m_pszTblFarmCropName[CSHCrop::CROP_STEP_MAX*(pCrop->GetKind()-1)+pCrop->GetStep()-CSHCrop::CROP_STEP_1].szName;
					pCrop->GetRenderObj()->SetName(pszName);
					pCrop->GetRenderObj()->GetObjectBalloon()->GetOBalloonName()->SetName(pszName);
					// 다이얼로그에 이름 변경
					{
						CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
						if (pGuageDlg)
						{
							if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
							{
								pGuageDlg->SetName( (CObject*)pCrop->GetRenderObj(), pszName ) ; 
							}
						}
					}
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP])
					{
						TARGETSET set;
						set.pTarget = pCrop->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_STEPUP], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
					}
				}
				break;
			case CSHCrop::CROP_RESULT_NEXT_STEP_TIME:
				{
					pCrop->SetNextStepTime(pPacket->nValue);
				}
				break;
			}
		}
		break;

	// 텃밭의 농작물 비료주기
	case MP_FARM_GARDEN_MANURE:
		{
			PACKET_FARM_GARDEN_CROP_MANURE* pPacket = (PACKET_FARM_GARDEN_CROP_MANURE*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(pPacket->nGardenID);
			CSHCrop*	 pCrop	   = pGarden->GetCrop(pPacket->nCropID);

			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
				{
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pCrop->GetOwner());
					if( !pPlayer )	return;
					pPlayer->StartFarmMotion(CSHFarmManager::FARM_MANURE_CHAR_MOTION_NUM, CSHCrop::CROP_PLANT_SEEDING_TIME);
					// 비료 준 시간 설정
					pCrop->SetManureTimeTick(gCurTime);
					pCrop->Manure(pPacket->nManureGrade, pPacket->nManureKind);
					// 비료로 + 되는 생명력은 랜덤이므로 서버로부터의 값으로 재설정 필요.
					pCrop->SetLife(pPacket->nCropLife);
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
					// 다시 비료주기 위해 필요한 시간 아이콘 표시
					if (HERO->GetID() == pPlayer->GetID())
					{
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
						if (pDlg) pDlg->GetFarmReManureTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_REMANURE, NULL, CSHCrop::CROP_MANURE_RETRY_TIME*1000*60);
					}
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_MANURE])
					{
						TARGETSET set;
						set.pTarget = pCrop->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_MANURE], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
					}
				}
				break;

			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1055 ) );
				break;
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1056 ) );
				break;
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1057 ), CSHCrop::CROP_MANURE_RETRY_TIME );
				break;
			case CSHMain::RESULT_FAIL_05:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1058 ) );
				break;
			}
		}
		break;

	// 텃밭의 농작물 수확하기
	case MP_FARM_GARDEN_HARVEST:
		{
 			PACKET_FARM_GARDEN_CROP_GROW* pPacket = (PACKET_FARM_GARDEN_CROP_GROW*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);
			CSHGarden*	 pGarden   = pFarm->GetGarden(pPacket->nGardenID);
			CSHCrop*	 pCrop	   = pGarden->GetCrop(pPacket->nCropID);

			switch(pPacket->nResult)
			{
			// 모션 시작
			case CSHMain::RESULT_OK:
				{
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pCrop->GetOwner());
					if( !pPlayer )	return;
					pPlayer->StartFarmMotion(CSHFarmManager::FARM_HARVEST_CHAR_MOTION_NUM, CSHCrop::CROP_PLANT_SEEDING_TIME);
					// 수확 모션 시간 설정
					pCrop->SetPlantTimeTick(gCurTime);
				}
				break;
			// 모션 종료 & 실제 수확
			case CSHMain::RESULT_OK_02:
				{
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_HARVEST])
					{
						TARGETSET set;
						set.pTarget = pCrop->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CROP_HARVEST], pCrop->GetRenderObj(), &set, 0, pCrop->GetRenderObj()->GetID());
					}

					// 기존 오브젝트 제거
 					CLI_RemoveRenderFarmCropObjToScreen(pCrop);
					pGarden->GetRenderObjEx(pPacket->nCropID+15)->GetEngineObject()->EnablePick();
					pCrop->SetCrop(CSHCrop::CROP_RESULT_DIE);

					if ((CObject*)pCrop->GetRenderObj() == OBJECTMGR->GetSelectedObject())
					{
						// 타켓 해제
 						if(OBJECTMGR->GetSelEffectHandle())
						{
							EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
						}

						if (OBJECTMGR->GetSelectedObject())
						{
							OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
							OBJECTMGR->GetSelectedObject()->OnDeselected();
							OBJECTMGR->SetSelectedObjectID(0);
						}
					}

					CLI_SetTargetObj(NULL);
				}
				break;
			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1059 ) );
				break;
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1060 ) );
				break;
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1061 ) );
				break;
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1062 ) );
				break;
			}
		}
		break;
	// 농장 업그레이드
	case MP_FARM_UPGRADE:
		{
 			PACKET_FARM_UPGRADE* pPacket = (PACKET_FARM_UPGRADE*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*	 pFarm	   = pFarmZone->GetFarm(pPacket->nFarmID);

			int nRenderObjKind = 0, nGrade = 0;
			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
 				switch(pPacket->nObjKind)
				{
				// ..텃밭
				case CSHFarm::FARM_EVENT_GARDEN:
					pFarm->GetGarden(0)->SetGrade(nGrade = pFarm->GetGarden(0)->GetGrade()+1);
					nRenderObjKind = RENDER_GARDENPATCH_KIND;
					if (HERO->GetID() == pFarm->GetOwner())
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1106 ), nGrade );
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_GARDEN])
					{
						TARGETSET set;
						set.pTarget = pFarm->GetGarden(0)->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_GARDEN], pFarm->GetGarden(0)->GetRenderObjEx(8), &set, 0, pFarm->GetGarden(0)->GetRenderObj()->GetID());
					}
					break;
				// ..축사
				case CSHFarm::FARM_EVENT_ANIMALCAGE:
					pFarm->GetAnimalCage(0)->SetGrade(nGrade = pFarm->GetAnimalCage(0)->GetGrade()+1);
					nRenderObjKind = RENDER_ANIMALCAGE_KIND;
					if (HERO->GetID() == pFarm->GetOwner())
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1107 ), nGrade );
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_ANIMALCAGE])
					{
						TARGETSET set;
						set.pTarget = pFarm->GetAnimalCage(0)->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_ANIMALCAGE], pFarm->GetAnimalCage(0)->GetRenderObj(), &set, 0, pFarm->GetAnimalCage(0)->GetRenderObj()->GetID());
					}
					break;
				// ..울타리
				case CSHFarm::FARM_EVENT_FENCE:
					pFarm->GetFence(0)->SetGrade(nGrade =pFarm->GetFence(0)->GetGrade()+1);
					nRenderObjKind = RENDER_FENCE_KIND;
					if (HERO->GetID() == pFarm->GetOwner())
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1109 ), nGrade );
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_FENCE])
					{
						TARGETSET set;
						set.pTarget = pFarm->GetFence(0)->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_FENCE], pFarm->GetFence(0)->GetRenderObj(), &set, 0, pFarm->GetFence(0)->GetRenderObj()->GetID());
					}
					break;
				// ..집
				case CSHFarm::FARM_EVENT_HOUSE:
					pFarm->GetHouse(0)->SetGrade(nGrade =pFarm->GetHouse(0)->GetGrade()+1);
					nRenderObjKind = RENDER_HOUSE_KIND;
					if (HERO->GetID() == pFarm->GetOwner())
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1108 ), nGrade );
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_HOUSE])
					{
						TARGETSET set;
						set.pTarget = pFarm->GetHouse(0)->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_HOUSE], pFarm->GetHouse(0)->GetRenderObj(), &set, 0, pFarm->GetHouse(0)->GetRenderObj()->GetID());
					}
					break;
				// ..창고
				case CSHFarm::FARM_EVENT_WAREHOUSE:
					pFarm->GetWarehouse(0)->SetGrade(nGrade =pFarm->GetWarehouse(0)->GetGrade()+1);
					nRenderObjKind = RENDER_WAREHOUSE_KIND;
					if (HERO->GetID() == pFarm->GetOwner())
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1110 ), nGrade );
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_WAREHOUSE])
					{
						TARGETSET set;
						set.pTarget = pFarm->GetWarehouse(0)->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_UPGRADE_WAREHOUSE], pFarm->GetWarehouse(0)->GetRenderObj(), &set, 0, pFarm->GetWarehouse(0)->GetRenderObj()->GetID());
					}
					break;
				}

				// 기존 오브젝트 제거
				CLI_RemoveRenderFarmObjFromScreen(pFarm, nRenderObjKind);
				// 새로운 오브젝트 추가
				CLI_AddRenderFarmObjToScreen(pFarm, nRenderObjKind);

				// 최대 등급이 되면 대화창 닫기
				if (nGrade >= CSHFarmObj::MAX_GRADE-2)
				{
					if (GAMEIN->GetFarmUpgradeDlg())
					{
						GAMEIN->GetFarmUpgradeDlg()->SetActive(FALSE);
					}
				}

				if (nGrade >= CSHFarmObj::MAX_GRADE-4 && pPacket->nObjKind == CSHFarm::FARM_EVENT_GARDEN)
				{
					if (GAMEIN->GetFarmUpgradeDlg())
					{
						GAMEIN->GetFarmUpgradeDlg()->SetActive(FALSE);
					}
				}

				// 업그레이드는 집사가 선택된 상태에서 진행이 계속 되어야 하기 때문에 타겟을 해제하면 안 됨.
				/*
				// 타켓 해제
 				if(OBJECTMGR->GetSelEffectHandle())
				{
					EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
				}

				if (OBJECTMGR->GetSelectedObject())
				{
					OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
					OBJECTMGR->GetSelectedObject()->OnDeselected();
					OBJECTMGR->SetSelectedObjectID(0);
				}

				CLI_SetTargetObj(NULL);
				*/
				break;

			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1063 ) );
				break;
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1064 ) );
				break;
			case CSHMain::RESULT_FAIL_03:	
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1065 ) );
				break;
			case CSHMain::RESULT_FAIL_05:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1066 ) );
				break;
			case CSHMain::RESULT_FAIL_06:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1160 ), FARM_BUY_FAMILY_MEMNUM );
				break;
			}
		}
		break;
	case MP_FARM_TIMEDELAY:
		{
 			MSG_DWORD3* pPacket = (MSG_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
			if( !pPlayer )	return;

			CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
			if (pDlg == NULL) return;

			switch(pPacket->dwData1)
			{
			case FARM_TIMEDELAY_KIND_PLANT:		pDlg->GetFarmRePlantTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_REPLANT, NULL, pPacket->dwData2*1000*60, pPacket->dwData3*1000*60);	break;
			case FARM_TIMEDELAY_KIND_MANURE:	pDlg->GetFarmReManureTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_REMANURE, NULL, pPacket->dwData2*1000*60, pPacket->dwData3*1000*60);	break;
			// 080508 KTH -- 가축 먹이 주기 딜레이 추가
			//case FARM_TIMEDELAY_KIND_FEED:		pDlg->GetFarmReManureTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_REMANURE, NULL, pPacket->dwData2*1000*60, pPacket->dwData3*1000*60);	break;
			}
			
		}
		break;
	// 관리비 납부
	case MP_FARM_PAY_TAX:
		{
			char szTxt[256];
 			MSG_NAME_DWORD3* pPacket = (MSG_NAME_DWORD3*)pMsg;

			switch(pPacket->dwData1)
			{
			// 납부
			case RESULT_OK:
				{
					// 납부 상태 설정
					CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->dwData2);
					if (pFarmZone == NULL) return;
					CSHFarm*	 pFarm	   = pFarmZone->GetFarm((WORD)pPacket->dwData3);

					pFarm->SetTaxArrearageFreq(0);
					pFarm->SetTaxPayPlayerName(pPacket->Name);

					if (HERO->GetID() == pPacket->dwObjectID)
					{
						WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, CHATMGR->GetChatMsg(1330));
					}
				}
				break;
			// 납부 금액을 받아서 창 띄우기
			case RESULT_OK_02:
				{
					sprintf(szTxt, CHATMGR->GetChatMsg(1331), pPacket->dwData2);
					WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_REQUEST, MBT_YESNO, szTxt );
				}
				break;
			case RESULT_FAIL_01:		
				WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, CHATMGR->GetChatMsg(1054) );
				break;
			case RESULT_FAIL_02:		
				sprintf(szTxt, CHATMGR->GetChatMsg(1332), pPacket->Name);
				WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, szTxt, pPacket->Name);		
				break;
			case RESULT_FAIL_03:
				WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, CHATMGR->GetChatMsg(1333));								
				break;
			}
		}
		break;
	// 멤버들에게 농장 삭제 알림
	case MP_FARM_DEL_NOTIFY_TO_MEMBER:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case FARM_DEL_KIND_TAX:
				{
					WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, CHATMGR->GetChatMsg(1334));

					// 농장관리 창 닫기
					CSHFarmManageDlg* pFarmDlg = GAMEIN->GetFarmManageDlg();
					if (pFarmDlg)
					{
						pFarmDlg->Clear();
						pFarmDlg->SetActive(FALSE);
						// 미니맵의 농장 아이콘 제거
						CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
						if (pMiniMapDlg)
						{
							pMiniMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
						}
						CBigMapDlg* pBigMapDlg = GAMEIN->GetBigMapDialog();
						if (pBigMapDlg)
						{
							pBigMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
						}
					}
				}
				break;
			}
		}
		break;
	// 관리비 미납 알림
	case MP_FARM_TAX_NONPAYMENT_NOTIFY:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			char* pszDay;
			char szTxt[256];

			switch(FARM_TAX_PAY_LIMIT_DAY)
			{
			case 0: pszDay = CHATMGR->GetChatMsg(1335);	break;
			case 1: pszDay = CHATMGR->GetChatMsg(1336);	break;
			case 2: pszDay = CHATMGR->GetChatMsg(1337);	break;
			case 3: pszDay = CHATMGR->GetChatMsg(1338);	break;
			case 4: pszDay = CHATMGR->GetChatMsg(1339);	break;
			case 5: pszDay = CHATMGR->GetChatMsg(1340);	break;
			case 6: pszDay = CHATMGR->GetChatMsg(1341);	break;
			default: pszDay = CHATMGR->GetChatMsg(1335);
			}

			if (pPacket->dwData > FARM_TAX_PAY_ARREARAGE_MAX_FREQ - 1)
			{
				sprintf(szTxt, CHATMGR->GetChatMsg(1342), pszDay, FARM_TAX_PAY_LIMIT_HOUR);
			}
			else
			{
				sprintf(szTxt, CHATMGR->GetChatMsg(1343), pszDay, FARM_TAX_PAY_LIMIT_HOUR, FARM_TAX_PAY_WEEK_INTERVAL, pszDay, FARM_TAX_PAY_LIMIT_HOUR);
			}
			WINDOWMGR->MsgBox( MBI_FARM_TAX_PAY_RESULT, MBT_OK, szTxt);
		}
		break;
	// 080430 KTH -- 축사에 가축 넣기
	case MP_FARM_ANIMAL_INSTALL:
		{
			PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket = (PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if( pFarmZone == NULL ) return;
			CSHFarm*	pFarm	= pFarmZone->GetFarm(pPacket->nFarmID);
			CPen*		pPen	= pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			CAnimal*	pAnimal	= pPen->GetAnimal(pPacket->nAnimalID);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->nOwner);

			if( !pPlayer )
			{
				return;
			}
			else
			{
				VECTOR3* pos;
				pos = pAnimal->GetPos();
				MOVEMGR->SetLookatPos(pPlayer,pos,0,gCurTime);
			}

			switch( pPacket->nResult )
			{
			case CSHMain::RESULT_OK:
				{
					pAnimal->Breed(pPacket->nAnimalOwner, pPen->GetGrade(), pPacket->nAnimalKind);
					pAnimal->SetContentment(pPacket->nAnimalContentment);
					pAnimal->SetInterest(pPacket->nAnimalInterest);

					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());
					if( !pPlayer ) return;
					// 모션
					pPlayer->StartFarmMotion(CSHFarmManager::FARM_BREED_CHAR_MOTION, CAnimal::ANIMAL_IN_STALL_TIME);

					if( HERO->GetID() == pPlayer->GetID() )
					{
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg();
						if( pDlg ) pDlg->GetFarmRePlantTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_ANIMAL_REINSTALL, NULL, CAnimal::ANIMAL_IN_STALL_TIME*1000*60);
					}

					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg();
					if( pGuageDlg )
					{
						if( (CObject*)pPen->GetRenderObjEx(pAnimal->GetID()+5) == OBJECTMGR->GetSelectedObject() )
						{
							pGuageDlg->GetMonsterGuage()->SetActive(TRUE);
						}
					}
				}
				break;
			// 비어있는 축사가 아님
			case CSHMain::RESULT_FAIL_01:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1557 ) );
				}
				break;
			case CSHMain::RESULT_FAIL_02:
				{
				}
				break;
			case CSHMain::RESULT_FAIL_03:
				{
				}
				break;
			// 가축과 거리가 멀다...
			case CSHMain::RESULT_FAIL_04:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1554 ) );
				break;
			// 해당 패밀리가 없거나 해당맴버가 아닐때 에러 처리
			case CSHMain::RESULT_FAIL_05:
			case CSHMain::RESULT_FAIL_06:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1555 ) );
				break;
			// 필요 패밀리 맴버수가 부족함.
			case CSHMain::RESULT_FAIL_07:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1564 ) );
				break;
			}
		}
		break;
	// 080508 KTH -- 가축 사료 주기
	case MP_FARM_ANIMAL_FEED:
		{
			PACKET_FARM_CAGE_ANIMAL_FEED* pPacket = (PACKET_FARM_CAGE_ANIMAL_FEED*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*		pFarm		= pFarmZone->GetFarm(pPacket->nFarmID);
			CPen*			pAnimalCage	= pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			CAnimal*		pAnimal		= pAnimalCage->GetAnimal(pPacket->nAnimalID);
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());

			if( !pPlayer )
				return;
			else
			{
				VECTOR3* pos;
				pos = pAnimal->GetPos();
				MOVEMGR->SetLookatPos(pPlayer,pos,0,gCurTime);
			}

			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
				{
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());
					if( !pPlayer )	return;

					pPlayer->StartFarmMotion(CSHFarmManager::FARM_FEED_CHAR_MOTION, CAnimal::ANIMAL_FEED_TIME);
					// 사료 준 시간 설정
					pAnimal->SetFeedTimeTick(gCurTime);
					pAnimal->Feed(pPacket->nFoodKind);
					pAnimal->SetContentment(pPacket->nAnimalContentment);
					pAnimal->SetInterest(pPacket->nAnimalInterest);

					EFFECTMGR->StartEffectProcess(FindEffectNum("stable_cattle_03.beff"), pAnimal->GetRenderObj(), NULL, 0, pAnimal->GetID());

					//pPlayer->StartFarmMotion(CSHFarmManager::FARM_CLEANING_CHAR_MOTION, CAnimal::ANIMAL_CLEANING_TIME);
					
					// 다시 비료주기 위해 필요한 시간 아이콘 표시 ..// 080616 기획팀의 요청으로 주석처리;
					/*if (HERO->GetID() == pPlayer->GetID())
					{
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
						if (pDlg) pDlg->GetFarmReManureTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_ANIMAL_REFEED, NULL, CAnimal::ANIMAL_FEED_TIME*1000*60);
					}*/
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_FEED])
					{
						TARGETSET set;
						set.pTarget = pAnimal->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_FEED], pAnimal->GetRenderObj(), &set, 0, pAnimal->GetRenderObj()->GetID());
					}
				}
				break;

			// 현재 해당하는곳에 가축이 없다.
			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1556 ) );
				break;
			// 소유자가 틀리다!
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1557 ) );
				break;
			// 이미 가축의 성장이 끝났다!
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1558 ) );
				break;
			// 먹이를 줄수 있는 딜레이 시간이 남아있다!
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1559 ), CAnimal::ANIMAL_FEED_DELAY_TIME );
				break;
			// 가축과의 거리가 멀다
			case CSHMain::RESULT_FAIL_05:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1554 ) );
				break;
			}
			GAMEIN->GetAnimalDialog()->RefreshAnimalInfo();
		}
		break;
	// 080508 KTH -- 가축 청소 하기
	case MP_FARM_ANIMAL_CLEANING:
		{
			PACKET_FARM_CAGE_ANIMAL_FEED* pPacket = (PACKET_FARM_CAGE_ANIMAL_FEED*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*		pFarm		= pFarmZone->GetFarm(pPacket->nFarmID);
			CPen*			pAnimalCage	= pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			CAnimal*		pAnimal		= pAnimalCage->GetAnimal(pPacket->nAnimalID);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());

			if( pPlayer != NULL )
			{
				VECTOR3* pos;
				pos = pAnimal->GetPos();
				MOVEMGR->SetLookatPos(pPlayer,pos,0,gCurTime);
			}

			switch(pPacket->nResult)
			{
			case CSHMain::RESULT_OK:
				{
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());
					if( !pPlayer )	return;
					pPlayer->StartFarmMotion(CSHFarmManager::FARM_CLEANING_CHAR_MOTION, CAnimal::ANIMAL_CLEANING_TIME);
					// 비료 준 시간 설정
					pAnimal->SetCleaningTimeTick(gCurTime);
					pAnimal->Cleaning(pPacket->nFoodKind);
					pAnimal->SetContentment(pPacket->nAnimalContentment);
					pAnimal->SetInterest(pPacket->nAnimalInterest);

					EFFECTMGR->StartEffectProcess(FindEffectNum("stable_cattle_02.beff"), pAnimal->GetRenderObj(), NULL, 0, pAnimal->GetID());
					
					// 다시 비료주기 위해 필요한 시간 아이콘 표시 ..// 080616 기획팀 요청으로 주석;
					/*if (HERO->GetID() == pPlayer->GetID())
					{
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
						if (pDlg) pDlg->GetFarmReManureTimeBar()->CLI_Start(CSHProgressBar::BARTYPE_FARM_ANIMAL_RECLEANING, NULL, CAnimal::ANIMAL_CLEANING_TIME*1000*60);
					}*/
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_CLEANNING])
					{
						TARGETSET set;
						set.pTarget = pAnimal->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_CLEANNING], pAnimal->GetRenderObj(), &set, 0, pAnimal->GetRenderObj()->GetID());
					}
				}
				break;

			// 현재 해당하는 곳에 가축이 없다!
			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1556 ) );
				break;
			// 소유자가 틀리다!
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1557 ) );
				break;
			// 이미 가축의 성장이 끝났다!
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1560 ) );
				break;
			// 아직 먹이를 줄 시간 딜레이가 끝나지 않았다!
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1561 ), CAnimal::ANIMAL_CLEANING_RETRY_TIME );
				break;
			case CSHMain::RESULT_FAIL_05:	
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1058 ) );
				break;
			}
			GAMEIN->GetAnimalDialog()->RefreshAnimalInfo();
		}
		break;
	case MP_FARM_ANIMAL_REWARD:
		{
 			PACKET_FARM_CAGE_ANIMAL_REWARD* pPacket = (PACKET_FARM_CAGE_ANIMAL_REWARD*)pMsg;
			
			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if (pFarmZone == NULL) return;
			CSHFarm*		pFarm			= pFarmZone->GetFarm(pPacket->nFarmID);
			CPen*			pAnimalCage		= pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			CAnimal*		pAnimal			= pAnimalCage->GetAnimal(pPacket->nAnimalID);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());
			if( !pPlayer )
				return;

			VECTOR3* pos;
			pos = pAnimal->GetPos();
			MOVEMGR->SetLookatPos(pPlayer,pos,0,gCurTime);

			switch(pPacket->nResult)
			{
			// 모션 시작
			case CSHMain::RESULT_OK:
				{
					// 캐릭터 모션 연출
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pAnimal->GetOwner());
					if( !pPlayer )	return;
					// 테스트 용도로 텃밭 수확과 같은 타임을 사용함.
					pPlayer->StartFarmMotion(CSHFarmManager::FARM_REWARD_ANIMAL_CHAR_MOTION, CAnimal::ANIMAL_STALL_REWARD_TIME);
					// 수확 모션 시간 설정
					pAnimal->SetBreedTimeTick(gCurTime);
				}
				break;
			// 모션 종료 & 실제 수확
			case CSHMain::RESULT_OK_02:
				{
					// ..이펙트
					if (FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_REWARD])
					{
						TARGETSET set;
						set.pTarget = pAnimal->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_REWARD], pAnimal->GetRenderObj(), &set, 0, pAnimal->GetRenderObj()->GetID());
					}

					// 기존 오브젝트 제거
 					CLI_RemoveRenderFarmAnimalObjToScreen(pAnimal);
					pAnimalCage->GetRenderObjEx(pPacket->nAnimalID+5)->GetEngineObject()->EnablePick();
					pAnimal->SetAnimal(CAnimal::ANIMAL_RESULT_DIE);

					// 타켓 해제
 					if(OBJECTMGR->GetSelEffectHandle())
					{
						EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
					}

					/*if ((CObject*)pAnimal->GetRenderObj() == OBJECTMGR->GetSelectedObject())
					{
						OBJECTMGR->GetSelectedObject()->ShowObjectName( FALSE );
						OBJECTMGR->GetSelectedObject()->OnDeselected();
						OBJECTMGR->SetSelectedObjectID(0);
					}*/

					CLI_SetTargetObj(NULL);
					GAMEIN->GetAnimalDialog()->SetActive(FALSE);
				}
				break;
			// 소유자가 틀리다
			case CSHMain::RESULT_FAIL_01:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1557 ) );
				break;
			// 가축이 다 자라지 않았다
			case CSHMain::RESULT_FAIL_02:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1562 ) );
				break;
			// 가축과의 거리가 멀다.
			case CSHMain::RESULT_FAIL_03:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1554 ) );
				break;
			// 보상을 받을 공간이 없다.
			case CSHMain::RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1563 ) );
				break;
			}
		}
		break;
	// 080417 KTH -- 가축 성장
	case MP_FARM_ANIMAL_GROW:
		{
			PACKET_FARM_CAGE_ANIMAL_GROW* pPacket = (PACKET_FARM_CAGE_ANIMAL_GROW*)pMsg;

			CSHFarmZone* pFarmZone = GetFarmZone((CSHFarmZone::FARM_ZONE)pPacket->nFarmZone);
			if( pFarmZone == NULL ) return;

			CSHFarm*	pFarm		= pFarmZone->GetFarm(pPacket->nFarmID);
			if( !pFarm )
				break;
			CPen*		pAnimalCage = pFarm->GetAnimalCage(pPacket->nAnimalCageID);
			if( !pAnimalCage )
				break;
			CAnimal*	pAnimal		= pAnimalCage->GetAnimal(pPacket->nAnimalID);
			if( !pAnimal )
				break;

			switch( pPacket->nResult )
			{
			// 만족도 변화
			case CAnimal::ANIMAL_RESULT_CONTENTMENT:
				{
					pAnimal->SetContentment(pPacket->nValue);
					GAMEIN->GetAnimalDialog()->RefreshAnimalInfo();
				}
				break;
			// 관심도 변화
			case CAnimal::ANIMAL_RESULT_INTEREST:
				{
					pAnimal->SetInterest(pPacket->nValue);
					GAMEIN->GetAnimalDialog()->RefreshAnimalInfo();
				}
				break;
			// 생명력 변화..
			case CAnimal::ANIMAL_RESULT_LIFE:
				{
					pAnimal->SetLife(pPacket->nValue);
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg();
					if( pGuageDlg )
					{
						if( (CObject*)pAnimal->GetRenderObj() == OBJECTMGR->GetSelectedObject() )
						{
							pGuageDlg->SetMonsterLife(pAnimal->GetLife(), pAnimal->GetMaxLife());
						}
					}
					GAMEIN->GetAnimalDialog()->RefreshAnimalInfo();
				}
				break;
			case CAnimal::ANIMAL_RESULT_REWARD:
				{
					//if( pAnimal->GetInterest() > 10 && pAnimal->GetContentment() == 100 )
					//	GAMEIN->GetAnimalDialog()->GetItemBtn()->SetActive(TRUE);
						//GAMEIN->GetAnimalDialog()->AbleGetItemBtn();
					if( GAMEIN->GetAnimalDialog()->IsActive() )
						GAMEIN->GetAnimalDialog()->SetActive(FALSE);
				}
				break;
			// 죽음..
			case CAnimal::ANIMAL_RESULT_DIE:
				{
					CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg();
					if( pGuageDlg )
					{
						if( (CObject*)pAnimal->GetRenderObj() == OBJECTMGR->GetSelectedObject() )
						{
							pGuageDlg->GetMonsterGuage()->SetActive(FALSE);
							if( OBJECTMGR->GetSelEffectHandle() )
							{
								EFFECTMGR->ForcedEndEffect(OBJECTMGR->GetSelEffectHandle());
							}

							if( OBJECTMGR->GetSelectedObject() )
							{
								OBJECTMGR->GetSelectedObject()->ShowObjectName(FALSE);
								OBJECTMGR->GetSelectedObject()->OnDeselected();
								OBJECTMGR->SetSelectedObjectID(0);
							}

							CLI_SetTargetObj(NULL);
						}
					}

					if( GAMEIN->GetAnimalDialog()->IsActive() )
						GAMEIN->GetAnimalDialog()->SetActive(FALSE);

					// 이펙트
					if( FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_DIE] )
					{
						TARGETSET set;
						if( !pAnimal->GetRenderObj() )
							return;
						set.pTarget = pAnimal->GetRenderObj();
						EFFECTMGR->StartEffectProcess(FARM_EFFECT_NUM[FARM_EFFECT_KIND_ANIMAL_DIE], pAnimal->GetRenderObj(), &set, 0, pAnimal->GetRenderObj()->GetID());
					}

					CLI_RemoveRenderFarmAnimalObjToScreen(pAnimal);

					pAnimal->SetAnimal((CAnimal::ANIMAL_RESULT)pPacket->nResult);
					pAnimalCage->GetRenderObjEx(pPacket->nAnimalID+5)->GetEngineObject()->EnablePick();
				}
				break;
			}
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SaveAnimalInfoToDB Method																					 DB에 가축 데이터 저장
//
VOID CSHFarmManager::SRV_SaveAnimalInfoToDB(WORD nFarmZone, WORD nFarmID, CAnimal* pAnimal)
{
#if defined(_MAPSERVER_)
	char szText[256];
	sprintf(szText, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",	STORED_FARM_SET_ANIMALINFO,
																		nFarmZone,
																		nFarmID, 
																		pAnimal->GetID(),
																		pAnimal->GetOwner(),
																		pAnimal->GetKind(), 
																		pAnimal->GetStep(), 
																		pAnimal->GetLife(), 
																		pAnimal->GetNextStepTime(),
																		pAnimal->GetContentment(),
																		pAnimal->GetInterest()); 
	g_DB.Query(eQueryType_FreeQuery, eFarm_SetAnimalInfo, 0, szText);

	// 점검 후에 농작물이 사라진다는 제보가 있어, 파일로도 데이터를 남긴다.
	// 이 데이터와 서버를 시작할 때 DB에서 로드한 데이터와 비교한다.
	char szFile[256];
	sprintf(szFile, "./Log/FARMANIMAL_VERIFY_%02d%02d%02d.txt", nFarmZone, nFarmID, pAnimal->GetID());
	if (pAnimal->GetLife())
	{
		FILE* fp;
		fp = fopen(szFile, "wt");
		if (fp)
		{
			fprintf(fp, "%d %d %d %d %d\n",
				pAnimal->GetOwner(),
				pAnimal->GetStep(),
				pAnimal->GetLife(),
				pAnimal->GetKind(),
				pAnimal->GetNextStepTime(),
				pAnimal->GetContentment(),
				pAnimal->GetInterest());
			fclose(fp);
		}
	}
	else
	{
		DeleteFile(szFile);
	}
#endif
}
// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendAnimalInfoFromDBQueue Method																		DB 큐를 참조하여 가축 설정
//
VOID CSHFarmManager::SRV_SetAnimalInfoFromDBQueue(CPlayer* pPlayer, CSHFarmManager::PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT* pPacket)
{
	#if defined(_MAPSERVER_)
	// 범위 체크
	if (pPacket->nFarmZone  >= CSHFarmZone::FARM_ZONE_MAX) return;
	if (pPacket->nFarmID >= m_pcsFarmZone[pPacket->nFarmZone]->GetFarmNum()) return;

	CSHFarm* pcsFarm = m_pcsFarmZone[pPacket->nFarmZone]->GetFarm(pPacket->nFarmID);
	if( pPacket->nAnimalCageID >= pcsFarm->GetAnimalCageNum() ) return;

	CPen*		pcsPen		= pcsFarm->GetAnimalCage(pPacket->nAnimalCageID);
	if( pPacket->nAnimalID >= pcsPen->GetAnimalNum() ) return;


	CAnimal* pcsAnimal = pcsPen->GetAnimal(pPacket->nAnimalID);

	pcsAnimal->SetOwner(pPacket->nAnimalOwner);
	pcsAnimal->SetStep((CAnimal::ANIMAL_STEP)pPacket->nAnimalStep);
	pcsAnimal->SetLife(pPacket->nAnimalLife);
	pcsAnimal->SetKind(pPacket->nAnimalKind);
	pcsAnimal->SetNextStepTime(pPacket->nAnimalNextStepTime);
	pcsAnimal->SetContentment(pPacket->nAnimalContentment);
	pcsAnimal->SetInterest(pPacket->nAnimalInterest);

	// 가축 로드 상태를 파일로 남긴다.
	SYSTEMTIME st;
	GetLocalTime(&st);

	FILE* fp;
	char szFile[256];

	// 파일로 남기는 마지막 농작물의 데이터와 비교한다.
	BOOL bWrongData = FALSE;
	sprintf(szFile, "./Log/FARMANIMAL_VERIFY_%02d%02d%02d.txt", pPacket->nFarmZone, pPacket->nFarmID, pcsAnimal->GetID());
	fp = fopen(szFile, "rt");
	if (fp)
	{
		int nOwner = 0, nStep = 0, nLife = 0, nKind = 0, nNextStepTime = 0, nAnimalContentment = 0, nAnimalInterest = 0;
		fscanf(fp, "%d %d %d %d %d %d %d %d", 
			&nOwner,
			&nStep,
			&nLife,
			&nKind,
			&nNextStepTime,
			&nAnimalContentment,
			&nAnimalInterest);
		fclose(fp);

		if (pcsAnimal->GetOwner()			!=	nOwner			||
			pcsAnimal->GetStep()			!=	nStep			||
			pcsAnimal->GetLife()			!=	nLife			||
			pcsAnimal->GetKind()			!=	nKind			||
			pcsAnimal->GetNextStepTime()	!=	nNextStepTime	||
			pcsAnimal->GetContentment()		!=	nAnimalContentment ||
			pcsAnimal->GetInterest()		!=	nAnimalInterest )
		{
			bWrongData = TRUE;
		}
	}
	else
	{
		// DB에서는 가져왔는 데 파일로는 안 남았다?
		bWrongData = 2;
	}

	sprintf(szFile, "./Log/FARMCROP_VERIFY_CHECK_%02d%02d%02d.txt", pPacket->nFarmZone, pPacket->nFarmID, pcsAnimal->GetID());
	fp = fopen(szFile, "wt");
	if (fp)
	{
		if (bWrongData == FALSE)		fprintf(fp, "DATA_FROM_FILE and FROM_DB OK!!");
		else if (bWrongData == TRUE)	fprintf(fp, "DATA_FROM_FILE and FROM_DB DATA DIFF!!");
		else if (bWrongData == 2)		fprintf(fp, "FROM_DB Only...??");
		fclose(fp);
	}

	// DB에서 로드한 데이터 기록
	sprintf(szFile, "./Log/FARMCROP_LOAD_START_%4d%02d%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	fp = fopen(szFile, "a+");
	if (fp)
	{
		fprintf(fp, "ZONE:%d FARM:%d CROP:%d OWNER:%d STEP:%d LIFE:%d SEEDGRD:%d KIND:%d NEXTSTEPTIME:%d CONTENTMENT:%d INTEREST:%d\n",
			pPacket->nFarmZone,
			pcsFarm->GetID(),
			pcsAnimal->GetID(),
			pcsAnimal->GetOwner(),
			pcsAnimal->GetStep(),
			pcsAnimal->GetLife(),
			pcsAnimal->GetKind(),
			pcsAnimal->GetNextStepTime(),
			pcsAnimal->GetContentment(),
			pcsAnimal->GetInterest());
		fclose(fp);
	}
#endif
}


// 080417 KTH 
// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_AddRenderFarmAnimalObjToScreen Method															  화면에 렌더링 가축 오브젝트 추가
//
VOID CSHFarmManager::CLI_AddRenderFarmAnimalObjToScreen(CAnimal* pAnimal)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if( pAnimal->GetStep() < CAnimal::ANIMAL_STEP_1 ) return;

	BASEOBJECT_INFO		binfo;
	BASEMOVE_INFO		minfo;
	CSHFarmRenderObj::stFarmRenderObjInfo stFarmRenderObj;

	int nCnt = 0;
	DWORD n = 0;
	while(m_pstBFarm[nCnt].nID)
	{
		if (m_pstBFarm[nCnt].nKind != RENDER_ANIMAL_KIND)
		{
			nCnt++;
			continue;
		}

		stFarmRenderObj.nKind = m_pstBFarm[nCnt].nKind + pAnimal->GetKind();
		//stFarmRenderObj.nKind = m_pstBFarm[nCnt].nKind;
		//int nStep = pCrop->GetStep() - CSHCrop::CROP_STEP_1;
		//if (nStep < 0) nStep = 0;
		//if (nStep > CSHCrop::CROP_STEP_MAX - 1) nStep = CSHCrop::CROP_STEP_MAX - 1;
		//stFarmRenderObj.nKind += ((pCrop->GetKind()-1)*100 + nStep);

		SafeStrCpy( binfo.ObjectName, m_pstBFarm[nCnt].szName, MAX_NAME_LENGTH+1 );

		stFarmRenderObj.nID = (pAnimal->GetParent()->GetParent()->GetID()+1)*100 + pAnimal->GetID()+1;
		stFarmRenderObj.nMapNum = m_pstBFarm[nCnt].nMapNum;

		binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		while(OBJECTMGR->GetObject(binfo.dwObjectID))
		{
			binfo.dwObjectID = FARM_RENDER_OBJ_ID_START + n++;
		}

		binfo.ObjectState = 0;
		minfo.bMoving = FALSE;
		minfo.KyungGongIdx = 0;
		minfo.MoveMode = 0;

		float fDir = DEGTORAD(CSHMath::GetRandomNum(1, 20000));

		minfo.CurPosition.x = pAnimal->GetPos()->x;
		minfo.CurPosition.y = 0;
 		minfo.CurPosition.z = pAnimal->GetPos()->z;
		fDir = pAnimal->GetParent()->GetParent()->GetDir();//pAnimal->GetDir();
		fDir += 1.5f;
		//fDir = pAnimal->GetDir();


		CSHFarmRenderObj* pFarmRenderObj = OBJECTMGR->AddFarmRenderObj(&binfo, &minfo, &stFarmRenderObj);

 		pFarmRenderObj->SetAngle(fDir);
		CLI_SetFarmObjForRender(pFarmRenderObj);
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_DelAnimal Method																										가축 삭제
//
VOID CSHFarmManager::SRV_DelAnimal(DWORD nOwnerID)
{
#if defined(_MAPSERVER_)
	// 농장맵인가?
	if (GetFarmZoneID(g_pServerSystem->GetMapNum(), NULL) == FALSE) return;

	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 농장 지역
 		for(int j=0; j<m_pcsFarmZone[i]->GetFarmNum(); j++)
		{
			// 농장
			CSHFarm* pFarm = m_pcsFarmZone[i]->GetFarm(j);
			for(int k=0; k<pFarm->GetAnimalCageNum(); k++)
			{
				CPen* pAnimalCage = pFarm->GetAnimalCage(k);
				for(int p=0; p<pAnimalCage->GetAnimalNum(); p++)
				{
					CAnimal* pAnimal = pAnimalCage->GetAnimal(p);
					if (pAnimal->GetOwner() == nOwnerID)
					{
						// Log
						InsertLogFarmAnimal( pFarm, pAnimal, eLog_FamilyFarmLostRight );

						pAnimal->AddEvent(NULL, CAnimal::ANIMAL_RESULT_DIE);
						pAnimal->SetAnimal(CAnimal::ANIMAL_RESULT_DIE);
					}
				}
			}
		}
	}							
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RemoveRenderFarmAnimalObjToScreen Method														  화면에 렌더링 가축 오브젝트 제거
//
VOID CSHFarmManager::CLI_RemoveRenderFarmAnimalObjToScreen(CAnimal* pAnimal)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	// 농작물
	OBJECTMGR->AddGarbageObject(pAnimal->GetRenderObj());
	pAnimal->SetRenderObj(NULL);

	CPen* pAnimalCage = (CPen*)pAnimal->GetParent();

	// 080428 KTH -- 보이게 해준다~!~!
	pAnimalCage->GetRenderObjEx(pAnimal->GetID()+5)->GetEngineObject()->Show();
	pAnimalCage->GetRenderObjEx(pAnimal->GetID()+5)->GetEngineObject()->EnablePick();

#endif
}

VOID CSHFarmManager::SRV_FeedAnimal(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	int nFoodKind = 0;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	if( nAnimalCageID >= pFarm->GetAnimalCageNum() )
	{
		return;
	}

	CPen* pAnimalCage = pFarm->GetAnimalCage(nAnimalCageID);

	// 가축 범위 체크
	if( nAnimalID >= pAnimalCage->GetAnimalNum() )
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_ANIMAL_FOOD)
	{
		return;
	}
	nFoodKind = pItemInfo->SupplyValue;
	//nManureGrade = pItemInfo->Grade;

	// 사료 종류 범위 체크
	if (nFoodKind > CAnimal::ANIMAL_FEED_KIND_MAX) 
	{
		return;
	}

	// 씨앗 등급 범위 체크
/*	if (nManureGrade > CSHCrop::CROP_SEED_GRADE_MAX)
	{
		return;
	}*/

	CAnimal* pAnimal = pAnimalCage->GetAnimal(nAnimalID);

	if( pAnimal->GetStep() < CAnimal::ANIMAL_STEP_1 )
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	if( pAnimal->GetOwner() != pPlayer->GetID() )
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	if( pAnimal->GetStep() == CAnimal::ANIMAL_STEP_COMPLETE )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	if( pPlayer->GetFarmInfo()->nAnimalFeedRetryTime )
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pAnimal->GetPos(), &stPlayerPos); 
	if (nDistance > PEN_OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_05;
		goto FAILED;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	pAnimal->Feed(nFoodKind);

	SRV_SaveAnimalInfoToDB(eFarmZone, nFarmID, pAnimal);

	pPlayer->SetFarmAnimalFeedRetryTime(CAnimal::ANIMAL_FEED_DELAY_TIME);
	Farm_SetTimeDelay(pPlayer->GetID(), FARM_TIMEDELAY_KIND_FEED, CAnimal::ANIMAL_FEED_DELAY_TIME);

	InsertLogFarmAnimal( pFarm, pAnimal, eLog_FamilyFarmFeedAnimal );

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_CAGE_ANIMAL_FEED stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_ANIMAL_FEED;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nAnimalCageID		= nAnimalCageID;
	stPacket.nAnimalID			= pAnimal->GetID();
	stPacket.nAnimalLife		= pAnimal->GetLife();
	stPacket.nAnimalContentment	= pAnimal->GetContentment();
	stPacket.nAnimalInterest	= pAnimal->GetInterest();
	stPacket.nFoodKind			= nFoodKind;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		//SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

VOID CSHFarmManager::CLI_RequestFeedAnimalInCage(WORD nItemTargetPos, DWORD nItemIdx)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_ANIMAL_KIND)
		{
			PACKET_FARM_CAGE_ANIMAL_FEED stPacket;

			stPacket.Category 			= MP_FARM;
			stPacket.Protocol 			= MP_FARM_ANIMAL_FEED;
			stPacket.dwObjectID			= HEROID;
			stPacket.nFarmZone			= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID			= m_stSelFarmObj.nFarmID;
			stPacket.nAnimalCageID		= m_stSelFarmObj.nAnimalCageID;
			stPacket.nAnimalID			= m_stSelFarmObj.nAnimalID;
			stPacket.nItemTargetPos		= nItemTargetPos;
			stPacket.nItemIdx			= nItemIdx;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

CAnimal* CSHFarmManager::CLI_GetAnimalInfo(CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID)
{
	CAnimal* pAnimal = NULL;
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return NULL;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return NULL;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return NULL;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	if( nAnimalCageID >= pFarm->GetAnimalCageNum() )
	{
		return NULL;
	}

	CPen* pAnimalCage = pFarm->GetAnimalCage(nAnimalCageID);

	// 가축 범위 체크
	if( nAnimalID >= pAnimalCage->GetAnimalNum() )
	{
		return NULL;
	}

	 pAnimal = pAnimalCage->GetAnimal(nAnimalID);

#endif
	if( !pAnimal )
		return NULL;
	return pAnimal;
}

VOID CSHFarmManager::SRV_CleaningAnimal(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nItemTargetPos, DWORD nItemIdx)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	int nCleaningKind = 0;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	if( nAnimalCageID >= pFarm->GetAnimalCageNum() )
	{
		return;
	}

	CPen* pAnimalCage = pFarm->GetAnimalCage(nAnimalCageID);

	// 가축 범위 체크
	if( nAnimalID >= pAnimalCage->GetAnimalNum() )
	{
		return;
	}

	// 아이템 체크
	ITEM_INFO * pItemInfo = ITEMMGR->GetUseItemInfo(pPlayer, nItemTargetPos, nItemIdx);
	if (pItemInfo == NULL || pItemInfo->SupplyType != ITEM_KIND_ANIMAL_CLEANER)
	{
		return;
	}
	nCleaningKind = pItemInfo->SupplyValue;

	// 클리러 종류 범위 체크
	if (nCleaningKind > CAnimal::ANIMAL_CLEANER_KIND_MAX) 
	{
		return;
	}

	CAnimal* pAnimal = pAnimalCage->GetAnimal(nAnimalID);

	if( pAnimal->GetStep() < CAnimal::ANIMAL_STEP_1 )
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	if( pAnimal->GetOwner() != pPlayer->GetID() )
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	if( pAnimal->GetStep() == CAnimal::ANIMAL_STEP_COMPLETE )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	if( pPlayer->GetFarmInfo()->nAnimalCleanRetryTime )
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pAnimal->GetPos(), &stPlayerPos); 
	if (nDistance > PEN_OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_05;
		goto FAILED;
	}

	// 아이템 제거
	if (ITEMMGR->UseItem(pPlayer, nItemTargetPos, nItemIdx) == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_USE_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.TargetPos	= nItemTargetPos;
		msg.wItemIdx	= nItemIdx;

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		return;
	}

	pAnimal->Cleaning(nCleaningKind);

	SRV_SaveAnimalInfoToDB(eFarmZone, nFarmID, pAnimal);

	pPlayer->SetFarmAnimalCleanRetryTime(CAnimal::ANIMAL_CLEANING_RETRY_TIME);
	Farm_SetTimeDelay(pPlayer->GetID(), FARM_TIMEDELAY_KIND_CLEAN, CAnimal::ANIMAL_CLEANING_RETRY_TIME);

	InsertLogFarmAnimal( pFarm, pAnimal, eLog_FamilyFarmCleanAnimal );

FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_CAGE_ANIMAL_FEED stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_ANIMAL_CLEANING;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nAnimalCageID		= nAnimalCageID;
	stPacket.nAnimalID			= pAnimal->GetID();
	stPacket.nAnimalLife		= pAnimal->GetLife();
	stPacket.nAnimalContentment	= pAnimal->GetContentment();
	stPacket.nAnimalInterest	= pAnimal->GetInterest();
	stPacket.nFoodKind			= nCleaningKind;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		//SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

VOID CSHFarmManager::CLI_RequestCleaningAnimalInCage(WORD nItemTargetPos, DWORD nItemIdx)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (m_pcsSelFarmObj)
	{
		if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_ANIMAL_KIND)
		{
			PACKET_FARM_CAGE_ANIMAL_FEED stPacket;

			stPacket.Category 			= MP_FARM;
			stPacket.Protocol 			= MP_FARM_ANIMAL_CLEANING;
			stPacket.dwObjectID			= HEROID;
			stPacket.nFarmZone			= m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID			= m_stSelFarmObj.nFarmID;
			stPacket.nAnimalCageID		= m_stSelFarmObj.nAnimalCageID;
			stPacket.nAnimalID			= m_stSelFarmObj.nAnimalID;
			stPacket.nItemTargetPos		= nItemTargetPos;
			stPacket.nItemIdx			= nItemIdx;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

VOID CSHFarmManager::CLI_RequestAnimalGetItem(WORD nType)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if( m_pcsSelFarmObj )
	{
		if( GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_ANIMAL_KIND )
		{
			PACKET_FARM_CAGE_ANIMAL_REWARD stPacket;

			stPacket.Category		=	MP_FARM;
			stPacket.Protocol		=	MP_FARM_ANIMAL_REWARD;
			stPacket.dwObjectID		=	HEROID;
			stPacket.nFarmZone		=	m_stSelFarmObj.nFarmZone;
			stPacket.nFarmID		=	m_stSelFarmObj.nFarmID;
			stPacket.nAnimalCageID	=	m_stSelFarmObj.nAnimalCageID;
			stPacket.nAnimalID		=	m_stSelFarmObj.nAnimalID;
			stPacket.nResult		=	nType;

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

VOID CSHFarmManager::SRV_AnimalGetItem(CPlayer* pPlayer, CSHFarmZone::FARM_ZONE eFarmZone, WORD nFarmID, WORD nAnimalCageID, WORD nAnimalID, WORD nType)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 농장 지역 체크
	if (eFarmZone >= CSHFarmZone::FARM_ZONE_MAX)
	{
		return;
	}

	CSHFarmZone* pFarmZone = m_pcsFarmZone[eFarmZone];
	if (pFarmZone == NULL) return;

	// 농장 범위 체크
	if (nFarmID >= pFarmZone->GetFarmNum())
	{
		return;
	}

	CSHFarm* pFarm = pFarmZone->GetFarm(nFarmID);

	// 축사 범위 체크
	if (nAnimalCageID >= pFarm->GetAnimalCageNum() )
	{
		return;
	}

	CPen* pAnimalCage = pFarm->GetAnimalCage(nAnimalCageID);

	// 가축 범위 체크
	if( nAnimalID >= pAnimalCage->GetAnimalNum() )
	{
		return;
	}

	CAnimal* pAnimal = pAnimalCage->GetAnimal(nAnimalID);

	// 가축 주인만...
	if( pAnimal->GetOwner() != pPlayer->GetID() )
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	if(pAnimal->GetContentment() == 100 && pAnimal->GetInterest() > 10)//m_nContentment == 100 && m_nInterest > 10
	{
		if( pAnimal->GetStep() != CAnimal::ANIMAL_STEP_COMPLETE )
		{
			pAnimal->SetStep(CAnimal::ANIMAL_STEP_COMPLETE);
		}
	}

	// 가축이 다 자랐나?
	if (pAnimal->GetStep() != CAnimal::ANIMAL_STEP_COMPLETE )
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 거리체크..
	VECTOR3 stPlayerPos;
	pPlayer->GetPosition(&stPlayerPos);
	float nDistance = CalcDistanceXZ(pAnimal->GetPos(), &stPlayerPos); 
	if (nDistance > PEN_OBJECT_CONTROL_DISTANCE)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 아이템 공간 체크
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"올바른 아이템 슬롯을 얻을 수 없습니다.");

	// 071211 KTH -- pPlayer 추가
	DWORD nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);
	if(nNum == 0)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 처음 수확 요청 시
	if (nType == RESULT_OK)
	{
		goto FAILED;
	}
	// 모션 종료 후 실제 수확 요청 시
	else
	{
		eResult = RESULT_OK_02;
	}

	DWORD nPercent = CSHMath::GetRandomNum(1, 1000);
	DWORD nResult = 0;
	DWORD nRow = 0;

	if( pAnimal->GetInterest() <= 20 )
	{
		nRow = CAnimal::ANIMAL_BREED_KIND_BAD;
	}
	else if( pAnimal->GetInterest() < 30 )
	{
		nRow = CAnimal::ANIMAL_BREED_KIND_NORMAL;
	}
	else
	{
		nRow = CAnimal::ANIMAL_BREED_KIND_GOOD;
	}

	if( m_pstAnimalReward[pAnimal->GetKind()].nPercent[nRow*3]*10 > nPercent )
	{
		nResult = (3*nRow)+CAnimal::ANIMAL_BREED_KIND_BAD;
	}
	else if( m_pstAnimalReward[pAnimal->GetKind()].nPercent[nRow*3]*10 + m_pstAnimalReward[pAnimal->GetKind()].nPercent[nRow*3+1]*10 > nPercent )
	{
		nResult = (3*nRow)+CAnimal::ANIMAL_BREED_KIND_NORMAL;
	}
	else
	{
		nResult = (3*nRow)+CAnimal::ANIMAL_BREED_KIND_GOOD;
	}

	DWORD nItemID = m_pstAnimalReward[pAnimal->GetKind()].nItemID[nResult];
	DWORD nItemNum = m_pstAnimalReward[pAnimal->GetKind()].nItemNum[nResult];

	// ..유효 아이템인가?
	// ..스크립트 데이터 파일이나 기타등등의 오류로 유효하지 않은 아이템이라면 처리하지 않는다.
	ITEM_INFO* pstItem = ITEMMGR->GetItemInfo(nItemID);
	if (pstItem == NULL)
	{
		return;
	}

	InsertLogFarmAnimal( pFarm, pAnimal, eLog_FamilyFarmAnimalReward );

	// 수확 했으니 제거
	pAnimal->SetAnimal(CAnimal::ANIMAL_RESULT_DIE);

	// DB에 저장
	SRV_SaveAnimalInfoToDB(eFarmZone, nFarmID, pAnimal);

	ITEMMGR->ObtainMonstermeterItem(pPlayer, nItemID, nItemNum);
	
FAILED:
	// 클라이언트에 알리기
	PACKET_FARM_CAGE_ANIMAL_REWARD stPacket;

	stPacket.Category			= MP_FARM;
	stPacket.Protocol			= MP_FARM_ANIMAL_REWARD;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFarmZone			= (WORD)eFarmZone;
	stPacket.nFarmID			= nFarmID;
	stPacket.nAnimalCageID		= nAnimalCageID;
	stPacket.nAnimalID			= nAnimalID;
	stPacket.nResult			= (WORD)eResult;

	if (eResult == RESULT_OK ||
		eResult == RESULT_OK_02)
	{
		SRV_SendFarmInfoToNearPlayer(pFarm, &stPacket, sizeof(stPacket));
		// 농장UI 정보 전송
		//SRV_SendFarmUIInfoToClient(eFarmZone, nFarmID, nGardenID, pCrop, pFarm->GetOwner());
	}
	else
	{
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}
