// cResourceManager.cpp: implementation of the cResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cResourceManager.h"
#include "../Engine/GraphicEngine.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cResourceManager);
cResourceManager::cResourceManager()
{
	m_pImageList = NULL;
	m_pImageInfoArray = NULL;
	m_nMaxImageInfoNum = 0;

	// 080320 LYW --- cResourceManager : Initialize class icon part.
 	m_ClassIconList.RemoveAll() ;
 	m_byIconCount = 0 ;
}

cResourceManager::~cResourceManager()
{
	Release();

	// 080320 LYW --- cResourceManager : Release class icon part.
 	ReleaseIconList() ;	
}
void cResourceManager::Init(char * szImagePath, char * szMsgPath)
{
	if(!m_pImageList)
		m_pImageList = new cPtrList;

	strcpy( m_szImageListPath,	szImagePath );
	strcpy( m_szMsgListPath,	szMsgPath );


	// ImageInfo List
	CMHFile fp;
	fp.Init(m_szImageListPath, "rb");
	m_nMaxImageInfoNum = fp.GetInt();
	m_pImageInfoArray = new IMAGE_NODE[ m_nMaxImageInfoNum ];
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		fp.GetInt();
		strcpy( m_pImageInfoArray[i].szFileName, fp.GetString() );
		m_pImageInfoArray[i].size.x			= fp.GetFloat();
		m_pImageInfoArray[i].size.y			= fp.GetFloat();
		m_pImageInfoArray[i].layer			= fp.GetInt();
		m_pImageInfoArray[i].pSpriteObject	= NULL;
	}
	fp.Release();

	// 080320 LYW --- cResourceManager : Ŭ���� �������� �ε��Ѵ�.
 	LoadClassIcon() ;
}
IMAGE_NODE * cResourceManager::GetInfo( int idx )
{	
	if( idx < 0 )
		return NULL;
	if( idx >= m_nMaxImageInfoNum ) 
		return NULL;
	return &m_pImageInfoArray[idx];
}
IDISpriteObject * cResourceManager::GetImageInfo( int idx )
{
	if( idx >= m_nMaxImageInfoNum ) 
		return NULL;

	if( !m_pImageInfoArray[idx].pSpriteObject )
	{
		m_pImageInfoArray[idx].pSpriteObject = g_pExecutive->GetRenderer()->CreateSpriteObject(m_pImageInfoArray[idx].szFileName,0);
	}

	return ( m_pImageInfoArray[idx].pSpriteObject );
}
IDISpriteObject * cResourceManager::GetImageInfo( char * fileName, cImageSize size, int layer )
{

	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		if( m_pImageInfoArray[i].pSpriteObject && 0 == strcmp( m_pImageInfoArray[i].szFileName, fileName ) )
		{
			return m_pImageInfoArray[i].pSpriteObject;
		}
	}

	PTRLISTPOS pos = m_pImageList->GetHeadPosition();
	while(pos)
	{
		IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetNext(pos);
		if(0 == strcmp( node->szFileName, fileName ) )
		{	
			return node->pSpriteObject;
		}
	}

	IMAGE_NODE * pNewNode	= new IMAGE_NODE;
	strcpy( pNewNode->szFileName, fileName );
	pNewNode->pSpriteObject = g_pExecutive->GetRenderer()->CreateSpriteObject(fileName,0);
	pNewNode->layer			= layer;
	pNewNode->size			= size;
	
	if( !pNewNode->pSpriteObject )
	{
		char buffer[255];
		GetCurrentDirectory(255, buffer);
		strcat(buffer, fileName);
		ASSERTMSG(0, buffer);
		if( pNewNode ) delete pNewNode;
	}

	m_pImageList->AddTail( pNewNode );
	return pNewNode->pSpriteObject;
}


void cResourceManager::Release()
{
	ReleaseResourceAll();
	if(m_pImageInfoArray)
	{
		delete [] m_pImageInfoArray;
		m_pImageInfoArray = NULL;
	}
	if(m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

void cResourceManager::ReleaseResourceAll()
{
	if( m_pImageList )
	{	
		for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
		{
			if( m_pImageInfoArray[i].pSpriteObject )
			{
				m_pImageInfoArray[i].pSpriteObject->Release();
				m_pImageInfoArray[i].pSpriteObject = NULL;
			}
		}

		PTRLISTPOS pos = m_pImageList->GetHeadPosition();
		while(pos)
		{
			IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetNext(pos);
			node->pSpriteObject->Release();
			node->pSpriteObject = NULL;
			SAFE_DELETE(node);
		}
		m_pImageList->RemoveAll();
	}
}

char * cResourceManager::GetMsg(int idx)
{
	static char msg[ 512 ];

	CMHFile fp;
	fp.Init(m_szMsgListPath, "rb");

	for(int i = 0 ; i < idx-1 ; ++i )
	{
		// 080225 LUJ, ���ʿ��ϰ� ������ ���� �ʵ��� üũ �ڵ� ����
		if( fp.IsEOF() )
		{
			return "";
		}

		fp.GetLine(msg, sizeof( msg ) );
	}

	fp.GetInt();
//	fp.Seek(1);
	fp.GetLine(msg, sizeof( msg ) );
	
	return msg;
}


void cResourceManager::ReleaseResource(int layer)
{
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		if( m_pImageInfoArray[i].pSpriteObject && m_pImageInfoArray[i].layer == layer)
		{
			m_pImageInfoArray[i].pSpriteObject->Release();
			m_pImageInfoArray[i].pSpriteObject = NULL;
			return;
		}
	}

	PTRLISTPOS pos = m_pImageList->GetHeadPosition();
	while(pos)
	{
		IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetAt(pos);
		if(node->layer == layer)
		{
			node->pSpriteObject->Release();
			node->pSpriteObject = NULL;
			SAFE_DELETE(node);
			m_pImageList->RemoveAt(pos);
			break;
		}
		m_pImageList->GetNext(pos);
	}
}

// 070723 LYW --- cResourceManager : Add function to return msg number of class name.
// ĳ������ Ŭ�������� �����ϴ� ���� ���Ƽ�, ���ҽ� �Ŵ����� Ŭ���� ���� �����ϴ� ����� �߰���.
int cResourceManager::GetClassNameNum(int classIdx)
{
	int msg = 0;
	switch( classIdx )
	{
	case 1111:	msg = 364;	break;
	case 1121:	msg = 365;	break;
	case 1122:	msg = 366;	break;
	case 1131:	msg = 367;	break;
	case 1132:	msg = 368;	break;
	case 1133:	msg = 369;	break;
	case 1141:	msg = 370;	break;
	case 1142:	msg = 371;	break;
	case 1143:	msg = 372;	break;
	case 1144:	msg = 373;	break;
	case 1151:	msg = 374;	break;
	case 1152:	msg = 375;	break;
	case 1153:	msg = 376;	break;
	case 1154:	msg = 377;	break;
	case 1155:	msg = 378;	break;
	case 1156:	msg = 379;	break;
	case 1161:	msg = 380;	break;
	case 1162:	msg = 381;	break;
	case 1163:	msg = 382;	break;

	case 1211:	msg = 364;	break;
	case 1221:	msg = 365;	break;
	case 1222:	msg = 366;	break;
	case 1231:	msg = 367;	break;
	case 1232:	msg = 368;	break;
	case 1233:	msg = 369;	break;
	case 1241:	msg = 370;	break;
	case 1242:	msg = 371;	break;
	case 1243:	msg = 372;	break;
	case 1244:	msg = 373;	break;
	case 1251:	msg = 374;	break;
	case 1252:	msg = 375;	break;
	case 1253:	msg = 376;	break;
	case 1254:	msg = 377;	break;
	case 1255:	msg = 378;	break;
	case 1256:	msg = 379;	break;
	case 1261:	msg = 380;	break;
	case 1262:	msg = 381;	break;
	case 1263:	msg = 382;	break;

	case 2111:	msg = 383;	break;
	case 2121:	msg = 384;	break;
	case 2122:	msg = 385;	break;
	case 2131:	msg = 386;	break;
	case 2132:	msg = 387;	break;
	case 2133:	msg = 388;	break;
	case 2141:	msg = 389;	break;
	case 2142:	msg = 390;	break;
	case 2143:	msg = 391;	break;
	case 2144:	msg = 392;	break;
	case 2151:	msg = 393;	break;
	case 2152:	msg = 394;	break;
	case 2153:	msg = 395;	break;
	case 2154:	msg = 396;	break;

	// 080709 LYW --- cResourceManager : ��ũ������ ������ �߰��Ѵ�.
	case 2155:  msg = 1004;	break;

	case 2161:	msg = 397;	break;
	case 2162:	msg = 398;	break;
	case 2163:	msg = 399;	break;

	case 2211:	msg = 383;	break;
	case 2221:	msg = 384;	break;
	case 2222:	msg = 385;	break;
	case 2231:	msg = 386;	break;
	case 2232:	msg = 387;	break;
	case 2233:	msg = 388;	break;
	case 2241:	msg = 389;	break;
	case 2242:	msg = 390;	break;
	case 2243:	msg = 391;	break;
	case 2244:	msg = 392;	break;
	case 2251:	msg = 393;	break;
	case 2252:	msg = 394;	break;
	case 2253:	msg = 395;	break;
	case 2254:	msg = 396;	break;

	// 080709 LYW --- cResourceManager : ��ũ������ ������ �߰��Ѵ�.
	case 2255:	msg = 1004;	break;

	case 2261:	msg = 397;	break;
	case 2262:	msg = 398;	break;
	case 2263:	msg = 399;	break;

	case 3111:	msg = 400;	break;
	case 3121:	msg = 401;	break;
	case 3122:	msg = 402;	break;
	case 3131:	msg = 403;	break;
	case 3132:	msg = 404;	break;
	case 3133:	msg = 405;	break;
	case 3141:	msg = 406;	break;
	case 3142:	msg = 407;	break;
	case 3143:	msg = 408;	break;
	case 3144:	msg = 409;	break;
	case 3151:	msg = 410;	break;
	case 3152:	msg = 411;	break;
	case 3153:	msg = 412;	break;
	case 3154:	msg = 413;	break;
	case 3155:	msg = 414;	break;
	case 3161:	msg = 415;	break;
	case 3162:	msg = 416;	break;
	case 3163:	msg = 417;	break;

	case 3211:	msg = 400;	break;
	case 3221:	msg = 401;	break;
	case 3222:	msg = 402;	break;
	case 3231:	msg = 403;	break;
	case 3232:	msg = 404;	break;
	case 3233:	msg = 405;	break;
	case 3241:	msg = 406;	break;
	case 3242:	msg = 407;	break;
	case 3243:	msg = 408;	break;
	case 3244:	msg = 409;	break;
	case 3251:	msg = 410;	break;
	case 3252:	msg = 411;	break;
	case 3253:	msg = 412;	break;
	case 3254:	msg = 413;	break;
	case 3255:	msg = 414;	break;
	case 3261:	msg = 415;	break;
	case 3262:	msg = 416;	break;
	case 3263:	msg = 417;	break;
	}

	return msg ;
}

// 080320 LYW --- cResourceManager : Add function to return class icon index.
 int cResourceManager::GetClassIconNum(int nClass)
 {
 	IDINDEX* pIdIdx = NULL ;														// �ε����� Ŭ���� �ε��� ������ ���� �����͸� �����Ѵ�.
 
 	PTRLISTPOS pos	= NULL ;														// ����Ʈ���� ��ġ ������ ���� �����͸� �����Ѵ�.
 
 	pos = m_ClassIconList.GetHeadPosition() ;										// Ŭ���� ������ ����Ʈ�� �ش��� �����Ѵ�.
 
 	while(pos)																		// ��ġ������ �ִµ���, while�� ������.
 	{
 		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos�� �ش��ϴ� �ε���, Ŭ���� �ε��� ������ �޴´�.
 
 		if( pIdIdx )																// ���� ������ ��ȿ���� üũ�Ѵ�.
 		{
 			if( pIdIdx->nClassIdx == nClass )										// ���ڷ� �Ѿ�� Ŭ���� �ε�����, ����Ʈ�� Ŭ���� �ε����� ���ٸ�,
 			{
 				return pIdIdx->nIdx ;												// �ε����� �����Ѵ�.
 			}
 		}
 	}
 
 	return -1 ;	
 }
 
 // 080320 LYW --- cResourceManager : Add function to load class icon.
 void cResourceManager::LoadClassIcon()
 {
 	CMHFile fp;																		// ���� ������ �����Ѵ�.
 
 	fp.Init( FILE_IMAGE_CLASS_ICON_PATH, "rb" ) ;									// Ŭ���� ������ ������ �о���δ�.
 
 	int nNull = 0 ;																	// �ǹ̾��� ���� ���� ������ �����Ѵ�.
 
 	while(TRUE)																		// while���� ������.
 	{
 		if( fp.IsEOF() != FALSE ) break ;											// ������ ���� �����ϸ�, break.
 
 		IDINDEX* pIdIdx = new IDINDEX ;												// �ε����� Ŭ���� �ε����� ���� �޸𸮸� �Ҵ��Ѵ�.
 
 		pIdIdx->nIdx = fp.GetInt() ;												// ���Ϸ� ���� �ε����� �޴´�.
 
 		pIdIdx->nClassIdx = fp.GetInt() ;											// ���Ϸ� ���� Ŭ���� �ε����� �޴´�.
 
 		m_ClassIconList.AddTail(pIdIdx) ;											// Ŭ���� ������ ����Ʈ�� �߰��Ѵ�.
 
 		for( int count = 0 ; count < 5 ; ++count )									// ������ ���� ���� �� ��ŭ for���� ������.
 		{
 			nNull = fp.GetInt() ;													// ���⼭ ������� �ʴ� ���� �׳� �б⸸ �Ѵ�.
 		}
 	}
 
 	fp.Release() ;																	// Ŭ���� ������ ������ �����Ѵ�.
 
 	m_byIconCount = m_ClassIconList.GetCount() ;
 }
 
 // 080320 LYW --- cResourceManager : Add function to release icon list.
 void cResourceManager::ReleaseIconList()
 {
 	IDINDEX* pIdIdx = NULL ;														// �ε����� Ŭ���� �ε��� ������ ���� �����͸� �����Ѵ�.
 
 	PTRLISTPOS pos	= NULL ;														// ����Ʈ���� ��ġ ������ ���� �����͸� �����Ѵ�.
 
 	pos = m_ClassIconList.GetHeadPosition() ;										// Ŭ���� ������ ����Ʈ�� �ش��� �����Ѵ�.
 
 	while(pos)																		// ��ġ������ �ִµ���, while�� ������.
 	{
 		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos�� �ش��ϴ� �ε���, Ŭ���� �ε��� ������ �޴´�.
 
 		if( pIdIdx )																// ���� ������ ��ȿ���� üũ�Ѵ�.
 		{
 			delete pIdIdx ;															// ���� �ּ��� �޸𸮸� �����Ѵ�.
 
 			pIdIdx = NULL ;															// �ּҸ� ����Ű�� �����͸� NULL ó�� �Ѵ�.
 		}
 	}
 
 	m_ClassIconList.RemoveAll() ;													// ����Ʈ�� ��� ����.
 }