#pragma once


#include "PictureEx.h"
#include "afxwin.h"


// CWaitDialog ��ȭ �����Դϴ�.

class CWaitDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWaitDialog();

	virtual void OnFinalRelease();
	virtual LRESULT SendMessage( UINT message, WPARAM = 0, LPARAM = 0 );

	void SetWaiting( bool );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WAITDIALOG };

protected:
	int mSecond;
	int mMaxSecond;

	bool	mIsWaiting;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	CPictureEx m_Picture;
	afx_msg void OnTimer(UINT nIDEvent);
	CStatic mTimeStatic;
	CButton mCancelButton;
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
