//////////////////////////////////////////////////////////////////////////////
/// \file MiniDumper.h
/// \author excel96
/// \date 2003.11.18
//////////////////////////////////////////////////////////////////////////////

#ifndef __MINIDUMPER_H__
#define __MINIDUMPER_H__

#ifndef __NONCOPYABLE_H__
#include "Noncopyable.h"
#endif

//////////////////////////////////////////////////////////////////////////////
/// \class MiniDumper
/// \brief �̴� ������ �����ϱ� ���� Ŭ����. 
///
/// Ư���� �ϴ� ���� ����, ���� ������ �Լ��� ������ �ִ� ���� �����̽� ���� 
/// ���� �ۿ��� �ϴ� ���� ����. ������ �̴� ������ �̷������ ���� 
/// TopLevelFilter �Լ���.
//////////////////////////////////////////////////////////////////////////////

class MiniDumper : private Noncopyable
{
public:
	/// \brief ������ �������� ����
	enum DumpLevel
	{
		DUMP_LEVEL_0, ///< MiniDumpNormal�� ���
		DUMP_LEVEL_1, ///< MiniDumpWithDataSegs�� ���
		DUMP_LEVEL_2  ///< MiniDumpWithFullMemory�� ���
	};


private:
	/// ���� ����.
	static DumpLevel s_DumpLevel;            

	/// ���� ���� �̸����ٰ� ���� ������ ������ ��¥�� ����ִ°��� ����.
	static bool s_bAddTimeStamp;

	/// ���� ���� �̸� ���ڿ�.
	static TCHAR s_szAppName[_MAX_PATH];

	/// ������ �߻��� ����
	static TCHAR s_szFaultReason[2048];


public:
	/// \brief ������
	MiniDumper(DumpLevel DL, bool bAddTimestamp=true);

	/// \brief �Ҹ���
	~MiniDumper();


private:
	/// \brief ���ܿ� ���� ������ �޾Ƽ�, �̴� ���� ������ �����Ѵ�. 
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExPtrs);

	/// \brief ������ ���ο� ���� ���ڿ��� ��ȯ�Ѵ�.
	static LPCTSTR GetFaultReason(struct _EXCEPTION_POINTERS* pExPtrs);
};

#endif //__MINIDUMPER_H__