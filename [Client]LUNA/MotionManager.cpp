// MotionManager.cpp: implementation of the CMotionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionManager.h"
#include "MOTIONDESC.h"
#include "MHFile.h"
#include "Object.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMotionManager)
CMotionManager::CMotionManager()
{
	m_pMontionHashTable = new CYHHashTable<CMOTIONDESC>;
	m_pMontionHashTable->Initialize(MAX_MOTION_NUM);
}

CMotionManager::~CMotionManager()
{
	if(m_pMontionHashTable)
	{

		CMOTIONDESC * p;
		m_pMontionHashTable->SetPositionHead();
		while(p = m_pMontionHashTable->GetData())
		{
			delete p;
		}
		m_pMontionHashTable->RemoveAll();

		delete m_pMontionHashTable;
		m_pMontionHashTable = NULL;
	}
}

CMOTIONDESC * CMotionManager::GetMotionNDesc(DWORD dwID)
{
	return m_pMontionHashTable->GetData(dwID);
}
void CMotionManager::AddMotionNDesc(CMOTIONDESC * pData, DWORD dwID)
{
	ASSERT(!m_pMontionHashTable->GetData(dwID));
	m_pMontionHashTable->Add(pData, dwID);
}

CMOTIONDESC * CMotionManager::LoadMotionInfo(char * MotionFileName)
{
	CMHFile motionfp;
	DIRECTORYMGR->SetLoadMode(eLM_Resource);
	if( motionfp.Init(MotionFileName, "rb", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT) == FALSE )
	{
		return NULL;
	}
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	DWORD dwID = 0;
	CMOTIONDESC * pDESC = NULL;
	CMOTIONBASE * pBASE = NULL;
	char buff[256];
	while(motionfp.GetString(buff) != EOF)
	{
		if(buff[0] == '@')
		{
			motionfp.GetLineX(buff, 256);
			continue;
		}
		else if(buff[0] == '{' || buff[0] == '}')
		{
			continue;
		}
		CMD_ST(buff)
		CMD_CS("#MOTIONID")
			dwID = motionfp.GetDword();
			pDESC = GetMotionNDesc(dwID);
			if( !pDESC )
			{
				pDESC = new CMOTIONDESC;
				pDESC->SetID(dwID);
			}
			else
			{
				ASSERT(0);
			}
		CMD_CS("#STANDRATE")
			pDESC->SetStandRate(motionfp.GetInt());
		CMD_CS("#STAND")
			pDESC->motionBase[eMonsterMotionN_Stand].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Stand];
		CMD_CS("#WALK")
			pDESC->motionBase[eMonsterMotionN_Walk].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Walk];
		CMD_CS("#RUN")
			pDESC->motionBase[eMonsterMotionN_Run].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Run];
		CMD_CS("#ATTACK")
			pDESC->motionBase[eMonsterMotionN_Attack].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Attack];
		CMD_CS("#DIE")
			pDESC->motionBase[eMonsterMotionN_Die].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Die];
		CMD_CS("#DAMAGE")
			pDESC->motionBase[eMonsterMotionN_Damage].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Damage];
		CMD_CS("#REST")
			pDESC->motionBase[eMonsterMotionN_Rest].InitMotion(motionfp.GetInt());
			pBASE = &pDESC->motionBase[eMonsterMotionN_Rest];
		CMD_CS("#MOTION")
			for(BYTE i = 0 ; i < pBASE->GetNum() ; ++i)
			{
				pBASE->AddMotion(i, motionfp.GetInt());
			}
		CMD_EN
	}

	return pDESC;
}
void CMotionManager::LoadMotionList()
{
	CMHFile listfp;
	DIRECTORYMGR->SetLoadMode(eLM_Resource);
	if( listfp.Init("motionList.bin", "rb", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT) == FALSE )
	{
		return;
	}
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	DWORD totalNum = listfp.GetDword();
	char montionFile[256];
	while(1)
	{
		DWORD tmp = listfp.GetDword();
		if(listfp.IsEOF())
			break;
		listfp.GetString( montionFile );
		
		if( montionFile != "" )
		{
			CMOTIONDESC * pNewDesc = LoadMotionInfo(montionFile);
			AddMotionNDesc(pNewDesc, pNewDesc->GetID());
		}
	}
}


void CMotionManager::SetMotionInfo(CObject * pObject, DWORD dwMontionID)
{
	pObject->m_pMotionDESC = GetMotionNDesc(dwMontionID);
}
