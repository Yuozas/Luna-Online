// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ������� ���� ������ �ʴ� ������ ���ܽ�ŵ�ϴ�.
#endif

// �Ʒ� ������ �÷������� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows 95 �� Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0400		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINNT 0x0500		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINDOWS 0x0410 // Windows Me ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_IE			// IE 4.0 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_IE 0x0400	// IE 5.0 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#include <afxmt.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#endif // _AFX_NO_AFXCMN_SUPPORT

#include <process.h>

// CxImage library: http://www.xdp.it/cximage.htm
//
// MFC���� ������ cximage599c_full_unicode ������ �ٿ�޾� CxImageMFCDLL ������Ʈ�� �������Ͽ� ����.
#include "CxImage/ximage.h"
// 080515 LUJ, �ؽ�Ʈ ������ ANSI/�����ڵ忡 ������� �о����
#include "StdioFileEx.h"
#include <vector>
#include <list>

#ifdef _DEBUG
#include <crtdbg.h>
#include <conio.h>
#endif

class CLog
{
public:
	static void Put( const TCHAR*, ... );
	static void SetEnable( bool );

protected:
	CLog();
	~CLog();

	static bool mIsEnable;
};

void DrawText(CxImage&, const CPoint&, LPCTSTR, const RGBQUAD&, long size, LPCTSTR font, long weight = 400, long outline = 0, const RGBQUAD& outlineColor = RGBQUAD());
// 090720 ShinJS --- ���� AutoPatch �� Log, Launcher �� MessageBox ����
void MsgBoxLog( LPCTSTR msg, LPCTSTR caption, HWND hwnd = NULL );

#define WM_MOUSEMOVE_COMBO (WM_USER + 1000)

#define WM_DOWNLOAD_END	   (WM_USER + 1001)
#define WM_PACKING_END	   (WM_USER + 1002)
/*
080702 LUJ, 2.1.0:	�ٿ�ε� ���� �� �ڵ� ��� ���� �޽����� �ܼ� â�� ǥ���ϵ��� ��
					�߸��� ���� ������ �ٿ���� ��� ��ġ�� ������� �ʵ��� ��
080919 LUJ, 2.1.1:	�븸 ���� �߰�
081201 LUJ, 2.2.0:	Ŭ���̾�Ʈ�� ��û�� ��ġ ������ ���� ���, ��ü ��ġ�� �õ��ϵ��� ��
081202 LUU, 2.3.0:	�߸� �޾��� ������ �����ϵ��� ��
					��ġ ������ �޸𸮿� �ٿ�޾� ó���ϴ� �ɼ� �߰�
091214 LUJ, 2.4.0:	�б� ���� ������ ������ �� �ֵ��� ��
091221 LUJ, 2.5.0:	���� ī�� ���� �ػ󵵸� �ν��ϵ��� ��
*/
#define VERSION _T( "2.5.0" )