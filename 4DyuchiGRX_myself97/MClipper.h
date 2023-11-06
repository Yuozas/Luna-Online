/*
	MClipper.h
	��Ʈ���� ���� �ε����� �����Ѵ�.

	�⺻ ��ǥ��� �޼տ� x, ������, y����, z����.

	������Ʈ ����:
		������ƽ �޽�, ������ƽ ����Ʈ.		<= �̳���� Ʈ�� ���鶧 ��� ����µ� �����ϰ�...

		���̳��� �޽�, ���̳��� ����Ʈ.		<= �̳���� �ϴ��� �׳� ���� Ÿ�� �� ����, ������ ��忡 ��������... ������..

*/
#if !defined(AFX_MCLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_)
#define AFX_MCLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchigrx_common/typedef.h"
#include "../4dyuchigrx_myself97/MIndexManager.h"
#include "../4dyuchigxExecutive/GXStaticMapObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGXExecutive/GXDecal.h"
#include "../4DyuchiGRX_common/IExecutive.h"


#define		_LOG_

class	CoExecutive;

struct	CLIPPER_CANDIDATE_BUFF
{
	CGXMapObject**			pppBuff[GX_MAP_OBJECT_TYPE_NUM];			// ���۰� Ÿ�Ժ��� 2002,01,20 ���� ������.
	DWORD					pdwCount[GX_MAP_OBJECT_TYPE_NUM];
};

struct	MClipperObjLink
{
	MClipperObjLink*			pPrev;
	MClipperObjLink*			pNext;
	int							iIndex;
};

struct MClipperNode		// Ʈ�� ���.
{
	DWORD							dwParent;		// �θ���.
	DWORD							dwChildNum;		// �ڱ� �ڽ��� �θ��� �������ΰ�.

	DWORD							pdwChild[4];		// �ڽĳ���.

	DWORD							dwLevel;			// �� ����� ��.
	MAABB							Box;			// �� ����� �ٿ�� �ڽ�.
	BOUNDING_SPHERE					Sphere;			// �ٿ�� �ڽ��� �浹ó�� �Ҷ� �ʹ� ����... (����..)

	// static object
	DWORD*							pStaticObjectIndex;		// static object�� �ε��� ����.
	DWORD							dwStaticObjectCount;		// static object�� ����.

	// dynamic object
	MClipperObjLink*				pDLink;					// ���̳��� ������Ʈ ��ũ.

	//	
};

struct	MClipperStaticObject
{
	CGXMapObject**			ppObj;							// ������ƽ ������Ʈ�� ����� ����.
	DWORD*					pdwType;								// ������Ʈ Ÿ��.
	MAABB*					pAABB;						// ������ƽ ������Ʈ�� aabb ����.
	BOUNDING_SPHERE*		pBS;							// �ٿ�� ���Ǿ�.
	BOUNDING_BOX*			pBB;
	DWORD					dwCount;						// ������ƽ ������Ư�� ����.

};

struct	MClipperDynamicObject
{
	INDEXCR_HANDLE			ICHandle;
//	MIndexManager			IndexManager;
	CGXMapObject**			ppObj;						// ���̳��� ������Ʈ�� ����� ����.
	DWORD*					pdwType;
	MAABB*					pAABB;					// ���̳��� ������Ʈ�� aabb ����.
	BOUNDING_SPHERE*		pBS;
	DWORD					dwCount;					// ���̳��� ������Ʈ�� ����.
	DWORD					dwMaxCount;					// ���̳��� ������Ʈ�� �ִ� ����.
};

class MClipper  
{
public:
	MClipper( );
	~MClipper();

	DWORD	Init( CoExecutive* pExecutive, DWORD dwMaxDObjectCount );	/// �ʱ�ȭ.
	// Ʈ�� ���� �Լ���.
	DWORD	AddStaticObject( CGXMapObject* pObject);		// Ʈ���� ����� ���� ������ƽ ������Ʈ(�� ������Ʈ)�� �ִ´�.
	// for������ AddStaticObject�� �Ѵ��� for�� ������ BuildTree �Ѵ�.
	DWORD	BuildTree( DWORD dwMode, MAABB WorldBox, DWORD dwMaxLevel);							// ���� ������ƽ ������Ʈ�� Ʈ���� �����Ѵ�.

	void	GetWorldBox( MAABB* pWorldBox){ *pWorldBox = mWorldBox;}

	// dynamic object �Լ���.
	DWORD	AddDynamicObject( CGXMapObject* pObject);	// ���̳��� ������Ʈ�� �߰��Ѵ�.
	BOOL	UpdateDynamicObject( DWORD dwIndex);			// ������Ʈ�� ���¸� ����.
	BOOL	DeleteDynamicObject( DWORD dwIndex);			// ���̳��� ������Ʈ�� �����Ѵ�.

	// ��Į �Լ���.
	DWORD	AddDecal( CGXDecal* pDecal);
	BOOL	UpdateDecal( DWORD dwIndex);
	BOOL	DeleteDecal( DWORD dwIndex);

	// ���� ����.
	void	SetMinFindNodeObjectCount( DWORD dwMinFindNodeObjectCount);		// Ʈ�� Ż�� ���� ������Ʈ������ ���⿡ ���ϸ� ���̻� �������� �ʴ´�.
	void	SetMaxFindNodeDepth( DWORD dwMaxFindDepth);						// Ʈ�� Ż�� ��� ���̰� ���̻��� �������� �ʴ´�.
	void	SetMaxRenderNodeDepth( DWORD dwMaxRenderDepth);					// Ʈ�� Ż�� ��� ���̰� ���̻��� �������� �ʴ´�.

	// ������ �ĺ� ã��.
	void		FindRenderCandidate( VIEW_VOLUME &vv);		// Ʈ���� Ÿ�� ������ �ĺ��� ã�´�.
	DWORD		GetRenderTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID);

	// ��ó�� ã�� 
	void		FindNearObject( BOUNDING_SPHERE& bs);
	void		FindNearObjectWithCapsule( BOUNDING_CAPSULE& bc);
	DWORD		GetTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID);

	DWORD		GetMeshObjects(CGXMapObject** ppBuffer,DWORD dwMaxNum);		// �̳��� ��������� �͵��� �� ���ļ� �ش�.

	// rebuildTree
	void		Rebuild(float fTop,float fBottom);

	// ��� �����.
	void		DeleteAll(void);

private:

	CoExecutive*				m_pExecutive;
	// static object�� ����� ����.
	MClipperStaticObject	ms;
	DWORD					mdwTempStaticObjectCount;					// �ӽú���. -_-;

	// dynamic object�� ����� ����.
	MClipperDynamicObject	md;

	// Ʈ�� ����.
	MAABB				mWorldBox;								// Ʈ���� ��ü �ٿ�� �ڽ�.
	MAABB				mTempWorldBox;							// �ܺο��� �ذ� �ƴ� ��ü���� �ڽ�. �������߰ڱ�.
	MClipperNode*		mpNode;
	DWORD				mdwNodeCount;
	DWORD				mdwNodeUsedCount;								// Ʈ���� �ִ� ����, 0���̽�.
	DWORD				mdwMaxLevel;								// Ʈ���� �ִ� ����, 0���̽�.
	DWORD				mdwMode;

	DWORD				mdwMaxFindNodeDepth;									// ã������ �ִ� ���� ����.
	DWORD				mdwMinFindNodeObjectCount ;							// ã���� ��尡 ���� ������Ʈ�� �� ���� �����̸� �׸� ����.
	DWORD				mdwMaxRenderNodeDepth;								// ã������ �ִ� ���� ����.

	// ������ ����.
	CLIPPER_CANDIDATE_BUFF		mRCBuff;			// Render Candidate Buff

	// �׿� ����� ����.
	CLIPPER_CANDIDATE_BUFF		mCBuff;				// Candidate Buff

	// ���̳��� ������Ʈ ����� ��ũ.
	STMPOOL_HANDLE			mDObjCandidatePool;
	MClipperObjLink*		mpDObjCandidateLink;
	DWORD					mdwDObjCandidateCount;

	// �ӽù���.
	BYTE*					mpTempStaticIndexBuffer;

	DWORD					mdwDBG_TraveledNodeCount;		// ��� ���� ����.

	STMPOOL_HANDLE			mDynamicNodePool;			// ��尡 ���� ���̳��� ������Ʈ �ε��� �޸�Ǯ.
	DWORD					mdwMaxDynamicNodePool;

protected:
	BOOL	TestWorldBox( MAABB& TempBox);

	// Ʈ�� ���鶧 ���� �Լ���.
	DWORD	CreateNodeReculsive( DWORD dwParent, DWORD dwChildNum);	// Ʈ���� �����. ���.
	MAABB	GetChildAABB( DWORD dwParent, DWORD dwChildNum);	// pParent�� iChildNum��° �ڽ��� AABB�� ���Ѵ�.

	void	CreateCandidateBuffer( DWORD dwMaxDynamicObjectCount, DWORD dwStaticObjectCount);

	// ���̳��� ������Ʈ �߰�, ���� �Լ���.
	void	AddDynamicObjectToTree( DWORD dwIndex);
	void	DeleteDynamicObjectFromTree( DWORD dwIndex);

	// ã�� �Լ��� ���� �Լ�.
	void	FindRenderCandidateReculsive( DWORD dwNode, VIEW_VOLUME& vv);	// ������ �ĺ��� ã�� ����Լ�.
	void	FindNearObjectReculsive( DWORD dwNode, BOUNDING_SPHERE& bs);
	void	FindNearObjectWithCapsuleReculsive( BOUNDING_CAPSULE& bc);

	// ��帵ũ, ������ ���۸�ũ �ٷ�� �Լ���.
	MClipperObjLink*	CreateDummyLink( STMPOOL_HANDLE pool);
	void	DeleteLink(STMPOOL_HANDLE pool, MClipperObjLink* pLink);
	void	AddIndexToLink( STMPOOL_HANDLE pool, MClipperObjLink* pNode, int iIndex);
	BOOL	DeleteIndexFromLink( STMPOOL_HANDLE pool, MClipperObjLink* pNode, int iIndex);			// �� �ε����� �־����� true

	// rebuild tree
	void	RefreshDObjects();			// ������ Ʈ���� ���� ���̳��� ������Ʈ�� Ʈ���� �ִ´�.

	// DeleteAll ���� �Լ���.
	void	DeleteNodes( );
	void	DeleteCandidateBuffer();
};

#endif // !defined(AFX_MOCTREECLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_)
