/*********************************************************************

	 ����		: SHFamilyManager.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/03

	 ���ϼ���	: CSHFamilyManager Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "SHFamily.h"

#if defined(_AGENTSERVER)
	#include "Network.h"
#elif defined(_MAPSERVER_)
#else
	#include "../FilteringTable.h"
	#include "../Player.h"
	#include "../ChatManager.h"
	#include "../GameIn.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFamilyMember
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFamilyMember Method																										  ������
//
CSHFamilyMember::CSHFamilyMember() : CSHGroupMember()
{
	ZeroMemory(&m_stInfoEx, sizeof(m_stInfoEx));
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFamilyMember Method																										  �ı���
//
CSHFamilyMember::~CSHFamilyMember()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFamily
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFamily Method																												  ������
//
CSHFamily::CSHFamily() : CSHGroup()
{
	ZeroMemory(&m_stInfoEx, sizeof(m_stInfoEx));
	m_nEmblemChangedNum = 0;
	m_nMemberNumMax = MAX_MEMBER_NUM;
	m_pcsMember = new CSHFamilyMember[MAX_MEMBER_NUM];
}

CSHFamily::CSHFamily(DWORD nMasterID, char* pszName) : CSHGroup(nMasterID, pszName)
{
	ZeroMemory(&m_stInfoEx, sizeof(m_stInfoEx));
	m_nMemberNumMax = MAX_MEMBER_NUM;
	m_pcsMember = new CSHFamilyMember[MAX_MEMBER_NUM];
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFamily Method																											  �ı���
//
CSHFamily::~CSHFamily()
{
	SAFE_DELETE_ARRAY(m_pcsMember);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// DelMember Method																											   ��� ����
//
VOID CSHFamily::DelMember(DWORD nMemberID)
{
	BOOL bSort = FALSE;
	for(UINT i=0; i<m_stInfo.nMemberNum; i++)
	{
		if (nMemberID == m_pcsMember[i].Get()->nID)
		{
			// ��� �� ����
			m_stInfo.nMemberNum--;
			bSort = TRUE;
		}

		// ����
		if (i<m_stInfo.nMemberNum && bSort)
		{
			SetMember(GetMember(i+1), i);
		}
	}

	if (bSort)
	{
		// ����
		CSHFamilyMember csMember;
		SetMember(&csMember, m_stInfo.nMemberNum);
	}
}

