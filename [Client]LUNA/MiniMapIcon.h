#pragma once


enum eQUEST_MARK
{
	eQM_NONE,
	eQM_EXCLAMATION,
	eQM_QUESTION,
	eQM_MAX,
};


enum
{
	eMINIMAPMODE_SMALL,
	eMINIMAPMODE_FULL,
	eMINIMAPMODE_MAX,
};


enum eMINIMAP_ICON
{
	eMNMICON_SELF,
	eMNMICON_PARTYMEMBER,
	eMNMICON_FAMILYMEMBER,
	eMNMICON_NORMALNPC,
	eMNMICON_LOADINGPOINT,
	eMNMICON_CAMERA,
	eMNMICON_QUEST1,
	eMNMICON_QUEST2,
	eMNMICON_QUEST3,
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	// ..���� �͵��� eBIGMAP_ICON �� �ִµ� �����ϱ� ���ؼ� NULL �� �߰�. �߰� �� �ϸ� ū �̴ϸʿ��� ������ ǥ�� �� ��.
	eMNMICON_NULL01,
	eMNMICON_NULL02,
	eMNMICON_NULL03,
	eMNMICON_NULL04,
	eMNMICON_FARM,
	// E ����ý��� �߰� added by hseos 2007.10.31

	eMNMICON_MAX,
};


enum eBIGMAP_ICON
{
	eBMICON_SELF,
	eBMICON_PARTYMEMBER,
	eBMICON_FAMILYMEMBER,
	eBMICON_STORE,
	eBMICON_TRAINER,
	eBMICON_BANK,
	eBMICON_NOMALNPC,
	eBMICON_LOADINGPOINT,
	eBMICON_LOGIN,
	eBMICON_CAMERA,
	eBMICON_QUEST1,	// ����Ʈ NPC ǥ�� ������ 1~3
	eBMICON_QUEST2,
	eBMICON_QUEST3,
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	eBMICON_FARM,
	// E ����ý��� �߰� added by hseos 2007.10.31

	eBMICON_MAX,
};


class cImage;
// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.10.31
class CSHFarm;
// E ����ý��� �߰� added by hseos 2007.10.31

class CMiniMapIconImage
{
public:
	cImage* m_pImage;
	VECTOR2 m_ImageWH;

public:
	CMiniMapIconImage() :
	m_pImage( 0 ) 
	{};

	~CMiniMapIconImage();
	
	void InitIconImage(cImage* pImage,DWORD w,DWORD h)
	{
		m_pImage = pImage;
		m_ImageWH.x = (float)w;
		m_ImageWH.y = (float)h;
	}
};

class CMiniMapIcon
{
protected:
	CMiniMapIconImage* m_pIconImage;

	//CMiniMapIconImage* m_pMarkExclamation[3];
	//CMiniMapIconImage* m_pMarkQuestion[3];
	typedef std::vector< CMiniMapIconImage* >	MiniMapIconVector;
	MiniMapIconVector							m_pMarkExclamation;
	MiniMapIconVector							m_pMarkQuestion;

	int		m_ShowQuestMark;
	
	CObject* m_pObject;
	VECTOR3 m_ObjectPos;

	VECTOR2 m_ScreenPos;

	int		m_Kind;
	VECTOR3 m_TargetPos;
	int		m_Interpolation;
	char	m_MemberName[MAX_NAME_LENGTH+1];
	
	DWORD	m_dwColor;
	BOOL	m_bAlwaysTooltip;
	
public:
	CMiniMapIcon();
	virtual ~CMiniMapIcon();

	void Init(CMiniMapIconImage* pIconImage, int Kind );
	void Init(CMiniMapIconImage* pIconImage,CObject* pObject, int Kind);
	void Init(CMiniMapIconImage* pIconImage,VECTOR3* pObjectPos, int Kind);
	void Init(CMiniMapIconImage* pIconImage,PARTY_MEMBER* pMember, int Kind);
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.10.31
	void Init(CMiniMapIconImage* pIconImage, CSHFarm* pFarm, int Kind);
	// E ����ý��� �߰� added by hseos 2007.10.31
	virtual void Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight);
	virtual void Render(VECTOR2* pMapStartPixel,RECT* pCullRect);

	void RenderQuestMark(VECTOR2* pMapStartPixel,RECT* pCullRect);
	
	int GetToolTip(int x,int y,VECTOR2* pMapStartPixel,char* pOutText, float* distOut = NULL );

	VECTOR3 GetPosition();
	VECTOR2* GetScreenPos()			{	 return &m_ScreenPos;	}

	void SetRGBColor( DWORD dwColor ) { m_dwColor = dwColor; }
	void SetAlwaysTooltip( BOOL bAlways ) { m_bAlwaysTooltip = bAlways;	}

	void SetObjectPos(DWORD PosX, DWORD PosZ)		{ m_ObjectPos.x = (float)PosX;	m_ObjectPos.z = (float)PosZ;	}
	void SetTargetPos(DWORD PosX, DWORD PosZ)		{ m_TargetPos.x = (float)PosX;	m_TargetPos.z = (float)PosZ;	}

	void SetMarkImage(CMiniMapIconImage* pMarkExclamation, CMiniMapIconImage* pMarkQuestion, SIZE_T size );
	void ShowQuestMark( int nKind ) { m_ShowQuestMark = nKind; }
};