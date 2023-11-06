#pragma once


enum eAutoNoteRoomState
{
	eANRS_NONE,
	eANRS_ASKING,
	eANRS_WAITANSWER,
	eANRS_FASTANSWER,
	eANRS_ENDANSWER,
	eANRS_MAX,
};



class CAutoNoteRoom
{
protected:

	DWORD	m_dwAskCharacter;
	DWORD	m_dwAutoCharacter;

	DWORD	m_dwAskUserIdx;
	DWORD	m_dwAutoUserIdx;

	char	m_AutoCharacterName[MAX_NAME_LENGTH+1];

	int		m_nANRState;			//---Room ����
	DWORD	m_dwCreateTime;			//---�� ���� �ð�
	int		m_nChance;


//	DWORD	m_dwKey;				//---Ŭ���̾�Ʈ�� ������ Ű��
//	DWORD	m_dwAnswerKey;			//---Ŭ���̾�Ʈ�� �亯 Ű ��

//	DWORD	m_dwColorTable[4];		//---������ ���� Į�����̺�
//	DWORD	m_dwAskColor[4];		//---���� ����

//---------------------------------------
	DWORD	m_dwAskColor[4];

public:
	CAutoNoteRoom(void);
	~CAutoNoteRoom(void);

	DWORD GetAskCharacterIdx() { return m_dwAskCharacter; }
	DWORD GetAutoCharacterIdx() { return m_dwAutoCharacter; }
	DWORD GetAskUserIdx() { return m_dwAskUserIdx; }
	DWORD GetAutoUserIdx() { return m_dwAutoUserIdx; }

	char* GetAutoCharacterName() { return m_AutoCharacterName; }

	void CreateRoom( CPlayer* pAskPlayer, CPlayer* pAutoPlayer, DWORD dwQuestion );
	void AskToAuto();
	BOOL CheckAnswerFromAuto( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 );

	BOOL IsTimeOver();
	int	GetChance()	{ return m_nChance; }

	void SetAutoNoteRoomState( int nANRState ) { m_nANRState = nANRState; }
	int GetAutoNoteRoomState() { return m_nANRState; }

	DWORD GetAnswerTime() { return gCurTime - m_dwCreateTime; }
	void FastAnswer();
};
