#include "stdafx.h"
#include ".\datematchingmanager.h"
#include "UserTable.h"
#include <crtdbg.h>
#include "Network.h"

extern DWORD g_dwMaxUser;			// AgentNetworkMsgParser ���� ����.


DWORD	CreateHashKey( char* szBuf, DWORD dwBufSize);

GLOBALTON(CDateMatchingManager)

CDateMatchingManager::CDateMatchingManager(void)
{
	_asm nop;
//	m_dwMaxOutput		=	0;
//	m_pOutput			=	0;

	m_Hash.Initialize(g_dwMaxUser);					// �ʱ�ȭ�� ���ָ� �ȴ�.


}

CDateMatchingManager::~CDateMatchingManager(void)
{
//	m_Hash.
	_asm nop;
}

void CDateMatchingManager::Init(DWORD dwMaxOutput)
{
	if( dwMaxOutput == 0 || dwMaxOutput > 0x000f0000)		// ���� �����̸� ������ �ִ� ���ڰ� ���Դ� ������.?
		_asm int 3;

//	m_dwMaxOutput	=	dwMaxOutput;

	//m_pOutput	=	new	DATE_MATCHING_SEARCH_OUTPUT[m_dwMaxOutput];
	//memset( m_pOutput, 0, sizeof( DATE_MATCHING_SEARCH_OUTPUT)*m_dwMaxOutput);
}

void CDateMatchingManager::Release(void)
{
	// ��� ���� ������,
	//delete[]			m_pOutput;
	//m_pOutput			=	0;
	//m_dwMaxOutput		=	0;

	// �ؽ� ������.
	m_Hash.RemoveAll();

	// ����Ʈ ������.
	int	iMale	=	m_MaleList.GetCount();
	if( iMale)
	{
		_asm nop;			// ���� �� ���ڴ� 0�̾�� �Ѵ�. 0�� �ƴ� ��� ����� ����� ������ �ȵǾ��ٴ� �ǹ�.
	}
	m_MaleList.RemoveAll();				// ���� ���� ����Ʈ.

	int	iFemale	=	m_FemaleList.GetCount();
	if( iFemale)
	{
		_asm nop;
	}
	m_FemaleList.RemoveAll();			// ���� ���� ����Ʈ.

	_asm nop;
}

// ���� ������ ���. Ŭ���̾�Ʈ���� search�� ������ ���� ȣ��ɵ�. �ι� �� �� ������ �ȿ��� üũ.
void CDateMatchingManager::AddUserInfo( USERINFO* pUser)
{
	if( pUser == 0)
		_asm int 3;						// ��� �ɰ�.

	if( pUser->DateMatchingInfo.bActive == TRUE)
	{
		// �ι� ���� ���. ���� �ִ�.
		return	;
	}

	pUser->DateMatchingInfo.bActive		=	TRUE;					// Ŭ���̾�Ʈ�� ����Ʈ ��Ī�� Ȱ��ȭ ��� 

	PTRLISTPOS	pos;

	// ������ ���� ���ۿ� ����.
	if( pUser->DateMatchingInfo.byGender == GENDER_MALE)			// male
		pos	=	m_MaleList.AddTail( pUser);
	else															// Female
		pos	=	m_FemaleList.AddTail( pUser);

	DWORD	dwHashKey	=	pUser->dwUserID;
	m_Hash.Add( pos, dwHashKey);
}

// ��ϵ� ���� ������ ����. Ŭ���̾�Ʈ���� DateMatchingDlg�� �ݴ���, USERINFO�� ��ȿȭ �Ǵ� �������� ���� ����� �Ѵ�.
void CDateMatchingManager::RemoveUserInfo( USERINFO* pUser)
{
	if( pUser == 0)
	{
		_asm int 3;				// ��� ����.
		return;
	}

	if( pUser->DateMatchingInfo.bActive == FALSE)			// ���� ��ϵ� ���� ������ �ٷ� ������.
		return;

	PTRLISTPOS pos	=	m_Hash.GetData( pUser->dwUserID);
	if( pos == NULL)
	{
		_asm int 3;				// ���� ��� ����.
		return	;
	}

	m_Hash.Remove( pUser->dwUserID);

	BOOL	bRemove	=	FALSE;
	BOOL	bMale	=	FALSE;
	BOOL	bFemale	=	FALSE;
	if( pUser->DateMatchingInfo.byGender == GENDER_MALE)
	{
		m_MaleList.RemoveAt( pos);
	}
	else
	{
		m_FemaleList.RemoveAt( pos);
	}

	pUser->DateMatchingInfo.bActive	=	FALSE;
}

// Gender�� GENDER_MALE, GENDER_FEMALE ���� �ϳ�.? �ƿ�ǲ�� ���?
void CDateMatchingManager::SearchUser( DWORD* pdwOutCount, DATE_MATCHING_SEARCH_OUTPUT* pOut, DWORD dwMaxOutput, USERINFO* pClient, DWORD dwAgeIndex, DWORD dwRegionIndex)
{
	_CrtCheckMemory();
	DWORD dwGender	=	pClient->DateMatchingInfo.byGender ^ 1;			// xor�ؼ� 1�̸� 0, 0�̸� 1�� �ٲ�.

	// output �ʱ�ȭ.
	*pdwOutCount		=	0;
	DWORD	dwLowestScoreIndex	=	-1;

	// ������ ���� ���� ����Ʈ�� ���鼭,
	cPtrList*	pList;
	if( dwGender == GENDER_MALE)
		pList	=	&m_MaleList;
	else
		pList	=	&m_FemaleList;
	PTRLISTPOS pPos	=	pList->GetHeadPosition();
	while( pPos)
	{
		USERINFO* pUser	=	(USERINFO*)pList->GetNext(pPos);
		if( pUser->DateMatchingInfo.bActive == FALSE) // �ѹ� �� �׽�Ʈ.
			continue;
		if( pUser->DateMatchingInfo.bIsValid == FALSE)
				continue;

		//////////////////////////////////////////////////////////////////////////////////////////
		// ���� �׽�Ʈ.
		// �ϴ� AgeIndex�� Ŭ���̾�Ʈ�� CDateMatchingDlg.cpp�� �ִ�. 
		DWORD	dwTargetAge	=	pUser->DateMatchingInfo.dwAge;
		DWORD	dwAgeScore	=	30;			// �ϴ� Age 30�� �ְ�.
		if( dwAgeIndex != 0)
		{
			DWORD	dwAgeMin;
			DWORD	dwAgeMax;
			if( dwAgeIndex >= 1 && dwAgeIndex <= 7)
			{
				dwAgeMin	=	dwAgeIndex * 5;
				dwAgeMax	=	dwAgeIndex * 5		+	4;
			}
			else if( dwAgeIndex == 8)		// 40��.
			{
				dwAgeMin	=	40;
				dwAgeMax	=	49;
			}
			else if( dwAgeIndex == 9)		// 50��.
			{
				dwAgeMin	=	50;
				dwAgeMax	=	59;
			}
			else
			{
				_asm int 3;				// ���ܻ�Ȳ. ��� ó������ ���̵������ �ʿ�.
				dwAgeMin	=	50;
				dwAgeMax	=	59;
			}

			// ���� ���ϰ�,
			if( dwTargetAge < dwAgeMin)
					dwAgeScore	-=	( dwAgeMin - dwTargetAge);
			else if( dwTargetAge > dwAgeMax)
					dwAgeScore	-=	( dwTargetAge - dwAgeMax);

			if( dwAgeScore > 30)	// dwAgeScore�� overflow ���� ��.
					dwAgeScore	=	0;
		}

		// ���� �׽�Ʈ.
		DWORD	dwRegionScore	=	20;
		if( dwRegionIndex != 0)
			if( pUser->DateMatchingInfo.dwRegion != dwRegionIndex)
					dwRegionScore	-=	10;									// ������ �ٸ� ��� 10�� ���.

		// ���� ���� �׽�Ʈ.
		DWORD	dwGoodFeelingScore	=	0;
		DWORD*	pdwGoodFeeling	=	pUser->DateMatchingInfo.pdwGoodFeeling;
		for( int i = 0; i < 3; i++)			// �����ϴ� ���� 3����.
		{
			for( int j = 0; j < 3; j++)
			{
				if( pdwGoodFeeling[i] == pClient->DateMatchingInfo.pdwGoodFeeling[j])
						dwGoodFeelingScore	+=	10;
			}
		}
		if( dwGoodFeelingScore > 30)		// 30�� �̻��� �� �� ����.
			_asm int 3;

		// �������ϴ� ���� �׽�Ʈ.
		DWORD	dwBadFeelingScore	=	0;
		DWORD*	pdwBadFeeling	=	pUser->DateMatchingInfo.pdwBadFeeling;
		for( int i = 0; i < 2; i++)
		{
			for( int j = 0; j < 2; j++)
			{
				if( pdwBadFeeling[i] == pClient->DateMatchingInfo.pdwBadFeeling[j])
						dwBadFeelingScore	+=	10;
			}
		}
		if( dwBadFeelingScore > 20)		// 20�� �̻��� �� �� ����.
			_asm int 3;


		// ��ü ���� ����.?
		DWORD	dwScore	=	dwAgeScore + dwRegionScore + dwGoodFeelingScore + dwBadFeelingScore;
		if( dwScore	> 100)
				_asm int 3;			// �����÷ο�.	���� ���� ���?

		//////////////////////////////////////////////////////////////////////////////////////////
		// ���ھ ���� ����ڸ� output ���ۿ� ����ְų�, ���۰� �� á�� ��� ������ ���� ���� �ѳ��� ƨ�ܳ���.
		DWORD	dwReplaceTarget	=	-1;
		if( *pdwOutCount == dwMaxOutput)		// ���۰� �� á����,
		{
			// �ϴ� for ���� ���鼭 ������ ã�Ƴ���. -_-; ������ ���� ����.
			DWORD	dwLowestScore	=	110;
			for( DWORD i = 0; i < dwMaxOutput; i++)
			{
				if( pOut[i].dwScore < dwLowestScore)		// �̳��� ���� ���� �������̱���~
				{
					dwLowestScore	=	pOut[i].dwScore;			// �ϴ� ���� ���� ���� �����ϰ�,
					if( dwLowestScore < dwScore)						// ����� ���ھ ���� �����𺸴� �� ������ ����.
					{
						dwReplaceTarget	=	i;						// ���� ������ ������ �ĺ�.
					}
				}
			}

		}
		else		// ���� ä������ ���̶��,
		{
			dwReplaceTarget	=	*pdwOutCount;
			(*pdwOutCount)++;									// ���� ��ĭ �� ���.
		}

		// output ���� ���� �����ϰ�,
		if( dwReplaceTarget < dwMaxOutput)
		{
			pOut[dwReplaceTarget].dwGoodFeelingScore	=	0;					// ȣ���� ����.
			pOut[dwReplaceTarget].dwScore				=	dwScore;			// ��Ī ����.
			pOut[dwReplaceTarget].dwAge					=	pUser->DateMatchingInfo.dwAge;
			pOut[dwReplaceTarget].dwUserID				=	pUser->dwUserID;
			strcpy( pOut[dwReplaceTarget].szName, pUser->DateMatchingInfo.szName);
		}

		_asm nop;

	}

	_asm nop;
}


void CDateMatchingManager::OnRequestTargetInfo( DWORD dwTargetUserID, USERINFO* pClient)
{
	// �ش� �̸��� ������ ã�ƶ�.
	USERINFO*	pTargetUser	=	0;
	PTRLISTPOS	pos	=	m_Hash.GetData( dwTargetUserID);		// �ϴ� �ؽÿ��� ������ ����Ʈ�� ã��,
	if( pos == NULL)
	{
		return	;				// �ش� ������ ����Ʈ ��Īâ�� �ݰų�, �α׾ƿ� �ϸ� ���� ���� �ִ�.
	}

	if( pClient->DateMatchingInfo.byGender == GENDER_MALE)		// ���� ����, ������ ����Ʈ���� ���� ������ �����´�.
		pTargetUser	=	(USERINFO*)m_FemaleList.GetAt( pos);
	else // GENDER_FEMALE;
		pTargetUser	=	(USERINFO*)m_MaleList.GetAt( pos);
	if( pTargetUser == NULL)
	{
		_asm nop;
		return	;
	}

	MSG_DATEMATCHING_TARGET_INFO	packet;							// Ŭ���̾�Ʈ�� ���� ��� ��Ŷ.
	packet.Category	=	MP_DATEMATCHING;
	packet.Protocol	=	MP_DATEMATCHING_REQUEST_TARGET_INFO_ACK;
	packet.TargetInfo	=	pTargetUser->DateMatchingInfo;

	g_Network.Send2User( pClient->dwConnectionIndex, (char*)&packet, sizeof(packet));		// ��û�� Ŭ���̾�Ʈ���� ��Ŷ �ѷ���.


	_asm nop;
}

void CDateMatchingManager::OnModifyIntroduce( char* szIntroduce, USERINFO* pUserInfo)
{
	_asm nop;
}

