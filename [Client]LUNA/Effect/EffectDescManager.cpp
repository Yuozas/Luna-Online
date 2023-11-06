// EffectDescManager.cpp: implementation of the CEffectDescManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectDescManager.h"

#include "Effect.h"
#include "EffectDesc.h"
#include "..\MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectDescManager::CEffectDescManager()
{
	m_MaxEffectDesc = 0;
	m_pEffectDescArray = NULL;

}

CEffectDescManager::~CEffectDescManager()
{
	Release();
}

void CEffectDescManager::Release()
{
	if(m_pEffectDescArray)
	{
		delete [] m_pEffectDescArray;
		m_pEffectDescArray = NULL;
	}
}

BOOL CEffectDescManager::LoadEffectList(int EffectKind,CMHFile* pFile, CEffectDescManager* pDefault /*= NULL*/ )
{
	m_MaxEffectDesc = pFile->GetDword();
	m_pEffectDescArray = new CEffectDesc[m_MaxEffectDesc];
	
	DWORD effnum;
	char efffile[256];
	CMHFile EffFile;
	BOOL bSuc;
	for(DWORD n=0;n<m_MaxEffectDesc;++n)
	{
		//ASSERT(!pFile->IsEOF());

		effnum = pFile->GetDword();
//		if( effnum == 17 )
//		{
//			int a=0;
//		}

		
		pFile->GetString(efffile);
		if(strcmp(efffile,"NULL") == 0)
			continue;

		//KES 로딩속도 향상 070717
		if( pDefault )
		{
			CEffectDesc* pDesc = pDefault->GetEffectDesc( effnum );
			if( pDesc )
			{
				char* pDefaultEffect = pDesc->GetFileName();
				if( pDefaultEffect )
				{
					if( strcmp( efffile, pDefaultEffect ) == 0 )	//이펙트 이름이 디폴트와 같으면 로딩안하고 통과
						continue;
				}
			}
		}
		
//		DIRECTORYMGR->SetLoadMode(eLM_EffectScript);

		bSuc = EffFile.Init(efffile,"rb",
			MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR|
			MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
//		DIRECTORYMGR->SetLoadMode(eLM_Root);

		
		if(bSuc == FALSE)
			continue;
		
//		if(effnum == 1321)
//		{
//			int a=0;
//		}
		if(m_pEffectDescArray[effnum].LoadEffectDesc(EffectKind,&EffFile) == FALSE)
		{
			char mes[256];
			sprintf(mes,"Error In Loding '%s' File... plz Check it!!",efffile);
			MessageBox(NULL,mes,0,0);
		}
		
		EffFile.Release();
	}

	return TRUE;
}

BOOL CEffectDescManager::GetEffect(int EffectDescNum,EFFECTPARAM* pParam,CEffect* pOutEffect,BOOL bWarning)
{
	if(EffectDescNum == -1)
		return FALSE;

	if((DWORD)EffectDescNum >= m_MaxEffectDesc)
	{
		ASSERTMSG(0,"연출번호가 이펙트 리스트의 최대갯수보다 큽니다. 확인해주세요");//pjslocal
		return FALSE;
	}

	CEffectDesc& effect = m_pEffectDescArray[EffectDescNum];

	if(	effect.GetMaxEffectUnitDesc()  &&
		effect.GetMaxEffectTriggerDesc() )
	{
		return effect.GetEffect(pParam,pOutEffect);
	}

	return FALSE;	
}


CEffectDesc* CEffectDescManager::GetEffectDesc(int EffectDescNum)
{
	if(EffectDescNum == -1)
		return NULL;

	if((DWORD)EffectDescNum >= m_MaxEffectDesc)
	{
		return NULL;
	}

	if(!(m_pEffectDescArray[EffectDescNum].GetMaxEffectUnitDesc() != 0 &&
		m_pEffectDescArray[EffectDescNum].GetMaxEffectTriggerDesc() != 0))
	{
		return NULL;
	}
	
	return &m_pEffectDescArray[EffectDescNum];
}
