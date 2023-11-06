/*********************************************************************

	 ����		: SHFamilyManager.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/03

	 ���ϼ���	: CSHFamilyManager Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "SHGroup.h"

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
// class CSHGroupMember
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHGroupMember Method																										  ������
//
CSHGroupMember::CSHGroupMember()
{
	ZeroMemory(&m_stInfo, sizeof(m_stInfo));
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHGroupMember Method																										  �ı���
//
CSHGroupMember::~CSHGroupMember()
{
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHGroup
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHGroup Method																												  ������
//
CSHGroup::CSHGroup()
{
	ZeroMemory(&m_stInfo, sizeof(m_stInfo));

	m_pcsMember = NULL;
	m_nIndexAtTbl = 0;
}

CSHGroup::CSHGroup(DWORD nMasterID, char* pszName)
{
	ZeroMemory(&m_stInfo, sizeof(m_stInfo));

	m_pcsMember = NULL;
	m_nIndexAtTbl = 0;

	m_stInfo.nMasterID = nMasterID;
	SafeStrCpy(m_stInfo.szName, pszName, MAX_NAME_LENGTH+1);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHGroup Method																												  �ı���
//
CSHGroup::~CSHGroup()
{
}

