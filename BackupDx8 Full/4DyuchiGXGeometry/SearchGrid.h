// SearchGrid.h: interface for the CSearchGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHGRID_H__E328693B_216B_458A_A51E_80087AEC7DE9__INCLUDED_)
#define AFX_SEARCHGRID_H__E328693B_216B_458A_A51E_80087AEC7DE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DYUCHIGRX_COMMON/typedef.h"

#include "SearchGridTree.h"

class CSearchGrid  
{
		MAABB			m_aabb;				// �� �׸����� �ε���.
		DWORD			m_dwTriCount;		// �� �׸��尡 ���� �ﰢ�� ����.
		DWORD*			m_pdwTriIndex;		// m_Tri.ppBuff �� �ε�����.

		CSearchGridTree*		m_pTree;
public:
	CSearchGrid();
	virtual ~CSearchGrid();

};

#endif // !defined(AFX_SEARCHGRID_H__E328693B_216B_458A_A51E_80087AEC7DE9__INCLUDED_)
