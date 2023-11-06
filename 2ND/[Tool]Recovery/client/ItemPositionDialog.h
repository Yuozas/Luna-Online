/*
080303	LUJ, �������� ������ ��ġ�� �����ϴ� â
*/
#pragma once


// CItemPositionDialog ��ȭ �����Դϴ�.

class CItemPositionDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemPositionDialog)

public:
	CItemPositionDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemPositionDialog();

	virtual void OnFinalRelease();

	// 080303 LUJ, ������ ���� ��ġ�� ��ȯ
	Item::Area GetAreaType() const;

	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMPOSITIONDIALOG };

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


private:
	Item::Area	mAreaType;
	
	CButton mInventoryRadioButton;
	CButton mPrivateStorageRadioButton;
	CButton mCashStorageRadioButton;

	bool mIsInventoryPosition;
	bool mIsPrivateStoragePosition;
	bool mIsCashStoragePosition;

	
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual INT_PTR DoModal( POSTYPE InventoryPosition, POSTYPE privateStoragePosition, POSTYPE cashStoragePosition );
	virtual BOOL OnInitDialog();
};
