#include "My2DLib.h"
//#include "../4dyuchigrx_common/typedef.h"

//	������ �ٿ�.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// ������ ������ 16��Ʈ.
	
	// �۾��� �̹����� �� ��Ʈ�� ���� �̹����� dwScale*dwScale��ŭ�� ��Ʈ�� ��� �����̴�.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// �ٿ� �����ϵ� �̹����� �������� ������ ����.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale�� 2�� ��� �����ǥ x,y�� ���� ������ǥ x,y; x+1,y; x,y+1; x+1,y+1�� �� ����� ��հ��̴�.
			// ���� �ؽ��Ŀ��� dwScale*dwScale��ŭ ���ø�.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
//					WORD	Component = pTex[(x*dwScale) + u + ((y+v)*dwScale*(dwWidth))];
//					WORD	Component = pTex[(x*dwScale) + u + ((y*dwScale+v)*(dwWidth))];
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR = ComR + ((Component&0xf800) >> 11);		// ��Ʈ �迭�� �ٲ�� ���� ����.
					ComG = ComG + ((Component&0x07e0) >> 5);
					ComB = ComB + ((Component&0x001f) );
				}
			}
			ComR = (ComR /(dwScale*dwScale)) << 11;		// dwScale*dwScale�� ������ 11��Ʈ ��������.
			ComG = (ComG /(dwScale*dwScale)) << 5;		// dwScale*dwScale�� ������ 5��Ʈ ��������.
			ComB = ComB /(dwScale*dwScale);				// dwScale*dwScale�� �����⸸..
			pDest[x+y*dwImageWidth] = unsigned short (ComR + ComG + ComB);
		}
	}

}

void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// ������ ������ 16��Ʈ.
	
	// �۾��� �̹����� �� ��Ʈ�� ���� �̹����� dwScale*dwScale��ŭ�� ��Ʈ�� ��� �����̴�.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// �ٿ� �����ϵ� �̹����� �������� ������ ����.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale�� 2�� ��� �����ǥ x,y�� ���� ������ǥ x,y; x+1,y; x,y+1; x+1,y+1�� �� ����� ��հ��̴�.
			// ���� �ؽ��Ŀ��� dwScale*dwScale��ŭ ���ø�.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component&0xf000) >> 12);
					ComG	=	ComG	+	((Component&0x0f00) >> 8);
					ComB	=	ComB	+	((Component&0x00f0) >> 4);
					ComA	=	ComA	+	((Component&0x000f));
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 12;
			ComG	=	(ComG / (dwScale*dwScale)) << 8;
			ComB	=	(ComB / (dwScale*dwScale)) << 4;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	unsigned short (ComR + ComG + ComB + ComA);
		}
	}

}

void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// ������ ������ 16��Ʈ.
	
	// �۾��� �̹����� �� ��Ʈ�� ���� �̹����� dwScale*dwScale��ŭ�� ��Ʈ�� ��� �����̴�.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// �ٿ� �����ϵ� �̹����� �������� ������ ����.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale�� 2�� ��� �����ǥ x,y�� ���� ������ǥ x,y; x+1,y; x,y+1; x+1,y+1�� �� ����� ��հ��̴�.
			// ���� �ؽ��Ŀ��� dwScale*dwScale��ŭ ���ø�.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					DWORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component & 0xff000000)>>24);
					ComG	=	ComG	+	((Component & 0x00ff0000)>>16);
					ComB	=	ComB	+	((Component & 0x0000ff00)>>8);
					ComA	=	ComA	+	(Component & 0x000000ff);
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 24;
			ComG	=	(ComG / (dwScale*dwScale)) << 16;
			ComB	=	(ComB / (dwScale*dwScale)) << 8;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	(ComR | ComG | ComB | ComA);
		}
	}

}

