#include "stdafx.h"
#include ".\datematchingmanager.h"
#include "UserTable.h"
#include <crtdbg.h>
#include "Network.h"

extern DWORD g_dwMaxUser;			// AgentNetworkMsgParser 에서 정의.


DWORD	CreateHashKey( char* szBuf, DWORD dwBufSize);

GLOBALTON(CDateMatchingManager)

CDateMatchingManager::CDateMatchingManager(void)
{
	_asm nop;
//	m_dwMaxOutput		=	0;
//	m_pOutput			=	0;

	m_Hash.Initialize(g_dwMaxUser);					// 초기화만 해주면 된다.


}

CDateMatchingManager::~CDateMatchingManager(void)
{
//	m_Hash.
	_asm nop;
}

void CDateMatchingManager::Init(DWORD dwMaxOutput)
{
	if( dwMaxOutput == 0 || dwMaxOutput > 0x000f0000)		// 대충 이쯤이면 문제가 있는 숫자가 들어왔단 말이지.?
		_asm int 3;

//	m_dwMaxOutput	=	dwMaxOutput;

	//m_pOutput	=	new	DATE_MATCHING_SEARCH_OUTPUT[m_dwMaxOutput];
	//memset( m_pOutput, 0, sizeof( DATE_MATCHING_SEARCH_OUTPUT)*m_dwMaxOutput);
}

void CDateMatchingManager::Release(void)
{
	// 출력 버퍼 날리고,
	//delete[]			m_pOutput;
	//m_pOutput			=	0;
	//m_dwMaxOutput		=	0;

	// 해시 날리고.
	m_Hash.RemoveAll();

	// 리스트 날린다.
	int	iMale	=	m_MaleList.GetCount();
	if( iMale)
	{
		_asm nop;			// 원래 이 숫자는 0이어야 한다. 0이 아닐 경우 제대로 사용자 추적이 안되었다는 의미.
	}
	m_MaleList.RemoveAll();				// 남자 유저 리스트.

	int	iFemale	=	m_FemaleList.GetCount();
	if( iFemale)
	{
		_asm nop;
	}
	m_FemaleList.RemoveAll();			// 여자 유저 리스트.

	_asm nop;
}

// 유저 인포를 등록. 클라이언트에서 search를 누르는 순간 호출될듯. 두번 들어갈 수 있으니 안에서 체크.
void CDateMatchingManager::AddUserInfo( USERINFO* pUser)
{
	if( pUser == 0)
		_asm int 3;						// 요건 심각.

	if( pUser->DateMatchingInfo.bActive == TRUE)
	{
		// 두번 들어온 경우. 흔히 있다.
		return	;
	}

	pUser->DateMatchingInfo.bActive		=	TRUE;					// 클라이언트의 데이트 매칭이 활성화 됬다 

	PTRLISTPOS	pos;

	// 성별에 따라 버퍼에 삽입.
	if( pUser->DateMatchingInfo.byGender == GENDER_MALE)			// male
		pos	=	m_MaleList.AddTail( pUser);
	else															// Female
		pos	=	m_FemaleList.AddTail( pUser);

	DWORD	dwHashKey	=	pUser->dwUserID;
	m_Hash.Add( pos, dwHashKey);
}

// 등록된 유저 인포를 해제. 클라이언트에서 DateMatchingDlg를 닫던가, USERINFO가 무효화 되는 시점에서 해제 해줘야 한다.
void CDateMatchingManager::RemoveUserInfo( USERINFO* pUser)
{
	if( pUser == 0)
	{
		_asm int 3;				// 비상 사태.
		return;
	}

	if( pUser->DateMatchingInfo.bActive == FALSE)			// 여기 등록된 적이 없으면 바로 빠지고.
		return;

	PTRLISTPOS pos	=	m_Hash.GetData( pUser->dwUserID);
	if( pos == NULL)
	{
		_asm int 3;				// 역시 비상 사태.
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

// Gender는 GENDER_MALE, GENDER_FEMALE 둘중 하나.? 아웃풋은 어떻게?
void CDateMatchingManager::SearchUser( DWORD* pdwOutCount, DATE_MATCHING_SEARCH_OUTPUT* pOut, DWORD dwMaxOutput, USERINFO* pClient, DWORD dwAgeIndex, DWORD dwRegionIndex)
{
	_CrtCheckMemory();
	DWORD dwGender	=	pClient->DateMatchingInfo.byGender ^ 1;			// xor해서 1이면 0, 0이면 1로 바꿔.

	// output 초기화.
	*pdwOutCount		=	0;
	DWORD	dwLowestScoreIndex	=	-1;

	// 성별에 따라 유저 리스트를 돌면서,
	cPtrList*	pList;
	if( dwGender == GENDER_MALE)
		pList	=	&m_MaleList;
	else
		pList	=	&m_FemaleList;
	PTRLISTPOS pPos	=	pList->GetHeadPosition();
	while( pPos)
	{
		USERINFO* pUser	=	(USERINFO*)pList->GetNext(pPos);
		if( pUser->DateMatchingInfo.bActive == FALSE) // 한번 더 테스트.
			continue;
		if( pUser->DateMatchingInfo.bIsValid == FALSE)
				continue;

		//////////////////////////////////////////////////////////////////////////////////////////
		// 나이 테스트.
		// 일단 AgeIndex는 클라이언트의 CDateMatchingDlg.cpp에 있다. 
		DWORD	dwTargetAge	=	pUser->DateMatchingInfo.dwAge;
		DWORD	dwAgeScore	=	30;			// 일단 Age 30점 주고.
		if( dwAgeIndex != 0)
		{
			DWORD	dwAgeMin;
			DWORD	dwAgeMax;
			if( dwAgeIndex >= 1 && dwAgeIndex <= 7)
			{
				dwAgeMin	=	dwAgeIndex * 5;
				dwAgeMax	=	dwAgeIndex * 5		+	4;
			}
			else if( dwAgeIndex == 8)		// 40대.
			{
				dwAgeMin	=	40;
				dwAgeMax	=	49;
			}
			else if( dwAgeIndex == 9)		// 50대.
			{
				dwAgeMin	=	50;
				dwAgeMax	=	59;
			}
			else
			{
				_asm int 3;				// 예외상황. 어떻게 처리할지 가이드라인이 필요.
				dwAgeMin	=	50;
				dwAgeMax	=	59;
			}

			// 빼고 더하고,
			if( dwTargetAge < dwAgeMin)
					dwAgeScore	-=	( dwAgeMin - dwTargetAge);
			else if( dwTargetAge > dwAgeMax)
					dwAgeScore	-=	( dwTargetAge - dwAgeMax);

			if( dwAgeScore > 30)	// dwAgeScore가 overflow 됬을 때.
					dwAgeScore	=	0;
		}

		// 지역 테스트.
		DWORD	dwRegionScore	=	20;
		if( dwRegionIndex != 0)
			if( pUser->DateMatchingInfo.dwRegion != dwRegionIndex)
					dwRegionScore	-=	10;									// 지역이 다를 경우 10점 깐다.

		// 좋은 성향 테스트.
		DWORD	dwGoodFeelingScore	=	0;
		DWORD*	pdwGoodFeeling	=	pUser->DateMatchingInfo.pdwGoodFeeling;
		for( int i = 0; i < 3; i++)			// 좋아하는 성향 3가지.
		{
			for( int j = 0; j < 3; j++)
			{
				if( pdwGoodFeeling[i] == pClient->DateMatchingInfo.pdwGoodFeeling[j])
						dwGoodFeelingScore	+=	10;
			}
		}
		if( dwGoodFeelingScore > 30)		// 30점 이상은 될 수 없다.
			_asm int 3;

		// 안좋아하는 성향 테스트.
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
		if( dwBadFeelingScore > 20)		// 20점 이상은 될 수 없다.
			_asm int 3;


		// 전체 점수 합은.?
		DWORD	dwScore	=	dwAgeScore + dwRegionScore + dwGoodFeelingScore + dwBadFeelingScore;
		if( dwScore	> 100)
				_asm int 3;			// 오버플로우.	오류 보고 어떻게?

		//////////////////////////////////////////////////////////////////////////////////////////
		// 스코어가 나온 사용자를 output 버퍼에 집어넣거나, 버퍼가 꽉 찼을 경우 점수가 제일 낮은 한놈을 튕겨낸다.
		DWORD	dwReplaceTarget	=	-1;
		if( *pdwOutCount == dwMaxOutput)		// 버퍼가 꽉 찼으면,
		{
			// 일단 for 루프 돌면서 날릴놈 찾아낸다. -_-; 뾰족한 수가 없네.
			DWORD	dwLowestScore	=	110;
			for( DWORD i = 0; i < dwMaxOutput; i++)
			{
				if( pOut[i].dwScore < dwLowestScore)		// 이놈이 점수 제일 낮은놈이구나~
				{
					dwLowestScore	=	pOut[i].dwScore;			// 일단 가장 낮은 점수 갱신하고,
					if( dwLowestScore < dwScore)						// 현재놈 스코어가 가장 낮은놈보다 더 높으면 갱신.
					{
						dwReplaceTarget	=	i;						// 가장 점수가 낮은놈 후보.
					}
				}
			}

		}
		else		// 새로 채워가는 중이라면,
		{
			dwReplaceTarget	=	*pdwOutCount;
			(*pdwOutCount)++;									// 버퍼 한칸 더 썼다.
		}

		// output 버퍼 내용 갱신하고,
		if( dwReplaceTarget < dwMaxOutput)
		{
			pOut[dwReplaceTarget].dwGoodFeelingScore	=	0;					// 호감도 점수.
			pOut[dwReplaceTarget].dwScore				=	dwScore;			// 매칭 점수.
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
	// 해당 이름의 유저를 찾아라.
	USERINFO*	pTargetUser	=	0;
	PTRLISTPOS	pos	=	m_Hash.GetData( dwTargetUserID);		// 일단 해시에서 포인터 리스트를 찾고,
	if( pos == NULL)
	{
		return	;				// 해당 유저가 데이트 매칭창을 닫거나, 로그아웃 하면 없을 수도 있다.
	}

	if( pClient->DateMatchingInfo.byGender == GENDER_MALE)		// 성별 별로, 포인터 리스트에서 유저 정보를 가져온다.
		pTargetUser	=	(USERINFO*)m_FemaleList.GetAt( pos);
	else // GENDER_FEMALE;
		pTargetUser	=	(USERINFO*)m_MaleList.GetAt( pos);
	if( pTargetUser == NULL)
	{
		_asm nop;
		return	;
	}

	MSG_DATEMATCHING_TARGET_INFO	packet;							// 클라이언트로 보낼 결과 패킷.
	packet.Category	=	MP_DATEMATCHING;
	packet.Protocol	=	MP_DATEMATCHING_REQUEST_TARGET_INFO_ACK;
	packet.TargetInfo	=	pTargetUser->DateMatchingInfo;

	g_Network.Send2User( pClient->dwConnectionIndex, (char*)&packet, sizeof(packet));		// 요청한 클라이언트에게 패킷 뿌려라.


	_asm nop;
}

void CDateMatchingManager::OnModifyIntroduce( char* szIntroduce, USERINFO* pUserInfo)
{
	_asm nop;
}

