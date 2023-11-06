/*********************************************************************

	 ����		: SHGroup.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/10

	 ���ϼ���	: CSHGroup Ŭ������ ���

 *********************************************************************/

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHGroupMember
//
class CSHGroupMember
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum MEMBER_CONSTATE																		// ��� ���� ����
	{
		MEMBER_CONSTATE_LOGOFF = 0,																// ..���� �� ��
		MEMBER_CONSTATE_LOGIN,																	// ..���� ��
	};

	struct stINFO																				// ��� ����
	{
		DWORD			nID;																	// ..ID
		int				nRace;																	// ..����
		int				nSex;																	// ..����
		int				nJobFirst;																// ..����(ó��)
		int				nJobCur;																// ..����(����)
		int				nJobGrade;																// ..�������
		int				nLV;																	// ..����
		MEMBER_CONSTATE	eConState;																// ..���� ����
		char			szNickname[MAX_NAME_LENGTH+1];											// ..ȣĪ
		char			szName[MAX_NAME_LENGTH+1];												// ..�̸�
	};

protected:
	//----------------------------------------------------------------------------------------------------------------
	stINFO				m_stInfo;																// ����

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHGroupMember();
	~CSHGroupMember();

	//----------------------------------------------------------------------------------------------------------------
	//				���� ����
	void			Set(stINFO* pstInfo)							{ m_stInfo = *pstInfo; }
	//				���� ���
	stINFO*			Get()											{ return &m_stInfo; }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHGroup
//
class CSHGroup
{
public:
	//----------------------------------------------------------------------------------------------------------------
	struct stINFO																				// �׷� ����
	{
		DWORD					nID;															// ..ID
		char					szName[MAX_NAME_LENGTH+1];										// ..�̸�
		DWORD					nMasterID;														// ..������ID
		char					szMasterName[MAX_NAME_LENGTH+1];								// ..������ �̸�
		UINT					nMemberNum;														// ..��� ��
	};

protected:
	//----------------------------------------------------------------------------------------------------------------
	stINFO						m_stInfo;														// ����
	CSHGroupMember*				m_pcsMember;													// ���
	UINT						m_nMemberNumMax;												// �ִ� ��� ��

	//----------------------------------------------------------------------------------------------------------------
	UINT						m_nIndexAtTbl;													// ���̺����� �ε���

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHGroup();
	CSHGroup(DWORD nMasterID, char* pszName);
	~CSHGroup();

    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//					���� ����
	VOID				Set(stINFO* pstInfo)	{ m_stInfo = *pstInfo; }
	//					���� ���
	stINFO*				Get()					{ return &m_stInfo; }

	//					��� �߰�
	VOID				AddMember(CSHGroupMember* pcsMember)		
						{
							m_pcsMember[m_stInfo.nMemberNum].Set(pcsMember->Get()); 
							m_stInfo.nMemberNum++; 
						}
	//					��� ����
	VOID				SetMember(CSHGroupMember* pcsMember, int nIndex)	{ m_pcsMember[nIndex].Set(pcsMember->Get()); }
	//					��� ���
	CSHGroupMember*		GetMember(int nIndex)								{ return &m_pcsMember[nIndex]; }
	UINT				GetMemberNumMax()									{ return m_nMemberNumMax; }

	//----------------------------------------------------------------------------------------------------------------
	//					���̺����� �ε��� ����
	VOID				SetIndexAtTbl(int nIndex)							{ m_nIndexAtTbl = nIndex; }
	//					���̺����� �ε��� ���
	UINT				GetIndexAtTbl()										{ return m_nIndexAtTbl; }
};