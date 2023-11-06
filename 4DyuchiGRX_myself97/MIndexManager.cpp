// MIndexManager.cpp: implementation of the MIndexManager class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "MIndexManager.h"
#include <memory.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MIndexManager::MIndexManager()
{
	memset( this, 0, sizeof(MIndexManager));
}

MIndexManager::~MIndexManager()
{
	this->Delete();
}

void MIndexManager::Init(int iMaxIndexCount)
{
#ifdef	_DEBUG
	if( mpBuffer != 0)	// �̹� �ʱ�ȭ�Ǿ� ���ǰ� �ִٸ�,
	{
		_asm int 3;
		this->Delete();
	}
#endif
	mpBuffer			=	new	MIndexNode[iMaxIndexCount];
	miMaxIndexCount	=	iMaxIndexCount;

	miUsedIndexEntry	=	-2;		// ���� ��...

	for( int i = 0; i < miMaxIndexCount; i++)
	{
		mpBuffer[i].iIndex = i;
		mpBuffer[i].iNext = i+1;
		mpBuffer[i].iPrev = i-1;
	}
	// ���� ù��° �ε����� iPrev�� -2��, ���� ������ �ε����� iNext�� -2;
	mpBuffer[0].iPrev	=	-2;
	mpBuffer[miMaxIndexCount-1].iNext = -2;

	miUsedIndexCount	=	0;
	miEmptyIndexEntry	=	0;

	mpIndexArrayBuff	=	new	int[iMaxIndexCount];
}

int MIndexManager::GetIndex()
{
#ifdef	_DEBUG
	if( miUsedIndexCount >= miMaxIndexCount)	// ���̻� �߱� ������ �ε����� ����.
	{
		_asm int 3;
		// �ִ� ��뷮 �ʰ�.
		return -1;
	}
#endif
	// �ε��� �߱�.
	if( miUsedIndexCount <= 0)		// �߱޵ȳ��� �ϳ��� ����, ���ݰ� ó���϶�.
	{
		// empty���� �ϳ� �����´�.
		miUsedIndexEntry	=	miEmptyIndexEntry;

		miEmptyIndexEntry	=	mpBuffer[miEmptyIndexEntry].iNext;	// ��ũ�� ����, 
		mpBuffer[miEmptyIndexEntry].iPrev	=	-2;

		mpBuffer[miUsedIndexEntry].iNext	=	-2;
		mpBuffer[miUsedIndexEntry].iPrev	=	-2;

		miUsedIndexCount	=	1; 

		return mpBuffer[miUsedIndexEntry].iIndex;
	}

	// ���� �Ҵ����� ���.
	int	iTempNode	=	miEmptyIndexEntry;		

	// ����Ƽ����� �� ��� ��ũ.
	if( mpBuffer[iTempNode].iNext == -2)		// ������ ��忴�ٸ�...
	{
		miEmptyIndexEntry	=	-2;
	}
	else
	{
		miEmptyIndexEntry	=	mpBuffer[iTempNode].iNext;
		mpBuffer[miEmptyIndexEntry].iPrev	=	-2;		// �ʿ� ���� �ڵ�������...
	}

	// �Ҵ��� ��� ����.
	mpBuffer[iTempNode].iNext	=	miUsedIndexEntry;		
	mpBuffer[iTempNode].iPrev	=	-2;
	miUsedIndexEntry		=	iTempNode;

	mpBuffer[mpBuffer[iTempNode].iNext].iPrev		=	iTempNode;

	miUsedIndexCount++;

	return mpBuffer[iTempNode].iIndex;
}

BOOL MIndexManager::ReleaseIndex(int iIndex)
{
	int		i = 0;
	int		iTempNode	=	-2;
#ifdef	_DEBUG
	if( iIndex < 0 || iIndex >= miMaxIndexCount)	// init�� ���� ���ں��� ū���� ���Դ�.
	{
		_asm int 3;
		return FALSE;
	}
	if( miUsedIndexCount <= 0)	// �ε����� ������ �ʰ� �ִµ� ȣ����.
	{
		_asm int 3;
		return	FALSE;
	}
	if( miUsedIndexCount > 0 && miUsedIndexEntry == -2)	// ���� ���� ����.
	{
		_asm int 3;
		return	FALSE;
	}
#endif
	// �ε����� �� �ϳ� ��������
	if( miUsedIndexCount == 1 || mpBuffer[miUsedIndexEntry].iNext == -2)
	{
#ifdef	_DEBUG
		if( mpBuffer[miUsedIndexEntry].iIndex != iIndex)		// Ȥ�ó�...
		{
			_asm int 3;		// ����... 
			return FALSE;
		}
#endif
		// ��带 iEmptyIndexEntry�� ���̰�,
		iTempNode	=	miUsedIndexEntry;
		if( miEmptyIndexEntry != -2)
		{
			mpBuffer[miEmptyIndexEntry].iPrev	=	iTempNode;
		}
		mpBuffer[iTempNode].iNext	=	miEmptyIndexEntry;
		mpBuffer[iTempNode].iPrev	=	-2;
		miEmptyIndexEntry		=	iTempNode;

		miUsedIndexEntry		=	-2;	
		miUsedIndexCount		=	0;
		return	TRUE;
	}

	// �ε����� �ΰ� �̻��϶�.
	iTempNode	=	miUsedIndexEntry;
	for( i = 0; i < miUsedIndexCount; iTempNode = mpBuffer[iTempNode].iNext, i++)
	{
		if( mpBuffer[iTempNode].iIndex == iIndex)	// ã�� ����,
		{
			if( mpBuffer[iTempNode].iPrev == -2)		// ���� ��尡 ���� �� �����,
			{
				mpBuffer[mpBuffer[iTempNode].iNext].iPrev	=	-2;				// iUsedIndexEntry���� ��ũ�� ����,
				miUsedIndexEntry	=	mpBuffer[iTempNode].iNext;

			goto	ReturnIndex;
			}
			if( mpBuffer[iTempNode].iNext == -2)		// ���� ��尡 ���� �� �����,
			{
				// iUsed���� ���� ����,
				mpBuffer[mpBuffer[iTempNode].iPrev].iNext	=	-2;

				// iEmpty�� ���̰�,
				goto	ReturnIndex;
			}

			// ����� �յڷ� ��尡 ������. 
			// iUsed���� ��带 �����.
			mpBuffer[mpBuffer[iTempNode].iPrev].iNext	=	mpBuffer[iTempNode].iNext;
			mpBuffer[mpBuffer[iTempNode].iNext].iPrev	=	mpBuffer[iTempNode].iPrev;
		
			// iEmpty�� �����ֱ�.
			goto	ReturnIndex;
		}
	}

#ifdef	_DEBUG
	// ������� ���� ������ ����ϴ�.
	_asm int 3;		// iIndex������ ����. ���� �߸��� ���.
	return FALSE;
#endif

ReturnIndex:
			if( miEmptyIndexEntry != -2)	// ����Ƽ�� ��尡 ������,
			{
				mpBuffer[miEmptyIndexEntry].iPrev	=	iTempNode;
			}
			mpBuffer[iTempNode].iNext			=	miEmptyIndexEntry;
			mpBuffer[iTempNode].iPrev			=	-2;
			miEmptyIndexEntry				=	iTempNode;

			miUsedIndexCount--;


			return	TRUE;
}
// ���� �������� ������ �ε��� �ѳ��� Release �Ѵ�..
DWORD MIndexManager::ReleaseLastCreated(void)
{
	// ����, ����� ������ ����, (�׳��� ���� �������� ������� �����״�)
	// �׳��� Release �ϰ� ����.
	if( miUsedIndexCount <= 0)	// �ε����� ������ �ʰ� �ִµ� ȣ����.
	{
		_asm int 3;			// ��·�� ������ �޼� ������ �ּ� ó�� �ص� ������� ������...
		return	;
	}

	int		iCurrentNode	=	miUsedIndexEntry;
	for( ; mpBuffer[iCurrentNode].iNext != -2; iCurrentNode = mpBuffer[iCurrentNode].iNext)	;

	// ������ �ε����� ������ �ε�������.
	BOOL	bResult	=	ReleaseIndex( iCurrentNode);
	if( bResult == FALSE)
		_asm int 3;

	return	iCurrentNode;
}

void MIndexManager::Delete()
{
	if( miUsedIndexCount != 0)
	{
		// ���� Ʈ���� ���̳��� ������Ʈ�� �������� �ʾҴ�.
//		_asm int 3;
	}
	if( mpBuffer)	delete[]	mpBuffer;
	mpBuffer			=	0;
	miMaxIndexCount	=	0;
	miUsedIndexCount	=	0;

	delete[]	mpIndexArrayBuff;
	mpIndexArrayBuff	=	0;

}

// �Ҵ�� ��� �ε����� �������Ѵ�.
void MIndexManager::ReleaseAllIndex(void)
{
	// �׳� Delete() �ѹ�, Init �ѹ�. -_-;
	int		iMaxIndexCount	=	miMaxIndexCount;
	Delete();
	Init(iMaxIndexCount);
}

// ���� ��ϵǾ��ִ� �ε��� ���۸� �����Ѵ�. ���ϰ��� ����.
DWORD MIndexManager::GetCurrentIndex( int** ppIndexArray)
{
	if( miUsedIndexCount == 0)
		return	0;
	// used ü���� ���鼭 �ε����� �ϳ��ϳ� ����.
	DWORD	dwAllocatedCount	=	0;
	DWORD	dwCurrentNode	=	miUsedIndexEntry;
	for(   ; mpBuffer[dwCurrentNode].iNext != -2; dwCurrentNode = mpBuffer[dwCurrentNode].iNext)
	{
		mpIndexArrayBuff[dwAllocatedCount]	=	mpBuffer[dwCurrentNode].iIndex;
		dwAllocatedCount++;
	}
	mpIndexArrayBuff[dwAllocatedCount]	=	mpBuffer[dwCurrentNode].iIndex;			// ���� ������ ���. -_-;	  ������ ���� ��� �ѹ��� �����ؾ��ϴµ�..
	dwAllocatedCount++;

	*ppIndexArray	=	mpIndexArrayBuff;

	if( dwAllocatedCount != miUsedIndexCount)
		_asm int 3;

	return	dwAllocatedCount;

	
}
