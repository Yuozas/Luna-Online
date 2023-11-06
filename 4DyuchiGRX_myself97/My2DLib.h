#ifndef	_My2DLib_h_
#define _My2DLib_h_

#include <windows.h>

// �ؽ����� ũ�⸦ ���δ�.
// pSrc�� ���� ����. pDest�� ����� ������. dwWidth�� ���� �ؽ����� ���α���, dwHeight�� ���� �ؽ����� ����,
//	dwScale�� ��� ������(2�� 1/2�� �پ���.)
// dwWidth�� dwHeight�� ������ 2�� n���ΰ� �����Ѵ�. �ƴϸ� ��.
// ��Ʈ ������ 565�� ����.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);

// 4444 ����.
void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);
// 8888 ���� 
void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);


#endif
