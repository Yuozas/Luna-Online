#if !defined(AFX_PETFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_)
#define AFX_PETFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PETFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPetFrm �������Դϴ�.

class CPetFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CPetFrm)
protected:
	CPetFrm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual ~CPetFrm();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PETFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_)

