// PackExtract.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CPackExtractApp:
// �� Ŭ������ ������ ���ؼ��� PackExtract.cpp�� �����Ͻʽÿ�.
//

class CPackExtractApp : public CWinApp
{
public:
	CPackExtractApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CPackExtractApp theApp;
