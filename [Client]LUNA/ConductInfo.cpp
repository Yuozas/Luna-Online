#include "stdafx.h"
#include ".\conductinfo.h"

cConductInfo::cConductInfo(void)
{
}

cConductInfo::~cConductInfo(void)
{
	// 070420 LYW --- ConductInfo : Release m_pConductInfo.
	if( m_pConductInfo )
	{
		delete m_pConductInfo ;

		m_pConductInfo = NULL ;
	}
}

BOOL cConductInfo::InitConductInfo( CMHFile* pFile )
{
	ASSERT(pFile->IsInited());
	ASSERT(pFile->IsEOF() == FALSE);

	m_pConductInfo = new CONDUCT_INFO;

	m_pConductInfo->ConductIdx = pFile->GetWord() ;

	SafeStrCpy( m_pConductInfo->ConductName, pFile->GetString(), MAX_NAME_LENGTH+1 ) ;
	
	m_pConductInfo->ConductTootipIdx = pFile->GetWord() ;

	m_pConductInfo->ConductKind = pFile->GetWord() ;

	m_pConductInfo->ConductPos = pFile->GetWord() ;

	m_pConductInfo->HighImage = pFile->GetInt() ;

	m_pConductInfo->MotionIdx = pFile->GetInt() ;

	return TRUE;
}

char* cConductInfo::GetConductName()
{
	return m_pConductInfo->ConductName ;
}

WORD cConductInfo::GetConductIdx()
{
	return m_pConductInfo->ConductIdx ;
}

WORD cConductInfo::GetConductTooltipIdx()
{
	return m_pConductInfo->ConductTootipIdx ;
}

WORD cConductInfo::GetConductKind()
{
	return m_pConductInfo->ConductKind ;
}

WORD cConductInfo::GetConductPos()
{
	return m_pConductInfo->ConductPos ;
}

//int cConductInfo::GetLowImage()
//{
//	return m_pConductInfo->LowImage ;
//}

int cConductInfo::GetHighImage()
{
	return m_pConductInfo->HighImage ;
}

int cConductInfo::GetMotionIdx()
{
	return m_pConductInfo->MotionIdx ;
}