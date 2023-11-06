// JackpotManager_Agent.h: interface for the JackpotManager_Agent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_)
#define AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	DB_UPDATE_TIMELENGTH	60000

#define JACKPOTMGR	USINGTON(JackpotManager_Agent)

class JackpotManager_Agent  
{
	MONEYTYPE	m_dwTotalMoney;
	DWORD		m_dwUpdateTimeLength;
	DWORD		m_dwLastDBUpdateTime;
	bool		m_bManager;
	

public:

	void	CheckManager();	//���̰����ϴ� Agent����
	void	SetTotalMoney(MONEYTYPE TotalMoney) {m_dwTotalMoney = TotalMoney;}
	void	SetUpdateTimeLength(DWORD TimeLength) {m_dwUpdateTimeLength = TimeLength;}
	void	GetTotalMoneyFrDB();	//DB�� ����...�����ɶ�..�׸��� ����������
	void	SendMsgAgentTotalMoney();
	void	ReceiveMsgAgentTotalMoney();
	void	SendMsgUserTotalMoney();	//�ѱݾ� ������ �������� ������. //!�� ���� �������� Ȯ��
//	void	SendMsgUserPrizeNotify();	//��÷ ������ �������� ������.	//MAP ���� ó��
	void	SendMsgAddUserTotalMoney(DWORD CharacterID);	//�ѱݾ� ������ Distribute_Server ���� �������� ������.

	void	Start();
	void	Process();
	void	Init();
	void	Release();

	JackpotManager_Agent();
	virtual ~JackpotManager_Agent();

};

EXTERNGLOBALTON(JackpotManager_Agent)

#endif // !defined(AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_)


/*
 *	���� �ð����� DB���� TotalMoney�� �޾ƿ� Client ���� ������.
 *	���� ������ ������ TotalMoney�� �����ϰ� Client ���� ������.
 */