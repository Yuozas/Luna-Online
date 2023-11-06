/*********************************************************************

	 ����		: SHMath.h
	 �ۼ���		: hseos
	 �ۼ���		: 2005/07/14

	 ���ϼ���	: ���� ���õ� Ŭ������ ���

 *********************************************************************/

#pragma once

#define SHMath_SECOND(x)		(x##000)
#define SHMath_MINUTE(x)		(SHMath_SECOND(60)*x)

#define SHMath_BIN8_00000001	0x0001
#define SHMath_BIN8_00000010	0x0002
#define SHMath_BIN8_00000100	0x0004
#define SHMath_BIN8_00001000	0x0008
#define SHMath_BIN8_00001000	0x0008
#define SHMath_BIN8_00010000	0x0010
#define SHMath_BIN8_00100000	0x0020
#define SHMath_BIN8_01000000	0x0040
#define SHMath_BIN8_10000000	0x0080

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
class CSHMath
{
private:

public:
	//-------------------------------------------------------------------------------------
	CSHMath();
	~CSHMath();

	//-------------------------------------------------------------------------------------
	// nStart ~ nEnd ������ ���� nNum���� �߻����� pBuf�� ����. bDuplicate �� �ߺ� ���� ����
	//
	static BOOL		GetRandomNum(int nNum, int nStart, int nEnd, int bDuplicate, int* pBuf);
	// ���� ���
	static int		GetRandomNum(int nStart, int nEnd)	
					{ 
						if (nStart > nEnd) 
						{
							int nTmp = nStart;
							nStart = nEnd;
							nEnd = nTmp;
						}
						else if (nStart == nEnd) return nStart;
						return (rand()%(nEnd-nStart+1))+nStart; 
					}

	//-------------------------------------------------------------------------------------
	// ��Ʈ üũ
	static BOOL		BitAndOp(int nTarget, int nBit)		{ return (nTarget & (1<<nBit)) ? TRUE : FALSE; }		
};