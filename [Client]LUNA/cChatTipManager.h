// cChatTipManager.h: interface for the cChatTipManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_)
#define AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHATTIPMGR	USINGTON(cChatTipManager)

//#define	CHATTIPMSG_TIMELEN	180000


struct sChatTip
{
	DWORD	dwIndex;		//�޽��� �ε���
	DWORD	FromLv;			//���� �� ����
	DWORD	ToLv;			//���� �� ����
	char*	pTipStr;		//MSG

	sChatTip():dwIndex(0),FromLv(0),ToLv(0),pTipStr(NULL)
	{}
};

class cChatTipManager  
{
protected:

	BOOL	m_bActive;			//���̻� ��ȿ �޽����� ������ ���۱׸�!
	DWORD	m_dwSize;
	DWORD	m_dwMsgTimeLen;
	DWORD	m_dwLastMsgTime;	//���� �޽��� ��� �ð�
	DWORD	m_dwCurHeroLevel;	//HEROINFO ����ü ��ü
	int		m_nCurIndex;		//���� �޽��� ��ȯ�� ���� ���� �ε��� ���

	sChatTip*	m_spChatTipMsg;	//MSG pool

public:
	void SetActive( BOOL bActive ) { m_bActive = bActive; };
	void SetCurHeroInfo(DWORD Lv) { m_dwCurHeroLevel = Lv; };//�ӽ� �׽�Ʈ��
//	char* GetStringInQuotation(char* buf);	//�ӽ�
	void Init();
	void Release();
	void Process();			
	void LoadChatTipMsg();	//�޽��� �ε�
	char* GetChatTipMsg();	//������ �ð� ���� ������ �޽��� ��ȯ
	bool IsTimePassed();	//��ȿ�ð� �Ǿ��°�

	void ReleaseMsgList();	//m_spChatTipMsg pTipStr ����

	cChatTipManager();
	virtual ~cChatTipManager();

};

EXTERNGLOBALTON(cChatTipManager)
#endif // !defined(AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_)
