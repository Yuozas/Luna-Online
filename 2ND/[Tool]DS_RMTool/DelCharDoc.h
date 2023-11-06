#if !defined(AFX_DELCHARDOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)
#define AFX_DELCHARDOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaDoc.h : header file
//

#include "basedoc.h"
#include "IndexGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CDelCharDoc 문서입니다.

class CDelCharDoc : public CBaseDoc
{
protected:
	CDelCharDoc(); 
	DECLARE_DYNCREATE(CDelCharDoc)
	
// Attributes
public:
	void					SetDelCharList(TMSG_PETBASEINFO* info);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Operations
public:
	virtual void			OpenData();

public:
	virtual ~CDelCharDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	void NetworkMsgParse(BYTE Protocol, void* pMsg);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELCHARDOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)