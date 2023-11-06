




#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include ".\questlimitkind_stage.h"											// ����Ʈ ����Ʈ ���� �������� ����� �ҷ��´�.

#include "QuestScriptLoader.h"												// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_															// �ʼ����� ����Ǿ� ������,

#include "Player.h"															// �÷��̾� ����� �ҷ��´�.

#else																		// �ʼ����� ����Ǿ� ���� ������,

#include "ObjectManager.h"													// ������Ʈ �Ŵ��� ����� �ҷ��´�.

#endif //_MAPSERVER_

// ������ �Լ�.
CQuestLimitKind_Stage::CQuestLimitKind_Stage(DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx)
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	byClass    = 0 ;												// Ŭ������ ���� ������ 0���� �����Ѵ�.
	byRacial   = 0 ;												// ������ ���� ������ 0���� �����Ѵ�.
	byJobGrade = 0 ;												// �� �׷��̵带 ���� ������ 0���� �����Ѵ�.
	byJobIndex = 0 ;												// �� �ε����� ���� ������ 0���� �����Ѵ�.

	dwClassIndex = 0 ;												// Ŭ���� �ε����� ���� ������ 0���� �����Ѵ�.

	m_pClassIndex = NULL ;											// Ŭ���� �ε��� �����͸� null ó���� �Ѵ�.

	m_byCount = 0 ;													// ī��Ʈ ������ 0���� �����Ѵ�.
    
	if( m_dwLimitKind == eQuestLimitKind_Stage )					// ����Ʈ ������ ���������� ������,
	{
		GetScriptParam( m_byCount, pTokens ) ;						// ī��Ʈ�� �޴´�.
	}

	if( m_byCount > 0 )												// ī��Ʈ�� 0���� ũ��,
	{
		m_pClassIndex = new DWORD[m_byCount] ;						// Ŭ���� �ε����� ���� �޸𸮸� �Ҵ��Ѵ�.
	}
	else															// �׷��� ������,
	{
		return ;													// ���� ó���� �Ѵ�.
	}

	for( BYTE count = 0 ; count < m_byCount ; ++count )				// ī��Ʈ �� ��ŭ for���� ������.
	{
		GetScriptParam( m_pClassIndex[count], pTokens ) ;			// Ŭ���� �ε����� �޴´�.
	}
}

CQuestLimitKind_Stage::~CQuestLimitKind_Stage(void)					// �Ҹ��� �Լ�.
{
	if( m_pClassIndex )												// Ŭ���� �ε��� ������ ��ȿ���� üũ�Ѵ�.
	{
		delete [] m_pClassIndex ;									// Ŭ���� �ε����� �����Ѵ�.
		m_pClassIndex = NULL ;										// Ŭ���� �ε��� �����͸� NULL ó���� �Ѵ�.
	}
}

// ����Ʈ ����Ʈ üũ �Լ�.(����)
BOOL CQuestLimitKind_Stage::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_													// �� ������ ����Ǿ� �ִٸ�,

	switch(m_dwLimitKind)											// ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Stage :									// ����Ʈ ������ ���������� ���ٸ�,
		{
			CHARACTER_TOTALINFO totalInfo ;							// ����Ʈ ��Ż ������ ���� ����ü�� �����Ѵ�.

			memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;	// ����ü ������ �޸� �� �Ѵ�.

			pPlayer->GetCharacterTotalInfo(&totalInfo) ;			// �÷��̾��� ĳ���� ��Ż ������ �޴´�.

			BYTE byClass    	 = totalInfo.Job[0] ;				// Ŭ���� ���� �޴´�.
			BYTE byRacial   	 = totalInfo.Race+1 ;				// ���� ���� �޴´�.
			BYTE byJobGrade 	 = totalInfo.JobGrade ;				// �� �׷��̵� ���� �޴´�.
			BYTE byJobIndex 	 = 0 ;								// �� �ε����� 0���� �����Ѵ�.
	
			if(byJobGrade == 1)										// �� �׷��̵尡 1�� ���ٸ�,
			{	
				byJobIndex = 1 ;									// �� �ε����� 1�� �����Ѵ�.
			}
			else													// �� �׷��̵尡 1�� �ƴ϶��,
			{
				byJobIndex = totalInfo.Job[byJobGrade-1] ;			// �� �ε����� �޴´�.
			}

			for( BYTE count = 0 ; count < m_byCount ; ++count )		// ī��Ʈ �� ��ŭ for���� ������.
			{
				if( m_pClassIndex[count] <= 1000 ) continue ;		// ī��Ʈ�� �ش��ϴ� Ŭ���� �ε����� 1000���� ������, ���~

				// Ŭ���� �ε����� �����Ѵ�.
				DWORD dwLimitClass	  = m_pClassIndex[count] / 1000 ;
				DWORD dwLimitRacial	  = (m_pClassIndex[count] - (1000*dwLimitClass)) / 100 ;
				DWORD dwLimitGrade	  = (m_pClassIndex[count] - (1000*dwLimitClass) - (100*dwLimitRacial)) / 10 ;
				DWORD dwLimitJobIndex = (m_pClassIndex[count] - (1000*dwLimitClass) - (100*dwLimitRacial) - (10*dwLimitGrade)) ;

				// �������� ������ �����ϸ�,
				if( (dwLimitClass == byClass) && (dwLimitRacial == byRacial) && (dwLimitGrade == byJobGrade) && (dwLimitJobIndex == byJobIndex) )
				{
					return TRUE ;									// TRUE�� �����Ѵ�.
				}
			}

			return FALSE ;											// FALSE�� �����Ѵ�.
		}
		break ;
	}
#endif // _MAPSERVER_

	return FALSE ;													// FALSE�� �����Ѵ�.
}

// ����Ʈ ����Ʈ üũ �Լ�.( Ŭ���̾�Ʈ )
BOOL CQuestLimitKind_Stage::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
#ifndef _MAPSERVER_													// �� ������ ����Ǿ� ���� �ʴٸ�,

	switch(m_dwLimitKind)											// ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Stage :									// ����Ʈ ������ ���������� ������,
		{
			CHARACTER_TOTALINFO totalInfo ;							// ĳ���� ��Ż ������ ���� ����ü�� �����Ѵ�.

			memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;	// ĳ���� ��Ż ������ ���� ����ü�� �޸� ���Ѵ�.

			HERO->GetCharacterTotalInfo(&totalInfo) ;				// HERO�� ĳ���� ��Ż ������ �޴´�.

			BYTE byClass    	 = totalInfo.Job[0] ;				// Ŭ���� ���� �޴´�.
			BYTE byRacial   	 = totalInfo.Race+1 ;				// ���� ���� �޴´�.
			BYTE byJobGrade 	 = totalInfo.JobGrade ;				// �� �׷��̵� ���� �޴´�.
			BYTE byJobIndex 	 = 0 ;								// �� �ε����� 0���� �����Ѵ�.

			if(byJobGrade == 1)										// �� �׷��̵尡 1�� ������,
			{
				byJobIndex = 1 ;									// �� �ε����� 1�� �����Ѵ�.
			}
			else													// �� �׷��̵尡 1�� ���� ������,
			{
				byJobIndex = totalInfo.Job[byJobGrade-1] ;			// �� �ε����� �޴´�.
			}

			for( BYTE count = 0 ; count < m_byCount ; ++count )		// ī��Ʈ �� ��ŭ for���� ������.
			{
				if( m_pClassIndex[count] <= 1000 ) continue ;		// Ŭ���� �ε����� 1000 ���϶��,

				// Ŭ���� �ε����� �����Ѵ�.
				DWORD dwLimitClass	  = m_pClassIndex[count] / 1000 ;
				DWORD dwLimitRacial	  = (m_pClassIndex[count] - (1000*dwLimitClass)) / 100 ;
				DWORD dwLimitGrade	  = (m_pClassIndex[count] - (1000*dwLimitClass) - (100*dwLimitRacial)) / 10 ;
				DWORD dwLimitJobIndex = (m_pClassIndex[count] - (1000*dwLimitClass) - (100*dwLimitRacial) - (10*dwLimitGrade)) ;

				// �������� ������ �����ϸ�,
				if( (dwLimitClass == byClass) && (dwLimitRacial == byRacial) && (dwLimitGrade == byJobGrade) && (dwLimitJobIndex == byJobIndex) )
				{
					return TRUE ;									// TRUE�� �����Ѵ�.
				}
			}

			return FALSE ;											// FALSE�� �����Ѵ�.
		}
		break ;
	}
#endif//_MAPSERVER_
	return FALSE ;													// FALSE�� �����Ѵ�.
}
