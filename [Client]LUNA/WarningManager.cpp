// WarningManager.cpp: implementation of the CWarningManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WarningManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CWarningManager);
CWarningManager::CWarningManager()
{

}

CWarningManager::~CWarningManager()
{

}

void CWarningManager::CheckWarning(WORD BeforeLife,WORD AfterLife,WORD MaxLife)
{
	BOOL bBeforeWaring = WARNINGMGR->IsWarning(BeforeLife,MaxLife);	
	BOOL bAfterWarning = WARNINGMGR->IsWarning(AfterLife,MaxLife);
	if(bBeforeWaring == FALSE && bAfterWarning == TRUE)
		WARNINGMGR->OnStartWarning();
	if(bBeforeWaring == TRUE && bAfterWarning == FALSE)
		WARNINGMGR->OnEndWarning();
}

BOOL CWarningManager::IsWarning(WORD CurLife,WORD MaxLife)
{
	float RemainRate = CurLife / (float)MaxLife;

	if(RemainRate > 0.5)
		return FALSE;
	else
		return TRUE;
}

void CWarningManager::OnStartWarning()
{
// ���⼭ ���� �ڵ� �Ҹ� �����Ѵ�.

}

void CWarningManager::OnEndWarning()
{
// ���⼭ ���� �ڵ� �Ҹ� ������.

}