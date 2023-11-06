#pragma once

#include "ImageComboBox.h"
#include "HtmlViewNotice.h"

class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParent = NULL);
	virtual ~CMainDialog();

	// Dialog Data
#ifdef _FOR_SERVER_
	enum { IDD = IDD_MHSERVERPATCH_DIALOG };
#else
	enum { IDD = IDD_MHAUTOPATCH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected: 
	CHtmlViewNotice* mHtmlViewNotice;
	CImageComboBox mResolutionCombo;
	CImageComboBox mInterfaceCombo;

	// Generated message map functions
	//{{AFX_MSG(CMainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	virtual void OnCancel();
	virtual void OnOK();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CxImage mImage;
	CxImage m_backgroundImage;
	CxImage m_beginImage;
	CxImage m_closeImage;
	CxImage m_checkBoxImage;
	CxImage mInterfaceArrowImage;
	CxImage mResolutionArrowImage;
	CxImage	mInterfaceTipImage;
	CxImage mResolutionTipImage;
	CxImage mLoadingTipImage;
	CxImage mPackingTipImage;
	
	struct ImageProgress
	{
		CxImage m_cellImage;
		float m_value;	// 0 ~ 1
	}
	m_progress;

	void PutProgress( double ratio, bool isInstant = false );
	void PutBackground();
	void PutTitle();
	void PutTip();

private:
	// 091214 LUJ, ��ŷ�� ������� �̷����� ����� ���� ��ǻ���� ��� ������ �������� ���ϴ� ��찡 �ִ�.
	//			��ŷ���� ���� �̹� �����־� �߻��Ѵ�. �̸� ���ϱ� ���� ��ó�� ����� �� ������ ���ϵ���
	//			���쵵�� �ߴ�
	void DeleteOldFile();
	void UpdateResolutionCombo();
	void UpdateInterfaceCombo();
	bool mIsReady;

public:	
	// 080306 LUJ, ESCŰ�� ���� ���α׷��� �����ϰų�, ����Ű�� ���� ��ġ �߿� ������ ������ �� �־� �̸� ���´�
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeInterface();
	afx_msg void OnCbnSelchangeScreensize();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// 080515 LUJ, �ٿ�ε� �� ���� ó��
public:
	// 080515 LUJ, ���� ������ üũ�Ѵ�
	static void CheckVersionToMemory( CZipMemFile& );
	// 080515 LUJ, ���� ������ üũ�Ѵ�
	static void CheckVersionToDisk( const TCHAR* );
	// 080812 LUJ, ��ġ ������ �޾Ҵ��� üũ�Ѵ�
	static void CheckPatchToMemory( CZipMemFile& );
	// 080812 LUJ, ��ġ ������ �޾Ҵ��� üũ�Ѵ�
	static void CheckPatchToDisk( const TCHAR* );
	// 080515 LUJ, �ٿ�ε���� ��ġ ������ ������ �����Ѵ�
	static void ExtractToMemory( CZipMemFile& );
	// 080515 LUJ, �ٿ�ε���� ��ġ ������ ������ �����Ѵ�
	static void ExtractToDisk( const TCHAR* );
	// 090819 ONS ���÷�óUI�� ���ۼӵ��� �����ð� ���.
	// 080515 LUJ, ����� ǥ���Ѵ�
	static void Pursuit( double percent, double remaintime, double down_speed );
	// 080515 LUJ, ������ ��ŷ�Ѵ�
	static UINT Pack( LPVOID );	
	// 080814 LUJ, ��ó�� �غ� ���·� �����
	static void Ready();
private:
	// 081202 LUJ, ��ġ ������ üũ�Ѵ�
	static void CheckVersion( CString serverVersion, CString clientVersion );
	// 090720 ShinJS --- ���������� �����ϰ� �ٿ�ε��� ���������� �����Ѵ�
	static void SaveVersion();
private:
	CString mPatchVersion;

	// 090819 ONS ���÷��� ������ �ڵ�
	HWND m_hTarget;
	// 090819 ONS ���÷�ó�� UI�� �����͸� �����ϴ� �Լ�
	void SendMessageToPurple(int nType, LPCTSTR lpszMsg, DWORD dwValue);
};