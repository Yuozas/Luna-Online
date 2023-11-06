// StatusIconDlg.h: interface for the CStatusIconDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)
#define AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cImage.h"
#include "./Interface/cMultiLineText.h"
class CObject;
class CMouse;
#define STATUSICONDLG USINGTON(CStatusIconDlg)



struct ICONRENDERINFO
{
	DWORD	ItemIndex;
	BOOL	bPlus;
	BOOL	bAlpha;
	int		Alpha;
};

struct StatusInfo
{
	cImage* Image;
	DWORD	Index;
	DWORD	Time;
	DWORD	ID;
	WORD	Count;
	float	fAlpha;
};

class CStatusIconDlg  
{
	int m_MaxDesc;
	StaticString* m_pDescriptionArray;

	WORD m_IconCount[eStatusIcon_Max];
	cImage m_StatusIcon[eStatusIcon_Max];
	ICONRENDERINFO m_IconInfo[eStatusIcon_Max];
	
	DWORD m_dwRemainTime[eStatusIcon_Max];
	DWORD m_dwStartTime[eStatusIcon_Max];

	int m_MaxIconPerLine;		// ���ٿ� ������ �?

	cMultiLineText m_toolTip;
	int m_CurIconNum;
//	LONG m_ttX, m_ttY;

//	BOOL m_bInit;

	CYHHashTable< StatusInfo > mStatusInfoTable;
	int		m_nQuestIconCount;
	VECTOR2	Scale;
	
	void LoadDescription();
protected:
	VECTOR2 m_DrawPosition;		// �׸� ��ġ
	CObject* m_pObject;

public:
	CStatusIconDlg();
	virtual ~CStatusIconDlg();

	void AddIcon(CObject* pObject,WORD StatusIconNum,DWORD ItemIdx=0, DWORD dwRemainTime = 0);
	void AddQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveIcon(CObject* pObject,WORD StatusIconNum, DWORD ItemIdx=0);
	void RemoveQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveAllQuestTimeIcon();

	void AddSkillIcon( DWORD ID, DWORD Index, DWORD time, WORD count );
	void RemoveSkillIcon( DWORD ID, DWORD Index );
	void UpdateSkillTime( DWORD ID, DWORD Index, DWORD time );
	void UpdateSkillCount( DWORD ID, DWORD Index, WORD count );

	virtual void Render();

	void Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine);
	void Release();

	void SetOneMinuteToShopItem(DWORD ItemIdx);

	void AddQuestIconCount()	{ ++m_nQuestIconCount; }

//	void ActionEvent(CMouse * mouseInfo);
//	BOOL IsInited() { return m_bInit; }
//	void SetToolTip( char* msg, DWORD color, cImage * image, DWORD imgColor );

	// 080318 LUJ, ��Ƽ�� ���� ��ų ǥ������ ������Ʈ ����
	inline void SetObject( CObject* object ) { m_pObject = object; }

	// 08318 LUJ, ���� ǥ�� ���¸� ���´�
	inline const CObject* GetObject() const { return m_pObject; };

	// 080318 LUJ, ���� ǥ�� ���� ��ų�� �ڷᱸ���� �����Ѵ�
	void Copy( std::list< StatusInfo >& );

	// 080318 LUJ, ����â ��ġ ����
	inline void SetPosition( const VECTOR2& position ) { m_DrawPosition = position; }

	// 080318 LUJ, ������ ũ�� ����
	void SetIconSize( float width, float height );	
private:
	VECTOR2	mIconSize;
	

	// 080318 LUJ, �ִ� ������ ǥ�� ����
public:
	inline void SetMaxIconQuantity( DWORD quantity ) { mMaxIconQuantity = quantity; }	
private:
	DWORD	mMaxIconQuantity;

	// 080318 LUJ, �ִ� ǥ�� ������ �ʰ��� �������� ���⿡ �����صξ��ٰ�, ������ ������ ǥ�õȴ�
	typedef std::list< StatusInfo* >	ReservedIconList;
	ReservedIconList					mReservedIconList;

	// 080318 LUJ, ��� �ð� ǥ��
	DWORD mElaspedTime;

	// 080318 LUJ, ��ų�� ǥ���ϰ� �ִ� Ÿ�� �÷��̾� �ε����� ��ȯ�Ѵ�
	DWORD GetPlayerIndex();	
};

EXTERNGLOBALTON(CStatusIconDlg)

#endif // !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)
