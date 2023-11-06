#pragma once

typedef void* ITEMQ_HANDLE;
typedef void* STMPOOL_HANDLE;
typedef void* VBHASH_HANDLE;
typedef void* QBHASH_HANDLE;
typedef void* INDEXCR_HANDLE;
typedef void* ITEMTABLE_HANDLE;
typedef void* DPCQ_HANDLE;
typedef void* BSAE_HANDLE;
typedef void* VHEAP_HANDLE;
typedef void* STRCPOOL_HANDLE;

struct SORT_ITEM_INT
{
	int		iCmp;
	void*	pItem;
};

struct SORT_ITEM_FLOAT
{
	float	fCmp;
	void*	pItem;
};

#define SORT_ITEM_SIZE	8

struct BSAENODE;

struct ITEM_CONTAINER_AE
{
	BSAENODE*			pOwner;
		
	ITEM_CONTAINER_AE*	pPrv;
	ITEM_CONTAINER_AE*	pNext;
	void*				pItem;
	DWORD				dwKey;
};


struct BSAENODE
{
	DWORD				dwKey;
	BSAENODE*			pParent;
	BSAENODE*			pLeft;
	BSAENODE*			pRight;
	DWORD				dwRefCount;
	ITEM_CONTAINER_AE*	pItemContainer;
};


struct SORT_LINK
{
	SORT_LINK*		pPrv;
	void*			pItem;
	SORT_LINK*		pNext;
};

enum HEAP_FAILE_ERROR_TYPE
{
	HEAP_ALLOC_FAIL_ERROR_GREATOR_THAN_ALIGNSIZE	= 0x00000001,	// �Ҵ��Ϸ��� ����� ����λ������ ũ��.
	HEAP_ALLOC_FAIL_ERROR_NOT_AVAILABLE_MEM_BLOCK	= 0x00000002,	// ����� �´� �޸� ���� ����.�޸� ����.
	HEAP_ALLOC_FAIL_ERROR_NOT_AVAILABLE_BLOCK_DESC	= 0x00000004,	// �޸� �� ��ũ���Ͱ� ���ڶ���.
	HEAP_ALLOC_FAIL_ERROR_FAIL_TO_ALIGN				= 0x00000008	// ���Ŀ� �����ߴ�
};

typedef void* (*RESOURCR_ALLOC_FUNC)(void* pArg1,void* pArg2,void* pArg3,DWORD dwSize);
typedef void (*RESOURCR_FREE_FUNC)(void* pResource);


