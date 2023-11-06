/*********************************************************************

	 ����		: SHMath.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2005/07/14

	 ���ϼ���	: ���� ���õ� ó���� �ϴ� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "SHMath.h"

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHMath Method																												  ������
//
CSHMath::CSHMath()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHMath Method																											  	  �ı���
//
CSHMath::~CSHMath()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
//-- GetRandomNum Method						   nStart ~ nEnd ������ ���� nNum���� �߻����� pBuf�� ����. bDuplicate �� �ߺ� ���� ����
// 
BOOL CSHMath::GetRandomNum(int nNum, int nStart, int nEnd, int bDuplicate, int* pBuf)
{
//	srand((unsigned int)time(NULL));

	BOOL* bAlready = new BOOL[nEnd-nStart+1];
	int nCnt = 0;

	ZeroMemory(bAlready, sizeof(bAlready[0])*(nEnd-nStart+1));

	DWORD nStartTime = GetTickCount();

	if (bDuplicate)
	{
		for(int i=0; i<nNum; i++)
		{
			pBuf[i] = (rand()%(nEnd-nStart+1))+nStart;
		}
	}
	else
	{
		while(nCnt < nNum)
		{
			int nValue = (rand()%(nEnd-nStart+1));

			if (bAlready[nValue] == FALSE)
			{
				pBuf[nCnt] = nValue+nStart;
				bAlready[nValue] = TRUE;
				nCnt++;
			}

			DWORD nCurTime = GetTickCount();
			if (nCurTime - nStartTime > 2000)
			{
				delete[] bAlready;
				return FALSE;
			}
		}
	}

	delete[] bAlready;

	return TRUE;
}