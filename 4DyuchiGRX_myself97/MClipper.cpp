// MClipper.cpp: implementation of the MClipper class.
//
//////////////////////////////////////////////////////////////////////

#include "MClipper.h"

#include "CheckClock.h"
#include "myself97common.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGXExecutive/GXObject.h"
#include "../4DyuchiGXExecutive/executive.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MClipper::MClipper( )
{
	memset( this, 0, sizeof(MClipper));

}

MClipper::~MClipper()
{
//	DeleteAll();
}


/****************************************************************************
	Init
	static object�� ��츸 buildTree���� �ϰ� ������ �ʱ�ȭ�� ��κ��� �Ѵ�.
****************************************************************************/
DWORD MClipper::Init( CoExecutive* pExecutive, DWORD dwMaxDynamicObjectCount )
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::Init( dwMaxDynamicObjectCount = %d)\n", dwMaxDynamicObjectCount);
#endif
	mdwTempStaticObjectCount	=	0;
	BOOL	bResult;
	m_pExecutive	=	pExecutive;

	md.ICHandle	=	ICCreate();
	ICInitialize( md.ICHandle, dwMaxDynamicObjectCount);

	md.dwMaxCount					=	dwMaxDynamicObjectCount;
	md.dwCount						=	0;
	md.ppObj						=	new CGXMapObject*[md.dwMaxCount];
	md.pdwType						=	new	DWORD[md.dwMaxCount];
	md.pAABB						=	new MAABB[md.dwMaxCount];
	md.pBS							=	new BOUNDING_SPHERE[md.dwMaxCount];
	memset( md.ppObj, 0, sizeof(CGXMapObject*)*md.dwMaxCount);
	memset( md.pdwType, 0, sizeof(DWORD)*md.dwMaxCount);
	memset( md.pAABB, 0, sizeof(MAABB)*md.dwMaxCount);
	memset( md.pBS, 0, sizeof(BOUNDING_SPHERE)*md.dwMaxCount);

	mDObjCandidatePool	=	CreateStaticMemoryPool();
	bResult	=	InitializeStaticMemoryPool( mDObjCandidatePool, sizeof(MClipperObjLink)  , 100, md.dwMaxCount + 2);		// +2�� ���� ��ũ.
	if( bResult	== FALSE)
	{
		_asm int 3;
	}

	// static�� �ӽ� ���۵� �����.
	ms.ppObj				= new CGXMapObject*[50000];	// ������ƽ ������Ʈ�� ����� ����.
	ms.pAABB				=	0;									// ������ƽ ������Ʈ�� aabb ����.
	ms.dwCount				=	50000;				// ������ƽ ������Ư�� ����. �ӽÿ� �ִ밹��.


	mdwMaxFindNodeDepth = 5;			// ��带 Ž���ϴ� �� �� �̻��� �ȳ�������.
	mdwMinFindNodeObjectCount= 4;	// ��带 Ž���ϴ� �� ����� ������Ʈ�� �̸�ŭ ���Ϸ� ������ �ȳ�������.

	mdwMaxRenderNodeDepth = 4;		// �������� ��带 Ž���ϴ� �� �� �̻��� �ȳ�������.

	mDynamicNodePool	=	CreateStaticMemoryPool();

	return 1;
}

/****************************************************************************
	AddStaticObject
	Ʈ���� ����� ���� static ������Ʈ(map ������Ʈ)�� �ִ´�.
	for�� �Ἥ �ѹ��� �Ѱ���.
****************************************************************************/
DWORD MClipper::AddStaticObject(CGXMapObject *pObject)
{

	if( ms.dwCount <= mdwTempStaticObjectCount)	// ������ ��ġ���� ������Ʈ�� �� ���̵����� �Ͷ����� iTemp�Žñ⸦ ����.
	{
		_asm int 3
		return 0xffffffff;
	}
	// ���۸� ä���.
	ms.ppObj[mdwTempStaticObjectCount] = pObject;
	mdwTempStaticObjectCount++;

	return mdwTempStaticObjectCount-1;		// �ε����� 0���̽�.
}

/****************************************************************************
	BuildTree
	AddStaticObject�� ���� ������Ʈ�� �̿��ؼ� Ʈ���� build �Ѵ�.
	�̶� Ʈ���� ������ ���������, �����ȴ�.
	�������� iMode�� �Ⱦ���.
****************************************************************************/
DWORD MClipper::BuildTree(DWORD dwMode, MAABB WorldBox, DWORD dwMaxLevel)
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::BuildTree(dwMode = %d, dwMaxLevel = %d)\n", dwMode, dwMaxLevel);
#endif

#ifdef	_DEBUG
	if( mpNode)
	{
		// Ʈ���� �������� �ʰ� �ι�° ��������� �ִ�.
		PrintfDebugString("MClipper::BuildTree() Line = %d Ʈ���� �������� �ʰ� �ι�° ��������� �ִ�.", __LINE__);
		_asm int 3;
		DeleteAll();
	}
#endif

	DWORD		dwBuildTick	=	GetTickCount();

	mdwMode	=	dwMode;

	if( dwMaxLevel == 0)
		dwMaxLevel	=	1;

	BOOL		bResult;

	mWorldBox				=	WorldBox;
	mdwMaxLevel				=	dwMaxLevel;

	DWORD i = 0, j = 0;

	// miStaticObjectCount, ppStaticObject.
	CGXMapObject**	pTemp = new CGXMapObject*[mdwTempStaticObjectCount];
	memcpy( pTemp, ms.ppObj, sizeof( CGXMapObject*) * mdwTempStaticObjectCount);
	delete[]	ms.ppObj;
	ms.ppObj			=	pTemp;
	ms.dwCount			=	mdwTempStaticObjectCount;

	ms.pdwType		=	new	DWORD[ms.dwCount];

	// ���� �ڽ� �ٽ� �����.
	COLLISION_MESH_OBJECT_DESC*	pTempDesc	=	ms.ppObj[0]->GetCollisionMeshObjectDesc();
	MAABB TempBox;
	TempBox.Max = TempBox.Min = pTempDesc->boundingBox.v3Oct[0];

	// ����.
	ms.pAABB		=	new		MAABB[ms.dwCount];
	ms.pBS			=	new		BOUNDING_SPHERE[ms.dwCount];
	ms.pBB			=	new		BOUNDING_BOX[ms.dwCount];
	for( i = 0; i < ms.dwCount; i++)
	{
		ms.pdwType[i]	=	ms.ppObj[i]->GetObjectType();

		COLLISION_MESH_OBJECT_DESC*	pDesc;
		pDesc = ms.ppObj[i]->GetCollisionMeshObjectDesc();

		// BB
		ms.pBB[i]		=	pDesc->boundingBox;

		// AABB
		MAABB Box;
		Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
		for( j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// �ִ�.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// �ּ�.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;

			// �ִ�.
			if( TempBox.Max.x < vec3.x) TempBox.Max.x = vec3.x;
			if( TempBox.Max.y < vec3.y) TempBox.Max.y = vec3.y;
			if( TempBox.Max.z < vec3.z) TempBox.Max.z = vec3.z;
			// �ּ�.x
			if( TempBox.Min.x > vec3.x) TempBox.Min.x = vec3.x;
			if( TempBox.Min.y > vec3.y) TempBox.Min.y = vec3.y;
			if( TempBox.Min.z > vec3.z) TempBox.Min.z = vec3.z;
		}
		ms.pAABB[i] = Box;

		// ����.
		ms.pBS[i]		=	GetSphereFromAABB(Box);
	}

	// ������ƽ ������Ʈ�� ���� TempBox�� ����ڽ��� ��, ū���� ����ڽ��� �����.
	bResult	=	TestWorldBox( TempBox);
	if( bResult == FALSE)
	{
//		_asm int 3;
	}

	if( mWorldBox.Max.x == mWorldBox.Min.x)
		mWorldBox.Max.x	=	mWorldBox.Min.x + 10.0f;
	if( mWorldBox.Max.y == mWorldBox.Min.y)
		mWorldBox.Max.y	=	mWorldBox.Min.y + 10.0f;
	if( mWorldBox.Max.z == mWorldBox.Min.z)
		mWorldBox.Max.z	=	mWorldBox.Min.z + 10.0f;

	PrintfDebugString("MClipper::BuildTree() WorldBox.Max = ( %f, %f, %f), Min = ( %f, %f, %f)\n", mWorldBox.Max.x, mWorldBox.Max.y, mWorldBox.Max.z, mWorldBox.Min.x, mWorldBox.Min.y, mWorldBox.Min.z);
	


	
	// Ʈ���� ���� ����� ����. Ǯ Ʈ���̹Ƿ� ���ϴ�.
	mdwNodeCount		=	(int(pow(4.0f,(int)( dwMaxLevel+1)) - 1) / (4 - 1));

	// ��尡 ���� dobj ��ũ Ǯ, �ִ� ������ ���� ������ �̰� Ʋ���� ������ �𸣰ڴ�.
	mdwMaxDynamicNodePool		=	((dwMaxLevel+1) * 4 * md.dwMaxCount) + 	mdwNodeCount*2;
	bResult	=	InitializeStaticMemoryPool( mDynamicNodePool, sizeof(MClipperObjLink)  , 1000, mdwMaxDynamicNodePool);
	if( bResult	== FALSE)
	{
//		_asm int 3;
	}

	// ���� Ʈ���� ����� �ǰ���.
	mdwNodeUsedCount	=	0;
	this->mpNode	=	new	MClipperNode[mdwNodeCount];
	memset( mpNode, 0, sizeof(MClipperNode)*mdwNodeCount);
	CreateNodeReculsive( 0xffffffff, 0);


	this->CreateCandidateBuffer( md.dwMaxCount, ms.dwCount);
	
	// �ӽù���.
	mpTempStaticIndexBuffer	=	new BYTE[ms.dwCount];



	dwBuildTick	=	GetTickCount()	-	dwBuildTick;
#ifdef	_DEBUG
	PrintfDebugString("MClipper::BuildTree Time = %d Tick\n", dwBuildTick);
#endif
	
	return 1;
}

/****************************************************************************
	AddDynamicObject
	���� �����ϴ� ������Ʈ �̿��� ���� add�Ѵ�. 
****************************************************************************/
DWORD MClipper::AddDynamicObject(CGXMapObject *pObject)
{
	
#ifdef	_DEBUG
	DWORD	dwTemp;
	// Ȥ�� ���� ������Ʈ�� �ι�° ������ �ʾҳ�.?
	for( dwTemp = 0; dwTemp < md.dwMaxCount; dwTemp++)
	{
		if( md.ppObj[dwTemp] == pObject)
		{
			PrintfDebugString("%s, %d ���� ���� ���������� �̹� Ŭ���ۿ� �Ҽ� �Ǿ� �ִ�.", __FILE__, __LINE__);
			_asm int 3;
		}
	}
#endif

	// �� �ε��� �ϳ� ��,
	DWORD	dwIndex	=	ICAllocIndex( md.ICHandle) - 1;
	if( dwIndex == 0xffffffff)
	{
		_asm int 3;			// ���̳��� ������Ʈ�� ���� �ʰ�.
		return	0xffffffff;
	}

	// �ε����� ������ ä���,
	md.ppObj[dwIndex]	=	pObject;
	md.pdwType[dwIndex]		=	pObject->GetObjectType();

	// aabb �ٿ���ڽ�.
	COLLISION_MESH_OBJECT_DESC*	pDesc;
	pDesc = pObject->GetCollisionMeshObjectDesc();

	MAABB Box;
	Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
	if( pObject->GetObjectType() == GX_MAP_OBJECT_TYPE_LIGHT)
	{
		Box.Max.x	=	pDesc->boundingSphere.v3Point.x	+	pDesc->boundingSphere.fRs;
		Box.Max.y	=	pDesc->boundingSphere.v3Point.y	+	pDesc->boundingSphere.fRs;
		Box.Max.z	=	pDesc->boundingSphere.v3Point.z	+	pDesc->boundingSphere.fRs;
		Box.Min.x	=	pDesc->boundingSphere.v3Point.x	-	pDesc->boundingSphere.fRs;
		Box.Min.y	=	pDesc->boundingSphere.v3Point.y	-	pDesc->boundingSphere.fRs;
		Box.Min.z	=	pDesc->boundingSphere.v3Point.z	-	pDesc->boundingSphere.fRs;
	}
	else
	{
		for( int j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// �ִ�.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// �ּ�.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;
		}
	}

	md.pAABB[dwIndex] = Box;

	// �ٿ�� ���Ǿ�.
	md.pBS[dwIndex] = pDesc->boundingSphere;

	// Ʈ���� �μ�Ʈ.
	if( mpNode != 0)
	{
		AddDynamicObjectToTree( dwIndex);
	}

	md.dwCount++;
	/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Add-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif*/
	return dwIndex;
}

/****************************************************************************
	UpdateDynamicObject
	dynamicobject�� ���¸� �����Ѵ�, ��ġ�������� �� �ѵ�, �浹ó����� �� �� �����غ����Ѵ�.
****************************************************************************/
BOOL MClipper::UpdateDynamicObject(DWORD dwIndex)
{

	DWORD	dwDeleteClock;
	DWORD	dwAddClock;
	DWORD	dwUpdateClock;
#ifdef	_DEBUG
	// �� �ε����� ���Ǵ°�.?
	if( md.ppObj[dwIndex] == 0)	// ���ǰ� ���� ������.
	{
		OutputDebugString( "MClipper::UpdateDynamicObject()�� ��ȿ���� ���� �ε����� ���Դ�.");
		_asm int 3;
		return FALSE;

	}
	if( dwIndex > md.dwMaxCount)
	{
		OutputDebugString( "MClipper::UpdateDynamicObject()�ִ밪�� �Ѿ �ε����� ���Դ�.");
		_asm int 3;
		return	FALSE;
	}
#endif
	
	dwDeleteClock	=	GetLowClock();
	// Ʈ������ �����.
	if( mpNode)
	{
		DeleteDynamicObjectFromTree( dwIndex);
	}
	dwDeleteClock	=	GetLowClock()	-	dwDeleteClock;

	dwUpdateClock	=	GetLowClock();
	COLLISION_MESH_OBJECT_DESC*	pDesc;
	pDesc = md.ppObj[dwIndex]->GetCollisionMeshObjectDesc();			// Ŭ���� �ʹ� ����.?
	MAABB Box;
	Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
	if( md.ppObj[dwIndex]->GetObjectType() == GX_MAP_OBJECT_TYPE_LIGHT)
	{
		Box.Max.x	=	pDesc->boundingSphere.v3Point.x	+	pDesc->boundingSphere.fRs;
		Box.Max.y	=	pDesc->boundingSphere.v3Point.y	+	pDesc->boundingSphere.fRs;
		Box.Max.z	=	pDesc->boundingSphere.v3Point.z	+	pDesc->boundingSphere.fRs;
		Box.Min.x	=	pDesc->boundingSphere.v3Point.x	-	pDesc->boundingSphere.fRs;
		Box.Min.y	=	pDesc->boundingSphere.v3Point.y	-	pDesc->boundingSphere.fRs;
		Box.Min.z	=	pDesc->boundingSphere.v3Point.z	-	pDesc->boundingSphere.fRs;
	}
	else
	{
		for( int j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// �ִ�.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// �ּ�.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;
		}
	}
	md.pAABB[dwIndex] = Box;
	// �ٿ�� ���Ǿ�.
	md.pBS[dwIndex] = pDesc->boundingSphere;

	dwUpdateClock	=	GetLowClock()	-	dwUpdateClock;

	dwAddClock		=	GetLowClock();
	// Ʈ�� ����.
	if( mpNode)
	{
		AddDynamicObjectToTree(dwIndex);
	}
	dwAddClock		=	GetLowClock()	-	dwAddClock;
/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Update-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif*/
	return TRUE;

}

/****************************************************************************
	DeleteDynamicObject
	Add�ߴ� ������Ʈ�� �����Ѵ�.
****************************************************************************/
BOOL MClipper::DeleteDynamicObject(DWORD dwIndex)
{

	// ������ �� �ε����� ���Ǵ°�.?
	if( md.ppObj[dwIndex] == NULL)		// ���ǰ� ���� ������.
	{
		_asm int 3;
#ifdef	_DEBUG
	PrintfDebugString("MClipper::DeleteDynamicObject(dwIndex = %d) Line = %d  �� �ε����� ���ǰ� ���� �ʴ�. �� _asm int 3�� �ּ�ó�� �ߴ°�.?\n", dwIndex, __LINE__);
#endif
		return FALSE;
	}

	// Ʈ������ �� ������Ʈ�� �����,
	if( mpNode )
	{
		DeleteDynamicObjectFromTree(dwIndex);
	}

	// �������� ���� �ش� gxmapobject�� Ŭ�����ε����� 0xffffffff�� �����.
	// �̴� gxmap�� �������� Ŭ���۰� �������ٰ� gxmap�� �ٽ� �����Ǿ� Ŭ���� ���� �ٽ� ���� �� ���¿���,
	// �ٱ����� �������� ���� gxmapobject�� updatetogxmap�� ȣ������ ���� ���¿��� Ŭ���۸� �ٷ��� ���ϰ� ��� �ϱ� �����̴�.
	// ���� ������ �ڵ尡 DeleteDynamicObject()�� DeleteDecal()�� �����Ѵ�.
	md.ppObj[dwIndex]->SetClipperIndex( 0xffffffff);

	// �ε����� ������ �����,
	md.ppObj[dwIndex]	=	NULL;
	memset( &(md.pAABB[dwIndex]), 0, sizeof( MAABB));
	// �ε��� ��ȯ.
	ICFreeIndex( md.ICHandle, dwIndex+1);
	md.dwCount--;

#ifdef	_DEBUG
	if( md.dwCount == 0xffffffff)
	{
		_asm int 3;		// ��ȣ�� - �Ʒ��� �������� �Ǵ°�.?
	}
#endif
/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Delete-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif
	*/
	return TRUE;
}

/****************************************************************************
	FindRenderCandidate
	vv�� �־��� ������ ������ ������ �ĺ��� ã�Ƴ���.
	disable ��Ű�� �ڱⰡ ���� ���۸� �״�� ������´�.
****************************************************************************/
void MClipper::FindRenderCandidate(VIEW_VOLUME &vv)
{
	if( mpNode == 0)
	{
		return ;
	}

	BOOL	bIn		=	FALSE;

	DWORD	dwObjType;
	mdwDBG_TraveledNodeCount = 0;
	unsigned int iLowClock = 0;

	DWORD i = 0;

		memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);		// �ӽ� �ε��� ����. static

		memset( mRCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);
		memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

		// dobj ��ũ ����.
		this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
		mdwDObjCandidateCount	=	0;
		mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

		iLowClock = GetLowClock();
		FindRenderCandidateReculsive( 0, vv);		// ���. ã��.
		iLowClock = GetLowClock() - iLowClock;

		iLowClock = GetLowClock();
		// static object ���� ����.
		for( i = 0; i < ms.dwCount; i++)
		{
			if( mpTempStaticIndexBuffer[i] != 0)
			{
				dwObjType		=	ms.pdwType[i];//mppStaticObject[i]->GetObjectType();

				// �亼�� Ŭ��.
//				bIn	=	ClipSphereWithViewVolume( &vv, &(ms.pBS[i]));
				bIn = ClipRenderObjectWithViewVolume(&vv,ms.ppObj[i]->GetCollisionMeshObjectDesc(),0);
				if( bIn)
				{
					mRCBuff.pppBuff[dwObjType][mRCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
					mRCBuff.pdwCount[dwObjType]++;
				}
			}
		}
		iLowClock = GetLowClock() - iLowClock;

		iLowClock = GetLowClock();
		// dynamic object ���� ����.
		MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;

		DWORD dwIndex;
		while(1)
		{
			dwIndex		=	pLink->iIndex;
			if( dwIndex	==	INT_MAX)
				break;

			// �亼�� Ŭ��.
//			bIn	=	ClipSphereWithViewVolume( &vv, &(md.pBS[dwIndex]));
			bIn = ClipRenderObjectWithViewVolume(&vv,md.ppObj[dwIndex]->GetCollisionMeshObjectDesc(),0);
			if( bIn)
			{
				dwObjType		=	md.pdwType[dwIndex];//md.ppObj[iIndex]->GetObjectType();
				mRCBuff.pppBuff[dwObjType][mRCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
				mRCBuff.pdwCount[dwObjType]++;
			}

			pLink	=	pLink->pNext;
			


		}

		iLowClock = GetLowClock() - iLowClock;


	return ;
}

/****************************************************************************
****************************************************************************/
void MClipper::DeleteAll()
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::DeleteAll()\n");
#endif

	DWORD	dwClock;
	DWORD	i;

	// static object
	{
		if( ms.ppObj)				delete[]	ms.ppObj;			// ������ƽ ������Ʈ�� ����� ����.
		if( ms.pdwType)				delete[]	ms.pdwType;
		if( ms.pAABB)				delete[]	ms.pAABB;			// ������ƽ ������Ʈ�� aabb ����
		if( ms.pBS)					delete[]	ms.pBS;
		if( ms.pBB)					delete[]	ms.pBB;
	}

	DWORD	dwRemainedObject	=	0;

	// dynamic object
	{
#ifdef	_DEBUG
	PrintfDebugString("�����ִ� Dynamic Object ���� = %d\n", md.dwCount);
	PrintfDebugString("�����ִ� Dynamic Object ����Ʈ\n");
#endif
		for( i = 0; i < md.dwMaxCount; i++)
		{
			if( md.ppObj[i])
			{
				dwRemainedObject++;
#ifdef	_DEBUG
	
	DWORD	dwHandleType	=	m_pExecutive->IsValidHandle( md.ppObj[i]);
	if( dwHandleType == GX_MAP_OBJECT_TYPE_INVALID)
	{
		PrintfDebugString("%d�ε����� ����Ű�� �ڵ��� ��ȿ�� �ڵ��� �ƴϴ�. Ȥ�� UpdateToGXMap�� �ι� ���� �ʾҳ�.?\n", i);
		_asm int 3;
		goto	lbHere;
	}

	switch( md.pdwType[i])
	{
	case	GX_MAP_OBJECT_TYPE_OBJECT:
		PrintfDebugString("%s\n", ((CoGXObject*)md.ppObj[i])->m_pModelFileItem->szFileName);
		break;
	case	GX_MAP_OBJECT_TYPE_LIGHT:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_LIGHT\n");
		break;
	case	GX_MAP_OBJECT_TYPE_TRIGGER:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_TRIGGER\n");
		break;
	case	GX_MAP_OBJECT_TYPE_DECAL:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_DECAL\n");
		break;
	default:
		PrintfDebugString("default??\n");
	}

lbHere:
#endif
				md.ppObj[i]->SetClipperIndex( 0xffffffff);
			}
		}
#ifdef	_DEBUG
	PrintfDebugString("���� ���� ������Ʈ �հ� = %d\n", dwRemainedObject);
	PrintfDebugString("��.\n");
#endif

		if( md.ppObj)		delete[]	md.ppObj;			// ���̳��� ������Ʈ�� ����� ����.
		if( md.pdwType)			delete[]	md.pdwType;
		if(	md.pAABB)	delete[]	md.pAABB;		// ���̳��� ������Ʈ�� aabb ����.
		if( md.pBS)		delete[]	md.pBS;			// ���̳��� ������Ʈ�� ���� ����.
	}

	// Ʈ�� �����.
	dwClock		=	GetLowClock();
	DeleteNodes( );
	dwClock		=	GetLowClock()	-	dwClock;

	if( mpTempStaticIndexBuffer)		delete[]	mpTempStaticIndexBuffer;

	// dobj ���� �ĺ�.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mpDObjCandidateLink	=	0;
	if( mDObjCandidatePool)
		ReleaseStaticMemoryPool(mDObjCandidatePool);
	mDObjCandidatePool	=	0;


	// ã���Լ� ���� �����.
	DeleteCandidateBuffer();
//	miStaticCandidateCount		=	0;
//	if( mppStaticCandidateBuffer)		delete[]	mppStaticCandidateBuffer;
//	miDynamicCandidateCount	=	0;
//	if( mppDynamicCandidateBuffer)		delete[]	mppDynamicCandidateBuffer;

	if (mDynamicNodePool)
		ReleaseStaticMemoryPool( mDynamicNodePool);
	mDynamicNodePool	=	0;

//	md.IndexManager.Delete();
	ICRelease( md.ICHandle);

	memset(this,0,sizeof(MClipper));
}

/****************************************************************************
	private:	CreateNodeReculsive
	�θ�, �ڽĹ�ȣ�� �޾� ��带 �ϳ� ����� �����´�.
	���.
****************************************************************************/
DWORD MClipper::CreateNodeReculsive(DWORD dwParent, DWORD dwChildNum)
{
	DWORD*	pTempIndex = NULL;
	MClipperNode*		pNode	=	0;
	DWORD				dwNode	=	0;
	DWORD i = 0;

	if( dwParent == 0xffffffff)	// top���.
	{
		pNode	=	&(mpNode[0]);
		dwNode	=	0;

		pNode->dwParent		=	0xffffffff;							// �θ���.
		pNode->dwChildNum	=	0;
		pNode->dwLevel		=	0;								// �� ����� ��.
		pNode->Box			=	mWorldBox;					// �� ����� �ٿ�� �ڽ�.

		// static obj.
		pNode->dwStaticObjectCount	=	ms.dwCount;			// static object�� ����.
		pNode->pStaticObjectIndex	=	new DWORD[ms.dwCount];	// static object�� �ε��� ����.
		for( i = 0; i < pNode->dwStaticObjectCount; i++)
		{
			pNode->pStaticObjectIndex[i] = i;
		}

		goto	lbCommonCode;

	}
	else		// �Ϲ� ���.
	{
		// �ϴ� ��带 �����. ���̳����� ����������� ���ư���. �ڽĳ�带 �װ��� �� ä����.....
		pNode	=	&(mpNode[mdwNodeUsedCount]);
		dwNode	=	mdwNodeUsedCount;
		if( mdwNodeUsedCount == mdwNodeCount)
			_asm int 3;
		// �ϴ� �ڱ� ��� ä���,
		pNode->dwParent = dwParent;
		pNode->dwLevel	=	mpNode[dwParent].dwLevel	+	1;
		pNode->dwChildNum	=	dwChildNum;
		pNode->Box		=	GetChildAABB( dwParent, dwChildNum);

		// �θ��� static object�� ���鼭 �ڱ����� ���Ѱ��� �������.
		pTempIndex = new DWORD[mpNode[pNode->dwParent].dwStaticObjectCount];
		pNode->dwStaticObjectCount	=	0;
		for( i = 0; i < mpNode[pNode->dwParent].dwStaticObjectCount; i++)
		{
			DWORD	dwObjIndex	=	mpNode[pNode->dwParent].pStaticObjectIndex[i];
			BOOL	bDidMeet	=	IsAABBMeetAABB( ms.pAABB[dwObjIndex], pNode->Box);
			if( bDidMeet)	// pTemp��忡 ���ϴ� ������Ʈ�� ������, 
			{
				pTempIndex[pNode->dwStaticObjectCount] = dwObjIndex;
				pNode->dwStaticObjectCount++;
			}
		}
		// ���� ����.
		pNode->pStaticObjectIndex = new DWORD[pNode->dwStaticObjectCount];
		memcpy( pNode->pStaticObjectIndex, pTempIndex, sizeof(DWORD)*pNode->dwStaticObjectCount);
		delete[] pTempIndex;
	}
	
lbCommonCode:		// ��Ʈ�� �ڽ� ���� �ڵ�.
	mdwNodeUsedCount++;		// ���ۿ��� ���� ����� ����. ��带 �Ҵ��Ҷ� �� ���� �����Ѵ�.

	pNode->Sphere	=	GetSphereFromAABB( pNode->Box);	

	// dynamic object�� ���� ���� ��ũ.
	pNode->pDLink	=	CreateDummyLink(mDynamicNodePool);

	// �ִ� �����ΰ�.?
	if(	mdwMaxLevel <= pNode->dwLevel)
	{
		return dwNode;
	}


	// �ƴϸ� �ڽĳ�带 ����.
	for( i = 0; i < 4; i++)
	{
		pNode->pdwChild[i] = CreateNodeReculsive( dwNode, i);
	}
	return dwNode;
}

/****************************************************************************
	private:	GetChildAABB
	�־��� ����� �־��� �ڽ��� AABB�� �����.
****************************************************************************/
MAABB MClipper::GetChildAABB(DWORD dwParent, DWORD dwChildNum)
{
	MAABB&		PBox = mpNode[dwParent].Box;
	VECTOR3		Middle;
	Middle.x = (PBox.Max.x + PBox.Min.x) / 2;
	Middle.z = (PBox.Max.z + PBox.Min.z) / 2;

	MAABB Box;	// �ڽ� �ڽ�.
	Box.Max.y	=	PBox.Max.y;
	Box.Min.y	=	PBox.Min.y;

	switch( dwChildNum)
	{
		case 0:		// ����.
			Box.Max.x	=	Middle.x;
			Box.Min.x	=	PBox.Min.x;
			Box.Max.z	=	PBox.Max.z;
			Box.Min.z	=	Middle.z;
			break;
		case 1:		// ����.
			Box.Max.x	=	PBox.Max.x;
			Box.Min.x	=	Middle.x;
			Box.Max.z	=	PBox.Max.z;
			Box.Min.z	=	Middle.z;
			break;
		case 2:		// ����.
			Box.Max.x	=	Middle.x;
			Box.Min.x	=	PBox.Min.x;
			Box.Max.z	=	Middle.z;
			Box.Min.z	=	PBox.Min.z;
			break;
		case 3:		// ����.
			Box.Max.x	=	PBox.Max.x;
			Box.Min.x	=	Middle.x;
			Box.Max.z	=	Middle.z;
			Box.Min.z	=	PBox.Min.z;
			break;
		default:	// ����.?
			_asm int 3;
			break;
	}
	return Box;
}


/****************************************************************************
	private:	FindRenderCandidateReculsive
	FindRenderCandidate���� ȣ��Ǵ� Ʈ���� Ÿ�� ����Լ�.
	���̳���, ������ƽ ���� ã�´�.
****************************************************************************/
void MClipper::FindRenderCandidateReculsive(DWORD dwNode, VIEW_VOLUME &vv)
{
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;
	int iResult = 0;
	DWORD	i = 0, j = 0;

	// ������ ��尡 vv�� ����.?

	DWORD	dwTemp	=	GetLowClock();
//	iResult =	WhereIsAABBFromViewVolume( pCurrentNode->Box, vv);
	iResult	=	WhereIsSphereFromViewVolume( pCurrentNode->Sphere, vv);
	dwTemp	=	GetLowClock() - dwTemp;
	// ������ �ȵ��� ����.
	if( iResult < 0)
	{
		goto Return;
	}

	// ������ ���� �������� ������Ʈ�� �ӽ� �ε����� ����ϰ� ����.
	// ������ ���ų�, ���� ��尡 uiFindMaxDepth�̰ų� ���� Ŭ��� �������.
	// ���� ��尡 ���� static object ������ ���� uiMinFindObjectCount �ϰ�� �������.
	if( pCurrentNode->dwLevel	==	mdwMaxLevel || iResult > 0 || pCurrentNode->dwLevel >= mdwMaxRenderNodeDepth)
	{
		// static object
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}


		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}


		goto Return;
	}
	// �ڽ��� �ִ� ���.
	else			// ��͸� goto�� �ٲ۰��� �ٽ��� ���� �ִ�. �׳� pCurrent�� �� �ٲ��ذ�.
	{
//		pCurrentNode = pCurrentNode->ppChild[0];
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[1];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[1])]);
		goto Start;
Child1Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[2];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[2])]);
		goto Start;
Child2Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[3];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[3])]);
		goto Start;
Child3Reculse:

//		pCurrentNode	=	pCurrentNode->pParent;
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

	goto Return;

Return:
	if( pCurrentNode->dwLevel == 0)		// ž����̸�,
	{
		return ;
	}
	switch(pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// ���� �߸����.
	}

	return;
}


/****************************************************************************
	private:	DeleteTreeReculsive
	Ʈ�� ��ü�� ��������� ����������.
****************************************************************************/
void MClipper::DeleteNodes()
{
	if( mpNode)
	{
		DWORD	dwClock		=	GetLowClock();
		for( DWORD i = 0; i < mdwNodeCount; i++)
		{
			// static object
			delete[]	mpNode[i].pStaticObjectIndex;
			mpNode[i].dwStaticObjectCount	=	0;
			
// ������Ҷ� Ǫ�°� �����ٵ�...
			// dynamic object
//			if( mpNode[i].pDLink->pNext->iIndex != INT_MAX)
//				_asm int 3;
			DeleteLink( mDynamicNodePool, mpNode[i].pDLink);
			
		}
		dwClock		=	GetLowClock()	-	dwClock;
		delete[]	mpNode;
		mpNode			=	0;
	}	
}


/****************************************************************************
****************************************************************************/
void MClipper::FindNearObject( BOUNDING_SPHERE& bs)
{
	mdwDBG_TraveledNodeCount = 0;
	int		iResult = 0;

	DWORD	dwObjType;

	memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);

	memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

	// dobj ��ũ ����.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mdwDObjCandidateCount	=	0;
	mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

	unsigned int iClock = GetLowClock();
	// Ʈ��Ÿ��.
	FindNearObjectReculsive( 0, bs);
	iClock = GetLowClock() - iClock;

	// SObj	���� ����.
	iClock = GetLowClock();
	DWORD i = 0, j = 0;
	for( i = 0; i < ms.dwCount; i++)
	{
		// ���� üũ.
		if( mpTempStaticIndexBuffer[i] != 0)
		{
			iResult = WhereIsAABBFromBoundingSphere( ms.pAABB[i], bs);
			if( iResult >= 0)
			{
				dwObjType		=	ms.pdwType[i];
				mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
				mCBuff.pdwCount[dwObjType]++;
			}
		}
	}
	iClock = GetLowClock() - iClock;

	// ���̳��� ������Ʈ.
	iClock = GetLowClock();
	// dynamic object ���� ����.
	MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;
	while(1)
	{
		DWORD dwIndex		=	pLink->iIndex;
		if( dwIndex	== INT_MAX)
			break;

		iResult			=	WhereIsAABBFromBoundingSphere( md.pAABB[dwIndex], bs);
		if( iResult >= 0)
		{
			dwObjType		=	md.pdwType[dwIndex];
			mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
			mCBuff.pdwCount[dwObjType]++;
		}
		pLink	=	pLink->pNext;
	}
	iClock = GetLowClock() - iClock;


	return ;
}

/****************************************************************************
	private:	FindNearObjectReculsive
****************************************************************************/
void MClipper::FindNearObjectReculsive(DWORD dwNode, BOUNDING_SPHERE &bs)
{
	int	iResult = 0;
	DWORD	i = 0;//, j = 0;
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;

	// ���� ���� ��尡 �ٿ�� ���Ǿ� �ȿ� ���°�.?
	iResult	=	WhereIsAABBFromBoundingSphere( pCurrentNode->Box, bs);
	if( iResult == -1)	// ������ �ȵ���.
	{
		goto Return ;
	}

	if( pCurrentNode->dwLevel == mdwMaxLevel || iResult == 1 || pCurrentNode->dwLevel >= mdwMaxFindNodeDepth || pCurrentNode->dwStaticObjectCount <= mdwMinFindNodeObjectCount)	// ������ ���ų� �ִ���̱��� �������...
	{
		// ������ ������Ʈ���� �ӽ��ε����� ����ϰ� ����.
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}

		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}

		goto Return ;
	}
	else 
	{
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[1]]);
		goto Start;
Child1Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[2]]);
		goto Start;
Child2Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[3]]);
		goto Start;
Child3Reculse:
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

Return:
	if( pCurrentNode->dwLevel == 0)
	{
		return ;
	}
	switch( pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3; //������� ���� �ȵ���.. -_-;
	}

}

/****************************************************************************
****************************************************************************/
void MClipper::FindNearObjectWithCapsule( BOUNDING_CAPSULE& bc)
{
	mdwDBG_TraveledNodeCount = 0;
	int		iResult = 0;

	DWORD	dwObjType;

	memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);

	memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

	// dobj ��ũ ����.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mdwDObjCandidateCount	=	0;
	mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

	unsigned int iClock = GetLowClock();
	// Ʈ��Ÿ��.
	FindNearObjectWithCapsuleReculsive( bc);
	iClock = GetLowClock() - iClock;

	// SObj	���� ����.
	iClock = GetLowClock();
	DWORD i = 0, j = 0;
	for( i = 0; i < ms.dwCount; i++)
	{
		// ���� üũ.
		if( mpTempStaticIndexBuffer[i] != 0)
		{
//			iResult = WhereIsAABBFromBoundingSphere( ms.pAABB[i], bs);
			iResult	=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, ms.pBS[i]);
			if( iResult >= 0)
			{
				dwObjType		=	ms.pdwType[i];
				mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
				mCBuff.pdwCount[dwObjType]++;
			}
		}
	}
	iClock = GetLowClock() - iClock;

	// ���̳��� ������Ʈ.
	iClock = GetLowClock();
	// dynamic object ���� ����.
	MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;
	while(1)
	{
		DWORD	dwIndex		=	pLink->iIndex;
		if( dwIndex	==	INT_MAX)
			break;

		iResult			=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, md.pBS[dwIndex]);
		if( iResult >= 0)
		{
			dwObjType		=	md.pdwType[dwIndex];
			mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
			mCBuff.pdwCount[dwObjType]++;
		}
		pLink	=	pLink->pNext;
	}
	iClock = GetLowClock() - iClock;

	return ;
}

void MClipper::FindNearObjectWithCapsuleReculsive( BOUNDING_CAPSULE& bc)
{
	int	iResult = 0;
	DWORD	i = 0;
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;

	// ���� ���� ��尡 �ٿ�� ���Ǿ� �ȿ� ���°�.?
//	iResult	=	WhereIsAABBFromBoundingSphere( pCurrentNode->Box, bs);
	iResult	=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, pCurrentNode->Sphere);
	if( iResult == -1)	// ������ �ȵ���.
	{
		goto Return ;
	}

	if( pCurrentNode->dwLevel == mdwMaxLevel || iResult == 1 || pCurrentNode->dwLevel >= mdwMaxFindNodeDepth || pCurrentNode->dwStaticObjectCount <= mdwMinFindNodeObjectCount)	// ������ ���ų� �ִ���̱��� �������...
	{
		// ������ ������Ʈ���� �ӽ��ε����� ����ϰ� ����.
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}

		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}

		goto Return ;
	}
	else 
	{
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[1]]);
		goto Start;
Child1Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[2]]);
		goto Start;
Child2Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[3]]);
		goto Start;
Child3Reculse:
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

Return:
	if( pCurrentNode->dwLevel == 0)
	{
		return ;
	}
	switch( pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3; //������� ���� �ȵ���.. -_-;
	}

}

/****************************************************************************
****************************************************************************/
void MClipper::SetMaxFindNodeDepth( DWORD dwMaxFindNodeDepth)
{
	mdwMaxFindNodeDepth = dwMaxFindNodeDepth;
}
/****************************************************************************
****************************************************************************/
void MClipper::SetMinFindNodeObjectCount( DWORD dwMinFindNodeObjectCount)
{
	mdwMinFindNodeObjectCount = dwMinFindNodeObjectCount;
}

/****************************************************************************
****************************************************************************/
void MClipper::AddIndexToLink(STMPOOL_HANDLE pool, MClipperObjLink *pLink, int iIndex)
{
#ifdef	_DEBUG
	if( iIndex <= -1 || iIndex >= INT_MAX)
	{
		PrintfDebugString( "%s, %d", __FILE__, __LINE__);
		_asm int 3;		// �Ѱ� �̻��� �ε����� ���Դ�.
	}
#endif

	MClipperObjLink*	pNew;

	while(1)
	{
		if( pLink->iIndex >= iIndex)
		{
			goto	lbReturn;
		}
		else if( pLink->pNext->iIndex > iIndex)		// �������� ����.
		{
			pNew	=	(MClipperObjLink*)LALAlloc( pool);
			pNew->iIndex			=	iIndex;
			pNew->pPrev				=	pLink;
			pNew->pNext				=	pLink->pNext;
			pLink->pNext->pPrev		=	pNew;
			pLink->pNext			=	pNew;
			goto	lbReturn;
		}
		

		pLink	=	pLink->pNext;
		if( pLink->pNext == 0)
			goto	lbReturn;
	}

lbReturn:
	return	;
}

/****************************************************************************
****************************************************************************/
BOOL MClipper::DeleteIndexFromLink(STMPOOL_HANDLE pool, MClipperObjLink* pLink, int iIndex)
{
#ifdef	_DEBUG
	if( iIndex <= -1 || iIndex >= INT_MAX)
	{
		PrintfDebugString( "%s, %d", __FILE__, __LINE__);
		_asm int 3;		// �Ѱ� �̻��� �ε����� ���Դ�.
		return	FALSE;
	}
#endif

	while(1)
	{
		if( pLink->iIndex == iIndex)	// ���� ���.
		{
			pLink->pPrev->pNext		=	pLink->pNext;
			pLink->pNext->pPrev		=	pLink->pPrev;

			LALFree( pool, pLink);
			return		TRUE;
		}

		if( pLink->iIndex > iIndex)
		{
			goto	lbReturnFalse;
		}

		pLink	=	pLink->pNext;
		if( pLink->pNext == 0)
			goto	lbReturnFalse;
	}

lbReturnFalse:
	return	FALSE;
}

/****************************************************************************
	// Ʈ���� ��忡 ���̳��� ������Ʈ�� ���̸�ũ �ΰ��� �ܴ�.
****************************************************************************/
MClipperObjLink* MClipper::CreateDummyLink(STMPOOL_HANDLE pool)
{
	MClipperObjLink*	pDLink	=	(MClipperObjLink*)LALAlloc( pool);
	pDLink->pPrev			=	0;
	pDLink->iIndex			=	-1;
	pDLink->pNext			=	(MClipperObjLink*)LALAlloc( pool);
	pDLink->pNext->pPrev	=	pDLink;
	pDLink->pNext->iIndex	=	INT_MAX;
	pDLink->pNext->pNext		=	0;

	return	pDLink;
}

/****************************************************************************
	Ʈ�� ���� dobj ��ũ�� �ִ´�.
****************************************************************************/
void MClipper::AddDynamicObjectToTree(DWORD dwIndex)
{
	BOOL	bResult;
	MClipperNode*		pNode	=	&mpNode[0];
	int					iNode	=	0;

lbStart:
	// aabb�� �浹.
	bResult		=	IsAABBMeetAABB( pNode->Box, md.pAABB[dwIndex]);
	// �� ���� ������ ��忡 �߰�.
	if( bResult == TRUE)
	{
		AddIndexToLink( mDynamicNodePool, pNode->pDLink, dwIndex);
	}

	// ���� ������ �ʰų� �� ��尡 ������� ���̻� ��� �ʰ� ����.
	if( bResult == FALSE || pNode->dwLevel == mdwMaxLevel)
	{
		goto	lbReturn;
	}
	// �ƴϸ� �ڽ� ���.
	else		
	{
		pNode	=	&(mpNode[pNode->pdwChild[0]]);
		goto	lbStart;
		// �ڽ� ���.
Child0Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[1])]);
		goto	lbStart;
Child1Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[2])]);
		goto	lbStart;
Child2Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[3])]);
		goto	lbStart;
Child3Reculse:

		pNode	=	&(mpNode[pNode->dwParent]);
	}

//	goto lbReturn;

lbReturn:
	if( pNode->dwLevel == 0)		// ž����̸�,
	{
		return ;
	}
	switch(pNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// ���� �߸����.
	}



	_asm int 3;
}

/****************************************************************************
	Ʈ������ dobj�� ����.
****************************************************************************/
void MClipper::DeleteDynamicObjectFromTree(DWORD dwIndex)
{

	BOOL		bResult;
	MClipperNode*		pNode	=	&mpNode[0];
	int					iNode	=	0;

//	return	;

lbStart:
	bResult	=	DeleteIndexFromLink( mDynamicNodePool, pNode->pDLink, dwIndex);
	if( bResult == FALSE || pNode->dwLevel == mdwMaxLevel)
	{
		goto	lbReturn;
	}
	// �ƴϸ� �ڽ� ���.
	else		
	{
		pNode	=	&(mpNode[pNode->pdwChild[0]]);
		goto	lbStart;
		// �ڽ� ���.
Child0Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[1])]);
		goto	lbStart;
Child1Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[2])]);
		goto	lbStart;
Child2Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[3])]);
		goto	lbStart;
Child3Reculse:

		pNode	=	&(mpNode[pNode->dwParent]);
	}

//	goto lbReturn;

lbReturn:
	if( pNode->dwLevel == 0)		// ž����̸�,
	{
		return ;
	}
	switch(pNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// ���� �߸����.
	}

	_asm int 3;

}

/****************************************************************************
****************************************************************************/
void MClipper::DeleteLink(STMPOOL_HANDLE pool, MClipperObjLink *pLink)
{
	if( pLink == 0 || pool == 0)
		goto	lbReturn;

	MClipperObjLink*	pNext;

	while(1)
	{
		pNext	=	pLink->pNext;
		LALFree( pool, pLink);
		pLink	=	pNext;

		if( pLink == 0)
			goto	lbReturn;
	}

lbReturn:
	return	;
}

/****************************************************************************
****************************************************************************/
BOOL MClipper::TestWorldBox( MAABB& TempBox)
{
	float	fTemp;
	BOOL	bResult	=	TRUE;
	// �ƽ��� ��.
	if( TempBox.Max.x > mWorldBox.Max.x)
	{
		bResult		=	FALSE;
		mWorldBox.Max.x	=	TempBox.Max.x;
	}
	if( TempBox.Max.y > mWorldBox.Max.y)
	{
		bResult		=	FALSE;
		mWorldBox.Max.y	=	TempBox.Max.y;
	}
	if( TempBox.Max.z > mWorldBox.Max.z)
	{
		bResult		=	FALSE;
		mWorldBox.Max.z	=	TempBox.Max.z;
	}

	// �ΰ� ��.
	if( TempBox.Min.x < mWorldBox.Min.x)
	{
		bResult		=	FALSE;
		mWorldBox.Min.x	=	TempBox.Min.x;
	}
	if( TempBox.Min.y < mWorldBox.Min.y)
	{
		bResult		=	FALSE;
		mWorldBox.Min.y	=	TempBox.Min.y;
	}
	if( TempBox.Min.z < mWorldBox.Min.z)
	{
		bResult		=	FALSE;
		mWorldBox.Min.z	=	TempBox.Min.z;
	}

	// �ƽ�,�� ���������� ��.
	if( mWorldBox.Max.x - mWorldBox.Min.x <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.x;
		mWorldBox.Max.x		=	mWorldBox.Min.x;
		mWorldBox.Min.x		=	fTemp;
		bResult		=	FALSE;
	}
	if( mWorldBox.Max.y - mWorldBox.Min.y <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.y;
		mWorldBox.Max.y		=	mWorldBox.Min.y;
		mWorldBox.Min.y		=	fTemp;
		bResult		=	FALSE;
	}
	if( mWorldBox.Max.z - mWorldBox.Min.z <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.z;
		mWorldBox.Max.z		=	mWorldBox.Min.z;
		mWorldBox.Min.z		=	fTemp;
		bResult		=	FALSE;
	}


	return	bResult;
}

DWORD MClipper::GetRenderTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)
{
	*pppBuff	=	mRCBuff.pppBuff[dwID];
	return		mRCBuff.pdwCount[dwID];
}

DWORD MClipper::GetTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)
{
	*pppBuff	=	mCBuff.pppBuff[dwID];
	return		mCBuff.pdwCount[dwID];
}

DWORD MClipper::GetMeshObjects(CGXMapObject** ppBuffer,DWORD dwMaxNum)
{
	DWORD	dwResultNum	=	0;

	DWORD	i;
	
	DWORD	dwComponentCount;
	CGXMapObject**	ppBuff;

	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		dwComponentCount	=	this->GetTargetBuffer(&ppBuff, i);

		if( dwComponentCount + dwResultNum > dwMaxNum)
		{
			_asm int 3;				// ���� ũ�⺸�� ������� ����.
			dwComponentCount	=	dwMaxNum	-	dwResultNum;
		}
		memcpy( ppBuffer, ppBuff, sizeof(CGXMapObject*)*dwComponentCount);
		ppBuffer	=	ppBuffer	+	dwComponentCount;		// ������ ���ϱ�.
		dwResultNum	=	dwResultNum		+	dwComponentCount;	// ī��Ʈ ���ϱ�.
	}

	return dwResultNum;
}

// ��常 ��¦ ����� �ٽ� �����Ѵ�.
void MClipper::Rebuild(float fTop,float fBottom)
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::Rebuild( fTop = %f, fBottom = %f)\n", fTop, fBottom);
#endif

	if( mpNode)
	{
		DeleteNodes();

		delete[]	ms.pAABB;
		ms.pAABB	=	0;
		delete[]	ms.pBS;
		ms.pBS		=	0;
		delete[]	ms.pBB;
		ms.pBB		=	0;

		DeleteCandidateBuffer();
		delete[]	mpTempStaticIndexBuffer;
		mpTempStaticIndexBuffer	=	0;
		delete[]	ms.pdwType;
		ms.pdwType		=	0;
		delete[]	mpTempStaticIndexBuffer;
		mpTempStaticIndexBuffer		=	0;

		mWorldBox.Max.y	=	fTop;
		mWorldBox.Min.y	=	fBottom;
		BuildTree( mdwMode, mWorldBox, mdwMaxLevel);

		RefreshDObjects();
	}
	else
	{
		_asm int 3;	// Ʈ���� ������� �ȾҴµ� �����尡 ȣ����.
	}

}

void MClipper::CreateCandidateBuffer(DWORD dwMaxDynamicObjectCount, DWORD dwStaticObjectCount)
{
	// ��¹��� 0, 3, 4�� ���̳���, 1, 2�� ������ƽ ������Ʈ.
	mRCBuff.pppBuff[0]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_OBJECT
	mRCBuff.pppBuff[1]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_STRUCT
	mRCBuff.pppBuff[2]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_HFIELD
	mRCBuff.pppBuff[3]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_DECAL
	mRCBuff.pppBuff[4]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_TRIGGER
	mRCBuff.pppBuff[5]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_LIGHT

	mCBuff.pppBuff[0]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_OBJECT
	mCBuff.pppBuff[1]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_STRUCT
	mCBuff.pppBuff[2]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_HFIELD
	mCBuff.pppBuff[3]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_DECAL
	mCBuff.pppBuff[4]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_TRIGGER
	mCBuff.pppBuff[5]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_LIGHT
}

void MClipper::DeleteCandidateBuffer()
{
	int	i;
	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		if( mRCBuff.pppBuff[i])
		{
			delete	mRCBuff.pppBuff[i];
			mRCBuff.pppBuff[i]		=	0;
		}
		mRCBuff.pdwCount[i]		=	0;
	}
	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		if( mCBuff.pppBuff[i])
		{
			delete	mCBuff.pppBuff[i];
			mCBuff.pppBuff[i]		=	0;
		}
		mRCBuff.pdwCount[i]		=	0;
	}

}

void MClipper::RefreshDObjects()
{
	DWORD	i;
	for( i = 0; i < md.dwMaxCount; i++)
	{
		if( md.ppObj[i] != 0)
		{
			UpdateDynamicObject( i);
		}
	}
}

DWORD MClipper::AddDecal( CGXDecal* pDecal)
{
	// �� �ε��� �ϳ� ��,
	DWORD	dwIndex	=	ICAllocIndex( md.ICHandle) - 1;

	// �� �ε����� ������ ����, ����.
	if( dwIndex == 0xffffffff)
	{
		_asm int 3;
		return	0xffffffff;
	}

	// �ε����� ������ ä���,
	md.ppObj[dwIndex]	=	(CGXMapObject*)pDecal;
	md.pdwType[dwIndex]		=	pDecal->GetObjectType();

	// aabb �ٿ���ڽ�.

	pDecal->GetAABB( &(md.pAABB[dwIndex]));

	// �ٿ�� ���Ǿ�.
	pDecal->GetBoundingSphere( &(md.pBS[dwIndex]));

	// Ʈ���� �μ�Ʈ.
	if( mpNode != 0)
	{
		AddDynamicObjectToTree( dwIndex);
	}

	md.dwCount++;

	return dwIndex;
}

BOOL MClipper::UpdateDecal( DWORD dwIndex)
{
	_asm int 3;			// ������ �����ɰǵ� �ǹ̰� ���� ������.?

	DWORD	dwDeleteClock;
	DWORD	dwAddClock;
	// �� �ε����� ���Ǵ°�.?
	if( md.ppObj[dwIndex] == 0)	// ���ǰ� ���� ������.
	{
		_asm int 3;
		return FALSE;
	}
	
	dwDeleteClock	=	GetLowClock();
	// Ʈ������ �����.
	if( mpNode)
	{
		DeleteDynamicObjectFromTree( dwIndex);
	}
	dwDeleteClock	=	GetLowClock()	-	dwDeleteClock;

	CGXDecal*	pDecal	=	(CGXDecal*)md.ppObj[dwIndex];
	// aabb �ٿ���ڽ�.
	pDecal->GetAABB( &(md.pAABB[dwIndex]));
	// �ٿ�� ���Ǿ�.
	pDecal->GetBoundingSphere( &(md.pBS[dwIndex]));

	dwAddClock		=	GetLowClock();
	// Ʈ�� ����.
	if( mpNode)
	{
		AddDynamicObjectToTree(dwIndex);
	}
	dwAddClock		=	GetLowClock()	-	dwAddClock;

	return TRUE;

}

BOOL MClipper::DeleteDecal( DWORD dwIndex)
{
	// ������ �� �ε����� ���Ǵ°�.?
	if( md.ppObj[dwIndex] == 0)		// ���ǰ� ���� ������.
	{
//		_asm int 3;
		return FALSE;
	}
	// Ʈ������ �� ������Ʈ�� �����,
	if( mpNode )
	{
		DeleteDynamicObjectFromTree(dwIndex);
	}
	// �������� ���� �ش� gxmapobject�� Ŭ�����ε����� 0xffffffff�� �����.
	// �̴� gxmap�� �������� Ŭ���۰� �������ٰ� gxmap�� �ٽ� �����Ǿ� Ŭ���� ���� �ٽ� ���� �� ���¿���,
	// �ٱ����� �������� ���� gxmapobject�� updatetogxmap�� ȣ������ ���� ���¿��� Ŭ���۸� �ٷ��� ���ϰ� ��� �ϱ� �����̴�.
	// ���� ������ �ڵ尡 DeleteDynamicObject()�� DeleteDecal()�� �����Ѵ�.
	md.ppObj[dwIndex]->SetClipperIndex( 0xffffffff);

	// �ε����� ������ �����,
	md.ppObj[dwIndex]	=	NULL;
	memset( &(md.pAABB[dwIndex]), 0, sizeof( MAABB));
	// �ε��� ��ȯ.
//	md.IndexManager.ReleaseIndex( dwIndex);
	ICFreeIndex( md.ICHandle, dwIndex + 1);
	md.dwCount--;

	return TRUE;
}
 