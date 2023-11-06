/*********************************************************************

	 ����		: SHMain.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���� Ŭ������ ���

 *********************************************************************/

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHMain
{
public:
	typedef enum											// �������� ����� ��� ���ϰ� ����
	{
		RESULT_OK = 0,										// ����
		RESULT_OK_02,
		RESULT_OK_03,
		RESULT_OK_04,
		RESULT_OK_05,
		RESULT_OK_06,

		RESULT_FAIL_INVALID,								// ����(�ý��� ������ ����� ����)

		RESULT_FAIL_01,										// ����01
		RESULT_FAIL_02,										// ����02
		RESULT_FAIL_03,										// ����03
		RESULT_FAIL_04,										// ����04
		RESULT_FAIL_05,										// ����05
		RESULT_FAIL_06,										// ����06
		RESULT_FAIL_07,										// ����07
		RESULT_FAIL_08,										// ����08
	} RESULT;

private:
	
public:
	CSHMain();
	~CSHMain();

	//					�ʱ�ȭ
	static VOID			Init();
	//					����
	static VOID			MainLoop();
	//					�׸���
	static VOID			Render();
};