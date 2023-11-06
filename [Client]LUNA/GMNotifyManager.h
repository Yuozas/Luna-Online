// GMNotifyManager.h: interface for the CGMNotifyManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_)
#define AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NOTIFYMGR USINGTON(CGMNotifyManager)


#define MAX_NOTIFYMSG_LENGTH			127												// ���� �޽��� ����
#define MAX_NOTIFYMSG_PER_LINE			64												// ���� �� ���� �޽��� ����
#define MAX_NOTIFYMSG_LINE				5												// �ִ� ���� �޽��� ����
#define NOTIFYMSG_LAST_TIME				10000											// ���� �޽��� �ð�

#define NOTIFYMSG_DEFAULT_X				200												// ���� �޽��� �⺻ X��ǥ
#define NOTIFYMSG_DEFAULT_Y				100												// ���� �޽��� �⺻ Y��ǥ
#define NOTIFYMSG_DEFAULT_WIDTH			624												// ���� �޽��� �⺻ ���λ�����
#define NOTIFYMSG_DEFAULT_LINESPACE		10												// ���� �޽��� �⺻ �� ����
#define NOTIFYMSG_DEFAULT_FONT			0												// ���� �޽��� �⺻ ��Ʈ

struct sGMNOTIFY																		// GM���� ����ü
{
	DWORD	dwReceiveTime;																// ���� �ð�
	DWORD	dwColor;																	// ����
	int		nStrLen;																	// ���ڿ� ����
	char	strMsg[MAX_NOTIFYMSG_LENGTH+1];												// ���� �޽��� ���ڿ�
};

enum eNOTIFY_CLASS																		// ���� �޽��� Ÿ��
{
	eNTC_DEFAULT,																		// �⺻ ����
	eNTC_EMERGENCY,																		// ��� ����
	eNTC_REMAINTIME,																	// ����															
	eNTC_TRACKING,																		// Ʈ��ŷ ����?
	eNTC_JACKPOT,																		// ���� ����
	eNTC_COUNT,																			// ī��Ʈ ����
};

const DWORD dwNotifyColor[eNTC_COUNT] = {												// ���� Ÿ�Կ� ���� ����

	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(255, 20, 20, 255),	//eNTC_TRACKING
	RGBA_MAKE(0, 240, 255, 0),		//eNTC_JACKPOT
};

class CGMNotifyManager																	// GM���� �Ŵ��� Ŭ����
{
protected:

	cPtrList	m_ListNotify;															// ���� ����Ʈ
	int			m_nMaxLine;																// �ִ� ����

	RECT		m_rcPos;																// ��� ���� ��Ʈ
	int			m_nLineSpace;															// ���� ����
	int			m_nFontIdx;																// ��Ʈ �ε���

	BOOL		m_bUseEventNotify;														// �̺�Ʈ ���� ��뿩�θ� ��� ����
	char		m_strEventNotifyTitle[32];												// �̺�Ʈ ���� ���� ���ڿ�
	char		m_strEventNotifyContext[128];											// �̺�Ʈ ���� ���� ���ڿ�

	BOOL		m_bNotifyChanged;														// ���� ���� ���θ� ��� ����
	
	BOOL		m_ApplyEventList[eEvent_Max];											// �̺�Ʈ ����Ʈ�� ���뿩�θ� ��� ���� �迭

protected:
	void PushMsg( char* pMsg, DWORD dwColor );											// �޽��� �߰� �Լ�
	void PopMsg();																		// �޽��� ��ȯ �Լ�

public:
	CGMNotifyManager();																	// ���� �Ŵ��� ������ �Լ�
	virtual ~CGMNotifyManager();														// ���� �Ŵ��� �Ҹ��� �Լ�

	void Init( int nMaxLine );															// �ʱ�ȭ �Լ�
	void Release();																		// ���� �Լ�

	void AddMsg( char* pMsg, int nClass = eNTC_DEFAULT );								// �޽��� �߰� �Լ�
	void Render();																		// ���� �Լ�

	void SetPosition( LONG lx, LONG ly, LONG lWidth );									// ��ġ ���� �Լ�
	void SetFont( int nFontIdx )		{ m_nFontIdx = nFontIdx; }						// ��Ʈ ���� �Լ�
	void SetLineSpace( int nLineSpace ) { m_nLineSpace = nLineSpace; }					// ���� ���� ���� �Լ�

	void SetEventNotifyStr( char* pStrTitle, char* pStrContext );						// �̺�Ʈ ���� �޽��� ���� �Լ�
	void SetEventNotify( BOOL bUse );													// �̺�Ʈ ���� ��� ���� ���� �Լ�
	BOOL IsEventNotifyUse()				{ return m_bUseEventNotify; }					// �̺�Ʈ ���� ��� ���θ� ��ȯ�ϴ� �Լ�
	char* GetEventNotifyTitle()			{ return m_strEventNotifyTitle; }				// �̺�Ʈ ���� ������ ��ȯ�ϴ� �Լ�
	char* GetEventNotifyContext()		{ return m_strEventNotifyContext; }				// �̺�Ʈ ���� ������ ��ȯ�ϴ� �Լ�

	void SetEventNotifyChanged( BOOL bChanged ) { m_bNotifyChanged = bChanged; }		// �̺�Ʈ ���� ���� ���θ� �����ϴ� �Լ�
	BOOL IsEventNotifyChanged()					{ return m_bNotifyChanged; }			// �̺�Ʈ ���� ���� ���θ� ��ȯ�ϴ� �Լ�

	void ResetEventApply()					{	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );		}	// �̺�Ʈ ������ ���� ��ħ�ϴ� �Լ�
	void SetEventApply( DWORD EventIdx )	{	m_ApplyEventList[EventIdx] = TRUE;		}	// �̺�Ʈ�� �����ϴ� �Լ�
	BOOL IsApplyEvent( DWORD EventIdx )		{	return m_ApplyEventList[EventIdx];		}	// �̺�Ʈ ���� ���θ� ��ȯ�ϴ� �Լ�

	void LoadGameDesc() ;																// ���� ������ �ε��ϴ� �Լ�
};

EXTERNGLOBALTON(CGMNotifyManager)

#endif // !defined(AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_)
