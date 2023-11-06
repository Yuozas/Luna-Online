#pragma once


#define AUTONOTEMGR USINGTON(CAutoNoteManager)

class CPlayer;

class CAutoNoteManager
{
protected:

	DWORD	m_dwColorTable[4];
	DWORD	m_dwAskColor[4];

	DWORD	m_dwAnswerKey;

public:
	CAutoNoteManager(void);
	~CAutoNoteManager(void);

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void ToggleAutoNote();

//---���� ������ ����
	void AskToAutoUser( DWORD dwAutoPlayerIdx, DWORD dwQuestion );

//---���� �亯�� ����
	void AnswerToQuestion( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 );
	DWORD GetAnswerKey() { return m_dwAnswerKey; }

};

EXTERNGLOBALTON(CAutoNoteManager)