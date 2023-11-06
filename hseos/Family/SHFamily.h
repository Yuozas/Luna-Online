/*********************************************************************

	 ����		: SHGroup.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/10

	 ���ϼ���	: CSHGroup Ŭ������ ���

 *********************************************************************/

#pragma once

#include "../Group/SHGroup.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFamilyMember
//
class CSHFamilyMember : public CSHGroupMember 
{
public:
	//----------------------------------------------------------------------------------------------------------------
	struct stINFO_EX																			// ��� ����
	{
		char			szGuild[MAX_NAME_LENGTH+1];												// ..���
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	stINFO_EX			m_stInfoEx;																// �߰� ����

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFamilyMember();
	~CSHFamilyMember();

	//----------------------------------------------------------------------------------------------------------------
	//				���� ����
	void			Set(stINFO* pstInfo)							{ CSHGroupMember::Set(pstInfo); }
	void			Set(stINFO* pstInfo, stINFO_EX* pstInfoEx)		{ CSHGroupMember::Set(pstInfo); m_stInfoEx = *pstInfoEx; }
	void			SetEx(stINFO_EX* pstInfoEx)						{ m_stInfoEx = *pstInfoEx; }
	//				�߰� ���� ���
	stINFO_EX*		GetEx()											{ return &m_stInfoEx; }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFamily
//
class CSHFamily : public CSHGroup
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static const int	MAX_MEMBER_NUM	=	5;													// �ִ� �ɹ� ��

	struct stINFO_EX																			// �йи� ����
	{
		DWORD			nHonorPoint;															// ..������Ʈ
		DWORD			nHonorPointTimeTick;													// ..������Ʈ �ð� ƽ
		int				nNicknameON;															// ..ȣĪON
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	stINFO_EX			m_stInfoEx;																// �߰� ����
	CSHFamilyMember*	m_pcsMember;

	DWORD				m_nEmblemChangedNum;													// ���� ���� ȸ��

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFamily();
	CSHFamily(DWORD nMasterID, char* pszName);
	~CSHFamily();

    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//					���� ����
	VOID				Set(CSHFamily* pcsFamily)
						{
							Set(pcsFamily->Get(), pcsFamily->GetEx());
							for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
							{
								m_pcsMember[i].Set(pcsFamily->GetMember(i)->Get(), pcsFamily->GetMember(i)->GetEx());
							}
							m_nEmblemChangedNum = pcsFamily->GetEmblemChangedNum();
						}

	VOID				Set(stINFO* pstInfo)										{ CSHGroup::Set(pstInfo); }
	VOID				Set(stINFO* pstInfo, stINFO_EX* pstInfoEx)					{ CSHGroup::Set(pstInfo); m_stInfoEx = *pstInfoEx; }
	VOID				SetEx(stINFO_EX* pstInfoEx)									{ m_stInfoEx = *pstInfoEx; }
	stINFO_EX*			GetEx()														{ return &m_stInfoEx; }

	//					��� �߰�
	VOID				AddMember(CSHFamilyMember* pcsMember)
						{
							m_pcsMember[m_stInfo.nMemberNum].Set(pcsMember->Get(), pcsMember->GetEx()); 
							m_stInfo.nMemberNum++; 
						}
	//					��� ����
	VOID				DelMember(DWORD nMemberID);
	//					��� ����
	VOID				SetMember(CSHFamilyMember* pcsMember, int nIndex)			{ m_pcsMember[nIndex].Set(pcsMember->Get(), pcsMember->GetEx()); }
	//					��� ���
	CSHFamilyMember*	GetMember(int nIndex)										{ return &m_pcsMember[nIndex]; }
	CSHFamilyMember*	GetMemberFromID(DWORD nID)
						{
							for(UINT i=0; i<Get()->nMemberNum; i++)
							{
								if (m_pcsMember[i].Get()->nID == nID)
								{
									return &m_pcsMember[i];
								}
							}
							return NULL;
						}

	//					���� ���� ȸ�� ���
	VOID				IncreaseEmblemChangedNum()									{ m_nEmblemChangedNum++; }
	//					���� ���� ȸ�� ���
	DWORD				GetEmblemChangedNum()										{ return m_nEmblemChangedNum; }

	//----------------------------------------------------------------------------------------------------------------
	//					���̺����� �ε��� ���
	int					GetIndexAtTbl()												{ return m_nIndexAtTbl; }
};