// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8BD744F7_F985_4828_ADC7_68CF524DCD86__INCLUDED_)
#define AFX_STDAFX_H__8BD744F7_F985_4828_ADC7_68CF524DCD86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#include <afxmt.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#endif // _AFX_NO_AFXCMN_SUPPORT

#define STL_USING_ALL
#include "STL.h"
#include <winsock2.h>
#include <sys/stat.h>
#include <process.h>
#include <tchar.h>

#include "FtpProtocolCode.h"
#include "FtpClient.h"

// CxImage library: http://www.xdp.it/cximage.htm
//
// MFC에서 쓰려면 cximage599c_full_unicode 버전을 다운받아 CxImageMFCDLL 프로젝트를 컴파일하여 쓴다.
#include "CxImage/ximage.h"
#include "../[Lib]ZipArchive/ZipArchive.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8BD744F7_F985_4828_ADC7_68CF524DCD86__INCLUDED_)
