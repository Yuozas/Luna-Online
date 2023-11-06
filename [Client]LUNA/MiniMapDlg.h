#pragma once


#include "Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"


class cStatic;
class cPushupButton;


struct MINIMAPIMAGE
{
	cImageSelf	image;
	SIZE		size;
};


//SW050802 snake_legs
struct stParticle
{
	VECTOR2 vPos;
	VECTOR2 vDir;
	stParticle()
	{
		vPos.x = vPos.y = 0;
		vDir.x = vPos.y = 0;
	}
};


struct stNumImage	//SW050722 for jackpot number dialog
{
	cImage* pImage;
	DWORD dwW,dwH;
	stNumImage():pImage(NULL),dwW(0),dwH(0){}
};


struct stCipherNum	//SW050722 for jackpot number dialog
{
	stCipherNum():dwNumber(99),dwRealCipherNum(0),bIsAni(FALSE){}
	DWORD dwNumber;// �ڸ��� ���� (��¿�)
	DWORD dwRealCipherNum;// �ڸ��� ���� (������)
	bool bIsAni;// ���� �ִ� ���� ������..(�ִϿ�)
};


class CMiniMapDlg : public cDialog  
{
//	DWORD TempID;
	
	cStatic* m_pMapName;
	// 080121 LYW --- MiniMapDlg : ä�ο� ����ƽ �߰�.
	cStatic* m_pChannel ;
	cStatic* m_pPosXY;	

	cPushupButton* m_pZoom;	

	// 070430 LYW --- MiniMapDlg : Add button for toggle minimap.
	cButton*	m_pToggleBtn ;

	MAPTYPE m_MapNum;

	CMiniMapHeroIcon m_HeroIcon;	
	CYHHashTable<CMiniMapIcon> m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon> m_IconPool;
	CMiniMapIconImage m_pIconImage[eMNMICON_MAX];
	
	int m_CurMode;
	MINIMAPIMAGE m_MinimapImage[eMINIMAPMODE_MAX];

	// ���� 070522
	struct stMiniMap
	{
		// cImageSelf*�� ���� ����ϴ� �̹����� �����ؼ� �� ��� DLL���� Access violation �߻�. 
		// �̾� cImage�� ��������Ʈ �̹����� �����ϴ� ��� ���
		cImage mImage;
		cImageRect mRect;
		VECTOR2 mScale;
		
		cImageSize mViewport;
		VECTOR2 mTranslation;
	}
	m_MiniMap;

//	cImage m_HeroImage;

//KES
	//cImage m_TooltipImage;
//	
	//float m_DisplayMinX,m_DisplayMinZ;
	//float m_DisplayMaxX,m_DisplayMaxZ;

	//VECTOR2 m_DiplayOriginPosOfImage;
	//VECTOR2 m_ImageDisplayWH;

	void InitIconImage();

	void SetZoomRate(float zoomrate);

	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,CObject* pObject);
	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember);
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind, CSHFarm* pFarm);
	// E ����ý��� �߰� added by hseos 2007.10.31
	void ReleaseMinniMapIcon(CMiniMapIcon* pIcon);

	void LoadMinimapImageInfo(MAPTYPE MapNum);

	void AddIcon(WORD Kind,CObject* pObject);
	void AddIcon(WORD Kind,PARTY_MEMBER* pMember);
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	void AddIcon(WORD Kind, CSHFarm* pFarm);
	// E ����ý��� �߰� added by hseos 2007.10.31

//SW050722 
//#define NUMIMAGE_W 8
//#define NUMIMAGE_H 14
//
	#define BASIC_ANI_TIMELENGTH	2000
	#define BETWEEN_ANI_TIMELENGTH	500		//interval between Numbers
	#define NUM_CHANGE_TIMELENGTH	10		//interval
	#define	DOT_NUM					2

	enum
	{
		DEFAULT_IMAGE = 99,
		NUM_0 = 0, 
		NUM_1,
		NUM_2, 
		NUM_3, 
		NUM_4, 
		NUM_5, 
		NUM_6, 
		NUM_7, 
		NUM_8, 
		NUM_9, 
		IMG_DOT, 
		IMAGE_COUNT, 
		CIPHER_NUM = 9
	};

	stNumImage	m_stNumImage[IMAGE_COUNT];	// ���� �̹���
	VECTOR2		m_vPos[CIPHER_NUM];		// ����â �ڸ��� ��ġ
	VECTOR2		m_vDotPos[DOT_NUM];		//SW050805	�ڸ��� ������ ��ġ �߰�

	DWORD		m_dwTotalMoney;			// �ѱݾ�	// ����� ���� ���� �ݾ�
	DWORD		m_dwOldTotalMoney;			// ���� �ݾ�(��ȭ��) // �񱳸� ���� ���� �ݾ�
	DWORD		m_dwTempMoney;			// @����	//

	stCipherNum m_stCipherNum[CIPHER_NUM];	// �� �ڸ��� ����� ����
	DWORD		m_dwAniStartTime;		// ���� �ִ� ���� �ð�
	DWORD		m_dwNumChangeTime;		// �ڸ� �ִ� ��ȯ ����	// ���� ���� ����
	DWORD		m_dwIntervalAniTime;	// �ڸ��� �ִ� ��ȯ ���� // ���� �ڸ� �ִϰ� ������ ..���� ���� �ڸ� �ִ� ���� �ϴ� ��~
	DWORD		m_dwMaxCipher;			// �ݾ� �� �ڸ��� (�ִϿ�)
	DWORD		m_dwCipherCount;		// �ڸ��� (�ִϿ�)
	bool		m_bIsAnimationing;			// ��ü �ִ� ���� ������..(�ִϿ�)
	bool		m_bDoSequenceAni;			// ForSequenceAni

/*	//SW050802 snake_legs
#define RND_RANGE	10
#define PARTICLE_PER_NUM 36
#define GRAVITY	0.5f
	stParticle	m_stPtc[CIPHER_NUM][36];
	DWORD	m_BeforeTime;*/
public:
	CMiniMapDlg();
	virtual ~CMiniMapDlg();
//KES
	virtual DWORD ActionEvent( CMouse* mouseInfo );
//	
	void InitMiniMap(MAPTYPE MapNum);
	void SetChannel(int nChannel);
	
	void Linking();
	virtual void SetActive( BOOL val );

	void ToggleMinimapMode();
	
	void AddMonsterIcon(CObject* pObject);
	void AddBossMonsterIcon(CObject* pObject);
	void AddStaticNpcIcon(CObject* pObject);

	void AddPartyMemberIcon(PARTY_MEMBER* pMember);
	void AddPartyMasterIcon(CObject* pObject);

	void AddQuestIcon( CObject* pTarget, int nKind );
	
	void AddHeroIcon(CObject* pObject);
//	void SetPartyIcon();

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	void AddFarmIcon(CSHFarm* pFarm);
	// E ����ý��� �߰� added by hseos 2007.10.31

	void RemoveIcon(DWORD ObjectID);

	//void FitToCenterIcon(CMiniMapIcon* pIcon);
	void Process();
	void Render();

	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	
	int GetCurMode()	{ return m_CurMode; }
	
	BOOL CanActive();

//	CMiniMapIconImage* GetMiniMapImage(WORD Kind) {	return &m_pIconImage[Kind]; }

	void RefreshMode();
	void ShowQuestMarkIcon( CObject* pTarget, int nKind );

//SW050722 
	void InitNumImage();				// ���� �̹��� ���
	void ReleaseNumImage();				// ���� �̹��� ����
	void SetNumImagePos();				// ���� �̹��� ��� ��ǥ ����	
	void ConvertCipherNum();			// ������ ����
	bool IsNumChanged();				// ���� ��ȭ üũ	// Check Num charnges
//	void DoNumChangeAni();				// ���� ��ȯ �ִ�	// Ticktime control
	void DoAni();
	void DoSequenceAni();
	void JPRender();
	void JPProcess();

	//SW050802 snake_legs
/*	void SetParticlePos();
	void CalcMotion();*/
	
	
	void InitForAni();
	void InitForSequenceAni();
	void JPInit();
};