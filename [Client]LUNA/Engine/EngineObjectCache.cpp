//
// Engine Object Cache
//
//			trustpak
//	



#include "StdAfx.h"
#include "EngineObject.h"
#include "EngineObjectCache.h"



/// Global variable //////////////////////////////////////////////////////////

CEngineObjectCache* g_pEngineObjectCache = NULL;



//////////////////////////////////////////////////////////////////////////////
// Name : CEngineObjectCache
// Desc :
//----------------------------------------------------------------------------
// Name : CEngineObjectCache::CEngineObjectCache
// Desc :
//----------------------------------------------------------------------------
CEngineObjectCache::CEngineObjectCache()
{
	m_dwCacheSize = 0;
	m_pExecutive = NULL;
}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::CEngineObjectCache
// Desc :
//----------------------------------------------------------------------------
CEngineObjectCache::~CEngineObjectCache()
{
}


//----------------------------------------------------------------------------
// Name : CEngineObjectCache::Init
// Desc :
//----------------------------------------------------------------------------
BOOL CEngineObjectCache::Init(I4DyuchiGXExecutive* pExecutive, DWORD dwCacheSize)
{
	m_pExecutive = pExecutive;
	m_dwCacheSize = dwCacheSize;	
	m_hashGXObject.Initialize(dwCacheSize * 4);

	m_pContainDataPool = new CACHE_CONTAIN_DATA[dwCacheSize];
	memset(m_pContainDataPool, 0, sizeof(CACHE_CONTAIN_DATA) * dwCacheSize);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::Release
// Desc :
//----------------------------------------------------------------------------
BOOL CEngineObjectCache::Release(VOID)
{	
	GXOBJECT_HANDLE*	pgxo = NULL;
	CACHE_CONTAIN_DATA* pCacheContainData = NULL;

	// ������ ���α׷��� ����� �� ��� �������� ���������� ó�����ش�.
	
	while ( pCacheContainData = (CACHE_CONTAIN_DATA*)m_lstGXObjects.RemoveTail() )
	{
		if (NULL != pCacheContainData->gxo &&
			TRUE == pCacheContainData->bUsage)
		{
			m_pExecutive->DeleteGXObject(pCacheContainData->gxo);
		}
	}

	m_hashGXObject.RemoveAll();

	if (NULL != m_pContainDataPool)
	{
		delete[] m_pContainDataPool;
		m_pContainDataPool = NULL;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::CreateGXObjectThroughCache
// Desc :
//----------------------------------------------------------------------------
GXOBJECT_HANDLE CEngineObjectCache::CreateGXObjectThroughCache(char* pszFileName, GXSchedulePROC pProc, void* pData,DWORD dwFlag)
{
//	char szDebugString[1024];
//	memset(szDebugString, 0, 1024);

	GXOBJECT_HANDLE gxo = NULL;
	GXOBJECT_HANDLE gxoCache = NULL;

	CACHE_CONTAIN_DATA* pCacheContainData = NULL;

	
	DWORD dwHashKey = CreateCacheHashKey(pszFileName, strlen(pszFileName));

	if (NULL == m_hashGXObject.GetData(dwHashKey))
	{
		// ĳ�������� 20% �̸��� �Ǹ� ����. �� �׼��� ���Ŀ� ��׶��忡�� �ð� üũ�� �����ϵ��� ��������.
		if (CEngineObjectCache::MAX_ENGINE_OBJECT_CACHE_SIZE - ( CEngineObjectCache::MAX_ENGINE_OBJECT_CACHE_SIZE / 5 ) <= m_hashGXObject.GetDataNum())			
		{
			// 20% ������ �߰� Ȯ���Ѵ�. Release�� �ʹ� ���� �Ͼ�� �ٶ������� ���ϸ� ��ġ�� ����.
			DeleteOldCacheDatas(( CEngineObjectCache::MAX_ENGINE_OBJECT_CACHE_SIZE / 5 ) );			
			
			///			
//			sprintf(szDebugString, "--> delete from Cache \n");
//			OutputDebugString(szDebugString);
			///
		}

		gxoCache = m_pExecutive->CreateGXObject(pszFileName, NULL, NULL, 0);		
		
		if (NULL != gxoCache)
		{		
			m_pExecutive->DisableSchedule(gxoCache);
			m_pExecutive->DisableRender(gxoCache);

			pCacheContainData = AllocContainData();
			if (NULL != pCacheContainData)
			{
				pCacheContainData->gxo = gxoCache;
				pCacheContainData->dwKey = dwHashKey;
			}

			m_lstGXObjects.AddTail((void*)pCacheContainData);
			m_hashGXObject.Add(&gxoCache, dwHashKey);		
		}



		///
		/*
		sprintf(szDebugString, "--> insert to Cache : %d, %s\n", dwHashKey, pszFileName );
		OutputDebugString(szDebugString);
		*/
		///
	}
	else
	{
		///
		/*
		sprintf(szDebugString, "--> Cache Hit! : %d, %s\n", dwHashKey, pszFileName );
		OutputDebugString(szDebugString);
		*/
		///
	}

	gxo	= m_pExecutive->CreateGXObject(pszFileName, pProc, pData, dwFlag);

	return gxo;
}


//----------------------------------------------------------------------------
// Name : CEngineObjectCache::CreateCacheHashKey
// Desc :
//----------------------------------------------------------------------------
DWORD CEngineObjectCache::CreateCacheHashKey(char* pszKeyData, int iKeyDataLength)
{
	DWORD dwKey = 0;

	int i = 0;
	for (i = 0; i < iKeyDataLength; i++)
	{
		dwKey += (DWORD)pszKeyData[i];
	}

	dwKey = dwKey % m_dwCacheSize;

	return dwKey;
}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::DeleteOldCacheDatas
// Desc :
//----------------------------------------------------------------------------
void CEngineObjectCache::DeleteOldCacheDatas(DWORD dwDeleteNumber)
{	
	int iLoopLimit = dwDeleteNumber;	
	
	CACHE_CONTAIN_DATA* pCacheContainData = NULL;
	GXOBJECT_HANDLE*	pgxo = NULL;	
	

	if (iLoopLimit > m_lstGXObjects.GetCount())
	{
		iLoopLimit = m_lstGXObjects.GetCount();
	}


	int i = 0;
	for (i = 0; i < iLoopLimit; i++)
	{
		pCacheContainData = (CACHE_CONTAIN_DATA*)m_lstGXObjects.RemoveHead();

		if (NULL != pCacheContainData )
		{
			AddGarbageObject(pCacheContainData->gxo);
			m_hashGXObject.Remove(pCacheContainData->dwKey);		
		}		

		FreeContainData(pCacheContainData);
	}
}


//----------------------------------------------------------------------------
// Name : CEngineObjectCache::FindEmptyContainData
// Desc :
//----------------------------------------------------------------------------
void CEngineObjectCache::DeleteOneOldCacheData(void)
{
	CACHE_CONTAIN_DATA* pCacheContainData = NULL;
	GXOBJECT_HANDLE*	pgxo = NULL;	

	if (0 < m_lstGXObjects.GetCount())
	{
		pCacheContainData = (CACHE_CONTAIN_DATA*)m_lstGXObjects.RemoveHead();
		if (NULL != pCacheContainData )
		{
			AddGarbageObject(pCacheContainData->gxo);
			m_hashGXObject.Remove(pCacheContainData->dwKey);		
		}		

		FreeContainData(pCacheContainData);
	}	

}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::FindEmptyContainData
// Desc :
//----------------------------------------------------------------------------
CACHE_CONTAIN_DATA*	CEngineObjectCache::AllocContainData(void)
{
	DWORD i = 0;
	for (i = 0; i < m_dwCacheSize; i++)
	{
		if (FALSE == m_pContainDataPool[i].bUsage)
		{
			m_pContainDataPool[i].bUsage = TRUE;

			return &m_pContainDataPool[i];
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
// Name : CEngineObjectCache::FindEmptyContainData
// Desc :
//----------------------------------------------------------------------------
VOID CEngineObjectCache::FreeContainData(CACHE_CONTAIN_DATA* pContainData)
{
	if (NULL != pContainData)
	{
		pContainData->bUsage = FALSE;
		pContainData->dwKey = 0;
		pContainData->gxo = NULL;
	}
}


//----------------------------------------------------------------------------
// Name : CEngineObjectCache::ProcessCacheDelete(void)
// Desc :
//----------------------------------------------------------------------------

#define CASH_DELETE_TIME 300000 

VOID CEngineObjectCache::ProcessCacheDelete(void)
{
	DWORD dwCurrentTick = GetTickCount();

	if (dwCurrentTick > m_dwLastDeleteTick + CASH_DELETE_TIME)
	{
		// �ð��� �Ǹ� �Ѱ���, �Ѱ��� �����. 
		DeleteOneOldCacheData();
		
		OutputDebugString("\n--> Cache delete one item. \n\n");		

		m_dwLastDeleteTick = dwCurrentTick;
	}
}

//----------------------------------------------------------------------------