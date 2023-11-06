#pragma once
#include "./Interface/cDialog.h"

class cTextArea;

class CFamilyNoticeDlg :
	public cDialog
{
	cTextArea * m_pNoticeText;

public:
	CFamilyNoticeDlg(void);
	~CFamilyNoticeDlg(void);

	void Linking();
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	void SetActive(BOOL val);
};
